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
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "funcs.h"



FILE * openfile ( const char * instr, const char * optName )
{
	FILE * fifi;
	char fname[FLEN];
	const char *useName;
	
	if( optName == NULL )
	{
		if( instr != NULL )
		{
			fputs (instr, stdout);
		}
		else
			fputs ("Enter file name: ", stdout);
		
		s_gets(fname, FLEN, stdin);
		cleanstringend ( fname );
		useName = fname;
	}
	else
	{
		useName = optName;
	}
	
	if( (fifi = fopen(useName, "rb")) == NULL )
	{
		fputs("can't open file \"", stdout);
		fputs(useName, stdout);
		fputs("\"\n", stdout);
	}
	
	return fifi;
}

FILE * emptyfile( const char * fname )
{
	FILE * fofo;
	
	if( (fofo = fopen(fname, "wb")) == NULL )
	{
		fputs("Can't create empty file \"", stdout);
		fputs(fname, stdout);
		fputs("\"\n", stdout);
	}
	
	return fofo;
}

		// Get string up to (n-1) characters or until newline char is found NULL char is appended
char * s_gets(char * st, int n, FILE * stream)
{
	char * ret_val;
	char * find;
	
	
	ret_val = fgets(st, n, stream);
	if(ret_val == st)
	{
		
		find = strchr(st, '\n');
		if(find)
			*find = '\0';
		else
			while(getc(stream) != '\n')
				continue;
		
		find = strchr(st, '\r');
		if(find)
			*find = '\0';
	}
	
	
	return ret_val;
}

int getname(FILE * fifi, char * name, int n)
{
	int retval, i = 0;
	
	
	if( n > 1 )
	{
		while( isblank(name[0] = getc(fifi)) || name[0] == '\r' )
				continue;
		
		retval = name[0];
		
		
		while( ( name[i] > '9' || name[i] < '0' ) && name[i] != EOF && name[i] != '\n' && name[i] != '-' && name[i] != '+' && i < n - 1 )
		{
			i++;
			name[i] = getc(fifi);
		}
		
		ungetc ( name[i], fifi );
		
		name[i] = '\0';
		

		cleanstringend ( name );
	}
	//printf( "%s %zd\n", name, strlen(name) );
	
	
	return retval;
}

void discard ( FILE * fifi )
{
	int ch;
	
	ch = getc ( fifi );
	while ( isblank ( ch ) )
		ch = getc ( fifi );
	
	while ( ! isspace( ch ) && ch != EOF )
		ch = getc ( fifi );
}


void clearline ( FILE * fifi )
{
	int ch;
	
	
	while ( ( ch = getc(fifi) ) != '\n' && ch != EOF )
		continue;
}

void cleanstringend ( char * st )
{
	while( isspace( st[strlen(st) - 1] ) || st[strlen(st) - 1] == '\r' )
		st[strlen(st) - 1] = '\0';
}

int checkEOF( FILE *fp )
{
	int found = 0;
	int ch;
	
	if ( fp != NULL )
	{
		while( isspace(ch = fgetc( fp )) || ch == '\r' )
			continue;
		
		if( ch == EOF )
			found = 1;
		else
			ungetc( ch, fp );
	}
	else
		found = 1;
	
	
	return found;
}

int getstattype( FILE * infile, char * nlist, int n )
{
	int ch, j;
	
	ch = getc( infile );
	while(  isblank( ch ) || ch == '\r' )
		ch = getc( infile );
	
	j = 0;
	while(  isalpha( ch ) && j < n - 1  )
	{
		nlist[j] = ch;
		j++;
		
		ch = getc( infile );
	}
	nlist[j] = '\0';
	
	ungetc( ch, infile );
	
	
	return j;
}
