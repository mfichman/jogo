
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     BIT_XOR_ASSIGN = 258,
     BIT_OR_ASSIGN = 259,
     BIT_AND_ASSIGN = 260,
     SUBTRACT_ASSIGN = 261,
     ADD_ASSIGN = 262,
     POWER_ASSIGN = 263,
     MODULUS_ASSIGN = 264,
     DIVIDE_ASSIGN = 265,
     MULTIPLY_ASSIGN = 266,
     OR = 267,
     AND = 268,
     XOR = 269,
     NOT_EQUAL = 270,
     EQUAL = 271,
     COMPARE = 272,
     GREATER_OR_EQUAL = 273,
     LESS_OR_EQUAL = 274,
     RIGHT_SHIFT = 275,
     LEFT_SHIFT = 276,
     NOT = 277,
     DECREMENT = 278,
     INCREMENT = 279,
     IDENTIFIER = 280,
     TYPE = 281,
     OPERATOR = 282,
     STRING = 283,
     NUMBER = 284,
     BOOLEAN = 285,
     PUBLIC = 286,
     PRIVATE = 287,
     STATIC = 288,
     NATIVE = 289,
     IMPORT = 290,
     SEPARATOR = 291,
     WHEN = 292,
     CASE = 293,
     WHILE = 294,
     ELSE = 295,
     UNTIL = 296,
     IF = 297,
     DO = 298,
     FOR = 299,
     RETURN = 300,
     RIGHT_ARROW = 301,
     LEFT_ARROW = 302,
     SCOPE = 303,
     LET = 304,
     IN = 305,
     YIELD = 306,
     FORK = 307
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 21 "src/compiler/Grammar.yy"
 Expression* expression; 

/* Line 1676 of yacc.c  */
#line 22 "src/compiler/Grammar.yy"
 Statement* statement; 

/* Line 1676 of yacc.c  */
#line 23 "src/compiler/Grammar.yy"
 Name* identifier; 

/* Line 1676 of yacc.c  */
#line 24 "src/compiler/Grammar.yy"
 Feature* feature; 

/* Line 1676 of yacc.c  */
#line 25 "src/compiler/Grammar.yy"
 Formal* formal; 

/* Line 1676 of yacc.c  */
#line 26 "src/compiler/Grammar.yy"
 Type* type; 

/* Line 1676 of yacc.c  */
#line 27 "src/compiler/Grammar.yy"
 Generic* generic; 

/* Line 1676 of yacc.c  */
#line 28 "src/compiler/Grammar.yy"
 Variable* variable; 

/* Line 1676 of yacc.c  */
#line 29 "src/compiler/Grammar.yy"
 int null; 

/* Line 1676 of yacc.c  */
#line 30 "src/compiler/Grammar.yy"
 int flag; 


/* Line 1676 of yacc.c  */
#line 144 "src/compiler/Grammar.hpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif



#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



