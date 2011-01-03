/* A Bison parser, made by GNU Bison 2.4.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006,
   2009, 2010 Free Software Foundation, Inc.
   
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
     DECREMENT = 258,
     INCREMENT = 259,
     RIGHT_SHIFT = 260,
     LEFT_SHIFT = 261,
     GREATER_OR_EQUAL = 262,
     LESS_OR_EQUAL = 263,
     NOT_EQUAL = 264,
     EQUAL = 265,
     AND = 266,
     OR = 267,
     MODULUS_ASSIGN = 268,
     DIVIDE_ASSIGN = 269,
     MULTIPLY_ASSIGN = 270,
     SUBTRACT_ASSIGN = 271,
     ADD_ASSIGN = 272,
     BIT_OR_ASSIGN = 273,
     BIT_AND_ASSIGN = 274,
     IDENTIFIER = 275,
     TYPE = 276,
     STRING = 277,
     NUMBER = 278,
     PUBLIC = 279,
     PRIVATE = 280,
     STATIC = 281,
     NATIVE = 282,
     CLASS = 283,
     INTERFACE = 284,
     STRUCT = 285,
     MODULE = 286,
     IMPORT = 287,
     DEF = 288,
     VAR = 289,
     INIT = 290,
     DESTROY = 291,
     SEPARATOR = 292,
     WHEN = 293,
     CASE = 294,
     WHILE = 295,
     ELSE = 296,
     UNTIL = 297,
     IF = 298,
     DO = 299,
     FOR = 300,
     RETURN = 301,
     RIGHT_ARROW = 302,
     LEFT_ARROW = 303,
     SCOPE = 304
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1685 of yacc.c  */
#line 20 "compiler2/grammar.y"
 Expression* expression; 

/* Line 1685 of yacc.c  */
#line 21 "compiler2/grammar.y"
 Statement* statement; 

/* Line 1685 of yacc.c  */
#line 22 "compiler2/grammar.y"
 Name* identifier; 

/* Line 1685 of yacc.c  */
#line 23 "compiler2/grammar.y"
 Unit* unit; 

/* Line 1685 of yacc.c  */
#line 24 "compiler2/grammar.y"
 Feature* feature; 

/* Line 1685 of yacc.c  */
#line 25 "compiler2/grammar.y"
 Formal* formal; 

/* Line 1685 of yacc.c  */
#line 26 "compiler2/grammar.y"
 Type* type; 

/* Line 1685 of yacc.c  */
#line 27 "compiler2/grammar.y"
 Generic* generic; 

/* Line 1685 of yacc.c  */
#line 28 "compiler2/grammar.y"
 int null; 

/* Line 1685 of yacc.c  */
#line 29 "compiler2/grammar.y"
 int flag; 


/* Line 1685 of yacc.c  */
#line 140 "compiler2/grammar.hpp"
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



