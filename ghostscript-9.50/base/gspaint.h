/* Copyright (C) 2001-2019 Artifex Software, Inc.
   All Rights Reserved.

   This software is provided AS-IS with no warranty, either express or
   implied.

   This software is distributed under license and may not be copied,
   modified or distributed except as expressly authorized under the terms
   of the license contained in the file LICENSE in this distribution.

   Refer to licensing information at http://www.artifex.com or contact
   Artifex Software, Inc.,  1305 Grant Avenue - Suite 200, Novato,
   CA 94945, U.S.A., +1(415)492-9861, for further information.
*/


/* Erase/fill/stroke procedures */
/* Requires gsstate.h */

#ifndef gspaint_INCLUDED
#  define gspaint_INCLUDED

#include "stdpre.h"
#include "gsgstate.h"

/* Painting */
int gs_erasepage(gs_gstate *),
    gs_fillpage(gs_gstate *),
    gs_fill(gs_gstate *),
    gs_eofill(gs_gstate *),
    gs_stroke(gs_gstate *);

/* Image tracing */
int gs_imagepath(gs_gstate *, int, int, const byte *);

#endif /* gspaint_INCLUDED */
