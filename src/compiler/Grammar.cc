
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
#define YYLSP_NEEDED 1



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "src/compiler/Grammar.yy"

#include "Parser.hpp"
#include "Expression.hpp"
#include "Statement.hpp"
#include "Formal.hpp"
#include "Feature.hpp"
#include "Type.hpp"
#include <iostream>
#include <cassert>

#define YYSTYPE ParseNode
#define YYLTYPE Location
#define YY_EXTRA_TYPE Parser*
#define YY_NO_INPUT
#define YYERROR_VERBOSE
int yylex(ParseNode *node, Location *loc, void *scanner);
void yyerror(Location *loc, Parser *parser, void *scanner, const char *message);



/* Line 189 of yacc.c  */
#line 94 "src/compiler/Grammar.cc"

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

/* Line 214 of yacc.c  */
#line 21 "src/compiler/Grammar.yy"
 Expression* expression; 

/* Line 214 of yacc.c  */
#line 22 "src/compiler/Grammar.yy"
 Statement* statement; 

/* Line 214 of yacc.c  */
#line 23 "src/compiler/Grammar.yy"
 Name* identifier; 

/* Line 214 of yacc.c  */
#line 24 "src/compiler/Grammar.yy"
 Feature* feature; 

/* Line 214 of yacc.c  */
#line 25 "src/compiler/Grammar.yy"
 Formal* formal; 

/* Line 214 of yacc.c  */
#line 26 "src/compiler/Grammar.yy"
 Type* type; 

/* Line 214 of yacc.c  */
#line 27 "src/compiler/Grammar.yy"
 Generic* generic; 

/* Line 214 of yacc.c  */
#line 28 "src/compiler/Grammar.yy"
 Variable* variable; 

/* Line 214 of yacc.c  */
#line 29 "src/compiler/Grammar.yy"
 int null; 

/* Line 214 of yacc.c  */
#line 30 "src/compiler/Grammar.yy"
 int flag; 


/* Line 214 of yacc.c  */
#line 222 "src/compiler/Grammar.cc"
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


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 247 "src/compiler/Grammar.cc"

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
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  14
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   871

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  75
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  110
/* YYNRULES -- Number of states.  */
#define YYNSTATES  238

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   307

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    34,     2,     2,     2,    32,    18,     2,
      71,    72,    30,    28,    73,    29,    39,    31,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      21,     9,    22,     3,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    40,     2,    74,    33,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    69,    16,    70,    35,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,     4,     5,
       6,     7,     8,    10,    11,    12,    13,    14,    15,    17,
      19,    20,    23,    24,    25,    26,    27,    36,    37,    38,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,     9,    12,    13,    17,    24,    26,
      29,    30,    32,    34,    36,    38,    40,    46,    51,    55,
      61,    66,    72,    77,    83,    88,    95,   101,   105,   108,
     112,   114,   117,   119,   120,   124,   131,   133,   138,   142,
     144,   146,   150,   153,   154,   157,   163,   167,   171,   175,
     181,   185,   188,   192,   196,   199,   202,   210,   217,   219,
     223,   229,   235,   239,   243,   247,   251,   255,   259,   263,
     267,   271,   275,   279,   283,   287,   291,   295,   299,   303,
     307,   311,   315,   319,   322,   325,   330,   334,   341,   347,
     349,   353,   355,   357,   359,   361,   365,   367,   371,   373,
     376,   381,   385,   390,   394,   401,   407,   412,   416,   419,
     421
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      76,     0,    -1,    78,    76,    -1,    77,    76,    -1,    82,
      76,    -1,    -1,    51,    90,    52,    -1,    90,    21,    90,
      69,    79,    70,    -1,     1,    -1,    80,    79,    -1,    -1,
      81,    -1,    83,    -1,    82,    -1,    85,    -1,    84,    -1,
      41,    90,     9,    97,    52,    -1,    41,     9,    97,    52,
      -1,    41,    90,    52,    -1,    41,    86,    90,    89,    93,
      -1,    41,    86,    89,    93,    -1,    43,    86,    90,    89,
      93,    -1,    43,    86,    89,    93,    -1,    41,    86,    90,
      89,    52,    -1,    41,    86,    89,    52,    -1,    41,    86,
      90,    89,    50,    52,    -1,    41,    86,    89,    50,    52,
      -1,    71,    87,    72,    -1,    71,    72,    -1,    88,    73,
      87,    -1,    88,    -1,    41,    90,    -1,    48,    -1,    -1,
      90,    64,    42,    -1,    90,    64,    42,    40,    91,    74,
      -1,    42,    -1,    42,    40,    91,    74,    -1,    92,    73,
      91,    -1,    92,    -1,    90,    -1,    69,    94,    70,    -1,
      95,    94,    -1,    -1,     1,    94,    -1,    60,    41,    66,
      97,    93,    -1,    65,    99,    93,    -1,    57,    97,    93,
      -1,    55,    97,    93,    -1,    54,    97,    69,   102,    70,
      -1,    61,    97,    52,    -1,    61,    52,    -1,    67,    97,
      52,    -1,    68,   101,    52,    -1,   100,    52,    -1,    97,
      52,    -1,    97,    40,    98,    74,     9,    97,    52,    -1,
      97,    39,    41,     9,    97,    52,    -1,    96,    -1,    58,
      97,    93,    -1,    58,    97,    93,    56,    93,    -1,    58,
      97,    93,    56,    96,    -1,    97,     3,    97,    -1,    97,
      14,    97,    -1,    97,    15,    97,    -1,    97,    17,    97,
      -1,    97,    16,    97,    -1,    97,    18,    97,    -1,    97,
      20,    97,    -1,    97,    19,    97,    -1,    97,    23,    97,
      -1,    97,    22,    97,    -1,    97,    21,    97,    -1,    97,
      24,    97,    -1,    97,    25,    97,    -1,    97,    27,    97,
      -1,    97,    26,    97,    -1,    97,    33,    97,    -1,    97,
      28,    97,    -1,    97,    29,    97,    -1,    97,    30,    97,
      -1,    97,    31,    97,    -1,    97,    32,    97,    -1,    36,
      97,    -1,    35,    97,    -1,    97,    40,    98,    74,    -1,
      97,    39,    41,    -1,    97,    39,    41,    71,    98,    72,
      -1,    97,    39,    41,    71,    72,    -1,   101,    -1,    71,
      97,    72,    -1,    44,    -1,    45,    -1,    46,    -1,    41,
      -1,    97,    73,    98,    -1,    97,    -1,   100,    73,    99,
      -1,   100,    -1,    41,    90,    -1,    41,    90,     9,    97,
      -1,    41,     9,    97,    -1,    41,    71,    98,    72,    -1,
      41,    71,    72,    -1,    90,    64,    41,    71,    98,    72,
      -1,    90,    64,    41,    71,    72,    -1,    90,    71,    98,
      72,    -1,    90,    71,    72,    -1,   103,   102,    -1,   103,
      -1,    53,    41,    90,    93,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    98,    98,   101,   104,   107,   111,   118,   122,   128,
     132,   138,   139,   140,   141,   142,   146,   150,   154,   160,
     163,   170,   173,   180,   183,   190,   193,   200,   201,   205,
     209,   215,   220,   221,   225,   228,   231,   234,   240,   244,
     250,   254,   260,   264,   267,   274,   277,   280,   286,   289,
     292,   295,   298,   301,   304,   307,   310,   323,   329,   335,
     338,   341,   346,   350,   353,   356,   360,   364,   368,   372,
     378,   383,   391,   395,   401,   408,   412,   416,   420,   424,
     428,   432,   436,   440,   443,   446,   451,   455,   459,   462,
     463,   464,   465,   466,   467,   471,   475,   480,   484,   490,
     493,   496,   502,   505,   509,   512,   515,   518,   523,   527,
     533
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "'?'", "BIT_XOR_ASSIGN", "BIT_OR_ASSIGN",
  "BIT_AND_ASSIGN", "SUBTRACT_ASSIGN", "ADD_ASSIGN", "'='", "POWER_ASSIGN",
  "MODULUS_ASSIGN", "DIVIDE_ASSIGN", "MULTIPLY_ASSIGN", "OR", "AND", "'|'",
  "XOR", "'&'", "NOT_EQUAL", "EQUAL", "'<'", "'>'", "COMPARE",
  "GREATER_OR_EQUAL", "LESS_OR_EQUAL", "RIGHT_SHIFT", "LEFT_SHIFT", "'+'",
  "'-'", "'*'", "'/'", "'%'", "'^'", "'!'", "'~'", "NOT", "DECREMENT",
  "INCREMENT", "'.'", "'['", "IDENTIFIER", "TYPE", "OPERATOR", "STRING",
  "NUMBER", "BOOLEAN", "PUBLIC", "PRIVATE", "STATIC", "NATIVE", "IMPORT",
  "SEPARATOR", "WHEN", "CASE", "WHILE", "ELSE", "UNTIL", "IF", "DO", "FOR",
  "RETURN", "RIGHT_ARROW", "LEFT_ARROW", "SCOPE", "LET", "IN", "YIELD",
  "FORK", "'{'", "'}'", "'('", "')'", "','", "']'", "$accept", "input",
  "import", "class", "feature_list", "feature", "attribute", "function",
  "operator", "prototype", "native", "formal_signature", "formal_list",
  "formal", "modifiers", "type", "generic_list", "generic", "block",
  "statement_list", "statement", "conditional", "expression",
  "expression_list", "variable_list", "variable", "call", "when_list",
  "when", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,    63,   258,   259,   260,   261,   262,    61,
     263,   264,   265,   266,   267,   268,   124,   269,    38,   270,
     271,    60,    62,   272,   273,   274,   275,   276,    43,    45,
      42,    47,    37,    94,    33,   126,   277,   278,   279,    46,
      91,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     299,   300,   301,   302,   303,   304,   305,   306,   307,   123,
     125,    40,    41,    44,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    75,    76,    76,    76,    76,    77,    78,    78,    79,
      79,    80,    80,    80,    80,    80,    81,    81,    81,    82,
      82,    83,    83,    84,    84,    85,    85,    86,    86,    87,
      87,    88,    89,    89,    90,    90,    90,    90,    91,    91,
      92,    93,    94,    94,    94,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    96,
      96,    96,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    98,    98,    99,    99,   100,
     100,   100,   101,   101,   101,   101,   101,   101,   102,   102,
     103
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     2,     0,     3,     6,     1,     2,
       0,     1,     1,     1,     1,     1,     5,     4,     3,     5,
       4,     5,     4,     5,     4,     6,     5,     3,     2,     3,
       1,     2,     1,     0,     3,     6,     1,     4,     3,     1,
       1,     3,     2,     0,     2,     5,     3,     3,     3,     5,
       3,     2,     3,     3,     2,     2,     7,     6,     1,     3,
       5,     5,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     4,     3,     6,     5,     1,
       3,     1,     1,     1,     1,     3,     1,     3,     1,     2,
       4,     3,     4,     3,     6,     5,     4,     3,     2,     1,
       4
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     8,     0,    36,     0,     0,     0,     0,     0,     0,
       0,    33,     0,     0,     1,     3,     2,     4,     0,     0,
       0,    28,     0,    30,    32,     0,    33,    40,     0,    39,
       6,     0,    34,    31,    27,     0,     0,    20,     0,    37,
       0,    10,     0,    29,     0,     0,     0,    94,    91,    92,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,    89,    19,    38,
       0,     0,     0,    10,    11,    13,    12,    15,    14,     0,
      44,    94,    84,    83,     0,     0,    99,     0,     0,     0,
       0,     0,    51,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,    41,    42,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    55,    54,
       0,    33,     0,    33,     7,     9,    35,     0,     0,   101,
     103,    96,     0,     0,     0,    48,    47,    59,     0,    50,
      46,     0,    52,    53,    90,     0,   107,     0,    62,    63,
      64,    66,    65,    67,    69,    68,    72,    71,    70,    73,
      74,    76,    75,    78,    79,    80,    81,    82,    77,    86,
       0,     0,     0,    33,     0,    18,     0,    33,    86,     0,
       0,   102,   100,     0,     0,   109,     0,     0,    97,     0,
     106,     0,     0,    85,    17,     0,    24,     0,     0,    22,
       0,    85,    95,     0,    49,   108,    60,    61,    45,   105,
       0,     0,    88,     0,     0,    26,     0,    23,    16,    21,
       0,   104,    57,    87,     0,    25,   110,    56
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,     6,     7,    72,    73,    74,     8,    76,    77,
      78,    11,    22,    23,    25,    61,    28,    29,    37,    62,
      63,    64,   141,   142,    95,    66,    67,   194,   195
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -101
static const yytype_int16 yypact[] =
{
      73,  -101,   -42,    -7,    13,    37,    73,    73,    73,    -5,
     -27,    29,    13,    32,  -101,  -101,  -101,  -101,    13,    22,
      13,  -101,    15,    35,  -101,    44,   -20,    53,    60,    75,
    -101,   -45,   106,    53,  -101,   109,    21,  -101,    44,  -101,
      13,    80,    13,  -101,    21,   -10,   -10,     1,  -101,  -101,
    -101,   -10,   -10,   -10,   -10,   117,   252,   125,   -10,    63,
     -10,   -25,    91,    21,  -101,   415,   116,  -101,  -101,  -101,
      12,   -42,   105,    80,  -101,  -101,  -101,  -101,  -101,   103,
    -101,   115,   114,   114,   -10,    65,     4,   353,   384,   384,
     384,   126,  -101,   454,     8,    44,   110,   493,   115,   130,
     322,   139,   143,  -101,  -101,   -10,   -10,   -10,   -10,   -10,
     -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,
     -10,   -10,   -10,   -10,   -10,   -10,   153,   -10,  -101,  -101,
     -10,    29,    -4,    29,  -101,  -101,  -101,   155,   -10,   688,
    -101,   246,   127,   -10,   144,  -101,  -101,   142,   -10,  -101,
    -101,   125,  -101,  -101,  -101,   129,  -101,   131,   715,   741,
     766,   789,   789,   811,   831,   831,   112,   112,   112,   112,
     112,   277,   277,   419,   419,    86,    86,    86,   114,    14,
     132,   532,    43,   -20,   -10,  -101,    44,   -20,   133,   135,
     -10,  -101,   688,   164,   169,   144,    11,   384,  -101,   166,
    -101,   -10,   211,   231,  -101,   190,  -101,    66,   571,  -101,
      44,  -101,  -101,    13,  -101,  -101,  -101,  -101,  -101,  -101,
     171,   610,  -101,   172,   -10,  -101,   193,  -101,  -101,  -101,
      34,  -101,  -101,  -101,   649,  -101,  -101,  -101
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -101,   121,  -101,  -101,   175,  -101,  -101,   -32,  -101,  -101,
    -101,   120,   215,  -101,   -11,     0,    90,  -101,   -37,   -14,
    -101,    55,   111,  -100,   107,   -54,   221,    89,  -101
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -44
static const yytype_int16 yytable[] =
{
       9,    68,   157,    96,    13,   184,     9,     9,     9,    75,
      84,    26,    27,   143,    20,    38,    18,    84,    31,    19,
      33,   130,    44,   201,    41,    45,    46,   180,    24,    10,
      80,    81,     3,    12,    48,    49,    50,    14,   189,   101,
      27,    75,    27,     3,    19,    21,   102,    86,   185,   104,
       3,   145,   146,   147,     3,     3,    45,    46,   150,    19,
      19,    60,    47,     3,    32,    48,    49,    50,    19,    54,
     132,     3,    85,    -5,     1,    51,    52,    24,    53,    54,
      36,    55,    56,    10,    30,   202,    57,    34,    58,    59,
     212,   -43,    60,   205,    86,   206,    19,    96,    19,   220,
      45,    46,   223,    36,    98,     3,    81,     3,    35,    48,
      49,    50,    36,    36,     2,     3,   226,    19,   227,   125,
     182,    70,   186,    71,     4,   137,   138,    15,    16,    17,
      69,   183,    79,   187,    39,    36,    60,   140,   118,   119,
     120,   121,   122,   123,   124,   125,    42,    65,    40,   209,
      20,   137,   138,   137,   138,    65,    82,    83,    91,   216,
     218,   103,    87,    88,    89,    90,    94,    93,   129,    97,
      68,   100,   207,   229,    65,   134,   210,   136,    45,    46,
     155,    32,   153,   151,    81,     3,    85,    48,    49,    50,
     131,   133,   148,   236,   179,   139,   188,   193,   196,   191,
     199,    45,    46,   200,   202,   213,   203,    81,     3,   211,
      48,    49,    50,   230,    60,   156,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,    60,   219,   214,
     224,   181,   225,   231,   233,   235,    45,    46,   135,   105,
      43,   217,    81,     3,   192,    48,    49,    50,   198,   197,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      99,     0,    60,   222,   215,   137,   138,    45,    46,     0,
       0,     0,     0,    81,     3,   208,    48,    49,    50,     0,
       0,     0,     0,     0,    92,   120,   121,   122,   123,   124,
     125,     0,   221,     0,     0,     0,   137,   138,     0,   190,
       0,     0,     0,    60,     0,   105,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   234,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   105,     0,     0,     0,
       0,   137,   138,     0,     0,     0,     0,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   105,     0,     0,
       0,     0,   137,   138,   154,     0,     0,     0,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   105,     0,
       0,     0,   144,   137,   138,     0,     0,     0,     0,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   122,
     123,   124,   125,    36,   126,   127,     0,   105,   137,   138,
       0,     0,     0,     0,     0,     0,     0,   128,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,     0,     0,
       0,     0,     0,   137,   138,     0,   105,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   149,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,     0,     0,     0,
       0,     0,   137,   138,     0,   105,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   152,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,     0,     0,     0,     0,
       0,   137,   138,     0,   105,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   204,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,     0,     0,     0,     0,     0,
     137,   138,     0,   105,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   228,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,     0,     0,     0,     0,     0,   137,
     138,     0,   105,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   232,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,     0,     0,     0,     0,     0,   137,   138,
       0,   105,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   237,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,     0,     0,     0,     0,     0,   137,   138,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,     0,
       0,     0,     0,     0,   137,   138,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,     0,     0,     0,     0,     0,
     137,   138,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
       0,     0,     0,     0,     0,   137,   138,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,     0,     0,     0,     0,     0,   137,   138,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,     0,     0,     0,     0,     0,
     137,   138,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,     0,     0,     0,     0,     0,
     137,   138
};

static const yytype_int16 yycheck[] =
{
       0,    38,   102,    57,     4,     9,     6,     7,     8,    41,
       9,    11,    12,     9,    41,    26,    21,     9,    18,    64,
      20,     9,     1,     9,    69,    35,    36,   127,    48,    71,
      44,    41,    42,    40,    44,    45,    46,     0,   138,    64,
      40,    73,    42,    42,    64,    72,    71,    47,    52,    63,
      42,    88,    89,    90,    42,    42,    35,    36,    95,    64,
      64,    71,    41,    42,    42,    44,    45,    46,    64,    58,
      70,    42,    71,     0,     1,    54,    55,    48,    57,    58,
      69,    60,    61,    71,    52,    71,    65,    72,    67,    68,
     190,    70,    71,    50,    94,    52,    64,   151,    64,   199,
      35,    36,   202,    69,    41,    42,    41,    42,    73,    44,
      45,    46,    69,    69,    41,    42,    50,    64,    52,    33,
     131,    41,   133,    43,    51,    39,    40,     6,     7,     8,
      40,   131,    42,   133,    74,    69,    71,    72,    26,    27,
      28,    29,    30,    31,    32,    33,    40,    36,    73,   186,
      41,    39,    40,    39,    40,    44,    45,    46,    41,   196,
     197,    70,    51,    52,    53,    54,    41,    56,    52,    58,
     207,    60,   183,   210,    63,    70,   187,    74,    35,    36,
      41,    42,    52,    73,    41,    42,    71,    44,    45,    46,
      70,    71,    66,   230,    41,    84,    41,    53,    56,    72,
      71,    35,    36,    72,    71,    41,    74,    41,    42,    74,
      44,    45,    46,   213,    71,    72,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    71,    72,    70,
       9,   130,    52,    72,    72,    52,    35,    36,    73,     3,
      35,   196,    41,    42,   143,    44,    45,    46,   151,   148,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      59,    -1,    71,    72,   195,    39,    40,    35,    36,    -1,
      -1,    -1,    -1,    41,    42,   184,    44,    45,    46,    -1,
      -1,    -1,    -1,    -1,    52,    28,    29,    30,    31,    32,
      33,    -1,   201,    -1,    -1,    -1,    39,    40,    -1,    73,
      -1,    -1,    -1,    71,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   224,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,     3,    -1,    -1,    -1,
      -1,    39,    40,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,     3,    -1,    -1,
      -1,    -1,    39,    40,    72,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,     3,    -1,
      -1,    -1,    69,    39,    40,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    30,
      31,    32,    33,    69,    39,    40,    -1,     3,    39,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    -1,    -1,
      -1,    -1,    -1,    39,    40,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    52,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    -1,    -1,    -1,
      -1,    -1,    39,    40,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    52,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    -1,    -1,    -1,    -1,
      -1,    39,    40,    -1,     3,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    52,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    -1,    -1,    -1,    -1,    -1,
      39,    40,    -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    52,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    -1,    -1,    -1,    -1,    -1,    39,
      40,    -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    52,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    -1,    -1,    -1,    -1,    -1,    39,    40,
      -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    -1,    -1,    -1,    -1,    -1,    39,    40,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    -1,
      -1,    -1,    -1,    -1,    39,    40,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    -1,    -1,    -1,    -1,    -1,
      39,    40,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      -1,    -1,    -1,    -1,    -1,    39,    40,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    -1,    -1,    -1,    -1,    -1,    39,    40,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    -1,    -1,    -1,    -1,    -1,
      39,    40,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    -1,    -1,    -1,    -1,    -1,
      39,    40
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,    41,    42,    51,    76,    77,    78,    82,    90,
      71,    86,    40,    90,     0,    76,    76,    76,    21,    64,
      41,    72,    87,    88,    48,    89,    90,    90,    91,    92,
      52,    90,    42,    90,    72,    73,    69,    93,    89,    74,
      73,    69,    40,    87,     1,    35,    36,    41,    44,    45,
      46,    54,    55,    57,    58,    60,    61,    65,    67,    68,
      71,    90,    94,    95,    96,    97,   100,   101,    93,    91,
      41,    43,    79,    80,    81,    82,    83,    84,    85,    91,
      94,    41,    97,    97,     9,    71,    90,    97,    97,    97,
      97,    41,    52,    97,    41,    99,   100,    97,    41,   101,
      97,    64,    71,    70,    94,     3,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    39,    40,    52,    52,
       9,    86,    90,    86,    70,    79,    74,    39,    40,    97,
      72,    97,    98,     9,    69,    93,    93,    93,    66,    52,
      93,    73,    52,    52,    72,    41,    72,    98,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    41,
      98,    97,    89,    90,     9,    52,    89,    90,    41,    98,
      73,    72,    97,    53,   102,   103,    56,    97,    99,    71,
      72,     9,    71,    74,    52,    50,    52,    89,    97,    93,
      89,    74,    98,    41,    70,   102,    93,    96,    93,    72,
      98,    97,    72,    98,     9,    52,    50,    52,    52,    93,
      90,    72,    52,    72,    97,    52,    93,    52
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
      yyerror (&yylloc, parser, scanner, YY_("syntax error: cannot back up")); \
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
# define YYLEX yylex (&yylval, &yylloc, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, &yylloc, scanner)
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
		  Type, Value, Location, parser, scanner); \
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, Parser* parser, void* scanner)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, parser, scanner)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    Parser* parser;
    void* scanner;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, Parser* parser, void* scanner)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp, parser, scanner)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    Parser* parser;
    void* scanner;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, parser, scanner);
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
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, Parser* parser, void* scanner)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule, parser, scanner)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
    Parser* parser;
    void* scanner;
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
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       , parser, scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule, parser, scanner); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, Parser* parser, void* scanner)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp, parser, scanner)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
    Parser* parser;
    void* scanner;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (parser);
  YYUSE (scanner);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {
      case 44: /* "STRING" */

/* Line 1000 of yacc.c  */
#line 38 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->expression); (yyvaluep->expression) = 0; };

/* Line 1000 of yacc.c  */
#line 1510 "src/compiler/Grammar.cc"
	break;
      case 45: /* "NUMBER" */

/* Line 1000 of yacc.c  */
#line 38 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->expression); (yyvaluep->expression) = 0; };

/* Line 1000 of yacc.c  */
#line 1519 "src/compiler/Grammar.cc"
	break;
      case 46: /* "BOOLEAN" */

/* Line 1000 of yacc.c  */
#line 38 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->expression); (yyvaluep->expression) = 0; };

/* Line 1000 of yacc.c  */
#line 1528 "src/compiler/Grammar.cc"
	break;
      case 77: /* "import" */

/* Line 1000 of yacc.c  */
#line 42 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1000 of yacc.c  */
#line 1537 "src/compiler/Grammar.cc"
	break;
      case 78: /* "class" */

/* Line 1000 of yacc.c  */
#line 42 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1000 of yacc.c  */
#line 1546 "src/compiler/Grammar.cc"
	break;
      case 79: /* "feature_list" */

/* Line 1000 of yacc.c  */
#line 42 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1000 of yacc.c  */
#line 1555 "src/compiler/Grammar.cc"
	break;
      case 80: /* "feature" */

/* Line 1000 of yacc.c  */
#line 42 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1000 of yacc.c  */
#line 1564 "src/compiler/Grammar.cc"
	break;
      case 81: /* "attribute" */

/* Line 1000 of yacc.c  */
#line 42 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1000 of yacc.c  */
#line 1573 "src/compiler/Grammar.cc"
	break;
      case 82: /* "function" */

/* Line 1000 of yacc.c  */
#line 42 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1000 of yacc.c  */
#line 1582 "src/compiler/Grammar.cc"
	break;
      case 83: /* "operator" */

/* Line 1000 of yacc.c  */
#line 42 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1000 of yacc.c  */
#line 1591 "src/compiler/Grammar.cc"
	break;
      case 84: /* "prototype" */

/* Line 1000 of yacc.c  */
#line 42 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1000 of yacc.c  */
#line 1600 "src/compiler/Grammar.cc"
	break;
      case 85: /* "native" */

/* Line 1000 of yacc.c  */
#line 42 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1000 of yacc.c  */
#line 1609 "src/compiler/Grammar.cc"
	break;
      case 86: /* "formal_signature" */

/* Line 1000 of yacc.c  */
#line 43 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->formal); (yyvaluep->formal) = 0; };

/* Line 1000 of yacc.c  */
#line 1618 "src/compiler/Grammar.cc"
	break;
      case 87: /* "formal_list" */

/* Line 1000 of yacc.c  */
#line 43 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->formal); (yyvaluep->formal) = 0; };

/* Line 1000 of yacc.c  */
#line 1627 "src/compiler/Grammar.cc"
	break;
      case 88: /* "formal" */

/* Line 1000 of yacc.c  */
#line 43 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->formal); (yyvaluep->formal) = 0; };

/* Line 1000 of yacc.c  */
#line 1636 "src/compiler/Grammar.cc"
	break;
      case 90: /* "type" */

/* Line 1000 of yacc.c  */
#line 44 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->type); (yyvaluep->type) = 0; };

/* Line 1000 of yacc.c  */
#line 1645 "src/compiler/Grammar.cc"
	break;
      case 91: /* "generic_list" */

/* Line 1000 of yacc.c  */
#line 45 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->generic); (yyvaluep->generic) = 0; };

/* Line 1000 of yacc.c  */
#line 1654 "src/compiler/Grammar.cc"
	break;
      case 92: /* "generic" */

/* Line 1000 of yacc.c  */
#line 45 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->generic); (yyvaluep->generic) = 0; };

/* Line 1000 of yacc.c  */
#line 1663 "src/compiler/Grammar.cc"
	break;
      case 93: /* "block" */

/* Line 1000 of yacc.c  */
#line 40 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->statement); (yyvaluep->statement) = 0; };

/* Line 1000 of yacc.c  */
#line 1672 "src/compiler/Grammar.cc"
	break;
      case 94: /* "statement_list" */

/* Line 1000 of yacc.c  */
#line 40 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->statement); (yyvaluep->statement) = 0; };

/* Line 1000 of yacc.c  */
#line 1681 "src/compiler/Grammar.cc"
	break;
      case 95: /* "statement" */

/* Line 1000 of yacc.c  */
#line 40 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->statement); (yyvaluep->statement) = 0; };

/* Line 1000 of yacc.c  */
#line 1690 "src/compiler/Grammar.cc"
	break;
      case 96: /* "conditional" */

/* Line 1000 of yacc.c  */
#line 40 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->statement); (yyvaluep->statement) = 0; };

/* Line 1000 of yacc.c  */
#line 1699 "src/compiler/Grammar.cc"
	break;
      case 97: /* "expression" */

/* Line 1000 of yacc.c  */
#line 38 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->expression); (yyvaluep->expression) = 0; };

/* Line 1000 of yacc.c  */
#line 1708 "src/compiler/Grammar.cc"
	break;
      case 98: /* "expression_list" */

/* Line 1000 of yacc.c  */
#line 38 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->expression); (yyvaluep->expression) = 0; };

/* Line 1000 of yacc.c  */
#line 1717 "src/compiler/Grammar.cc"
	break;
      case 99: /* "variable_list" */

/* Line 1000 of yacc.c  */
#line 46 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->variable); (yyvaluep->variable) = 0; };

/* Line 1000 of yacc.c  */
#line 1726 "src/compiler/Grammar.cc"
	break;
      case 100: /* "variable" */

/* Line 1000 of yacc.c  */
#line 46 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->variable); (yyvaluep->variable) = 0; };

/* Line 1000 of yacc.c  */
#line 1735 "src/compiler/Grammar.cc"
	break;
      case 101: /* "call" */

/* Line 1000 of yacc.c  */
#line 38 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->expression); (yyvaluep->expression) = 0; };

/* Line 1000 of yacc.c  */
#line 1744 "src/compiler/Grammar.cc"
	break;
      case 102: /* "when_list" */

/* Line 1000 of yacc.c  */
#line 40 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->statement); (yyvaluep->statement) = 0; };

/* Line 1000 of yacc.c  */
#line 1753 "src/compiler/Grammar.cc"
	break;
      case 103: /* "when" */

/* Line 1000 of yacc.c  */
#line 40 "src/compiler/Grammar.yy"
	{ delete (yyvaluep->statement); (yyvaluep->statement) = 0; };

/* Line 1000 of yacc.c  */
#line 1762 "src/compiler/Grammar.cc"
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
int yyparse (Parser* parser, void* scanner);
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
yyparse (Parser* parser, void* scanner)
#else
int
yyparse (parser, scanner)
    Parser* parser;
    void* scanner;
#endif
#endif
{
/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

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

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[2];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
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
  yylsp = yyls;

#if YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 1;
#endif

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
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);

	yyls = yyls1;
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
	YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
  *++yylsp = yylloc;
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

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 98 "src/compiler/Grammar.yy"
    {
        parser->module()->feature((yyvsp[(1) - (2)].feature));
    ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 101 "src/compiler/Grammar.yy"
    {
        parser->module()->feature((yyvsp[(1) - (2)].feature));
    ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 104 "src/compiler/Grammar.yy"
    {
        parser->module()->feature((yyvsp[(1) - (2)].feature));
    ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 107 "src/compiler/Grammar.yy"
    { ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 111 "src/compiler/Grammar.yy"
    {
        // TODO: Generic in the import? That's probably bad.
        (yyval.feature) = new Import((yyloc), (yyvsp[(2) - (3)].type));
    ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 118 "src/compiler/Grammar.yy"
    {
        (yyval.feature) = new Class((yyloc), (yyvsp[(1) - (6)].type), (yyvsp[(5) - (6)].feature));
        delete (yyvsp[(3) - (6)].type); // FixMe
    ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 122 "src/compiler/Grammar.yy"
    {
        (yyval.feature) = 0;    
    ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 128 "src/compiler/Grammar.yy"
    { 
        (yyval.feature) = (yyvsp[(1) - (2)].feature);
        (yyval.feature)->next((yyvsp[(2) - (2)].feature));
    ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 132 "src/compiler/Grammar.yy"
    {
        (yyval.feature) = 0;
    ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 138 "src/compiler/Grammar.yy"
    { (yyval.feature) = (yyvsp[(1) - (1)].feature); ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 139 "src/compiler/Grammar.yy"
    { (yyval.feature) = (yyvsp[(1) - (1)].feature); ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 140 "src/compiler/Grammar.yy"
    { (yyval.feature) = (yyvsp[(1) - (1)].feature); ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 141 "src/compiler/Grammar.yy"
    { (yyval.feature) = (yyvsp[(1) - (1)].feature); ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 142 "src/compiler/Grammar.yy"
    { (yyval.feature) = (yyvsp[(1) - (1)].feature); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 146 "src/compiler/Grammar.yy"
    {
		// TODO: Set symbol table for class-level
		(yyval.feature) = new Attribute((yyloc), (yyvsp[(1) - (5)].name), (yyvsp[(2) - (5)].type), (yyvsp[(4) - (5)].expression));
    ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 150 "src/compiler/Grammar.yy"
    {
		// TODO: Set symbol table for class-level
		(yyval.feature) = new Attribute((yyloc), (yyvsp[(1) - (4)].name), parser->environment()->void_type(), (yyvsp[(3) - (4)].expression));
    ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 154 "src/compiler/Grammar.yy"
    {
		(yyval.feature) = new Attribute((yyloc), (yyvsp[(1) - (3)].name), (yyvsp[(2) - (3)].type), new Empty((yyloc)));
    ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 160 "src/compiler/Grammar.yy"
    {
        (yyval.feature) = new Function((yyloc), (yyvsp[(1) - (5)].name), (yyvsp[(2) - (5)].formal), (yyvsp[(3) - (5)].type), (yyvsp[(5) - (5)].statement));
    ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 163 "src/compiler/Grammar.yy"
    {
        Type* type = parser->environment()->void_type();
        (yyval.feature) = new Function((yyloc), (yyvsp[(1) - (4)].name), (yyvsp[(2) - (4)].formal), type, (yyvsp[(4) - (4)].statement));
	;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 170 "src/compiler/Grammar.yy"
    {
        (yyval.feature) = new Function((yyloc), (yyvsp[(1) - (5)].name), (yyvsp[(2) - (5)].formal), (yyvsp[(3) - (5)].type), (yyvsp[(5) - (5)].statement));
    ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 173 "src/compiler/Grammar.yy"
    {
        Type* type = parser->environment()->void_type();
        (yyval.feature) = new Function((yyloc), (yyvsp[(1) - (4)].name), (yyvsp[(2) - (4)].formal), type, (yyvsp[(4) - (4)].statement));
	;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 180 "src/compiler/Grammar.yy"
    {
        (yyval.feature) = new Function((yyloc), (yyvsp[(1) - (5)].name), (yyvsp[(2) - (5)].formal), (yyvsp[(3) - (5)].type), 0);
	;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 183 "src/compiler/Grammar.yy"
    {
        Type* type = parser->environment()->void_type();
        (yyval.feature) = new Function((yyloc), (yyvsp[(1) - (4)].name), (yyvsp[(2) - (4)].formal), type, 0);
	;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 190 "src/compiler/Grammar.yy"
    {
        (yyval.feature) = new Function((yyloc), (yyvsp[(1) - (6)].name), (yyvsp[(2) - (6)].formal), (yyvsp[(3) - (6)].type), 0);
	;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 193 "src/compiler/Grammar.yy"
    {
        Type* type = parser->environment()->void_type();
        (yyval.feature) = new Function((yyloc), (yyvsp[(1) - (5)].name), (yyvsp[(2) - (5)].formal), type, 0);
	;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 200 "src/compiler/Grammar.yy"
    { (yyval.formal) = (yyvsp[(2) - (3)].formal); ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 201 "src/compiler/Grammar.yy"
    { (yyval.formal) = 0; ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 205 "src/compiler/Grammar.yy"
    { 
        (yyval.formal) = (yyvsp[(1) - (3)].formal);
        (yyval.formal)->next((yyvsp[(3) - (3)].formal));
	;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 209 "src/compiler/Grammar.yy"
    { 
        (yyval.formal) = (yyvsp[(1) - (1)].formal);
	;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 215 "src/compiler/Grammar.yy"
    {
        (yyval.formal) = new Formal((yyloc), (yyvsp[(1) - (2)].name), (yyvsp[(2) - (2)].type));
    ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 220 "src/compiler/Grammar.yy"
    { (yyval.flag) = 0; ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 221 "src/compiler/Grammar.yy"
    { (yyval.flag) = 0; ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 225 "src/compiler/Grammar.yy"
    { 
        (yyval.type) = new Type((yyvsp[(1) - (3)].type), (yyvsp[(3) - (3)].name), 0, parser->environment()); 
    ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 228 "src/compiler/Grammar.yy"
    {
        (yyval.type) = new Type((yyvsp[(1) - (6)].type), (yyvsp[(3) - (6)].name), (yyvsp[(5) - (6)].generic), parser->environment());
    ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 231 "src/compiler/Grammar.yy"
    {
        (yyval.type) = new Type(0, (yyvsp[(1) - (1)].name), 0, parser->environment()); 
    ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 234 "src/compiler/Grammar.yy"
    {
        (yyval.type) = new Type(0, (yyvsp[(1) - (4)].name), (yyvsp[(3) - (4)].generic), parser->environment());
    ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 240 "src/compiler/Grammar.yy"
    {
        (yyval.generic) = (yyvsp[(1) - (3)].generic);
        (yyval.generic)->next((yyvsp[(3) - (3)].generic));
    ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 244 "src/compiler/Grammar.yy"
    {
        (yyval.generic) = (yyvsp[(1) - (1)].generic);
    ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 250 "src/compiler/Grammar.yy"
    { (yyval.generic) = new Generic((yyvsp[(1) - (1)].type)); ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 254 "src/compiler/Grammar.yy"
    { 
		(yyval.statement) = new Block((yyloc), (yyvsp[(2) - (3)].statement)); 
	;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 260 "src/compiler/Grammar.yy"
    { 
		(yyval.statement) = (yyvsp[(1) - (2)].statement);
        (yyval.statement)->next((yyvsp[(2) - (2)].statement));
	;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 264 "src/compiler/Grammar.yy"
    {
        (yyval.statement) = 0;
    ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 267 "src/compiler/Grammar.yy"
    { 
        (yyval.statement) = (yyvsp[(2) - (2)].statement); 
    ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 274 "src/compiler/Grammar.yy"
    {
		(yyval.statement) = new For((yyloc), (yyvsp[(2) - (5)].name), (yyvsp[(4) - (5)].expression), (yyvsp[(5) - (5)].statement));
	;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 277 "src/compiler/Grammar.yy"
    {
        (yyval.statement) = new Let((yyloc), (yyvsp[(2) - (3)].variable), (yyvsp[(3) - (3)].statement));
    ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 280 "src/compiler/Grammar.yy"
    {
        // TODO: FIX UNTIL
        Name* op = parser->environment()->name("!");
		(yyval.statement) = new While((yyloc), new Unary((yyloc), op, (yyvsp[(2) - (3)].expression)), (yyvsp[(3) - (3)].statement));
        
	;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 286 "src/compiler/Grammar.yy"
    {
		(yyval.statement) = new While((yyloc), (yyvsp[(2) - (3)].expression), (yyvsp[(3) - (3)].statement));
	;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 289 "src/compiler/Grammar.yy"
    {
        (yyval.statement) = new Case((yyloc), (yyvsp[(2) - (5)].expression), static_cast<When*>((yyvsp[(4) - (5)].statement)));
    ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 292 "src/compiler/Grammar.yy"
    { 
        (yyval.statement) = new Return((yyloc), (yyvsp[(2) - (3)].expression));
    ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 295 "src/compiler/Grammar.yy"
    { 
        (yyval.statement) = new Return((yyloc), new Empty((yyloc)));
    ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 298 "src/compiler/Grammar.yy"
    {
        (yyval.statement) = new Yield((yyloc), (yyvsp[(2) - (3)].expression));
    ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 301 "src/compiler/Grammar.yy"
    {
        (yyval.statement) = new Fork((yyloc), (yyvsp[(2) - (3)].expression));
    ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 304 "src/compiler/Grammar.yy"
    {
        (yyval.statement) = (yyvsp[(1) - (2)].variable);
    ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 307 "src/compiler/Grammar.yy"
    { 
		(yyval.statement) = new Simple((yyloc), (yyvsp[(1) - (2)].expression)); 
	;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 310 "src/compiler/Grammar.yy"
    {
        /* Indexed assignment */
        Environment* env = parser->environment();
        (yyvsp[(1) - (7)].expression)->next((yyvsp[(3) - (7)].expression));
        Expression* last = (yyvsp[(3) - (7)].expression);
        while (last->next()) {
            last = last->next();
        }
        last->next((yyvsp[(6) - (7)].expression));

        /* Invokes the special @insert method, a la Lua and Python */
        (yyval.statement) = new Simple((yyloc), new Dispatch((yyloc), env->name("@insert"), (yyvsp[(1) - (7)].expression)));
    ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 323 "src/compiler/Grammar.yy"
    {
        /* Attribute assignment, calls the mutator function */
        Name* name = parser->environment()->name((yyvsp[(3) - (6)].name)->string() + "=");
        (yyvsp[(1) - (6)].expression)->next((yyvsp[(5) - (6)].expression));
        (yyval.statement) = new Simple((yyloc), new Dispatch((yyloc), name, (yyvsp[(1) - (6)].expression)));
    ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 329 "src/compiler/Grammar.yy"
    {
        (yyval.statement) = (yyvsp[(1) - (1)].statement);
    ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 335 "src/compiler/Grammar.yy"
    {
		(yyval.statement) = new Conditional((yyloc), (yyvsp[(2) - (3)].expression), (yyvsp[(3) - (3)].statement), new Block((yyloc), 0));
	;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 338 "src/compiler/Grammar.yy"
    {
        (yyval.statement) = new Conditional((yyloc), (yyvsp[(2) - (5)].expression), (yyvsp[(3) - (5)].statement), (yyvsp[(5) - (5)].statement));
    ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 341 "src/compiler/Grammar.yy"
    {
        (yyval.statement) = new Conditional((yyloc), (yyvsp[(2) - (5)].expression), (yyvsp[(3) - (5)].statement), (yyvsp[(5) - (5)].statement));
    ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 346 "src/compiler/Grammar.yy"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("?"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        assert("Not implemented");
	;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 350 "src/compiler/Grammar.yy"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("or"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 353 "src/compiler/Grammar.yy"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("and"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 356 "src/compiler/Grammar.yy"
    {
        (yyvsp[(1) - (3)].expression)->next((yyvsp[(3) - (3)].expression));
        (yyval.expression) = new Dispatch((yyloc), parser->environment()->name("@bitxor"), (yyvsp[(1) - (3)].expression));
    ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 360 "src/compiler/Grammar.yy"
    { 
        (yyvsp[(1) - (3)].expression)->next((yyvsp[(3) - (3)].expression));
		(yyval.expression) = new Dispatch((yyloc), parser->environment()->name("@bitor"), (yyvsp[(1) - (3)].expression)); 
	;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 364 "src/compiler/Grammar.yy"
    { 
        (yyvsp[(1) - (3)].expression)->next((yyvsp[(3) - (3)].expression));
		(yyval.expression) = new Dispatch((yyloc), parser->environment()->name("@bitand"), (yyvsp[(1) - (3)].expression)); 
	;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 368 "src/compiler/Grammar.yy"
    { 
        (yyvsp[(1) - (3)].expression)->next((yyvsp[(3) - (3)].expression));
		(yyval.expression) = new Dispatch((yyloc), parser->environment()->name("@equal"), (yyvsp[(1) - (3)].expression)); 
	;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 372 "src/compiler/Grammar.yy"
    { 
        (yyvsp[(1) - (3)].expression)->next((yyvsp[(3) - (3)].expression));
        Environment* env = parser->environment();
        Dispatch* equal = new Dispatch((yyloc), env->name("@equal"), (yyvsp[(1) - (3)].expression));
        (yyval.expression) = new Unary((yyloc), env->name("!"), equal);
	;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 378 "src/compiler/Grammar.yy"
    {
        (yyvsp[(1) - (3)].expression)->next((yyvsp[(3) - (3)].expression));
        Environment* env = parser->environment();
        (yyval.expression) = new Dispatch((yyloc), env->name("@compare"), (yyvsp[(1) - (3)].expression));
    ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 383 "src/compiler/Grammar.yy"
    { 
        (yyvsp[(1) - (3)].expression)->next((yyvsp[(3) - (3)].expression));
        Environment* env = parser->environment();
        Dispatch* less = new Dispatch((yyloc), env->name("@less"), (yyvsp[(1) - (3)].expression)); 
        Dispatch* equal = new Dispatch((yyloc), env->name("@equal"), (yyvsp[(1) - (3)].expression));
        Binary* child = new Binary((yyloc), env->name("||"), less, equal); 
		(yyval.expression) = new Unary((yyloc), env->name("!"), child); 
	;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 391 "src/compiler/Grammar.yy"
    { 
        (yyvsp[(1) - (3)].expression)->next((yyvsp[(3) - (3)].expression));
		(yyval.expression) = new Dispatch((yyloc), parser->environment()->name("@less"), (yyvsp[(1) - (3)].expression)); 
	;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 395 "src/compiler/Grammar.yy"
    { 
        (yyvsp[(1) - (3)].expression)->next((yyvsp[(3) - (3)].expression));
        Environment* env = parser->environment();
        Dispatch* less = new Dispatch((yyloc), env->name("@less"), (yyvsp[(1) - (3)].expression));
        (yyval.expression) = new Unary((yyloc), env->name("!"), less); 
	;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 401 "src/compiler/Grammar.yy"
    { 
        (yyvsp[(1) - (3)].expression)->next((yyvsp[(3) - (3)].expression));
        Environment* env = parser->environment();
        Dispatch* less = new Dispatch((yyloc), env->name("@less"), (yyvsp[(1) - (3)].expression));
        Dispatch* equal = new Dispatch((yyloc), env->name("@equal"), (yyvsp[(1) - (3)].expression));
        (yyval.expression) = new Binary((yyloc), env->name("||"), less, equal);  
	;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 408 "src/compiler/Grammar.yy"
    { 
        (yyvsp[(1) - (3)].expression)->next((yyvsp[(3) - (3)].expression));
		(yyval.expression) = new Dispatch((yyloc), parser->environment()->name("@shift"), (yyvsp[(1) - (3)].expression)); 
	;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 412 "src/compiler/Grammar.yy"
    { 
        (yyvsp[(1) - (3)].expression)->next((yyvsp[(3) - (3)].expression));
		(yyval.expression) = new Dispatch((yyloc), parser->environment()->name("@unshift"), (yyvsp[(1) - (3)].expression)); 
	;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 416 "src/compiler/Grammar.yy"
    { 
        (yyvsp[(1) - (3)].expression)->next((yyvsp[(3) - (3)].expression));
		(yyval.expression) = new Dispatch((yyloc), parser->environment()->name("@power"), (yyvsp[(1) - (3)].expression)); 
	;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 420 "src/compiler/Grammar.yy"
    { 
        (yyvsp[(1) - (3)].expression)->next((yyvsp[(3) - (3)].expression));
		(yyval.expression) = new Dispatch((yyloc), parser->environment()->name("@add"), (yyvsp[(1) - (3)].expression)); 
	;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 424 "src/compiler/Grammar.yy"
    { 
        (yyvsp[(1) - (3)].expression)->next((yyvsp[(3) - (3)].expression));
		(yyval.expression) = new Dispatch((yyloc), parser->environment()->name("@subtract"), (yyvsp[(1) - (3)].expression)); 
	;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 428 "src/compiler/Grammar.yy"
    { 
        (yyvsp[(1) - (3)].expression)->next((yyvsp[(3) - (3)].expression));
		(yyval.expression) = new Dispatch((yyloc), parser->environment()->name("@multiply"), (yyvsp[(1) - (3)].expression)); 
	;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 432 "src/compiler/Grammar.yy"
    { 
        (yyvsp[(1) - (3)].expression)->next((yyvsp[(3) - (3)].expression));
		(yyval.expression) = new Dispatch((yyloc), parser->environment()->name("@divide"), (yyvsp[(1) - (3)].expression)); 
	;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 436 "src/compiler/Grammar.yy"
    { 
        (yyvsp[(1) - (3)].expression)->next((yyvsp[(3) - (3)].expression));
		(yyval.expression) = new Dispatch((yyloc), parser->environment()->name("@modulus"), (yyvsp[(1) - (3)].expression)); 
	;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 440 "src/compiler/Grammar.yy"
    { 
        (yyval.expression) = new Unary((yyloc), parser->environment()->name("!"), (yyvsp[(2) - (2)].expression)); 
    ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 443 "src/compiler/Grammar.yy"
    { 
        (yyval.expression) = new Dispatch((yyloc), parser->environment()->name("@complement"), (yyvsp[(2) - (2)].expression)); 
    ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 446 "src/compiler/Grammar.yy"
    {
        /* Invokes the special method, a la Lua and Python */
        (yyvsp[(1) - (4)].expression)->next((yyvsp[(3) - (4)].expression));
        (yyval.expression) = new Dispatch((yyloc), parser->environment()->name("@index"), (yyvsp[(1) - (4)].expression));
    ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 451 "src/compiler/Grammar.yy"
    {
        /* Attribute read, calls the accessor function */
        (yyval.expression) = new Dispatch((yyloc), (yyvsp[(3) - (3)].name), (yyvsp[(1) - (3)].expression));
    ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 455 "src/compiler/Grammar.yy"
    {
        (yyvsp[(1) - (6)].expression)->next((yyvsp[(5) - (6)].expression));
        (yyval.expression) = new Dispatch((yyloc), (yyvsp[(3) - (6)].name), (yyvsp[(1) - (6)].expression));
    ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 459 "src/compiler/Grammar.yy"
    {
        (yyval.expression) = new Dispatch((yyloc), (yyvsp[(3) - (5)].name), (yyvsp[(1) - (5)].expression));
    ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 462 "src/compiler/Grammar.yy"
    { (yyval.expression) = (yyvsp[(1) - (1)].expression); ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 463 "src/compiler/Grammar.yy"
    { (yyval.expression) = (yyvsp[(2) - (3)].expression); ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 464 "src/compiler/Grammar.yy"
    { (yyval.expression) = (yyvsp[(1) - (1)].expression); ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 465 "src/compiler/Grammar.yy"
    { (yyval.expression) = (yyvsp[(1) - (1)].expression); ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 466 "src/compiler/Grammar.yy"
    { (yyval.expression) = (yyvsp[(1) - (1)].expression); ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 467 "src/compiler/Grammar.yy"
    { (yyval.expression) = new Identifier((yyloc), (yyvsp[(1) - (1)].name)); ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 471 "src/compiler/Grammar.yy"
    { 
		(yyval.expression) = (yyvsp[(1) - (3)].expression);
        (yyval.expression)->next((yyvsp[(3) - (3)].expression));
	;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 475 "src/compiler/Grammar.yy"
    { (yyval.expression) = (yyvsp[(1) - (1)].expression); ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 480 "src/compiler/Grammar.yy"
    {
        (yyval.variable) = (yyvsp[(1) - (3)].variable);
        (yyval.variable)->next((yyvsp[(3) - (3)].variable));
    ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 484 "src/compiler/Grammar.yy"
    {
        (yyval.variable) = (yyvsp[(1) - (1)].variable);
    ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 490 "src/compiler/Grammar.yy"
    {
        (yyval.variable) = new Variable((yyloc), (yyvsp[(1) - (2)].name), (yyvsp[(2) - (2)].type), new Empty((yyloc)));
    ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 493 "src/compiler/Grammar.yy"
    {
        (yyval.variable) = new Variable((yyloc), (yyvsp[(1) - (4)].name), (yyvsp[(2) - (4)].type), (yyvsp[(4) - (4)].expression));
    ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 496 "src/compiler/Grammar.yy"
    {
        (yyval.variable) = new Variable((yyloc), (yyvsp[(1) - (3)].name), parser->environment()->void_type(), (yyvsp[(3) - (3)].expression));
    ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 502 "src/compiler/Grammar.yy"
    {
		(yyval.expression) = new Call((yyloc), 0, (yyvsp[(1) - (4)].name), (yyvsp[(3) - (4)].expression));
    ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 505 "src/compiler/Grammar.yy"
    {
		/* Call on an object expression */
		(yyval.expression) = new Call((yyloc), 0, (yyvsp[(1) - (3)].name), 0);
	;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 509 "src/compiler/Grammar.yy"
    {
        (yyval.expression) = new Call((yyloc), (yyvsp[(1) - (6)].type), (yyvsp[(3) - (6)].name), (yyvsp[(5) - (6)].expression));
    ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 512 "src/compiler/Grammar.yy"
    {
        (yyval.expression) = new Call((yyloc), (yyvsp[(1) - (5)].type), (yyvsp[(3) - (5)].name), 0);
    ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 515 "src/compiler/Grammar.yy"
    {
        (yyval.expression) = new Construct((yyloc), (yyvsp[(1) - (4)].type), (yyvsp[(3) - (4)].expression));
    ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 518 "src/compiler/Grammar.yy"
    {
        (yyval.expression) = new Construct((yyloc), (yyvsp[(1) - (3)].type), 0);
    ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 523 "src/compiler/Grammar.yy"
    {
        (yyval.statement) = (yyvsp[(1) - (2)].statement);
        (yyval.statement)->next((yyvsp[(2) - (2)].statement));
    ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 527 "src/compiler/Grammar.yy"
    {
        (yyval.statement) = (yyvsp[(1) - (1)].statement);
    ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 533 "src/compiler/Grammar.yy"
    {
        (yyval.statement) = new When((yyloc), (yyvsp[(2) - (4)].name), (yyvsp[(3) - (4)].type), (yyvsp[(4) - (4)].statement));
    ;}
    break;



/* Line 1455 of yacc.c  */
#line 3106 "src/compiler/Grammar.cc"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

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
      yyerror (&yylloc, parser, scanner, YY_("syntax error"));
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
	    yyerror (&yylloc, parser, scanner, yymsg);
	  }
	else
	  {
	    yyerror (&yylloc, parser, scanner, YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }

  yyerror_range[0] = yylloc;

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
		      yytoken, &yylval, &yylloc, parser, scanner);
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

  yyerror_range[0] = yylsp[1-yylen];
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

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp, parser, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

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
  yyerror (&yylloc, parser, scanner, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc, parser, scanner);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp, parser, scanner);
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



