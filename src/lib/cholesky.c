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

/****************************************************************
    $Id: cholesky.c,v 1.2 2002/12/25 05:34:34 sako Exp $

    Solve Linear Set of Equations
		       Using Cholesky Decomposition 

	int	cholesky(c, a, b, n, eps)	< Ca=b >

	double	*c  : symmetrical coefficient matrix
	double	*a  : solution vector
	double	*b  : constant vector
	int	n   : order of vector
	double  eps : error check (if -1.0, 1.0e-6 is assumed)

	return value : 	0  -> normally completed
			-1 -> abnormally completed

****************************************************************/

#include <stdio.h>
#include <SPTK.h>

int cholesky(c, a, b, n, eps)
double	*c, *b, *a, eps;
int	n;
{
    register int	i, j, k;
    static double	*d = NULL, *y, *v, *vp;
    static int		size;

    if(d == NULL){
	d = dgetmem(n*(n+2));
	y = d + n;
	v = y + n;
	size = n;
    }

    if(n > size){
	free(d);
	d = dgetmem(n*(n+2));
	y = d + n;
	v = y + n;
	size = n;
    }

    if(eps < 0.0) eps = 1.0e-6;
    
    for(j=0; j<n; j++, c+=n){
	d[j] = c[j];
	vp = v + j*n;
	for(k = 0; k < j; ++k)
	    d[j] -= vp[k] * vp[k] * d[k];

	if(ABS(d[j]) <= eps) return(-1);

	for(i=j+1; i<n; i++){
	    vp = v + i*n;
	    vp[j] = c[i];
	    for(k=0; k<j; ++k)
		vp[j] -= vp[k] * v[j*n + k] * d[k];
	    vp[j] /= d[j];
	}
    }

    for(i=0; i<n; i++){
	y[i] = b[i];
	vp = v + i*n;
	for(k=0; k<i; k++)
	    y[i] -= vp[k] * y[k];
    }

    for(i=n-1; i>=0; i--){
	a[i] = y[i] / d[i];
	for(k=i+1; k<n; k++)
	    a[i] -= v[n*k + i] * a[k];
    }
    return(0);
}


