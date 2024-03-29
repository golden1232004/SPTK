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

/******************************************************************
 $Id: _c2ir.c,v 1.2 2002/12/25 05:28:45 sako Exp $

	cepstrum to impulse response
		c2ir(c,nc,h,leng)

		double	*c   : cepstral sequence
		int	nc   : order of cepstrum
		double	*h   : impulse response
		int	leng : length of impulse response

	impulse response to cepstrum
		ic2ir(h,leng,nc,c)

		double	*h   : impulse response
		int	nc   : order of cepstrum
		double	*c   : cepstral sequence
		int	leng : length of impulse response

			Naohiro Isshiki,	Dec.1995
********************************************************************/

void c2ir(c,nc,h,leng)
double 	*c,*h;
int 	leng,nc;
{
	register int	n, k, nr, upl;
	double	d, exp(), log();

	h[0] = exp(c[0]);
	for(n = 1; n < leng; ++n) {
		d = 0;
		upl = (n >= nc) ? nc - 1 : n;
		for(k = 1; k <= upl; ++k)
			d += k * c[k] * h[n - k];
		h[n] = d / n;
	}
}

void ic2ir(h,leng,c,nc)
double 	*h,*c;
int 	leng,nc;
{
	register int	n, k, nr, upl;
	double	d, exp(), log();

	c[0] = log(h[0]);
	for(n = 1; n < nc; ++n) {
		d = (n >= leng) ? 0 : n * h[n];
		upl = (n > leng) ? n - leng + 1 : 1;
		for(k = upl; k < n; ++k)
			d -= k * c[k] * h[n - k];
		c[n] = d / (n * h[0]);
	}
}
 
