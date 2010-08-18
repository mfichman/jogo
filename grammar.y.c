
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "grammar.y"

#include <parser.h>
#include <expr.h>
#include <type.h>
#include <stmt.h>
#include <var.h>
#include <func.h>
#include <unit.h>
#include <import.h>
#include <symtab.h>
#include <def.h>
#include <op.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PARSER_CHECK(x) if (!((x))) YYERROR;


/* Line 189 of yacc.c  */
#line 93 "grammar.y.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOK_TYPE = 258,
     TOK_PRIMITIVE = 259,
     TOK_IDENT = 260,
     TOK_CONST = 261,
     TOK_STRING = 262,
     TOK_NUMBER = 263,
     TOK_CLASS = 264,
     TOK_INTERFACE = 265,
     TOK_STRUCT = 266,
     TOK_MODULE = 267,
     TOK_IMPORT = 268,
     TOK_DEF = 269,
     TOK_INIT = 270,
     TOK_DESTROY = 271,
     TOK_PUBLIC = 272,
     TOK_PRIVATE = 273,
     TOK_PROTECTED = 274,
     TOK_STATIC = 275,
     TOK_NATIVE = 276,
     TOK_WHILE = 277,
     TOK_ELSE = 278,
     TOK_UNTIL = 279,
     TOK_IF = 280,
     TOK_DO = 281,
     TOK_FOR = 282,
     TOK_RETURN = 283,
     TOK_EQUAL = 284,
     TOK_NOTEQUAL = 285,
     TOK_SCOPE = 286,
     TOK_OR = 287,
     TOK_AND = 288,
     TOK_GE = 289,
     TOK_LE = 290,
     TOK_LSHIFT = 291,
     TOK_RSHIFT = 292,
     TOK_MUL_ASSIGN = 293,
     TOK_DIV_ASSIGN = 294,
     TOK_SUB_ASSIGN = 295,
     TOK_ADD_ASSIGN = 296,
     TOK_MOD_ASSIGN = 297,
     TOK_BITOR_ASSIGN = 298,
     TOK_BITAND_ASSIGN = 299,
     TOK_INC = 300,
     TOK_DEC = 301
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 20 "grammar.y"
 expr_t *expr; 

/* Line 214 of yacc.c  */
#line 21 "grammar.y"
 stmt_t *stmt; 

/* Line 214 of yacc.c  */
#line 22 "grammar.y"
 type_t *type; 

/* Line 214 of yacc.c  */
#line 23 "grammar.y"
 unit_t *unit; 

/* Line 214 of yacc.c  */
#line 24 "grammar.y"
 var_t *var; 

/* Line 214 of yacc.c  */
#line 25 "grammar.y"
 func_t *func; 

/* Line 214 of yacc.c  */
#line 26 "grammar.y"
 import_t *import; 

/* Line 214 of yacc.c  */
#line 27 "grammar.y"
 def_t *def; 

/* Line 214 of yacc.c  */
#line 28 "grammar.y"
 char *string; 

/* Line 214 of yacc.c  */
#line 29 "grammar.y"
 int null; 

/* Line 214 of yacc.c  */
#line 30 "grammar.y"
 int flag; 


/* Line 214 of yacc.c  */
#line 219 "grammar.y.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 231 "grammar.y.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  12
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   498

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  71
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  41
/* YYNRULES -- Number of rules.  */
#define YYNRULES  153
/* YYNRULES -- Number of states.  */
#define YYNSTATES  319

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   301

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    66,     2,     2,     2,    65,    59,     2,
      49,    50,    52,    62,    51,    63,    68,    64,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    56,    47,
      61,    48,    60,    53,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    69,     2,    70,    58,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    54,    57,    55,    67,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     8,    13,    18,    23,    26,    29,    32,
      35,    38,    41,    44,    47,    48,    51,    54,    57,    60,
      61,    64,    67,    70,    73,    76,    79,    80,    83,    86,
      89,    92,    93,    97,   102,   110,   116,   124,   129,   134,
     141,   147,   155,   162,   167,   171,   175,   178,   183,   186,
     188,   190,   192,   193,   195,   196,   198,   201,   204,   206,
     209,   212,   216,   218,   222,   224,   228,   231,   235,   236,
     246,   255,   261,   267,   275,   283,   287,   289,   292,   296,
     299,   304,   306,   307,   313,   321,   323,   325,   329,   331,
     335,   339,   343,   347,   351,   355,   359,   363,   365,   369,
     371,   375,   377,   381,   385,   387,   391,   393,   397,   401,
     403,   407,   411,   415,   419,   421,   425,   429,   431,   435,
     439,   441,   445,   449,   453,   455,   458,   461,   464,   467,
     470,   473,   476,   478,   485,   491,   495,   500,   503,   506,
     508,   513,   517,   519,   526,   532,   536,   540,   545,   549,
     553,   555,   557,   561
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      72,     0,    -1,     9,    90,    47,    73,    -1,    10,    90,
      47,    74,    -1,    11,    90,    47,    75,    -1,    12,    90,
      47,    76,    -1,    77,    73,    -1,    78,    73,    -1,    79,
      73,    -1,    80,    73,    -1,    81,    73,    -1,    82,    73,
      -1,    83,    73,    -1,     1,    73,    -1,    -1,    77,    74,
      -1,    78,    74,    -1,    84,    74,    -1,     1,    74,    -1,
      -1,    77,    75,    -1,    78,    75,    -1,    79,    75,    -1,
      80,    75,    -1,    82,    75,    -1,     1,    75,    -1,    -1,
      77,    76,    -1,    78,    76,    -1,    82,    76,    -1,     1,
      76,    -1,    -1,    13,    90,    47,    -1,    14,    89,     3,
      47,    -1,    87,    88,    89,     5,    48,    96,    47,    -1,
      87,    88,    89,     5,    47,    -1,    87,    88,    89,     6,
      48,    96,    47,    -1,    15,    85,    87,    91,    -1,    16,
      49,    50,    91,    -1,     5,    85,    87,    88,    89,    91,
      -1,     5,    85,    87,    88,    91,    -1,     5,    85,    87,
      88,    21,    89,    47,    -1,     5,    85,    87,    88,    21,
      47,    -1,     5,    85,    89,    47,    -1,     5,    85,    47,
      -1,    49,    86,    50,    -1,    49,    50,    -1,    89,     5,
      51,    86,    -1,    89,     5,    -1,    17,    -1,    18,    -1,
      19,    -1,    -1,    20,    -1,    -1,     4,    -1,     4,    52,
      -1,     4,    53,    -1,    90,    -1,    90,    52,    -1,    90,
      53,    -1,     3,    31,    90,    -1,     3,    -1,     6,    31,
      90,    -1,     6,    -1,    54,    92,    55,    -1,    92,    93,
      -1,    92,     1,    47,    -1,    -1,    27,    49,    94,    47,
      94,    47,    94,    50,    91,    -1,    27,    49,    89,     5,
      56,    96,    50,    91,    -1,    24,    49,    94,    50,    91,
      -1,    22,    49,    94,    50,    91,    -1,    26,    91,    24,
      49,    94,    50,    47,    -1,    26,    91,    22,    49,    94,
      50,    47,    -1,    89,     5,    47,    -1,    95,    -1,    94,
      47,    -1,    28,    96,    47,    -1,    28,    47,    -1,    89,
       5,    48,    96,    -1,    96,    -1,    -1,    25,    49,    94,
      50,    91,    -1,    25,    49,    94,    50,    91,    23,    95,
      -1,    91,    -1,    97,    -1,    97,    53,    98,    -1,    98,
      -1,   108,    48,    98,    -1,   108,    38,    98,    -1,   108,
      39,    98,    -1,   108,    42,    98,    -1,   108,    40,    98,
      -1,   108,    41,    98,    -1,   108,    44,    98,    -1,   108,
      43,    98,    -1,    99,    -1,    99,    32,   100,    -1,   100,
      -1,   100,    33,   101,    -1,   101,    -1,   101,    57,   102,
      -1,   101,    58,   102,    -1,   102,    -1,   102,    59,   103,
      -1,   103,    -1,   103,    29,   104,    -1,   103,    30,   104,
      -1,   104,    -1,   104,    60,   105,    -1,   104,    61,   105,
      -1,   104,    34,   105,    -1,   104,    35,   105,    -1,   105,
      -1,   105,    36,   106,    -1,   105,    37,   106,    -1,   106,
      -1,   106,    62,   107,    -1,   106,    63,   107,    -1,   107,
      -1,   107,    52,   108,    -1,   107,    64,   108,    -1,   107,
      65,   108,    -1,   108,    -1,    45,   108,    -1,    46,   108,
      -1,    62,   108,    -1,    63,   108,    -1,    66,   108,    -1,
      67,   108,    -1,    52,   108,    -1,   109,    -1,   109,    68,
       5,    49,   111,    50,    -1,   109,    68,     5,    49,    50,
      -1,   109,    68,     5,    -1,   109,    69,    96,    70,    -1,
     109,    45,    -1,   109,    46,    -1,   110,    -1,     5,    49,
     111,    50,    -1,     5,    49,    50,    -1,     5,    -1,    89,
      68,     5,    49,   111,    50,    -1,    89,    68,     5,    49,
      50,    -1,    89,    68,     5,    -1,    89,    68,     6,    -1,
      89,    49,   111,    50,    -1,    89,    49,    50,    -1,    49,
      96,    50,    -1,     7,    -1,     8,    -1,    96,    51,   111,
      -1,    96,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   136,   136,   140,   144,   148,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   167,   168,   169,   170,   171,
     175,   176,   177,   178,   179,   180,   181,   185,   186,   187,
     188,   189,   193,   197,   201,   205,   208,   214,   221,   227,
     231,   238,   242,   249,   252,   258,   259,   263,   267,   273,
     274,   275,   276,   280,   281,   285,   286,   287,   288,   291,
     295,   302,   303,   304,   307,   313,   320,   321,   322,   329,
     332,   335,   338,   341,   344,   347,   350,   351,   352,   353,
     357,   360,   363,   367,   370,   373,   376,   379,   382,   386,
     389,   392,   395,   398,   401,   404,   407,   410,   414,   417,
     421,   424,   428,   431,   434,   438,   441,   445,   448,   451,
     455,   458,   461,   464,   467,   471,   474,   477,   481,   484,
     487,   491,   494,   497,   500,   504,   505,   506,   507,   508,
     509,   510,   511,   515,   519,   523,   527,   530,   533,   536,
     540,   544,   548,   552,   556,   560,   564,   568,   572,   576,
     577,   578,   582,   586
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOK_TYPE", "TOK_PRIMITIVE", "TOK_IDENT",
  "TOK_CONST", "TOK_STRING", "TOK_NUMBER", "TOK_CLASS", "TOK_INTERFACE",
  "TOK_STRUCT", "TOK_MODULE", "TOK_IMPORT", "TOK_DEF", "TOK_INIT",
  "TOK_DESTROY", "TOK_PUBLIC", "TOK_PRIVATE", "TOK_PROTECTED",
  "TOK_STATIC", "TOK_NATIVE", "TOK_WHILE", "TOK_ELSE", "TOK_UNTIL",
  "TOK_IF", "TOK_DO", "TOK_FOR", "TOK_RETURN", "TOK_EQUAL", "TOK_NOTEQUAL",
  "TOK_SCOPE", "TOK_OR", "TOK_AND", "TOK_GE", "TOK_LE", "TOK_LSHIFT",
  "TOK_RSHIFT", "TOK_MUL_ASSIGN", "TOK_DIV_ASSIGN", "TOK_SUB_ASSIGN",
  "TOK_ADD_ASSIGN", "TOK_MOD_ASSIGN", "TOK_BITOR_ASSIGN",
  "TOK_BITAND_ASSIGN", "TOK_INC", "TOK_DEC", "';'", "'='", "'('", "')'",
  "','", "'*'", "'?'", "'{'", "'}'", "':'", "'|'", "'^'", "'&'", "'>'",
  "'<'", "'+'", "'-'", "'/'", "'%'", "'!'", "'~'", "'.'", "'['", "']'",
  "$accept", "translation_unit", "class", "interface", "struct", "module",
  "import", "def", "variable", "constructor", "destructor", "function",
  "native", "prototype", "parameter_signature", "parameter_list", "access",
  "static", "type", "qualified_name", "block", "statement_list",
  "statement", "clause", "conditional", "expression", "nullable",
  "assignment", "logical_or", "logical_and", "bitwise_or", "bitwise_and",
  "equality", "relation", "shift", "addition", "multiplication", "unary",
  "postfix", "primary", "argument_list", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,    59,    61,    40,
      41,    44,    42,    63,   123,   125,    58,   124,    94,    38,
      62,    60,    43,    45,    47,    37,    33,   126,    46,    91,
      93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    71,    72,    72,    72,    72,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    74,    74,    74,    74,    74,
      75,    75,    75,    75,    75,    75,    75,    76,    76,    76,
      76,    76,    77,    78,    79,    79,    79,    80,    81,    82,
      82,    83,    83,    84,    84,    85,    85,    86,    86,    87,
      87,    87,    87,    88,    88,    89,    89,    89,    89,    89,
      89,    90,    90,    90,    90,    91,    92,    92,    92,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      94,    94,    94,    95,    95,    95,    96,    97,    97,    98,
      98,    98,    98,    98,    98,    98,    98,    98,    99,    99,
     100,   100,   101,   101,   101,   102,   102,   103,   103,   103,
     104,   104,   104,   104,   104,   105,   105,   105,   106,   106,
     106,   107,   107,   107,   107,   108,   108,   108,   108,   108,
     108,   108,   108,   109,   109,   109,   109,   109,   109,   109,
     110,   110,   110,   110,   110,   110,   110,   110,   110,   110,
     110,   110,   111,   111
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     4,     4,     4,     2,     2,     2,     2,
       2,     2,     2,     2,     0,     2,     2,     2,     2,     0,
       2,     2,     2,     2,     2,     2,     0,     2,     2,     2,
       2,     0,     3,     4,     7,     5,     7,     4,     4,     6,
       5,     7,     6,     4,     3,     3,     2,     4,     2,     1,
       1,     1,     0,     1,     0,     1,     2,     2,     1,     2,
       2,     3,     1,     3,     1,     3,     2,     3,     0,     9,
       8,     5,     5,     7,     7,     3,     1,     2,     3,     2,
       4,     1,     0,     5,     7,     1,     1,     3,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     3,     1,
       3,     1,     3,     3,     1,     3,     1,     3,     3,     1,
       3,     3,     3,     3,     1,     3,     3,     1,     3,     3,
       1,     3,     3,     3,     1,     2,     2,     2,     2,     2,
       2,     2,     1,     6,     5,     3,     4,     2,     2,     1,
       4,     3,     1,     6,     5,     3,     3,     4,     3,     3,
       1,     1,     3,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,    62,    64,     0,     0,
       0,     0,     1,     0,     0,     0,     0,     0,     0,    61,
      63,     0,     0,     0,     0,     0,     0,    49,    50,    51,
       2,     0,     0,     0,     0,     0,     0,     0,    54,     0,
       0,     3,     0,     0,     0,     0,     0,     4,     0,     0,
       0,     0,     0,     0,     5,     0,     0,     0,    13,     0,
      52,     0,    55,     0,    58,    52,     0,     6,     7,     8,
       9,    10,    11,    12,    53,     0,    18,     0,    15,    16,
      17,    25,    52,    20,    21,    22,    23,    24,    30,    27,
      28,    29,    46,     0,     0,    54,    32,    56,    57,     0,
      59,    60,     0,     0,     0,    44,     0,    54,    45,    48,
       0,    33,    68,    37,    38,     0,     0,    43,     0,     0,
       0,     0,    40,     0,    35,     0,     0,    47,    42,     0,
      39,     0,   142,   150,   151,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     0,     0,     0,     0,
       0,    85,    66,     0,    76,    81,    86,    88,    97,    99,
     101,   104,   106,   109,   114,   117,   120,   124,   132,   139,
       0,     0,     0,    41,    67,     0,    82,    82,    82,     0,
      82,    79,     0,   125,   126,     0,   131,   127,   128,   129,
     130,     0,     0,     0,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   137,   138,     0,     0,    34,    36,   141,   153,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    78,
     149,    75,     0,   148,     0,   145,   146,    87,    98,   124,
     100,   102,   103,   105,   107,   108,   112,   113,   110,   111,
     115,   116,   118,   119,   121,   122,   123,    90,    91,    93,
      94,    92,    96,    95,    89,   135,     0,     0,   140,     0,
       0,     0,     0,    82,    82,     0,    82,    80,   147,     0,
       0,   136,   152,    72,    71,    83,     0,     0,     0,     0,
     144,     0,   134,     0,     0,     0,     0,     0,    82,   143,
     133,    84,    74,    73,     0,     0,    70,     0,    69
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,    30,    41,    47,    54,    31,    32,    33,    34,
      35,    36,    37,    44,    60,    93,    38,    75,   170,    64,
     122,   123,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     230
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -187
static const yytype_int16 yypact[] =
{
     288,    27,    27,    27,    27,    23,    34,    41,     7,    28,
      43,    46,  -187,    27,    27,   420,   278,   441,   301,  -187,
    -187,   420,   -15,    27,   445,   -15,    68,  -187,  -187,  -187,
    -187,   420,   420,   420,   420,   420,   420,   420,   102,   278,
     -15,  -187,   278,   278,   278,   441,   -15,  -187,   441,   441,
     441,   441,   441,   301,  -187,   301,   301,   301,  -187,    64,
     233,    93,   -40,   144,    -5,   233,   110,  -187,  -187,  -187,
    -187,  -187,  -187,  -187,  -187,   445,  -187,   112,  -187,  -187,
    -187,  -187,   233,  -187,  -187,  -187,  -187,  -187,  -187,  -187,
    -187,  -187,  -187,   116,   172,   102,  -187,  -187,  -187,   165,
    -187,  -187,   137,   137,   122,  -187,   188,   102,  -187,   210,
      25,  -187,  -187,  -187,  -187,   160,   216,  -187,    58,   445,
     117,   137,  -187,   241,  -187,   138,   138,  -187,  -187,   238,
    -187,   242,   231,  -187,  -187,   245,   256,   271,   137,   279,
      11,   138,   138,   138,   138,  -187,   138,   138,   138,   138,
       3,  -187,  -187,   284,  -187,  -187,   294,  -187,   331,   336,
     128,   314,   209,    90,   219,   214,   150,   432,    37,  -187,
     -13,   329,   342,  -187,  -187,    35,   138,   138,   138,   133,
     138,  -187,   347,  -187,  -187,   361,  -187,  -187,  -187,  -187,
    -187,   234,   126,   334,  -187,   138,   138,   138,   138,   138,
     138,   138,   138,   138,   138,   138,   138,   138,   138,   138,
     138,   138,   138,   138,   138,   138,   138,   138,   138,   138,
     138,   138,  -187,  -187,   417,   138,  -187,  -187,  -187,   380,
     382,    20,   393,   400,   403,   408,   428,    40,   422,  -187,
    -187,  -187,   138,  -187,   429,   433,  -187,  -187,   336,  -187,
     128,   314,   314,   209,    90,    90,   219,   219,   219,   219,
     214,   214,   150,   150,  -187,  -187,  -187,  -187,  -187,  -187,
    -187,  -187,  -187,  -187,  -187,   434,   411,   138,  -187,   430,
     137,   137,   137,   138,   138,    44,   138,  -187,  -187,   191,
     267,  -187,  -187,  -187,  -187,   461,   435,   436,   138,   440,
    -187,   438,  -187,   439,    -4,   443,   444,   442,   138,  -187,
    -187,  -187,  -187,  -187,   137,   446,  -187,   137,  -187
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -187,  -187,   431,   167,   273,   315,   293,   335,   350,   358,
    -187,   309,  -187,  -187,   -14,   374,   -16,    12,   -24,   135,
    -101,  -187,  -187,  -173,   190,  -116,  -187,   198,  -187,   299,
     300,   157,   298,   180,   224,   189,   195,    21,  -187,  -187,
    -186
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -83
static const yytype_int16 yytable[] =
{
      63,   113,   114,   232,   233,   234,   244,   238,   191,   171,
     172,    65,    97,    98,     6,    62,   132,     7,   133,   134,
     130,   137,   151,    12,   182,   279,    77,   185,     6,    62,
       6,     7,    82,     7,    59,    94,   192,   179,     6,    62,
     132,     7,   133,   134,    95,   285,   120,   100,   101,   102,
     112,   104,   192,   106,    15,   193,   141,   142,   181,   229,
     143,     6,    62,   144,     7,    13,   107,     6,    62,   192,
       7,   193,    14,   146,   147,    16,   229,   148,   149,   112,
     141,   142,   222,   223,   143,   228,   121,   144,   193,   192,
      17,   292,   242,    18,   121,    94,   129,   146,   147,   150,
     298,   148,   149,   301,   303,   224,   225,   110,   193,   276,
     296,   297,   112,   299,    92,     6,    62,    66,     7,   118,
       6,    62,    74,     7,   203,   204,   287,   115,   116,     6,
      62,   132,     7,   133,   134,   315,     8,     9,    10,    11,
      96,     6,    62,   132,     7,   133,   134,    99,    19,    20,
     205,   206,   231,   231,   231,   235,   237,   236,    61,   105,
     103,   229,   183,   184,   128,   186,   108,   187,   188,   189,
     190,   141,   142,   229,   229,   143,   243,   109,   144,   293,
     294,   295,   307,   141,   142,   198,   199,   143,   146,   147,
     144,   112,   148,   149,     6,    62,   132,     7,   133,   134,
     146,   147,   211,   151,   148,   149,    76,   124,   125,    78,
      79,    80,   111,   316,   212,   213,   318,   249,   249,   249,
     249,   249,   249,   249,   249,   249,   249,   249,   249,   249,
     249,   249,   264,   265,   266,   117,   141,   142,   201,   202,
     143,   300,   131,   144,     6,    62,   132,     7,   133,   134,
      27,    28,    29,   146,   147,   207,   208,   148,   149,   231,
     231,   119,   231,   135,   126,   136,   137,   138,   139,   140,
       6,    62,   132,     7,   133,   134,   209,   210,   -19,    39,
     175,   241,   242,    40,   231,   173,   141,   142,   -82,   174,
     143,    23,    24,   144,   176,   112,   145,     1,     2,     3,
       4,   -31,    53,   146,   147,   177,    46,   148,   149,    42,
      48,    55,   141,   142,    23,    24,   143,   302,    81,   144,
     178,    83,    84,    85,    86,    87,    52,    57,   180,   146,
     147,   194,    42,   148,   149,    42,    42,    42,    48,   245,
     246,    48,    48,    48,    48,    48,    55,   195,    55,    55,
      55,    43,    49,    56,    52,   251,   252,    52,    52,    52,
      52,    52,    57,   196,    57,    57,    57,    50,    88,   197,
      89,    90,    91,   200,    43,    51,   226,    43,    43,    43,
      49,   254,   255,    49,    49,    49,    49,    49,    56,   227,
      56,    56,    56,   247,   239,    50,   260,   261,    50,    50,
      50,    50,    50,    51,   262,   263,    51,    51,    51,    51,
      51,   240,   267,   268,   269,   270,   271,   272,   273,   274,
     -14,    21,   275,   -52,   -52,    22,   -52,   256,   257,   258,
     259,   277,   278,    23,    24,    25,    26,    27,    28,    29,
     -52,   -26,    45,   280,   -52,   -52,    46,   -52,     6,    62,
     281,     7,    58,   282,    23,    24,    25,   283,    27,    28,
      29,   -52,    67,    68,    69,    70,    71,    72,    73,   286,
     214,   215,   216,   217,   218,   219,   220,   284,   242,   288,
     221,   291,   289,   290,   304,   305,   306,   308,   309,   310,
     312,   313,   314,   127,   311,   248,   317,   250,   253
};

static const yytype_uint16 yycheck[] =
{
      24,   102,   103,   176,   177,   178,   192,   180,     5,   125,
     126,    25,    52,    53,     3,     4,     5,     6,     7,     8,
     121,    25,   123,     0,   140,     5,    40,   143,     3,     4,
       3,     6,    46,     6,    49,    59,    49,   138,     3,     4,
       5,     6,     7,     8,    60,     5,    21,    52,    53,    65,
      54,    75,    49,    77,    47,    68,    45,    46,    47,   175,
      49,     3,     4,    52,     6,    31,    82,     3,     4,    49,
       6,    68,    31,    62,    63,    47,   192,    66,    67,    54,
      45,    46,    45,    46,    49,    50,   110,    52,    68,    49,
      47,   277,    48,    47,   118,   119,   120,    62,    63,   123,
      56,    66,    67,   289,   290,    68,    69,    95,    68,   225,
     283,   284,    54,   286,    50,     3,     4,    49,     6,   107,
       3,     4,    20,     6,    34,    35,   242,     5,     6,     3,
       4,     5,     6,     7,     8,   308,     1,     2,     3,     4,
      47,     3,     4,     5,     6,     7,     8,     3,    13,    14,
      60,    61,   176,   177,   178,    22,   180,    24,    23,    47,
      50,   277,   141,   142,    47,   144,    50,   146,   147,   148,
     149,    45,    46,   289,   290,    49,    50,     5,    52,   280,
     281,   282,   298,    45,    46,    57,    58,    49,    62,    63,
      52,    54,    66,    67,     3,     4,     5,     6,     7,     8,
      62,    63,    52,   304,    66,    67,    39,    47,    48,    42,
      43,    44,    47,   314,    64,    65,   317,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,    47,    45,    46,    29,    30,
      49,    50,     1,    52,     3,     4,     5,     6,     7,     8,
      17,    18,    19,    62,    63,    36,    37,    66,    67,   283,
     284,    51,   286,    22,    48,    24,    25,    26,    27,    28,
       3,     4,     5,     6,     7,     8,    62,    63,     0,     1,
      49,    47,    48,     5,   308,    47,    45,    46,    47,    47,
      49,    13,    14,    52,    49,    54,    55,     9,    10,    11,
      12,     0,     1,    62,    63,    49,     5,    66,    67,    16,
      17,    18,    45,    46,    13,    14,    49,    50,    45,    52,
      49,    48,    49,    50,    51,    52,    17,    18,    49,    62,
      63,    47,    39,    66,    67,    42,    43,    44,    45,     5,
       6,    48,    49,    50,    51,    52,    53,    53,    55,    56,
      57,    16,    17,    18,    45,   198,   199,    48,    49,    50,
      51,    52,    53,    32,    55,    56,    57,    17,    53,    33,
      55,    56,    57,    59,    39,    17,    47,    42,    43,    44,
      45,   201,   202,    48,    49,    50,    51,    52,    53,    47,
      55,    56,    57,   195,    47,    45,   207,   208,    48,    49,
      50,    51,    52,    45,   209,   210,    48,    49,    50,    51,
      52,    50,   214,   215,   216,   217,   218,   219,   220,   221,
       0,     1,     5,     3,     4,     5,     6,   203,   204,   205,
     206,    51,    50,    13,    14,    15,    16,    17,    18,    19,
      20,     0,     1,    50,     3,     4,     5,     6,     3,     4,
      50,     6,    21,    50,    13,    14,    15,    49,    17,    18,
      19,    20,    31,    32,    33,    34,    35,    36,    37,    47,
      38,    39,    40,    41,    42,    43,    44,    49,    48,    50,
      48,    70,    49,    49,    23,    50,    50,    47,    50,    50,
      47,    47,    50,   119,   304,   196,    50,   197,   200
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     9,    10,    11,    12,    72,     3,     6,    90,    90,
      90,    90,     0,    31,    31,    47,    47,    47,    47,    90,
      90,     1,     5,    13,    14,    15,    16,    17,    18,    19,
      73,    77,    78,    79,    80,    81,    82,    83,    87,     1,
       5,    74,    77,    78,    84,     1,     5,    75,    77,    78,
      79,    80,    82,     1,    76,    77,    78,    82,    73,    49,
      85,    90,     4,    89,    90,    85,    49,    73,    73,    73,
      73,    73,    73,    73,    20,    88,    74,    85,    74,    74,
      74,    75,    85,    75,    75,    75,    75,    75,    76,    76,
      76,    76,    50,    86,    89,    87,    47,    52,    53,     3,
      52,    53,    87,    50,    89,    47,    89,    87,    50,     5,
      88,    47,    54,    91,    91,     5,     6,    47,    88,    51,
      21,    89,    91,    92,    47,    48,    48,    86,    47,    89,
      91,     1,     5,     7,     8,    22,    24,    25,    26,    27,
      28,    45,    46,    49,    52,    55,    62,    63,    66,    67,
      89,    91,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
      89,    96,    96,    47,    47,    49,    49,    49,    49,    91,
      49,    47,    96,   108,   108,    96,   108,   108,   108,   108,
     108,     5,    49,    68,    47,    53,    32,    33,    57,    58,
      59,    29,    30,    34,    35,    60,    61,    36,    37,    62,
      63,    52,    64,    65,    38,    39,    40,    41,    42,    43,
      44,    48,    45,    46,    68,    69,    47,    47,    50,    96,
     111,    89,    94,    94,    94,    22,    24,    89,    94,    47,
      50,    47,    48,    50,   111,     5,     6,    98,   100,   108,
     101,   102,   102,   103,   104,   104,   105,   105,   105,   105,
     106,   106,   107,   107,   108,   108,   108,    98,    98,    98,
      98,    98,    98,    98,    98,     5,    96,    51,    50,     5,
      50,    50,    50,    49,    49,     5,    47,    96,    50,    49,
      49,    70,   111,    91,    91,    91,    94,    94,    56,    94,
      50,   111,    50,   111,    23,    50,    50,    96,    47,    50,
      50,    95,    47,    47,    50,    94,    91,    50,    91
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (parser, scanner, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, scanner)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, parser, scanner); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, parser_t *parser, void *scanner)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, parser, scanner)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    parser_t *parser;
    void *scanner;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (parser);
  YYUSE (scanner);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, parser_t *parser, void *scanner)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, parser, scanner)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    parser_t *parser;
    void *scanner;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, parser, scanner);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule, parser_t *parser, void *scanner)
#else
static void
yy_reduce_print (yyvsp, yyrule, parser, scanner)
    YYSTYPE *yyvsp;
    int yyrule;
    parser_t *parser;
    void *scanner;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       , parser, scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule, parser, scanner); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
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



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, parser_t *parser, void *scanner)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, parser, scanner)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    parser_t *parser;
    void *scanner;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (parser);
  YYUSE (scanner);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {
      case 3: /* "TOK_TYPE" */

/* Line 1000 of yacc.c  */
#line 39 "grammar.y"
	{ type_free((yyvaluep->type)); (yyvaluep->type) = 0; };

/* Line 1000 of yacc.c  */
#line 1469 "grammar.y.c"
	break;
      case 4: /* "TOK_PRIMITIVE" */

/* Line 1000 of yacc.c  */
#line 39 "grammar.y"
	{ type_free((yyvaluep->type)); (yyvaluep->type) = 0; };

/* Line 1000 of yacc.c  */
#line 1478 "grammar.y.c"
	break;
      case 5: /* "TOK_IDENT" */

/* Line 1000 of yacc.c  */
#line 43 "grammar.y"
	{ free((yyvaluep->string)); };

/* Line 1000 of yacc.c  */
#line 1487 "grammar.y.c"
	break;
      case 6: /* "TOK_CONST" */

/* Line 1000 of yacc.c  */
#line 43 "grammar.y"
	{ free((yyvaluep->string)); };

/* Line 1000 of yacc.c  */
#line 1496 "grammar.y.c"
	break;
      case 7: /* "TOK_STRING" */

/* Line 1000 of yacc.c  */
#line 37 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1505 "grammar.y.c"
	break;
      case 8: /* "TOK_NUMBER" */

/* Line 1000 of yacc.c  */
#line 37 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1514 "grammar.y.c"
	break;
      case 73: /* "class" */

/* Line 1000 of yacc.c  */
#line 40 "grammar.y"
	{ unit_free((yyvaluep->unit)); (yyvaluep->unit) = 0; };

/* Line 1000 of yacc.c  */
#line 1523 "grammar.y.c"
	break;
      case 74: /* "interface" */

/* Line 1000 of yacc.c  */
#line 40 "grammar.y"
	{ unit_free((yyvaluep->unit)); (yyvaluep->unit) = 0; };

/* Line 1000 of yacc.c  */
#line 1532 "grammar.y.c"
	break;
      case 75: /* "struct" */

/* Line 1000 of yacc.c  */
#line 40 "grammar.y"
	{ unit_free((yyvaluep->unit)); (yyvaluep->unit) = 0; };

/* Line 1000 of yacc.c  */
#line 1541 "grammar.y.c"
	break;
      case 76: /* "module" */

/* Line 1000 of yacc.c  */
#line 40 "grammar.y"
	{ unit_free((yyvaluep->unit)); (yyvaluep->unit) = 0; };

/* Line 1000 of yacc.c  */
#line 1550 "grammar.y.c"
	break;
      case 79: /* "variable" */

/* Line 1000 of yacc.c  */
#line 41 "grammar.y"
	{ var_free((yyvaluep->var)); (yyvaluep->var) = 0; };

/* Line 1000 of yacc.c  */
#line 1559 "grammar.y.c"
	break;
      case 80: /* "constructor" */

/* Line 1000 of yacc.c  */
#line 42 "grammar.y"
	{ func_free((yyvaluep->func)); (yyvaluep->func) = 0; };

/* Line 1000 of yacc.c  */
#line 1568 "grammar.y.c"
	break;
      case 81: /* "destructor" */

/* Line 1000 of yacc.c  */
#line 42 "grammar.y"
	{ func_free((yyvaluep->func)); (yyvaluep->func) = 0; };

/* Line 1000 of yacc.c  */
#line 1577 "grammar.y.c"
	break;
      case 82: /* "function" */

/* Line 1000 of yacc.c  */
#line 42 "grammar.y"
	{ func_free((yyvaluep->func)); (yyvaluep->func) = 0; };

/* Line 1000 of yacc.c  */
#line 1586 "grammar.y.c"
	break;
      case 83: /* "native" */

/* Line 1000 of yacc.c  */
#line 42 "grammar.y"
	{ func_free((yyvaluep->func)); (yyvaluep->func) = 0; };

/* Line 1000 of yacc.c  */
#line 1595 "grammar.y.c"
	break;
      case 84: /* "prototype" */

/* Line 1000 of yacc.c  */
#line 42 "grammar.y"
	{ func_free((yyvaluep->func)); (yyvaluep->func) = 0; };

/* Line 1000 of yacc.c  */
#line 1604 "grammar.y.c"
	break;
      case 85: /* "parameter_signature" */

/* Line 1000 of yacc.c  */
#line 41 "grammar.y"
	{ var_free((yyvaluep->var)); (yyvaluep->var) = 0; };

/* Line 1000 of yacc.c  */
#line 1613 "grammar.y.c"
	break;
      case 86: /* "parameter_list" */

/* Line 1000 of yacc.c  */
#line 41 "grammar.y"
	{ var_free((yyvaluep->var)); (yyvaluep->var) = 0; };

/* Line 1000 of yacc.c  */
#line 1622 "grammar.y.c"
	break;
      case 89: /* "type" */

/* Line 1000 of yacc.c  */
#line 39 "grammar.y"
	{ type_free((yyvaluep->type)); (yyvaluep->type) = 0; };

/* Line 1000 of yacc.c  */
#line 1631 "grammar.y.c"
	break;
      case 90: /* "qualified_name" */

/* Line 1000 of yacc.c  */
#line 39 "grammar.y"
	{ type_free((yyvaluep->type)); (yyvaluep->type) = 0; };

/* Line 1000 of yacc.c  */
#line 1640 "grammar.y.c"
	break;
      case 91: /* "block" */

/* Line 1000 of yacc.c  */
#line 38 "grammar.y"
	{ stmt_free((yyvaluep->stmt)); (yyvaluep->stmt) = 0; };

/* Line 1000 of yacc.c  */
#line 1649 "grammar.y.c"
	break;
      case 92: /* "statement_list" */

/* Line 1000 of yacc.c  */
#line 38 "grammar.y"
	{ stmt_free((yyvaluep->stmt)); (yyvaluep->stmt) = 0; };

/* Line 1000 of yacc.c  */
#line 1658 "grammar.y.c"
	break;
      case 93: /* "statement" */

/* Line 1000 of yacc.c  */
#line 38 "grammar.y"
	{ stmt_free((yyvaluep->stmt)); (yyvaluep->stmt) = 0; };

/* Line 1000 of yacc.c  */
#line 1667 "grammar.y.c"
	break;
      case 94: /* "clause" */

/* Line 1000 of yacc.c  */
#line 38 "grammar.y"
	{ stmt_free((yyvaluep->stmt)); (yyvaluep->stmt) = 0; };

/* Line 1000 of yacc.c  */
#line 1676 "grammar.y.c"
	break;
      case 95: /* "conditional" */

/* Line 1000 of yacc.c  */
#line 38 "grammar.y"
	{ stmt_free((yyvaluep->stmt)); (yyvaluep->stmt) = 0; };

/* Line 1000 of yacc.c  */
#line 1685 "grammar.y.c"
	break;
      case 96: /* "expression" */

/* Line 1000 of yacc.c  */
#line 37 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1694 "grammar.y.c"
	break;
      case 97: /* "nullable" */

/* Line 1000 of yacc.c  */
#line 37 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1703 "grammar.y.c"
	break;
      case 98: /* "assignment" */

/* Line 1000 of yacc.c  */
#line 37 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1712 "grammar.y.c"
	break;
      case 99: /* "logical_or" */

/* Line 1000 of yacc.c  */
#line 37 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1721 "grammar.y.c"
	break;
      case 100: /* "logical_and" */

/* Line 1000 of yacc.c  */
#line 37 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1730 "grammar.y.c"
	break;
      case 101: /* "bitwise_or" */

/* Line 1000 of yacc.c  */
#line 37 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1739 "grammar.y.c"
	break;
      case 102: /* "bitwise_and" */

/* Line 1000 of yacc.c  */
#line 37 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1748 "grammar.y.c"
	break;
      case 103: /* "equality" */

/* Line 1000 of yacc.c  */
#line 37 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1757 "grammar.y.c"
	break;
      case 104: /* "relation" */

/* Line 1000 of yacc.c  */
#line 37 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1766 "grammar.y.c"
	break;
      case 105: /* "shift" */

/* Line 1000 of yacc.c  */
#line 37 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1775 "grammar.y.c"
	break;
      case 106: /* "addition" */

/* Line 1000 of yacc.c  */
#line 37 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1784 "grammar.y.c"
	break;
      case 107: /* "multiplication" */

/* Line 1000 of yacc.c  */
#line 37 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1793 "grammar.y.c"
	break;
      case 108: /* "unary" */

/* Line 1000 of yacc.c  */
#line 37 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1802 "grammar.y.c"
	break;
      case 109: /* "postfix" */

/* Line 1000 of yacc.c  */
#line 37 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1811 "grammar.y.c"
	break;
      case 110: /* "primary" */

/* Line 1000 of yacc.c  */
#line 37 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1820 "grammar.y.c"
	break;
      case 111: /* "argument_list" */

/* Line 1000 of yacc.c  */
#line 37 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1829 "grammar.y.c"
	break;

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (parser_t *parser, void *scanner);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */





/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (parser_t *parser, void *scanner)
#else
int
yyparse (parser, scanner)
    parser_t *parser;
    void *scanner;
#endif
#endif
{
/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 136 "grammar.y"
    {
        unit_name((yyvsp[(4) - (4)].unit), (yyvsp[(2) - (4)].type)); 
		parser_class(parser, (yyvsp[(4) - (4)].unit));
    ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 140 "grammar.y"
    {
        unit_name((yyvsp[(4) - (4)].unit), (yyvsp[(2) - (4)].type)); 
		parser_interface(parser, (yyvsp[(4) - (4)].unit));
    ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 144 "grammar.y"
    {
        unit_name((yyvsp[(4) - (4)].unit), (yyvsp[(2) - (4)].type)); 
		parser_struct(parser, (yyvsp[(4) - (4)].unit));
    ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 148 "grammar.y"
    {
        unit_name((yyvsp[(4) - (4)].unit), (yyvsp[(2) - (4)].type));
		parser_module(parser, (yyvsp[(4) - (4)].unit));
    ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 155 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_import((yyval.unit), (yyvsp[(1) - (2)].import)); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 156 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_def((yyval.unit), (yyvsp[(1) - (2)].def)); ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 157 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_var((yyval.unit), (yyvsp[(1) - (2)].var)); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 158 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_ctor((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 159 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_dtor((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 160 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_func((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 161 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_func((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 162 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 163 "grammar.y"
    { (yyval.unit) = unit_alloc(UNIT_TYPE_CLASS); ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 167 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_import((yyval.unit), (yyvsp[(1) - (2)].import)); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 168 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_def((yyval.unit), (yyvsp[(1) - (2)].def)); ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 169 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_func((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 170 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 171 "grammar.y"
    { (yyval.unit) = unit_alloc(UNIT_TYPE_INTERFACE); ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 175 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_import((yyval.unit), (yyvsp[(1) - (2)].import)); ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 176 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_def((yyval.unit), (yyvsp[(1) - (2)].def)); ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 177 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_var((yyval.unit), (yyvsp[(1) - (2)].var)); ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 178 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_ctor((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 179 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_func((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 180 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 181 "grammar.y"
    { (yyval.unit) = unit_alloc(UNIT_TYPE_STRUCT); ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 185 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_import((yyval.unit), (yyvsp[(1) - (2)].import)); ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 186 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_def((yyval.unit), (yyvsp[(1) - (2)].def)); ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 187 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_func((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 188 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 189 "grammar.y"
    { (yyval.unit) = unit_alloc(UNIT_TYPE_MODULE); ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 193 "grammar.y"
    { (yyval.import) = import_alloc((yyvsp[(2) - (3)].type)); ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 197 "grammar.y"
    { (yyval.def) = def_alloc((yyvsp[(2) - (4)].type), (yyvsp[(3) - (4)].type)); ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 201 "grammar.y"
    {
		// TODO: Set symbol table for class-level
		(yyval.var) = var_alloc((yyvsp[(4) - (7)].string), (yyvsp[(1) - (7)].flag)|(yyvsp[(2) - (7)].flag), (yyvsp[(3) - (7)].type), (yyvsp[(6) - (7)].expr));
    ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 205 "grammar.y"
    {
		(yyval.var) = var_alloc((yyvsp[(4) - (5)].string), (yyvsp[(1) - (5)].flag)|(yyvsp[(2) - (5)].flag), (yyvsp[(3) - (5)].type), 0);
    ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 208 "grammar.y"
    {
		(yyval.var) = var_alloc((yyvsp[(4) - (7)].string), (yyvsp[(1) - (7)].flag)|(yyvsp[(2) - (7)].flag)|UNIT_FLAG_CONST, (yyvsp[(3) - (7)].type), (yyvsp[(6) - (7)].expr));
	;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 214 "grammar.y"
    { 
		(yyval.func) = func_alloc(strdup("@init"), (yyvsp[(2) - (4)].var), 0, (yyvsp[(4) - (4)].stmt));
		(yyval.func)->flags = (yyvsp[(3) - (4)].flag);
	;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 221 "grammar.y"
    { 
		(yyval.func) = func_alloc(strdup("@destroy"), 0, 0, (yyvsp[(4) - (4)].stmt)); 
	;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 227 "grammar.y"
    {
		(yyval.func) = func_alloc((yyvsp[(1) - (6)].string), (yyvsp[(2) - (6)].var), (yyvsp[(5) - (6)].type), (yyvsp[(6) - (6)].stmt));
		(yyval.func)->flags = (yyvsp[(3) - (6)].flag)|(yyvsp[(4) - (6)].flag);
    ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 231 "grammar.y"
    {
		(yyval.func) = func_alloc((yyvsp[(1) - (5)].string), (yyvsp[(2) - (5)].var), 0, (yyvsp[(5) - (5)].stmt));
		(yyval.func)->flags = (yyvsp[(3) - (5)].flag)|(yyvsp[(4) - (5)].flag);
	;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 238 "grammar.y"
    {
		(yyval.func) = func_alloc((yyvsp[(1) - (7)].string), (yyvsp[(2) - (7)].var), (yyvsp[(6) - (7)].type), 0); 
		(yyval.func)->flags = (yyvsp[(3) - (7)].flag)|(yyvsp[(4) - (7)].flag)|UNIT_FLAG_NATIVE;
	;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 242 "grammar.y"
    {
		(yyval.func) = func_alloc((yyvsp[(1) - (6)].string), (yyvsp[(2) - (6)].var), 0, 0);
		(yyval.func)->flags = (yyvsp[(3) - (6)].flag)|(yyvsp[(4) - (6)].flag)|UNIT_FLAG_NATIVE;
	;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 249 "grammar.y"
    {
		(yyval.func) = func_alloc((yyvsp[(1) - (4)].string), (yyvsp[(2) - (4)].var), (yyvsp[(3) - (4)].type), 0);
    ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 252 "grammar.y"
    {
		(yyval.func) = func_alloc((yyvsp[(1) - (3)].string), (yyvsp[(2) - (3)].var), 0, 0);
	;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 258 "grammar.y"
    { (yyval.var) = (yyvsp[(2) - (3)].var); ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 259 "grammar.y"
    { (yyval.var) = 0; ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 263 "grammar.y"
    { 
		(yyval.var) = (yyvsp[(4) - (4)].var);
		(yyval.var)->next = var_alloc((yyvsp[(2) - (4)].string), 0, (yyvsp[(1) - (4)].type), 0); 
	;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 267 "grammar.y"
    { 
		(yyval.var) = var_alloc((yyvsp[(2) - (2)].string), 0, (yyvsp[(1) - (2)].type), 0);
	;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 273 "grammar.y"
    { (yyval.flag) = UNIT_FLAG_PUBLIC; ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 274 "grammar.y"
    { (yyval.flag) = UNIT_FLAG_PRIVATE; ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 275 "grammar.y"
    { (yyval.flag) = UNIT_FLAG_PROTECTED; ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 276 "grammar.y"
    { (yyval.flag) = 0; ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 280 "grammar.y"
    { (yyval.flag) = UNIT_FLAG_STATIC; ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 281 "grammar.y"
    { (yyval.flag) = 0; ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 285 "grammar.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 286 "grammar.y"
    { (yyval.type) = (yyvsp[(1) - (2)].type); /* TODO: Range */ (yyval.type)->pointer = 1; ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 287 "grammar.y"
    { (yyval.type) = (yyvsp[(1) - (2)].type); /* TODO: Nullable */ ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 288 "grammar.y"
    { 
		parser_resolve_type((yyval.type) = (yyvsp[(1) - (1)].type));
	;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 291 "grammar.y"
    { 
		parser_resolve_type((yyval.type) = (yyvsp[(1) - (2)].type));
		(yyval.type)->pointer = 1;
	;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 295 "grammar.y"
    { 
		parser_resolve_type((yyval.type) = (yyvsp[(1) - (2)].type));
		(yyval.type)->pointer = 1; /* TODO: Nullable */
	;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 302 "grammar.y"
    { (yyval.type) = type_concat((yyvsp[(1) - (3)].type), (yyvsp[(3) - (3)].type)); ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 303 "grammar.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 304 "grammar.y"
    { 
		(yyval.type) = type_concat(type_object((yyvsp[(1) - (3)].string)), (yyvsp[(3) - (3)].type)); 
	;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 307 "grammar.y"
    { 
		(yyval.type) = type_object((yyvsp[(1) - (1)].string)); 
	;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 313 "grammar.y"
    { 
		(yyval.stmt) = (yyvsp[(2) - (3)].stmt); 
		parser->symbols = (yyval.stmt)->symbols ? symtab_get_parent((yyval.stmt)->symbols) : 0;
	;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 320 "grammar.y"
    { (yyval.stmt) = stmt_append((yyvsp[(1) - (2)].stmt), (yyvsp[(2) - (2)].stmt)); ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 321 "grammar.y"
    { (yyval.stmt) = (yyvsp[(1) - (3)].stmt); ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 322 "grammar.y"
    { 
		(yyval.stmt) = stmt_block(parser->symbols);
		parser->symbols = (yyval.stmt)->symbols;
	;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 329 "grammar.y"
    {
		(yyval.stmt) = stmt_for((yyvsp[(3) - (9)].stmt), (yyvsp[(5) - (9)].stmt), (yyvsp[(7) - (9)].stmt), (yyvsp[(9) - (9)].stmt));
	;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 332 "grammar.y"
    {
		(yyval.stmt) = stmt_foreach(var_alloc((yyvsp[(4) - (8)].string), 0, (yyvsp[(3) - (8)].type), (yyvsp[(6) - (8)].expr)), (yyvsp[(8) - (8)].stmt));
	;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 335 "grammar.y"
    {
		(yyval.stmt) = stmt_until((yyvsp[(3) - (5)].stmt), (yyvsp[(5) - (5)].stmt));
	;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 338 "grammar.y"
    {
		(yyval.stmt) = stmt_while((yyvsp[(3) - (5)].stmt), (yyvsp[(5) - (5)].stmt));
	;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 341 "grammar.y"
    {
		(yyval.stmt) = stmt_dountil((yyvsp[(2) - (7)].stmt), (yyvsp[(5) - (7)].stmt));
	;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 344 "grammar.y"
    {
		(yyval.stmt) = stmt_dowhile((yyvsp[(2) - (7)].stmt), (yyvsp[(5) - (7)].stmt));
	;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 347 "grammar.y"
    { 
		(yyval.stmt) = stmt_decl(parser, var_alloc((yyvsp[(2) - (3)].string), 0, (yyvsp[(1) - (3)].type), 0)); 
	;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 350 "grammar.y"
    { (yyval.stmt) = (yyvsp[(1) - (1)].stmt); ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 351 "grammar.y"
    { (yyval.stmt) = (yyvsp[(1) - (2)].stmt); ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 352 "grammar.y"
    { (yyval.stmt) = stmt_return((yyvsp[(2) - (3)].expr)); ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 353 "grammar.y"
    { (yyval.stmt) = stmt_return(0); ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 357 "grammar.y"
    { 
		(yyval.stmt) = stmt_decl(parser, var_alloc((yyvsp[(2) - (4)].string), 0, (yyvsp[(1) - (4)].type), (yyvsp[(4) - (4)].expr))); 
	;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 360 "grammar.y"
    { 
		(yyval.stmt) = stmt_expr((yyvsp[(1) - (1)].expr)); 
	;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 363 "grammar.y"
    { (yyval.stmt) = 0; ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 367 "grammar.y"
    {
		(yyval.stmt) = stmt_conditional((yyvsp[(3) - (5)].stmt), (yyvsp[(5) - (5)].stmt), 0);
	;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 370 "grammar.y"
    {
		(yyval.stmt) = stmt_conditional((yyvsp[(3) - (7)].stmt), (yyvsp[(5) - (7)].stmt), (yyvsp[(7) - (7)].stmt));
	;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 373 "grammar.y"
    { (yyval.stmt) = (yyvsp[(1) - (1)].stmt); ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 376 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 379 "grammar.y"
    { 
		(yyval.expr) = expr_binary(parser, strdup("?"), (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
	;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 386 "grammar.y"
    { 
		(yyval.expr) = expr_binary(parser, strdup("="), (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 389 "grammar.y"
    { 
		(yyval.expr) = expr_binary(parser, strdup("*="), (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 392 "grammar.y"
    { 
		(yyval.expr) = expr_binary(parser, strdup("/="), (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 395 "grammar.y"
    { 
		(yyval.expr) = expr_binary(parser, strdup("%="), (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 398 "grammar.y"
    { 
		(yyval.expr) = expr_binary(parser, strdup("-="), (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 401 "grammar.y"
    { 
		(yyval.expr) = expr_binary(parser, strdup("+="), (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 404 "grammar.y"
    { 
		(yyval.expr) = expr_binary(parser, strdup("&="), (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 407 "grammar.y"
    { 
		(yyval.expr) = expr_binary(parser, strdup("|="), (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 410 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 414 "grammar.y"
    { 
		(yyval.expr) = expr_binary(parser, strdup("||"), (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 417 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 421 "grammar.y"
    { 
		(yyval.expr) = expr_binary(parser, strdup("&&"), (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 424 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 428 "grammar.y"
    { 
		(yyval.expr) = expr_binary(parser, strdup("|"), (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 431 "grammar.y"
    { 
		(yyval.expr) = expr_binary(parser, strdup("^"), (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 434 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 438 "grammar.y"
    { 
		(yyval.expr) = expr_binary(parser, strdup("&"), (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 441 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 445 "grammar.y"
    { 
		(yyval.expr) = expr_binary(parser, strdup("=="), (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 448 "grammar.y"
    { 
		(yyval.expr) = expr_binary(parser, strdup("!="), (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 451 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 455 "grammar.y"
    { 
		(yyval.expr) = expr_binary(parser, strdup(">"), (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 458 "grammar.y"
    { 
		(yyval.expr) = expr_binary(parser, strdup("<"), (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 461 "grammar.y"
    { 
		(yyval.expr) = expr_binary(parser, strdup(">="), (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 464 "grammar.y"
    { 
		(yyval.expr) = expr_binary(parser, strdup("<="), (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 467 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 471 "grammar.y"
    { 
		(yyval.expr) = expr_binary(parser, strdup("<<"), (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 474 "grammar.y"
    { 
		(yyval.expr) = expr_binary(parser, strdup(">>"), (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 477 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 481 "grammar.y"
    { 
		(yyval.expr) = expr_binary(parser, strdup("+"), (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 484 "grammar.y"
    { 
		(yyval.expr) = expr_binary(parser, strdup("-"), (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 487 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 491 "grammar.y"
    { 
		(yyval.expr) = expr_binary(parser, strdup("*"), (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 494 "grammar.y"
    { 
		(yyval.expr) = expr_binary(parser, strdup("/"), (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 497 "grammar.y"
    { 
		(yyval.expr) = expr_binary(parser, strdup("%"), (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 500 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 504 "grammar.y"
    { (yyval.expr) = expr_unary(parser, strdup("++"), (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 505 "grammar.y"
    { (yyval.expr) = expr_unary(parser, strdup("--"), (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 506 "grammar.y"
    { (yyval.expr) = expr_unary(parser, strdup("+"), (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 507 "grammar.y"
    { (yyval.expr) = expr_unary(parser, strdup("-"), (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 508 "grammar.y"
    { (yyval.expr) = expr_unary(parser, strdup("!"), (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 509 "grammar.y"
    { (yyval.expr) = expr_unary(parser, strdup("~"), (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 510 "grammar.y"
    { (yyval.expr) = expr_unary(parser, strdup("*"), (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 511 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 515 "grammar.y"
    {
		/* Call on an object expression */
		(yyval.expr) = expr_mcall(parser, (yyvsp[(1) - (6)].expr), (yyvsp[(3) - (6)].string), (yyvsp[(5) - (6)].expr));
	;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 519 "grammar.y"
    {
		/* Call on an object expression */
		(yyval.expr) = expr_mcall(parser, (yyvsp[(1) - (5)].expr), (yyvsp[(3) - (5)].string), 0);
	;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 523 "grammar.y"
    { 
		/* Member variable access */
		(yyval.expr) = expr_member(parser, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].string)); 
	;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 527 "grammar.y"
    { 
		(yyval.expr) = expr_index(parser, (yyvsp[(1) - (4)].expr), (yyvsp[(3) - (4)].expr)); 
	;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 530 "grammar.y"
    { 
		(yyval.expr) = expr_unary(parser, strdup("++"), (yyvsp[(1) - (2)].expr)); 
	;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 533 "grammar.y"
    { 
		(yyval.expr) = expr_unary(parser, strdup("--"), (yyvsp[(1) - (2)].expr)); 
	;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 536 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 540 "grammar.y"
    { 
		/* Free call, maybe with implicit "self" */
		(yyval.expr) = expr_call(parser, (yyvsp[(1) - (4)].string), (yyvsp[(3) - (4)].expr)); 
	;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 544 "grammar.y"
    { 
		/* Free call, maybe with implicit "self" */
		(yyval.expr) = expr_call(parser, (yyvsp[(1) - (3)].string), 0); 
	;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 548 "grammar.y"
    { 
		/* Free variable access, maybe with implicit "self" */
		(yyval.expr) = expr_var(parser, (yyvsp[(1) - (1)].string)); 
	;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 552 "grammar.y"
    {
		/* Static function call */
		(yyval.expr) = expr_scall(parser, (yyvsp[(1) - (6)].type), (yyvsp[(3) - (6)].string), (yyvsp[(5) - (6)].expr)); 
	;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 556 "grammar.y"
    {
		/* Static function call */
		(yyval.expr) = expr_scall(parser, (yyvsp[(1) - (5)].type), (yyvsp[(3) - (5)].string), 0);
	;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 560 "grammar.y"
    { 
		/* Static varable access */
		(yyval.expr) = expr_static(parser, (yyvsp[(1) - (3)].type), (yyvsp[(3) - (3)].string)); 
	;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 564 "grammar.y"
    { 
		/* Static constant access */
		(yyval.expr) = expr_static(parser, (yyvsp[(1) - (3)].type), (yyvsp[(3) - (3)].string)); 
	;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 568 "grammar.y"
    { 
		/* Constructor with arguments */
		(yyval.expr) = expr_ctor(parser, (yyvsp[(1) - (4)].type), (yyvsp[(3) - (4)].expr)); 
	;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 572 "grammar.y"
    { 
		/* Constructor without arguments */
		(yyval.expr) = expr_ctor(parser, (yyvsp[(1) - (3)].type), 0); 
	;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 576 "grammar.y"
    { (yyval.expr) = (yyvsp[(2) - (3)].expr); ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 577 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 578 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 582 "grammar.y"
    { 
		(yyval.expr) = (yyvsp[(3) - (3)].expr);
		(yyval.expr)->next = (yyvsp[(1) - (3)].expr);
	;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 586 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;



/* Line 1455 of yacc.c  */
#line 3375 "grammar.y.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (parser, scanner, YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (parser, scanner, yymsg);
	  }
	else
	  {
	    yyerror (parser, scanner, YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
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
		      yytoken, &yylval, parser, scanner);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
		  yystos[yystate], yyvsp, parser, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (parser, scanner, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, parser, scanner);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, parser, scanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



