/*
 * MACDRV Cocoa interface declarations
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

/* This header serves as a C interface between the Wine parts of MACDRV
 * and the Objective-C parts.  It should restrict itself to C and the C-based
 * Mac APIs, avoiding both Wine and Objective-C/Cocoa features, so that it can
 * be included by both sides without contaminating either.
 */

#ifndef __WINE_MACDRV_COCOA_H
#define __WINE_MACDRV_COCOA_H


#define GetCurrentProcess MacGetCurrentProcess
#define GetCurrentThread MacGetCurrentThread
#define LoadResource MacLoadResource
#define AnimatePalette MacAnimatePalette
#define EqualRgn MacEqualRgn
#define FillRgn MacFillRgn
#define FrameRgn MacFrameRgn
#define GetPixel MacGetPixel
#define InvertRgn MacInvertRgn
#define LineTo MacLineTo
#define OffsetRgn MacOffsetRgn
#define PaintRgn MacPaintRgn
#define Polygon MacPolygon
#define ResizePalette MacResizePalette
#define SetRectRgn MacSetRectRgn
#define EqualRect MacEqualRect
#define FillRect MacFillRect
#define FrameRect MacFrameRect
#define GetCursor MacGetCursor
#define InvertRect MacInvertRect
#define OffsetRect MacOffsetRect
#define PtInRect MacPtInRect
#define SetCursor MacSetCursor
#define SetRect MacSetRect
#define ShowCursor MacShowCursor
#define UnionRect MacUnionRect

#include <ApplicationServices/ApplicationServices.h>

#undef GetCurrentProcess
#undef GetCurrentThread
#undef LoadResource
#undef AnimatePalette
#undef EqualRgn
#undef FillRgn
#undef FrameRgn
#undef GetPixel
#undef InvertRgn
#undef LineTo
#undef OffsetRgn
#undef PaintRgn
#undef Polygon
#undef ResizePalette
#undef SetRectRgn
#undef EqualRect
#undef FillRect
#undef FrameRect
#undef GetCursor
#undef InvertRect
#undef OffsetRect
#undef PtInRect
#undef SetCursor
#undef SetRect
#undef ShowCursor
#undef UnionRect
#undef DPRINTF


#ifndef DECLSPEC_HIDDEN
# if defined(__MSC_VER) || defined(__MINGW32__) || defined(__CYGWIN__)
#  define DECLSPEC_HIDDEN
# elif defined(__GNUC__) && ((__GNUC__ > 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 3)))
#  define DECLSPEC_HIDDEN __attribute__((visibility ("hidden")))
# else
#  define DECLSPEC_HIDDEN
# endif
#endif


typedef struct macdrv_opaque_window* macdrv_window;

struct macdrv_display {
    CGDirectDisplayID displayID;
    CGRect frame;
    CGRect work_frame;
};


/* main */
extern int macdrv_start_cocoa_app(void) DECLSPEC_HIDDEN;


/* display */
extern int macdrv_get_displays(struct macdrv_display** displays, int* count) DECLSPEC_HIDDEN;
extern void macdrv_free_displays(struct macdrv_display* displays) DECLSPEC_HIDDEN;


/* window */
struct macdrv_window_features {
    unsigned int    title_bar:1;
    unsigned int    close_button:1;
    unsigned int    minimize_button:1;
    unsigned int    resizable:1;
    unsigned int    utility:1;
    unsigned int    shadow:1;
};

struct macdrv_window_state {
    unsigned int    disabled:1;
    unsigned int    no_activate:1;
    unsigned int    floating:1;
    unsigned int    excluded_by_expose:1;
    unsigned int    excluded_by_cycle:1;
};

extern macdrv_window macdrv_create_cocoa_window(const struct macdrv_window_features* wf,
        CGRect frame) DECLSPEC_HIDDEN;
extern void macdrv_destroy_cocoa_window(macdrv_window w) DECLSPEC_HIDDEN;
extern void macdrv_set_cocoa_window_features(macdrv_window w,
        const struct macdrv_window_features* wf) DECLSPEC_HIDDEN;
extern void macdrv_set_cocoa_window_state(macdrv_window w,
        const struct macdrv_window_state* state) DECLSPEC_HIDDEN;
extern void macdrv_set_cocoa_window_title(macdrv_window w, const UniChar* title,
        size_t length) DECLSPEC_HIDDEN;
extern int macdrv_order_cocoa_window(macdrv_window w, macdrv_window prev,
        macdrv_window next) DECLSPEC_HIDDEN;
extern void macdrv_hide_cocoa_window(macdrv_window w) DECLSPEC_HIDDEN;
extern int macdrv_set_cocoa_window_frame(macdrv_window w, const CGRect* new_frame) DECLSPEC_HIDDEN;
extern void macdrv_set_cocoa_parent_window(macdrv_window w, macdrv_window parent) DECLSPEC_HIDDEN;

#endif  /* __WINE_MACDRV_COCOA_H */
