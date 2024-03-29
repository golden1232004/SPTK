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
*								*
*    XY-Plotter simulator for X Window System			*
*								*
*				1992.03 T. KANNO		*
*								*
*	usage:							*
*		xgr [ options ] [ infile ]			*
*	options:						*
*		-s  s  : window reduction			*
*		-l     : landscape mode				*
*		-rv    : reverse mode				*
*		-m     : monochrome display mode		*
*		-bg bg : background color			*
*		-hl hl : highlight color			*
*		-bd bd : border color				*
*		-ms ms : mouse color				*
*		-g  g  : window geometry			*
*		-d  d  : display server				*
*		-t  t  : window title				*
*	infile:							*
*		plotter commands				*
*								*
****************************************************************/

/*  Standard C Libraries */
#include	<stdio.h>
#include	"xgr.h"

typedef enum _Boolean {FA, TR} Boolean;
char *BOOL[] = {"FALSE", "TRUE"};

#if	HAS_STDLIB
#include	<strings.h>	/* for rindex() */
#define	RINDEX(s, c)	rindex(s, c)
#else
#include	<string.h>
#define	RINDEX(s, c)	strrchr(s, c)
#endif

/* Defalut Value */
#define LANDSCAPE	FA
#define REVERSE		FA
#define C_FLG		FA
#define M_FLG		FA
#define FNO		1
#define FORECOLOR	"black"
#define BACKCOLOR	"white"
#define HIGHCOLOR	"blue"
#define BRDRCOLOR	"blue"
#define MOUSCOLOR	"red"
#define GEOMETRY	NULL
#define DISPLAY		NULL

/* Command Name */
char	*cmnd;

usage(status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - XY-plotter simulator for X-window system\n\n",cmnd);
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -s  s  : shrink                  [%g]\n",DEFAULT_SHRINK);
    fprintf(stderr, "       -l     : landscape               [%s]\n",BOOL[LANDSCAPE]);
    fprintf(stderr, "       -rv    : revese mode             [%s]\n",BOOL[REVERSE]);
    fprintf(stderr, "       -m     : monochrome display mode [%s]\n",BOOL[M_FLG]);
    fprintf(stderr, "       -bg bg : background color        [%s]\n",BACKCOLOR);
    fprintf(stderr, "       -hl hl : highlight color         [%s]\n",HIGHCOLOR);
    fprintf(stderr, "       -bd bd : border color            [%s]\n",BRDRCOLOR);
    fprintf(stderr, "       -ms ms : mouse color             [%s]\n",MOUSCOLOR);
    fprintf(stderr, "       -g  g  : geometry                [NULL]\n");
    fprintf(stderr, "       -d  d  : display                 [NULL]\n");
    fprintf(stderr, "       -t  t  : window title            [%s]\n",cmnd);
    fprintf(stderr, "       -h     : print this message \n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       plotter commands                 [stdin]\n");
    fprintf(stderr, "\n");
    exit(status);
}

float	shrink = DEFAULT_SHRINK;
int	landscape = LANDSCAPE;
int	reverse = REVERSE, c_flg = C_FLG, m_flg = M_FLG;
int	fno = FNO;
char	*fore_color = FORECOLOR, *back_color = BACKCOLOR,
	*high_color = HIGHCOLOR, *brdr_color = BRDRCOLOR,
	*mous_color = MOUSCOLOR;
char	*geometry = GEOMETRY, *dpy = DISPLAY, *windowtitle;

main(argc, argv)
	int	argc;
	char	*argv[];
{
	char	c;
	double	atof();

	cmnd = windowtitle = *argv;
	if (RINDEX(cmnd, '/'))
		cmnd = windowtitle = (char *)(RINDEX(cmnd, '/') + 1);

	while (--argc)  {
		if ((strcmp(*++argv, "-s") == 0) && argc > 1)  {
			shrink = atof(*++argv);
			if (shrink <= 0)
				usage();
			argc--;
		}
		else if (strcmp(*argv, "-landscape") == 0 || 
			 strcmp(*argv, "-land"	   ) == 0 ||
			 strcmp(*argv, "-l"	   ) == 0    )  {
			landscape = 1 - landscape;
		}
		else if (strcmp(*argv, "-reverse") == 0 || 
			 strcmp(*argv, "-rv"     ) == 0    )  {
			reverse = 1 - reverse;
		}
		else if (strcmp(*argv, "-color") == 0 || 
			 strcmp(*argv, "-c"    ) == 0    )  {
			c_flg = 1 - c_flg;
		}
		else if (strcmp(*argv, "-mono") == 0 || 
			 strcmp(*argv, "-m"    ) == 0    )  {
			m_flg = 1 - m_flg;
		}
		else if (strcmp(*argv, "-fno") == 0 && argc > 1)  {
			fno = atoi(*++argv);
			argc--;
		}
		else if (strcmp(*argv, "-bold") == 0 ||
			 strcmp(*argv, "-b"   ) == 0    )  {
			fno += 4;
		}
		else if (strcmp(*argv, "-fg") == 0 && argc > 1)  {
			fore_color = *++argv;
			argc--;
		}
		else if (strcmp(*argv, "-bg") == 0 && argc > 1)  {
			back_color = *++argv;
			argc--;
		}
		else if (strcmp(*argv, "-hl") == 0 && argc > 1)  {
			high_color = *++argv;
			argc--;
		}
		else if (strcmp(*argv, "-bd") == 0 && argc > 1)  {
			brdr_color = *++argv;
			argc--;
		}
		else if (strcmp(*argv, "-ms") == 0 && argc > 1)  {
			mous_color = *++argv;
			argc--;
		}
		else if ((strcmp(*argv, "-geometry") == 0 ||
			  strcmp(*argv, "-g"       ) == 0    ) && argc > 1)  {
			geometry = *++argv;
			argc--;
		}
		else if ((strcmp(*argv, "-display") == 0 ||
			  strcmp(*argv, "-d"      ) == 0    ) && argc > 1)  {
			dpy = *++argv;
			argc--;
		}
		else if ((strcmp(*argv, "-title") == 0 ||
			  strcmp(*argv, "-t"    ) == 0    ) && argc > 1)  {
			windowtitle = *++argv;
			argc--;
		}
		else if (strcmp(*argv, "-h") == 0 ||
			 strcmp(*argv, "--"   ) == 0 ||
			 strcmp(*argv, "-?"   ) == 0    )  {
			usage(0);
		}
		else
			usage(1);
	}
	ungetc(c = getchar(), stdin);
	if (c == EOF)
		return(-1);
	else if (c != '=')  {
		fprintf(stderr, "%s: Unexpected data format!\n", cmnd);
		return(-1);
	}
	open_display(dpy);
	init_window(argc, argv);
	init_pixmap();
	plot();
	main_loop();
	close_window();
	return (0);
}

