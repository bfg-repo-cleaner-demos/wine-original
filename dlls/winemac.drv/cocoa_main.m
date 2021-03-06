/*
 * MACDRV Cocoa initialization code
 *
 * Copyright 2011, 2012, 2013 Ken Thomases for CodeWeavers Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#import <AppKit/AppKit.h>

#include "macdrv_cocoa.h"
#import "cocoa_app.h"


/* Condition values for an NSConditionLock. Used to signal between run_cocoa_app
   and macdrv_start_cocoa_app so the latter knows when the former is running
   the application event loop. */
enum {
    COCOA_APP_NOT_RUNNING,
    COCOA_APP_RUNNING,
};


/***********************************************************************
 *              run_cocoa_app
 *
 * Transforms the main thread from merely idling in its run loop to
 * being a Cocoa application running its event loop.
 *
 * This will be the perform callback of a custom run loop source that
 * will be scheduled in the main thread's run loop from a secondary
 * thread by macdrv_start_cocoa_app.  This function communicates that
 * it has successfully started the application by changing the condition
 * of a shared NSConditionLock, passed in via the info parameter.
 *
 * This function never returns.  It's the new permanent home of the
 * main thread.
 */
static void run_cocoa_app(void* info)
{
    NSConditionLock* lock = info;

    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

    [WineApplication sharedApplication];
    [NSApp setDelegate:(WineApplication*)NSApp];

    /* Retain the lock while we're using it, so macdrv_start_cocoa_app()
       doesn't deallocate it in the middle of us unlocking it. */
    [lock retain];
    [lock lock];
    [lock unlockWithCondition:COCOA_APP_RUNNING];
    [lock release];

    [pool release];

    /* Never returns */
    [NSApp run];
}


/***********************************************************************
 *              macdrv_start_cocoa_app
 *
 * Tells the main thread to transform itself into a Cocoa application.
 *
 * Returns 0 on success, non-zero on failure.
 */
int macdrv_start_cocoa_app(void)
{
    int ret = -1;
    CFRunLoopSourceRef source;
    NSConditionLock* lock;
    NSDate* timeLimit;
    CFRunLoopSourceContext source_context = { 0 };

    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

    /* Make sure Cocoa is in multi-threading mode by detaching a
       do-nothing thread. */
    [NSThread detachNewThreadSelector:@selector(self)
                             toTarget:[NSThread class]
                           withObject:nil];

    lock = [[NSConditionLock alloc] initWithCondition:COCOA_APP_NOT_RUNNING];
    timeLimit = [NSDate dateWithTimeIntervalSinceNow:5];

    source_context.info = lock;
    source_context.perform = run_cocoa_app;
    source = CFRunLoopSourceCreate(NULL, 0, &source_context);

    if (source && lock && timeLimit)
    {
        CFRunLoopAddSource(CFRunLoopGetMain(), source, kCFRunLoopCommonModes);
        CFRunLoopSourceSignal(source);
        CFRunLoopWakeUp(CFRunLoopGetMain());

        if ([lock lockWhenCondition:COCOA_APP_RUNNING beforeDate:timeLimit])
        {
            [lock unlock];
            ret = 0;
        }
    }

    if (source)
        CFRelease(source);
    [lock release];
    [pool release];
    return ret;
}
