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

    $Id: _iglsadf.c,v 1.2 2002/12/25 05:30:25 sako Exp $

    Inverse GLSA Digital Filter

	double	iglsadf(x, c, m, n, d)

	double	x   : input
	double	*c  : filter coefficients (K, g*c'(1), ..., g*c'(m))
	int	m   : order of cepstrum
	int     n   : -1/gamma
	double  *d  : delay

	return value : filtered data

*****************************************************************/

double iglsadf(x, c, m, n, d)
double x, *c, *d;
int m, n;
{
    int  	i;
    double      zerodf1();
    
    for(i=0; i<n; i++)
	x = zerodf1(x, c, m, &d[m*i]);

    return(x);
}

double iglsadf1(x, c, m, n, d)
double x, *c, *d;
int m, n;
{
    int  	i;
    double      gamma, gzerodf();

    gamma = -1 / (double)n;
    
    for(i=0; i<n; i++)
	x = gzerodf(x, c, m, gamma, &d[m*i]);
    
    return(x);
}

double gzerodf(x, c, m, g, d)
double x, *c, *d, g;
int m;
{
    double   y = 0.0;
    
    for(m--; m>0; m--){
	y += c[m+1] * d[m];
	d[m] = d[m-1];
    }
    y += c[1] * d[0];
    d[0] = x;

    x += y * g;

    return(x);
}

