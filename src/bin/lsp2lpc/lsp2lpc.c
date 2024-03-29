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
*    Transform LSP to LPC					 	*
*									*
*					1996.1  K.Koishida		*
*									*
*	usage:								*
*		lsp2lpc [ options ] [ infile ] > stdout			*
*	options:							*
*		-m m     :  order of LPC		[25]		*
*		-s s     :  sampling frequency (kHz)	[10]		*
*		-k       :  input & output gain		[TRUE]		*
*		-i i     :  input format (see infile)	[0]		*
*	infile:								*
*	      input format	LSP					*
*		    0		normalized frequency (0 ~ pi)		*
*		    1		normalized frequency (0 ~ 0.5)		*
*		    2		frequency (kHz)				*
*		    3		frequency (Hz)				*
*		LSP							*
*		    , f(1), ..., f(m),					*
*	stdout:								*
*		LPC							*
*		    , K(=1), a(1), ..., a(M),				*
*	require:							*
*		lsp2lpc()						*
*									*
************************************************************************/

static char *rcs_id = "$Id: lsp2lpc.c,v 1.2 2002/12/25 05:31:10 sako Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/*  Required Functions  */
void	lsp2lpc();


/*  Default Values  */
#define ORDER		25
#define ITYPE		0
#define SAMPLING	10
#define GAIN		1


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - transform LSP to LPC\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -m m  : order of LPC        [%d]\n", ORDER);
    fprintf(stderr, "       -s s  : sampling frequency  [%d]\n", SAMPLING);
    fprintf(stderr, "       -k    : input & output gain [TRUE]\n");
    fprintf(stderr, "       -i i  : input format        [%d]\n", ITYPE);
    fprintf(stderr, "                 0 (normalized frequency <0...pi>)\n");
    fprintf(stderr, "                 1 (normalized frequency <0...0.5>)\n");
    fprintf(stderr, "                 2 (frequency (kHz))\n");
    fprintf(stderr, "                 3 (frequency (Hz))\n");
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       LSP (float)                [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       LP coefficients (float)\n");
    fprintf(stderr, "\n");
    exit(status);
}


void main(int argc, char **argv)
{
    int		m = ORDER, sampling = SAMPLING, itype = ITYPE, i, gain = GAIN;
    FILE	*fp = stdin;
    double	*a, *lsp;

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
		case 's':
		    sampling = atoi(*++argv);
		    --argc;
		    break;
		case 'k':
		    gain = 0;
		    break;
		case 'i':
		    itype = atoi(*++argv);
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

    lsp = dgetmem(m+m+1+gain);
    a = lsp + m + gain;

    while (freadf(lsp, sizeof(*lsp), m+gain, fp) == m+gain){
	if(itype == 0)
	    for(i=gain; i<m+gain; i++)
		lsp[i] /= PI2;
	else if (itype == 2 || itype ==3)
	    for(i=gain; i<m+gain; i++)
		lsp[i] /= sampling;
	
	if(itype == 3)
	    for(i=gain; i<m+gain; i++)
		lsp[i] /= 1000;
	    
	lsp2lpc(lsp+gain, a, m);
	if(gain)fwritef(lsp, sizeof(*lsp), 1, stdout);
	fwritef(a+gain, sizeof(*a), m+1-gain, stdout);
    }
    exit(0);
}

