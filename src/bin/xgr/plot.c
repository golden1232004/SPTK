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
*	Interpret X-Y Ploter Commands				*
****************************************************************/
#include	<X11/Xlib.h>
#include	<stdio.h>
#include	<limits.h>
#include	"xgr.h"
#include	"gcdata.h"

#define abs(x)		((x) >= 0 ? (x) : (-x))
#define	norm(x)		((short)((x)/shrink+0.5))
#define	normx(x)	((short)((x)/shrink+0.5))
/*
#define	normy(y)	((short)((YLENG-(y))/shrink+0.5))
*/
#define direction(x)	{}

static struct	cord  {
	int	x;
	int	y;
}  pb;

static int	sp = 1;
static int	cw = FCW, ch = FCH, th = 0;

extern Display		*display;
extern Window		main_window;
extern int		screen;
extern GC		gc;
extern unsigned long	forepix, backpix;
extern float		shrink;
extern int		landscape;
extern int		c_flg, m_flg;
extern int		fno;

static XPoint		points[SIZE];
static int		line_width = 1;
static int		line_style = LineSolid;
static XRectangle	rect;

static line(),dplot(),reset_fill(),fillbox(),fillpoly(),text();

static char	*symbol_upper = "ABGDEZHQIKLMNXOPRSTUFCYW";
static char	*symbol_lower = "abgdezhqiklmnxoprstufcyw";
#define		INFTY	165
#define		SPACE	32


static short normy( y )
	int	y;
{
	if (!landscape)
		return ((short)((YLENG-(y))/shrink+0.5));
	else
		return ((short)((XLENG-(y))/shrink+0.5));
}

static _flush()
{
	if (sp > 1)  {
/*		polylines(sp);
*/		line(points, sp);
		points[0].x = points[sp-1].x;
		points[0].y = points[sp-1].y;
		sp = 1;
	}
}

static _send( buf )
	int	*buf;
{
	if (sp == SIZE)
		_flush();

	points[sp  ].x = normx(*buf++);
	points[sp++].y = normy(*buf);
}

static _getcord( buf )
	int	*buf;
{
	register int	c;

	while ((c = getchar()) == '\n' || c == ' ')
		;
	if (c)  {
		ungetc(c, stdin);
		scanf("%d %d", buf, buf+1);
		return(1);
	}
	else
		return(0);
}

static _line()
{
	while (_getcord(&pb))
		_send(&pb);

	_flush();
}


static _move( x, y )
	int	x, y;
{
	points[0].x = normx(x);
	points[0].y = normy(y);
}


static line( points, n )
	XPoint	*points;
	int	n;
{
	XDrawLines(display, main_window, gc, points, n, CoordModeOrigin);
}

static polyline( points, frame, fill, n )
	XPoint	*points;
	int	frame, fill;
	int	n;
{
	if ( fill != -1 && (fill%=10) != 9 )  {
		fillpoly(points, fill+6, n+1);
		reset_fill();
	}
	if (frame)
		line(points, n+1);
}

static polyg( type )
	int	type;
{
        register int    n;
	int		x, y, w, h;

	scanf("%d %d %d %d", &x, &y, &w, &h);
	dplot(type, normx(x), normy(y+h), norm(w), norm(h));

	return(0);
}

#define	LEVEL	256
#define	POINTS	1024

static dplot( density, x, y, w, h )
	int	density;
	short	x, y, w, h;
{
	register int	n, k, l;
	int		n_max, n_plot, flg[POINTS];
	int		p;
	XPoint		pos[POINTS];

	n_max = ++w * ++h;
	n_plot = (density * n_max) / LEVEL;

#if BSD
	bzero((char *)flg, sizeof(*flg)*POINTS);
#else
	memset((char *)flg, 0, sizeof(*flg)*POINTS);
#endif

	for (n=0; n<n_plot; n++)  {
		p = (int)(n_max * (double)rand() / (double)INT_MAX);
		if (flg[p] == 0)  {
			flg[p] = 1;
			pos[n].x = x + (short)p % w;
			pos[n].y = y - (short)p / w;
		}
	}
	XDrawPoints(display, main_window, gc, pos, n_plot, CoordModeOrigin);
}

static hatching( type )
	int	type;
{
	register int	n;
	int		style, frame;
	int		x, y, d, angle;

	scanf("%d %d", &d, &angle);

	for (n=0; _getcord(&pb); n++)  {
		points[n].x = normx(pb.x);
		points[n].y = normy(pb.y);
	}
	points[n].x = points[0].x;
	points[n].y = points[0].y;

	switch ( type-=20 )  {
	    case 1:	frame = 1;	type = -1;	break;
	    case 2:	frame = 0;	type = -1;	break;
	    case 3:	frame = 1;	type = -1;	break;
	    default:	if (type < 0)  {
				frame = 0;
				type = -type;
			}
			else
				frame = 1;
							break;
	}
	polyline(points, frame, type, n);
}

static reset_fill()
{
	fillbox(15, 0, 0, 0, 0);
}

static box( x, y, w, h )
	short	x, y, w, h;
{
	XDrawRectangle(display, main_window, gc, x, y, w, h);
}

static fillbox( type, x, y, w, h )
	int	type;
	short	x, y, w, h;
{
	Pixmap	till_pmap;

	till_pmap = XCreatePixmapFromBitmapData(display, main_window,
			till_bits[type], till_width, till_height,
			forepix, backpix, DefaultDepth(display, 0));

	XSetTile(display, gc, till_pmap);
	XSetFillStyle(display, gc, FillTiled);
	XFillRectangle(display, main_window, gc, x, y, w, h);

	XFreePixmap(display, till_pmap);
}

static fillpoly( points, type, n )
	XPoint	*points;
	int	type, n;
{
	Pixmap	till_pmap;

	till_pmap = XCreatePixmapFromBitmapData(display, main_window,
			till_bits[type], till_width, till_height,
			forepix, backpix, DefaultDepth(display, 0));

	XSetTile(display, gc, till_pmap);
	XSetFillStyle(display, gc, FillTiled);
	XFillPolygon(display, main_window, gc, points, n, Convex, CoordModeOrigin);

	XFreePixmap(display, till_pmap);
}

static get_str()
{		    
	int	c, i, j, k, sfg=0;
	char	s[512];
	float	adj = 0.29;

	for (i=j=k=0; c=getchar(); i++, j++)  {
		if ((c &= 0xff) <= 192)  {	/*  not Symbol	  */
			if (sfg)  {		/*  flush buffer  */
				s[i] = '\0';
				points[0].x += (int)(cw * k * adj);
				text(s, i, FSymbol);
				k = j;
				i = j = 0;
				sfg = 0;
			}
			s[i] = c;
		}
		else  {				/*  Symbol        */
			if (!sfg)  {		/*  flush buffer  */
				s[i] = '\0';
				points[0].x += (int)(cw * k * adj);
				text(s, i, fno);
				k = j;
				i = j = 0;
				sfg = 1;
			}
			if (c <= 216)
				sprintf(s+i,"%c", symbol_upper[c-193]);
			else if (c >= 225 && c <= 248)
				sprintf(s+i,"%c", symbol_lower[c-225]);
			else if (c == 254)
				sprintf(s+i,"%c", INFTY);
			else
				sprintf(s+i,"%c", SPACE);
		}
	}
	s[i] = '\0';
	points[0].x += (int)(cw * k * adj);
	if (! sfg)
		text(s, i, fno);
	else
		text(s, i, FSymbol);
}

static text( s, n, fn )
	char	*s;
	int	n, fn;
{
	register int	cx, cy;
	static int	flg=0, cfn=-1, ccw=-1, cch=-1;
	float		xadj, yadj;

	if (n <= 0)	return;

	if (fn != cfn)   {
		ccw = cw;	cch = ch;
		if (ch == LFCH)
			cfn = fn + 1;
		else
			cfn = fn;
		flg = 1;
	}
	if (flg || cw != ccw || ch != cch)  {
		if (cw < ccw || ch < cch)
			cfn = fn - 1;
		else if (cw > ccw || ch > cch)
			cfn = fn + 1;

		XSetFont(display, gc, XLoadFont(display, f_name[cfn]));
		ccw = cw;	cch = ch;
		flg = 0;
	}

	if (th == 0)
		XDrawString(display, main_window, gc, 
			    points[0].x, points[0].y, s, n);
	else  {
		xadj = ccw / shrink;
		yadj = (cch > FCH ? 0.75 : 1.25) * cch / shrink;
		while (n)  {
			cx = points[0].x - xadj;
			cy = points[0].y - yadj * --n;
			XDrawString(display, main_window, gc, cx, cy, s, 1);
			*s++;
		}
	}
}

static newpen( w )
	int	w;
{
	if (w < 0 || w > 10)
		w = 1;

	if (!c_flg || m_flg)  {
		line_width = l_width[w];
		XSetLineAttributes(display, gc,
				   line_width, line_style, CapButt, JoinMiter);
	}
	else
		XSetForeground(display, gc, get_color_pix(c_name[w]));
}

static line_type( w )
	int	w;
{
	int	dash_offset = 0;

	if (w == 0)
		line_style = LineSolid;
	else if (w > 0 && w < 12)
		line_style = LineOnOffDash;
	else
		return;

	XSetLineAttributes(display, gc,
			   line_width, line_style, CapButt, JoinMiter);
	if (w > 0)  {
		XSetDashes(display, gc, dash_offset,
			   l_style[w].list, l_style[w].no);
	}
}

static clip( xmin, ymin, xmax, ymax )
	int	xmin, ymin, xmax, ymax;
{
	rect.x = xmin;
	rect.y = ymin;
	rect.width = xmax - xmin;
	rect.height = ymax - ymin;

	XSetClipRectangles(display, gc, 0, 0, &rect, 1, Unsorted);
}

static mark( w )
	int	w;
{
	Pixmap	mark_pmap;

	mark_pmap = XCreatePixmapFromBitmapData(display, main_window,
			mark_bits[w], mark_width, mark_height,
			forepix, backpix,
			DefaultDepth(display, 0));

	XCopyArea(display, mark_pmap, main_window, gc,
		  0, 0, mark_width, mark_height,
		  points[0].x - mark_width/2,
		  points[0].y - mark_height/2);

	XFreePixmap(display, mark_pmap);
}

static circle( x0, y0, r1, r2, arg1, arg2 )
	int	x0, y0, r1, r2, arg1, arg2;
{
	int		x, y;
	unsigned int	width, height;
	double		cos(), sin();

	arg1 /= 10;
	arg2 /= 10;
	width  = normx(abs(r1 * cos((double)arg1)));
	height = normx(abs(r2 * sin((double)arg2)));
	x = normx(x0) - width;
	y = normy(y0) - height;
	width  *= 2;
	height *= 2;

	XDrawArc(display, main_window, gc, x, y,
		 width, height, arg1*64, arg2*64);
}

plot()
{
	register int	c;
	int		w, h, n, xmin, ymin, xmax, ymax;
	int		x0, y0, r1, r2, arg1, arg2;
	short		normy();

	while ((c = getchar()) != EOF)  {
		switch (c)  {
		    case 'M':	scanf("%d %d", &pb.x, &pb.y);
				_move(pb.x, pb.y);			break;
		    case 'D':	_line();				break;
		    case '%':	scanf("%d", &n);
				hatching(n);				break;
/*				polyg(n);				break;
*/		    case 'P':	get_str();				break;
		    case 'S':	scanf("%d", &ch);			break;
		    case 'Q':	scanf("%d", &cw);			break;
		    case 'R':	scanf("%d", &th);
				th = (th == 0) ? 0 : 1;			break;
		    case 'L':	scanf("%d", &w);	line_type(w);	break;
		    case 'W':	scanf("%d %d %d %d %d %d",
				      &x0, &y0, &r1, &r2, &arg1, &arg2);
				circle(x0, y0, r1, r2, arg1, arg2);	break;
		    case 'N':	scanf("%d", &w);	mark(w);	break;
		    case 'J':	scanf("%d", &w);	newpen(w);	break;
		    case '\\':	scanf("%d %d", &xmin, &ymin);		break;
		    case 'Z':	scanf("%d %d", &xmax, &ymax);
				if (!landscape)  {
					if (xmax > XLENG)	xmax = XLENG;
					if (ymax > YLENG)	ymax = YLENG;
				}
				else  {
					if (xmax > YLENG)	xmax = YLENG;
					if (ymax > XLENG)	ymax = XLENG;
				}
				clip(normx(xmin), normy(ymax),
				     normx(xmax), normy(ymin));		break;
		    case ';':
		    case ':':						break;
		    default:						break;
		}
	}
}
