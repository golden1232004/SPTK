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

    $Id: _acep.c,v 1.2 2002/12/25 05:28:19 sako Exp $

    Adaptive Cepstral Analysis

	double acep(x, c, m, lambda, step, pd);

	double   x      : input sequence
	double   *c     : cepstrum
	int      m      : order of cepstrum
	double   lambda : leakage factor
	double   step   : step size
	int	 pd	: order of pade approximation
	double   eps	: minimum value for epsilon

	return value  : prediction error

*****************************************************************/

#include <stdio.h>
#include <SPTK.h>

#define PADEORD 	4

double acep(x, c, m, lambda, step, tau, pd, eps)
double x, *c, lambda, step, eps;
int m, pd;
{
    register int   i;
    static double  *cc = NULL, *e, *ep, *d, gg = 1.0;
    static int     size;
    double 	   mu, tx, lmadf(), log();
    
    if(cc == NULL){
	cc = dgetmem(m+m+m+3+(m+1)*PADEORD*2);
	e  = cc + m + 1;
	ep = e + m + 1;
	d  = ep + m + 1;
	size = m;
    }
    if(m > size){
	free(cc);
	cc = dgetmem(m+m+m+3+(m+1)*PADEORD*2);
	e  = cc + m + 1;
	ep = e + m + 1;
	d  = ep + m + 1;
	size = m;
    }

    for(i=1; i<=m; i++)
	cc[i] = -c[i];
    
    x = lmadf(x, cc, m, pd, d);

    for(i=m; i>=1; i--)
	e[i] = e[i-1];
    e[0] = x;
    
    gg = gg * lambda + (1.0 - lambda) * e[0] * e[0];
    c[0] = 0.5 * log(gg);
    
    gg = ( gg < eps )? eps : gg;
    mu = step / (double) m / gg;
    tx = 2 * (1.0 - tau) * x;
    
    for(i=1; i<=m; i++){
	ep[i] = tau * ep[i] - tx * e[i];
	c[i] -= mu * ep[i];
    }
    
    return(x);
}
