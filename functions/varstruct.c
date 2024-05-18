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

#include "varstruct.h"



// functions

void SetTypeNone( varType *varP )
{	
	varP->type = 'n';
	
}

void SetTypeFloat( varType *varP )
{	
	varP->type = 'f';
	
}

void SetTypeInt( varType *varP )
{	
	varP->type = 'i';
	
}

char GetVarType( varType *varP )
{
	
	return varP->type;
}

float GetVarFValue( varType *varP )
{
	
	if( varP->type != 'f' )
	{
		fputs( varP->name, stdout );
		fputs( " not a float type. (Get)\n", stdout );
	}
	
	return varP->fvalue;
}

void AssignVarFValue( varType *varP, float value )
{
	if( varP->type != 'f' )
	{
		fputs( varP->name, stdout );
		fputs( " not a float type. (Assign)\n", stdout );
	}
	else
		varP->fvalue = value;
}

void VarFOp( varType *varP, char op, float value )
{
	switch (op)
	{
		case '+' :
			AssignVarFValue( varP, varP->fvalue + value );
			break;
		case '-' :
			AssignVarFValue( varP, varP->fvalue - value );
			break;
		case '*' :
			AssignVarFValue( varP, varP->fvalue * value );
			break;
		case '/' :
			AssignVarFValue( varP, varP->fvalue / value );
	}
		
}

int GetVarIValue( varType *varP )
{
	if( varP->type != 'i' )
	{
		fputs( varP->name, stdout );
		fputs( " not an int type. (Get)\n", stdout );
	}
	
	return varP->ivalue;
}

void AssignVarIValue( varType *varP, int value )
{
	if( varP->type != 'i' )
	{
		fputs( varP->name, stdout );
		fputs( " not an int type. (Assign)\n", stdout );
	}
	else
		varP->ivalue = value;
}

void VarIOp( varType *varP, char op, int value )
{
	switch (op)
	{
		case '+' :
			AssignVarIValue( varP, varP->ivalue + value );
			break;
		case '-' :
			AssignVarIValue( varP, varP->ivalue - value );
			break;
		case '*' :
			AssignVarIValue( varP, varP->ivalue * value );
			break;
		case '/' :
			AssignVarIValue( varP, varP->ivalue / value );
	}
		
}

void AssignVarName( varType *varP, char * name )
{
	strncpy( varP->name, name, STATLEN - 1 );
	varP->name[STATLEN - 1] = '\0';
}

void RemoveVarName( varType *varP )
{
	varP->name[0] = '\0';
	
}

int VarNameMatch( varType *varP, char * name )
{
	
	return !strncmp( varP->name, name, STATLEN );
}
