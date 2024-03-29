#! /bin/csh -f
# ----------------------------------------------------------------
#       Speech Signal Processing Toolkit (SPTK): version 3.0
# 		       SPTK Working Group
# 
# 		 Department of Computer Science
# 		 Nagoya Institute of Technology
# 			      and
#   Interdisciplinary Graduate School of Science and Engineering
# 		 Tokyo Institute of Technology
# 		    Copyright (c) 1984-2000
# 		      All Rights Reserved.
# 
# Permission is hereby granted, free of charge, to use and
# distribute this software and its documentation without
# restriction, including without limitation the rights to use,
# copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of this work, and to permit persons to whom this
# work is furnished to do so, subject to the following conditions:
# 
#   1. The code must retain the above copyright notice, this list
#      of conditions and the following disclaimer.
# 
#   2. Any modifications must be clearly marked as such.
#                                                                        
# NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF TECHNOLOGY,
# SPTK WORKING GROUP, AND THE CONTRIBUTORS TO THIS WORK DISCLAIM
# ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
# SHALL NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF
# TECHNOLOGY, SPTK WORKING GROUP, NOR THE CONTRIBUTORS BE LIABLE
# FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
# DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
# WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
# ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
# PERFORMANCE OF THIS SOFTWARE.
# ----------------------------------------------------------------


#########################################################################
#									#
#   Draw a Log Spectrum Graph						#
#									#
#						1988.5  K.Tokuda	#
#						1996.6  K.Koishida	#
#									#
#########################################################################

set path	= ( /usr/local/SPTK/bin $path )

set cmnd	= $0
set cmnd	= $cmnd:t

set h		= 0.6
set w		= 0.6
@ on		= 1
@ x		= 1
@ ymin		= 0
@ ymax		= 100
@ yscale	= 20
set yy		= ("" "")
set p		= 1
set ln		= 1
@ g		= 2
set file

@ s		= 0
@ l		= 256

set help	= 0

set xname = ( 	"Normalized frequency" \
		"Normalized frequency (rad)" "" \
		"Frequency (kHz)" \
		"Frequency (kHz)" "" "" \
		"Frequency (kHz)" "" \
		"Frequency (kHz)" )
set xs = (	"0 0.25 0.5" \
		0\ \'1.57\ \"\\pi/2\"\ \'3.14\ \"\\pi\" ""\
		"0 1 2 3 4" \
		"0 1 2 3 4 5" "" ""\
		"0 2 4 6 8" "" \
		"0 2 4 6 8 10" )
set xx = ("0 0.5" "0 3.14" "" "0 4" "0 5" "" "" "0 8" "" "0 10")
set xl = (0.5 3.14 "" 4 5 "" "" 8 "" 10)

set on_x = (40 125 40 125 40 125)
set on_y = (205 205 120 120 35 35)

@ i = 0
while ($i < $#argv)
	@ i++
	switch ($argv[$i])
	case -f:
		@ i++
		set f = $argv[$i]
		breaksw
	case -v:
		set v = over
		breaksw
	case -H:
		@ i++
		set h = $argv[$i]
		breaksw
	case -W:
		@ i++
		set w = $argv[$i]
		breaksw
	case -o:
		@ i++
		@ xo = $argv[$i]
		@ i++
		@ yo = $argv[$i]
		breaksw
	case -O:
		@ i++
		@ on = $argv[$i]
		breaksw
	case -x:
		@ i++
		@ x = $argv[$i]
		breaksw
	case -y:
		@ i++
		@ ymin = $argv[$i]
		@ i++
		@ ymax = $argv[$i]
		breaksw
	case -ys:
		@ i++
		@ yscale = $argv[$i]
		breaksw
	case -p:
		@ i++
		set p = $argv[$i]
		breaksw
	case -ln:
		@ i++
		set ln = $argv[$i]
		breaksw
	case -c:
		@ i++
		set c = "$argv[$i]"
		breaksw
	case -g:
		@ i++
		@ g = $argv[$i]
		breaksw
	case -s:
		@ i++
		set s = $argv[$i]
		breaksw
	case -l:
		@ i++
		@ l = $argv[$i]
		breaksw
	case -help:
		set help = 1
	case -h:
		set exit_status = 0
		goto usage
		breaksw
	default:
		set file = $argv[$i]
		if ( ! -f $file ) then
			echo2 "${cmnd}: Can't open file "'"'"$file"'"'" \!"
			set exit_status = 1
			goto usage
		endif
		breaksw
	endsw
end
goto main

usage:
	echo2 ''
	echo2 " $cmnd - draw a log spectrum graph"
	echo2 ''
	echo2 '  usage:'
	echo2 "       $cmnd [ options ] [ infile ] > stdout"
	echo2 '  options:'
	echo2 '       -O  O         : origin of graph              [1]    '
if ( $help ) then
	echo2 '                        1 ( 40, 205) <mm> '
	echo2 '                        2 (125, 205) <mm>   +--------+'
	echo2 '                        3 ( 40, 120) <mm>   | 1    2 |'
	echo2 '                        4 (125, 120) <mm>   | 3    4 |'
	echo2 '                        5 ( 40,  35) <mm>   | 5    6 |'
	echo2 '                        6 (125,  35) <mm>   +--------+'
endif
	echo2 '       -x  x         : x scale                      [1]'
if ( $help ) then
	echo2 '                        1  (Normalized frequency <0, 0.5>)'
	echo2 '                        2  (Normalized frequency <0, pi/2, pi>)'
	echo2 '                        4  (Frequency <0, 1, 2, 3, 4     kHz>)'
	echo2 '                        5  (Frequency <0, 1, 2, 3, 4, 5  kHz>)'
	echo2 '                        8  (Frequency <0, 2, 4, 6, 8     kHz>)'
	echo2 '                        10 (Frequency <0, 2, 4, 6, 8, 10 kHz>)'
endif
	echo2 '       -y  ymin ymax : y scale                      [0 100]'
	echo2 '       -ys ys        : Y-axis scaling factor        [20]'
	echo2 '       -p  p         : pen no.                      [1]'
	echo2 '       -ln ln        : line number                  [1]'
	echo2 '       -s  s         : start frame number           [0]'
	echo2 '       -l  l         : frame length                 [256]'
	echo2 '       -c  "c"       : coment for the graph         [""]'
	echo2 '       -h            : print this message'
	echo2 '       -help         : print help in detail'
if ( $help ) then
	echo2 '       (level 2)'
	echo2 '       -W  W         : width of the graph <100mm>   [0.6]'
	echo2 '       -H  H         : height of the graph <100mm>  [0.6]'
	echo2 '       -v            : over write mode              [FALSE]'
	echo2 '       -o  xo yo     : origin of the graph          [40, 205]'
	echo2 '                      (if -o option exists, -O is not effective)'
	echo2 '       -g  g         : type of the graph            [2]'
	echo2 '                        0 (no frame)'
	echo2 '                        1 (half frame)'
	echo2 '                        2 (with frame)'
	echo2 '       -f  file      : additional data file for fig [NULL]'
endif
	echo2 '  infile:'
	echo2 '       log spectrum (float)                         [stdin]'
	echo2 '  stdout:'
	echo2 '       XY-plotter command'
	echo2 ''
exit $exit_status

main:
if (! $?xo) then
	@ xo = $on_x[$on]
	@ yo = $on_y[$on]
endif

@ yy[1] = $ymin
@ yy[2] = $ymax

set ys = ($ymin)

@ i = 1
while (($ys[$i] + $yscale) <= $ymax)
  @ tmp = $ys[$i] + $yscale
  set ys = ($ys $tmp)
  @ i++
end

if ($?c) then
	# echo "$c"
	@ yoc = $yo - 8
	fig -W $w -H $h -o $xo $yoc -g 0 << EOF
		xname "$c"
EOF
endif

fig -W $w -H $h -o $xo $yo -g $g << EOF
	x $xx[$x]
	y $yy
	xscale $xs[$x]
	xname "$xname[$x]"
	yscale $ys
	yname "Log magnitude (dB)"

	line 2
	0 0
	$xl[$x] 0
	eod

EOF

if ($?f) then
	fig $f -W $w -H $h -o $xo $yo -g 0
endif

@ lhi = $l / 2 + 1
@ s = $s * $lhi
if ($?v) then
	set ee = ""
else
	@ e = $s + $lhi - 1
	set ee = "-e $e"
endif

bcut -s $s $ee +f $file |\
fdrw -W $w -H $h -o $xo $yo -g 0 -y $yy -n $lhi -p $p -m $ln
