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





int main( void )
{
	int stattype;
	int statcount;
	char tmpstatname[STATLEN];
	
	char names[ITEMS][NAME];
	ifunion statsArray[STATS][ITEMS];
	int itemCount;
	
	int prompt;
	
	int i, j;
	
	int finExpr;
	
	
	
	
	
	
	// name, position, name, pointer
	variable * curVar, *shiftVar;
	char tmpExpr[EXPR_SIZE];
	expression exprArray[EXPRESSIONS];
	int numExpr = 0;
	int iExpr;
	
	// Navori variables
	//int tari, SBi;
	//float target, atkSB;
	
	// varType: type, value, name
	
	// variablesArray
	// holds all variable types mentiond by item's file and expressions
	varType variablesArray[STATS+EXPRESSIONS];
	// usedVarsIndex
	// holds index number of variabls found in expressions
	// value of -1 indicates no index number here or later in the array
	int usedVarsIndex[STATS+EXPRESSIONS];
	float varsInitial[STATS];
	
	
	float tottmp;
	
	FILE *itemfile, *exprfile;
	


	// pre-checks
	
	// expr
	exprfile = openfile ( NULL, EXPR_FILE );
	if( exprfile == NULL )
	{
		exit(EXIT_FAILURE);
	}
	
	
	// items
	itemfile = openfile ( NULL, ITEMS_FILE );
	if( itemfile == NULL )
	{
		exit(EXIT_FAILURE);
	}
	
	
	
	// Initialize
	InitVarArray( variablesArray, STATS+EXPRESSIONS );
	
	
	
	for(i = 0; i < STATS; i++)
		usedVarsIndex[i] = -1;
	
	//for(i = 0; i < EXPRESSIONS; i++)
		//for(j = 0; j < STATS+EXPRESSIONS; j++)
			//exprArray[i].used[j] = -1;
	
	for(i = 0; i < STATS; i++)
		varsInitial[i] = 0.0f;
	
	for(i = 0; i < EXPRESSIONS; i++)
		for(j = 0; j < STATS+EXPRESSIONS; j++)
			exprArray[i].pos[j] = NULL;
	
	
	//for(i = 0; i < EXPRESSIONS; i++)
		//cmpVar[i][0] = -1;
			
	
	
	
	
	// Detect the number of stat columns the item file contains
	// Detect each stats' type
	statcount = 0;
	while ( statcount < STATS && getstattype( itemfile, tmpstatname, STATLEN) )
	{
		while( (stattype = getc( itemfile )) != '(' && stattype != EOF && stattype != '\n' )
			continue;
		while( !isalpha(stattype = getc( itemfile )) && stattype != EOF && stattype != '\n' ) 
			continue;
		
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
		
		
		
		fputs( "Would you like to include another item? ", stdout );
		prompt = PromptFunc( 'y', 'n', "(Enter yes or no): " );
	}	
	
	
	
	
	
	
	
	
	
	//Navori
	//tari = SearchVarArray( variablesArray, "TRG", STATS + EXPRESSIONS );
	//SBi = SearchVarArray( variablesArray, "ASB", STATS + EXPRESSIONS );
	
	
	// Continue if there are no clashes
	
	// calculate variables' values
	i = 0;
	while( i < STATS+EXPRESSIONS && usedVarsIndex[i] >= 0  )
	{
		if( usedVarsIndex[i] < statcount )
		{
			(variablesArray + usedVarsIndex[i])->fvalue = varsInitial[usedVarsIndex[i]];
		}
		i++;
	}
	
	
	for( iExpr = 0; iExpr < numExpr; iExpr++ )
	{
		//  Place totals calcutated from the stats into appropriate places of the expression variable
		i = 0;
		while( usedVarsIndex[i] >= 0 && i < STATS+EXPRESSIONS  )
		//while( i < STATS+EXPRESSIONS  )
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
		//printf("%s val: %f\n", (variablesArray + exprArray[iExpr].varIndex)->name, (variablesArray + exprArray[iExpr].varIndex)->fvalue );
	}	
	
	// navori
	//target = (variablesArray + tari)->fvalue;
	//atkSB = (variablesArray + SBi)->fvalue;
	
	
	
		
		
		
		
		
	// Calulate results
	//tottmp = eval( &test1 );
	tottmp = (variablesArray + finExpr)->fvalue;
	
	//navori
	//tottmp = SumT( target, atkSB);
	
	
	
	
	
	
	printf( "%.4f\n", tottmp );
	
	
	
	
	
	
	
	
	fclose(itemfile);
	fclose(exprfile);
	
	// free memory
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



