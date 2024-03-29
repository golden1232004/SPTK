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
*    Generate Sinusoidal Sequence					*
*									*
*					1989.7  K.Tokuda		*
*					1996.4  K.Koishida		*
*									*
*	usage:								*
*		sin [options] > stdout					*
*	options:							*
*		-l l     :  length		 	[256]		*
*		-p p     :  period		 	[10.0]		*
*		-m m     :  magnitude		 	[1.0]		*
*	stdout:								*
*		sinusoidal sequence					*
*		    , sin(2pi*0/p), sin(2pi*1/p), ..., sin(2pi*(l-1)/p	*
*	notice:								*
*		if l < 0, generate infinite sequence			*
*									*
************************************************************************/

static char *rcs_id = "$Id: sin.c,v 1.2 2002/12/25 05:32:39 sako Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/*  Default Values  */
#define LENG 		256
#define PERIOD 		10.0
#define MAGNITUDE	1.0


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - generate sinusoidal sequence\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n", cmnd);
    fprintf(stderr, "       %s [ options ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -l l  : length             [%d]\n", LENG);
    fprintf(stderr, "       -p p  : period             [%g]\n", PERIOD);
    fprintf(stderr, "       -m m  : magnitude          [%g]\n", MAGNITUDE);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       sinusoidal sequence (float)\n");
    fprintf(stderr, "  notice:\n");
    fprintf(stderr, "       if l < 0, generate infinite sequence\n");
    fprintf(stderr, "\n");
    exit(status);
}


void main(int argc, char **argv)
{
    int		l = LENG, i;
    double	mag = MAGNITUDE, period = PERIOD, x, sin(), atof();
    
    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if (**++argv == '-') {
	    switch (*(*argv+1)) {
		case 'l':
		    l = atoi(*++argv);
		    --argc;
		    break;
		case 'p':
		    period = atof(*++argv);
		    --argc;
		    break;
		case 'm':
		    mag = atof(*++argv);
		    --argc;
		    break;
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}

    for(i=0;; i++){
	x = mag * sin(PI2 * (double) i / period);
	fwritef(&x, sizeof(x), 1, stdout);

	if(i == l-1) break;
    }

    exit(0);
}

