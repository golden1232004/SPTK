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
*    Generate Normal Distrubuted Random Value				*
*									*
*					1991.9 T.Kanno			*
*					1996.1 N.Miyazaki modified	*
*									*
*	usage:								*
*		nrand [ options ] > stdout				*
*	options:							*
*		-l l	 : output length		[256]		*
*		-s s     : seed for nrand		[1]		*
*	stdout:								*
*		random value (float)					*
*	notice:								*
*		if l < 0, generate infinite sequence			*
*									*
************************************************************************/

/*  Standard C Libraries  */
#include	<stdio.h>
#include	<math.h>
#include	<string.h>
#include	<SPTK.h>


/*  Default Values  */
#define	RAND_MAX	32767


double	rnd();
unsigned long	srnd();
double	nrandom();
int	nrand();

#define LENG	256
#define SEED	1


/*  Command Name  */
char	*cmnd;


void usage()
{
	fprintf(stderr, "\n");
	fprintf(stderr, " %s - generate normal distributed random value\n", cmnd);
	fprintf(stderr, "\n");
	fprintf(stderr, "  usage:\n");
	fprintf(stderr, "       %s [ options ] > stdout\n", cmnd);
	fprintf(stderr, "  options:\n");
	fprintf(stderr, "       -l l  : output length      [%d]\n",LENG);
	fprintf(stderr, "       -s s  : seed for nrand     [%d]\n",SEED);
	fprintf(stderr, "       -h    : print this message\n");
	fprintf(stderr, "  stdout:\n");
	fprintf(stderr, "       random values (float)\n");
	fprintf(stderr, "  notice:\n");
	fprintf(stderr, "       if l < 0, generate infinite sequence\n");
	fprintf(stderr, "\n");
	exit(1);
}


main( argc, argv )
	int 	argc;
	char	*argv[];
{
	char	*str, flg;
	int	leng = LENG, seed = SEED,  i;
	long	next = SEED;
	double	p;

        if ((cmnd = strrchr(argv[0], '/')) == NULL)
	    cmnd = argv[0];
        else
	    cmnd++;

	while (--argc)  {
		if (*(str = *++argv) == '-')  {
			flg = *++str;
			if (*++str == '\0')  {
				str = *++argv;
				argc--;
			}
			switch (flg)  {
			    case 'l':	leng = atoi(str);	break;
			    case 's':	seed = atoi(str);	break;
			    case 'h':	
			    default :	usage();
			}
		} else usage();
		
	}

	if(seed != 1) next = srnd((unsigned)seed);
	
	for(i=0;; i++){
	    p = (double)nrandom(&next);
	    fwritef(&p, sizeof(p), 1, stdout);
	    
	    if(i == leng-1) break;
	}
	exit(0);
}

