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
*    FFT Cepstral Analysis						*
*									*
*					1985.11 K.Tokuda		*
*					1996.3  K.Koishida		*
*									*
*	usage:								*
*		fftcep [ options ] [ infile ] > stdout			*
*	options:							*
*		-m  m    :  order of cepstrum				*
*		-l  l    :  frame length				*
*		-j  j    :  number of iteration				*
*		-k  k    :  accelation factor				*
*	infile:								*
*		data sequence						*
*		    , x(0), x(1), ..., x(l-1),				*
*	stdout:								*
*		cepstral coefficients					*
*		    , c(0), c(1), ..., c(m),				*
*	require:							*
*		fftcep()						*
*	note:								*
*		When -j & -k options are specified,			*
*		improved cepstral analysis is performed.		* 
*									*
************************************************************************/

static char *rcs_id = "$Id: fftcep.c,v 1.4 2002/12/25 05:29:33 sako Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/*  Required Functions  */
void	fftcep();


/*  Default Values  */
#define ORDER		25
#define FLNG		256
#define MAXITR		0
#define ACCELATION	0.0
#define EPS		0.0


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - FFT cepstral analysis\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -m m  : order of cepstrum   [%d]\n", ORDER);
    fprintf(stderr, "       -l l  : frame length        [%d]\n", FLNG);
    fprintf(stderr, "       -j j  : number of iteration [%d]\n", MAXITR);
    fprintf(stderr, "       -k k  : accelation factor   [%g]\n", ACCELATION);
    fprintf(stderr, "       -e e  : epsilon             [%g]\n", EPS);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       windowed sequence (float)   [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       cepstrum (float)\n");
    fprintf(stderr, "  note:\n");
    fprintf(stderr, "       When -j & -k options are specified,\n");
    fprintf(stderr, "       improved cepstral analysis is performed.\n");
    fprintf(stderr, "\n");
    exit(status);
}

void main(int argc, char **argv)
{
    int		m = ORDER, l = FLNG, itr = MAXITR, i;
    double	ac = ACCELATION, eps = EPS, *x, *y, *c, log(), atof();
    FILE	*fp = stdin;
    
    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if (**++argv == '-') {
	    switch (*(*argv+1)) {
		case 'm':
		    m = atoi(*++argv);
		    --argc;
		    break;
		case 'l':
		    l = atoi(*++argv);
		    --argc;
		    break;
		case 'j':
		    itr = atoi(*++argv);
		    --argc;
		    break;
		case 'k':
		    ac = atof(*++argv);
		    --argc;
		    break;
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}
	else 
	    fp = getfp(*argv, "r");

    x = dgetmem(l+l+m+1);
    y = x + l; c = y + l;
    
    while (freadf(x, sizeof(*x), l, fp) == l){
	fftr(x, y, l);
	for(i=0; i<l; i++)
	    x[i] = log(x[i]*x[i] + y[i]*y[i] + eps);

	fftcep(x, l, c, m, itr, ac);
	
	fwritef(c, sizeof(*c), m+1, stdout);
    }
    exit(0);
}
