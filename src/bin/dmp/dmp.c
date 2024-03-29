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
*    Binary File Dump							*
*									*
*					1996.5  K.Koishida		*
*									*
*	usage:								*
*		dmp [options] [infile] > stdout				*
*	options:							*
*		-n n     :  block order  (0,...,n)           [INFINITE]	*
*		-l l	 :  block length (1,...,l)           [INFINITE] *
*		+type    :  data type 			     [f]	*
*				c (char)     s (short)			*
*				i (int)      l (long)			*
*				f (float)    d (double)			*
*		%format :  print format			     [N/A]	*
*									*
************************************************************************/

static char *rcs_id = "$Id: dmp.c,v 1.2 2002/12/25 05:29:15 sako Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <SPTK.h>


/*  Default Values  */
#define SIZE  128


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - binary file dump\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n", cmnd);
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -n n  : block order   (0,...,n)      [EOD]\n");
    fprintf(stderr, "       -l l  : block length  (1,...,l)      [EOD]\n");
    fprintf(stderr, "       +type : data type                    [f]\n");
    fprintf(stderr, "                c (char)      s (short)\n");
    fprintf(stderr, "                i (int)       l (long)\n");
    fprintf(stderr, "                f (float)     d (double)\n");
    fprintf(stderr, "       %%form : print format(printf style)   [N/A]\n");
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       data sequence                        [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       dumped sequence\n");
    fprintf(stderr, "\n");

    exit(status);
}

void main(int argc, char **argv)
{
    int 	   n = -1, size = sizeof(float), i = 0, eflag = 0, lflag = 0;
    FILE 	   *fp = stdin;
    register char  *s, c, cc = 'f';
    char format[SIZE],form[SIZE];
    int ff = 0;
    union          u{ short  s;
		      int    i;
		      float  f;
		      double d;
		      char   c;
		     } x;
    
    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if(*(s = *++argv) == '-') {
	    c = *++s;
	    switch(c) {
		case 'n':
		    n = atoi(*++argv)+1;
		    --argc;
		    break;
		case 'l':
		    n = atoi(*++argv)-1;
		    lflag = 1;
		    --argc;
		    break;
		case 'e':
		    eflag = 1 - eflag;
		    break;
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}
	else if (*s == '+') {
	    c = *++s;
	    switch(c) {
		case 'c':
		    cc = 'c';
		    size = sizeof(char);
		    break;
		case 's':
		    cc = 's';
		    size = sizeof(short);
		    break;
		case 'l':
		    cc = 'l';
		    size = sizeof(long);
		    break;
		case 'i':
		    cc = 'i';
		    size = sizeof(int);
		    break;
		case 'f':
		    cc = 'f';
		    size = sizeof(float);
		    break;
		case 'd':
		    cc = 'd';
		    size = sizeof(double);
		    break;
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
	    }
	}
	else if( *s == '%'){
		strcpy(format,s);
		ff = 1;
	}
	else
	    fp = getfp(*argv, "r");

    for (i = 0;; i++){
	if (n >= 0 && i >= n+lflag)
	    i = 0;
	if (fread(&x.f, size, 1, fp) != 1)
	    break;
	if (eflag)
	    printf("%d\t0\n", i+lflag);
	switch(cc){
	    case 's':
		strcpy(form,"%d\t%d\n");
		if(ff){
			strcpy(form,"%d\t");
			strcat(form,format);
			strcat(form,"\n");
		}
	        printf(form, i+lflag, x.s);
		break;
	    case 'i':
		strcpy(form,"%d\t%d\n");
		if(ff){
			strcpy(form,"%d\t");
			strcat(form,format);
			strcat(form,"\n");
		}
		printf(form, i+lflag, x.i);
		break;
	    case 'f':
		strcpy(form,"%d\t%g\n");
		if(ff){
			strcpy(form,"%d\t");
			strcat(form,format);
			strcat(form,"\n");
		}
		printf(form, i+lflag, x.f);
		break;
	    case 'd':
		strcpy(form,"%d\t%g\n");
		if(ff){
			strcpy(form,"%d\t");
			strcat(form,format);
			strcat(form,"\n");
		}
		printf(form, i+lflag, x.d);
		break;
	    case 'c':
		strcpy(form,"%d\t%d\n");
		if(ff){
			strcpy(form,"%d\t");
			strcat(form,format);
			strcat(form,"\n");
		}
		printf(form, i+lflag, x.c);
		break;
	    }
	if (eflag)
	    printf("%d\t0\n", i+lflag);
    }
    exit(0);
}
