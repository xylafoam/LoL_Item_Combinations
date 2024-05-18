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



#ifndef VAR_STRUCT_H
#define VAR_STRUCT_H


#include "items.h"

// structs
typedef struct
{
	char type;
	union {float fvalue; int ivalue;};
	char name[STATLEN];
} varType;


// functions

void SetTypeNone( varType *varP );

void SetTypeFloat( varType *varP ); //

void SetTypeInt( varType *varP ); //

char GetVarType( varType *varP ); //

float GetVarFValue( varType *varP ); //

void AssignVarFValue( varType *varP, float value ); //

void VarFOp( varType *varP, char op, float value );

int GetVarIValue( varType *varP ); //

void AssignVarIValue( varType *varP, int value ); //

void VarIOp( varType *varP, char op, int value );

void AssignVarName( varType *varP, char * name );

void RemoveVarName( varType *varP );

int VarNameMatch( varType *varP, char * name );

#endif