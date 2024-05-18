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
#include "items.h"
#include "primes.h"


#define OUT_FILE_EXT ".txt"
#define FINNAME "stats.txt"

int main(int argc, char *argv[])
{
	char nlist[ITEMS][NAME];
	float istats[ITEMS][STATS] = {0};
	float gold[ITEMS] = {0};
	int igroups[ITEMS];
	
	FILE * fp;
	
	char name[NAME];
	float tmpVal;
	
	int i, j, match, ch;
	int items, nstats;
	int gtmp;
	
	char stats[STATS][STATLEN];
	char Fname[STATLEN+4];
	
	int goldopt, groupopt;
	
	
	
	
	for(i = 0; i < ITEMS; i++)
		igroups[i] = 1;
	
	
	fp = openfile( NULL, FINNAME );
	if( fp == NULL )
			exit(EXIT_FAILURE);
	
	checkEOF( fp );
	for( nstats = 0; nstats < STATS && getstattype( fp, stats[nstats], STATLEN ); nstats++ )
	{
		for( j = 0; stats[nstats][j] != '\0'; j++ )
			stats[nstats][j] = (char) toupper( (int) stats[nstats][j] );
	}
	fclose( fp );
	
	for( i = 0; i < nstats; i++)
	{
		for( j = 0; j < i; j++)
			if( strcmp( stats[j], stats[i] ) == 0 )
			{
				fputs("Stat \"", stdout);
				fputs(stats[j], stdout);
				fputs("\" included more than once\nin file \"", stdout);
				fputs(FINNAME, stdout);
				fputs("\"\n", stdout);
				
				exit(EXIT_FAILURE);
			}
	}
	
	for( i = 0, items = 0; i < nstats && items < ITEMS; i++ )
	{
		// get files
		strncpy( Fname, stats[i], STATLEN );
		Fname[STATLEN - 1] = '\0';
		
		if( (size_t) sizeof Fname - strlen( Fname ) < strlen(OUT_FILE_EXT) + (size_t) 1 )
		{
			fputs( "Can't strcat\n", stderr );
			exit(EXIT_FAILURE);
		}
		strcat( Fname, OUT_FILE_EXT );
		
		
		fp = openfile ( NULL, Fname );
		if( fp == NULL )
			exit(EXIT_FAILURE);
		
		
		// clear header
		//while( getc( fp ) != '\n' )
			//continue;
		
		//get items
		while( checkEOF( fp ) != 1 && items < ITEMS  )
		{
			getname( fp, name, NAME);
			
			fscanf( fp, "%f", &tmpVal );
			
			for(j = 0, match = 1; j < items; j++)
			{
				if(strncmp(name, nlist[j], NAME) == 0)
				{
					match = 0;
					break;
				}
			}
			if(match)
			{
				strncpy(nlist[items], name, NAME);
				nlist[items][NAME - 1] = '\0';
				
				istats[items][i] = tmpVal;
				items++;
			}
			else
			{
				istats[j][i] = tmpVal;
				
				
			}
			
		}
		
		
		
		fclose( fp );
	}
	// get files
	
	
	fputs("Add gold values? (enter a number)\n0) No  1) Yes : ", stdout);
	while( scanf("%d", &goldopt) != 1 || goldopt < 0 || goldopt > 1 )
	{
		while( getchar() != '\n' )
			continue;
		fputs("0) No  1) Yes : ", stdout);
		
	}
	while( getchar() != '\n' )
		continue;
	
	if(goldopt)
	{	
		fp = openfile ( "Enter name of gold file: ", NULL );
		if( fp == NULL )
			exit(EXIT_FAILURE);
		
		while( checkEOF( fp ) != 1 )
		{
			getname( fp, name, NAME );
			 
			fscanf( fp, "%f", &tmpVal );
			
			while( (ch = getc(fp)) != '\n' && ch != EOF )
				continue;
			
			for(i = 0; i < items; i++)
				if( strncmp(name, nlist[i], NAME) == 0 )
					gold[i] = tmpVal;
		}
			
		
		fclose(fp);
	}
	
	//        ADD MYTHIC STATUS TO ITEMS 
	
	fputs("Add group column? (enter a number)\n0) No  1) Yes : ", stdout);
	while( scanf("%d", &groupopt) != 1 || groupopt < 0 || groupopt > 1 )
	{
		while( getchar() != '\n' )
			continue;
		fputs("0) No  1) Yes : ", stdout);
		
	}
	while( getchar() != '\n' )
		continue;
	
	if(groupopt)
	{
		fp = openfile ( NULL, GROUPS_FILE );
		if( fp == NULL )
			exit(EXIT_FAILURE);
		
		
		while ( checkEOF( fp ) != 1 )
		{
			fscanf( fp, "%d", &gtmp);
			
			clearline( fp );
			
			
			//printf("prime: %d\n", gtmp);
			
			while( (ch = getname( fp, name, NAME)) != '\n' && ch != EOF )
			{
				for(j = 0; j < items; j++)
					if( strncmp(name, nlist[j], NAME) == 0 )
						igroups[j] *= gtmp;
				
				clearline ( fp );
			}
			
			
			
		}
		
		
		fclose(fp);
	}
	
	
	
	
	//        OUTPUT
	fp = emptyfile( ITEMS_FILE );
	if( fp == NULL )
			exit(EXIT_FAILURE);
		
	for ( j = 0; j < nstats; j++ )
		fprintf(fp, "%s(f) ", stats[j]);
	
	if ( goldopt )
			fprintf(fp, "GLD(f) ");
	
	if ( groupopt )
			fprintf(fp, "GRP(i) ");
		
	fprintf(fp, "\n");
	
	
	for(i = 0; i < items; i++)
	{
		fprintf(fp, "%s", nlist[i]);
		
		for(j = 0; j < nstats; j++)
			fprintf(fp, " %.2f", istats[i][j]);
		
		if ( goldopt )
			fprintf(fp, " %.0f", gold[i]);
		
		if ( groupopt )
			fprintf(fp, " %d", igroups[i]);
		
		putc('\n', fp);
	}
	
	fclose(fp);
	
	
	
	
	
	// 
	/*chnum = 1;
	match = 1;
	while ( match )
	{
		match = 0;
		
		for ( i = 0; i < items; i++ )
		{
			for ( j = i + 1; j < items; j++ )
			{
				if ( strncmp(nlist[i], nlist[j], chnum) == 0 )
				{
					match = 1;
					break;
				}
				if ( match )
					break;
			}
		}
		
		if ( match )
			chnum++;
	}
	printf ( "unique characters: %d\n", chnum );
	*/
	
	
	puts( "Output placed in file \"" ITEMS_FILE "\"");
	
	return 0;
}
