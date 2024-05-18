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



#include "vararray.h"

void InitVarArray( varType varP[], int n )
{
	int i;
	
	for( i = 0; i < n ; ++i )
	{
		RemoveVarName( varP + i );
		
		SetTypeNone( varP + i );
	}
}

int SearchVarArray( varType varP[], char * name, int n )
{
	int i, match = -1;
	
	for( i = 0; i < n ; ++i )
		if( VarNameMatch( varP + i, name ) )
			match = i;
	
	return match;
}

int AddVar( varType varP[], char * name, char type, int n )
{
	int i, added = 0;
	
	if( SearchVarArray( varP, name, n ) == -1 )
	{
		for( i = 0; i < n ; ++i )
			if( (varP + i)->name[0] == '\0' )
			{
				if( type == 'f' )
					SetTypeFloat( varP + i );
				else if( type == 'i' )
					SetTypeInt( varP + i );
				
				AssignVarName( varP + i, name );
				break;
			}
				
		added = 1;
	}
	
	return added;
}

int RemoveVar( varType varP[], char * name, int n )
{
	int i, removed = 0;
	
	i = SearchVarArray( varP, name, n );
	
	if( i >= 0 )
	{
		RemoveVarName( varP + i );
		
		SetTypeNone( varP + i );
		
		removed = 1;
	}
	
	return removed;
}

int ChangeVarType( varType varP[], char * name, char type, int n )
{
	int i, changed = 0;
	
	i = SearchVarArray( varP, name, n );
	if( i >= 0 )
	{
		
		if( type == 'f' )
			SetTypeFloat( varP + i );
		else if( type == 'i' )
			SetTypeInt( varP + i );
		
		changed = 1;
	}
	
	return changed;
}