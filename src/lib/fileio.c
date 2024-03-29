/*
  ----------------------------------------------------------------
	Speech Signal Processing Toolkit (SPTK): version 3.0
			 SPTK Working Group

		   Department of Computer Science
		   Nagoya Institute of Technology
				and
    Interdisciplinary Graduate School of Science and Engineering
		   Tokyo Institute of Technology
		      Copyright (c) 1984-2000
			All Rights Reserved.

  Permission is hereby granted, free of charge, to use and
  distribute this software and its documentation without
  restriction, including without limitation the rights to use,
  copy, modify, merge, publish, distribute, sublicense, and/or
  sell copies of this work, and to permit persons to whom this
  work is furnished to do so, subject to the following conditions:

    1. The code must retain the above copyright notice, this list
       of conditions and the following disclaimer.

    2. Any modifications must be clearly marked as such.

  NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF TECHNOLOGY,
  SPTK WORKING GROUP, AND THE CONTRIBUTORS TO THIS WORK DISCLAIM
  ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
  SHALL NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF
  TECHNOLOGY, SPTK WORKING GROUP, NOR THE CONTRIBUTORS BE LIABLE
  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
  DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
  ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
  PERFORMANCE OF THIS SOFTWARE.
 ----------------------------------------------------------------
*/

/*
	freadf  : read float type datas and convert to double type.

	fwritef : convert double type datas to float type and write.
								*/


#include <stdio.h>
#include <SPTK.h>
static	float	*f;
static	int	items;


#ifndef DOUBLE

int	fwritef(ptr, size, nitems, fp)
double	*ptr;
unsigned int	size;
int	nitems;
FILE	*fp;
{
	int	i;
	if (items < nitems) {
		if (f != NULL)
			free(f);
		items = nitems;
		f = fgetmem(items);
	}
	for (i=0; i<nitems; i++)
		f[i] = ptr[i];
	return fwrite(f, sizeof(float), nitems, fp);
}

int	freadf(ptr, size, nitems, fp)
double	*ptr;
int	size;
int	nitems;
FILE	*fp;
{
	int	i, n;
	if (items < nitems) {
		if (f != NULL)
			free(f);
		items = nitems;
		f = fgetmem(items);
	}
	n = fread(f, sizeof(float), nitems, fp);
	for (i=0; i<n; i++)
		ptr[i] = f[i];
	return n;
}

#endif	/* DOUBLE */

