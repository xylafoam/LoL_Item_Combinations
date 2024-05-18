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



#ifndef EXPR_LIST_H
#define EXPR_LIST_H

#include "items.h"


#define EXPR_FILE "expr.txt"

#define EXPRESSIONS 10
#define EXPR_SIZE 150


typedef struct varStruct 
{
	char name[STATLEN];
	int operandPos;
	float sign;
	struct varStruct * next;
} variable;

typedef struct
{
	variable *vars; //new
	int *opPos;
	int opcount;
	char *operators;
	float *operands;
	int *fillorder
} exprrecipe;


// evaluate the expression represented by an "exprrecipe" variable
float eval( exprrecipe * formula );

// The next seven function check a string for proper grammar.
int parenthcheck(char * expr, int n);

int popcheck(char * expr, int n);

int operatorcheck(char * expr, int n);

int numbercheck(char * expr, int n);

int depthcheck( char * expr, int n );

int signcheck(char * expr, int n);

int gramcheck(char * expr, int n);

int validparenth(char * expr, int n);

// allchecks runs the seven checks above.
int allchecks(char * expr, int n);

// gets the number of operators found in a string. 
int opcount(char * expr, int n);

// Reads a string containing an expression. After parsing the string it fills an exprrecipe variable 
// with the data necessary for it to be equivalent to the string expression.
// A variable pointer will be given the address to a linked list of variables found in the string. 
void extract( exprrecipe * formula, char * expr, int n );

// Loads an exprrecipe variable with the data necessary for placing results of operations in the correct order. 
void fillOrder ( exprrecipe * test1 );

//free memory
void freeExprRecipe ( exprrecipe * test1 );

// add variable to linked list
variable * addVar( variable * curVar, char * name, int pos, float sign);

// start the linked list of variables
variable * startVarList( char * name, int pos, float sign );

// free memory
void cleanVarList( variable * varList );

#endif