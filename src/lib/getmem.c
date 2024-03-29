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

/********************************************************************
    $Id: getmem.c,v 1.2 2002/12/25 05:34:41 sako Exp $

    Memory Allocation Fanctions

	short	*sgetmem(leng)
	long	*lgetmem(leng)
	double	*dgetmem(leng)
	float	*fgetmem(leng)
	real	*rgetmem(leng)
	float	**fgetmem(leng)

	int leng : data length

	char	*getmem(leng, type)

	int     leng : data length
	unsignd type : size of data type

**********************************************************************/

#include	<stdio.h>

#ifdef	DOUBLE
#define	real	double
#else
#define	real	float
#endif


short *sgetmem(leng)
int leng;
{
    char *getmem();

    return ( (short *)getmem(leng, sizeof(short)) );
}


long *lgetmem(leng)
int leng;
{
    char *getmem();

    return ( (long *)getmem(leng, sizeof(long)) );
}


double *dgetmem(leng)
int leng;
{
    char *getmem();

    return ( (double *)getmem(leng, sizeof(double)) );
}


float *fgetmem(leng)
int leng;
{
    char *getmem();

    return ( (float *)getmem(leng, sizeof(float)) );
}


real *rgetmem(leng)
int leng;
{
    char *getmem();

    return ( (real *)getmem(leng, sizeof(real)) );
}


float **ffgetmem(leng)
int leng;
{
    char *getmem();

    return ( (float **)getmem(leng, sizeof(float *)) );
}


char *getmem(leng, size)
int	 leng;
unsigned size;
{
    char *p = NULL;

    if ((p = (char *)calloc(leng, size)) == NULL){
	fprintf(stderr, "Memory allocation error !\n");
	exit(3);
    }
    return (p);
}
