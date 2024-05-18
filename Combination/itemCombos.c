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
#include <ctype.h>
#include <stdlib.h>
#include <float.h>


#include "funcs.h"
#include "items.h"
#include "exprlist.h"
#include "varstruct.h"
#include "vararray.h"
#include "primes.h"


#define RESULTS_FILE "result.txt"


int PromptFunc( char opt1, char opt2, char *instruct );

//navori
//float SumT( float target, float atkSB );


typedef struct 
{
	exprrecipe rep;
	variable *pos[STATS+EXPRESSIONS];
	int varIndex;
	//int used[STATS+EXPRESSIONS];
} expression;


typedef	union{int ivalue; float fvalue;} ifunion;





int main( int argc, char *argv[] )
{
	int stattype;
	int statcount;
	char tmpstatname[STATLEN];
	
	char names[ITEMS][NAME];
	ifunion statsArray[STATS][ITEMS];
	int itemCount;
	
	int prompt;
	
	int * selected;
	int slot, i, slots, j; 
	
	int nprimes = 0;
	int lim = 1, conflict, confT; 
	int groupopt, *groupproduct, grouprow;
	int grpInit = 1;
	int skip;
	
	variable * curVar, *shiftVar;
	char tmpExpr[EXPR_SIZE];
	expression exprArray[EXPRESSIONS];
	int numExpr;
	int iExpr;
	int finExpr;
	
	// Navori variables
	//int tari, SBi;
	//float target, atkSB;
	
	// variablesArray
	// holds all variable types mentiond by item's file and expressions
	varType variablesArray[STATS+EXPRESSIONS];
	// usedVarsIndex
	// holds index number of variabls found in expressions
	// value of -1 indicates no index number here or later in the array
	int usedVarsIndex[STATS+EXPRESSIONS];
	float varsInitial[STATS];
	float statMin[STATS], statMax[STATS], statLimit[STATS];
	int fVars = 0, iVars = 0;
	
	float diff;
	float tottmp, total;
	float *small, *large;
	
	FILE *itemfile, *exprfile, *outputfile, *nprimesfile;
	


	// pre-checks
	if( argc == 2)
	{
		// expr
		exprfile = openfile ( NULL, EXPR_FILE );
		if( exprfile == NULL )
		{
			exit(EXIT_FAILURE);
		}
		
		
		// slots
		for( i = 0; i < strlen ( argv[1] ); i++ )
			if( ! isdigit ( argv[1][i] ) )
			{
				fputs( "Item slots argument must contain only digits\n", stdout);
				fputs( "./combos <Number of item slots>\n\n", stdout);
				exit(EXIT_FAILURE);
			}
		// slots
		slots = atoi ( argv[1] );
		if(slots <= 0)
		{
			fputs( "Item slots argument must be 1 or greater\n", stdout);
			fputs( "./combos <Number of item slots>\n\n", stdout);
			exit(EXIT_FAILURE);
		}
		// slots
		selected = malloc( slots * sizeof (*selected) );
		if( selected == NULL )
		{
			fputs( "Could not allocate space in memory\n\n", stdout);
			exit(EXIT_FAILURE);
		}
		
		
		// items
		itemfile = openfile ( NULL, ITEMS_FILE );
		if( itemfile == NULL )
		{
			exit(EXIT_FAILURE);
		}
		
		
	}
	else
	{
		fputs( "Incorrect number of arguments.\n", stdout);
		fputs( "Program usage:\n", stdout);
		fputs( "./combos <Number of item slots>\n\n", stdout);
		exit(EXIT_FAILURE);
	}
	
	
	// Initialize
	InitVarArray( variablesArray, STATS+EXPRESSIONS );
	
	for(i = 0; i < slots; i++)
		selected[i] = i;
	
	for(i = 0; i < STATS+EXPRESSIONS; i++)
		usedVarsIndex[i] = -1;
	
	//for(i = 0; i < EXPRESSIONS; i++)
		//for(j = 0; j < STATS+EXPRESSIONS; j++)
			//exprArray[i].used[j] = -1;
	
	for(i = 0; i < STATS; i++)
		statMax[i] = FLT_MAX;
	
	for(i = 0; i < STATS; i++)
		statLimit[i] = FLT_MAX;
	
	for(i = 0; i < STATS; i++)
		statMin[i] = -FLT_MAX;
	
	for(i = 0; i < STATS; i++)
		varsInitial[i] = 0.0f;
	
	for(i = 0; i < EXPRESSIONS; i++)
		for(j = 0; j < STATS+EXPRESSIONS; j++)
			exprArray[i].pos[j] = NULL;
			
	
	//for(i = 0; i < EXPRESSIONS; i++)
		//cmpVar[i][0] = -1;
	
	
	
	// search for largest or smallest 
	fputs( "Are you looking for the smallest or largest value? ", stdout);
	prompt = PromptFunc( 'S', 'L', "(Enter S or L ): " );
	if(prompt == 'S')
	{
		small = &tottmp;
		total = FLT_MAX;
		large = &total;
	}
	else
	{
		small = &total;
		total = -FLT_MAX;
		large = &tottmp;
	}
	
	fputs("Enter allowed difference: ", stdout);
	while(  scanf( "%f", &diff ) != 1 || diff < 0 )
	{
		while( getchar() != '\n' )
			continue;
		
		
		fputs("Enter allowed difference: ", stdout);
	}
	fputc( '\n', stdout );
	
	// Detect the number of stat columns the item file contains
	// Detect each stats' type
	statcount = 0;
	while ( statcount < STATS && getstattype( itemfile, tmpstatname, STATLEN) )
	{
		while( (stattype = getc( itemfile )) != '(' && stattype != EOF && stattype != '\n' )
			continue;
		while( !isalpha(stattype = getc( itemfile )) && stattype != EOF && stattype != '\n' ) 
			continue;
		
		if( stattype == 'f' )
			fVars++;
		else if(stattype == 'i' )
			iVars++;
		else
			fputs("stat missing a type\n", stdout);
		
		if( ! AddVar( variablesArray, tmpstatname, (char)stattype, STATS + EXPRESSIONS ) )
		{
			fputs( tmpstatname, stdout );
			fputs( " variable not added\n", stdout );
			fputs( "There may be a clash between\n", stdout );
			fputs( "names of expressions in \"", stdout );
			fputs( EXPR_FILE, stdout );
			fputs( "\"\nand stat names in \"", stdout );
			fputs( ITEMS_FILE, stdout );
			fputs( "\"\n", stdout );
			fputs( "\nor there are to many stats and expressions\n", stdout );
			exit(EXIT_FAILURE);
			
		}
		//fputs( tmpstatname, stdout );
		//fputs( " variable added\n", stdout );
		
		while( (stattype = getc( itemfile )) != ')' && stattype != EOF )
			continue;
		
		statcount++;
	}
	clearline(itemfile);
	//printf( "stats: %d\n", statcount );
	
	
	// Get the names and  stats of items found
	itemCount = 0;
	while( itemCount < ITEMS && checkEOF( itemfile ) != 1 )
	{
		getname( itemfile, names[itemCount], NAME );
		
		for ( i = 0; i < statcount; i++ )
		{
			if( variablesArray[i].type == 'f' )
				fscanf ( itemfile, "%f", &(statsArray[i][itemCount].fvalue) );
			else
				if( variablesArray[i].type == 'i' )
					fscanf ( itemfile, "%d", &(statsArray[i][itemCount].ivalue) );
				else
				{
					fputs( "Missing stat's type indicator in \"items.txt\" file.\n", stdout );
					exit(EXIT_FAILURE);
				}
		}
		
		clearline(itemfile);
		
		itemCount++;
	}
	//printf("items: %d\n", itemCount);
	
	
	
	
	// expressions
	numExpr = 0;
	while( numExpr < EXPRESSIONS && checkEOF( exprfile ) != 1 )
	{
		if( getstattype( exprfile, tmpstatname, STATLEN) < 1 )
		{
			fputs( "missing variable name (in expression file).\n\n", stdout);
			exit(EXIT_FAILURE);
		}
		clearline(exprfile);
		
		
		
		if( ! AddVar( variablesArray, tmpstatname, 'f', STATS + EXPRESSIONS ) )
		{
			fputs( tmpstatname, stdout );
			fputs( " variable not added\n", stdout );
			fputs( "There may be a clash between\n", stdout );
			fputs( "names of expressions in \"", stdout );
			fputs( EXPR_FILE, stdout );
			fputs( "\"\nand stat names in \"", stdout );
			fputs( ITEMS_FILE, stdout );
			fputs( "\"\n", stdout );
			fputs( "\nor there are to many stats and expressions\n", stdout );
			exit(EXIT_FAILURE);
		}
		
		
		//fputs( tmpstatname, stdout );
		//fputs( " variable added\n", stdout );
		
		
		while( isspace(prompt = getc(exprfile)) )
			continue;
		if(prompt != '"')
		{
			fputs( "Expression missing double quote at start.\n\n", stdout);
			exit(EXIT_FAILURE);
		}
		
		for(i = 0; (prompt = getc(exprfile)) != '"' && prompt != EOF && prompt != '\n' && i < EXPR_SIZE - 1; i++ )
		{
			tmpExpr[i] = prompt;
			
		}
		tmpExpr[i] = '\0';
		
		if( prompt != '"' )
		{
			fputs( "Expression missing double quote at end.\n\n", stdout);
			exit(EXIT_FAILURE);
		}
		
		clearline(exprfile);
		
		
		
		if ( allchecks( tmpExpr, strlen ( tmpExpr ) ) == 0 )
		{
			fputs( "Invalid expression\n\n", stdout);
			exit(EXIT_FAILURE);
		}
		
		
		//expr
		extract( &(exprArray[numExpr].rep), tmpExpr, strlen ( tmpExpr ) );
		if( (exprArray[numExpr].varIndex = SearchVarArray( variablesArray, tmpstatname, STATS + EXPRESSIONS )) < 0 )
		{
			printf("No var index for expression %d, name %s\n\n", numExpr, tmpstatname );
			exit(EXIT_FAILURE);
		}
		finExpr = exprArray[numExpr].varIndex;
		numExpr++;
	}
	
	
	
	// match stats' name's from expressions and item file
	for( iExpr = 0, i = 0; iExpr < numExpr; iExpr++ )
	{
		
		curVar = exprArray[iExpr].rep.vars;
		while(curVar != NULL)
		{
			
			if( (usedVarsIndex[i] = SearchVarArray( variablesArray, curVar->name, STATS + EXPRESSIONS )) >= 0 && variablesArray[usedVarsIndex[i]].type == 'f' )
			{
				for(j = 0; j < i; j++)
					if( usedVarsIndex[i] == usedVarsIndex[j] )
					{
						//fputs( "Duplicate ", stdout );
						usedVarsIndex[i] = -1;
						i--;
					}
				
				//fputs( curVar->name, stdout );
				//fputs( " variable found\n", stdout );
				i++;
			}
			else
			{
				fputs( curVar->name, stdout );
				fputs( " variable not found. Exiting program\n", stdout );
				exit(EXIT_FAILURE);
			}
			
			curVar = curVar->next;
		}
	}
	
	
	for( iExpr = 0; iExpr < numExpr; iExpr++ )
	{
		// sort variables by name
		
		curVar = exprArray[iExpr].rep.vars;
		exprArray[iExpr].rep.vars = NULL;
		
		while( curVar != NULL )
		{
			shiftVar = curVar->next;
			
			i = SearchVarArray( variablesArray, curVar->name, STATS + EXPRESSIONS );
			if( exprArray[iExpr].pos[i] == NULL )
			{
				exprArray[iExpr].pos[i] = curVar;
				curVar->next = NULL;
				
			}
			else
			{
				curVar->next = exprArray[iExpr].pos[i];
				exprArray[iExpr].pos[i] = curVar;
				
			}
			
			curVar = shiftVar;
		}
		
	}
	
	
	// include stat
	fputs( "Include a stat that is not found in the \"expr.txt\" file? ", stdout );
	prompt = PromptFunc( 'y', 'n', "(Enter yes or no): " );
	while( prompt == 'y' && statcount > 0)
	{
		for( i = 0; i < statcount ; i++ )
		{
			if( variablesArray[i].type == 'f' )
				printf( "%d) %s\n", i, variablesArray[i].name );
		}
		fputc('\n', stdout);
		
		while(usedVarsIndex[i] >= 0 && i < STATS + EXPRESSIONS )
			i++;
		if(i < STATS + EXPRESSIONS)
		{
			fputs( "\nEnter number related to desired variable: ", stdout );	
			while( scanf( "%d", &j ) != 1 || j < 0 || j >= statcount || variablesArray[j].type != 'f' )
			{
				while( getchar() != '\n' )
					continue;
				fputs( "Enter number related to desired variable: ", stdout );
			}
			
			
			for(i = 0; usedVarsIndex[i] >= 0 && usedVarsIndex[i] != j; i++ )
				continue;
			if(usedVarsIndex[i] == -1)
			{
				usedVarsIndex[i] = j;
			}
			else
			{
				fputs(variablesArray[j].name, stdout);
				fputs(" already in list.\n\n", stdout);
				
				
			}
			fputs( "Include another stat that should have a min and max value? ", stdout );
			prompt = PromptFunc( 'y', 'n', "(Enter yes or no): " );
		}
		else
		{
			fputs( "No space available for more min/max values\n\n", stdout );
			prompt = 'n';
		}
		
	}
	
	// set maximums and minimums
	fputs( "Set min and max value for a stat? ", stdout );
	prompt = PromptFunc( 'y', 'n', "(Enter yes or no): " );
	while( prompt == 'y' && fVars > 0 && STATS > 0 )
	{
		for( i = 0; i < STATS+EXPRESSIONS && usedVarsIndex[i] >= 0; i++ )
		{
			if( usedVarsIndex[i] < statcount )
				printf( "%d) %s\n", i, variablesArray[usedVarsIndex[i]].name );
		}
		fputc('\n', stdout);
		
		do
		{
			fputs( "Enter number related to desired variable: ", stdout );	
			while( scanf( "%d", &i ) != 1 || i < 0 || i >= STATS+EXPRESSIONS )
			{
				while( getchar() != '\n' )
					continue;
				fputs( "Enter number related to desired variable: ", stdout );
			}
			
			if( usedVarsIndex[i] < statcount && usedVarsIndex[i] >= 0 )
			{
				fputs("Selected ", stdout);
				fputs(variablesArray[usedVarsIndex[i]].name, stdout);
				fputs("\nIs this the desired variable? ", stdout);
				prompt = PromptFunc( 'y', 'n', "(Enter yes or no): " );
			}
			
		}while( usedVarsIndex[i] >= statcount || usedVarsIndex[i] < 0 || prompt == 'n' );
		
		
		do
		{
			fputs( "Enter minimum ", stdout );
			fputs( variablesArray[usedVarsIndex[i]].name, stdout );
			fputs( " value: ", stdout );
			while( scanf( "%f", statMin + usedVarsIndex[i] ) != 1 )
			{
				while( getchar() != '\n' )
					continue;
				fputs( "Enter minimum ", stdout );
				fputs( variablesArray[usedVarsIndex[i]].name, stdout );
				fputs( " value: ", stdout );
			}
			
			fputs( "Enter maximum ", stdout );
			fputs( variablesArray[usedVarsIndex[i]].name, stdout );
			fputs( " value: ", stdout );
			while( scanf( "%f", statMax + usedVarsIndex[i] ) != 1 )
			{
				while( getchar() != '\n' )
					continue;
				fputs( "Enter maximum ", stdout );
				fputs( variablesArray[usedVarsIndex[i]].name, stdout );
				fputs( " value: ", stdout );
			}
			
			fputs( "Enter hard maximum ", stdout );
			fputs( variablesArray[usedVarsIndex[i]].name, stdout );
			fputs( " value: ", stdout );
			while( scanf( "%f", statLimit + usedVarsIndex[i] ) != 1 )
			{
				while( getchar() != '\n' )
					continue;
				fputs( "Enter hard maximum ", stdout );
				fputs( variablesArray[usedVarsIndex[i]].name, stdout );
				fputs( " value: ", stdout );
			}
			
			printf("Min = %f    Max = %f    Hard Max = %f\n", statMin[usedVarsIndex[i]], statMax[usedVarsIndex[i]], statLimit[usedVarsIndex[i]]);
			if( statMin[usedVarsIndex[i]] >= statMax[usedVarsIndex[i]] || statMin[usedVarsIndex[i]] >= statLimit[usedVarsIndex[i]] )
				fputs( "Minimum must be smaller than maximuns.\n", stdout );
			else
			{
				fputs("Re-enter Min and Max? ", stdout);
				prompt = PromptFunc( 'y', 'n', "(Enter yes or no): " );
			}
		}while( statMin[usedVarsIndex[i]] >= statMax[usedVarsIndex[i]] || statMin[usedVarsIndex[i]] >= statLimit[usedVarsIndex[i]] || prompt == 'y' );
		
		
		fputs( "Set min and max for another stat? ", stdout );
		prompt = PromptFunc( 'y', 'n', "(Enter yes or no): " );
	}
	
	
	
	
	// give option to check for illegal combinations
	groupopt = 0;
	
	fputs( "Ignore illegal combinations? ", stdout );
	prompt = PromptFunc( 'y', 'n', "(Enter yes or no): " );
	if( prompt == 'y' )
		groupopt = 1;
	
	
	if( groupopt && iVars > 0 )
	{
		// number of primes
		nprimesfile = openfile ( NULL, NOP_FILE );
		if( nprimesfile == NULL )
		{
			exit(EXIT_FAILURE);
		}
		
		// number of primes used	
		if( fscanf( nprimesfile, "%d", &nprimes ) != 1 )
		{
			fputs("No number found in file \"", stdout);
			fputs(NOP_FILE, stdout);
			fputs("\"\n", stdout);
			exit(EXIT_FAILURE);
		}
		fclose(nprimesfile);
		
		if( nprimes > IGROUPS || nprimes < 0 )
		{
			printf("Number found in file \"%s\"\nshould be %d or less\nand be 0 or greater\n", NOP_FILE, IGROUPS);
			
			exit(EXIT_FAILURE);
		}
		
		
		for( i = 0; i < STATS && variablesArray[i].type != 'i'; i++ )
			continue;
		
		
		grouprow = i;
		groupproduct = &variablesArray[i].ivalue;
		fputs( "int variable used: ", stdout );
		fputs( variablesArray[i].name, stdout );
		fputc( '\n', stdout );
		
		for( i = 0; i < itemCount; i++ )
			if( statsArray[grouprow][i].ivalue < 1 )
			{
				fputs("int less than 1 in GRP column\n", stdout);
				exit(EXIT_FAILURE);
			}
		
	}
	else if( groupopt && iVars <= 0 )
	{
		fputs( "No int type stats available.\n", stdout );
		exit(EXIT_FAILURE);
		
		
	}
	
	// force use of an item in combinations
	fputs( "Would you like to include an item? ", stdout );
	prompt = PromptFunc( 'y', 'n', "(Enter yes or no): " );
	while(prompt == 'y')
	{
		do
		{
			fputs( "Enter index number of item: ", stdout );
			while( scanf( "%d", &i ) != 1 || i >= itemCount+2 || i < 0+2 )
				{
					while( getchar() != '\n' )
						continue;
					fputs("No item associated with that number.\n", stdout);
					fputs( "Enter index number of item: ", stdout );
				}
			i = i-2;
			printf("\nItem: %s\n", names[i]);
			fputs("Is this the right item? ", stdout);
			prompt = PromptFunc( 'y', 'n', "(Enter yes or no): " );
			
		}while( prompt == 'n' );
		
		
		for(j = 0; usedVarsIndex[j] >= 0 && j < STATS; j++ )
		{
			if( usedVarsIndex[j] < statcount )
			{
				varsInitial[usedVarsIndex[j]] += statsArray[ usedVarsIndex[j] ][i].fvalue;
				
				statsArray[ usedVarsIndex[j] ][i].fvalue = 0.0f;
			}
			
		}
		if( groupopt )
		{
			for(j = 0; j < nprimes; j++ )
			{
				if( grpInit % primes[j] != 0 && statsArray[grouprow][i].ivalue % primes[j] == 0 )
					grpInit *= primes[j];
			}
			
		}
		
		fputs( "Would you like to include another item? ", stdout );
		prompt = PromptFunc( 'y', 'n', "(Enter yes or no): " );
	}	
	
	
	// Check if there are enough items to fill the number of slots 
	if (slots > itemCount)
	{
		fputs( "Not enough items for the ammount of slots\n\n", stdout);
		exit(EXIT_FAILURE);
	}
	
	
	// Open empty file in which results will be placed
	outputfile = emptyfile( RESULTS_FILE );
	if( outputfile == NULL )
	{
		exit(EXIT_FAILURE);
	}
	
	
	
	//Navori
	//tari = SearchVarArray( variablesArray, "TRG", STATS + EXPRESSIONS );
	//SBi = SearchVarArray( variablesArray, "ASB", STATS + EXPRESSIONS );
	
			
	
	// Start of loop that will test combinations
	while( selected[slots - 1] < itemCount )
	{
		
		// Detect clashes due to items from same group type
		if ( groupopt )
		{
			for( i = 0, *groupproduct = grpInit; i < slots-1; ++i )
				*groupproduct *= statsArray[grouprow][selected[i]].ivalue;
			
			for( i = 0, lim = 1; i < nprimes && lim != 0; i++ )
				if( *groupproduct % ( primes[i] * primes[i] ) == 0 )
				{
					lim = 0;
					confT = 1;
					//printf("clash %d\ni = %d\n", primes[i], i);
				}
			
			
			while( lim == 0 && confT == 1 )
			{
				for( i = 0, conflict = 0, confT = 0; i < nprimes; i++ )
					if( *groupproduct % ( primes[i] * primes[i] ) == 0 )
					{
						for(j = 0; j < slots-1; j++)
						{
							if(statsArray[grouprow][selected[j]].ivalue % primes[i] == 0 && j > conflict)
								if( selected[j] < itemCount - (slots - j) )
								{
									conflict = j;
									confT = 1;
								}
						}
					}
				
				if( confT )
				{
					slot = conflict;
					
					selected[slot] = selected[slot] + 1;
					
					while( ++slot < slots )
						selected[slot] = selected[slot - 1] + 1;
				}
				
				for( i = 0, *groupproduct = grpInit; i < slots-1; ++i )
					*groupproduct *= statsArray[grouprow][selected[i]].ivalue;
				
				for( i = 0, lim = 1; i < nprimes; i++ )
					if( *groupproduct % ( primes[i] * primes[i] ) == 0 )
						lim = 0;
			}
			
			for( i = 0, *groupproduct = grpInit; i < slots; ++i )
				*groupproduct *= statsArray[grouprow][selected[i]].ivalue;
			
			for( i = 0, lim = 1; i < nprimes; i++ )
				if( *groupproduct % ( primes[i] * primes[i] ) == 0 )
					lim = 0;
		}
		
		
		// Continue if there are no clashes
		if(lim)
		{
			
			// calculate variables' values
			i = 0;
			while( i < STATS+EXPRESSIONS && usedVarsIndex[i] >= 0  )
			{
				if( usedVarsIndex[i] < statcount )
				{
					
					(variablesArray + usedVarsIndex[i])->fvalue = varsInitial[usedVarsIndex[i]];
					
					for(j = 0; j < slots; ++j)
						(variablesArray + usedVarsIndex[i])->fvalue += statsArray[usedVarsIndex[i]][selected[j]].fvalue;
				}
				i++;
			}
			
			
			
			
			
			// min max enforce
			i = 0;
			skip = 1;
			while( usedVarsIndex[i] >= 0 && i < STATS+EXPRESSIONS )
			{
				if( usedVarsIndex[i] < statcount )
				{
					if( (variablesArray + usedVarsIndex[i])->fvalue < statMin[usedVarsIndex[i]] || (variablesArray + usedVarsIndex[i])->fvalue > statLimit[usedVarsIndex[i]] )
						skip = 0;
					else if( (variablesArray + usedVarsIndex[i])->fvalue > statMax[usedVarsIndex[i]] ) 
						(variablesArray + usedVarsIndex[i])->fvalue = statMax[usedVarsIndex[i]];
				}
				
				i++;
			}
			
			// navori
			//target = (variablesArray + tari)->fvalue;
			//atkSB = (variablesArray + SBi)->fvalue;
			
			
			
			// skip if minimum is not met
			if(skip)
			{
				
				for( iExpr = 0; iExpr < numExpr; iExpr++ )
				{
					//  Place totals calcutated from the stats into appropriate places of the expression variable
					i = 0;
					while( usedVarsIndex[i] >= 0 && i < STATS+EXPRESSIONS  )
					{
						curVar = exprArray[iExpr].pos[ usedVarsIndex[i] ];
						
						while( curVar != NULL )
						{
							(exprArray[iExpr].rep.operands)[curVar->operandPos] = curVar->sign * (variablesArray + usedVarsIndex[i])->fvalue;
							
							curVar = curVar->next;
						}
						
						
						
						//(variablesArray + statcount + iExpr)->fvalue = eval( &(exprArray[iExpr].rep) );
						
						i++;
					}
					
					(variablesArray + exprArray[iExpr].varIndex)->fvalue = eval( &(exprArray[iExpr].rep) );
					
				}
				
				
				
				// Calulate results
				//tottmp = eval( &test1 );
				tottmp = (variablesArray + finExpr)->fvalue;
				
				//navori
				//tottmp = SumT( target, atkSB);
				
				
				
				// S
				//  tmp   <=  total
				// *small <= *large
				
				// S with variance
				//  tmp <= total + variance
				// *small <= *large
				
				// L
				//  total <=  tmp
				// *small <= *large
				
				// L with variance
				//  total - variance <=  tmp
				// *small <= *large
				
				
				// Place results in output file if the result is larger than previous result 
				if(*small < *large + diff)
				{
					
					for(i = 0; i < slots; ++i)
						fprintf( outputfile, "%d ", selected[i] + 2 );
					
					fprintf( outputfile, "= %.4f\n", tottmp );
					
					if(*small < *large)
					{
						total = tottmp;
					}
				}
			}
		}
		
		
		
		// Sort through combinations
		selected[slots - 1] = selected[slots - 1] + 1;
		
		if( selected[slots - 1] >= itemCount && selected[0] < itemCount - slots )
		{
			selected[slots - 1] = itemCount - 1; 
			
			slot = slots - 1;
			while( slot-- > 0 )
			{
				
				if( selected[slot] + 1 != selected[slot + 1] )
				{
					selected[slot] = selected[slot] + 1;
					
					while( ++slot < slots )
						selected[slot] = selected[slot - 1] + 1;
					
					break;
				}
			}
		}
	}
	fprintf( outputfile, "Best = %.4f\n", total );
	
	
	fputs("Output place in file \"", stdout);
	fputs(RESULTS_FILE, stdout);
	fputs("\"\n", stdout);
	
	fclose(itemfile);
	fclose(outputfile);
	fclose(exprfile);
	
	// free memory
	free ( selected );
	for( i = 0; i < numExpr; i++ )
		freeExprRecipe ( &(exprArray[i].rep) );
	
	for(iExpr = 0; iExpr < numExpr; iExpr++)
		for(i = 0; i < STATS + EXPRESSIONS; i++) // fix
			cleanVarList( exprArray[iExpr].pos[i] );
	
	return 0;
}



int PromptFunc( char opt1, char opt2, char *instruct )
{
	int prompt;
	
	while( prompt == opt1 || prompt == opt2 )
		prompt++;

	fputs( instruct, stdout);
		
	while( isspace(prompt = getchar()) )
			continue;
	
	while( prompt != opt1 && prompt != opt2 )
	{
		while( getchar() != '\n' )
			continue;
		
		fputs( instruct, stdout);
		while( isspace(prompt = getchar()) )
			continue;
	}
	while( getchar() != '\n' )
		continue;
	
	return prompt;
}




// Navori Func
/*float SumT( float target, float atkSB )
{
	float t = 0.0f, hits, tSplit;
	float reduce, reduceTot = 0.0f;
	
	hits = 0.625f+(atkSB+0.025f*8.0f*(0.7025f+0.0175f*8.0f))*0.651f;
	tSplit = 1.0f/hits;
	
	while(t < target)
	{
		t += tSplit;
		
		if(t < target)
		{
			reduce = (target-t)*0.12f;
			reduceTot += reduce; 
			t += reduce;
		}
		
		
		
		
		
		
		
	}
	
	return target - reduceTot;
}*/



