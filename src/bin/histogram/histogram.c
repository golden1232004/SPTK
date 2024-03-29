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

/************************************************************************
*									*
*    Histogram								*
*									*
*					1986.9	K.Tokuda		*
*					1995.7	K.Tokuda		*
*					1996.3  N.Miyazaki		*
*					2000.3  Y.Nankaku		*
*			                              			*
*	usage:								*
*		histogram [ options ] [ infile ] > stdout		*
*	options:							*
*		-l l	 : sequence length		[256]		*
*		-i i	 : infimum			[0]		*
*		-j j	 : supremum			[1]		*
*		-s s	 : step size			[0.1]		*
*		-n	 : normalization		[off]		*
*	infile:								*
*		sample data (float)					*
*	stdout:								*
*		histogram (float)					*
*									*
************************************************************************/


/*  Standard C Libraries  */
#include <stdio.h>
#include <SPTK.h>
#include <stdlib.h>
#include <string.h>


/*  Command Name  */
char	*cmnd;

int	usage(status)
{
	fprintf(stderr, "\n");
	fprintf(stderr, " %s - histogram\n", cmnd);
	fprintf(stderr, "\n");
	fprintf(stderr, "  usage:\n");
	fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
	fprintf(stderr, "  options:\n");
	fprintf(stderr, "       -l l  : frame size         [0]\n");
	fprintf(stderr, "       -i i  : infimum            [0.0]\n");
	fprintf(stderr, "       -j j  : supremum           [1.0]\n");
	fprintf(stderr, "       -s s  : step size          [0.1]\n");
	fprintf(stderr, "       -n    : normalization      FALSE\n");
	fprintf(stderr, "       -h    : print this message\n");
	fprintf(stderr, "  infile:\n");
	fprintf(stderr, "       data sequence (float)      [stdin]\n");
	fprintf(stderr, "  stdout:\n");
	fprintf(stderr, "       histogram (float)\n");
	fprintf(stderr, "  notice:\n");
	fprintf(stderr, "       if l > 0, calculate histogram frame by frame\n");
	fprintf(stderr, "\n");
	exit(status);
}

main(argc, argv)
	int argc;
	char *argv[];
{
  	char	*s, c;
	double  i=0, j=1, st=0.1;
	int	l=0, k, ii;
	int	flg=0, n=0;
	double  *h;
	double  *x, xx;
	FILE *fp = stdin;

	
	if ((cmnd = strrchr(argv[0], '/')) == NULL)
	        cmnd = argv[0];
	else
	        cmnd++;
	while (--argc){
		if(*(s = *++argv) == '-') {
			c = *++s;
			if(c != 'n' && *++s == '\0') {
				s = *++argv;
				--argc;
			}
			switch (c) {
			case 'l':
				l = atoi(s);
				break;
			case 'i':
				i = atof(s);
				break;
			case 'j':
				j = atof(s);
				break;
			case 's':
				st = atof(s);
				break;
			case 'n':
				n = 1;
				break;
			case 'h':
				usage(0);
			default:
				fprintf(stderr,
					"%s: unknown option '%c'\n", cmnd, c);
				usage(1);
				break;
			}
		} else
			fp = getfp(*argv, "r");
	}
	
	k = (int)((j - i) / st + 1.0);
	h = dgetmem(k+2);

	if (l) {
		x = dgetmem(l);
		while ( freadf(x, sizeof(*x), l, fp) == l) {
			flg += histogram(x, l, i, j, st, h);
		  
			if (n && l)
				for(ii = 0; ii <= k; ii++)
					h[ii] /= (double)l;

			fwritef(h, sizeof(*h), k, stdout);
		}
	}
	else{
		fillz(h, sizeof(*h), k+2);
		while (freadf(&xx, sizeof(xx), 1, fp) == 1){
			if (xx < i || xx > j) flg = 1;
			else for (ii = 0; ii <= k; ii++)
				if (xx < i + (ii + 1) * st){
					h[ii]++;
					break;
				}
			l++;
		}
	  
		if (n && l)
			for(ii = 0; ii <= k; ii++)
				h[ii] /= (double)l;
	  
		fwritef(h, sizeof(*h), k, stdout);
	}
	
	exit(flg);
}
