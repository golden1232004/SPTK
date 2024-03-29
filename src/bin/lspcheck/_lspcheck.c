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

    $Id: _lspcheck.c,v 1.2 2002/12/25 05:31:12 sako Exp $

    Check order of LSP

	int lspcheck(lsp, ord)

	double   	*lsp : LSP
	int    	ord  : order of LSP

	return value : 0 -> normal
		       -1-> ill condition

*****************************************************************/

int lspcheck(lsp, ord)
double *lsp;
int ord;
{
    register int	i;
    
    if(lsp[0] <=0.0 || lsp[0] >= 0.5) 
	return(-1);

    for(i=1; i<ord; i++){
	if(lsp[i] <= lsp[i-1]) 
	    return(-1);
	if(lsp[i] <= 0.0 || lsp[i] >= 0.5)
	    return(-1);
    }
    return(0);
}

/****************************************************************

    $Id: _lspcheck.c,v 1.2 2002/12/25 05:31:12 sako Exp $

    Rearrangement of LSP

	void lsparng(lsp, ord)

	double   	*lsp : LSP
	int    	ord  : order of LSP

*****************************************************************/

void lsparrange(lsp, ord)
double *lsp;
int ord;
{
    register int	i, flag;
    double 		tmp;
    
    /* check out of range */
    for(i=0; i<ord; i++){
	if(lsp[i] < 0.0) 
	    lsp[i] = -lsp[i];
	if(lsp[i] > 0.5) 
	    lsp[i] = 1.0 - lsp[i];
    }
    
    /* check unmonotonic */
    for(;;){
	flag = 0;
	for(i=1; i<ord; i++)
	    if(lsp[i] < lsp[i-1]){
		tmp = lsp[i];
		lsp[i] = lsp[i-1];
		lsp[i-1] = tmp;
		flag = 1;
	    }

	if(! flag) break;
    }
}

