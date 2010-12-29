/* A Bison parser, made by GNU Bison 2.4.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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
#define YYBISON_VERSION "2.4.3"

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
#line 1 "compiler2/Grammar.y"

#include "Parser.h"
#include "Expression.h"
#include "Statement.h"
#include "Formal.h"
#include "Feature.h"
#include "Unit.h"

#define YYSTYPE ParseNode
#define YYLTYPE Location
#define YY_EXTRA_TYPE Parser*
#define YY_NO_INPUT
#define YYERROR_VERBOSE
int yylex(ParseNode *node, Location *loc, void *scanner);
void yyerror(Location *loc, Parser *parser, void *scanner, const char *message);



/* Line 189 of yacc.c  */
#line 91 "compiler2/Grammar.cc"

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
     STRING = 276,
     NUMBER = 277,
     PUBLIC = 278,
     PRIVATE = 279,
     STATIC = 280,
     NATIVE = 281,
     CLASS = 282,
     INTERFACE = 283,
     STRUCT = 284,
     MODULE = 285,
     IMPORT = 286,
     DEF = 287,
     VAR = 288,
     INIT = 289,
     DESTROY = 290,
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
     LEFT_ARROW = 302
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 19 "compiler2/Grammar.y"
 Expression *expression; 

/* Line 214 of yacc.c  */
#line 20 "compiler2/Grammar.y"
 Statement *statement; 

/* Line 214 of yacc.c  */
#line 21 "compiler2/Grammar.y"
 Name *identifier; 

/* Line 214 of yacc.c  */
#line 22 "compiler2/Grammar.y"
 Unit *unit; 

/* Line 214 of yacc.c  */
#line 23 "compiler2/Grammar.y"
 Feature *feature; 

/* Line 214 of yacc.c  */
#line 24 "compiler2/Grammar.y"
 Formal *formal; 

/* Line 214 of yacc.c  */
#line 25 "compiler2/Grammar.y"
 int null; 

/* Line 214 of yacc.c  */
#line 26 "compiler2/Grammar.y"
 int flag; 


/* Line 214 of yacc.c  */
#line 206 "compiler2/Grammar.cc"
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
#line 231 "compiler2/Grammar.cc"

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
# if defined YYENABLE_NLS && YYENABLE_NLS
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
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   815

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  71
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  26
/* YYNRULES -- Number of rules.  */
#define YYNRULES  107
/* YYNRULES -- Number of states.  */
#define YYNSTATES  229

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   302

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     7,     2,     2,     2,    11,    22,     2,
      67,    68,     9,    12,    69,    13,     3,    10,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    66,     2,
      16,    27,    17,    35,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     4,     2,    70,    24,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    64,    23,    65,     8,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,     5,     6,
      14,    15,    18,    19,    20,    21,    25,    26,    28,    29,
      30,    31,    32,    33,    34,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     9,    15,    21,    27,    28,    30,    33,
      35,    37,    39,    41,    43,    45,    47,    49,    51,    55,
      60,    68,    74,    79,    85,    93,    99,   107,   113,   122,
     129,   133,   136,   140,   142,   146,   148,   150,   152,   155,
     158,   159,   161,   165,   170,   174,   178,   181,   183,   186,
     194,   198,   202,   208,   213,   221,   225,   228,   231,   234,
     238,   244,   248,   253,   257,   261,   265,   269,   273,   277,
     281,   285,   289,   293,   297,   301,   305,   309,   313,   317,
     321,   325,   329,   333,   337,   341,   345,   349,   353,   357,
     361,   364,   367,   370,   373,   378,   382,   389,   395,   397,
     401,   403,   405,   407,   411,   413,   416,   418
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      72,     0,    -1,    43,    36,    64,    73,    65,    -1,    44,
      36,    64,    73,    64,    -1,    45,    36,    64,    73,    65,
      -1,    46,    36,    64,    73,    65,    -1,    -1,     1,    -1,
      74,    73,    -1,    74,    -1,    75,    -1,    76,    -1,    77,
      -1,    78,    -1,    79,    -1,    80,    -1,    82,    -1,    81,
      -1,    47,    36,    52,    -1,    48,    87,    36,    52,    -1,
      49,    36,    66,    87,    27,    93,    52,    -1,    49,    36,
      66,    87,    52,    -1,    48,    50,    83,    88,    -1,    48,
      51,    67,    68,    88,    -1,    48,    36,    83,    66,    87,
      86,    88,    -1,    48,    36,    83,    86,    88,    -1,    48,
      36,    83,    66,    87,    86,    52,    -1,    48,    36,    83,
      86,    52,    -1,    48,    36,    83,    66,    87,    86,    42,
      52,    -1,    48,    36,    83,    86,    42,    52,    -1,    67,
      84,    68,    -1,    67,    68,    -1,    85,    69,    84,    -1,
      85,    -1,    36,    66,    87,    -1,    39,    -1,    40,    -1,
      41,    -1,    39,    41,    -1,    40,    41,    -1,    -1,    36,
      -1,    64,    89,    65,    -1,    62,    61,    93,    52,    -1,
      62,    61,    52,    -1,    62,    93,    52,    -1,    90,    89,
      -1,    90,    -1,     1,    89,    -1,    60,    36,    66,    87,
      63,    93,    88,    -1,    57,    93,    88,    -1,    55,    93,
      88,    -1,    54,    93,    64,    95,    65,    -1,    49,    36,
      66,    87,    -1,    49,    36,    66,    87,    27,    93,    52,
      -1,    61,    93,    52,    -1,    61,    52,    -1,    93,    52,
      -1,    92,    52,    -1,    58,    93,    88,    -1,    58,    93,
      88,    56,    88,    -1,    93,     3,    36,    -1,    93,     4,
      93,    70,    -1,    91,    27,    93,    -1,    91,    30,    93,
      -1,    91,    29,    93,    -1,    91,    28,    93,    -1,    91,
      31,    93,    -1,    91,    32,    93,    -1,    91,    34,    93,
      -1,    91,    33,    93,    -1,    93,    35,    93,    -1,    93,
      26,    93,    -1,    93,    25,    93,    -1,    93,    23,    93,
      -1,    93,    24,    93,    -1,    93,    22,    93,    -1,    93,
      21,    93,    -1,    93,    20,    93,    -1,    93,    17,    93,
      -1,    93,    16,    93,    -1,    93,    18,    93,    -1,    93,
      19,    93,    -1,    93,    15,    93,    -1,    93,    14,    93,
      -1,    93,    12,    93,    -1,    93,    13,    93,    -1,    93,
       9,    93,    -1,    93,    10,    93,    -1,    93,    11,    93,
      -1,     7,    93,    -1,     8,    93,    -1,    93,     6,    -1,
      93,     5,    -1,    36,    67,    94,    68,    -1,    36,    67,
      68,    -1,    93,     3,    36,    67,    94,    68,    -1,    93,
       3,    36,    67,    68,    -1,    91,    -1,    67,    93,    68,
      -1,    37,    -1,    38,    -1,    36,    -1,    93,    69,    94,
      -1,    93,    -1,    96,    95,    -1,    96,    -1,    53,    36,
      66,    87,    88,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    88,    88,    91,    94,    97,   100,   104,   108,   112,
     118,   119,   120,   121,   122,   123,   124,   125,   129,   135,
     141,   145,   151,   158,   165,   168,   174,   177,   183,   186,
     192,   193,   197,   201,   207,   212,   213,   214,   215,   216,
     217,   221,   225,   228,   231,   234,   240,   244,   247,   254,
     257,   263,   266,   269,   272,   275,   278,   281,   284,   287,
     290,   296,   300,   305,   309,   312,   315,   318,   321,   324,
     327,   334,   337,   340,   343,   346,   349,   352,   355,   358,
     361,   364,   367,   370,   373,   376,   379,   382,   385,   388,
     391,   394,   397,   400,   403,   406,   410,   414,   417,   418,
     419,   420,   421,   425,   429,   434,   438,   444
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "'.'", "'['", "DECREMENT", "INCREMENT",
  "'!'", "'~'", "'*'", "'/'", "'%'", "'+'", "'-'", "RIGHT_SHIFT",
  "LEFT_SHIFT", "'<'", "'>'", "GREATER_OR_EQUAL", "LESS_OR_EQUAL",
  "NOT_EQUAL", "EQUAL", "'&'", "'|'", "'^'", "AND", "OR", "'='",
  "MODULUS_ASSIGN", "DIVIDE_ASSIGN", "MULTIPLY_ASSIGN", "SUBTRACT_ASSIGN",
  "ADD_ASSIGN", "BIT_OR_ASSIGN", "BIT_AND_ASSIGN", "'?'", "IDENTIFIER",
  "STRING", "NUMBER", "PUBLIC", "PRIVATE", "STATIC", "NATIVE", "CLASS",
  "INTERFACE", "STRUCT", "MODULE", "IMPORT", "DEF", "VAR", "INIT",
  "DESTROY", "SEPARATOR", "WHEN", "CASE", "WHILE", "ELSE", "UNTIL", "IF",
  "DO", "FOR", "RETURN", "RIGHT_ARROW", "LEFT_ARROW", "'{'", "'}'", "':'",
  "'('", "')'", "','", "']'", "$accept", "unit", "feature_list", "feature",
  "import", "define", "attribute", "constructor", "destructor", "function",
  "prototype", "native", "formal_signature", "formal_list", "formal",
  "modifiers", "type", "block", "statement_list", "statement", "storage",
  "assignment", "expression", "expression_list", "when_list", "when", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,    46,    91,   258,   259,    33,   126,    42,
      47,    37,    43,    45,   260,   261,    60,    62,   262,   263,
     264,   265,    38,   124,    94,   266,   267,    61,   268,   269,
     270,   271,   272,   273,   274,    63,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     299,   300,   301,   302,   123,   125,    58,    40,    41,    44,
      93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    71,    72,    72,    72,    72,    72,    72,    73,    73,
      74,    74,    74,    74,    74,    74,    74,    74,    75,    76,
      77,    77,    78,    79,    80,    80,    81,    81,    82,    82,
      83,    83,    84,    84,    85,    86,    86,    86,    86,    86,
      86,    87,    88,    88,    88,    88,    89,    89,    89,    90,
      90,    90,    90,    90,    90,    90,    90,    90,    90,    90,
      90,    91,    91,    92,    92,    92,    92,    92,    92,    92,
      92,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    94,    94,    95,    95,    96
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     5,     5,     5,     5,     0,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     4,
       7,     5,     4,     5,     7,     5,     7,     5,     8,     6,
       3,     2,     3,     1,     3,     1,     1,     1,     2,     2,
       0,     1,     3,     4,     3,     3,     2,     1,     2,     7,
       3,     3,     5,     4,     7,     3,     2,     2,     2,     3,
       5,     3,     4,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     2,     2,     4,     3,     6,     5,     1,     3,
       1,     1,     1,     3,     1,     2,     1,     5
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     7,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     1,     0,     0,     0,     0,     0,     0,     0,     0,
       9,    10,    11,    12,    13,    14,    15,    17,    16,     0,
       0,     0,     0,    41,     0,     0,     0,     0,     2,     8,
       3,     4,     5,    18,     0,    40,     0,     0,     0,     0,
       0,    31,     0,    33,    35,    36,    37,     0,     0,     0,
       0,    22,     0,    19,    41,     0,     0,    30,     0,    38,
      39,    40,     0,    27,    25,     0,     0,   102,   100,   101,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    23,     0,    21,
      34,    32,     0,    29,    90,    91,     0,    44,     0,     0,
       0,     0,    93,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    45,    48,     0,     0,     0,     0,     0,
       0,    56,     0,    42,    46,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    57,     0,     0,    26,    24,    95,
     104,     0,    43,    99,    61,     0,    87,    88,    89,    85,
      86,    84,    83,    80,    79,    81,    82,    78,    77,    76,
      74,    75,    73,    72,    71,     0,     0,    51,    50,    59,
       0,    55,    63,    66,    65,    64,    67,    68,    70,    69,
      20,    28,     0,    94,     0,    62,    53,     0,     0,   106,
       0,     0,   103,    97,     0,     0,     0,    52,   105,    60,
       0,    96,     0,     0,     0,    54,     0,    49,   107
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     6,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    45,    52,    53,    58,    36,    61,    92,    93,
      82,    95,    96,   161,   208,   209
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -71
static const yytype_int16 yypact[] =
{
      40,   -71,   -21,     1,     8,    12,    57,    23,    24,    33,
      39,   -71,     5,     5,     5,     5,    66,   -29,    93,    77,
       5,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,    79,
      82,    97,   106,    96,    96,    99,   124,   131,   -71,   -71,
     -71,   -71,   -71,   -71,   -32,   -28,    92,   100,   123,   162,
     133,   -71,   137,   138,   165,   168,   -71,   162,   -36,   188,
     207,   -71,    92,   -71,   -71,   -25,   162,   -71,   174,   -71,
     -71,    38,   159,   -71,   -71,   196,   196,   145,   -71,   -71,
     355,   196,   -71,   423,   207,   177,   196,   196,   196,   196,
     191,   358,   163,   193,   189,   183,   467,   -71,   196,   -71,
     -71,   -71,   -13,   -71,   714,   714,   286,   -71,   511,   266,
     201,   196,   -71,   -71,   196,   196,   196,   196,   196,   196,
     196,   196,   196,   196,   196,   196,   196,   196,   196,   196,
     196,   196,   196,   -71,   -71,   172,   326,   293,   293,   293,
     173,   -71,   555,   -71,   -71,   196,   196,   196,   196,   196,
     196,   196,   196,   -71,   -71,   599,   194,   -71,   -71,   -71,
     167,   184,   -71,   -71,   190,    95,   738,   738,   738,   760,
     760,   780,   780,   102,   102,   102,   102,   109,   109,   126,
      21,    21,   -17,   205,   -71,   162,   206,   -71,   -71,   185,
     162,   -71,   687,   687,   687,   687,   687,   687,   687,   687,
     -71,   -71,   196,   -71,   351,   -71,   239,   237,   230,   206,
      92,   257,   -71,   -71,   232,   196,   255,   -71,   -71,   -71,
     196,   -71,   643,   162,   293,   -71,    92,   -71,   -71
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -71,   -71,   125,   -71,   -71,   -71,   -71,   -71,   -71,   -71,
     -71,   -71,   291,   258,   -71,   256,   -49,   -57,   -70,   -71,
     -50,   -71,   -56,   -47,   147,   -71
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -48
static const yytype_int16 yytable[] =
{
      65,    74,    98,    83,    50,    97,    72,    33,    71,   131,
      94,    54,    55,    56,   134,     7,    73,   100,   132,   104,
     105,    34,    35,   144,   108,   109,    59,    99,    60,   156,
     136,   137,   138,   139,    94,   142,    51,     8,    57,   157,
      -6,     1,   155,    94,     9,   158,   130,   131,    10,    59,
     160,    60,    16,    17,    18,   165,   132,    11,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,    54,    55,    56,
     187,   188,   189,     2,     3,     4,     5,    12,    13,   192,
     193,   194,   195,   196,   197,   198,   199,    14,   110,   111,
     112,   113,    32,    15,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   125,   126,   127,   128,   129,   130,   131,    37,
     132,   127,   128,   129,   130,   131,   206,   132,    29,    30,
      31,   211,    38,    40,   132,    39,   160,    41,   160,   128,
     129,   130,   131,   219,    59,   212,    60,   214,    43,   222,
      48,   132,    42,    44,   224,   205,    47,   227,    62,   228,
     110,   111,   112,   113,   226,    63,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,    84,    75,    76,    49,    64,    66,
      75,    76,   132,    75,    76,    67,    69,    68,    84,    70,
      50,   103,   106,   135,    75,    76,   145,   146,   147,   148,
     149,   150,   151,   152,    77,    78,    79,   140,   143,    77,
      78,    79,    77,    78,    79,   153,   202,   164,   185,   190,
     132,   210,    85,    77,    78,    79,   201,    86,    87,    80,
      88,    89,   203,    90,    91,    81,    85,   204,   -47,   207,
      81,    86,    87,    81,    88,    89,   215,    90,    91,   110,
     111,   112,   113,   216,    81,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,    75,    76,   217,   110,   111,   112,   113,
     221,   132,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     220,   223,    77,    78,    79,    46,   101,   102,   132,   110,
     111,   112,   113,     0,   163,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,    81,   159,    59,   218,    60,    75,    76,
       0,   132,    75,    76,     0,    75,    76,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    77,    78,    79,
     186,    77,    78,    79,    77,    78,    79,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   107,     0,     0,
     141,     0,     0,     0,     0,     0,     0,     0,    81,   213,
       0,     0,    81,     0,     0,    81,   110,   111,   112,   113,
       0,     0,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
       0,     0,     0,     0,     0,     0,     0,     0,   132,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     110,   111,   112,   113,     0,   133,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,     0,     0,     0,     0,     0,     0,
       0,     0,   132,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   110,   111,   112,   113,     0,   154,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,     0,     0,
       0,     0,     0,     0,     0,     0,   132,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   110,   111,
     112,   113,     0,   162,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,     0,     0,     0,     0,     0,     0,     0,     0,
     132,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,   111,   112,   113,     0,   191,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,     0,     0,     0,     0,
       0,     0,     0,     0,   132,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   110,   111,   112,   113,
       0,   200,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
       0,     0,     0,     0,     0,     0,     0,     0,   132,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     110,   111,   112,   113,     0,   225,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,     0,     0,     0,     0,     0,     0,
       0,     0,   132,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,     0,     0,     0,     0,     0,     0,     0,     0,   132,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,     0,     0,     0,     0,     0,
       0,     0,     0,   132,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,     0,     0,     0,
       0,     0,     0,     0,     0,   132,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,     0,     0,     0,
       0,     0,     0,     0,     0,   132
};

static const yytype_int16 yycheck[] =
{
      49,    58,    27,    59,    36,    62,    42,    36,    57,    26,
      60,    39,    40,    41,    84,    36,    52,    66,    35,    75,
      76,    50,    51,    93,    80,    81,    62,    52,    64,    42,
      86,    87,    88,    89,    84,    91,    68,    36,    66,    52,
       0,     1,    98,    93,    36,   102,    25,    26,    36,    62,
     106,    64,    47,    48,    49,   111,    35,     0,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,    39,    40,    41,
     137,   138,   139,    43,    44,    45,    46,    64,    64,   145,
     146,   147,   148,   149,   150,   151,   152,    64,     3,     4,
       5,     6,    36,    64,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    20,    21,    22,    23,    24,    25,    26,    36,
      35,    22,    23,    24,    25,    26,   185,    35,    13,    14,
      15,   190,    65,    64,    35,    20,   202,    65,   204,    23,
      24,    25,    26,   210,    62,   202,    64,   204,    52,   215,
      36,    35,    65,    67,   220,    70,    67,   224,    68,   226,
       3,     4,     5,     6,   223,    52,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     1,     7,     8,    66,    36,    66,
       7,     8,    35,     7,     8,    68,    41,    69,     1,    41,
      36,    52,    67,    36,     7,     8,    27,    28,    29,    30,
      31,    32,    33,    34,    36,    37,    38,    36,    65,    36,
      37,    38,    36,    37,    38,    52,    69,    36,    66,    66,
      35,    56,    49,    36,    37,    38,    52,    54,    55,    61,
      57,    58,    68,    60,    61,    67,    49,    67,    65,    53,
      67,    54,    55,    67,    57,    58,    27,    60,    61,     3,
       4,     5,     6,    36,    67,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     7,     8,    65,     3,     4,     5,     6,
      68,    35,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      63,    66,    36,    37,    38,    34,    68,    71,    35,     3,
       4,     5,     6,    -1,    68,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    67,    68,    62,   209,    64,     7,     8,
      -1,    35,     7,     8,    -1,     7,     8,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      64,    36,    37,    38,    36,    37,    38,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      -1,    -1,    67,    -1,    -1,    67,     3,     4,     5,     6,
      -1,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    52,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    52,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    52,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    52,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    52,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    52,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    35
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,    43,    44,    45,    46,    72,    36,    36,    36,
      36,     0,    64,    64,    64,    64,    47,    48,    49,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    73,
      73,    73,    36,    36,    50,    51,    87,    36,    65,    73,
      64,    65,    65,    52,    67,    83,    83,    67,    36,    66,
      36,    68,    84,    85,    39,    40,    41,    66,    86,    62,
      64,    88,    68,    52,    36,    87,    66,    68,    69,    41,
      41,    87,    42,    52,    88,     7,     8,    36,    37,    38,
      61,    67,    91,    93,     1,    49,    54,    55,    57,    58,
      60,    61,    89,    90,    91,    92,    93,    88,    27,    52,
      87,    84,    86,    52,    93,    93,    67,    52,    93,    93,
       3,     4,     5,     6,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    35,    52,    89,    36,    93,    93,    93,    93,
      36,    52,    93,    65,    89,    27,    28,    29,    30,    31,
      32,    33,    34,    52,    52,    93,    42,    52,    88,    68,
      93,    94,    52,    68,    36,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    66,    64,    88,    88,    88,
      66,    52,    93,    93,    93,    93,    93,    93,    93,    93,
      52,    52,    69,    68,    67,    70,    87,    53,    95,    96,
      56,    87,    94,    68,    94,    27,    36,    65,    95,    88,
      63,    68,    93,    66,    93,    52,    87,    88,    88
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
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

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
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, Parser *parser, void *scanner)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, parser, scanner)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    Parser *parser;
    void *scanner;
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, Parser *parser, void *scanner)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp, parser, scanner)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    Parser *parser;
    void *scanner;
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
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, Parser *parser, void *scanner)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule, parser, scanner)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
    Parser *parser;
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, Parser *parser, void *scanner)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp, parser, scanner)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
    Parser *parser;
    void *scanner;
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
      case 37: /* "STRING" */

/* Line 1009 of yacc.c  */
#line 34 "compiler2/Grammar.y"
	{ delete (yyvaluep->expression); (yyvaluep->expression) = 0; };

/* Line 1009 of yacc.c  */
#line 1482 "compiler2/Grammar.cc"
	break;
      case 38: /* "NUMBER" */

/* Line 1009 of yacc.c  */
#line 34 "compiler2/Grammar.y"
	{ delete (yyvaluep->expression); (yyvaluep->expression) = 0; };

/* Line 1009 of yacc.c  */
#line 1491 "compiler2/Grammar.cc"
	break;
      case 73: /* "feature_list" */

/* Line 1009 of yacc.c  */
#line 38 "compiler2/Grammar.y"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1009 of yacc.c  */
#line 1500 "compiler2/Grammar.cc"
	break;
      case 74: /* "feature" */

/* Line 1009 of yacc.c  */
#line 38 "compiler2/Grammar.y"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1009 of yacc.c  */
#line 1509 "compiler2/Grammar.cc"
	break;
      case 75: /* "import" */

/* Line 1009 of yacc.c  */
#line 38 "compiler2/Grammar.y"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1009 of yacc.c  */
#line 1518 "compiler2/Grammar.cc"
	break;
      case 76: /* "define" */

/* Line 1009 of yacc.c  */
#line 38 "compiler2/Grammar.y"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1009 of yacc.c  */
#line 1527 "compiler2/Grammar.cc"
	break;
      case 77: /* "attribute" */

/* Line 1009 of yacc.c  */
#line 38 "compiler2/Grammar.y"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1009 of yacc.c  */
#line 1536 "compiler2/Grammar.cc"
	break;
      case 78: /* "constructor" */

/* Line 1009 of yacc.c  */
#line 38 "compiler2/Grammar.y"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1009 of yacc.c  */
#line 1545 "compiler2/Grammar.cc"
	break;
      case 79: /* "destructor" */

/* Line 1009 of yacc.c  */
#line 38 "compiler2/Grammar.y"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1009 of yacc.c  */
#line 1554 "compiler2/Grammar.cc"
	break;
      case 80: /* "function" */

/* Line 1009 of yacc.c  */
#line 38 "compiler2/Grammar.y"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1009 of yacc.c  */
#line 1563 "compiler2/Grammar.cc"
	break;
      case 81: /* "prototype" */

/* Line 1009 of yacc.c  */
#line 38 "compiler2/Grammar.y"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1009 of yacc.c  */
#line 1572 "compiler2/Grammar.cc"
	break;
      case 82: /* "native" */

/* Line 1009 of yacc.c  */
#line 38 "compiler2/Grammar.y"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1009 of yacc.c  */
#line 1581 "compiler2/Grammar.cc"
	break;
      case 83: /* "formal_signature" */

/* Line 1009 of yacc.c  */
#line 39 "compiler2/Grammar.y"
	{ delete (yyvaluep->formal); (yyvaluep->formal) = 0; };

/* Line 1009 of yacc.c  */
#line 1590 "compiler2/Grammar.cc"
	break;
      case 84: /* "formal_list" */

/* Line 1009 of yacc.c  */
#line 39 "compiler2/Grammar.y"
	{ delete (yyvaluep->formal); (yyvaluep->formal) = 0; };

/* Line 1009 of yacc.c  */
#line 1599 "compiler2/Grammar.cc"
	break;
      case 85: /* "formal" */

/* Line 1009 of yacc.c  */
#line 39 "compiler2/Grammar.y"
	{ delete (yyvaluep->formal); (yyvaluep->formal) = 0; };

/* Line 1009 of yacc.c  */
#line 1608 "compiler2/Grammar.cc"
	break;
      case 88: /* "block" */

/* Line 1009 of yacc.c  */
#line 36 "compiler2/Grammar.y"
	{ delete (yyvaluep->statement); (yyvaluep->statement) = 0; };

/* Line 1009 of yacc.c  */
#line 1617 "compiler2/Grammar.cc"
	break;
      case 89: /* "statement_list" */

/* Line 1009 of yacc.c  */
#line 36 "compiler2/Grammar.y"
	{ delete (yyvaluep->statement); (yyvaluep->statement) = 0; };

/* Line 1009 of yacc.c  */
#line 1626 "compiler2/Grammar.cc"
	break;
      case 90: /* "statement" */

/* Line 1009 of yacc.c  */
#line 36 "compiler2/Grammar.y"
	{ delete (yyvaluep->statement); (yyvaluep->statement) = 0; };

/* Line 1009 of yacc.c  */
#line 1635 "compiler2/Grammar.cc"
	break;
      case 91: /* "storage" */

/* Line 1009 of yacc.c  */
#line 34 "compiler2/Grammar.y"
	{ delete (yyvaluep->expression); (yyvaluep->expression) = 0; };

/* Line 1009 of yacc.c  */
#line 1644 "compiler2/Grammar.cc"
	break;
      case 92: /* "assignment" */

/* Line 1009 of yacc.c  */
#line 34 "compiler2/Grammar.y"
	{ delete (yyvaluep->expression); (yyvaluep->expression) = 0; };

/* Line 1009 of yacc.c  */
#line 1653 "compiler2/Grammar.cc"
	break;
      case 93: /* "expression" */

/* Line 1009 of yacc.c  */
#line 34 "compiler2/Grammar.y"
	{ delete (yyvaluep->expression); (yyvaluep->expression) = 0; };

/* Line 1009 of yacc.c  */
#line 1662 "compiler2/Grammar.cc"
	break;
      case 94: /* "expression_list" */

/* Line 1009 of yacc.c  */
#line 34 "compiler2/Grammar.y"
	{ delete (yyvaluep->expression); (yyvaluep->expression) = 0; };

/* Line 1009 of yacc.c  */
#line 1671 "compiler2/Grammar.cc"
	break;
      case 95: /* "when_list" */

/* Line 1009 of yacc.c  */
#line 36 "compiler2/Grammar.y"
	{ delete (yyvaluep->statement); (yyvaluep->statement) = 0; };

/* Line 1009 of yacc.c  */
#line 1680 "compiler2/Grammar.cc"
	break;
      case 96: /* "when" */

/* Line 1009 of yacc.c  */
#line 36 "compiler2/Grammar.y"
	{ delete (yyvaluep->statement); (yyvaluep->statement) = 0; };

/* Line 1009 of yacc.c  */
#line 1689 "compiler2/Grammar.cc"
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
int yyparse (Parser *parser, void *scanner);
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
yyparse (Parser *parser, void *scanner)
#else
int
yyparse (parser, scanner)
    Parser *parser;
    void *scanner;
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
    YYLTYPE yyerror_range[3];

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

#if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
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

/* Line 1464 of yacc.c  */
#line 88 "compiler2/Grammar.y"
    {
        parser->environment()->unit(new Class((yyloc), (yyvsp[(2) - (5)].name), (yyvsp[(4) - (5)].feature))); 
    ;}
    break;

  case 3:

/* Line 1464 of yacc.c  */
#line 91 "compiler2/Grammar.y"
    {
        parser->environment()->unit(new Interface((yyloc), (yyvsp[(2) - (5)].name), (yyvsp[(4) - (5)].feature))); 
    ;}
    break;

  case 4:

/* Line 1464 of yacc.c  */
#line 94 "compiler2/Grammar.y"
    {
        parser->environment()->unit(new Structure((yyloc), (yyvsp[(2) - (5)].name), (yyvsp[(4) - (5)].feature)));    
    ;}
    break;

  case 5:

/* Line 1464 of yacc.c  */
#line 97 "compiler2/Grammar.y"
    {
        parser->environment()->unit(new Module((yyloc), (yyvsp[(2) - (5)].name), (yyvsp[(4) - (5)].feature)));    
    ;}
    break;

  case 6:

/* Line 1464 of yacc.c  */
#line 100 "compiler2/Grammar.y"
    { 
		yyerror(&(yyloc), parser, scanner, "Input file is empty"); 
		YYERROR;
	;}
    break;

  case 7:

/* Line 1464 of yacc.c  */
#line 104 "compiler2/Grammar.y"
    { ;}
    break;

  case 8:

/* Line 1464 of yacc.c  */
#line 108 "compiler2/Grammar.y"
    { 
        (yyval.feature) = (yyvsp[(1) - (2)].feature);
        (yyval.feature)->next((yyvsp[(2) - (2)].feature));
    ;}
    break;

  case 9:

/* Line 1464 of yacc.c  */
#line 112 "compiler2/Grammar.y"
    {
        (yyval.feature) = (yyvsp[(1) - (1)].feature);
    ;}
    break;

  case 10:

/* Line 1464 of yacc.c  */
#line 118 "compiler2/Grammar.y"
    { (yyval.feature) = (yyvsp[(1) - (1)].feature); ;}
    break;

  case 11:

/* Line 1464 of yacc.c  */
#line 119 "compiler2/Grammar.y"
    { (yyval.feature) = (yyvsp[(1) - (1)].feature); ;}
    break;

  case 12:

/* Line 1464 of yacc.c  */
#line 120 "compiler2/Grammar.y"
    { (yyval.feature) = (yyvsp[(1) - (1)].feature); ;}
    break;

  case 13:

/* Line 1464 of yacc.c  */
#line 121 "compiler2/Grammar.y"
    { (yyval.feature) = (yyvsp[(1) - (1)].feature); ;}
    break;

  case 14:

/* Line 1464 of yacc.c  */
#line 122 "compiler2/Grammar.y"
    { (yyval.feature) = (yyvsp[(1) - (1)].feature); ;}
    break;

  case 15:

/* Line 1464 of yacc.c  */
#line 123 "compiler2/Grammar.y"
    { (yyval.feature) = (yyvsp[(1) - (1)].feature); ;}
    break;

  case 16:

/* Line 1464 of yacc.c  */
#line 124 "compiler2/Grammar.y"
    { (yyval.feature) = (yyvsp[(1) - (1)].feature); ;}
    break;

  case 17:

/* Line 1464 of yacc.c  */
#line 125 "compiler2/Grammar.y"
    { (yyval.feature) = (yyvsp[(1) - (1)].feature); ;}
    break;

  case 18:

/* Line 1464 of yacc.c  */
#line 129 "compiler2/Grammar.y"
    { 
		(yyval.feature) = new Import((yyloc), (yyvsp[(2) - (3)].name));
	;}
    break;

  case 19:

/* Line 1464 of yacc.c  */
#line 135 "compiler2/Grammar.y"
    { 
		(yyval.feature) = new Define((yyloc), (yyvsp[(3) - (4)].name), (yyvsp[(2) - (4)].name));
	;}
    break;

  case 20:

/* Line 1464 of yacc.c  */
#line 141 "compiler2/Grammar.y"
    {
		// TODO: Set symbol table for class-level
		(yyval.feature) = new Attribute((yyloc), (yyvsp[(2) - (7)].name), (yyvsp[(4) - (7)].name), (yyvsp[(6) - (7)].expression));
    ;}
    break;

  case 21:

/* Line 1464 of yacc.c  */
#line 145 "compiler2/Grammar.y"
    {
		(yyval.feature) = new Attribute((yyloc), (yyvsp[(2) - (5)].name), (yyvsp[(4) - (5)].name), 0);
    ;}
    break;

  case 22:

/* Line 1464 of yacc.c  */
#line 151 "compiler2/Grammar.y"
    { 
        Name* name = parser->environment()->name("@init");
        (yyval.feature) = new Function((yyloc), name, (yyvsp[(3) - (4)].formal), 0, (yyvsp[(4) - (4)].statement));
	;}
    break;

  case 23:

/* Line 1464 of yacc.c  */
#line 158 "compiler2/Grammar.y"
    { 
        Name* name = parser->environment()->name("@destroy");
        (yyval.feature) = new Function((yyloc), name, 0, 0, (yyvsp[(5) - (5)].statement));
	;}
    break;

  case 24:

/* Line 1464 of yacc.c  */
#line 165 "compiler2/Grammar.y"
    {
        (yyval.feature) = new Function((yyloc), (yyvsp[(2) - (7)].name), (yyvsp[(3) - (7)].formal), (yyvsp[(5) - (7)].name), (yyvsp[(7) - (7)].statement));
    ;}
    break;

  case 25:

/* Line 1464 of yacc.c  */
#line 168 "compiler2/Grammar.y"
    {
        (yyval.feature) = new Function((yyloc), (yyvsp[(2) - (5)].name), (yyvsp[(3) - (5)].formal), 0, (yyvsp[(5) - (5)].statement));
	;}
    break;

  case 26:

/* Line 1464 of yacc.c  */
#line 174 "compiler2/Grammar.y"
    {
        (yyval.feature) = new Function((yyloc), (yyvsp[(2) - (7)].name), (yyvsp[(3) - (7)].formal), (yyvsp[(5) - (7)].name), 0);
	;}
    break;

  case 27:

/* Line 1464 of yacc.c  */
#line 177 "compiler2/Grammar.y"
    {
        (yyval.feature) = new Function((yyloc), (yyvsp[(2) - (5)].name), (yyvsp[(3) - (5)].formal), 0, 0);
	;}
    break;

  case 28:

/* Line 1464 of yacc.c  */
#line 183 "compiler2/Grammar.y"
    {
        (yyval.feature) = new Function((yyloc), (yyvsp[(2) - (8)].name), (yyvsp[(3) - (8)].formal), (yyvsp[(5) - (8)].name), 0);
	;}
    break;

  case 29:

/* Line 1464 of yacc.c  */
#line 186 "compiler2/Grammar.y"
    {
        (yyval.feature) = new Function((yyloc), (yyvsp[(2) - (6)].name), (yyvsp[(3) - (6)].formal), 0, 0);
	;}
    break;

  case 30:

/* Line 1464 of yacc.c  */
#line 192 "compiler2/Grammar.y"
    { (yyval.formal) = (yyvsp[(2) - (3)].formal); ;}
    break;

  case 31:

/* Line 1464 of yacc.c  */
#line 193 "compiler2/Grammar.y"
    { (yyval.formal) = 0; ;}
    break;

  case 32:

/* Line 1464 of yacc.c  */
#line 197 "compiler2/Grammar.y"
    { 
        (yyval.formal) = (yyvsp[(1) - (3)].formal);
        (yyval.formal)->next((yyvsp[(3) - (3)].formal));
	;}
    break;

  case 33:

/* Line 1464 of yacc.c  */
#line 201 "compiler2/Grammar.y"
    { 
        (yyval.formal) = (yyvsp[(1) - (1)].formal);
	;}
    break;

  case 34:

/* Line 1464 of yacc.c  */
#line 207 "compiler2/Grammar.y"
    {
        (yyval.formal) = new Formal((yyloc), (yyvsp[(1) - (3)].name), (yyvsp[(3) - (3)].name));
    ;}
    break;

  case 35:

/* Line 1464 of yacc.c  */
#line 212 "compiler2/Grammar.y"
    { (yyval.flag) = 0; ;}
    break;

  case 36:

/* Line 1464 of yacc.c  */
#line 213 "compiler2/Grammar.y"
    { (yyval.flag) = 0; ;}
    break;

  case 37:

/* Line 1464 of yacc.c  */
#line 214 "compiler2/Grammar.y"
    { (yyval.flag) = 0; ;}
    break;

  case 38:

/* Line 1464 of yacc.c  */
#line 215 "compiler2/Grammar.y"
    { (yyval.flag) = 0; ;}
    break;

  case 39:

/* Line 1464 of yacc.c  */
#line 216 "compiler2/Grammar.y"
    { (yyval.flag) = 0; ;}
    break;

  case 40:

/* Line 1464 of yacc.c  */
#line 217 "compiler2/Grammar.y"
    { (yyval.flag) = 0; ;}
    break;

  case 41:

/* Line 1464 of yacc.c  */
#line 221 "compiler2/Grammar.y"
    { (yyval.name) = (yyvsp[(1) - (1)].name); ;}
    break;

  case 42:

/* Line 1464 of yacc.c  */
#line 225 "compiler2/Grammar.y"
    { 
		(yyval.statement) = new Block((yyloc), (yyvsp[(2) - (3)].statement)); 
	;}
    break;

  case 43:

/* Line 1464 of yacc.c  */
#line 228 "compiler2/Grammar.y"
    { 
        (yyval.statement) = new Return((yyloc), (yyvsp[(3) - (4)].expression)); 
    ;}
    break;

  case 44:

/* Line 1464 of yacc.c  */
#line 231 "compiler2/Grammar.y"
    { 
        (yyval.statement) = new Return((yyloc), 0); 
    ;}
    break;

  case 45:

/* Line 1464 of yacc.c  */
#line 234 "compiler2/Grammar.y"
    { 
		(yyval.statement) = new Simple((yyloc), (yyvsp[(2) - (3)].expression)); 
    ;}
    break;

  case 46:

/* Line 1464 of yacc.c  */
#line 240 "compiler2/Grammar.y"
    { 
		(yyval.statement) = (yyvsp[(1) - (2)].statement);
        (yyval.statement)->next((yyvsp[(2) - (2)].statement));
	;}
    break;

  case 47:

/* Line 1464 of yacc.c  */
#line 244 "compiler2/Grammar.y"
    {
        (yyval.statement) = (yyvsp[(1) - (1)].statement);
    ;}
    break;

  case 48:

/* Line 1464 of yacc.c  */
#line 247 "compiler2/Grammar.y"
    { 
        (yyval.statement) = (yyvsp[(2) - (2)].statement); 
    ;}
    break;

  case 49:

/* Line 1464 of yacc.c  */
#line 254 "compiler2/Grammar.y"
    {
		(yyval.statement) = new For((yyloc), (yyvsp[(2) - (7)].name), (yyvsp[(4) - (7)].name), (yyvsp[(6) - (7)].expression), (yyvsp[(7) - (7)].statement));
	;}
    break;

  case 50:

/* Line 1464 of yacc.c  */
#line 257 "compiler2/Grammar.y"
    {
        // TODO: FIX UNTIL
        Name* op = parser->environment()->name("!");
		(yyval.statement) = new While((yyloc), new Unary((yyloc), op, (yyvsp[(2) - (3)].expression)), (yyvsp[(3) - (3)].statement));
        
	;}
    break;

  case 51:

/* Line 1464 of yacc.c  */
#line 263 "compiler2/Grammar.y"
    {
		(yyval.statement) = new While((yyloc), (yyvsp[(2) - (3)].expression), (yyvsp[(3) - (3)].statement));
	;}
    break;

  case 52:

/* Line 1464 of yacc.c  */
#line 266 "compiler2/Grammar.y"
    {
        (yyval.statement) = new Case((yyloc), (yyvsp[(2) - (5)].expression), static_cast<When*>((yyvsp[(4) - (5)].statement)));
    ;}
    break;

  case 53:

/* Line 1464 of yacc.c  */
#line 269 "compiler2/Grammar.y"
    { 
		(yyval.statement) = new Variable((yyloc), (yyvsp[(2) - (4)].name), (yyvsp[(4) - (4)].name), 0); 
	;}
    break;

  case 54:

/* Line 1464 of yacc.c  */
#line 272 "compiler2/Grammar.y"
    { 
        (yyval.statement) = new Variable((yyloc), (yyvsp[(2) - (7)].name), (yyvsp[(4) - (7)].name), (yyvsp[(6) - (7)].expression)); 
	;}
    break;

  case 55:

/* Line 1464 of yacc.c  */
#line 275 "compiler2/Grammar.y"
    { 
        (yyval.statement) = new Return((yyloc), (yyvsp[(2) - (3)].expression));
    ;}
    break;

  case 56:

/* Line 1464 of yacc.c  */
#line 278 "compiler2/Grammar.y"
    { 
        (yyval.statement) = new Return((yyloc), 0);
    ;}
    break;

  case 57:

/* Line 1464 of yacc.c  */
#line 281 "compiler2/Grammar.y"
    { 
		(yyval.statement) = new Simple((yyloc), (yyvsp[(1) - (2)].expression)); 
	;}
    break;

  case 58:

/* Line 1464 of yacc.c  */
#line 284 "compiler2/Grammar.y"
    { 
		(yyval.statement) = new Simple((yyloc), (yyvsp[(1) - (2)].expression)); 
	;}
    break;

  case 59:

/* Line 1464 of yacc.c  */
#line 287 "compiler2/Grammar.y"
    {
		(yyval.statement) = new Conditional((yyloc), (yyvsp[(2) - (3)].expression), (yyvsp[(3) - (3)].statement), 0);
	;}
    break;

  case 60:

/* Line 1464 of yacc.c  */
#line 290 "compiler2/Grammar.y"
    {
        (yyval.statement) = new Conditional((yyloc), (yyvsp[(2) - (5)].expression), (yyvsp[(3) - (5)].statement), (yyvsp[(5) - (5)].statement));
    ;}
    break;

  case 61:

/* Line 1464 of yacc.c  */
#line 296 "compiler2/Grammar.y"
    { 
		/* Member variable access */
		(yyval.expression) = new Member((yyloc), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].name)); 
	;}
    break;

  case 62:

/* Line 1464 of yacc.c  */
#line 300 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Index((yyloc), (yyvsp[(1) - (4)].expression), (yyvsp[(3) - (4)].expression));
	;}
    break;

  case 63:

/* Line 1464 of yacc.c  */
#line 305 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("="), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
        (yyval.expression) = new Binary((yyloc), parser->environment()->name("="), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
	;}
    break;

  case 64:

/* Line 1464 of yacc.c  */
#line 309 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("*="), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 65:

/* Line 1464 of yacc.c  */
#line 312 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("/="), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 66:

/* Line 1464 of yacc.c  */
#line 315 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("%="), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 67:

/* Line 1464 of yacc.c  */
#line 318 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("-="), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 68:

/* Line 1464 of yacc.c  */
#line 321 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("+="), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 69:

/* Line 1464 of yacc.c  */
#line 324 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("&="), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 70:

/* Line 1464 of yacc.c  */
#line 327 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("|="), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 71:

/* Line 1464 of yacc.c  */
#line 334 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("?"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
	;}
    break;

  case 72:

/* Line 1464 of yacc.c  */
#line 337 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("||"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 73:

/* Line 1464 of yacc.c  */
#line 340 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("&&"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 74:

/* Line 1464 of yacc.c  */
#line 343 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("|"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 75:

/* Line 1464 of yacc.c  */
#line 346 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("^"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 76:

/* Line 1464 of yacc.c  */
#line 349 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("&"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 77:

/* Line 1464 of yacc.c  */
#line 352 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("=="), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 78:

/* Line 1464 of yacc.c  */
#line 355 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("!="), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 79:

/* Line 1464 of yacc.c  */
#line 358 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name(">"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 80:

/* Line 1464 of yacc.c  */
#line 361 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("<"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 81:

/* Line 1464 of yacc.c  */
#line 364 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name(">="), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 82:

/* Line 1464 of yacc.c  */
#line 367 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("<="), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 83:

/* Line 1464 of yacc.c  */
#line 370 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("<<"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 84:

/* Line 1464 of yacc.c  */
#line 373 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name(">>"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 85:

/* Line 1464 of yacc.c  */
#line 376 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("+"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 86:

/* Line 1464 of yacc.c  */
#line 379 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("-"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 87:

/* Line 1464 of yacc.c  */
#line 382 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("*"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 88:

/* Line 1464 of yacc.c  */
#line 385 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("/"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 89:

/* Line 1464 of yacc.c  */
#line 388 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("%"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 90:

/* Line 1464 of yacc.c  */
#line 391 "compiler2/Grammar.y"
    { 
        (yyval.expression) = new Unary((yyloc), parser->environment()->name("!"), (yyvsp[(2) - (2)].expression)); 
    ;}
    break;

  case 91:

/* Line 1464 of yacc.c  */
#line 394 "compiler2/Grammar.y"
    { 
        (yyval.expression) = new Unary((yyloc), parser->environment()->name("~"), (yyvsp[(2) - (2)].expression)); 
    ;}
    break;

  case 92:

/* Line 1464 of yacc.c  */
#line 397 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Unary((yyloc), parser->environment()->name("++"), (yyvsp[(1) - (2)].expression)); 
	;}
    break;

  case 93:

/* Line 1464 of yacc.c  */
#line 400 "compiler2/Grammar.y"
    { 
		(yyval.expression) = new Unary((yyloc), parser->environment()->name("--"), (yyvsp[(1) - (2)].expression)); 
	;}
    break;

  case 94:

/* Line 1464 of yacc.c  */
#line 403 "compiler2/Grammar.y"
    {
		(yyval.expression) = new Call((yyloc), (yyvsp[(1) - (4)].name), (yyvsp[(3) - (4)].expression));
    ;}
    break;

  case 95:

/* Line 1464 of yacc.c  */
#line 406 "compiler2/Grammar.y"
    {
		/* Call on an object expression */
		(yyval.expression) = new Call((yyloc), (yyvsp[(1) - (3)].name), 0);
	;}
    break;

  case 96:

/* Line 1464 of yacc.c  */
#line 410 "compiler2/Grammar.y"
    {
        (yyvsp[(1) - (6)].expression)->next((yyvsp[(5) - (6)].expression));
        (yyval.expression) = new Dispatch((yyloc), (yyvsp[(3) - (6)].name), (yyvsp[(1) - (6)].expression));
    ;}
    break;

  case 97:

/* Line 1464 of yacc.c  */
#line 414 "compiler2/Grammar.y"
    {
        (yyval.expression) = new Dispatch((yyloc), (yyvsp[(3) - (5)].name), (yyvsp[(1) - (5)].expression));
    ;}
    break;

  case 98:

/* Line 1464 of yacc.c  */
#line 417 "compiler2/Grammar.y"
    { (yyval.expression) = (yyvsp[(1) - (1)].expression); ;}
    break;

  case 99:

/* Line 1464 of yacc.c  */
#line 418 "compiler2/Grammar.y"
    { (yyval.expression) = (yyvsp[(2) - (3)].expression); ;}
    break;

  case 100:

/* Line 1464 of yacc.c  */
#line 419 "compiler2/Grammar.y"
    { (yyval.expression) = (yyvsp[(1) - (1)].expression); ;}
    break;

  case 101:

/* Line 1464 of yacc.c  */
#line 420 "compiler2/Grammar.y"
    { (yyval.expression) = (yyvsp[(1) - (1)].expression); ;}
    break;

  case 102:

/* Line 1464 of yacc.c  */
#line 421 "compiler2/Grammar.y"
    { (yyval.expression) = new Identifier((yyloc), (yyvsp[(1) - (1)].name)); ;}
    break;

  case 103:

/* Line 1464 of yacc.c  */
#line 425 "compiler2/Grammar.y"
    { 
		(yyval.expression) = (yyvsp[(1) - (3)].expression);
        (yyval.expression)->next((yyvsp[(3) - (3)].expression));
	;}
    break;

  case 104:

/* Line 1464 of yacc.c  */
#line 429 "compiler2/Grammar.y"
    { (yyval.expression) = (yyvsp[(1) - (1)].expression); ;}
    break;

  case 105:

/* Line 1464 of yacc.c  */
#line 434 "compiler2/Grammar.y"
    {
        (yyval.statement) = (yyvsp[(1) - (2)].statement);
        (yyval.statement)->next((yyvsp[(2) - (2)].statement));
    ;}
    break;

  case 106:

/* Line 1464 of yacc.c  */
#line 438 "compiler2/Grammar.y"
    {
        (yyval.statement) = (yyvsp[(1) - (1)].statement);
    ;}
    break;

  case 107:

/* Line 1464 of yacc.c  */
#line 444 "compiler2/Grammar.y"
    {
        (yyval.statement) = new When((yyloc), (yyvsp[(2) - (5)].name), (yyvsp[(4) - (5)].name), (yyvsp[(5) - (5)].statement));
    ;}
    break;



/* Line 1464 of yacc.c  */
#line 2943 "compiler2/Grammar.cc"
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

  yyerror_range[1] = yylloc;

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

  yyerror_range[1] = yylsp[1-yylen];
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp, parser, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
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



