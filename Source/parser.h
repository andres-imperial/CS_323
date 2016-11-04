// ----------------------------------------------------------------------------
// Andres Imperial
// CSCP 323 mw 11:30am
// Assignment 2: Parser 
//
// file: parser.h
// ----------------------------------------------------------------------------

#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

bool Parser(string fileName)
{

	// Code ....

}

// ----- Rat16F ---------------------------------------------------------------
// ----------------------------------------------------------------------------
bool Rat16F(void)
{

	// Create compile flag
	bool compile = false;
	// Program must start with $$ marker
	if (lexeme_is("$$")){
		if (Opt_Funct_Def()){
			if (lexeme_is("$$")){
				if (Opt_Declar_List()){
					if (Statement_List()){
						if (lexeme_is("$$")){
							// File was syntactically correct.
							compile = true;
						}
					}
				}
			}
		}
	}

	return compile;

} // End of Rat16F()


// ----- Opt_Funct_Def --------------------------------------------------------
// ----------------------------------------------------------------------------
bool Opt_Funct_Def(void)
{

	// Create compile flag
	bool compile = true;
	
	if (Funct_Def()){
		return compile;
	}

	// It was empty, but acceptable
	return compile;

} // End of Opt_Funct_Def()	


// ----- Opt_Declar_List ------------------------------------------------------
// ----------------------------------------------------------------------------
bool Opt_Declar_List(void)
{

	// Create compile flag
	bool compile = true;

	if (Declar_List()){
		return compile;
	}

	// It was empty, but acceptable
	return compile;

} // End of Opt_Declar_List()


// ----- Statement_List ------------------------------------------------------
// ----------------------------------------------------------------------------
bool Statement_List(void)
{
	
	// Create compile flag
	bool compile = true;
	
	if (Statement()){
		if Statement_List(){
		}
	}
	else{
		// Fail on Statement()
		compile = false;
	}

	return compile;

} // End of Statement_List()

		
// ----- Funct_Def ------------------------------------------------------------
// ----------------------------------------------------------------------------
bool Funct_Def(void)
{
	
	// Create compile flag
	bool compile = true;
	
	if (Function()){
		if (Funct_Def()){
		}
	}
	else{
		// Fail on Function()
		compile = false;
	}

	return compile;

} // End of Funct_Def()


// ----- Function ------------------------------------------------------------
// ----------------------------------------------------------------------------
bool Function(void)
{
	
	// Create compile flag
	bool compile = true;

	// Function must start with keyword function
	if (lexeme_is("function")){
		if (Identifier()){
			if (lexeme_is("[")){
				if (Opt_Param_List()){
					if (lexeme_is("]")){
						if (Opt_Declar_List()){
							if (Body()){
							}
							else{
								// Failed on Body()
								compile = false;
							}
						}
						else{
							// Failed on Opt_Declar_List()
							compile = false;
						}
					}
					else{
						// Failed on lexeme_is("]")
						compile = false;
					}
				}
				else{
					// Failed on Opt_Param_List()
					compile = false;
				}
			}
			else{
				// Failed on lexeme_is("[")
				compile = false;
			}
		}
		else{
			// Failed on Identifier()
			compile = false;
		}
	}
	else{
		// Failed on lexeme_is("function")
		compile = false;
	}

	return compile;

} // End of Function()


// ----- Identifier -----------------------------------------------------------
// ----------------------------------------------------------------------------
bool Identifier(void)
{
	
	// Create compile flag
	bool compile = true;

	if(!token_is("identifer")){
		// Failed on token_is identifer
		compile = false;
	}

	return compile;
	
} // End of Identifier()


// ----- Opt_Param_List -----------------------------------------------------------
// ----------------------------------------------------------------------------
bool Opt_Param_List(void)
{
	
	// Create compile flag
	bool compile = true;

	if(Param_List()){
		return compile;
	}

	// It was empty but acceptable.
	return compile;

} // End of Opt_Param_List()

	
// ----- Body -----------------------------------------------------------
// ----------------------------------------------------------------------------
bool Body(void)
{
	
	// Create compile flag
	bool compile = true;

	// Body must start with {
	if(lexeme_is("{")){
		if(Statement_List()){
			if(lexeme_is("}"){
				// used <Body>  ::=  {  < Statement List>  } 
			}
			else{
				// failed on lexeme_is("}")
				compile = false;
			}
		}
		else{
			// failed on Statement_List()
			compile = false;
		}
	}
	else{
		// failed on lexeme_is("{")
		compile = false;
	}

	return compile;

} // End of Body()

