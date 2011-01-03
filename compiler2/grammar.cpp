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
#line 1 "compiler2/grammar.y"

#include "parser.hpp"
#include "expression.hpp"
#include "statement.hpp"
#include "formal.hpp"
#include "feature.hpp"
#include "unit.hpp"
#include "type.hpp"

#define YYSTYPE ParseNode
#define YYLTYPE Location
#define YY_EXTRA_TYPE Parser*
#define YY_NO_INPUT
#define YYERROR_VERBOSE
int yylex(ParseNode *node, Location *loc, void *scanner);
void yyerror(Location *loc, Parser *parser, void *scanner, const char *message);



/* Line 189 of yacc.c  */
#line 92 "compiler2/grammar.cpp"

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

/* Line 214 of yacc.c  */
#line 20 "compiler2/grammar.y"
 Expression* expression; 

/* Line 214 of yacc.c  */
#line 21 "compiler2/grammar.y"
 Statement* statement; 

/* Line 214 of yacc.c  */
#line 22 "compiler2/grammar.y"
 Name* identifier; 

/* Line 214 of yacc.c  */
#line 23 "compiler2/grammar.y"
 Unit* unit; 

/* Line 214 of yacc.c  */
#line 24 "compiler2/grammar.y"
 Feature* feature; 

/* Line 214 of yacc.c  */
#line 25 "compiler2/grammar.y"
 Formal* formal; 

/* Line 214 of yacc.c  */
#line 26 "compiler2/grammar.y"
 Type* type; 

/* Line 214 of yacc.c  */
#line 27 "compiler2/grammar.y"
 Generic* generic; 

/* Line 214 of yacc.c  */
#line 28 "compiler2/grammar.y"
 int null; 

/* Line 214 of yacc.c  */
#line 29 "compiler2/grammar.y"
 int flag; 


/* Line 214 of yacc.c  */
#line 217 "compiler2/grammar.cpp"
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
#line 242 "compiler2/grammar.cpp"

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
#define YYFINAL  12
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   837

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  73
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  28
/* YYNRULES -- Number of rules.  */
#define YYNRULES  113
/* YYNRULES -- Number of states.  */
#define YYNSTATES  241

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   304

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     7,     2,     2,     2,    11,    22,     2,
      69,    70,     9,    12,    71,    13,     3,    10,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    68,     2,
      16,    27,    17,    35,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     4,     2,    72,    24,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    66,    23,    67,     8,     2,     2,     2,
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
      61,    62,    63,    64,    65
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
     158,   159,   163,   170,   172,   177,   181,   183,   185,   189,
     194,   198,   202,   205,   207,   210,   218,   222,   226,   232,
     237,   245,   249,   252,   255,   258,   262,   268,   272,   277,
     281,   285,   289,   293,   297,   301,   305,   309,   313,   317,
     321,   325,   329,   333,   337,   341,   345,   349,   353,   357,
     361,   365,   369,   373,   377,   381,   385,   388,   391,   394,
     397,   402,   406,   413,   419,   421,   425,   427,   429,   431,
     435,   437,   440,   442
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      74,     0,    -1,    44,    89,    66,    75,    67,    -1,    45,
      89,    66,    75,    66,    -1,    46,    89,    66,    75,    67,
      -1,    47,    89,    66,    75,    67,    -1,    -1,     1,    -1,
      76,    75,    -1,    76,    -1,    77,    -1,    78,    -1,    79,
      -1,    80,    -1,    81,    -1,    82,    -1,    84,    -1,    83,
      -1,    48,    89,    53,    -1,    49,    89,    37,    53,    -1,
      50,    36,    68,    89,    27,    97,    53,    -1,    50,    36,
      68,    89,    53,    -1,    49,    51,    85,    92,    -1,    49,
      52,    69,    70,    92,    -1,    49,    36,    85,    68,    89,
      88,    92,    -1,    49,    36,    85,    88,    92,    -1,    49,
      36,    85,    68,    89,    88,    53,    -1,    49,    36,    85,
      88,    53,    -1,    49,    36,    85,    68,    89,    88,    43,
      53,    -1,    49,    36,    85,    88,    43,    53,    -1,    69,
      86,    70,    -1,    69,    70,    -1,    87,    71,    86,    -1,
      87,    -1,    36,    68,    89,    -1,    40,    -1,    41,    -1,
      42,    -1,    40,    42,    -1,    41,    42,    -1,    -1,    89,
      65,    37,    -1,    89,    65,    37,     4,    90,    72,    -1,
      37,    -1,    37,     4,    90,    72,    -1,    91,    71,    90,
      -1,    91,    -1,    89,    -1,    66,    93,    67,    -1,    63,
      62,    97,    53,    -1,    63,    62,    53,    -1,    63,    97,
      53,    -1,    94,    93,    -1,    94,    -1,     1,    93,    -1,
      61,    36,    68,    89,    64,    97,    92,    -1,    58,    97,
      92,    -1,    56,    97,    92,    -1,    55,    97,    66,    99,
      67,    -1,    50,    36,    68,    89,    -1,    50,    36,    68,
      89,    27,    97,    53,    -1,    62,    97,    53,    -1,    62,
      53,    -1,    97,    53,    -1,    96,    53,    -1,    59,    97,
      92,    -1,    59,    97,    92,    57,    92,    -1,    97,     3,
      36,    -1,    97,     4,    97,    72,    -1,    95,    27,    97,
      -1,    95,    30,    97,    -1,    95,    29,    97,    -1,    95,
      28,    97,    -1,    95,    31,    97,    -1,    95,    32,    97,
      -1,    95,    34,    97,    -1,    95,    33,    97,    -1,    97,
      35,    97,    -1,    97,    26,    97,    -1,    97,    25,    97,
      -1,    97,    23,    97,    -1,    97,    24,    97,    -1,    97,
      22,    97,    -1,    97,    21,    97,    -1,    97,    20,    97,
      -1,    97,    17,    97,    -1,    97,    16,    97,    -1,    97,
      18,    97,    -1,    97,    19,    97,    -1,    97,    15,    97,
      -1,    97,    14,    97,    -1,    97,    12,    97,    -1,    97,
      13,    97,    -1,    97,     9,    97,    -1,    97,    10,    97,
      -1,    97,    11,    97,    -1,     7,    97,    -1,     8,    97,
      -1,    97,     6,    -1,    97,     5,    -1,    36,    69,    98,
      70,    -1,    36,    69,    70,    -1,    97,     3,    36,    69,
      98,    70,    -1,    97,     3,    36,    69,    70,    -1,    95,
      -1,    69,    97,    70,    -1,    38,    -1,    39,    -1,    36,
      -1,    97,    71,    98,    -1,    97,    -1,   100,    99,    -1,
     100,    -1,    54,    36,    68,    89,    92,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    95,    95,    98,   101,   104,   107,   111,   115,   119,
     125,   126,   127,   128,   129,   130,   131,   132,   136,   142,
     148,   152,   158,   165,   172,   175,   181,   184,   190,   193,
     199,   200,   204,   208,   214,   219,   220,   221,   222,   223,
     224,   228,   231,   234,   237,   243,   247,   253,   257,   260,
     263,   266,   272,   276,   279,   286,   289,   295,   298,   301,
     304,   307,   310,   313,   316,   319,   322,   328,   332,   337,
     341,   344,   347,   350,   353,   356,   359,   366,   369,   372,
     375,   378,   381,   384,   387,   390,   393,   396,   399,   402,
     405,   408,   411,   414,   417,   420,   423,   426,   429,   432,
     435,   438,   442,   446,   449,   450,   451,   452,   453,   457,
     461,   466,   470,   476
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
  "TYPE", "STRING", "NUMBER", "PUBLIC", "PRIVATE", "STATIC", "NATIVE",
  "CLASS", "INTERFACE", "STRUCT", "MODULE", "IMPORT", "DEF", "VAR", "INIT",
  "DESTROY", "SEPARATOR", "WHEN", "CASE", "WHILE", "ELSE", "UNTIL", "IF",
  "DO", "FOR", "RETURN", "RIGHT_ARROW", "LEFT_ARROW", "SCOPE", "'{'",
  "'}'", "':'", "'('", "')'", "','", "']'", "$accept", "unit",
  "feature_list", "feature", "import", "define", "attribute",
  "constructor", "destructor", "function", "prototype", "native",
  "formal_signature", "formal_list", "formal", "modifiers", "type",
  "generic_list", "generic", "block", "statement_list", "statement",
  "storage", "assignment", "expression", "expression_list", "when_list",
  "when", 0
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
     299,   300,   301,   302,   303,   304,   123,   125,    58,    40,
      41,    44,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    73,    74,    74,    74,    74,    74,    74,    75,    75,
      76,    76,    76,    76,    76,    76,    76,    76,    77,    78,
      79,    79,    80,    81,    82,    82,    83,    83,    84,    84,
      85,    85,    86,    86,    87,    88,    88,    88,    88,    88,
      88,    89,    89,    89,    89,    90,    90,    91,    92,    92,
      92,    92,    93,    93,    93,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    95,    95,    96,
      96,    96,    96,    96,    96,    96,    96,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    98,
      98,    99,    99,   100
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     5,     5,     5,     5,     0,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     4,
       7,     5,     4,     5,     7,     5,     7,     5,     8,     6,
       3,     2,     3,     1,     3,     1,     1,     1,     2,     2,
       0,     3,     6,     1,     4,     3,     1,     1,     3,     4,
       3,     3,     2,     1,     2,     7,     3,     3,     5,     4,
       7,     3,     2,     2,     2,     3,     5,     3,     4,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     2,
       4,     3,     6,     5,     1,     3,     1,     1,     1,     3,
       1,     2,     1,     5
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     7,     0,     0,     0,     0,     0,    43,     0,     0,
       0,     0,     1,     0,     0,     0,     0,     0,     0,    47,
       0,    46,    41,     0,     0,     0,     0,     9,    10,    11,
      12,    13,    14,    15,    17,    16,     0,     0,     0,    44,
       0,     0,     0,     0,     0,     0,     0,     0,     2,     8,
       3,     4,     5,    45,     0,    18,     0,    40,     0,     0,
       0,     0,    42,     0,    31,     0,    33,    35,    36,    37,
       0,     0,     0,     0,    22,     0,    19,     0,     0,    30,
       0,    38,    39,    40,     0,    27,    25,     0,     0,   108,
     106,   107,     0,     0,   104,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   104,     0,     0,    23,
       0,    21,    34,    32,     0,    29,    96,    97,     0,    50,
       0,     0,     0,     0,    99,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,    54,     0,     0,     0,
       0,     0,     0,    62,     0,    48,    52,     0,     0,     0,
       0,     0,     0,     0,     0,    64,    63,     0,     0,    26,
      24,   101,   110,     0,    49,   105,    67,     0,    93,    94,
      95,    91,    92,    90,    89,    86,    85,    87,    88,    84,
      83,    82,    80,    81,    79,    78,    77,     0,     0,    57,
      56,    65,     0,    61,    69,    72,    71,    70,    73,    74,
      76,    75,    20,    28,     0,   100,     0,    68,    59,     0,
       0,   112,     0,     0,   109,   103,     0,     0,     0,    58,
     111,    66,     0,   102,     0,     0,     0,    60,     0,    55,
     113
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     6,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    57,    65,    66,    71,    19,    20,    21,    74,
     104,   105,    94,   107,   108,   173,   220,   221
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -202
static const yytype_int16 yypact[] =
{
      66,  -202,   -28,   -28,   -28,   -28,    28,    31,   -13,    49,
      92,    95,  -202,   -28,    19,    21,    21,    21,    21,    -7,
      -8,     8,    77,   -28,    80,   115,    86,    21,  -202,  -202,
    -202,  -202,  -202,  -202,  -202,  -202,    90,    96,   100,  -202,
     -28,   -28,   -34,    99,    99,   101,   -27,   105,  -202,  -202,
    -202,  -202,  -202,  -202,   112,  -202,   -31,   -17,   -33,   116,
     132,   -28,  -202,   119,  -202,   118,   122,   148,   159,  -202,
     -28,   109,   378,   229,  -202,   -33,  -202,   -21,   -28,  -202,
     163,  -202,  -202,   -24,   153,  -202,  -202,   392,   392,   138,
    -202,  -202,   383,   392,  -202,   459,   229,   191,   392,   392,
     392,   392,   195,   388,   165,   190,   468,   182,   504,  -202,
     392,  -202,    -7,  -202,   111,  -202,   756,   756,     4,  -202,
     549,   290,   202,   392,  -202,  -202,   392,   392,   392,   392,
     392,   392,   392,   392,   392,   392,   392,   392,   392,   392,
     392,   392,   392,   392,   392,  -202,  -202,   171,   352,   317,
     317,   317,   179,  -202,   594,  -202,  -202,   392,   392,   392,
     392,   392,   392,   392,   392,  -202,  -202,   639,   197,  -202,
    -202,  -202,   199,   184,  -202,  -202,   186,   124,   780,   780,
     780,   802,   802,   385,   385,   251,   251,   251,   251,   157,
     157,   218,     1,     1,    -6,   221,  -202,   -28,   204,  -202,
    -202,   203,   -28,  -202,   729,   729,   729,   729,   729,   729,
     729,   729,  -202,  -202,   392,  -202,   343,  -202,   -20,   225,
     196,   204,   -33,    22,  -202,  -202,   192,   392,   198,  -202,
    -202,  -202,   392,  -202,   684,   -28,   317,  -202,    12,  -202,
    -202
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -202,  -202,    91,  -202,  -202,  -202,  -202,  -202,  -202,  -202,
    -202,  -202,   220,   189,  -202,   200,    -2,   125,  -202,   -67,
     -48,  -202,   -59,  -202,   -38,  -201,    57,  -202
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -54
static const yytype_int16 yytable[] =
{
       8,     9,    10,    11,    86,    63,   110,   227,   109,     7,
      60,    87,    88,   224,   106,   226,    67,    68,    69,    55,
     143,    42,    46,    67,    68,    69,   142,   143,    12,   144,
      72,    14,   111,    73,    95,    13,   144,   106,    14,    64,
      89,    14,    90,    91,    14,    14,   106,   170,   146,   116,
     117,    70,    14,    15,   120,   121,    22,   156,    14,    77,
     148,   149,   150,   151,    39,   154,    -6,     1,    83,    23,
      24,    25,   167,    93,   171,    72,   112,    14,    73,    40,
     172,    41,   199,   200,   201,   177,   232,    14,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,    36,    37,    38,
       2,     3,     4,     5,    14,    16,    43,     7,    49,   204,
     205,   206,   207,   208,   209,   210,   211,   122,   123,   124,
     125,    44,    45,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,    47,    84,    48,   168,   231,    50,    14,    17,   144,
      14,    18,    85,    51,   169,    53,    54,    52,    56,   239,
      59,   240,    72,    61,    72,    73,   172,    73,   172,   139,
     140,   141,   142,   143,    62,    76,    75,    78,    79,   234,
      81,    96,   144,    80,   236,   218,   217,    87,    88,    63,
     223,    82,   122,   123,   124,   125,   115,   118,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,    89,   147,    90,    91,
      96,   152,   155,   238,   144,   165,    87,    88,   176,   197,
      97,   140,   141,   142,   143,    98,    99,   202,   100,   101,
     213,   102,   103,   144,   215,   216,   144,   -53,   219,    93,
     222,   228,   233,   229,    58,    89,   235,    90,    91,   113,
     214,   137,   138,   139,   140,   141,   142,   143,   230,    97,
       0,     0,     0,   114,    98,    99,   144,   100,   101,     0,
     102,   103,     0,   122,   123,   124,   125,     0,    93,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,     0,     0,     0,
     122,   123,   124,   125,     0,   144,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,     0,     0,     0,     0,     0,     0,
      87,    88,   144,     0,     0,   122,   123,   124,   125,     0,
     175,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,    89,
      72,    90,    91,    73,     0,    87,    88,   144,     0,     0,
      87,    88,     0,     0,     0,    87,    88,     0,     0,    87,
      88,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,    93,   225,    89,     0,    90,    91,   198,    89,
     144,    90,    91,     0,    89,     0,    90,    91,    89,     0,
      90,    91,     0,     0,     0,     0,   119,     0,     0,     0,
      92,   153,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,    93,     0,     0,     0,     0,    93,     0,     0,
       0,    93,   122,   123,   124,   125,     0,     0,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,     0,     0,     0,     0,
       0,     0,     0,     0,   144,   157,   158,   159,   160,   161,
     162,   163,   164,     0,     0,     0,     0,   122,   123,   124,
     125,     0,   145,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,     0,     0,     0,     0,     0,     0,     0,     0,   144,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   122,   123,   124,   125,     0,   166,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,     0,     0,     0,     0,
       0,     0,     0,     0,   144,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   122,   123,   124,
     125,     0,   174,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,     0,     0,     0,     0,     0,     0,     0,     0,   144,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   122,   123,   124,   125,     0,   203,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,     0,     0,     0,     0,
       0,     0,     0,     0,   144,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   122,   123,   124,
     125,     0,   212,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,     0,     0,     0,     0,     0,     0,     0,     0,   144,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   122,   123,   124,   125,     0,   237,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,     0,     0,     0,     0,
       0,     0,     0,     0,   144,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,     0,     0,     0,     0,     0,     0,     0,
       0,   144,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,     0,     0,     0,
       0,     0,     0,     0,     0,   144,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,     0,
       0,     0,     0,     0,     0,     0,     0,   144
};

static const yytype_int16 yycheck[] =
{
       2,     3,     4,     5,    71,    36,    27,    27,    75,    37,
      37,     7,     8,   214,    73,   216,    40,    41,    42,    53,
      26,    23,    24,    40,    41,    42,    25,    26,     0,    35,
      63,    65,    53,    66,    72,     4,    35,    96,    65,    70,
      36,    65,    38,    39,    65,    65,   105,   114,    96,    87,
      88,    68,    65,    66,    92,    93,    37,   105,    65,    61,
      98,    99,   100,   101,    72,   103,     0,     1,    70,    48,
      49,    50,   110,    69,    70,    63,    78,    65,    66,    71,
     118,     4,   149,   150,   151,   123,    64,    65,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,    16,    17,    18,
      44,    45,    46,    47,    65,    66,    36,    37,    27,   157,
     158,   159,   160,   161,   162,   163,   164,     3,     4,     5,
       6,    51,    52,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    36,    43,    67,    43,   222,    66,    65,    66,    35,
      65,    66,    53,    67,    53,    40,    41,    67,    69,   236,
      69,   238,    63,    68,    63,    66,   214,    66,   216,    22,
      23,    24,    25,    26,    72,    53,    70,    68,    70,   227,
      42,     1,    35,    71,   232,   197,    72,     7,     8,    36,
     202,    42,     3,     4,     5,     6,    53,    69,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    36,    36,    38,    39,
       1,    36,    67,   235,    35,    53,     7,     8,    36,    68,
      50,    23,    24,    25,    26,    55,    56,    68,    58,    59,
      53,    61,    62,    35,    70,    69,    35,    67,    54,    69,
      57,    36,    70,    67,    44,    36,    68,    38,    39,    80,
      71,    20,    21,    22,    23,    24,    25,    26,   221,    50,
      -1,    -1,    -1,    83,    55,    56,    35,    58,    59,    -1,
      61,    62,    -1,     3,     4,     5,     6,    -1,    69,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    35,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
       7,     8,    35,    -1,    -1,     3,     4,     5,     6,    -1,
      70,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    36,
      63,    38,    39,    66,    -1,     7,     8,    35,    -1,    -1,
       7,     8,    -1,    -1,    -1,     7,     8,    -1,    -1,     7,
       8,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    69,    70,    36,    -1,    38,    39,    66,    36,
      35,    38,    39,    -1,    36,    -1,    38,    39,    36,    -1,
      38,    39,    -1,    -1,    -1,    -1,    53,    -1,    -1,    -1,
      62,    53,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      -1,    69,     3,     4,     5,     6,    -1,    -1,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    27,    28,    29,    30,    31,
      32,    33,    34,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    53,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    53,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    53,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    53,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    53,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    53,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    35
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,    44,    45,    46,    47,    74,    37,    89,    89,
      89,    89,     0,     4,    65,    66,    66,    66,    66,    89,
      90,    91,    37,    48,    49,    50,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    75,    75,    75,    72,
      71,     4,    89,    36,    51,    52,    89,    36,    67,    75,
      66,    67,    67,    90,    90,    53,    69,    85,    85,    69,
      37,    68,    72,    36,    70,    86,    87,    40,    41,    42,
      68,    88,    63,    66,    92,    70,    53,    89,    68,    70,
      71,    42,    42,    89,    43,    53,    92,     7,     8,    36,
      38,    39,    62,    69,    95,    97,     1,    50,    55,    56,
      58,    59,    61,    62,    93,    94,    95,    96,    97,    92,
      27,    53,    89,    86,    88,    53,    97,    97,    69,    53,
      97,    97,     3,     4,     5,     6,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    35,    53,    93,    36,    97,    97,
      97,    97,    36,    53,    97,    67,    93,    27,    28,    29,
      30,    31,    32,    33,    34,    53,    53,    97,    43,    53,
      92,    70,    97,    98,    53,    70,    36,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    68,    66,    92,
      92,    92,    68,    53,    97,    97,    97,    97,    97,    97,
      97,    97,    53,    53,    71,    70,    69,    72,    89,    54,
      99,   100,    57,    89,    98,    70,    98,    27,    36,    67,
      99,    92,    64,    70,    97,    68,    97,    53,    89,    92,
      92
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
      case 38: /* "STRING" */

/* Line 1009 of yacc.c  */
#line 37 "compiler2/grammar.y"
	{ delete (yyvaluep->expression); (yyvaluep->expression) = 0; };

/* Line 1009 of yacc.c  */
#line 1511 "compiler2/grammar.cpp"
	break;
      case 39: /* "NUMBER" */

/* Line 1009 of yacc.c  */
#line 37 "compiler2/grammar.y"
	{ delete (yyvaluep->expression); (yyvaluep->expression) = 0; };

/* Line 1009 of yacc.c  */
#line 1520 "compiler2/grammar.cpp"
	break;
      case 75: /* "feature_list" */

/* Line 1009 of yacc.c  */
#line 41 "compiler2/grammar.y"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1009 of yacc.c  */
#line 1529 "compiler2/grammar.cpp"
	break;
      case 76: /* "feature" */

/* Line 1009 of yacc.c  */
#line 41 "compiler2/grammar.y"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1009 of yacc.c  */
#line 1538 "compiler2/grammar.cpp"
	break;
      case 77: /* "import" */

/* Line 1009 of yacc.c  */
#line 41 "compiler2/grammar.y"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1009 of yacc.c  */
#line 1547 "compiler2/grammar.cpp"
	break;
      case 78: /* "define" */

/* Line 1009 of yacc.c  */
#line 41 "compiler2/grammar.y"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1009 of yacc.c  */
#line 1556 "compiler2/grammar.cpp"
	break;
      case 79: /* "attribute" */

/* Line 1009 of yacc.c  */
#line 41 "compiler2/grammar.y"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1009 of yacc.c  */
#line 1565 "compiler2/grammar.cpp"
	break;
      case 80: /* "constructor" */

/* Line 1009 of yacc.c  */
#line 41 "compiler2/grammar.y"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1009 of yacc.c  */
#line 1574 "compiler2/grammar.cpp"
	break;
      case 81: /* "destructor" */

/* Line 1009 of yacc.c  */
#line 41 "compiler2/grammar.y"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1009 of yacc.c  */
#line 1583 "compiler2/grammar.cpp"
	break;
      case 82: /* "function" */

/* Line 1009 of yacc.c  */
#line 41 "compiler2/grammar.y"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1009 of yacc.c  */
#line 1592 "compiler2/grammar.cpp"
	break;
      case 83: /* "prototype" */

/* Line 1009 of yacc.c  */
#line 41 "compiler2/grammar.y"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1009 of yacc.c  */
#line 1601 "compiler2/grammar.cpp"
	break;
      case 84: /* "native" */

/* Line 1009 of yacc.c  */
#line 41 "compiler2/grammar.y"
	{ delete (yyvaluep->feature); (yyvaluep->feature) = 0; };

/* Line 1009 of yacc.c  */
#line 1610 "compiler2/grammar.cpp"
	break;
      case 85: /* "formal_signature" */

/* Line 1009 of yacc.c  */
#line 42 "compiler2/grammar.y"
	{ delete (yyvaluep->formal); (yyvaluep->formal) = 0; };

/* Line 1009 of yacc.c  */
#line 1619 "compiler2/grammar.cpp"
	break;
      case 86: /* "formal_list" */

/* Line 1009 of yacc.c  */
#line 42 "compiler2/grammar.y"
	{ delete (yyvaluep->formal); (yyvaluep->formal) = 0; };

/* Line 1009 of yacc.c  */
#line 1628 "compiler2/grammar.cpp"
	break;
      case 87: /* "formal" */

/* Line 1009 of yacc.c  */
#line 42 "compiler2/grammar.y"
	{ delete (yyvaluep->formal); (yyvaluep->formal) = 0; };

/* Line 1009 of yacc.c  */
#line 1637 "compiler2/grammar.cpp"
	break;
      case 89: /* "type" */

/* Line 1009 of yacc.c  */
#line 43 "compiler2/grammar.y"
	{ delete (yyvaluep->type); (yyvaluep->type) = 0; };

/* Line 1009 of yacc.c  */
#line 1646 "compiler2/grammar.cpp"
	break;
      case 90: /* "generic_list" */

/* Line 1009 of yacc.c  */
#line 44 "compiler2/grammar.y"
	{ delete (yyvaluep->generic); (yyvaluep->generic) = 0; };

/* Line 1009 of yacc.c  */
#line 1655 "compiler2/grammar.cpp"
	break;
      case 91: /* "generic" */

/* Line 1009 of yacc.c  */
#line 44 "compiler2/grammar.y"
	{ delete (yyvaluep->generic); (yyvaluep->generic) = 0; };

/* Line 1009 of yacc.c  */
#line 1664 "compiler2/grammar.cpp"
	break;
      case 92: /* "block" */

/* Line 1009 of yacc.c  */
#line 39 "compiler2/grammar.y"
	{ delete (yyvaluep->statement); (yyvaluep->statement) = 0; };

/* Line 1009 of yacc.c  */
#line 1673 "compiler2/grammar.cpp"
	break;
      case 93: /* "statement_list" */

/* Line 1009 of yacc.c  */
#line 39 "compiler2/grammar.y"
	{ delete (yyvaluep->statement); (yyvaluep->statement) = 0; };

/* Line 1009 of yacc.c  */
#line 1682 "compiler2/grammar.cpp"
	break;
      case 94: /* "statement" */

/* Line 1009 of yacc.c  */
#line 39 "compiler2/grammar.y"
	{ delete (yyvaluep->statement); (yyvaluep->statement) = 0; };

/* Line 1009 of yacc.c  */
#line 1691 "compiler2/grammar.cpp"
	break;
      case 95: /* "storage" */

/* Line 1009 of yacc.c  */
#line 37 "compiler2/grammar.y"
	{ delete (yyvaluep->expression); (yyvaluep->expression) = 0; };

/* Line 1009 of yacc.c  */
#line 1700 "compiler2/grammar.cpp"
	break;
      case 96: /* "assignment" */

/* Line 1009 of yacc.c  */
#line 37 "compiler2/grammar.y"
	{ delete (yyvaluep->expression); (yyvaluep->expression) = 0; };

/* Line 1009 of yacc.c  */
#line 1709 "compiler2/grammar.cpp"
	break;
      case 97: /* "expression" */

/* Line 1009 of yacc.c  */
#line 37 "compiler2/grammar.y"
	{ delete (yyvaluep->expression); (yyvaluep->expression) = 0; };

/* Line 1009 of yacc.c  */
#line 1718 "compiler2/grammar.cpp"
	break;
      case 98: /* "expression_list" */

/* Line 1009 of yacc.c  */
#line 37 "compiler2/grammar.y"
	{ delete (yyvaluep->expression); (yyvaluep->expression) = 0; };

/* Line 1009 of yacc.c  */
#line 1727 "compiler2/grammar.cpp"
	break;
      case 99: /* "when_list" */

/* Line 1009 of yacc.c  */
#line 39 "compiler2/grammar.y"
	{ delete (yyvaluep->statement); (yyvaluep->statement) = 0; };

/* Line 1009 of yacc.c  */
#line 1736 "compiler2/grammar.cpp"
	break;
      case 100: /* "when" */

/* Line 1009 of yacc.c  */
#line 39 "compiler2/grammar.y"
	{ delete (yyvaluep->statement); (yyvaluep->statement) = 0; };

/* Line 1009 of yacc.c  */
#line 1745 "compiler2/grammar.cpp"
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
#line 95 "compiler2/grammar.y"
    {
        parser->environment()->unit(new Class((yyloc), (yyvsp[(2) - (5)].type), (yyvsp[(4) - (5)].feature))); 
    ;}
    break;

  case 3:

/* Line 1464 of yacc.c  */
#line 98 "compiler2/grammar.y"
    {
        parser->environment()->unit(new Interface((yyloc), (yyvsp[(2) - (5)].type), (yyvsp[(4) - (5)].feature))); 
    ;}
    break;

  case 4:

/* Line 1464 of yacc.c  */
#line 101 "compiler2/grammar.y"
    {
        parser->environment()->unit(new Structure((yyloc), (yyvsp[(2) - (5)].type), (yyvsp[(4) - (5)].feature)));    
    ;}
    break;

  case 5:

/* Line 1464 of yacc.c  */
#line 104 "compiler2/grammar.y"
    {
        parser->environment()->unit(new Module((yyloc), (yyvsp[(2) - (5)].type), (yyvsp[(4) - (5)].feature)));    
    ;}
    break;

  case 6:

/* Line 1464 of yacc.c  */
#line 107 "compiler2/grammar.y"
    { 
		yyerror(&(yyloc), parser, scanner, "Input file is empty"); 
		YYERROR;
	;}
    break;

  case 7:

/* Line 1464 of yacc.c  */
#line 111 "compiler2/grammar.y"
    { ;}
    break;

  case 8:

/* Line 1464 of yacc.c  */
#line 115 "compiler2/grammar.y"
    { 
        (yyval.feature) = (yyvsp[(1) - (2)].feature);
        (yyval.feature)->next((yyvsp[(2) - (2)].feature));
    ;}
    break;

  case 9:

/* Line 1464 of yacc.c  */
#line 119 "compiler2/grammar.y"
    {
        (yyval.feature) = (yyvsp[(1) - (1)].feature);
    ;}
    break;

  case 10:

/* Line 1464 of yacc.c  */
#line 125 "compiler2/grammar.y"
    { (yyval.feature) = (yyvsp[(1) - (1)].feature); ;}
    break;

  case 11:

/* Line 1464 of yacc.c  */
#line 126 "compiler2/grammar.y"
    { (yyval.feature) = (yyvsp[(1) - (1)].feature); ;}
    break;

  case 12:

/* Line 1464 of yacc.c  */
#line 127 "compiler2/grammar.y"
    { (yyval.feature) = (yyvsp[(1) - (1)].feature); ;}
    break;

  case 13:

/* Line 1464 of yacc.c  */
#line 128 "compiler2/grammar.y"
    { (yyval.feature) = (yyvsp[(1) - (1)].feature); ;}
    break;

  case 14:

/* Line 1464 of yacc.c  */
#line 129 "compiler2/grammar.y"
    { (yyval.feature) = (yyvsp[(1) - (1)].feature); ;}
    break;

  case 15:

/* Line 1464 of yacc.c  */
#line 130 "compiler2/grammar.y"
    { (yyval.feature) = (yyvsp[(1) - (1)].feature); ;}
    break;

  case 16:

/* Line 1464 of yacc.c  */
#line 131 "compiler2/grammar.y"
    { (yyval.feature) = (yyvsp[(1) - (1)].feature); ;}
    break;

  case 17:

/* Line 1464 of yacc.c  */
#line 132 "compiler2/grammar.y"
    { (yyval.feature) = (yyvsp[(1) - (1)].feature); ;}
    break;

  case 18:

/* Line 1464 of yacc.c  */
#line 136 "compiler2/grammar.y"
    { 
		(yyval.feature) = new Import((yyloc), (yyvsp[(2) - (3)].type));
	;}
    break;

  case 19:

/* Line 1464 of yacc.c  */
#line 142 "compiler2/grammar.y"
    { 
		(yyval.feature) = new Define((yyloc), (yyvsp[(3) - (4)].name), (yyvsp[(2) - (4)].type));
	;}
    break;

  case 20:

/* Line 1464 of yacc.c  */
#line 148 "compiler2/grammar.y"
    {
		// TODO: Set symbol table for class-level
		(yyval.feature) = new Attribute((yyloc), (yyvsp[(2) - (7)].name), (yyvsp[(4) - (7)].type), (yyvsp[(6) - (7)].expression));
    ;}
    break;

  case 21:

/* Line 1464 of yacc.c  */
#line 152 "compiler2/grammar.y"
    {
		(yyval.feature) = new Attribute((yyloc), (yyvsp[(2) - (5)].name), (yyvsp[(4) - (5)].type), 0);
    ;}
    break;

  case 22:

/* Line 1464 of yacc.c  */
#line 158 "compiler2/grammar.y"
    { 
        Name* name = parser->environment()->name("@init");
        (yyval.feature) = new Function((yyloc), name, (yyvsp[(3) - (4)].formal), 0, (yyvsp[(4) - (4)].statement));
	;}
    break;

  case 23:

/* Line 1464 of yacc.c  */
#line 165 "compiler2/grammar.y"
    { 
        Name* name = parser->environment()->name("@destroy");
        (yyval.feature) = new Function((yyloc), name, 0, 0, (yyvsp[(5) - (5)].statement));
	;}
    break;

  case 24:

/* Line 1464 of yacc.c  */
#line 172 "compiler2/grammar.y"
    {
        (yyval.feature) = new Function((yyloc), (yyvsp[(2) - (7)].name), (yyvsp[(3) - (7)].formal), (yyvsp[(5) - (7)].type), (yyvsp[(7) - (7)].statement));
    ;}
    break;

  case 25:

/* Line 1464 of yacc.c  */
#line 175 "compiler2/grammar.y"
    {
        (yyval.feature) = new Function((yyloc), (yyvsp[(2) - (5)].name), (yyvsp[(3) - (5)].formal), 0, (yyvsp[(5) - (5)].statement));
	;}
    break;

  case 26:

/* Line 1464 of yacc.c  */
#line 181 "compiler2/grammar.y"
    {
        (yyval.feature) = new Function((yyloc), (yyvsp[(2) - (7)].name), (yyvsp[(3) - (7)].formal), (yyvsp[(5) - (7)].type), 0);
	;}
    break;

  case 27:

/* Line 1464 of yacc.c  */
#line 184 "compiler2/grammar.y"
    {
        (yyval.feature) = new Function((yyloc), (yyvsp[(2) - (5)].name), (yyvsp[(3) - (5)].formal), 0, 0);
	;}
    break;

  case 28:

/* Line 1464 of yacc.c  */
#line 190 "compiler2/grammar.y"
    {
        (yyval.feature) = new Function((yyloc), (yyvsp[(2) - (8)].name), (yyvsp[(3) - (8)].formal), (yyvsp[(5) - (8)].type), 0);
	;}
    break;

  case 29:

/* Line 1464 of yacc.c  */
#line 193 "compiler2/grammar.y"
    {
        (yyval.feature) = new Function((yyloc), (yyvsp[(2) - (6)].name), (yyvsp[(3) - (6)].formal), 0, 0);
	;}
    break;

  case 30:

/* Line 1464 of yacc.c  */
#line 199 "compiler2/grammar.y"
    { (yyval.formal) = (yyvsp[(2) - (3)].formal); ;}
    break;

  case 31:

/* Line 1464 of yacc.c  */
#line 200 "compiler2/grammar.y"
    { (yyval.formal) = 0; ;}
    break;

  case 32:

/* Line 1464 of yacc.c  */
#line 204 "compiler2/grammar.y"
    { 
        (yyval.formal) = (yyvsp[(1) - (3)].formal);
        (yyval.formal)->next((yyvsp[(3) - (3)].formal));
	;}
    break;

  case 33:

/* Line 1464 of yacc.c  */
#line 208 "compiler2/grammar.y"
    { 
        (yyval.formal) = (yyvsp[(1) - (1)].formal);
	;}
    break;

  case 34:

/* Line 1464 of yacc.c  */
#line 214 "compiler2/grammar.y"
    {
        (yyval.formal) = new Formal((yyloc), (yyvsp[(1) - (3)].name), (yyvsp[(3) - (3)].type));
    ;}
    break;

  case 35:

/* Line 1464 of yacc.c  */
#line 219 "compiler2/grammar.y"
    { (yyval.flag) = 0; ;}
    break;

  case 36:

/* Line 1464 of yacc.c  */
#line 220 "compiler2/grammar.y"
    { (yyval.flag) = 0; ;}
    break;

  case 37:

/* Line 1464 of yacc.c  */
#line 221 "compiler2/grammar.y"
    { (yyval.flag) = 0; ;}
    break;

  case 38:

/* Line 1464 of yacc.c  */
#line 222 "compiler2/grammar.y"
    { (yyval.flag) = 0; ;}
    break;

  case 39:

/* Line 1464 of yacc.c  */
#line 223 "compiler2/grammar.y"
    { (yyval.flag) = 0; ;}
    break;

  case 40:

/* Line 1464 of yacc.c  */
#line 224 "compiler2/grammar.y"
    { (yyval.flag) = 0; ;}
    break;

  case 41:

/* Line 1464 of yacc.c  */
#line 228 "compiler2/grammar.y"
    { 
        (yyval.type) = new Type((yyvsp[(1) - (3)].type), (yyvsp[(3) - (3)].name), 0, parser->environment()); 
    ;}
    break;

  case 42:

/* Line 1464 of yacc.c  */
#line 231 "compiler2/grammar.y"
    {
        (yyval.type) = new Type((yyvsp[(1) - (6)].type), (yyvsp[(3) - (6)].name), (yyvsp[(5) - (6)].generic), parser->environment());
    ;}
    break;

  case 43:

/* Line 1464 of yacc.c  */
#line 234 "compiler2/grammar.y"
    {
        (yyval.type) = new Type(0, (yyvsp[(1) - (1)].name), 0, parser->environment()); 
    ;}
    break;

  case 44:

/* Line 1464 of yacc.c  */
#line 237 "compiler2/grammar.y"
    {
        (yyval.type) = new Type(0, (yyvsp[(1) - (4)].name), (yyvsp[(3) - (4)].generic), parser->environment());
    ;}
    break;

  case 45:

/* Line 1464 of yacc.c  */
#line 243 "compiler2/grammar.y"
    {
        (yyval.generic) = (yyvsp[(1) - (3)].generic);
        (yyval.generic)->next((yyvsp[(3) - (3)].generic));
    ;}
    break;

  case 46:

/* Line 1464 of yacc.c  */
#line 247 "compiler2/grammar.y"
    {
        (yyval.generic) = (yyvsp[(1) - (1)].generic);
    ;}
    break;

  case 47:

/* Line 1464 of yacc.c  */
#line 253 "compiler2/grammar.y"
    { (yyval.generic) = new Generic((yyvsp[(1) - (1)].type)); ;}
    break;

  case 48:

/* Line 1464 of yacc.c  */
#line 257 "compiler2/grammar.y"
    { 
		(yyval.statement) = new Block((yyloc), (yyvsp[(2) - (3)].statement)); 
	;}
    break;

  case 49:

/* Line 1464 of yacc.c  */
#line 260 "compiler2/grammar.y"
    { 
        (yyval.statement) = new Return((yyloc), (yyvsp[(3) - (4)].expression)); 
    ;}
    break;

  case 50:

/* Line 1464 of yacc.c  */
#line 263 "compiler2/grammar.y"
    { 
        (yyval.statement) = new Return((yyloc), 0); 
    ;}
    break;

  case 51:

/* Line 1464 of yacc.c  */
#line 266 "compiler2/grammar.y"
    { 
		(yyval.statement) = new Simple((yyloc), (yyvsp[(2) - (3)].expression)); 
    ;}
    break;

  case 52:

/* Line 1464 of yacc.c  */
#line 272 "compiler2/grammar.y"
    { 
		(yyval.statement) = (yyvsp[(1) - (2)].statement);
        (yyval.statement)->next((yyvsp[(2) - (2)].statement));
	;}
    break;

  case 53:

/* Line 1464 of yacc.c  */
#line 276 "compiler2/grammar.y"
    {
        (yyval.statement) = (yyvsp[(1) - (1)].statement);
    ;}
    break;

  case 54:

/* Line 1464 of yacc.c  */
#line 279 "compiler2/grammar.y"
    { 
        (yyval.statement) = (yyvsp[(2) - (2)].statement); 
    ;}
    break;

  case 55:

/* Line 1464 of yacc.c  */
#line 286 "compiler2/grammar.y"
    {
		(yyval.statement) = new For((yyloc), (yyvsp[(2) - (7)].name), (yyvsp[(4) - (7)].type), (yyvsp[(6) - (7)].expression), (yyvsp[(7) - (7)].statement));
	;}
    break;

  case 56:

/* Line 1464 of yacc.c  */
#line 289 "compiler2/grammar.y"
    {
        // TODO: FIX UNTIL
        Name* op = parser->environment()->name("!");
		(yyval.statement) = new While((yyloc), new Unary((yyloc), op, (yyvsp[(2) - (3)].expression)), (yyvsp[(3) - (3)].statement));
        
	;}
    break;

  case 57:

/* Line 1464 of yacc.c  */
#line 295 "compiler2/grammar.y"
    {
		(yyval.statement) = new While((yyloc), (yyvsp[(2) - (3)].expression), (yyvsp[(3) - (3)].statement));
	;}
    break;

  case 58:

/* Line 1464 of yacc.c  */
#line 298 "compiler2/grammar.y"
    {
        (yyval.statement) = new Case((yyloc), (yyvsp[(2) - (5)].expression), static_cast<When*>((yyvsp[(4) - (5)].statement)));
    ;}
    break;

  case 59:

/* Line 1464 of yacc.c  */
#line 301 "compiler2/grammar.y"
    { 
		(yyval.statement) = new Variable((yyloc), (yyvsp[(2) - (4)].name), (yyvsp[(4) - (4)].type), 0); 
	;}
    break;

  case 60:

/* Line 1464 of yacc.c  */
#line 304 "compiler2/grammar.y"
    { 
        (yyval.statement) = new Variable((yyloc), (yyvsp[(2) - (7)].name), (yyvsp[(4) - (7)].type), (yyvsp[(6) - (7)].expression)); 
	;}
    break;

  case 61:

/* Line 1464 of yacc.c  */
#line 307 "compiler2/grammar.y"
    { 
        (yyval.statement) = new Return((yyloc), (yyvsp[(2) - (3)].expression));
    ;}
    break;

  case 62:

/* Line 1464 of yacc.c  */
#line 310 "compiler2/grammar.y"
    { 
        (yyval.statement) = new Return((yyloc), 0);
    ;}
    break;

  case 63:

/* Line 1464 of yacc.c  */
#line 313 "compiler2/grammar.y"
    { 
		(yyval.statement) = new Simple((yyloc), (yyvsp[(1) - (2)].expression)); 
	;}
    break;

  case 64:

/* Line 1464 of yacc.c  */
#line 316 "compiler2/grammar.y"
    { 
		(yyval.statement) = new Simple((yyloc), (yyvsp[(1) - (2)].expression)); 
	;}
    break;

  case 65:

/* Line 1464 of yacc.c  */
#line 319 "compiler2/grammar.y"
    {
		(yyval.statement) = new Conditional((yyloc), (yyvsp[(2) - (3)].expression), (yyvsp[(3) - (3)].statement), 0);
	;}
    break;

  case 66:

/* Line 1464 of yacc.c  */
#line 322 "compiler2/grammar.y"
    {
        (yyval.statement) = new Conditional((yyloc), (yyvsp[(2) - (5)].expression), (yyvsp[(3) - (5)].statement), (yyvsp[(5) - (5)].statement));
    ;}
    break;

  case 67:

/* Line 1464 of yacc.c  */
#line 328 "compiler2/grammar.y"
    { 
		/* Member variable access */
		(yyval.expression) = new Member((yyloc), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].name)); 
	;}
    break;

  case 68:

/* Line 1464 of yacc.c  */
#line 332 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Index((yyloc), (yyvsp[(1) - (4)].expression), (yyvsp[(3) - (4)].expression));
	;}
    break;

  case 69:

/* Line 1464 of yacc.c  */
#line 337 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("="), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
        (yyval.expression) = new Binary((yyloc), parser->environment()->name("="), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
	;}
    break;

  case 70:

/* Line 1464 of yacc.c  */
#line 341 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("*="), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 71:

/* Line 1464 of yacc.c  */
#line 344 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("/="), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 72:

/* Line 1464 of yacc.c  */
#line 347 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("%="), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 73:

/* Line 1464 of yacc.c  */
#line 350 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("-="), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 74:

/* Line 1464 of yacc.c  */
#line 353 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("+="), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 75:

/* Line 1464 of yacc.c  */
#line 356 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("&="), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 76:

/* Line 1464 of yacc.c  */
#line 359 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("|="), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 77:

/* Line 1464 of yacc.c  */
#line 366 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("?"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
	;}
    break;

  case 78:

/* Line 1464 of yacc.c  */
#line 369 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("||"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 79:

/* Line 1464 of yacc.c  */
#line 372 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("&&"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 80:

/* Line 1464 of yacc.c  */
#line 375 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("|"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 81:

/* Line 1464 of yacc.c  */
#line 378 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("^"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 82:

/* Line 1464 of yacc.c  */
#line 381 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("&"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 83:

/* Line 1464 of yacc.c  */
#line 384 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("=="), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 84:

/* Line 1464 of yacc.c  */
#line 387 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("!="), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 85:

/* Line 1464 of yacc.c  */
#line 390 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name(">"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 86:

/* Line 1464 of yacc.c  */
#line 393 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("<"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 87:

/* Line 1464 of yacc.c  */
#line 396 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name(">="), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 88:

/* Line 1464 of yacc.c  */
#line 399 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("<="), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 89:

/* Line 1464 of yacc.c  */
#line 402 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("<<"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 90:

/* Line 1464 of yacc.c  */
#line 405 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name(">>"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 91:

/* Line 1464 of yacc.c  */
#line 408 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("+"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 92:

/* Line 1464 of yacc.c  */
#line 411 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("-"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 93:

/* Line 1464 of yacc.c  */
#line 414 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("*"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 94:

/* Line 1464 of yacc.c  */
#line 417 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("/"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 95:

/* Line 1464 of yacc.c  */
#line 420 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Binary((yyloc), parser->environment()->name("%"), (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); 
	;}
    break;

  case 96:

/* Line 1464 of yacc.c  */
#line 423 "compiler2/grammar.y"
    { 
        (yyval.expression) = new Unary((yyloc), parser->environment()->name("!"), (yyvsp[(2) - (2)].expression)); 
    ;}
    break;

  case 97:

/* Line 1464 of yacc.c  */
#line 426 "compiler2/grammar.y"
    { 
        (yyval.expression) = new Unary((yyloc), parser->environment()->name("~"), (yyvsp[(2) - (2)].expression)); 
    ;}
    break;

  case 98:

/* Line 1464 of yacc.c  */
#line 429 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Unary((yyloc), parser->environment()->name("++"), (yyvsp[(1) - (2)].expression)); 
	;}
    break;

  case 99:

/* Line 1464 of yacc.c  */
#line 432 "compiler2/grammar.y"
    { 
		(yyval.expression) = new Unary((yyloc), parser->environment()->name("--"), (yyvsp[(1) - (2)].expression)); 
	;}
    break;

  case 100:

/* Line 1464 of yacc.c  */
#line 435 "compiler2/grammar.y"
    {
		(yyval.expression) = new Call((yyloc), (yyvsp[(1) - (4)].name), (yyvsp[(3) - (4)].expression));
    ;}
    break;

  case 101:

/* Line 1464 of yacc.c  */
#line 438 "compiler2/grammar.y"
    {
		/* Call on an object expression */
		(yyval.expression) = new Call((yyloc), (yyvsp[(1) - (3)].name), 0);
	;}
    break;

  case 102:

/* Line 1464 of yacc.c  */
#line 442 "compiler2/grammar.y"
    {
        (yyvsp[(1) - (6)].expression)->next((yyvsp[(5) - (6)].expression));
        (yyval.expression) = new Dispatch((yyloc), (yyvsp[(3) - (6)].name), (yyvsp[(1) - (6)].expression));
    ;}
    break;

  case 103:

/* Line 1464 of yacc.c  */
#line 446 "compiler2/grammar.y"
    {
        (yyval.expression) = new Dispatch((yyloc), (yyvsp[(3) - (5)].name), (yyvsp[(1) - (5)].expression));
    ;}
    break;

  case 104:

/* Line 1464 of yacc.c  */
#line 449 "compiler2/grammar.y"
    { (yyval.expression) = (yyvsp[(1) - (1)].expression); ;}
    break;

  case 105:

/* Line 1464 of yacc.c  */
#line 450 "compiler2/grammar.y"
    { (yyval.expression) = (yyvsp[(2) - (3)].expression); ;}
    break;

  case 106:

/* Line 1464 of yacc.c  */
#line 451 "compiler2/grammar.y"
    { (yyval.expression) = (yyvsp[(1) - (1)].expression); ;}
    break;

  case 107:

/* Line 1464 of yacc.c  */
#line 452 "compiler2/grammar.y"
    { (yyval.expression) = (yyvsp[(1) - (1)].expression); ;}
    break;

  case 108:

/* Line 1464 of yacc.c  */
#line 453 "compiler2/grammar.y"
    { (yyval.expression) = new Identifier((yyloc), (yyvsp[(1) - (1)].name)); ;}
    break;

  case 109:

/* Line 1464 of yacc.c  */
#line 457 "compiler2/grammar.y"
    { 
		(yyval.expression) = (yyvsp[(1) - (3)].expression);
        (yyval.expression)->next((yyvsp[(3) - (3)].expression));
	;}
    break;

  case 110:

/* Line 1464 of yacc.c  */
#line 461 "compiler2/grammar.y"
    { (yyval.expression) = (yyvsp[(1) - (1)].expression); ;}
    break;

  case 111:

/* Line 1464 of yacc.c  */
#line 466 "compiler2/grammar.y"
    {
        (yyval.statement) = (yyvsp[(1) - (2)].statement);
        (yyval.statement)->next((yyvsp[(2) - (2)].statement));
    ;}
    break;

  case 112:

/* Line 1464 of yacc.c  */
#line 470 "compiler2/grammar.y"
    {
        (yyval.statement) = (yyvsp[(1) - (1)].statement);
    ;}
    break;

  case 113:

/* Line 1464 of yacc.c  */
#line 476 "compiler2/grammar.y"
    {
        (yyval.statement) = new When((yyloc), (yyvsp[(2) - (5)].name), (yyvsp[(4) - (5)].type), (yyvsp[(5) - (5)].statement));
    ;}
    break;



/* Line 1464 of yacc.c  */
#line 3054 "compiler2/grammar.cpp"
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



