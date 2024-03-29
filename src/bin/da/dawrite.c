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
*                                                                       *
*    play 16-bit linear PCM data on LINUX and SS10                      *
*                                                                       *
*                                       1998.1  T.Kobayashi             *
*					2000.3  M.Tamura                *
*                                                                       *
*       usage:                                                          *
*               dawrite [ options ] infile1 infile2 ... > stdout        *
*       options:                                                        *
*               -s s  :  sampling frequency (8,10,12,16,32,48           *
*                                            11.025,22.05,44.1kHz)[10]  *
*               -c c  :  filename of low pass filter coef.   [Default]  *
*               -g g  :  gain (.., -2, -1, 0, 1, 2, ..)            [0]  *
*               -a a  :  amplitude gain (0..100)                 [N/A]	*
*               -o o  :  output port                               [s]  *
*                          s (speaker)    h (headphone)                 *
*               -H H  :  header size in byte                       [0]  *
*               -v    :  display filename                      [FALSE]  *
*               +x    :  data format                               [s]  *
*                          s (short)      f (float)                     *
*       infile:                                                         *
*               data                                           [stdin]  *
*       notice:                                                         *
*               number of infile < 128                                  *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id: dawrite.c,v 1.5 2002/12/25 05:29:03 sako Exp $";

/* Standard C Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "da.h"

typedef enum _Boolean {FA, TR} Boolean;
char *BOOL[] = {"FALSE","TRUE"};


/* Default Value */

#define	SIZE		256*400
#define	MAXFILES	128
#define	INITGAIN	0

#define OUTPORT		's'
#define GAIN		(0+INITGAIN)
#define HEADERSIZE	0
#define VERBOSE		FA

/* Command Name */
char *cmnd;

void usage(int status)
{
	fprintf(stderr, "\n");
	fprintf(stderr, " %s - play 16-bit linear PCM data\n\n",cmnd);
	fprintf(stderr, "  usage:\n");
	fprintf(stderr, "       %s [ options ] infile1 infile2 ... > stdout\n", cmnd);
	fprintf(stderr, "  options:\n");
	fprintf(stderr, "       -s s  : sampling frequency (%skHz) [%d]\n", AVAILABLE_FREQ, DEFAULT_FREQ);
	fprintf(stderr, "       -g g  : gain (..,-2,-1,0,1,2,..)                  [%d]\n",GAIN);
	fprintf(stderr, "       -a a  : amplitude gain (0..100)                   [N/A]\n");
#ifdef SPARC
	fprintf(stderr, "       -o o  : output port                               [%c]\n",OUTPORT);
	fprintf(stderr,	"                  s(speaker)    h(headphone)\n");
#endif /* SPARC */
	fprintf(stderr, "       -H H  : header size in byte                       [%d]\n",HEADERSIZE);
	fprintf(stderr, "       -v    : display filename                          [%s]\n",BOOL[VERBOSE]);
	fprintf(stderr, "       -w    : byteswap                                  [FALSE]\n");
	fprintf(stderr, "       +x    : data format                               [s]\n");
	fprintf(stderr, "                  s(short)    f(float)\n");
	fprintf(stderr, "       -h    : print this message\n");
	fprintf(stderr, "  infile:\n");
	fprintf(stderr, "       data                                              [stdin]\n");
	fprintf(stderr, "  notice:\n");
	fprintf(stderr, "       number of infile < %d\n",MAXFILES);
	fprintf(stderr, "\n");
	exit(status);
}

static char	outport = OUTPORT;
static short	*y = NULL, *xs;
static int	gain = GAIN, is_verbose = VERBOSE;
static int	hdr_size = HEADERSIZE, data_size = sizeof(short);
static int	freq = DEFAULT_FREQ;
static int	fleng, indx = 0;
static float	*x, fgain = 1;
float		ampgain = -1;
int		byteswap = 0;
size_t		abuf_size;

#if defined(LINUX) || defined(FreeBSD)
int	org_vol, org_channels, org_precision, org_freq;
#endif /* LINUX or FreeBSD */

#if defined(SOLARIS) || defined(SUNOS)
audio_info_t	org_data;
#endif /* SOLARIS */
void		reset_audiodev();

int main(argc,argv)
int	argc;
char	*argv[];
{
	FILE	*fp, *fopen();
	char	*s, *infile[MAXFILES], c, *getenv();
	int	i, nfiles = 0;
	void	sndinit(), direct();
	double	atof();

	if((s = getenv("DA_FLOAT")) != NULL)
		data_size = sizeof(float);
	if((s = getenv("DA_SMPLFREQ")) != NULL)
		freq = (int)(1000*atof(s));
	if((s = getenv("DA_GAIN")) != NULL)
		gain = atoi(s) + INITGAIN;
	if((s = getenv("DA_AMPGAIN")) != NULL)
		ampgain = atof(s);
	if((s = getenv("DA_PORT")) != NULL)
		outport = *s;
	if((s = getenv("DA_HDRSIZE")) != NULL)
		hdr_size = atoi(s);

	if (( cmnd = strrchr(argv[0], '/')) == NULL)
	    cmnd = argv[0];
	else
	    cmnd++;

	while(--argc)
	    if(*(s = *++argv) == '-') {
		c = *++s;
		switch(c) {
		    case 's':
			freq = (int)(1000*atof(*++argv));
			--argc;
			break;
	  	    case 'g':
			gain = atoi(*++argv) + INITGAIN;
			--argc;
			break;
		    case 'a':
			ampgain = atof(*++argv);
			--argc;
			break;
	 	    case 'H':
			hdr_size = atoi(*++argv);
			--argc;
			break;
		    case 'v':
			is_verbose = 1 - is_verbose;
			break;
		    case 'w':
			byteswap = 1;
			break;
		    case 'o':
			outport = **++argv;
			--argc;
			break;
		    case 'h':
			usage(0);
		    default:
			fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
			usage(1);
		}
	    } else if ( *s == '+') {
		c = *++s;
		switch(c) {
		    case 's':
			data_size = sizeof(short);
			break;
		    case 'f':
			data_size = sizeof(float);
			break;
		    default:
			fprintf(stderr, "%s : Invalid option '%c' !\n",cmnd, *(*argv+1));
			usage(1);
		}
	    }
	    else{
		if (nfiles < MAXFILES)
		    infile[nfiles++] = s;
		else{
	  	    fprintf(stderr, "%s: Number of files exceed %d\n", cmnd, MAXFILES);
		    exit(1);
		}
	}

	if((x = (float *)calloc(SIZE, sizeof(float))) == NULL) {
		fprintf(stderr, "%s: cannot allocate memory\n", cmnd);
		exit(1);
	}
	xs = (short *)x;
	if((y = (short *)calloc(SIZE*2, sizeof(float))) == NULL) {
		fprintf(stderr, "%s: cannot allocate memory\n", cmnd);
		exit(1);
	}

		sndinit();
		i = (gain < 0) ? -gain : gain;
		while(i--)
			fgain *= 2;
		if(gain < 0)
			fgain = 1 / fgain;

	if (nfiles) {
		for(i = 0; i < nfiles; ++i) {
			if((fp = fopen(infile[i], "r")) == NULL) {
				fprintf(stderr, "%s: cannot open %s\n", cmnd, infile[i]);
			} else {
				if(is_verbose) {
					fprintf(stderr, "%s: %s\n", cmnd, infile[i]);
				}
				direct(fp);
				fclose(fp);
			}
		}
	}
	else direct(stdin);

	fclose( adfp);
	close( ACFD);

	reset_audiodev();
	exit(0);
}

void direct(fp)
FILE	*fp;
{
	int	k, nread;
	double	d;
	void	sndout();

	if(hdr_size) fseek(fp, (long)hdr_size, 0);

	while(nread = fread(x, data_size, SIZE, fp)) {
		for(k = 0; k < nread; ++k) {
			if(data_size == sizeof(float))
				d = x[k];
			else
				d = *(xs + k);
			y[k] = d * fgain;
		}
		if( byteswap > 0) byteswap_vec( y, sizeof(short), nread);
		sndout(nread);
	}
}


void sndinit()
{
	int	port, dtype;
	void	init_audiodev();
	void	change_play_gain(), change_output_port();

	switch(freq) {	
	case 8000:
		dtype =_8000_16BIT_LINEAR;
		break;
	case 11000:
		dtype =_11025_16BIT_LINEAR;
		break;
	case 11025:
		dtype =_11025_16BIT_LINEAR;
		break;
	case 16000:
		dtype =_16000_16BIT_LINEAR;
		break;
	case 22000:
		dtype =_22050_16BIT_LINEAR;
		break;
	case 22050:
		dtype =_22050_16BIT_LINEAR;
		break;
	case 32000:
		dtype =_32000_16BIT_LINEAR;
		break;
	case 44000:
		dtype =_44100_16BIT_LINEAR;
		break;
	case 44100:
		dtype =_44100_16BIT_LINEAR;
		break;
	case 48000:
		dtype =_48000_16BIT_LINEAR;
		break;
	default:
		fprintf(stderr,"%s: unavailable sampling frequency\n", cmnd);
		exit(1);
	}
	init_audiodev(dtype);

	if(ampgain >= 0){
		if( ampgain > 100) ampgain = 100;
		change_play_gain(ampgain);
	}
#ifdef SPARC
	if(outport == 's')
		port = SPEAKER;
	else if(outport == 'h')
		port = HEADPHONE;
	change_output_port( port | LINE_OUT );
#endif /* SPARC */
}

void sndout(leng)
int	leng;
{
	fwrite( y, sizeof(short), leng, adfp);
	write( ADFD, y, 0);
}

void init_audiodev(dtype)
int	dtype;
{
#if defined(LINUX) || defined(FreeBSD)
	int arg;

	if( (adfp = fopen( AUDIO_DEV, "w")) == NULL){
		fprintf( stderr, "%s: can't open audio device\n", cmnd);
		exit(1);
	}
#ifdef LINUX
	ADFD = adfp->_fileno;
#else /* FreeBSD */
	ADFD = adfp->_file;
#endif
	ACFD = open( MIXER_DEV, O_RDWR, 0);

	ioctl(ADFD, SNDCTL_DSP_GETBLKSIZE, &abuf_size);
	ioctl(ADFD, SOUND_PCM_READ_BITS, &org_precision);
	ioctl(ADFD, SOUND_PCM_READ_CHANNELS, &org_channels);
	ioctl(ADFD, SOUND_PCM_READ_RATE, &org_freq);
	ioctl(ACFD, SOUND_MIXER_READ_PCM, &org_vol);
	
	arg = data_type[dtype].precision;
	ioctl(ADFD, SOUND_PCM_WRITE_BITS, &arg);
/*	arg = data_type[dtype].channel; */
	arg = 0;
	ioctl(ADFD, SOUND_PCM_WRITE_CHANNELS, &arg);
	arg = data_type[dtype].sample;
	ioctl(ADFD, SOUND_PCM_WRITE_RATE, &arg);
#endif /* LINUX || FreeBSD */

#ifdef SPARC
	audio_info_t	data;

	ACFD = open(AUDIO_CTLDEV, O_RDWR, 0);
	adfp = fopen(AUDIO_DEV, "w");
	ADFD = adfp->_file;

	AUDIO_INITINFO(&data);
	ioctl(ACFD, AUDIO_GETINFO, &data);
	bcopy( &data, &org_data, sizeof( audio_info_t));

	data.play.sample_rate = data_type[dtype].sample;
	data.play.precision   = data_type[dtype].precision;
	data.play.encoding    = data_type[dtype].encoding;

	ioctl(ADFD,AUDIO_SETINFO,&data);
#endif /* SPARC */
}

void change_output_port(port)
unsigned port;
{
#ifdef LINUX
	
#endif /* LINUX */

#ifdef SPARC
	audio_info_t	data;

	AUDIO_INITINFO(&data);
	ioctl(ACFD, AUDIO_GETINFO, &data);

	data.play.port=port;	
	
	ioctl(ACFD, AUDIO_SETINFO, &data);
#endif /* SPARC */
}

void change_play_gain(volume)
float volume;
{
	int vol, arg;

#if defined(LINUX) || defined(FreeBSD)
	vol = (int) ((MAXAMPGAIN*volume)/100);
	
	arg = vol | (vol << 8 );
	ioctl( ACFD, MIXER_WRITE(SOUND_MIXER_PCM), &arg);
#endif /* LINUX */

#ifdef SPARC
	audio_info_t	data;

	vol = (int) ((MAXAMPGAIN*volume)/100);
	AUDIO_INITINFO(&data);
	ioctl(ACFD, AUDIO_GETINFO, &data);

	data.play.gain=vol;

	ioctl(ACFD, AUDIO_SETINFO, &data);
#endif /* SPARC */
}

void reset_audiodev()
{

#if defined(LINUX) || defined(FreeBSD)
	ACFD = open( MIXER_DEV, O_RDWR, 0);
	ADFD = open( AUDIO_DEV, O_RDWR, 0);

	ioctl(ADFD, SOUND_PCM_WRITE_BITS, &org_precision);
	ioctl(ADFD, SOUND_PCM_WRITE_CHANNELS, &org_channels);
	ioctl(ADFD, SOUND_PCM_WRITE_RATE, &org_freq);
	ioctl(ACFD, SOUND_MIXER_WRITE_PCM, &org_vol);

	close( ADFD);
	close( ACFD);
#endif /* LINUX or FreeBSD */

#ifdef SPARC
	ACFD = open(AUDIO_CTLDEV, O_RDWR, 0);
	ioctl( ACFD, AUDIO_SETINFO, &org_data);
	close( ACFD);
#endif /* SPARC */
}

int byteswap_vec( vec, size, blocks)
void *vec;
int size;
int blocks;
{
	char *q;
	register char t;
	int i, j;

	q = (char *)vec;
	for( i = 0; i < blocks; i++){
		for( j = 0; j < (size/2); j++){
			t = *(q+j);
			*(q+j) = *(q+(size-1-j));
			*(q+(size-1-j)) = t;
		}
		q += size;
	}

	return i;		/* number of blocks */
}
