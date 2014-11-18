#include <assert.h>
#include <cairo.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "kplot.h"
#include "compat.h"
#include "extern.h"

struct kdata *
kdata_bucket_alloc(size_t rmin, size_t rmax)
{
	struct kdata	*d;
	size_t		 i;

	if (NULL == (d = calloc(1, sizeof(struct kdata))))
		return(NULL);

	d->refs = 1;
	d->pairsz = d->pairbufsz = rmax - rmin;
	d->pairs = calloc(d->pairsz, sizeof(struct kpair));
	if (NULL == d->pairs) {
		free(d);
		return(NULL);
	}

	for (i = 0; i < d->pairsz; i++) 
		d->pairs[i].x = rmin + i;

	d->type = KDATA_BUCKET;
	d->d.bucket.rmin = rmin;
	d->d.bucket.rmax = rmax;
	return(d);
}

int
kdata_bucket_increment(struct kdata *d, size_t v)
{

	assert(KDATA_BUCKET == d->type);

	if (v < d->d.bucket.rmin)
		return(0);
	else if (v >= d->d.bucket.rmax)
		return(0);

	d->pairs[v - d->d.bucket.rmin].y++;
	return(1);
}