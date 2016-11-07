// ----------------------------------------------------------------------------
// Andres Imperial
// CSCP 323 mw 11:30am
// Assignment 2: Parser
//
// file: parser.h
// ----------------------------------------------------------------------------

#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <fstream>
#include "parser_functs.h"
using namespace std;
const int token = 0;
const int lexeme = 1;
// Global variables
string** lexerArr;
int arrIndex = 1;
ofstream parserFile;

bool Parser(string fileName, string sourceName, int lineCount)
{

	ifstream lexerFile;

	cout << "lineCount: " << lineCount << endl;

	lexerArr = new string*[lineCount + 1];
	bool compile = true;

	// Open file for reading
	lexerFile.open(fileName.c_str());
	parserFile.open((string("parser_") + sourceName).c_str());

	// If file opened properly
	if(!lexerFile.is_open()){
		cout << "Error file -- " << fileName << " -- could not be opened!\n";
		return 0;
	}
	if(!parserFile.is_open()){
		cout << "Error -- parser.txt could not be opened.\n";
		return 0;
	}
	
	for(int i = 0; !lexerFile.eof(); ++i){
		lexerArr[i] = new string[2];
		// Load array
		lexerFile >> lexerArr[i][token]; // Token
		lexerFile >> lexerArr[i][lexeme]; // Lexeme
	}

	// Start the Parsing at the root
	if(!Rat16F()){
		// Failed Parsing
		cout << "Error unable to parse file!\n";
		compile = false;
	}

	// Delete dynamic memory
	for(int i = 0; i < lineCount + 1; ++i){
		delete[] lexerArr[i];
	}
	delete[] lexerArr;

	lexerFile.close();
	parserFile.close();

	return compile;

} // End of Parser()


// ----- ErrorMsg =------------------------------------------------------------
// ----------------------------------------------------------------------------
void ErrorMsg(string msg)
{
	// Print out error message with line number, given token and lexeme, and 
	// expected lexeme or token
	
	cout << "Error on line: " << arrIndex << " -- expected " << msg 
		<< " instead received lexeme: " << lexerArr[arrIndex][lexeme]
		<< " token type: " << lexerArr[arrIndex][token] << endl;

	// Exit program
	exit(0);

} // End of ErrorMsg()


// ----- lexeme_is ------------------------------------------------------------
// ----------------------------------------------------------------------------
bool lexeme_is(string target)
{
	
	// Create compile flag
	bool compile = true;

	if(lexerArr[arrIndex][lexeme] == target){
		++arrIndex;

		parserFile << "\nToken: " << lexerArr[arrIndex][token] << "\tLexeme: "
		<< lexerArr[arrIndex][lexeme] << endl;
	}
	else{
		// Lexemes did not match
		compile = false;
	}

	return compile;

} // End of lexeme_is()


// ----- token_is ---------------------------------------------------------------
// ----------------------------------------------------------------------------
bool token_is(string target)
{

	// Create compile flag
	bool compile = true;

	if(lexerArr[arrIndex][token] == target){
		++arrIndex;

		parserFile << "\nToken: " << lexerArr[arrIndex][token] << "\tLexeme: "
		<< lexerArr[arrIndex][lexeme] << endl;
	}
	else{
		// Tokens did not match
		compile = false;
	}

	return compile;

} // End of token_is()


// ----- Rat16F ---------------------------------------------------------------
// ----------------------------------------------------------------------------
bool Rat16F(void)
{

	// First Production Rule
	parserFile << "Token: " << lexerArr[arrIndex][token] << "\tLexeme: "
		<< lexerArr[arrIndex][lexeme] << endl;
	parserFile << "<Rat16F> -> $$ <Opt Function Definitions>\n"
				"$$ <Opt Declaration List> <Statement List> $$\n";

	// Create compile flag
	bool compile = false;
	// Program must start with $$ marker
	if(lexeme_is("$$")){
		if(Opt_Funct_Def()){
			if(lexeme_is("$$")){
				if(Opt_Declar_List()){
					if(Statement_List()){
						if(lexeme_is("$$")){
							// File was syntactically correct.
							compile = true;
						}
						else{
							ErrorMsg("$$");
						}
					}
					else{
						ErrorMsg("<Statement_List>");
					}
				}
			}
			else{
				ErrorMsg("$$");
			}
		}
	}
	else{
		ErrorMsg("$$");
	}

	return compile;

} // End of Rat16F()


// ----- Opt_Funct_Def --------------------------------------------------------
// ----------------------------------------------------------------------------
bool Opt_Funct_Def(void)
{

	// Production Rule
	parserFile << "<Opt Function Definitions> -> <Function Definitions> | <Empty>\n";

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

	// Production Rule
	parserFile << "<Opt Declaration List> -> <Declaration List> | <Empty>\n";

	// Create compile flag
	bool compile = true;

	if (Declar_List()){
		return compile;
	}

	// It was empty, but acceptable
	return compile;

} // End of Opt_Declar_List()


// ----- Statement_List -------------------------------------------------------
// ----------------------------------------------------------------------------
bool Statement_List(void)
{

	// Production Rule
	parserFile << "<Statement List> -> <Statement> | <Statement> <Statement List>\n";

	// Create compile flag
	bool compile = true;

	if(Statement()){
		while(Statement());
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

	// Production Rule
	parserFile << "<Function Definitions> -> <Function> | "
			"<Function> <Function Definitions>\n";

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

	// Production Rule
	parserFile << "<Function> -> function  <Identifier> [ <Opt Parameter List> ] "
			"<Opt Declaration List> <Body>\n";

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
						ErrorMsg("]");
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
				ErrorMsg("[");
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

	if(!token_is("identifier")){
		// Failed on token_is identifer
		compile = false;
	}

	return compile;

} // End of Identifier()


// ----- Opt_Param_List -------------------------------------------------------
// ----------------------------------------------------------------------------
bool Opt_Param_List(void)
{

	// Production Rule
	parserFile << "<Opt Parameter List> ->  <Parameter List> | <Empty>\n";

	// Create compile flag
	bool compile = true;

	if(Param_List()){
		return compile;
	}

	// It was empty but acceptable.
	return compile;

} // End of Opt_Param_List()


// ----- Body -----------------------------------------------------------------
// ----------------------------------------------------------------------------
bool Body(void)
{

	// Production Rule
	parserFile << "<Body> -> { <Statement List> }\n";

	// Create compile flag
	bool compile = true;

	// Body must start with {
	if(lexeme_is("{")){
		if(Statement_List()){
			if(lexeme_is("}")){
				// used <Body>  ::=  {  < Statement List>  }
			}
			else{
				// failed on lexeme_is("}")
				ErrorMsg("}");
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
		ErrorMsg("{");
		compile = false;
	}

	return compile;

} // End of Body()


// ----- Param_List -----------------------------------------------------------
// ----------------------------------------------------------------------------
bool Param_List(void)
{

	// Production Rule
	parserFile << "<Parameter List> -> <Parameter> | <Parameter> , "
			"<Parameter List>\n";

	// Create compile flag
	bool compile = true;

    if(Parameter()){
		while(lexeme_is(",")){
			Parameter();
		}
    }
    else{
        // Failed on Parameter()
        compile = false;
    }

    return compile;

} // End of Param_List()


// ----- Parameter ------------------------------------------------------------
// ----------------------------------------------------------------------------
bool Parameter(void)
{

	// Production Rule
	parserFile << "<Parameter> -> <IDs> : <Qualifier>\n";

	// Create compile flag
	bool compile = true;

    if(IDs()){
        if(lexeme_is(":")){
            if(Qualifier()){
                // <Parameter> ::=  <IDs > : <Qualifier>
            }
            else{
                // Failed on Qualifier()
                compile = false;
            }
        }
        else{
            // Failed on lexeme_is(":")
			ErrorMsg(":");
            compile = false;
        }
    }
    else{
        // Failed on IDs()
        compile = false;
    }

    return compile;

} // End of Parameter()

// ----- IDs ------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool IDs(void)
{

	// Production Rule
	parserFile << "<IDs> -> <Identifier> | <Identifier>, <IDs>\n";

	// Create compile flag
	bool compile = true;

    if(Identifier()){
		while(lexeme_is(",")){
			if(Identifier()){
			}
		}
    }
    else{
        // Failed on Identifier()
		ErrorMsg("<identifier>");
        compile = false;
    }

    return compile;

} // End of IDs()


// ----- Qualifier ------------------------------------------------------------
// ----------------------------------------------------------------------------
bool Qualifier(void)
{

	// Production Rule
	parserFile << "<Qualifier> -> integer | boolean | real\n";

	// Create compile flag
	bool compile = true;

    if(lexeme_is("integer") | lexeme_is("boolean") | lexeme_is("real")){
        // <Qualifier> ::= integer   |  boolean  |  real
    }
    else{
        // Failed on lexeme_is() expected.....
        compile = false;
    }

    return compile;

} // End of Qualifier()


// ----- Declar_List ----------------------------------------------------------
// ----------------------------------------------------------------------------
bool Declar_List(void)
{

	// Production Rule
	parserFile << "<Declaration List> -> <Declaration> ; | <Declaration> ; "
			"<Declaration List>\n";

	// Create compile flag
	bool compile = true;

    if(Declaration()){
        if(lexeme_is(";")){
            Declar_List();
        }
        else{
            // Failed on lexeme_is(";")
			ErrorMsg(";");
            compile = false;
        }
    }
    else{
        // Failed on Declaration()
        compile = false;
    }

    return compile;

} // End of Declar_List()


// ----- Declaration ----------------------------------------------------------
// ----------------------------------------------------------------------------
bool Declaration(void)
{

	// Production Rule
	parserFile << "<Declaration> -> <Qualifier> <IDs>\n";
	// Create compile flag
	bool compile = true;

    if(Qualifier()){
        if(IDs()){
            // <Declaration> ::=  <Qualifier > <IDs>
        }
        else{
            // Failed on IDs()
            compile = false;
        }
    }
    else{
        // Failed on Qualifier()
        compile = false;
    }

    return compile;

} // End of Declaration()


// ----- Statement ------------------------------------------------------------
// ----------------------------------------------------------------------------
bool Statement(void)
{

	// Production Rule
	parserFile << "<Statement> -> <Compound> | <Assign> | <If> |  <Return> | "
				"<Write> | <Read> | <While>\n";
	
	// Create compile flag
	bool compile = true;

    if(Compound() | Assign() | If() | Return() | Write() | Read() | While()){
        // <Statement> ::=  <Compound> | <Assign> | <If> |  <Return> | <Write>
        // | <Read> | <While>
    }
    else{
        // Failed on <Compound> | <Assign> | <If> |  <Return>
        // | <Write> | <Read> | <While>
        compile = false;
    }

    return compile;

} // End of Statement()


// ----- Compound -------------------------------------------------------------
// ----------------------------------------------------------------------------
bool Compound(void)
{

	// Production Rule
	parserFile << "<Compound> -> { <Statement List> }\n";

	// Create compile flag
	bool compile = true;

    // Must start with "{"
    if(lexeme_is("{")){
        if(Statement_List()){
            if(lexeme_is("}")){
                // <Compound> ::= {  <Statement List>  }
            }
            else{
                // Failed on lexeme_is("}")
				ErrorMsg("}");
                compile = false;
            }
        }
        else{
            // Failed on Statement_List()
            compile = false;
        }
    }
    else{
        // Failed on lexeme_is("{")
        compile = false;
    }

    return compile;

} // End of Compound()


// ----- Assign ---------------------------------------------------------------
// ----------------------------------------------------------------------------
bool Assign(void)
{

	// Production Rule
	parserFile << "<Assign> -> <Identifier> := <Expression>;\n";

	// Create compile flag
	bool compile = true;

    if(Identifier()){
        if(lexeme_is(":=")){
            if(Expression()){
                if(lexeme_is(";")){
                    // <Assign> ::=   <Identifier> := <Expression> ;
                }
                else{
                    // Failed on lexeme_is(";")
					ErrorMsg(";");
                    compile = false;
                }
            }
            else{
                // Failed on Expression()
                compile = false;
            }
        }
        else{
            // Failed on lexeme_is(":=")
			ErrorMsg(":=");
            compile = false;
        }
    }
    else{
       // Failed on Identifier()
       compile = false;
    }

    return compile;

} // End of Assign()


// ----- If -------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool If(void)
{

	// Production Rule
	parserFile << "<If> -> if (<Condition>) <Statement> endif |\n"
		        "if (<Condition>) <Statement> else <Statement> endif\n";

	// Create compile flag
	bool compile = true;

    // Must start with "if" keyword
    if(lexeme_is("if")){
        if(lexeme_is("(")){
            if(Condition()){
                if(lexeme_is(")")){
                    if(Statement()){
                        if(lexeme_is("endif")){
                            // <If> ::=     if  ( <Condition>  ) <Statement>
                            // endif    |
                        }
                        else if(lexeme_is("else")){
                            if(Statement()){
                                if(lexeme_is("endif")){
                                    // <If> ::= if ( <Condition>  ) <Statement>
                                    // else <Statement> endif
                                }
                                else{
                                    // Failed on lexeme_is("endif")
									ErrorMsg("endif");
                                    compile = false;
                                }
                            }
                            else{
                                // Failed on Statement()
                                compile = false;
                            }
                        }
                        else{
                            // Failed on lexeme_is() expected endif or else
							ErrorMsg("endif | else");
                            compile = false;
                        }
                    }
                    else{
                        // Failed on Statement()
                        compile = false;
                    }
                }
                else{
                    // Failed on lexeme_is(")")
					ErrorMsg(")");
                    compile = false;
                }
            }
            else{
                // Failed on Condition()
                compile = false;
            }
        }
        else{
            // Failed on lexeme_is("(")
			ErrorMsg("(");
            compile = false;
        }
    }
    else{
        // Failed on lexeme_is("if")
        compile = false;
    }

    return compile;

} // End of If()


// ----- Return ---------------------------------------------------------------
// ----------------------------------------------------------------------------
bool Return(void)
{

	// Production Rule
	parserFile << "<Return> -> return ; | return <Expression> ;\n";

	// Create compile flag
	bool compile = true;

    if(lexeme_is("return")){
        if(lexeme_is(";")){
            // <Return> ::=  return ;
        }
        else if(Expression()){
            if(lexeme_is(";")){
                // <Return> ::= return <Expression> ;
            }
            else{
                // Failed on lexeme_is(";")
				ErrorMsg(";");
                compile = false;
            }
        }
        else{
            // Failed, expected ";" or Expression
			ErrorMsg("; | <Expression>");
            compile = false;
        }
    }
    else{
        // Failed on lexeme_is("return")
        compile = false;
    }

    return compile;

} // End of Return()


// ----- Write ----------------------------------------------------------------
// ----------------------------------------------------------------------------
bool Write(void)
{

	// Production Rule
	parserFile << "<Write> -> print (<Expression>);\n";

	// Create compile flag
	bool compile = true;

    // Must start with keyword "print"
    if(lexeme_is("print")){
        if(lexeme_is("(")){
            if(Expression()){
                if(lexeme_is(")")){
                    if(lexeme_is(";")){
                        // <Write> ::=   print ( <Expression>);
                    }
                    else{
                        // Failed on lexeme_is(";")
						ErrorMsg(";");
                        compile = false;
                    }
                }
                else{
                    // Failed on lexeme_is(")")
					ErrorMsg(")");
                    compile = false;
                }
            }
            else{
                // Failed on Expression()
                compile = false;
            }
        }
        else{
            // Failed on lexeme_is("(")
			ErrorMsg("(");
            compile = false;
        }
    }
    else{
        // Failed on lexeme_is("print")
        compile = false;
    }

    return compile;

} // End of Write()


// ----- Read -----------------------------------------------------------------
// ----------------------------------------------------------------------------
bool Read(void)
{

	// Production Rule
	parserFile << "<Read> -> read (<IDs>);\n";

	// Create compile flag
	bool compile = true;

    // Must start with keyword "read"
    if(lexeme_is("read")){
        if(lexeme_is("(")){
            if(IDs()){
                if(lexeme_is(")")){
                    if(lexeme_is(";")){
                        // <Read> ::=    read ( <IDs> );
                    }
                    else{
                        // Failed on lexeme_is(";")
						ErrorMsg(";");
                        compile = false;
                    }
                }
                else{
                    // Failed on lexeme_is(")")
					ErrorMsg(")");
                    compile = false;
                }
            }
            else{
                // Failed on IDs()
                compile = false;
            }
        }
        else{
            // Failed on lexeme_is("(")
			ErrorMsg("(");
            compile = false;
        }
    }
    else{
        // Failed on lexeme_is("read")
        compile = false;
    }

    return compile;

} // End of Read()


// ----- While ----------------------------------------------------------------
// ----------------------------------------------------------------------------
bool While(void)
{

	// Production Rule
	parserFile << "<While> -> while (<Condition>) <Statement>\n";

	// Create compile flag
	bool compile = true;

    // Must start with keyword "while"
    if(lexeme_is("while")){
        if(lexeme_is("(")){
            if(Condition()){
                if(lexeme_is(")")){
                    if(Statement()){
                        // <While> ::= while ( <Condition>  )  <Statement>
                    }
                    else{
                        // Failed on Statement()
                        compile = false;
                    }
                }
                else{
                    // Failed on lexeme_is(")")
					ErrorMsg(")");
                    compile = false;
                }
            }
            else{
                // Failed on Condition()
                compile = false;
            }
        }
        else{
            // Failed on lexeme_is("(")
			ErrorMsg("(");
            compile = false;
        }
    }
    else{
        // Failed on lexeme_is("while")
        compile = false;
    }

    return compile;

}// End of While()


// ----- Expression -----------------------------------------------------------
// ----------------------------------------------------------------------------
bool Expression(void)
{

	// Production Rule
	parserFile << "<Expression> -> <Term> <Expression Prime>\n";

	// Create compile flag
	bool compile = true;

    if(Term()){
        if(Expression_Prime()){
            // <Expression> ::= <Term> <Expression Prime>
        }
        else{
            // Failed on Expression_Prime()
            compile = false;
        }
    }
    else{
        // Failed on Term()
        compile = false;
    }

    return compile;

} // End of Expression()


// ----- Condition ------------------------------------------------------------
// ----------------------------------------------------------------------------
bool Condition(void)
{

	// Production Rule
	parserFile << "<Condition> -> <Expression> <Relop> <Expression>\n";

	// Create compile flag
	bool compile = true;

    if(Expression()){
        if(Relop()){
            if(Expression()){
                // <Condition> ::= <Expression> <Relop> <Expression>
            }
            else{
                // Failed on Expression()
                compile = false;
            }
        }
        else{
            // Failed on Relop()
            compile = false;
        }
    }
    else{
        // Failed on Expression()
        compile = false;
    }

    return compile;

} // End of Condition()


// ----- Relop ----------------------------------------------------------------
// ----------------------------------------------------------------------------
bool Relop(void)
{

	// Production Rule
	parserFile << "<Relop> -> = | /= | > | < | => | <=\n";

	// Create compile flag
	bool compile = true;

    if(lexeme_is("=") | lexeme_is("/=") | lexeme_is(">") | lexeme_is("<") |
        lexeme_is("=>") | lexeme_is("<=")){
        //<Relop> ::=   = |  /=  |   >   | <   |  =>   | <=
    }
    else{
        // Failed on <Relop> ::=   = |  /=  |   >   | <   |  =>   | <=
		ErrorMsg("= | /= | > | < | => | <=");
        compile = false;
    }

    return compile;

} // End of Relop()


// ----- Expression_Prime -----------------------------------------------------
// ----------------------------------------------------------------------------
bool Expression_Prime(void)
{

	// Production Rule
	parserFile << "<Expression Prime> -> +<Term> <Expression Prime> | "
				"-<Term> <Expression Prime> | epsilon\n";

	// Create compile flag
	bool compile = true;

    if(lexeme_is("+") | lexeme_is("-")){
        if(Term()){
            if(Expression_Prime()){
                //<Expression Prime> ::= +<Term> <Expression Prime> | -<Term>
                //<Expression Prime> | epsilon
            }
            else{
                // Failed on Expression_Prime()
                compile = false;
            }
        }
        else{
            // Failed on Term()
            compile = false;
        }
    }
    else{
        // Was empty and moved to epsilon
        // <Expression Prime> ::= epsilon
    }

    return compile;

} // End of Expression_Prime()


// ----- Term -----------------------------------------------------------------
// ----------------------------------------------------------------------------
bool Term(void)
{

	// Production Rule
	parserFile << "<Term> -> <Factor> <Term Prime>\n";

	// Create compile flag
	bool compile = true;

    if(Factor()){
        if(Term_Prime()){
            // <Term>  ::= <Factor> <Term Prime>
        }
        else{
            // Failed on Term_Prime()
            compile = false;
        }
    }
    else{
        // Failed on Factor()
        compile = false;
    }

    return compile;

} // End of Term()


// ----- Term_Prime -----------------------------------------------------------
// ----------------------------------------------------------------------------
bool Term_Prime(void)
{

	// Production Rule
	parserFile << "<Term Prime> -> * <Factor> <Term Prime> | "
				"/ Factor <Term Prime> | epsilon\n";

	// Create compile flag
	bool compile = true;

    if(lexeme_is("*") | lexeme_is("/")){
        if(Factor()){
            if(Term_Prime()){
                // <Term Prime> ::=  * <Factor> <Term Prime> | / Factor <Term
                // Prime> | epsilon
            }
            else{
                // Failed on Term_Prime()
                compile = false;
            }
        }
        else{
            // Failed on Factor()
            compile = false;
        }
    }
    else{
        // It moved to epsilon
        // <Term Prime> ::= epsilon
    }

    return compile;

} // End Term_Prime()


// ----- Factor ---------------------------------------------------------------
// ----------------------------------------------------------------------------
bool Factor(void)
{

	// Production Rule
	parserFile << "<Factor> -> - <Primary> | <Primary>\n";

	// Create compile flag
	bool compile = true;

    if(lexeme_is("-")){
        if(Primary()){
            // <Factor> ::= - <Primary>
        }
        else{
            // Failed on Primary()
            compile = false;
        }
    }
    else if(Primary()){
        // <Factor> ::= <Primary>
    }
    else{
        // Failed expected "-" or Primary
		ErrorMsg("- | <Primary>");
        compile = false;
    }

    return compile;

} // Factor()


// ----- Primary --------------------------------------------------------------
// ----------------------------------------------------------------------------
bool Primary(void)
{

	// Production Rule
	parserFile << "<Primary> -> <Identifier> | <Integer> | <Identifier> "
				"[<IDs>] | (<Expression>) | <Real> | true | false\n";

	// Create compile flag
	bool compile = true;

    if(Identifier()){
        if(lexeme_is("[")){
            if(IDs()){
                if(lexeme_is("]")){
                    // <Primary> ::= <Identifier> [<IDs>]
                }
                else{
                    // Failed on lexeme_is("]")
					ErrorMsg("]");
                    compile = false;
                }
            }
            else{
                // Failed on IDs()
                compile = false;
            }
        }
        else{
            // <Primary> ::= <Identifier>
        }
    }
    else if(token_is("integer")){
        // <Primary> ::= <Integer>
    }
    else if(lexeme_is("(")){
        if(Expression()){
            if(lexeme_is(")")){
                // <Primary> ::= ( <Expression> )
            }
            else{
                // Failed on lexeme_is(")")
				ErrorMsg(")");
                compile = false;
            }
        }
        else{
            // Failed on Expression()
            compile = false;
        }
    }
    else if(token_is("real")){
        // <Primary> ::= <Real>
    }
    else if(lexeme_is("true") | lexeme_is("false")){
        // <Primary> ::= true | false
    }
    else{
        // Failed expected . . . .
		ErrorMsg("<Identifier> | <Integer> | <Identifier> [<IDs>] | "
				"<Expression> | <Real> | true | false");
        compile = false;
    }

    return compile;

} // End of Primary()

#endif // End of parser.h
