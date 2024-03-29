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
*    clipping								*
*									*
*					1989.8.31   K.Tokuda		*
*									*
*	usage:								*
*		clip [ -y Ymin Ymax] [infile] > stdout			*
*	options:							*
*	        -y    ymin ymax : lower bound & upper bound [-1.0 1.0]  *
*	        -ymin ymin      : lower bound (ymax = inf)  [N/A]       *
*	        -ymax ymax      : upper bound (ymin = -inf) [N/A]       *
*	infile:								*
*		data sequence (float)					*
*	stdout:								*
*		clipped data sequence (float)				*
*	require:							*
*		clip()							*
*									*
************************************************************************/


/* Standard C Libraries */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/* Default Value */
#define FLT_MAX 3.4e+38
#define FLT_MIN -3.4e+38
#define YMIN	-1.0
#define YMAX	1.0


/* Command Name */
char	*cmnd;

void usage()
{
	fprintf(stderr, "\n");
	fprintf(stderr, " %s - data clipping\n", cmnd);
	fprintf(stderr, "\n");
	fprintf(stderr, "  usage:\n");
	fprintf(stderr, "       %s [ option ] [ infile ] > outfile\n", cmnd);
	fprintf(stderr, "  option:\n");
	fprintf(stderr, "       -y    ymin ymax : lower bound & upper bound [-1.0 1.0]\n");
	fprintf(stderr, "       -ymin ymin      : lower bound (ymax = inf)  [N/A]\n");
	fprintf(stderr, "       -ymax ymax      : upper bound (ymin = -inf) [N/A]\n");
	fprintf(stderr, "       -h              : print this message\n");
	fprintf(stderr, "  infile:\n");
	fprintf(stderr, "       data sequence (float)                       [stdin]\n");
	fprintf(stderr, "  stdout:\n");
	fprintf(stderr, "       clipped data sequence (float)\n");
	fprintf(stderr, "\n");
	exit(1);
}

main(argc, argv)
	int	argc;
	char	*argv[];
{
        FILE    *fp = stdin;
	int	size;
	double	ymin = YMIN, ymax = YMAX;
	double	*x, *y;
	double	atof();

	if ((cmnd = strrchr(argv[0],'/')) == NULL)
	    cmnd = argv[0];
	else
	    cmnd++;
	while (--argc)
		if (**++argv == '-'){
			argc--;
			switch (*(*argv+1)) {
			case 'y':
				if((*(*argv+2))=='m'){
					switch (*(*argv+3)) {
						case 'i':
							ymin = atof(*++argv);
							ymax = FLT_MAX;
							break;
						case 'a':
							ymax = atof(*++argv);
							ymin = FLT_MIN;
							break;
					}
				} else{
					ymin = atof(*++argv);
					argc--;
					ymax = atof(*++argv);
				}
				break;
			case 'h':
			default:
				usage();
			}
		} 
		else 
		    fp = getfp(*argv, "r");

	x = dgetmem(2*256);
	y = x + 256;

	while ((size = freadf(x, sizeof(*x), 256, fp)) !=0){
		clip (x, size, ymin, ymax, y);
		fwritef(y, sizeof(*y), size, stdout);
	}
	exit(0);
}
