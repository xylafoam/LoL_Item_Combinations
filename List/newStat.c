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

#define IN_FILE "sList.txt"
#define OUT_FILE_EXT ".txt"


int main(void)
{
	char name[NAME * 2];
	float stat = 0; 
	int ch, i;
	FILE * fifi, *fofo;
	char outFile[STATLEN+4];
	//char header[STATLEN];
	int row;
	
	
	
	fifi = openfile ( NULL, IN_FILE );
	if( fifi == NULL )
		exit(EXIT_FAILURE);
	
	
	fputs( "Enter the name, in abbreviated form (1-3 letters),\n", stdout );
	fputs( "of the stat the items have in common: ", stdout );
	s_gets ( outFile, STATLEN, stdin );
	//s_gets ( header, STATLEN, stdin );
	
	for( i = 0; outFile[i] != '\0'; i++ )
		outFile[i] = (char) toupper( (int) outFile[i] );
	
	//strncpy( outFile, header, STATLEN );
	//outFile[STATLEN - 1] = '\0';
	
	if( (size_t) sizeof outFile - strlen( outFile ) < strlen(OUT_FILE_EXT) + (size_t) 1 )
	{
		fputs( "Can't strcat\n", stderr );
		exit(EXIT_FAILURE);
	}
	strcat( outFile, OUT_FILE_EXT );
	
	fofo = emptyfile( outFile );
	if( fofo == NULL )
		exit(EXIT_FAILURE);
	
	do
	{
		fputs("Enter row to get value from: ", stdout);
		
		
	}while( scanf( "%d", &row ) != 1 || row <= 0 );
	row--;
	
	
	//fputs ( header, fofo );
	//fputc ( '\n', fofo );
	
	while( checkEOF( fifi ) != 1 )
	{
		getname( fifi, name, NAME * 2);
		
		for (i = 0; i < row; i++)
			discard ( fifi );
		
		fscanf( fifi, "%f", &stat );
		
		while ( isblank(ch = getc(fifi)) )
			continue;
		if( ch == '%' )
			stat /= 100.0f;
		else
			ungetc( ch, fifi );
		
		while ((ch = getc(fifi)) != '\n' && ch != EOF)
			continue;
		
		cleanstringend ( name );
		name[strlen(name)/2] = '\0';
		cleanstringend ( name );
		
		fprintf( fofo, "%s %.2f\n", name, stat );
	}
	
	fclose(fifi);
	fclose(fofo);
	
	fputs( "Output placed in file \"", stdout );
	fputs( outFile, stdout );
	fputs( "\"\n", stdout);
	
	return 0;
}
