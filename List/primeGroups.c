// SPDX-License-Identifier: BSD-3-Clause
// 
// Copyright (c) 2024 Timothy Farr
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of the author nor the names of its contributors may
//    be used to endorse or promote products derived from this software
// 
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


#include "funcs.h"
#include "items.h"
#include "primes.h"



int main(void)
{
	char name[NAME * 2];
	int ch;
	FILE * fifi, *fofo;
	int primecount;
	
	
	
	fifi = openfile ( "Enter name of file to format: ", NULL );
	if( fifi == NULL )
			exit(EXIT_FAILURE);
	
	
	fofo = emptyfile( GROUPS_FILE );
	if( fofo == NULL )
			exit(EXIT_FAILURE);
	
	//for(primecount = 0; primecount < IGROUPS; primecount++)
		//printf( "%d\n", primes[primecount] );
	
	while ( isspace( ch = getc(fifi) ) || ch == '\r' )
			continue;
	ungetc(ch, fifi);
	
	
	
	primecount = 0;
	while ( checkEOF( fifi ) != 1 && primecount < IGROUPS)
	{
		
		while( (ch = getc(fifi)) != '\n' && ch != EOF )
			continue;
		
		fprintf( fofo, "%d\n", primes[primecount] );
		primecount++;
		
		while( (ch = getname( fifi, name, NAME * 2)) != '\n' && ch != EOF )
		{
			cleanstringend ( name );
			name[strlen(name)/2] = '\0';
			cleanstringend ( name );
			fputs (name, fofo);
			fputs ("\n", fofo);
			
			clearline ( fifi );
		}
		fputs ( "\n", fofo );
	
	}
	
	
	fclose(fifi);
	fclose(fofo);
	
	fofo = emptyfile( NOP_FILE );
	if( fofo == NULL )
		exit(EXIT_FAILURE);
	
	fprintf( fofo, "%d", primecount );
	
	fclose(fofo);
	
	puts( "Output placed in file \"" GROUPS_FILE "\"");
	puts( "Number of primes used placed in file \"" NOP_FILE "\"");
	return 0;
}
