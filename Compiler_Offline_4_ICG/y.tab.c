/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "1905114.y"


#include<fstream>
#include<iostream>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include "1905114.h"
//#define YYSTYPE SymbolInfo*

using namespace std;

int yyparse(void);
int yylex(void);
void yyerror(char *s)
{
	//write your code
}


extern FILE *yyin;
int line_count=1;

ofstream writer;
extern ofstream writer2;
ofstream writer3;
ofstream asmWriter;

SymbolTable *symbolTable=new SymbolTable(10);
string global_array="";
string local_array="";
SymbolInfo *temp1;
int stack_offset=0;
int stack_back_offset=0;
int label_count=0;
int curly_brace_count=0;
string current_function="";
int function_param_count=0;
vector<string> op_stack;
bool code=false;


bool is_inside_expression_statement=false;
bool is_inside_assignment_var=false;
bool is_inside_expression=false;
string function_name="";
bool var_assginment=false;
string temp_assign="";
vector<string> param_stack;


//----------Printer---------------------------------------------------------------------------------------
void asm_program(){
	asmWriter<<";-------\n;\n;-------"<<endl;
	asmWriter<<".MODEL SMALL"<<endl;
	asmWriter<<".STACK 1000H"<<endl;
	asmWriter<<".Data"<<endl;
	asmWriter<<"	CR EQU 0DH"<<endl;
	asmWriter<<"	LF EQU 0AH"<<endl;
	asmWriter<<"	number DB \"00000$\""<<endl;
}
void asm_unit_var_single(SymbolInfo* head){
	asmWriter<<"	"+head->getName()+" DW 1 DUP (0000H)"<<endl;
}
void asm_unit_var_array1(SymbolInfo* head){
	asmWriter<<"	"+head->getName()+" DW ";
}
void asm_unit_var_array2(SymbolInfo* head){
	asmWriter<<head->getName()+" DUP (0000H)"<<endl;;
}
void asm_state_var_single(){
	asmWriter<<"	SUB SP, 2"<<endl;
}
			//function____________
void asm_function_ID(SymbolInfo* head){
	asmWriter<<head->getName()<<" "<<"PROC"<<endl;
	asmWriter<<"	PUSH BP"<<endl;
	asmWriter<<"	MOV BP, SP"<<endl;
}
void asm_function_ID_main(SymbolInfo* head){
	asmWriter<<head->getName()<<" "<<"PROC"<<endl;
	asmWriter<<"	MOV AX, @DATA"<<endl;
	asmWriter<<"	MOV DS, AX"<<endl;
	asmWriter<<"	PUSH BP"<<endl;
	asmWriter<<"	MOV BP, SP"<<endl;
}

void asm_function_end(){
	if(current_function=="main")
		{
			if(stack_offset!=0)
				asmWriter<<"	ADD SP, "<<stack_offset<<endl;
			asmWriter<<"	POP BP"<<endl;
			asmWriter<<"	MOV AX,4CH"<<endl;
			asmWriter<<"	INT 21H"<<endl;
			asmWriter<<"main ENDP"<<endl;
		}
	else{
		if(stack_offset!=0)
			asmWriter<<"	ADD SP,"<<stack_offset-2<<endl;
		asmWriter<<"	POP BP"<<endl;
		if(function_param_count==0)
			asmWriter<<"	RET"<<endl;
		else
			asmWriter<<"	RET "<<function_param_count<<endl;
		function_param_count=0;
		asmWriter<<current_function<<" ENDP"<<endl;
	}
}
void asm_expression(){
	while(op_stack.size()!=0){
		string temp=op_stack.back();
		op_stack.pop_back();
		if(temp=="+")
			{
				asmWriter<<"	POP BX"<<endl;
				asmWriter<<"	POP AX"<<endl;
				asmWriter<<"	ADD AX, BX"<<endl;
				asmWriter<<"	PUSH AX"<<endl;
			}
			//
		if(temp=="-")
			{
				asmWriter<<"	POP BX"<<endl;
				asmWriter<<"	POP AX"<<endl;
				asmWriter<<"	SUB AX, BX"<<endl;
				asmWriter<<"	PUSH AX"<<endl;
			}
			//
		if(temp=="*")
			{
				asmWriter<<"	POP CX"<<endl;
				asmWriter<<"	POP AX"<<endl;
				asmWriter<<"	CWD"<<endl;
				asmWriter<<"	MUL CX"<<endl;
				asmWriter<<"	MOV AX, CX"<<endl;
				asmWriter<<"	PUSH AX"<<endl;
			}
			//
		if(temp=="=")
			{	
				asmWriter<<"	POP AX"<<endl;
				asmWriter<<"	MOV "<<temp_assign<<", AX"<<endl;
			}
			//
	}
}




//---------------------------------------------------------------------------------------------------------

//---------Helper------------------------------------------------------------------------------------------
string name_type(SymbolInfo* head){
	return head->getType()+" : "+head->getName();
}
bool check_global_var(string check){
	return (check=="unit_var" || check=="unit_var_single" || check=="unit_var_array");
}
bool check_local_var(string check){
	return (check=="state_var" || check=="state_var_single" || check=="state_var_array");
}
	//expression_helper
//---------------------------------------------------------------------------------------------------------

//---------State_machine-----------------------------------------------------------------------------------
string Check_state_machine(SymbolInfo* head,string check){
		//global_and_local_variable_and_array---------------------
		if(name_type(head)=="unit : var_declaration"){
			check= "unit_var";
		}	
		if(name_type(head)=="statement : var_declaration"){
			check= "state_var";
		}	
		if(name_type(head)=="declaration_list : declaration_list COMMA ID" && check_global_var(check)){
			check= "unit_var_single";
		}	
		if(name_type(head)=="declaration_list : ID" && check_global_var(check)){
			check= "unit_var_single";
		}
		if(name_type(head)=="declaration_list : declaration_list COMMA ID LTHIRD CONST_INT RTHIRD" && check_global_var(check)){
			check= "unit_var_array";
		}	
		if(name_type(head)=="declaration_list : ID LTHIRD CONST_INT RTHIRD" && check_global_var(check)){
			check= "unit_var_array";
		}	
		if(name_type(head)=="declaration_list : declaration_list COMMA ID" && check_local_var(check) ){
			check= "state_var_single";
		}
		if(name_type(head)=="declaration_list : ID" && check_local_var(check) ){
			check= "state_var_single";
		}
		if(name_type(head)=="declaration_list : declaration_list COMMA ID LTHIRD CONST_INT RTHIRD" && check_local_var(check) ){
			check= "state_var_array";
		}
		if(name_type(head)=="declaration_list : ID LTHIRD CONST_INT RTHIRD" && check_local_var(check)){
			check= "state_var_array";
		}
		//function---------------------------------------------
		if(name_type(head)=="unit : func_definition")
			check="function_start";
		if(name_type(head)=="parameter_list : parameter_list COMMA type_specifier ID"&& check=="function_start")
			check="inside_param";
		if(name_type(head)=="parameter_list : type_specifier ID"&& check=="function_start")
			check="inside_param";

		if(name_type(head)=="compound_statement : LCURL statements RCURL"&& check=="function_start")
			check="inside_function";

		if(name_type(head)=="LCURL : {"&& check=="inside_function")
		{
			check="param_calculation";
		}

		if(name_type(head)=="statement : RETURN expression SEMICOLON" && check=="inside_function")
			check="return";
			
		
		
		//expression-------------------------------------------
			

		if(name_type(head)=="expression_statement : expression SEMICOLON")
			check="expression_statement";
		if(name_type(head)=="expression : variable ASSIGNOP logic_expression")
		{
			check="var_assign";		
		}
		if(name_type(head)=="variable : ID" and check=="var_assign")
		{
			check="var_assign_id";			
		}
		if(name_type(head)=="logic_expression : rel_expression" and check=="var_assign")
		{
			check="inside_expression";		
		}
		if(name_type(head)=="factor : ID LPAREN argument_list RPAREN" and check=="inside_expression")
		{
			check="function_call";			
		}
		if(name_type(head)=="argument_list : arguments" and check=="function_call")
		{
			check="params";			
		}
	
		return check;
}
//----------------------------------------------------------------------------------------------------------

//--------writter_parent------------------------------------------------------------------------------------
void asmWriter_parent(SymbolInfo* head, string check){


		if(name_type(head)=="unit : func_definition" && code==false)
		{	
			asmWriter<<".CODE"<<endl;
			
			code=true;
		}


		
		if(name_type(head)=="start : program")
			asm_program();
		if(head->getType()=="ID" && check=="unit_var_array")
		{
			asm_unit_var_array1(head);
			global_array=head->getName();
			symbolTable->Insert(head->getName(),"global_array");			
		}	
		if(head->getType()=="CONST_INT" && check=="unit_var_array")
		{
			asm_unit_var_array2(head);
			temp1=symbolTable->Look_Up(global_array);
			temp1->array_size=stoi(head->getName());
		}		
		if(head->getType()=="ID" && check=="unit_var_single")
		{
			asm_unit_var_single(head);
			symbolTable->Insert(head->getName(),"global_var");
		}
		if(head->getType()=="ID" && check=="state_var_array")
		{
			local_array=head->getName();			
		}	
		if(head->getType()=="CONST_INT" && check=="state_var_array")
		{
			stack_offset=stack_offset+2;
			for(int i=0;i<stoi(head->getName());i++){
				symbolTable->Insert(local_array+"["+to_string(i)+"]","local_array");
				temp1=symbolTable->Look_Up(local_array+"["+to_string(i)+"]");
				temp1->stack_offset=stack_offset;
				stack_offset=stack_offset+2;
				asm_state_var_single();
			}
		}
		if(head->getType()=="ID" && check=="state_var_single")
		{
			asm_state_var_single();
			symbolTable->Insert(head->getName(),"local_var");
			stack_offset=stack_offset+2;
			temp1=symbolTable->Look_Up(head->getName());
			temp1->stack_offset=stack_offset;
		}	

		//function----------------------------------------
		if(head->getType()=="ID" && check=="function_start")
		{
			if(head->getName()=="main")
				{
					asm_function_ID_main(head);
					current_function="main";
				}	
			else
			{
				asm_function_ID(head);
				current_function=head->getName();
			}	
			symbolTable->Enter_Scope();
			symbolTable->Insert(head->getName(),"function");
			temp1=symbolTable->Look_Up(head->getName());
			
			stack_offset=0;
			stack_back_offset=0;
		}
		if(head->getType()=="ID"&& check=="inside_param")
			{
				function_param_count++;
				param_stack.push_back(head->getName());
			}
		if(head->getType()=="LCURL")
			curly_brace_count++;
		if(head->getType()=="RCURL")
			{
				curly_brace_count--;
				if(curly_brace_count==0){
					asm_function_end();
					symbolTable->Exit();
				}
					

			}
		if(check=="param_calculation")
			{	
				
				stack_back_offset=4;
				while(param_stack.size()!=0){
				string param1=param_stack.back();
				param_stack.pop_back();
				symbolTable->Insert(param1,"local_var");
				temp1=symbolTable->Look_Up(param1);
				temp1->stack_offset=-stack_back_offset;
				stack_back_offset=stack_back_offset+2;
				}
			}
		if(head->getType()=="ID" && check=="return")
			{
				temp1=symbolTable->Look_Up(head->getName());
				if(temp1->getType()=="local_var"){
					if(temp1->stack_offset>0)
						asmWriter<<"	MOV AX, [BP-"<<to_string(temp1->stack_offset)<<"]"<<endl;
					else
						asmWriter<<"	MOV AX, [BP+"<<to_string(-temp1->stack_offset)<<"]"<<endl;
				}		
				if(temp1->getType()=="global_var"){
					asmWriter<<"	MOV AX, "<<head->getName()<<endl;
				}
			}

		//expression-----------------------------

		if(head->getType()=="ID" && check=="var_assign_id")
			{
				
				temp1=symbolTable->Look_Up(head->getName());
				if(temp1->getType()=="local_var"){
					if(temp1->stack_offset>0)
					temp_assign="[BP-"+to_string(temp1->stack_offset)+"]";
					else
					temp_assign="[BP+"+to_string(-temp1->stack_offset)+"]";
				}		
				if(temp1->getType()=="global_var"){
					temp_assign=head->getName();
				}

				op_stack.push_back("=");
			}
		if(head->getType()=="ID" && check=="inside_expression")
			{
				
				temp1=symbolTable->Look_Up(head->getName());
				if(temp1->getType()=="local_var"){
					if(temp1->stack_offset>0)
					asmWriter<<"	MOV AX, [BP-"<<to_string(temp1->stack_offset)<<"]"<<endl;
					else
					asmWriter<<"	MOV AX, [BP+"<<to_string(-temp1->stack_offset)<<"]"<<endl;

					asmWriter<<"	PUSH AX"<<endl;
				}		
				if(temp1->getType()=="global_var"){
					asmWriter<<"	MOV AX, "<<head->getName()<<endl;
					asmWriter<<"	PUSH AX";
				}
			}
		if(head->getType()=="CONST_INT" && check=="inside_expression")
			{
					asmWriter<<"	MOV AX, "<<head->getName()<<endl;
					asmWriter<<"	PUSH AX"<<endl;
			}
		if(head->getName()=="+" && check=="inside_expression")
			{
				op_stack.push_back("+");
			}
		if(head->getName()=="-" && check=="inside_expression")
			{
				op_stack.push_back("-");
			}
		if(head->getName()=="*" && check=="inside_expression")
			{
				op_stack.push_back("*");
			}
		if(head->getType()=="ID" && check=="function_call")
			{
				function_name=head->getName();
			}
		if(head->getType()=="RPAREN" && check=="function_call")
			{
				check="inside_expression";
				asmWriter<<"	CALL "<<function_name<<endl;
				asmWriter<<"	PUSH AX"<<endl;
			}
		if(head->getType()=="ID" && check=="params")
			{
				temp1=symbolTable->Look_Up(head->getName());
				if(temp1->getType()=="local_var"){
					if(temp1->stack_offset>0)
					asmWriter<<"	MOV AX, [BP-"<<to_string(temp1->stack_offset)<<"]"<<endl;
					else
					asmWriter<<"	MOV AX, [BP+"<<to_string(-temp1->stack_offset)<<"]"<<endl;
					asmWriter<<"	PUSH AX"<<endl;
				}		
				if(temp1->getType()=="global_var"){
					asmWriter<<"	MOV AX, "<<head->getName()<<endl;
					asmWriter<<"	PUSH AX";
				}
				
			}
		if(head->getType()=="CONST_INT"&& check=="params")
			{
				asmWriter<<"	MOV AX, "<<head->getName()<<endl;
				asmWriter<<"	PUSH AX"<<endl;
			}
		if(head->getType()=="SEMICOLON" && check=="expression_statement")
			{
				asm_expression();
				check="";
			}
}
//----------------------------------------------------------------------------------------------------------

//---------parse_tree---------------------------------------------------------------------------------------
void printTree(SymbolInfo* head, int depth, string check){
		
		check=Check_state_machine(head,check);

		asmWriter_parent(head,check);

		for(int j=0;j<depth;j++)
			writer<<" ";

		writer<<head->getType()<<" : "<<head->getName()<<" 	";
		if(head->isLeaf==true)
			writer<<"<Line: "<<head->sl<<">"<<endl;
		else
			writer<<"<Line: "<<head->sl<<"-"<<head->el<<">"<<endl;
	
		for (auto i : head->children) {
      	 printTree(i,depth+1,check);
   		 }
}
//------------------------------------------------------------------------------------------------------------
void deleteTree(SymbolInfo* head){
		if(head->children.size()==0)
		{
			delete head;
			return;
		}
		else
			for (auto i : head->children) {
      	 		deleteTree(i);
   		 	}
}



#line 568 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IF = 258,                      /* IF  */
    ELSE = 259,                    /* ELSE  */
    LOWER_THAN_ELSE = 260,         /* LOWER_THAN_ELSE  */
    FOR = 261,                     /* FOR  */
    WHILE = 262,                   /* WHILE  */
    DO = 263,                      /* DO  */
    BREAK = 264,                   /* BREAK  */
    INT = 265,                     /* INT  */
    CHAR = 266,                    /* CHAR  */
    FLOAT = 267,                   /* FLOAT  */
    DOUBLE = 268,                  /* DOUBLE  */
    VOID = 269,                    /* VOID  */
    SWITCH = 270,                  /* SWITCH  */
    CASE = 271,                    /* CASE  */
    DEFAULT = 272,                 /* DEFAULT  */
    CONTINUE = 273,                /* CONTINUE  */
    ASSIGNOP = 274,                /* ASSIGNOP  */
    NOT = 275,                     /* NOT  */
    LPAREN = 276,                  /* LPAREN  */
    RPAREN = 277,                  /* RPAREN  */
    LCURL = 278,                   /* LCURL  */
    RCURL = 279,                   /* RCURL  */
    LTHIRD = 280,                  /* LTHIRD  */
    RTHIRD = 281,                  /* RTHIRD  */
    COMMA = 282,                   /* COMMA  */
    SEMICOLON = 283,               /* SEMICOLON  */
    PRINTLN = 284,                 /* PRINTLN  */
    RETURN = 285,                  /* RETURN  */
    INCOP = 286,                   /* INCOP  */
    DECOP = 287,                   /* DECOP  */
    CONST_INT = 288,               /* CONST_INT  */
    CONST_CHAR = 289,              /* CONST_CHAR  */
    CONST_FLOAT = 290,             /* CONST_FLOAT  */
    ADDOP = 291,                   /* ADDOP  */
    MULOP = 292,                   /* MULOP  */
    RELOP = 293,                   /* RELOP  */
    LOGICOP = 294,                 /* LOGICOP  */
    BITOP = 295,                   /* BITOP  */
    ID = 296                       /* ID  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define IF 258
#define ELSE 259
#define LOWER_THAN_ELSE 260
#define FOR 261
#define WHILE 262
#define DO 263
#define BREAK 264
#define INT 265
#define CHAR 266
#define FLOAT 267
#define DOUBLE 268
#define VOID 269
#define SWITCH 270
#define CASE 271
#define DEFAULT 272
#define CONTINUE 273
#define ASSIGNOP 274
#define NOT 275
#define LPAREN 276
#define RPAREN 277
#define LCURL 278
#define RCURL 279
#define LTHIRD 280
#define RTHIRD 281
#define COMMA 282
#define SEMICOLON 283
#define PRINTLN 284
#define RETURN 285
#define INCOP 286
#define DECOP 287
#define CONST_INT 288
#define CONST_CHAR 289
#define CONST_FLOAT 290
#define ADDOP 291
#define MULOP 292
#define RELOP 293
#define LOGICOP 294
#define BITOP 295
#define ID 296

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 498 "1905114.y"

	SymbolInfo* symbol_info;	

#line 707 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IF = 3,                         /* IF  */
  YYSYMBOL_ELSE = 4,                       /* ELSE  */
  YYSYMBOL_LOWER_THAN_ELSE = 5,            /* LOWER_THAN_ELSE  */
  YYSYMBOL_FOR = 6,                        /* FOR  */
  YYSYMBOL_WHILE = 7,                      /* WHILE  */
  YYSYMBOL_DO = 8,                         /* DO  */
  YYSYMBOL_BREAK = 9,                      /* BREAK  */
  YYSYMBOL_INT = 10,                       /* INT  */
  YYSYMBOL_CHAR = 11,                      /* CHAR  */
  YYSYMBOL_FLOAT = 12,                     /* FLOAT  */
  YYSYMBOL_DOUBLE = 13,                    /* DOUBLE  */
  YYSYMBOL_VOID = 14,                      /* VOID  */
  YYSYMBOL_SWITCH = 15,                    /* SWITCH  */
  YYSYMBOL_CASE = 16,                      /* CASE  */
  YYSYMBOL_DEFAULT = 17,                   /* DEFAULT  */
  YYSYMBOL_CONTINUE = 18,                  /* CONTINUE  */
  YYSYMBOL_ASSIGNOP = 19,                  /* ASSIGNOP  */
  YYSYMBOL_NOT = 20,                       /* NOT  */
  YYSYMBOL_LPAREN = 21,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 22,                    /* RPAREN  */
  YYSYMBOL_LCURL = 23,                     /* LCURL  */
  YYSYMBOL_RCURL = 24,                     /* RCURL  */
  YYSYMBOL_LTHIRD = 25,                    /* LTHIRD  */
  YYSYMBOL_RTHIRD = 26,                    /* RTHIRD  */
  YYSYMBOL_COMMA = 27,                     /* COMMA  */
  YYSYMBOL_SEMICOLON = 28,                 /* SEMICOLON  */
  YYSYMBOL_PRINTLN = 29,                   /* PRINTLN  */
  YYSYMBOL_RETURN = 30,                    /* RETURN  */
  YYSYMBOL_INCOP = 31,                     /* INCOP  */
  YYSYMBOL_DECOP = 32,                     /* DECOP  */
  YYSYMBOL_CONST_INT = 33,                 /* CONST_INT  */
  YYSYMBOL_CONST_CHAR = 34,                /* CONST_CHAR  */
  YYSYMBOL_CONST_FLOAT = 35,               /* CONST_FLOAT  */
  YYSYMBOL_ADDOP = 36,                     /* ADDOP  */
  YYSYMBOL_MULOP = 37,                     /* MULOP  */
  YYSYMBOL_RELOP = 38,                     /* RELOP  */
  YYSYMBOL_LOGICOP = 39,                   /* LOGICOP  */
  YYSYMBOL_BITOP = 40,                     /* BITOP  */
  YYSYMBOL_ID = 41,                        /* ID  */
  YYSYMBOL_YYACCEPT = 42,                  /* $accept  */
  YYSYMBOL_start = 43,                     /* start  */
  YYSYMBOL_program = 44,                   /* program  */
  YYSYMBOL_unit = 45,                      /* unit  */
  YYSYMBOL_func_declaration = 46,          /* func_declaration  */
  YYSYMBOL_func_definition = 47,           /* func_definition  */
  YYSYMBOL_parameter_list = 48,            /* parameter_list  */
  YYSYMBOL_compound_statement = 49,        /* compound_statement  */
  YYSYMBOL_var_declaration = 50,           /* var_declaration  */
  YYSYMBOL_type_specifier = 51,            /* type_specifier  */
  YYSYMBOL_declaration_list = 52,          /* declaration_list  */
  YYSYMBOL_statements = 53,                /* statements  */
  YYSYMBOL_statement = 54,                 /* statement  */
  YYSYMBOL_expression_statement = 55,      /* expression_statement  */
  YYSYMBOL_variable = 56,                  /* variable  */
  YYSYMBOL_expression = 57,                /* expression  */
  YYSYMBOL_logic_expression = 58,          /* logic_expression  */
  YYSYMBOL_rel_expression = 59,            /* rel_expression  */
  YYSYMBOL_simple_expression = 60,         /* simple_expression  */
  YYSYMBOL_term = 61,                      /* term  */
  YYSYMBOL_unary_expression = 62,          /* unary_expression  */
  YYSYMBOL_factor = 63,                    /* factor  */
  YYSYMBOL_argument_list = 64,             /* argument_list  */
  YYSYMBOL_arguments = 65                  /* arguments  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   184

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  42
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  64
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  118

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   296


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   511,   511,   529,   538,   548,   556,   564,   574,   587,
     601,   614,   629,   640,   650,   659,   670,   680,   691,   703,
     711,   719,   729,   740,   753,   762,   775,   783,   794,   802,
     810,   818,   832,   845,   860,   872,   884,   896,   904,   915,
     923,   936,   944,   956,   964,   976,   984,   996,  1004,  1016,
    1024,  1037,  1047,  1057,  1068,  1077,  1089,  1100,  1109,  1118,
    1128,  1140,  1149,  1158,  1169
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IF", "ELSE",
  "LOWER_THAN_ELSE", "FOR", "WHILE", "DO", "BREAK", "INT", "CHAR", "FLOAT",
  "DOUBLE", "VOID", "SWITCH", "CASE", "DEFAULT", "CONTINUE", "ASSIGNOP",
  "NOT", "LPAREN", "RPAREN", "LCURL", "RCURL", "LTHIRD", "RTHIRD", "COMMA",
  "SEMICOLON", "PRINTLN", "RETURN", "INCOP", "DECOP", "CONST_INT",
  "CONST_CHAR", "CONST_FLOAT", "ADDOP", "MULOP", "RELOP", "LOGICOP",
  "BITOP", "ID", "$accept", "start", "program", "unit", "func_declaration",
  "func_definition", "parameter_list", "compound_statement",
  "var_declaration", "type_specifier", "declaration_list", "statements",
  "statement", "expression_statement", "variable", "expression",
  "logic_expression", "rel_expression", "simple_expression", "term",
  "unary_expression", "factor", "argument_list", "arguments", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-68)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      51,   -68,   -68,   -68,    14,    51,   -68,   -68,   -68,   -68,
      -7,   -68,   -68,    -4,    26,     6,    27,     0,   -68,    -1,
       8,    30,    49,    55,    67,   -68,   -68,    10,    51,   -68,
     -68,    50,    61,    63,    64,   143,   143,   -68,   -68,    65,
     143,   -68,   -68,   143,    41,   -68,   -68,    48,   103,   -68,
     -68,    -6,    66,   -68,    53,    40,    56,   -68,   -68,   -68,
     -68,    57,    73,   143,    23,   143,    -8,   -68,    79,    70,
      76,   -68,   143,   143,    80,   -68,   -68,   143,   -68,   -68,
     -68,   143,   143,   143,   143,   -68,   -68,    85,    23,    90,
     -68,    92,   -68,   -68,    94,    91,    93,   -68,   -68,    56,
      84,   -68,   139,   143,   139,    97,   -68,   143,   -68,   117,
     100,   -68,   -68,   -68,   139,   139,   -68,   -68
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    19,    20,    21,     0,     2,     4,     6,     7,     5,
       0,     1,     3,    24,     0,     0,     0,     0,    18,     0,
       0,    15,     0,    22,     0,     9,    11,     0,     0,    14,
      25,     0,     0,     0,     0,     0,     0,    17,    37,     0,
       0,    57,    58,     0,    39,    30,    28,     0,     0,    26,
      29,    54,     0,    41,    43,    45,    47,    49,    53,     8,
      10,    13,     0,     0,     0,     0,    54,    52,     0,     0,
       0,    51,    62,     0,    24,    16,    27,     0,    59,    60,
      38,     0,     0,     0,     0,    12,    23,     0,     0,     0,
      56,     0,    36,    64,     0,    61,     0,    42,    44,    48,
      46,    50,     0,     0,     0,     0,    55,     0,    40,    32,
       0,    34,    35,    63,     0,     0,    33,    31
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -68,   -68,   -68,   123,   -68,   -68,   -68,   -12,    45,     4,
     -68,   -68,   -47,   -52,   -35,   -34,   -67,    54,    46,    52,
     -32,   -68,   -68,   -68
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     4,     5,     6,     7,     8,    20,    45,    46,    47,
      14,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    94,    95
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      66,    76,    68,    67,    10,    93,    70,    26,    66,    10,
      97,    71,    88,    77,    11,    60,     1,    15,     2,    21,
       3,    16,    24,    78,    79,    78,    79,    25,    19,    87,
      27,    89,    61,    24,    13,    28,   103,    66,    59,    96,
     113,    23,    66,    35,    36,     9,    66,    66,    66,    66,
       9,    38,   101,    17,    18,   109,    41,   111,    42,    43,
      22,     1,    72,     2,    44,     3,    73,   116,   117,   110,
      32,    29,    66,    33,    34,    30,    82,     1,    83,     2,
      31,     3,    63,    62,    64,    65,    69,    35,    36,    74,
      24,    37,    81,    84,    80,    38,    39,    40,    85,    86,
      41,    90,    42,    43,    92,    16,    32,   102,    44,    33,
      34,    91,   104,     1,   105,     2,   106,     3,   107,   108,
      82,   114,   115,    35,    36,   112,    24,    75,    12,   100,
       0,    38,    39,    40,    99,    98,    41,     0,    42,    43,
       0,     0,    32,     0,    44,    33,    34,     0,     0,     1,
       0,     2,     0,     3,     0,     0,     0,     0,     0,    35,
      36,     0,    24,    35,    36,     0,     0,    38,    39,    40,
       0,     0,    41,     0,    42,    43,    41,     0,    42,    43,
      44,     0,     0,     0,    44
};

static const yytype_int8 yycheck[] =
{
      35,    48,    36,    35,     0,    72,    40,    19,    43,     5,
      77,    43,    64,    19,     0,    27,    10,    21,    12,    15,
      14,    25,    23,    31,    32,    31,    32,    28,    22,    63,
      22,    65,    28,    23,    41,    27,    88,    72,    28,    73,
     107,    41,    77,    20,    21,     0,    81,    82,    83,    84,
       5,    28,    84,    27,    28,   102,    33,   104,    35,    36,
      33,    10,    21,    12,    41,    14,    25,   114,   115,   103,
       3,    41,   107,     6,     7,    26,    36,    10,    38,    12,
      25,    14,    21,    33,    21,    21,    21,    20,    21,    41,
      23,    24,    39,    37,    28,    28,    29,    30,    41,    26,
      33,    22,    35,    36,    28,    25,     3,    22,    41,     6,
       7,    41,    22,    10,    22,    12,    22,    14,    27,    26,
      36,     4,    22,    20,    21,    28,    23,    24,     5,    83,
      -1,    28,    29,    30,    82,    81,    33,    -1,    35,    36,
      -1,    -1,     3,    -1,    41,     6,     7,    -1,    -1,    10,
      -1,    12,    -1,    14,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    23,    20,    21,    -1,    -1,    28,    29,    30,
      -1,    -1,    33,    -1,    35,    36,    33,    -1,    35,    36,
      41,    -1,    -1,    -1,    41
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    10,    12,    14,    43,    44,    45,    46,    47,    50,
      51,     0,    45,    41,    52,    21,    25,    27,    28,    22,
      48,    51,    33,    41,    23,    28,    49,    22,    27,    41,
      26,    25,     3,     6,     7,    20,    21,    24,    28,    29,
      30,    33,    35,    36,    41,    49,    50,    51,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    28,
      49,    51,    33,    21,    21,    21,    56,    62,    57,    21,
      57,    62,    21,    25,    41,    24,    54,    19,    31,    32,
      28,    39,    36,    38,    37,    41,    26,    57,    55,    57,
      22,    41,    28,    58,    64,    65,    57,    58,    59,    61,
      60,    62,    22,    55,    22,    22,    22,    27,    26,    54,
      57,    54,    28,    58,     4,    22,    54,    54
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    42,    43,    44,    44,    45,    45,    45,    46,    46,
      47,    47,    48,    48,    48,    48,    49,    49,    50,    51,
      51,    51,    52,    52,    52,    52,    53,    53,    54,    54,
      54,    54,    54,    54,    54,    54,    54,    55,    55,    56,
      56,    57,    57,    58,    58,    59,    59,    60,    60,    61,
      61,    62,    62,    62,    63,    63,    63,    63,    63,    63,
      63,    64,    64,    65,    65
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     6,     5,
       6,     5,     4,     3,     2,     1,     3,     2,     3,     1,
       1,     1,     3,     6,     1,     4,     1,     2,     1,     1,
       1,     7,     5,     7,     5,     5,     3,     1,     2,     1,
       4,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     2,     2,     1,     1,     4,     3,     1,     1,     2,
       2,     1,     0,     3,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* start: program  */
#line 512 "1905114.y"
        {
		(yyval.symbol_info)=new SymbolInfo("start","program");
		(yyval.symbol_info)->sl=(yyvsp[0].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		
		printTree((yyval.symbol_info),0,"start");
		asmWriter<<"END main"<<endl;

		symbolTable->Print_All_Scope_Table();
		deleteTree((yyval.symbol_info));
		//write your code in this block in all the similar blocks below
		
	}
#line 1856 "y.tab.c"
    break;

  case 3: /* program: program unit  */
#line 529 "1905114.y"
                       {
		writer2<<"program : program unit"<<endl;
		(yyval.symbol_info)=new SymbolInfo("program","program unit");
		(yyval.symbol_info)->sl=(yyvsp[-1].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 1870 "y.tab.c"
    break;

  case 4: /* program: unit  */
#line 538 "1905114.y"
               {
		writer2<<"program : unit"<<endl;
		(yyval.symbol_info)=new SymbolInfo("program","unit");
		(yyval.symbol_info)->sl=(yyvsp[0].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 1883 "y.tab.c"
    break;

  case 5: /* unit: var_declaration  */
#line 548 "1905114.y"
                       {
		writer2<<"unit : var_declaration"<<endl;
		(yyval.symbol_info)=new SymbolInfo("unit","var_declaration");
		(yyval.symbol_info)->sl=(yyvsp[0].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 1896 "y.tab.c"
    break;

  case 6: /* unit: func_declaration  */
#line 556 "1905114.y"
                       {
		writer2<<"unit : func_declaration"<<endl;
		(yyval.symbol_info)=new SymbolInfo("unit","func_declaration");
		(yyval.symbol_info)->sl=(yyvsp[0].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 1909 "y.tab.c"
    break;

  case 7: /* unit: func_definition  */
#line 564 "1905114.y"
                      {
		writer2<<"unit : func_definition"<<endl;
		(yyval.symbol_info)=new SymbolInfo("unit","func_definition");
		(yyval.symbol_info)->sl=(yyvsp[0].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 1922 "y.tab.c"
    break;

  case 8: /* func_declaration: type_specifier ID LPAREN parameter_list RPAREN SEMICOLON  */
#line 574 "1905114.y"
                                                                            {
		writer2<<"func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON"<<endl;
		(yyval.symbol_info)=new SymbolInfo("func_declaration","type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
		(yyval.symbol_info)->sl=(yyvsp[-5].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-5].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-4].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-3].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-2].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 1940 "y.tab.c"
    break;

  case 9: /* func_declaration: type_specifier ID LPAREN RPAREN SEMICOLON  */
#line 587 "1905114.y"
                                                    {
		writer2<<"func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON"<<endl;
		(yyval.symbol_info)=new SymbolInfo("func_declaration","type_specifier ID LPAREN RPAREN SEMICOLON");
		(yyval.symbol_info)->sl=(yyvsp[-4].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-4].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-3].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-2].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 1957 "y.tab.c"
    break;

  case 10: /* func_definition: type_specifier ID LPAREN parameter_list RPAREN compound_statement  */
#line 601 "1905114.y"
                                                                                    {
		writer2<<"func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement"<<endl;
		(yyval.symbol_info)=new SymbolInfo("func_definition","type_specifier ID LPAREN parameter_list RPAREN compound_statement");
		(yyval.symbol_info)->sl=(yyvsp[-5].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-5].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-4].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-3].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-2].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 1975 "y.tab.c"
    break;

  case 11: /* func_definition: type_specifier ID LPAREN RPAREN compound_statement  */
#line 614 "1905114.y"
                                                             {
		writer2<<"func_definition : type_specifier ID LPAREN RPAREN compound_statement"<<endl;
		(yyval.symbol_info)=new SymbolInfo("func_definition","type_specifier ID LPAREN RPAREN compound_statement");
		(yyval.symbol_info)->sl=(yyvsp[-4].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-4].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-3].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-2].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 1992 "y.tab.c"
    break;

  case 12: /* parameter_list: parameter_list COMMA type_specifier ID  */
#line 629 "1905114.y"
                                                         {
		writer2<<"parameter_list : parameter_list COMMA type_specifier ID"<<endl;
		(yyval.symbol_info)=new SymbolInfo("parameter_list","parameter_list COMMA type_specifier ID");
		(yyval.symbol_info)->sl=(yyvsp[-3].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-3].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-2].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2008 "y.tab.c"
    break;

  case 13: /* parameter_list: parameter_list COMMA type_specifier  */
#line 640 "1905114.y"
                                              {
		writer2<<"parameter_list : parameter_list COMMA type_specifier"<<endl;
		(yyval.symbol_info)=new SymbolInfo("parameter_list","parameter_list COMMA type_specifier");
		(yyval.symbol_info)->sl=(yyvsp[-2].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-2].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2023 "y.tab.c"
    break;

  case 14: /* parameter_list: type_specifier ID  */
#line 650 "1905114.y"
                            {
		writer2<<"parameter_list : type_specifier ID"<<endl;
		(yyval.symbol_info)=new SymbolInfo("parameter_list","type_specifier ID");
		(yyval.symbol_info)->sl=(yyvsp[-1].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2037 "y.tab.c"
    break;

  case 15: /* parameter_list: type_specifier  */
#line 659 "1905114.y"
                         {
		writer2<<"parameter_list : type_specifier"<<endl;
		(yyval.symbol_info)=new SymbolInfo("parameter_list","type_specifier");
		(yyval.symbol_info)->sl=(yyvsp[0].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2050 "y.tab.c"
    break;

  case 16: /* compound_statement: LCURL statements RCURL  */
#line 670 "1905114.y"
                                            {
	writer2<<"compound_statement : LCURL statements RCURL"<<endl;
		(yyval.symbol_info)=new SymbolInfo("compound_statement","LCURL statements RCURL");
		(yyval.symbol_info)->sl=(yyvsp[-2].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-2].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2065 "y.tab.c"
    break;

  case 17: /* compound_statement: LCURL RCURL  */
#line 680 "1905114.y"
                      {
		writer2<<"compound_statement : LCURL RCURL"<<endl;
		(yyval.symbol_info)=new SymbolInfo("compound_statement","LCURL RCURL");
		(yyval.symbol_info)->sl=(yyvsp[-1].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2079 "y.tab.c"
    break;

  case 18: /* var_declaration: type_specifier declaration_list SEMICOLON  */
#line 691 "1905114.y"
                                                            {
		writer2<<"var_declaration : type_specifier declaration_list SEMICOLON"<<endl;
		(yyval.symbol_info)=new SymbolInfo("var_declaration","type_specifier declaration_list SEMICOLON");
		(yyval.symbol_info)->sl=(yyvsp[-2].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-2].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2094 "y.tab.c"
    break;

  case 19: /* type_specifier: INT  */
#line 703 "1905114.y"
                      {
		writer2<<"type_specifier : INT"<<endl;
		(yyval.symbol_info)=new SymbolInfo("type_specifier","INT");
		(yyval.symbol_info)->sl=(yyvsp[0].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2107 "y.tab.c"
    break;

  case 20: /* type_specifier: FLOAT  */
#line 711 "1905114.y"
                {
		writer2<<"type_specifier : FLOAT"<<endl;
		(yyval.symbol_info)=new SymbolInfo("type_specifier","FLOAT");
		(yyval.symbol_info)->sl=(yyvsp[0].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2120 "y.tab.c"
    break;

  case 21: /* type_specifier: VOID  */
#line 719 "1905114.y"
               {
		writer2<<"type_specifier : VOID"<<endl;
		(yyval.symbol_info)=new SymbolInfo("type_specifier","VOID");
		(yyval.symbol_info)->sl=(yyvsp[0].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2133 "y.tab.c"
    break;

  case 22: /* declaration_list: declaration_list COMMA ID  */
#line 729 "1905114.y"
                                             {
		writer2<<"declaration_list : declaration_list COMMA ID"<<endl;
		(yyval.symbol_info)=new SymbolInfo("declaration_list","declaration_list COMMA ID");
		(yyval.symbol_info)->sl=(yyvsp[-2].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-2].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;

		}
#line 2149 "y.tab.c"
    break;

  case 23: /* declaration_list: declaration_list COMMA ID LTHIRD CONST_INT RTHIRD  */
#line 740 "1905114.y"
                                                            {
		writer2<<"declaration_list : declaration_list COMMA ID LTHIRD CONST_INT RTHIRD"<<endl;
		(yyval.symbol_info)=new SymbolInfo("declaration_list","declaration_list COMMA ID LTHIRD CONST_INT RTHIRD");
		(yyval.symbol_info)->sl=(yyvsp[-5].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-5].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-4].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-3].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-2].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2167 "y.tab.c"
    break;

  case 24: /* declaration_list: ID  */
#line 753 "1905114.y"
             {
		writer2<<"declaration_list : ID"<<endl;
		(yyval.symbol_info)=new SymbolInfo("declaration_list","ID");
		(yyval.symbol_info)->sl=(yyvsp[0].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;

		}
#line 2181 "y.tab.c"
    break;

  case 25: /* declaration_list: ID LTHIRD CONST_INT RTHIRD  */
#line 762 "1905114.y"
                                     {
		writer2<<"declaration_list : ID LTHIRD CONST_INT RTHIRD"<<endl;
		(yyval.symbol_info)=new SymbolInfo("declaration_list","ID LTHIRD CONST_INT RTHIRD");
		(yyval.symbol_info)->sl=(yyvsp[-3].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-3].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-2].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2197 "y.tab.c"
    break;

  case 26: /* statements: statement  */
#line 775 "1905114.y"
                       {
		writer2<<"statements : statement"<<endl;
		(yyval.symbol_info)=new SymbolInfo("statements","statement");
		(yyval.symbol_info)->sl=(yyvsp[0].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2210 "y.tab.c"
    break;

  case 27: /* statements: statements statement  */
#line 783 "1905114.y"
                               {
		writer2<<"statements : statements statement"<<endl;
		(yyval.symbol_info)=new SymbolInfo("statements","statements statement");
		(yyval.symbol_info)->sl=(yyvsp[-1].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2224 "y.tab.c"
    break;

  case 28: /* statement: var_declaration  */
#line 794 "1905114.y"
                            {
		writer2<<"statement : var_declaration"<<endl;
		(yyval.symbol_info)=new SymbolInfo("statement","var_declaration");
		(yyval.symbol_info)->sl=(yyvsp[0].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2237 "y.tab.c"
    break;

  case 29: /* statement: expression_statement  */
#line 802 "1905114.y"
                               {
		writer2<<"statement : expression_statement"<<endl;
		(yyval.symbol_info)=new SymbolInfo("statement","expression_statement");
		(yyval.symbol_info)->sl=(yyvsp[0].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2250 "y.tab.c"
    break;

  case 30: /* statement: compound_statement  */
#line 810 "1905114.y"
                             {
		writer2<<"statement : compound_statement"<<endl;
		(yyval.symbol_info)=new SymbolInfo("statement","compound_statement");
		(yyval.symbol_info)->sl=(yyvsp[0].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2263 "y.tab.c"
    break;

  case 31: /* statement: FOR LPAREN expression_statement expression_statement expression RPAREN statement  */
#line 818 "1905114.y"
                                                                                           {
		writer2<<"statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement"<<endl;
		(yyval.symbol_info)=new SymbolInfo("statement","FOR LPAREN expression_statement expression_statement expression RPAREN statement");
		(yyval.symbol_info)->sl=(yyvsp[-6].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-6].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-5].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-4].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-3].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-2].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2282 "y.tab.c"
    break;

  case 32: /* statement: IF LPAREN expression RPAREN statement  */
#line 832 "1905114.y"
                                                                      {
		writer2<<"statement : IF LPAREN expression RPAREN statement"<<endl;
		(yyval.symbol_info)=new SymbolInfo("statement","IF LPAREN expression RPAREN statement");
		(yyval.symbol_info)->sl=(yyvsp[-4].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-4].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-3].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-2].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;

		}
#line 2300 "y.tab.c"
    break;

  case 33: /* statement: IF LPAREN expression RPAREN statement ELSE statement  */
#line 845 "1905114.y"
                                                               {
		writer2<<"statement : IF LPAREN expression RPAREN statement ELSE statement"<<endl;
		(yyval.symbol_info)=new SymbolInfo("statement","IF LPAREN expression RPAREN statement ELSE statement");
		(yyval.symbol_info)->sl=(yyvsp[-6].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-6].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-5].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-4].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-3].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-2].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;

		}
#line 2320 "y.tab.c"
    break;

  case 34: /* statement: WHILE LPAREN expression RPAREN statement  */
#line 860 "1905114.y"
                                                   {
		writer2<<"statement : WHILE LPAREN expression RPAREN statement"<<endl;
		(yyval.symbol_info)=new SymbolInfo("statement","WHILE LPAREN expression RPAREN statement");
		(yyval.symbol_info)->sl=(yyvsp[-4].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-4].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-3].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-2].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2337 "y.tab.c"
    break;

  case 35: /* statement: PRINTLN LPAREN ID RPAREN SEMICOLON  */
#line 872 "1905114.y"
                                             {
		writer2<<"statement : PRINTLN LPAREN ID RPAREN SEMICOLON"<<endl;
		(yyval.symbol_info)=new SymbolInfo("statement","PRINTLN LPAREN ID RPAREN SEMICOLON");
		(yyval.symbol_info)->sl=(yyvsp[-4].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-4].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-3].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-2].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2354 "y.tab.c"
    break;

  case 36: /* statement: RETURN expression SEMICOLON  */
#line 884 "1905114.y"
                                      {
		writer2<<"statement : RETURN expression SEMICOLON"<<endl;
		(yyval.symbol_info)=new SymbolInfo("statement","RETURN expression SEMICOLON");
		(yyval.symbol_info)->sl=(yyvsp[-2].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-2].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2369 "y.tab.c"
    break;

  case 37: /* expression_statement: SEMICOLON  */
#line 896 "1905114.y"
                                        {
		writer2<<"expression_statement : SEMICOLON"<<endl;
		(yyval.symbol_info)=new SymbolInfo("expression_statement","SEMICOLON");
		(yyval.symbol_info)->sl=(yyvsp[0].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2382 "y.tab.c"
    break;

  case 38: /* expression_statement: expression SEMICOLON  */
#line 904 "1905114.y"
                               {
		writer2<<"expression_statement : expression SEMICOLON"<<endl;
		(yyval.symbol_info)=new SymbolInfo("expression_statement","expression SEMICOLON");
		(yyval.symbol_info)->sl=(yyvsp[-1].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2396 "y.tab.c"
    break;

  case 39: /* variable: ID  */
#line 915 "1905114.y"
              {
		writer2<<"variable : ID"<<endl;
		(yyval.symbol_info)=new SymbolInfo("variable","ID");
		(yyval.symbol_info)->sl=(yyvsp[0].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2409 "y.tab.c"
    break;

  case 40: /* variable: ID LTHIRD expression RTHIRD  */
#line 923 "1905114.y"
                                      {
		writer2<<"variable : ID LTHIRD expression RTHIRD"<<endl;
		(yyval.symbol_info)=new SymbolInfo("variable","ID LTHIRD expression RTHIRD");
		(yyval.symbol_info)->sl=(yyvsp[-3].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-3].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-2].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2425 "y.tab.c"
    break;

  case 41: /* expression: logic_expression  */
#line 936 "1905114.y"
                                {
		writer2<<"expression : logic_expression"<<endl;
		(yyval.symbol_info)=new SymbolInfo("expression","logic_expression");
		(yyval.symbol_info)->sl=(yyvsp[0].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2438 "y.tab.c"
    break;

  case 42: /* expression: variable ASSIGNOP logic_expression  */
#line 944 "1905114.y"
                                             {
		writer2<<"expression : variable ASSIGNOP logic_expression"<<endl;
		(yyval.symbol_info)=new SymbolInfo("expression","variable ASSIGNOP logic_expression");
		(yyval.symbol_info)->sl=(yyvsp[-2].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-2].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2453 "y.tab.c"
    break;

  case 43: /* logic_expression: rel_expression  */
#line 956 "1905114.y"
                                        {
		writer2<<"logic_expression : rel_expression"<<endl;
		(yyval.symbol_info)=new SymbolInfo("logic_expression","rel_expression");
		(yyval.symbol_info)->sl=(yyvsp[0].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2466 "y.tab.c"
    break;

  case 44: /* logic_expression: rel_expression LOGICOP rel_expression  */
#line 964 "1905114.y"
                                                {
		writer2<<"logic_expression : rel_expression LOGICOP rel_expression"<<endl;
		(yyval.symbol_info)=new SymbolInfo("logic_expression","rel_expression LOGICOP rel_expression");
		(yyval.symbol_info)->sl=(yyvsp[-2].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-2].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2481 "y.tab.c"
    break;

  case 45: /* rel_expression: simple_expression  */
#line 976 "1905114.y"
                                    {
		writer2<<"rel_expression : simple_expression"<<endl;
		(yyval.symbol_info)=new SymbolInfo("rel_expression","simple_expression");
		(yyval.symbol_info)->sl=(yyvsp[0].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2494 "y.tab.c"
    break;

  case 46: /* rel_expression: simple_expression RELOP simple_expression  */
#line 984 "1905114.y"
                                                    {
		writer2<<"rel_expression : simple_expression RELOP simple_expression"<<endl;
		(yyval.symbol_info)=new SymbolInfo("rel_expression","simple_expression RELOP simple_expression");
		(yyval.symbol_info)->sl=(yyvsp[-2].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-2].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2509 "y.tab.c"
    break;

  case 47: /* simple_expression: term  */
#line 996 "1905114.y"
                         {
		writer2<<"simple_expression : term"<<endl;
		(yyval.symbol_info)=new SymbolInfo("simple_expression","term");
		(yyval.symbol_info)->sl=(yyvsp[0].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2522 "y.tab.c"
    break;

  case 48: /* simple_expression: simple_expression ADDOP term  */
#line 1004 "1905114.y"
                                       {
		writer2<<"simple_expression : simple_expression ADDOP term"<<endl;
		(yyval.symbol_info)=new SymbolInfo("simple_expression","simple_expression ADDOP term");
		(yyval.symbol_info)->sl=(yyvsp[-2].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-2].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2537 "y.tab.c"
    break;

  case 49: /* term: unary_expression  */
#line 1016 "1905114.y"
                         {
		writer2<<"term : unary_expression"<<endl;
		(yyval.symbol_info)=new SymbolInfo("term","unary_expression");
		(yyval.symbol_info)->sl=(yyvsp[0].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		}
#line 2550 "y.tab.c"
    break;

  case 50: /* term: term MULOP unary_expression  */
#line 1024 "1905114.y"
                                   {
		writer2<<"term : term MULOP unary_expression"<<endl;
		(yyval.symbol_info)=new SymbolInfo("term","term MULOP unary_expression");
		(yyval.symbol_info)->sl=(yyvsp[-2].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-2].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		
		}
#line 2566 "y.tab.c"
    break;

  case 51: /* unary_expression: ADDOP unary_expression  */
#line 1037 "1905114.y"
                                          {
		writer2<<"unary_expression : ADDOP unary_expression"<<endl;
		(yyval.symbol_info)=new SymbolInfo("unary_expression","ADDOP unary_expression");
		(yyval.symbol_info)->sl=(yyvsp[-1].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		
		}
#line 2581 "y.tab.c"
    break;

  case 52: /* unary_expression: NOT unary_expression  */
#line 1047 "1905114.y"
                               {
		writer2<<"unary_expression : NOT unary_expression"<<endl;
		(yyval.symbol_info)=new SymbolInfo("unary_expression","NOT unary_expression");
		(yyval.symbol_info)->sl=(yyvsp[-1].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		
		}
#line 2596 "y.tab.c"
    break;

  case 53: /* unary_expression: factor  */
#line 1057 "1905114.y"
                 {
		writer2<<"unary_expression : factor"<<endl;
		(yyval.symbol_info)=new SymbolInfo("unary_expression","factor");
		(yyval.symbol_info)->sl=(yyvsp[0].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		
		}
#line 2610 "y.tab.c"
    break;

  case 54: /* factor: variable  */
#line 1068 "1905114.y"
                   {
		writer2<<"factor : variable"<<endl;
		(yyval.symbol_info)=new SymbolInfo("factor","variable");
		(yyval.symbol_info)->sl=(yyvsp[0].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		
		}
#line 2624 "y.tab.c"
    break;

  case 55: /* factor: ID LPAREN argument_list RPAREN  */
#line 1077 "1905114.y"
                                        {
		writer2<<"factor : ID LPAREN argument_list RPAREN"<<endl;
		(yyval.symbol_info)=new SymbolInfo("factor","ID LPAREN argument_list RPAREN");
		(yyval.symbol_info)->sl=(yyvsp[-3].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-3].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-2].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		
		}
#line 2641 "y.tab.c"
    break;

  case 56: /* factor: LPAREN expression RPAREN  */
#line 1089 "1905114.y"
                                  {
		writer2<<"factor : ID LPAREN argument_list RPAREN"<<endl;
		(yyval.symbol_info)=new SymbolInfo("factor","LPAREN expression RPAREN");
		(yyval.symbol_info)->sl=(yyvsp[-2].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-2].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		
		}
#line 2657 "y.tab.c"
    break;

  case 57: /* factor: CONST_INT  */
#line 1100 "1905114.y"
                   {
		writer2<<"factor : CONST_INT"<<endl;
		(yyval.symbol_info)=new SymbolInfo("factor","CONST_INT");
		(yyval.symbol_info)->sl=(yyvsp[0].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		
		}
#line 2671 "y.tab.c"
    break;

  case 58: /* factor: CONST_FLOAT  */
#line 1109 "1905114.y"
                     {
		writer2<<"factor : CONST_FLOAT"<<endl;
		(yyval.symbol_info)=new SymbolInfo("factor","CONST_FLOAT");
		(yyval.symbol_info)->sl=(yyvsp[0].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		
		}
#line 2685 "y.tab.c"
    break;

  case 59: /* factor: variable INCOP  */
#line 1118 "1905114.y"
                        {
		writer2<<"factor : variable INCOP"<<endl;
		(yyval.symbol_info)=new SymbolInfo("factor","variable INCOP");
		(yyval.symbol_info)->sl=(yyvsp[-1].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		
		}
#line 2700 "y.tab.c"
    break;

  case 60: /* factor: variable DECOP  */
#line 1128 "1905114.y"
                        {
		writer2<<"factor : variable DECOP"<<endl;
		(yyval.symbol_info)=new SymbolInfo("factor","variable DECOP");
		(yyval.symbol_info)->sl=(yyvsp[-1].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		
		}
#line 2715 "y.tab.c"
    break;

  case 61: /* argument_list: arguments  */
#line 1140 "1905114.y"
                         {
		writer2<<"argument_list : arguments"<<endl;
		(yyval.symbol_info)=new SymbolInfo("argument_list","arguments");
		(yyval.symbol_info)->sl=(yyvsp[0].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		
		}
#line 2729 "y.tab.c"
    break;

  case 62: /* argument_list: %empty  */
#line 1149 "1905114.y"
          {
		writer2<<"argument_list : ephsilon"<<endl;
		(yyval.symbol_info)=new SymbolInfo("argument_list","ephsilon");
		(yyval.symbol_info)->sl=line_count;
		(yyval.symbol_info)->el=line_count;
		(yyval.symbol_info)->isLeaf=true;
	}
#line 2741 "y.tab.c"
    break;

  case 63: /* arguments: arguments COMMA logic_expression  */
#line 1158 "1905114.y"
                                            {
		writer2<<"arguments : arguments COMMA logic_expression"<<endl;
		(yyval.symbol_info)=new SymbolInfo("arguments","arguments COMMA logic_expression");
		(yyval.symbol_info)->sl=(yyvsp[-2].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[-2].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[-1].symbol_info));
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info));
		(yyval.symbol_info)->isLeaf=false;
		
		}
#line 2757 "y.tab.c"
    break;

  case 64: /* arguments: logic_expression  */
#line 1169 "1905114.y"
                          {
		writer2<<"arguments : logic_expression"<<endl;
		(yyval.symbol_info)=new SymbolInfo("arguments","logic_expression");
		(yyval.symbol_info)->sl=(yyvsp[0].symbol_info)->sl;
		(yyval.symbol_info)->el=(yyvsp[0].symbol_info)->el;
		(yyval.symbol_info)->children.push_back((yyvsp[0].symbol_info)); 
		(yyval.symbol_info)->isLeaf=false;
		
		}
#line 2771 "y.tab.c"
    break;


#line 2775 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1181 "1905114.y"

int main(int argc,char *argv[])
{
	

	if(argc != 2){
        cout<<"Please provide input file name and try again."<<endl;
        return 0;
    }
    FILE *fin = freopen(argv[1], "r", stdin);
    if(fin == nullptr){
        cout<<"Can't open specified file."<<endl;
        return 0;
    }

	cout<<argv[1]<<" opened successfully."<<endl;

	

	writer.open("parsetree.txt");
	writer2.open("log.txt");
	writer3.open("error.txt");
	asmWriter.open("test_1.asm");


	yyin=fin;
	yyparse();

	fclose(fin);

	writer.close();
	writer2.close();
	writer3.close();
	asmWriter.close();

	return 0;
}

