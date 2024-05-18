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



#include <ctype.h>
#include <stdlib.h>

#include <math.h>

#include "exprlist.h"



float eval( exprrecipe * formula )
{
	int i = 0, j = 0;
	
	while (i < formula->opcount) 
	{
		switch( (formula->operators)[i] ) 
		{
			case '+' :
			(formula->operands)[(formula->fillorder)[i]] = (formula->operands)[j] + (formula->operands)[j+1];
			break;
			
			case '-' :
			(formula->operands)[(formula->fillorder)[i]] = (formula->operands)[j] - (formula->operands)[j+1];
			break;
			
			case '*' :
			(formula->operands)[(formula->fillorder)[i]] = (formula->operands)[j] * (formula->operands)[j+1];
			break;
			
			case '/' :
			(formula->operands)[(formula->fillorder)[i]] = (formula->operands)[j] / (formula->operands)[j+1];
			//break;
			
			//case '^' :
			//(formula->operands)[(formula->fillorder)[i]] = powf((formula->operands)[j], (formula->operands)[j+1]);
		}
		
		i++;
		j += 2;
	}
	
	return (formula->operands)[j];
}


int parenthcheck(char * expr, int n)
{
	int i, pair;
	
	for(i = 0, pair = 0; i < n; i++)
	{
		if (expr[i] == '(')
			pair++;
		else if (expr[i] == ')')
			pair--;
		
	}
	
	pair = !pair;
	
	return pair;
}

int operatorcheck(char * expr, int n)
{
	int i = 0;//, tmpi; 
	int pass = 1;
	
	
	while( isblank(expr[i]) && i < n )
			i++;
		
	switch (expr[i])
	{
		case '*' :
		case '/' :
		case '+' :
		case '-' :
		//case '^' :
			pass = 0;
	}
	i++;
	
	while(i < n && pass)
	{
		
		
		while( isblank(expr[i]) && i < n )
			i++;
		
		
		
		switch (expr[i])
		{
			case '+' :
			case '-' :
			case '*' :
			case '/' :
			//case '^' :
				
				i++;
				while( isblank(expr[i]) && i < n )
					i++;
				
				if (i >= n)
				{
					pass = 0;
				}
				else
				{
					switch (expr[i])
					{
						case '+' :
						case '-' :
						case '*' :
						case '/' :
						case ')' :
						//case '^' :
							pass = 0;
					}
				}
		}
		
		i++;
	}
	
	
	return pass;
}


int numbercheck( char * expr, int n )
{
	int i, pass = 1, period;
	
	for(i = 0; i < n && pass; i++)
	{
		while( isblank(expr[i]) && i < n )
					i++;
		
		if ( isdigit(expr[i]) )
		{
			period = 0;
			while( ( isdigit(expr[++i]) || expr[i] == '.' ) && i < n )
			{
				if(expr[i] == '.')
					period++;
			}
			if(period >= 2)
				pass = 0;
			
			
			while( isblank(expr[i]) && i < n )
						i++;
			
			if ( isalnum(expr[i]) )
				pass = 0;
		}
		else if ( isalpha(expr[i]) )
		{
			period = 0;
			while( isalpha(expr[++i]) && i < n )
				continue;
				
			while( isblank(expr[i]) && i < n )
						i++;
			
			if ( isalnum(expr[i]) )
				pass = 0;
		}
	}
	
	
	return pass;
}


void extract( exprrecipe * formula, char * expr, int n /* , variable ** varList */ )
{
	int i, tmpi, depth, maxdepth, tmpdepth, opi = 0, andi = 0, /*filli = 0,*/ match;
	variable * curVar;
	
	
	maxdepth = depthcheck( expr, n );
	
	formula->opcount = opcount(expr, n);
	
	// allocate memmory
	formula->operators = malloc( formula->opcount * sizeof (*formula->operators) );
	if( formula->operators == NULL )
		exit(EXIT_FAILURE);
	
	formula->operands = malloc( 1 + 2 * formula->opcount * sizeof (*formula->operands) );
	if( formula->operands == NULL )
		exit(EXIT_FAILURE);
	
	formula->fillorder = malloc( formula->opcount * sizeof (*formula->fillorder) );
	if( formula->fillorder == NULL )
		exit(EXIT_FAILURE);
	
	formula->opPos = malloc( formula->opcount * sizeof (*formula->opPos) );
	if( formula->opPos == NULL )
		exit(EXIT_FAILURE);
	
	
	formula->vars = NULL;
	//*varList = NULL;
	
	// start of parsing
	while(maxdepth >= 0)
	{
		// get ^ operators and their operands
		/*for(i = 0, depth = 0; i < n; i++)
		{
			if (expr[i] == '(')
			{
				tmpi = i + 1;
				while ( isblank(expr[tmpi]) )
					tmpi++;
				
				if( expr[tmpi] == '-' )
				{
					while(expr[i] != ')')
						i++;
				}
				else
				depth++;
			}
			else if(expr[i] == ')')
				depth--;
			else if(depth == maxdepth)
			{
				switch( expr[i] )
				{
					case '^' :
						(formula->operators)[opi] = expr[i];
						(formula->opPos)[opi] = i;
						opi++;
						
						// check left
						tmpi = i - 1;
						match = 1;
						while(tmpi >= 0 && match )
						{
							switch( expr[tmpi] )
							{
								case '^' :
									andi++;
									
									match = 0;
									break;
								case ')' :
									tmpdepth = 1;
									tmpi--;
									while(tmpdepth > 0)
									{
										if(expr[tmpi] == ')')
											tmpdepth++;
										else if(expr[tmpi] == '(')
											tmpdepth--;
										tmpi--;
									}
									tmpi += 2;
									
									while( isblank(expr[tmpi]) )
										tmpi++;
									
									if(expr[tmpi] == '-')
									{
										tmpi++;
										while( isblank(expr[tmpi]) )
											tmpi++;
										
										if( isalpha ( expr[tmpi] ) )
										{
											if (  formula->vars == NULL )
											{
												formula->vars = startVarList( expr+tmpi, andi, -1.0f );
												curVar = formula->vars;
											}
											else
												curVar = addVar( curVar, expr+tmpi, andi, -1.0f);
										}
										else
											(formula->operands)[andi] = -1.0f * strtof(expr+tmpi, NULL);
									}
									
									
									andi++;
									
									match = 0;
									break;
									
								case '(' :
								case '+' :
								case '-' :
								case '*' :
								case '/' :

									
									tmpi++;
									while ( isblank ( expr[tmpi] ) )
										tmpi++;
									
									if( isalpha ( expr[tmpi] ) )
									{
										if ( formula->vars == NULL )
										{
											formula->vars = startVarList( expr+tmpi, andi, 1.0f );
											curVar = formula->vars;
										}
										else
											curVar = addVar( curVar, expr+tmpi, andi, 1.0f);
										
										
									}
									else
										(formula->operands)[andi] = strtof(expr+tmpi, NULL);
									
									andi++;
									match = 0;
							}
							
							
							tmpi--;
						}
						if (tmpi < 0 && match)
						{
							tmpi = 0;
							
							while ( isblank ( expr[tmpi] ) )
								tmpi++;
							
							if( isalpha ( expr[tmpi] ) )
							{
								if (formula->vars == NULL )
								{
									formula->vars = startVarList( expr+tmpi, andi, 1.0f );
									curVar = formula->vars;
								}
								else
									curVar = addVar( curVar, expr+tmpi, andi, 1.0f);
								
								
							}
							else
							(formula->operands)[andi] = strtof(expr+tmpi, NULL);
							
							andi++;
						}
						
						// check right
						tmpi = i + 1;
						match = 1;
						while(tmpi < n && match )
						{
							switch( expr[tmpi] )
							{
								case '^' :
								case '*' :
								case '/' :
								case '+' :
								case '-' :
								case ')' :
									
									tmpi = i + 1;
									while( isblank(expr[tmpi]) )
										tmpi++;
									
									if( isalpha ( expr[tmpi] ) )
									{
										if (formula->vars == NULL )
										{
											formula->vars = startVarList( expr+tmpi, andi, 1.0f );
											curVar = formula->vars;
										}
										else
											curVar = addVar( curVar, expr+tmpi, andi, 1.0f);
									}
									else
									(formula->operands)[andi] = strtof(expr+tmpi, NULL);
									
									andi++;
									
									match = 0;
									break;
								case '(' :
									
									tmpi++;
									while( isblank(expr[tmpi]) )
										tmpi++;
									
									if(expr[tmpi] == '-')
									{
										tmpi++;
										while( isblank(expr[tmpi]) )
											tmpi++;
										
										if( isalpha ( expr[tmpi] ) )
										{
											if (formula->vars == NULL )
											{
												formula->vars = startVarList( expr+tmpi, andi, -1.0f );
												curVar = formula->vars;
											}
											else
												curVar = addVar( curVar, expr+tmpi, andi, -1.0f );
										}
										else
											(formula->operands)[andi] = -1.0f * strtof(expr+tmpi, NULL);
										
									}
									
									andi++;
									
									match = 0;
							}
							
							
							tmpi++;
						}
						if (tmpi >= n && match)
						{
							tmpi = i + 1;
							while( isblank(expr[tmpi]) )
								tmpi++;
							
							if( isalpha ( expr[tmpi] ) )
							{
								if (formula->vars == NULL )
								{
									formula->vars = startVarList( expr+tmpi, andi, 1.0f );
									curVar = formula->vars;
								}
								else
									curVar = addVar( curVar, expr+tmpi, andi, 1.0f);
							}
							else
								(formula->operands)[andi] = strtof(expr+tmpi, NULL);
							
							andi++;
						}
				}
			}
			
		}*/
		// ^ end
		
		
		// get * / operators and their operands
		for(i = 0, depth = 0; i < n; i++)
		{
			if (expr[i] == '(')
			{
				tmpi = i + 1;
				while ( isblank(expr[tmpi]) )
					tmpi++;
				
				if( expr[tmpi] == '-' )
				{
					while(expr[i] != ')')
						i++;
				}
				else
				depth++;
			}
			else if(expr[i] == ')')
				depth--;
			else if(depth == maxdepth)
			{
				switch( expr[i] )
				{
					case '/' :
					case '*' :
						(formula->operators)[opi] = expr[i];
						(formula->opPos)[opi] = i;
						opi++;
						
						// check left
						tmpi = i - 1;
						match = 1;
						while(tmpi >= 0 && match )
						{
							switch( expr[tmpi] )
							{
								//case '^' :
								case '*' :
								case '/' :
									andi++;
									
									match = 0;
									break;
								case ')' :
									tmpdepth = 1;
									tmpi--;
									while(tmpdepth > 0)
									{
										if(expr[tmpi] == ')')
											tmpdepth++;
										else if(expr[tmpi] == '(')
											tmpdepth--;
										tmpi--;
									}
									tmpi += 2;
									
									while( isblank(expr[tmpi]) )
										tmpi++;
									
									if(expr[tmpi] == '-')
									{
										tmpi++;
										while( isblank(expr[tmpi]) )
											tmpi++;
										
										if( isalpha ( expr[tmpi] ) )
										{
											if ( /* *varList */ formula->vars == NULL )
											{
												/* *varList */ formula->vars = startVarList( expr+tmpi, andi, -1.0f );
												curVar = formula->vars /* *varList */;
											}
											else
												curVar = addVar( curVar, expr+tmpi, andi, -1.0f);
										}
										else
											(formula->operands)[andi] = -1.0f * strtof(expr+tmpi, NULL);
									}
									
									
									andi++;
									
									match = 0;
									break;
									
								case '(' :
								case '+' :
								case '-' :
									
									tmpi++;
									while ( isblank ( expr[tmpi] ) )
										tmpi++;
									
									if( isalpha ( expr[tmpi] ) )
									{
										if ( /* *varList */ formula->vars == NULL )
										{
											/* *varList */ formula->vars = startVarList( expr+tmpi, andi, 1.0f );
											curVar = formula->vars /* *varList */;
										}
										else
											curVar = addVar( curVar, expr+tmpi, andi, 1.0f);
										
										
									}
									else
										(formula->operands)[andi] = strtof(expr+tmpi, NULL);
									
									andi++;
									match = 0;
							}
							
							
							tmpi--;
						}
						if (tmpi < 0 && match)
						{
							tmpi = 0;
							
							while ( isblank ( expr[tmpi] ) )
								tmpi++;
							
							if( isalpha ( expr[tmpi] ) )
							{
								if (formula->vars == NULL )
								{
									formula->vars = startVarList( expr+tmpi, andi, 1.0f );
									curVar = formula->vars;
								}
								else
									curVar = addVar( curVar, expr+tmpi, andi, 1.0f);
								
								
							}
							else
							(formula->operands)[andi] = strtof(expr+tmpi, NULL);
							
							andi++;
						}
						
						// check right
						tmpi = i + 1;
						match = 1;
						while(tmpi < n && match )
						{
							switch( expr[tmpi] )
							{
								/*case '^' :
									andi++;
									
									match = 0;
									break;
								*/
								case '*' :
								case '/' :
								case '+' :
								case '-' :
								case ')' :
									
									tmpi = i + 1;
									while( isblank(expr[tmpi]) )
										tmpi++;
									
									if( isalpha ( expr[tmpi] ) )
									{
										if (formula->vars == NULL )
										{
											formula->vars = startVarList( expr+tmpi, andi, 1.0f );
											curVar = formula->vars;
										}
										else
											curVar = addVar( curVar, expr+tmpi, andi, 1.0f);
									}
									else
									(formula->operands)[andi] = strtof(expr+tmpi, NULL);
									
									andi++;
									
									match = 0;
									break;
								case '(' :
									
									tmpi++;
									while( isblank(expr[tmpi]) )
										tmpi++;
									
									if(expr[tmpi] == '-')
									{
										tmpi++;
										while( isblank(expr[tmpi]) )
											tmpi++;
										
										if( isalpha ( expr[tmpi] ) )
										{
											if (formula->vars == NULL )
											{
												formula->vars = startVarList( expr+tmpi, andi, -1.0f );
												curVar = formula->vars;
											}
											else
												curVar = addVar( curVar, expr+tmpi, andi, -1.0f );
										}
										else
											(formula->operands)[andi] = -1.0f * strtof(expr+tmpi, NULL);
										
									}
									
									andi++;
									
									match = 0;
							}
							
							
							tmpi++;
						}
						if (tmpi >= n && match)
						{
							tmpi = i + 1;
							while( isblank(expr[tmpi]) )
								tmpi++;
							
							if( isalpha ( expr[tmpi] ) )
							{
								if (formula->vars == NULL )
								{
									formula->vars = startVarList( expr+tmpi, andi, 1.0f );
									curVar = formula->vars;
								}
								else
									curVar = addVar( curVar, expr+tmpi, andi, 1.0f);
							}
							else
								(formula->operands)[andi] = strtof(expr+tmpi, NULL);
							
							andi++;
						}
				}
			}
			
		}
		// * / end
		
		// get + - operators and their operands
		for(i = 0, depth = 0; i < n; i++)
		{
			if (expr[i] == '(')
			{
				tmpi = i + 1;
				while ( isblank(expr[tmpi]) )
					tmpi++;
				
				if( expr[tmpi] == '-' )
				{
					while(expr[i] != ')')
						i++;
				}
				else
				depth++;
			}
			else if(expr[i] == ')')
				depth--;
			else if(depth == maxdepth)
			{
				 
				switch( expr[i] )
				{
					case '+' :
					case '-' :
						(formula->operators)[opi] = expr[i];
						(formula->opPos)[opi] = i;
						opi++;
						
						// check  left
						tmpi = i - 1;
						match = 1;
						while(tmpi >= 0 && match )
						{
							switch( expr[tmpi] )
							{
								//case '^' :
								case '*' :
								case '/' :
								case '+' :
								case '-' :
									andi++;
									
									match = 0;
									break;
								case ')' :
									tmpdepth = 1;
									tmpi--;
									while(tmpdepth > 0)
									{
										if(expr[tmpi] == ')')
											tmpdepth++;
										else if(expr[tmpi] == '(')
											tmpdepth--;
										tmpi--;
									}
									tmpi += 2;
									
									while( isblank(expr[tmpi]) )
										tmpi++;
									
									if(expr[tmpi] == '-')
									{
										tmpi++;
										while( isblank(expr[tmpi]) )
											tmpi++;
										
										if( isalpha ( expr[tmpi] ) )
										{
											if (formula->vars == NULL )
											{
												formula->vars = startVarList( expr+tmpi, andi, -1.0f );
												curVar = formula->vars;
											}
											else
												curVar = addVar( curVar, expr+tmpi, andi, -1.0f);
										}
										else
											(formula->operands)[andi] = -1.0f * strtof(expr+tmpi, NULL);
									}
									
									andi++;
									
									match = 0;
									break;
									
								case '(' :
									tmpi++;
									while( isblank(expr[tmpi]) )
										tmpi++;
									
									if( isalpha ( expr[tmpi] ) )
									{
										if (formula->vars == NULL )
										{
											formula->vars = startVarList( expr+tmpi, andi, 1.0f );
											curVar = formula->vars;
										}
										else
											curVar = addVar( curVar, expr+tmpi, andi, 1.0f);
									}
									else
										(formula->operands)[andi] = strtof(expr+tmpi, NULL);
									
									andi++;
									match = 0;
							}
							
							
							tmpi--;
						}
						if (tmpi < 0 && match)
						{
							tmpi = 0;
							while( isblank(expr[tmpi]) )
								tmpi++;
							
							if( isalpha ( expr[tmpi] ) )
							{
								if (formula->vars == NULL )
								{
									formula->vars = startVarList( expr+tmpi, andi, 1.0f );
									curVar = formula->vars;
								}
								else
									curVar = addVar( curVar, expr+tmpi, andi, 1.0f);
							}
							else
								(formula->operands)[andi] = strtof(expr+tmpi, NULL);
							andi++;
						}
						
						// check right
						tmpi = i + 1;
						match = 1;
						while(tmpi < n && match )
						{
							switch( expr[tmpi] )
							{
								case '+' :
								case '-' :
								case ')' :
									tmpi = i + 1;
									while( isblank(expr[tmpi]) )
										tmpi++;
									
									if( isalpha ( expr[tmpi] ) )
									{
										if (formula->vars == NULL )
										{
											formula->vars = startVarList( expr+tmpi, andi, 1.0f );
											curVar = formula->vars;
										}
										else
											curVar = addVar( curVar, expr+tmpi, andi, 1.0f);
									}
									else
										(formula->operands)[andi] = strtof(expr+tmpi, NULL);
									
									andi++;
									match = 0;
									break;
								case '(' :
									
									tmpi++;
									while( isblank(expr[tmpi]) )
										tmpi++;
									
									if(expr[tmpi] == '-')
									{
										tmpi++;
										while( isblank(expr[tmpi]) )
											tmpi++;
										
										if( isalpha ( expr[tmpi] ) )
										{
											if (formula->vars == NULL )
											{
												formula->vars = startVarList( expr+tmpi, andi, -1.0f );
												curVar = formula->vars;
											}
											else
												curVar = addVar( curVar, expr+tmpi, andi, -1.0f);
										}
										else
											(formula->operands)[andi] = -1.0f * strtof(expr+tmpi, NULL);
									}
									
									andi++;
									match = 0;
									break;
								//case '^' :
								case '*' :
								case '/' :
									andi++;
									match = 0;
							}
							
							
							tmpi++;
						}
						if (tmpi >= n && match)
						{
							tmpi = i + 1;
							while( isblank(expr[tmpi]) )
								tmpi++;
							
							if( isalpha ( expr[tmpi] ) )
							{
								if (formula->vars == NULL )
								{
									formula->vars = startVarList( expr+tmpi, andi, 1.0f );
									curVar = formula->vars;
								}
								else
									curVar = addVar( curVar, expr+tmpi, andi, 1.0f);
							}
							else
								(formula->operands)[andi] = strtof(expr+tmpi, NULL);
							
							andi++;
						}
				}
			}
			
		}
		// + - end
		
		maxdepth--;
	}
	
	
	fillOrder ( formula );
}



int depthcheck( char * expr, int n )
{
	int i, depth, maxdepth, pos;
	
	for(i = 0, depth = 0, maxdepth = 0; i < n; i++)
	{
		
		
		if (expr[i] == '(')
		{
			pos = i;
			i++;
			while( isblank(expr[i]) )
				i++;
			
			if(expr[i] != '-')
			{
				depth++;
				
				if(depth > maxdepth)
					maxdepth = depth;
				
				i = pos;
			}
			else
				while(expr[i] != ')')
					i++;
			
		}
		else if(expr[i] == ')')
			depth--;
	}
	
	
	return maxdepth;
}


int signcheck(char * expr, int n)
{
	int i, pass = 1, pos;
	
	for(i = 0; i < n && pass; i++)
	{
		
		if (expr[i] == '(')
		{
			pos = i;
			i++;
			while( isblank(expr[i]) && i < n )
				i++;
			
			switch (expr[i])
			{
				case '-' :
					i++;
					while( isblank(expr[i]) && i < n )
						i++;
					if( !isalnum(expr[i]) )
						pass = 0;
					
					while(expr[i] != ')' && i < n && pass)
					{
						i++;
						switch (expr[i])
						{
							case '+' :
							case '-' :
							case '*' :
							case '/' :
							case '(' :
							//case '^' :
								pass = 0;
						}
					}
					break;
					
				case '+' :
				case '*' :
				case '/' :
				//case '^' :
					pass = 0;
			}
			
			
			i = pos;
		}
		
		
	}
	
	
	return pass;
}


int gramcheck(char * expr, int n)
{
	int i, pass = 1;
	
	for(i = 0; i < n && pass; i++)
	{
		
		if ( ! isalnum(expr[i]) )
		{
			
			switch (expr[i])
			{
				case '+' :
				case '-' :
				case '*' :
				case '/' :
				case '(' :
				case ')' :
				//case '^' :
				case '.' :
				case ' ' :
				case '\t' :
					break;
				default :
					pass = 0;
			}
		}
	}
	
	
	return pass;
}


int allchecks(char * expr, int n)
{
	int pass = 0;
	
	if( gramcheck(expr, n) )
		if( parenthcheck(expr, n) )
			if( popcheck(expr, n) )
				if( operatorcheck(expr, n) )
					if( numbercheck(expr, n) )
						if( signcheck(expr, n) )
							if( validparenth(expr, n) )
								pass = 1;
	
	
	return pass;
}


int opcount(char * expr, int n)
{
	int i, opc, pos;
	
	for(i = 0, opc = 0; i < n; i++)
	{
			
		switch (expr[i])
		{
			
			case '-' :
				pos = i - 1;
				while( isblank(expr[pos]) )
					pos--;
				if( expr[pos] != '(' )
					opc++;
				break;
			case '*' :
			case '/' :
			case '+' :
			//case '^' :
				opc++;
				
		}
	}
	
	
	return opc;
}



int validparenth(char * expr, int n)
{
	int depth, tmpi, i, opc, pass = 1;
	
	
	for(i = 0; i < n; i++)
	{
		if(expr[i] == '(')
		{
			tmpi = i + 1;
			depth = 1;
			opc = 0;
			while(depth > 0)
			{
				if(expr[tmpi] == '(')
					depth++;
				else if(expr[tmpi] == ')')
					depth--;
				else if (depth == 1)
				{
					switch (expr[tmpi])
					{
						case '+' :
						case '-' :
						case '*' :
						case '/' :
						//case '^' :
							opc++;
						
					}
				}
				tmpi++;
			}
			
			if(opc == 0)
				pass = 0;
		}
	}
	
	return pass;
}

int popcheck(char * expr, int n)
{
	int tmpi, i, pass = 1;
	
	
	for(i = 0; i < n; i++)
	{
		if(expr[i] == ')')
		{
			tmpi = i + 1;
			while(tmpi < n && isblank(expr[tmpi]) )
				tmpi++;
			
			if(expr[tmpi] == '(')
				pass = 0;
		}
	}
	
	return pass;
}

void fillOrder ( exprrecipe * test1 )
{
	int itmp, i, ihead;
	
	for ( i = 0; i < test1->opcount - 1; i++ )
	{
			ihead = i + 1;
			itmp = ihead + 1;
			while ( itmp < test1->opcount )
			{
				if( (test1->opPos)[itmp] > (test1->opPos)[i] && (test1->opPos)[itmp] < (test1->opPos)[ihead] )
				{
					ihead++;
					itmp = ihead + 1;
				}
				else if ( (test1->opPos)[itmp] < (test1->opPos)[i] && (test1->opPos)[itmp] > (test1->opPos)[ihead] )
				{
					ihead++;
					itmp = ihead + 1;
				}
				else
					itmp++;
			}
			
			if ( (test1->opPos)[i] > (test1->opPos)[ihead] )
			{
				(test1->fillorder)[i] = ihead * 2 + 1;
				
			}
			else 
			{
				(test1->fillorder)[i] = ihead * 2;
				
			}
			
	}
		ihead = i + 1;
		(test1->fillorder)[i] = ihead * 2;
	
}

void freeExprRecipe ( exprrecipe * test1 )
{
	if( test1 != NULL )
	{
		free(test1->operators);
		free(test1->operands);
		free(test1->fillorder);
		free(test1->opPos);
	}
}



variable * addVar( variable * curVar, char * name, int pos, float sign)
{
	int chi = 0;
	variable * newVar = NULL;
	
	curVar->next = malloc ( sizeof (*curVar) );
	if( curVar->next != NULL )
	{
		curVar = curVar->next;
		
		while( isalpha ( name[chi] ) && chi < STATLEN - 1 )
		{
			(curVar->name)[chi] = name[chi] ;
			
			chi++;
		}
		(curVar->name)[chi] = '\0';
		
		curVar->operandPos = pos;
		
		curVar->sign = sign;
		
		curVar->next = NULL;
		
		newVar = curVar;
	}
	
	return newVar;
}



variable * startVarList( char * name, int pos, float sign )
{
	int chi = 0;
	variable * varList;
	
	varList = malloc ( sizeof (*varList) );
	if( varList == NULL )
		exit(EXIT_FAILURE);
	
	
	while( isalpha ( name[chi] ) && chi < STATLEN - 1 )
	{
		(varList->name)[chi] = name[chi] ;
		
		chi++;
	}
	(varList->name)[chi] = '\0';
	
	varList->operandPos = pos;
	
	varList->sign = sign;
	
	varList->next = NULL;
	
	return varList;
}

void cleanVarList( variable * varList )
{
	variable * nextVar;
	
	while( varList != NULL )
	{
		nextVar = varList->next;
		free ( varList );
		varList = nextVar;
	}
}