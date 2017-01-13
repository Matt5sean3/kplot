/*	$Id$ */
/*
 * Copyright (c) 2015 Kristaps Dzonsons <kristaps@bsd.lv>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include "compat.h"

#include <assert.h>
#include <cairo.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __linux__
#include <bsd/stdlib.h>
#endif

#include "kplot.h"

static void
fill(size_t pos, struct kpair *val, void *dat)
{

	val->x = pos;
#if defined(__unix__) || defined(__APPLE__) || defined(LIBBSD_STDLIB_H)
	val->y = arc4random() / (double)UINT32_MAX;
#else
  /* Just use a non-BSD random generator */
  val->y = (double)rand() / (double)RAND_MAX;
#endif
}

int
main(int argc, char *argv[])
{
	struct kdata	*d;
	struct kplot	*p;
	cairo_surface_t	*surf;
	cairo_t		*cr;
	cairo_status_t	 st;
	int		 rc, c;

	rc = EXIT_FAILURE;

	d = kdata_array_alloc(NULL, 20);
	assert(NULL != d);
	c = kdata_array_fill(d, NULL, fill);
	assert(c);

	if (NULL == (p = kplot_alloc(NULL))) {
		perror(NULL);
		goto out;
	}

	if ( ! kplot_attach_data(p, d, KPLOT_LINESPOINTS, NULL)) {
		perror(NULL);
		goto out;
	} 
	
	if ( ! kplot_attach_smooth(p, d, KPLOT_LINESPOINTS, 
			NULL, KSMOOTH_MOVAVG, NULL)) {
		perror(NULL);
		goto out;
	}

	kdata_destroy(d);
	d = NULL;

	surf = cairo_image_surface_create
		(CAIRO_FORMAT_ARGB32, 600, 400);

	st = cairo_surface_status(surf);
	if (CAIRO_STATUS_SUCCESS != st) {
		fprintf(stderr, "%s", cairo_status_to_string(st));
		cairo_surface_destroy(surf);
		kplot_free(p);
		return(EXIT_FAILURE);
	}

	cr = cairo_create(surf);
	cairo_surface_destroy(surf);

	st = cairo_status(cr);
	if (CAIRO_STATUS_SUCCESS != st) {
		fprintf(stderr, "%s", cairo_status_to_string(st));
		cairo_destroy(cr);
		kplot_free(p);
		return(EXIT_FAILURE);

	}

	cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); 
	cairo_rectangle(cr, 0.0, 0.0, 600.0, 400.0);
	cairo_fill(cr);
	kplot_draw(p, 600.0, 400.0, cr);

	st = cairo_surface_write_to_png
		(cairo_get_target(cr), "example7.png");

	if (CAIRO_STATUS_SUCCESS != st) {
		fprintf(stderr, "%s", cairo_status_to_string(st));
		cairo_destroy(cr);
		kplot_free(p);
		return(EXIT_FAILURE);
	}

	cairo_destroy(cr);
	rc = EXIT_SUCCESS;
out:
	kplot_free(p);
	kdata_destroy(d);
#ifdef	__APPLE__
	cairo_debug_reset_static_data();
#endif
	return(rc);
}
