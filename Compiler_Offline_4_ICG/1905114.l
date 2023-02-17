%option noyywrap

%{
#include<cstdio>
#include<cstdlib>
#include<string>
#include<vector>
#include<bits/stdc++.h>
#include "1905114.h"
#include "y.tab.h"


using namespace std;

extern YYSTYPE yylval;


//void yyerror(char *);
extern int line_count;

int error_count=0;

extern FILE *yyin;
ofstream writer2;



char characterConverter(char *chr){
        if(chr[1] != '\\'){
            return chr[1];
        } 
		if(chr[1] == '\\') {
			if(chr[2] == '\"')
                return (char)34;
            else if(chr[2] == 'n')
                return (char)10;
            else if(chr[2] == 't')
                return (char)9;
            else if(chr[2] == '\\')
                return (char)92;
            else if(chr[2] == '\'')
                return (char)39;
            else if(chr[2] == 'a')
                return (char)7;
            else if(chr[2] == 'f')
                return (char)12;
            else if(chr[2] == 'r')
                return (char)13;
            else if(chr[2] == 'b')
                return (char)8;
            else if(chr[2] == 'v')
                return (char)11;
            else if(chr[2] == '0')
                return (char)0;
                
        }
        return *chr;
    }

%}

WHITESPACE [ \t\f\r\v]+ 
DIGIT [0-9]
NEWLINE (\r)?\n
DIGITS {DIGIT}+

INTEGER {DIGITS}
FLOAT {DIGITS}?(\.{DIGITS})?([E][+-]?{DIGITS})?
CHARACTER '([ -`]{-}['\\])|(\\[nt\\\'\"afrbv0'])'

ID [a-zA-Z_][a-zA-Z0-9_]*

STRING_START (\")
SINGLE_LINE_COMMENT_START (\/\/)
MULTI_LINE_COMMENT_START (\/\*)


REDUNDANT_DECIMAL_ERROR {DIGITS}?(\.{DIGITS})(\.{DIGITS})+([E][+-]?{DIGITS})?
ILL_FORMED_NUMBER_ERROR ({DIGITS}\.)+|({DIGITS}*([Ee][+-]?{DIGITS})+(\.{DIGITS})+)
INVALID_SUFIX_PREFIX_ERROR ([a-zA-Z_]+[0-9]+)|([0-9]+[a-zA-Z_]+)
MULTIPLE_CHARACTER_ERROR '(([ -~]{-}['\\])([ -~]{-}['\\])+)'
UNFINISHED_CHAR_ERROR ('([ -~]{-}['\\])*)|('(\\[nt\\\'afrbv0'])*)
UNFINISHED_COMMENT_ERROR (\/\*)([^\*]|\*[^\/])*

%%

{NEWLINE} {line_count++;
			}

"if"	{
			writer2<<"Line# "<<line_count<<": Token <IF> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("IF",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return IF;
		}
		
"else"	{
			writer2<<"Line# "<<line_count<<": Token <ELSE> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("ELSE",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return ELSE;
		}

"for"	{
			writer2<<"Line# "<<line_count<<": Token <FOR> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("FOR",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return FOR;
		}		

"while"	{
			writer2<<"Line# "<<line_count<<": Token <WHILE> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("WHILE",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return WHILE;
		}
"do"	{
			writer2<<"Line# "<<line_count<<": Token <DO> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("DO",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return DO;
		}

"break"	{
			writer2<<"Line# "<<line_count<<": Token <BREAK> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("BREAK",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return BREAK;
		}

"int"	{
			writer2<<"Line# "<<line_count<<": Token <INT> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("INT",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return INT;
		}
"char"	{
			writer2<<"Line# "<<line_count<<": Token <CHAR> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("CHAR",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return CHAR;
		}

"float"	{
			writer2<<"Line# "<<line_count<<": Token <FLOAT> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("FLOAT",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return FLOAT;
		}

"double"	{
			writer2<<"Line# "<<line_count<<": Token <DOUBLE> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("DOUBLE",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return DOUBLE;
		}

"void"	{
			writer2<<"Line# "<<line_count<<": Token <VOID> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("VOID",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return VOID;
		}

"switch"	{
			writer2<<"Line# "<<line_count<<": Token <SWITCH> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("SWITCH",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return SWITCH;
		}

"case"	{
			writer2<<"Line# "<<line_count<<": Token <CASE> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("CASE",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return CASE;
		}

"default"	{
			writer2<<"Line# "<<line_count<<": Token <DEFAULT> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("DEFAULT",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return DEFAULT;
		}	

"continue"	{
			writer2<<"Line# "<<line_count<<": Token <CONTINUE> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("CONTINUE",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return CONTINUE;
		}

"printf"	{
			writer2<<"Line# "<<line_count<<": Token <PRINTLN> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("PRINTLN",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return PRINTLN;
		}

"return"	{
			writer2<<"Line# "<<line_count<<": Token <RETURN> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("RETURN",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return RETURN;
		}

"++"	{
			writer2<<"Line# "<<line_count<<": Token <INCOP> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("INCOP",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return INCOP;
        }

"--"	{
			writer2<<"Line# "<<line_count<<": Token <DECOP> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("DECOP",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return DECOP;
        }											

		
{INTEGER} 	{
			writer2<<"Line# "<<line_count<<": Token <CONST_INT> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("CONST_INT",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return CONST_INT;
			}

{FLOAT} 	{
			writer2<<"Line# "<<line_count<<": Token <CONST_FLOAT> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("CONST_FLOAT",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return CONST_FLOAT;
			}	

{CHARACTER} 	{
			writer2<<"Line# "<<line_count<<": Token <CONST_CHAR> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("CONST_CHAR",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return CONST_CHAR;
			}		


"+"|"-"	{
			writer2<<"Line# "<<line_count<<": Token <ADDOP> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("ADDOP",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return ADDOP;
        }


"*"|"/"|"%"	{
			writer2<<"Line# "<<line_count<<": Token <MULOP> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("MULOP",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return MULOP;
        }

"<"|"<="|">"|">="|"=="|"!=" {
			writer2<<"Line# "<<line_count<<": Token <RELOP> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("RELOP",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return RELOP;
        }


"="	{
			writer2<<"Line# "<<line_count<<": Token <ASSIGNOP> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("ASSIGNOP",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return ASSIGNOP;
        }


"&&"|"||"	{
			writer2<<"Line# "<<line_count<<": Token <LOGICOP> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("LOGICOP",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return LOGICOP;
        }


"&"|"|"|"^"|"<<"|">>"	{
			writer2<<"Line# "<<line_count<<": Token <BITOP> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("BITOP",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return BITOP;
        }


"!"	{
			writer2<<"Line# "<<line_count<<": Token <NOT> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("NOT",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return NOT;
        }


"("	{
			writer2<<"Line# "<<line_count<<": Token <LPAREN> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("LPAREN",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return LPAREN;
        }

")"	{
			writer2<<"Line# "<<line_count<<": Token <RPAREN> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("RPAREN",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return RPAREN;
        }																


"{" {
			writer2<<"Line# "<<line_count<<": Token <LCURL> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("LCURL",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return LCURL;
        }

"}"	{
			writer2<<"Line# "<<line_count<<": Token <RCURL> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("RCURL",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return RCURL;
        }

"["	{
			writer2<<"Line# "<<line_count<<": Token <LTHIRD> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("LTHIRD",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return LTHIRD;
        }		

"]"	{
			writer2<<"Line# "<<line_count<<": Token <RTHIRD> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("RTHIRD",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return RTHIRD;
        }			

","	{
			writer2<<"Line# "<<line_count<<": Token <COMMA> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("COMMA",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return COMMA;
        }

";"	{
			writer2<<"Line# "<<line_count<<": Token <SEMICOLON> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("SEMICOLON",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return SEMICOLON;
        }

{ID}   {
			writer2<<"Line# "<<line_count<<": Token <ID> Lexeme "<<yytext<<" found"<<endl;
			yylval.symbol_info=new SymbolInfo("ID",yytext);
			yylval.symbol_info->sl=line_count;
			yylval.symbol_info->el=line_count;
			yylval.symbol_info->isLeaf=true;
			return ID;
        }

{REDUNDANT_DECIMAL_ERROR}	{
			error_count++;
		}

{ILL_FORMED_NUMBER_ERROR}	{
			error_count++;
		}

{INVALID_SUFIX_PREFIX_ERROR}	{
			error_count++;
		}

{MULTIPLE_CHARACTER_ERROR}	{
			error_count++;
		}

{UNFINISHED_CHAR_ERROR}	{
			error_count++;
		}

{UNFINISHED_COMMENT_ERROR}	{ 
			error_count++;
		}						

%%


