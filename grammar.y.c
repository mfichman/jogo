
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
#include <op.h>


/* Line 189 of yacc.c  */
#line 85 "grammar.y.c"

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
     TOK_FOREACH = 281,
     TOK_DO = 282,
     TOK_FOR = 283,
     TOK_RETURN = 284,
     TOK_EQUAL = 285,
     TOK_NOTEQUAL = 286,
     TOK_SCOPE = 287,
     TOK_OR = 288,
     TOK_AND = 289,
     TOK_GE = 290,
     TOK_LE = 291,
     TOK_LSHIFT = 292,
     TOK_RSHIFT = 293,
     TOK_MUL_ASSIGN = 294,
     TOK_DIV_ASSIGN = 295,
     TOK_SUB_ASSIGN = 296,
     TOK_ADD_ASSIGN = 297,
     TOK_MOD_ASSIGN = 298,
     TOK_BITOR_ASSIGN = 299,
     TOK_BITAND_ASSIGN = 300,
     TOK_INC = 301,
     TOK_DEC = 302
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 12 "grammar.y"
 expr_t *expr; 

/* Line 214 of yacc.c  */
#line 13 "grammar.y"
 stmt_t *stmt; 

/* Line 214 of yacc.c  */
#line 14 "grammar.y"
 type_t *type; 

/* Line 214 of yacc.c  */
#line 15 "grammar.y"
 unit_t *unit; 

/* Line 214 of yacc.c  */
#line 16 "grammar.y"
 var_t *var; 

/* Line 214 of yacc.c  */
#line 17 "grammar.y"
 func_t *func; 

/* Line 214 of yacc.c  */
#line 18 "grammar.y"
 char *string; 

/* Line 214 of yacc.c  */
#line 19 "grammar.y"
 int null; 

/* Line 214 of yacc.c  */
#line 20 "grammar.y"
 int flag; 


/* Line 214 of yacc.c  */
#line 204 "grammar.y.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 216 "grammar.y.c"

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
#define YYLAST   470

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  71
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  40
/* YYNRULES -- Number of rules.  */
#define YYNRULES  145
/* YYNRULES -- Number of states.  */
#define YYNSTATES  309

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
       2,     2,     2,    66,     2,     2,     2,    65,    59,     2,
      50,    51,    53,    62,    52,    63,    70,    64,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    56,    48,
      61,    49,    60,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    68,     2,    69,    58,     2,     2,     2,     2,     2,
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
      45,    46,    47
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
     188,   190,   192,   193,   195,   196,   198,   201,   203,   206,
     210,   212,   216,   218,   222,   225,   226,   236,   245,   251,
     257,   265,   273,   277,   279,   282,   285,   289,   292,   297,
     299,   300,   306,   314,   316,   318,   322,   326,   330,   334,
     338,   342,   346,   350,   352,   356,   358,   362,   364,   368,
     372,   374,   378,   380,   384,   388,   390,   394,   398,   402,
     406,   408,   412,   416,   418,   422,   426,   428,   432,   436,
     440,   442,   445,   448,   451,   454,   457,   460,   463,   465,
     470,   474,   479,   483,   486,   489,   491,   493,   495,   497,
     501,   505,   510,   514,   518,   522
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      72,     0,    -1,     9,    90,    48,    73,    -1,    10,    90,
      48,    74,    -1,    11,    90,    48,    75,    -1,    12,    90,
      48,    76,    -1,    77,    73,    -1,    78,    73,    -1,    79,
      73,    -1,    80,    73,    -1,    81,    73,    -1,    82,    73,
      -1,    83,    73,    -1,     1,    73,    -1,    -1,    77,    74,
      -1,    78,    74,    -1,    84,    74,    -1,     1,    74,    -1,
      -1,    77,    75,    -1,    78,    75,    -1,    79,    75,    -1,
      80,    75,    -1,    82,    75,    -1,     1,    75,    -1,    -1,
      77,    76,    -1,    78,    76,    -1,    82,    76,    -1,     1,
      76,    -1,    -1,    13,    90,    48,    -1,    14,    89,     3,
      48,    -1,    87,    88,    89,     5,    49,    96,    48,    -1,
      87,    88,    89,     5,    48,    -1,    87,    88,    89,     6,
      49,    96,    48,    -1,    15,    85,    87,    91,    -1,    16,
      50,    51,    91,    -1,     5,    85,    87,    88,    89,    91,
      -1,     5,    85,    87,    88,    91,    -1,     5,    85,    87,
      88,    21,    89,    48,    -1,     5,    85,    87,    88,    21,
      48,    -1,     5,    85,    89,    48,    -1,     5,    85,    48,
      -1,    50,    86,    51,    -1,    50,    51,    -1,    89,     5,
      52,    86,    -1,    89,     5,    -1,    17,    -1,    18,    -1,
      19,    -1,    -1,    20,    -1,    -1,     4,    -1,     4,    53,
      -1,    90,    -1,    90,    53,    -1,     3,    32,    90,    -1,
       3,    -1,     6,    32,    90,    -1,     6,    -1,    54,    92,
      55,    -1,    93,    92,    -1,    -1,    28,    50,    94,    48,
      94,    48,    94,    51,    91,    -1,    26,    50,    89,     5,
      56,    96,    51,    91,    -1,    24,    50,    94,    51,    91,
      -1,    22,    50,    94,    51,    91,    -1,    27,    91,    24,
      50,    94,    51,    48,    -1,    27,    91,    22,    50,    94,
      51,    48,    -1,    89,     5,    48,    -1,    95,    -1,    94,
      48,    -1,     1,    48,    -1,    29,    96,    48,    -1,    29,
      48,    -1,    89,     5,    49,    96,    -1,    96,    -1,    -1,
      25,    50,    94,    51,    91,    -1,    25,    50,    94,    51,
      91,    23,    95,    -1,    91,    -1,    97,    -1,   107,    49,
      97,    -1,   107,    39,    97,    -1,   107,    40,    97,    -1,
     107,    43,    97,    -1,   107,    41,    97,    -1,   107,    42,
      97,    -1,   107,    45,    97,    -1,   107,    44,    97,    -1,
      98,    -1,    98,    33,    99,    -1,    99,    -1,    99,    34,
     100,    -1,   100,    -1,   100,    57,   101,    -1,   100,    58,
     101,    -1,   101,    -1,   101,    59,   102,    -1,   102,    -1,
     102,    30,   103,    -1,   102,    31,   103,    -1,   103,    -1,
     103,    60,   104,    -1,   103,    61,   104,    -1,   103,    35,
     104,    -1,   103,    36,   104,    -1,   104,    -1,   104,    37,
     105,    -1,   104,    38,   105,    -1,   105,    -1,   105,    62,
     106,    -1,   105,    63,   106,    -1,   106,    -1,   106,    53,
     107,    -1,   106,    64,   107,    -1,   106,    65,   107,    -1,
     107,    -1,    46,   107,    -1,    47,   107,    -1,    62,   107,
      -1,    63,   107,    -1,    66,   107,    -1,    67,   107,    -1,
      53,   107,    -1,   108,    -1,   108,    50,   110,    51,    -1,
     108,    50,    51,    -1,   108,    68,    96,    69,    -1,   108,
      70,     5,    -1,   108,    46,    -1,   108,    47,    -1,   109,
      -1,     7,    -1,     8,    -1,     5,    -1,    89,    70,     6,
      -1,    89,    70,     5,    -1,    89,    50,   110,    51,    -1,
      89,    50,    51,    -1,    50,    96,    51,    -1,    96,    52,
     110,    -1,    96,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   124,   124,   128,   132,   136,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   155,   156,   157,   158,   159,
     163,   164,   165,   166,   167,   168,   169,   173,   174,   175,
     176,   177,   181,   188,   195,   198,   201,   207,   214,   220,
     224,   231,   235,   242,   245,   251,   252,   256,   260,   264,
     265,   266,   267,   271,   272,   276,   277,   278,   279,   283,
     284,   285,   289,   296,   300,   301,   305,   308,   311,   314,
     317,   320,   323,   326,   327,   328,   329,   330,   334,   337,
     340,   344,   347,   350,   353,   356,   359,   362,   365,   368,
     371,   374,   377,   380,   384,   385,   389,   390,   394,   395,
     396,   400,   401,   405,   406,   409,   413,   414,   415,   416,
     417,   421,   422,   423,   427,   428,   429,   433,   434,   435,
     436,   440,   441,   442,   443,   444,   445,   446,   447,   451,
     452,   453,   454,   455,   456,   457,   461,   462,   463,   464,
     465,   466,   467,   468,   472,   476
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
  "TOK_IF", "TOK_FOREACH", "TOK_DO", "TOK_FOR", "TOK_RETURN", "TOK_EQUAL",
  "TOK_NOTEQUAL", "TOK_SCOPE", "TOK_OR", "TOK_AND", "TOK_GE", "TOK_LE",
  "TOK_LSHIFT", "TOK_RSHIFT", "TOK_MUL_ASSIGN", "TOK_DIV_ASSIGN",
  "TOK_SUB_ASSIGN", "TOK_ADD_ASSIGN", "TOK_MOD_ASSIGN", "TOK_BITOR_ASSIGN",
  "TOK_BITAND_ASSIGN", "TOK_INC", "TOK_DEC", "';'", "'='", "'('", "')'",
  "','", "'*'", "'{'", "'}'", "':'", "'|'", "'^'", "'&'", "'>'", "'<'",
  "'+'", "'-'", "'/'", "'%'", "'!'", "'~'", "'['", "']'", "'.'", "$accept",
  "translation_unit", "class", "interface", "struct", "module", "import",
  "def", "variable", "constructor", "destructor", "function", "native",
  "prototype", "parameter_signature", "parameter_list", "access", "static",
  "type", "qualified_name", "block", "statement_list", "statement",
  "clause", "conditional", "expression", "assignment", "logical_or",
  "logical_and", "bitwise_or", "bitwise_and", "equality", "relation",
  "shift", "addition", "multiplication", "unary", "postfix", "primary",
  "argument_list", 0
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
     295,   296,   297,   298,   299,   300,   301,   302,    59,    61,
      40,    41,    44,    42,   123,   125,    58,   124,    94,    38,
      62,    60,    43,    45,    47,    37,    33,   126,    91,    93,
      46
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
      87,    87,    87,    88,    88,    89,    89,    89,    89,    90,
      90,    90,    90,    91,    92,    92,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    94,    94,
      94,    95,    95,    95,    96,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    98,    98,    99,    99,   100,   100,
     100,   101,   101,   102,   102,   102,   103,   103,   103,   103,
     103,   104,   104,   104,   105,   105,   105,   106,   106,   106,
     106,   107,   107,   107,   107,   107,   107,   107,   107,   108,
     108,   108,   108,   108,   108,   108,   109,   109,   109,   109,
     109,   109,   109,   109,   110,   110
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     4,     4,     4,     2,     2,     2,     2,
       2,     2,     2,     2,     0,     2,     2,     2,     2,     0,
       2,     2,     2,     2,     2,     2,     0,     2,     2,     2,
       2,     0,     3,     4,     7,     5,     7,     4,     4,     6,
       5,     7,     6,     4,     3,     3,     2,     4,     2,     1,
       1,     1,     0,     1,     0,     1,     2,     1,     2,     3,
       1,     3,     1,     3,     2,     0,     9,     8,     5,     5,
       7,     7,     3,     1,     2,     2,     3,     2,     4,     1,
       0,     5,     7,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     3,     1,     3,     1,     3,     3,
       1,     3,     1,     3,     3,     1,     3,     3,     3,     3,
       1,     3,     3,     1,     3,     3,     1,     3,     3,     3,
       1,     2,     2,     2,     2,     2,     2,     2,     1,     4,
       3,     4,     3,     2,     2,     1,     1,     1,     1,     3,
       3,     4,     3,     3,     3,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,    60,    62,     0,     0,
       0,     0,     1,     0,     0,     0,     0,     0,     0,    59,
      61,     0,     0,     0,     0,     0,     0,    49,    50,    51,
       2,     0,     0,     0,     0,     0,     0,     0,    54,     0,
       0,     3,     0,     0,     0,     0,     0,     4,     0,     0,
       0,     0,     0,     0,     5,     0,     0,     0,    13,     0,
      52,     0,    55,     0,    57,    52,     0,     6,     7,     8,
       9,    10,    11,    12,    53,     0,    18,     0,    15,    16,
      17,    25,    52,    20,    21,    22,    23,    24,    30,    27,
      28,    29,    46,     0,     0,    54,    32,    56,     0,    58,
       0,     0,     0,    44,     0,    54,    45,    48,     0,    33,
       0,    37,    38,     0,     0,    43,     0,     0,     0,     0,
      40,     0,   138,   136,   137,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,     0,     0,     0,    73,    79,    84,    93,    95,
      97,   100,   102,   105,   110,   113,   116,   120,   128,   135,
      35,     0,     0,    47,    42,     0,    39,    75,    80,    80,
      80,     0,     0,    80,    77,     0,     0,   121,   122,     0,
     127,   123,   124,   125,   126,     0,     0,     0,    63,    64,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   133,   134,     0,
       0,     0,     0,     0,    41,     0,     0,     0,     0,     0,
       0,     0,     0,    76,   143,    72,     0,   142,   145,     0,
     140,   139,    94,   120,    96,    98,    99,   101,   103,   104,
     108,   109,   106,   107,   111,   112,   114,   115,   117,   118,
     119,    86,    87,    89,    90,    88,    92,    91,    85,   130,
       0,     0,   132,    34,    36,     0,     0,     0,     0,     0,
      80,    80,    80,    78,     0,   141,   129,   131,    69,    68,
      81,     0,     0,     0,     0,   144,     0,     0,     0,     0,
      80,    82,     0,    71,    70,     0,    67,     0,    66
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,    30,    41,    47,    54,    31,    32,    33,    34,
      35,    36,    37,    44,    60,    93,    38,    75,   175,    64,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   239
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -214
static const yytype_int16 yypact[] =
{
     195,   117,   117,   117,   117,    42,    -6,    14,    18,    40,
      51,    61,  -214,   117,   117,   429,    54,   450,    97,  -214,
    -214,   429,    68,   117,   147,    68,    84,  -214,  -214,  -214,
    -214,   429,   429,   429,   429,   429,   429,   429,    81,    54,
      68,  -214,    54,    54,    54,   450,    68,  -214,   450,   450,
     450,   450,   450,    97,  -214,    97,    97,    97,  -214,    19,
      64,   100,   101,   156,   146,    64,   158,  -214,  -214,  -214,
    -214,  -214,  -214,  -214,  -214,   147,  -214,    30,  -214,  -214,
    -214,  -214,    64,  -214,  -214,  -214,  -214,  -214,  -214,  -214,
    -214,  -214,  -214,   175,   233,    81,  -214,  -214,   207,  -214,
     210,   210,   108,  -214,   223,    81,  -214,   227,    10,  -214,
     215,  -214,  -214,    12,   253,  -214,    15,   147,    37,   210,
    -214,   243,  -214,  -214,  -214,   256,   257,   260,   261,   210,
     262,   242,   319,   319,   319,   319,   319,   319,   319,   319,
       6,  -214,   248,   215,   265,  -214,  -214,  -214,   287,   301,
      75,   277,   107,    13,   135,   148,    26,   289,   -18,  -214,
    -214,   319,   319,  -214,  -214,   273,  -214,  -214,   319,   319,
     319,   147,    65,   319,  -214,   -23,   294,  -214,  -214,   286,
    -214,  -214,  -214,  -214,  -214,   187,   293,   254,  -214,  -214,
    -214,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,  -214,  -214,   311,
     319,   336,   297,   315,  -214,     7,   320,   324,   325,   365,
     329,   330,   335,  -214,  -214,  -214,   319,  -214,   332,   339,
    -214,  -214,   301,  -214,    75,   277,   277,   107,    13,    13,
     135,   135,   135,   135,   148,   148,    26,    26,  -214,  -214,
    -214,  -214,  -214,  -214,  -214,  -214,  -214,  -214,  -214,  -214,
     340,   323,  -214,  -214,  -214,   344,   210,   210,   210,   338,
     319,   319,   319,  -214,   319,  -214,  -214,  -214,  -214,  -214,
     373,   319,   346,   353,   360,  -214,   -10,   358,   363,   364,
     319,  -214,   210,  -214,  -214,   366,  -214,   210,  -214
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -214,  -214,   196,   132,   235,   145,   113,   371,    55,    91,
    -214,   350,  -214,  -214,    -1,   308,    -2,   -30,   -24,   211,
     -99,   275,  -214,  -165,   140,  -124,   138,  -214,   240,   245,
      73,   244,    76,    53,    72,    89,   -11,  -214,  -214,  -213
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -81
static const yytype_int16 yytable[] =
{
      63,   111,   112,   226,   227,   228,   270,   176,   232,   120,
     179,   185,   275,     6,    62,   127,     7,   120,     6,    62,
     166,     7,     6,    62,    65,     7,    13,   186,   217,   218,
     172,   118,   219,     6,    62,    94,     7,   222,   223,    77,
       6,    62,    12,     7,   110,    82,    14,   187,   198,   199,
     220,   102,   221,   104,   -19,    39,   186,   186,    95,    40,
     160,   161,   238,   100,   110,   108,    15,    23,    24,   110,
      92,   295,    50,   200,   201,   116,   187,   187,   103,   206,
     105,    27,    28,    29,   119,   164,   140,   230,    16,   231,
     207,   208,   119,    94,   165,   238,   271,   -31,    53,    17,
      50,    74,    46,    50,    50,    50,    50,    50,    51,    18,
      23,    24,   283,   113,   114,   292,   293,   294,    59,   140,
       6,   177,   178,     7,   180,   181,   182,   183,   184,    42,
      48,    55,   193,   194,    66,   305,    51,   196,   197,    51,
      51,    51,    51,    51,   225,   225,   225,   229,    96,   225,
       6,    62,    42,     7,    97,    42,    42,    42,    48,    98,
     238,    48,    48,    48,    48,    48,    55,   297,    55,    55,
      55,    76,   202,   203,    78,    79,    80,   288,   289,   290,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   258,   259,   260,    88,    99,
      89,    90,    91,   306,     1,     2,     3,     4,   308,   101,
     204,   205,     8,     9,    10,    11,   121,    58,     6,    62,
     122,     7,   123,   124,    19,    20,   106,    67,    68,    69,
      70,    71,    72,    73,    61,   235,   236,   125,   107,   126,
     127,   128,   129,   130,   131,     6,    62,   122,     7,   123,
     124,   250,   251,   252,   253,   109,   225,   225,   225,   240,
     241,   132,   133,   -80,   110,   134,   245,   246,   135,   110,
     -65,   115,   248,   249,   254,   255,   225,   136,   137,   117,
      81,   138,   139,    83,    84,    85,    86,    87,   132,   133,
     174,   167,   134,   256,   257,   135,     6,    62,   122,     7,
     123,   124,   162,   188,   136,   137,   168,   169,   138,   139,
     170,   171,   173,   190,     6,    62,   122,     7,   123,   124,
     191,   224,     6,    62,   122,     7,   123,   124,   209,   210,
     211,   212,   213,   214,   215,   192,   195,   234,   216,   132,
     133,   272,   233,   134,   237,   273,   135,   261,   262,   263,
     264,   265,   266,   267,   268,   136,   137,   132,   133,   138,
     139,   134,   269,   274,   135,   132,   133,    52,    57,   134,
     279,   276,   135,   136,   137,   277,   278,   138,   139,   280,
     281,   136,   137,   282,   284,   138,   139,    43,    49,    56,
     285,   286,   287,   236,   291,    52,   296,   298,    52,    52,
      52,    52,    52,    57,   299,    57,    57,    57,   300,   302,
      43,   303,   304,    43,    43,    43,    49,   307,   189,    49,
      49,    49,    49,    49,    56,   163,    56,    56,    56,   -14,
      21,   242,   -52,   -52,    22,   -52,   301,   244,     0,   247,
       0,     0,    23,    24,    25,    26,    27,    28,    29,   -52,
     -26,    45,     0,   -52,   -52,    46,   -52,     0,     0,     0,
       0,     0,     0,    23,    24,    25,     0,    27,    28,    29,
     -52
};

static const yytype_int16 yycheck[] =
{
      24,   100,   101,   168,   169,   170,   219,   131,   173,   108,
     134,     5,     5,     3,     4,    25,     6,   116,     3,     4,
     119,     6,     3,     4,    25,     6,    32,    50,    46,    47,
     129,    21,    50,     3,     4,    59,     6,   161,   162,    40,
       3,     4,     0,     6,    54,    46,    32,    70,    35,    36,
      68,    75,    70,    77,     0,     1,    50,    50,    60,     5,
      48,    49,   186,    65,    54,    95,    48,    13,    14,    54,
      51,   284,    17,    60,    61,   105,    70,    70,    48,    53,
      82,    17,    18,    19,   108,    48,   110,    22,    48,    24,
      64,    65,   116,   117,   118,   219,   220,     0,     1,    48,
      45,    20,     5,    48,    49,    50,    51,    52,    17,    48,
      13,    14,   236,     5,     6,   280,   281,   282,    50,   143,
       3,   132,   133,     6,   135,   136,   137,   138,   139,    16,
      17,    18,    57,    58,    50,   300,    45,    30,    31,    48,
      49,    50,    51,    52,   168,   169,   170,   171,    48,   173,
       3,     4,    39,     6,    53,    42,    43,    44,    45,     3,
     284,    48,    49,    50,    51,    52,    53,   291,    55,    56,
      57,    39,    37,    38,    42,    43,    44,   276,   277,   278,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,    53,    53,
      55,    56,    57,   302,     9,    10,    11,    12,   307,    51,
      62,    63,     1,     2,     3,     4,     1,    21,     3,     4,
       5,     6,     7,     8,    13,    14,    51,    31,    32,    33,
      34,    35,    36,    37,    23,    48,    49,    22,     5,    24,
      25,    26,    27,    28,    29,     3,     4,     5,     6,     7,
       8,   198,   199,   200,   201,    48,   280,   281,   282,     5,
       6,    46,    47,    48,    54,    50,   193,   194,    53,    54,
      55,    48,   196,   197,   202,   203,   300,    62,    63,    52,
      45,    66,    67,    48,    49,    50,    51,    52,    46,    47,
      48,    48,    50,   204,   205,    53,     3,     4,     5,     6,
       7,     8,    49,    55,    62,    63,    50,    50,    66,    67,
      50,    50,    50,    48,     3,     4,     5,     6,     7,     8,
      33,    48,     3,     4,     5,     6,     7,     8,    39,    40,
      41,    42,    43,    44,    45,    34,    59,    51,    49,    46,
      47,     5,    48,    50,    51,    48,    53,   209,   210,   211,
     212,   213,   214,   215,   216,    62,    63,    46,    47,    66,
      67,    50,    51,    48,    53,    46,    47,    17,    18,    50,
       5,    51,    53,    62,    63,    51,    51,    66,    67,    50,
      50,    62,    63,    48,    52,    66,    67,    16,    17,    18,
      51,    51,    69,    49,    56,    45,    23,    51,    48,    49,
      50,    51,    52,    53,    51,    55,    56,    57,    48,    51,
      39,    48,    48,    42,    43,    44,    45,    51,   143,    48,
      49,    50,    51,    52,    53,   117,    55,    56,    57,     0,
       1,   191,     3,     4,     5,     6,   296,   192,    -1,   195,
      -1,    -1,    13,    14,    15,    16,    17,    18,    19,    20,
       0,     1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      -1,    -1,    -1,    13,    14,    15,    -1,    17,    18,    19,
      20
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     9,    10,    11,    12,    72,     3,     6,    90,    90,
      90,    90,     0,    32,    32,    48,    48,    48,    48,    90,
      90,     1,     5,    13,    14,    15,    16,    17,    18,    19,
      73,    77,    78,    79,    80,    81,    82,    83,    87,     1,
       5,    74,    77,    78,    84,     1,     5,    75,    77,    78,
      79,    80,    82,     1,    76,    77,    78,    82,    73,    50,
      85,    90,     4,    89,    90,    85,    50,    73,    73,    73,
      73,    73,    73,    73,    20,    88,    74,    85,    74,    74,
      74,    75,    85,    75,    75,    75,    75,    75,    76,    76,
      76,    76,    51,    86,    89,    87,    48,    53,     3,    53,
      87,    51,    89,    48,    89,    87,    51,     5,    88,    48,
      54,    91,    91,     5,     6,    48,    88,    52,    21,    89,
      91,     1,     5,     7,     8,    22,    24,    25,    26,    27,
      28,    29,    46,    47,    50,    53,    62,    63,    66,    67,
      89,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
      48,    49,    49,    86,    48,    89,    91,    48,    50,    50,
      50,    50,    91,    50,    48,    89,    96,   107,   107,    96,
     107,   107,   107,   107,   107,     5,    50,    70,    55,    92,
      48,    33,    34,    57,    58,    59,    30,    31,    35,    36,
      60,    61,    37,    38,    62,    63,    53,    64,    65,    39,
      40,    41,    42,    43,    44,    45,    49,    46,    47,    50,
      68,    70,    96,    96,    48,    89,    94,    94,    94,    89,
      22,    24,    94,    48,    51,    48,    49,    51,    96,   110,
       5,     6,    99,   107,   100,   101,   101,   102,   103,   103,
     104,   104,   104,   104,   105,   105,   106,   106,   107,   107,
     107,    97,    97,    97,    97,    97,    97,    97,    97,    51,
     110,    96,     5,    48,    48,     5,    51,    51,    51,     5,
      50,    50,    48,    96,    52,    51,    51,    69,    91,    91,
      91,    56,    94,    94,    94,   110,    23,    96,    51,    51,
      48,    95,    51,    48,    48,    94,    91,    51,    91
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
#line 29 "grammar.y"
	{ type_free((yyvaluep->type)); (yyvaluep->type) = 0; };

/* Line 1000 of yacc.c  */
#line 1437 "grammar.y.c"
	break;
      case 4: /* "TOK_PRIMITIVE" */

/* Line 1000 of yacc.c  */
#line 29 "grammar.y"
	{ type_free((yyvaluep->type)); (yyvaluep->type) = 0; };

/* Line 1000 of yacc.c  */
#line 1446 "grammar.y.c"
	break;
      case 5: /* "TOK_IDENT" */

/* Line 1000 of yacc.c  */
#line 33 "grammar.y"
	{ free((yyvaluep->string)); };

/* Line 1000 of yacc.c  */
#line 1455 "grammar.y.c"
	break;
      case 6: /* "TOK_CONST" */

/* Line 1000 of yacc.c  */
#line 33 "grammar.y"
	{ free((yyvaluep->string)); };

/* Line 1000 of yacc.c  */
#line 1464 "grammar.y.c"
	break;
      case 7: /* "TOK_STRING" */

/* Line 1000 of yacc.c  */
#line 33 "grammar.y"
	{ free((yyvaluep->string)); };

/* Line 1000 of yacc.c  */
#line 1473 "grammar.y.c"
	break;
      case 8: /* "TOK_NUMBER" */

/* Line 1000 of yacc.c  */
#line 33 "grammar.y"
	{ free((yyvaluep->string)); };

/* Line 1000 of yacc.c  */
#line 1482 "grammar.y.c"
	break;
      case 73: /* "class" */

/* Line 1000 of yacc.c  */
#line 30 "grammar.y"
	{ unit_free((yyvaluep->unit)); (yyvaluep->unit) = 0; };

/* Line 1000 of yacc.c  */
#line 1491 "grammar.y.c"
	break;
      case 74: /* "interface" */

/* Line 1000 of yacc.c  */
#line 30 "grammar.y"
	{ unit_free((yyvaluep->unit)); (yyvaluep->unit) = 0; };

/* Line 1000 of yacc.c  */
#line 1500 "grammar.y.c"
	break;
      case 75: /* "struct" */

/* Line 1000 of yacc.c  */
#line 30 "grammar.y"
	{ unit_free((yyvaluep->unit)); (yyvaluep->unit) = 0; };

/* Line 1000 of yacc.c  */
#line 1509 "grammar.y.c"
	break;
      case 76: /* "module" */

/* Line 1000 of yacc.c  */
#line 30 "grammar.y"
	{ unit_free((yyvaluep->unit)); (yyvaluep->unit) = 0; };

/* Line 1000 of yacc.c  */
#line 1518 "grammar.y.c"
	break;
      case 79: /* "variable" */

/* Line 1000 of yacc.c  */
#line 31 "grammar.y"
	{ var_free((yyvaluep->var)); (yyvaluep->var) = 0; };

/* Line 1000 of yacc.c  */
#line 1527 "grammar.y.c"
	break;
      case 80: /* "constructor" */

/* Line 1000 of yacc.c  */
#line 32 "grammar.y"
	{ func_free((yyvaluep->func)); (yyvaluep->func) = 0; };

/* Line 1000 of yacc.c  */
#line 1536 "grammar.y.c"
	break;
      case 81: /* "destructor" */

/* Line 1000 of yacc.c  */
#line 32 "grammar.y"
	{ func_free((yyvaluep->func)); (yyvaluep->func) = 0; };

/* Line 1000 of yacc.c  */
#line 1545 "grammar.y.c"
	break;
      case 82: /* "function" */

/* Line 1000 of yacc.c  */
#line 32 "grammar.y"
	{ func_free((yyvaluep->func)); (yyvaluep->func) = 0; };

/* Line 1000 of yacc.c  */
#line 1554 "grammar.y.c"
	break;
      case 83: /* "native" */

/* Line 1000 of yacc.c  */
#line 32 "grammar.y"
	{ func_free((yyvaluep->func)); (yyvaluep->func) = 0; };

/* Line 1000 of yacc.c  */
#line 1563 "grammar.y.c"
	break;
      case 84: /* "prototype" */

/* Line 1000 of yacc.c  */
#line 32 "grammar.y"
	{ func_free((yyvaluep->func)); (yyvaluep->func) = 0; };

/* Line 1000 of yacc.c  */
#line 1572 "grammar.y.c"
	break;
      case 85: /* "parameter_signature" */

/* Line 1000 of yacc.c  */
#line 31 "grammar.y"
	{ var_free((yyvaluep->var)); (yyvaluep->var) = 0; };

/* Line 1000 of yacc.c  */
#line 1581 "grammar.y.c"
	break;
      case 86: /* "parameter_list" */

/* Line 1000 of yacc.c  */
#line 31 "grammar.y"
	{ var_free((yyvaluep->var)); (yyvaluep->var) = 0; };

/* Line 1000 of yacc.c  */
#line 1590 "grammar.y.c"
	break;
      case 89: /* "type" */

/* Line 1000 of yacc.c  */
#line 29 "grammar.y"
	{ type_free((yyvaluep->type)); (yyvaluep->type) = 0; };

/* Line 1000 of yacc.c  */
#line 1599 "grammar.y.c"
	break;
      case 90: /* "qualified_name" */

/* Line 1000 of yacc.c  */
#line 29 "grammar.y"
	{ type_free((yyvaluep->type)); (yyvaluep->type) = 0; };

/* Line 1000 of yacc.c  */
#line 1608 "grammar.y.c"
	break;
      case 91: /* "block" */

/* Line 1000 of yacc.c  */
#line 28 "grammar.y"
	{ stmt_free((yyvaluep->stmt)); (yyvaluep->stmt) = 0; };

/* Line 1000 of yacc.c  */
#line 1617 "grammar.y.c"
	break;
      case 92: /* "statement_list" */

/* Line 1000 of yacc.c  */
#line 28 "grammar.y"
	{ stmt_free((yyvaluep->stmt)); (yyvaluep->stmt) = 0; };

/* Line 1000 of yacc.c  */
#line 1626 "grammar.y.c"
	break;
      case 93: /* "statement" */

/* Line 1000 of yacc.c  */
#line 28 "grammar.y"
	{ stmt_free((yyvaluep->stmt)); (yyvaluep->stmt) = 0; };

/* Line 1000 of yacc.c  */
#line 1635 "grammar.y.c"
	break;
      case 94: /* "clause" */

/* Line 1000 of yacc.c  */
#line 28 "grammar.y"
	{ stmt_free((yyvaluep->stmt)); (yyvaluep->stmt) = 0; };

/* Line 1000 of yacc.c  */
#line 1644 "grammar.y.c"
	break;
      case 95: /* "conditional" */

/* Line 1000 of yacc.c  */
#line 28 "grammar.y"
	{ stmt_free((yyvaluep->stmt)); (yyvaluep->stmt) = 0; };

/* Line 1000 of yacc.c  */
#line 1653 "grammar.y.c"
	break;
      case 96: /* "expression" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1662 "grammar.y.c"
	break;
      case 97: /* "assignment" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1671 "grammar.y.c"
	break;
      case 98: /* "logical_or" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1680 "grammar.y.c"
	break;
      case 99: /* "logical_and" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1689 "grammar.y.c"
	break;
      case 100: /* "bitwise_or" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1698 "grammar.y.c"
	break;
      case 101: /* "bitwise_and" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1707 "grammar.y.c"
	break;
      case 102: /* "equality" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1716 "grammar.y.c"
	break;
      case 103: /* "relation" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1725 "grammar.y.c"
	break;
      case 104: /* "shift" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1734 "grammar.y.c"
	break;
      case 105: /* "addition" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1743 "grammar.y.c"
	break;
      case 106: /* "multiplication" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1752 "grammar.y.c"
	break;
      case 107: /* "unary" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1761 "grammar.y.c"
	break;
      case 108: /* "postfix" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1770 "grammar.y.c"
	break;
      case 109: /* "primary" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1779 "grammar.y.c"
	break;
      case 110: /* "argument_list" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1788 "grammar.y.c"
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
#line 124 "grammar.y"
    {
        unit_name((yyvsp[(4) - (4)].unit), (yyvsp[(2) - (4)].type)); 
		parser_class(parser, (yyvsp[(4) - (4)].unit));
    ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 128 "grammar.y"
    {
        unit_name((yyvsp[(4) - (4)].unit), (yyvsp[(2) - (4)].type)); 
		parser_interface(parser, (yyvsp[(4) - (4)].unit));
    ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 132 "grammar.y"
    {
        unit_name((yyvsp[(4) - (4)].unit), (yyvsp[(2) - (4)].type)); 
		parser_struct(parser, (yyvsp[(4) - (4)].unit));
    ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 136 "grammar.y"
    {
        unit_name((yyvsp[(4) - (4)].unit), (yyvsp[(2) - (4)].type));
		parser_module(parser, (yyvsp[(4) - (4)].unit));
    ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 143 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 144 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 145 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_var((yyval.unit), (yyvsp[(1) - (2)].var)); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 146 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_ctor((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 147 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_dtor((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 148 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_func((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 149 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_func((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 150 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 151 "grammar.y"
    { (yyval.unit) = unit_alloc(UNIT_TYPE_CLASS); ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 155 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 156 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 157 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_func((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 158 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 159 "grammar.y"
    { (yyval.unit) = unit_alloc(UNIT_TYPE_INTERFACE); ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 163 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 164 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 165 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_var((yyval.unit), (yyvsp[(1) - (2)].var)); ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 166 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_ctor((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 167 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_func((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 168 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 169 "grammar.y"
    { (yyval.unit) = unit_alloc(UNIT_TYPE_STRUCT); ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 173 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 174 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 175 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_func((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 176 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 177 "grammar.y"
    { (yyval.unit) = unit_alloc(UNIT_TYPE_MODULE); ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 181 "grammar.y"
    {
        //parser_import(parser, $2);
		//$$ = 0;
    ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 188 "grammar.y"
    {
        //parser_def(parser, $2, $3);
		//$$ = 0;
    ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 195 "grammar.y"
    {
		(yyval.var) = var_alloc((yyvsp[(4) - (7)].string), (yyvsp[(1) - (7)].flag)|(yyvsp[(2) - (7)].flag), (yyvsp[(3) - (7)].type), (yyvsp[(6) - (7)].expr));
    ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 198 "grammar.y"
    {
		(yyval.var) = var_alloc((yyvsp[(4) - (5)].string), (yyvsp[(1) - (5)].flag)|(yyvsp[(2) - (5)].flag), (yyvsp[(3) - (5)].type), 0);
    ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 201 "grammar.y"
    {
		(yyval.var) = var_alloc((yyvsp[(4) - (7)].string), (yyvsp[(1) - (7)].flag)|(yyvsp[(2) - (7)].flag)|UNIT_FLAG_CONST, (yyvsp[(3) - (7)].type), (yyvsp[(6) - (7)].expr));
	;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 207 "grammar.y"
    { 
		(yyval.func) = func_alloc("@init", (yyvsp[(2) - (4)].var), 0, (yyvsp[(4) - (4)].stmt));
		(yyval.func)->flags = (yyvsp[(3) - (4)].flag);
	;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 214 "grammar.y"
    { 
		(yyval.func) = func_alloc("@destroy", 0, 0, (yyvsp[(4) - (4)].stmt)); 
	;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 220 "grammar.y"
    {
		(yyval.func) = func_alloc((yyvsp[(1) - (6)].string), (yyvsp[(2) - (6)].var), (yyvsp[(5) - (6)].type), (yyvsp[(6) - (6)].stmt));
		(yyval.func)->flags = (yyvsp[(3) - (6)].flag)|(yyvsp[(4) - (6)].flag);
    ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 224 "grammar.y"
    {
		(yyval.func) = func_alloc((yyvsp[(1) - (5)].string), (yyvsp[(2) - (5)].var), 0, (yyvsp[(5) - (5)].stmt));
		(yyval.func)->flags = (yyvsp[(3) - (5)].flag)|(yyvsp[(4) - (5)].flag);
	;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 231 "grammar.y"
    {
		(yyval.func) = func_alloc((yyvsp[(1) - (7)].string), (yyvsp[(2) - (7)].var), (yyvsp[(6) - (7)].type), 0); 
		(yyval.func)->flags = (yyvsp[(3) - (7)].flag)|(yyvsp[(4) - (7)].flag)|UNIT_FLAG_NATIVE;
	;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 235 "grammar.y"
    {
		(yyval.func) = func_alloc((yyvsp[(1) - (6)].string), (yyvsp[(2) - (6)].var), 0, 0);
		(yyval.func)->flags = (yyvsp[(3) - (6)].flag)|(yyvsp[(4) - (6)].flag)|UNIT_FLAG_NATIVE;
	;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 242 "grammar.y"
    {
		(yyval.func) = func_alloc((yyvsp[(1) - (4)].string), (yyvsp[(2) - (4)].var), (yyvsp[(3) - (4)].type), 0);
    ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 245 "grammar.y"
    {
		(yyval.func) = func_alloc((yyvsp[(1) - (3)].string), (yyvsp[(2) - (3)].var), 0, 0);
	;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 251 "grammar.y"
    { (yyval.var) = (yyvsp[(2) - (3)].var); ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 252 "grammar.y"
    { (yyval.var) = 0; ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 256 "grammar.y"
    { 
		(yyval.var) = (yyvsp[(4) - (4)].var);
		(yyval.var)->next = var_alloc((yyvsp[(2) - (4)].string), 0, (yyvsp[(1) - (4)].type), 0); 
	;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 260 "grammar.y"
    { (yyval.var) = var_alloc((yyvsp[(2) - (2)].string), 0, (yyvsp[(1) - (2)].type), 0); ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 264 "grammar.y"
    { (yyval.flag) = UNIT_FLAG_PUBLIC; ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 265 "grammar.y"
    { (yyval.flag) = UNIT_FLAG_PRIVATE; ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 266 "grammar.y"
    { (yyval.flag) = UNIT_FLAG_PROTECTED; ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 267 "grammar.y"
    { (yyval.flag) = 0; ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 271 "grammar.y"
    { (yyval.flag) = UNIT_FLAG_STATIC; ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 272 "grammar.y"
    { (yyval.flag) = 0; ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 276 "grammar.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 277 "grammar.y"
    { (yyval.type) = (yyvsp[(1) - (2)].type); /* TODO: Array */ ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 278 "grammar.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 279 "grammar.y"
    { (yyval.type) = (yyvsp[(1) - (2)].type); /* TODO: Array */ ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 283 "grammar.y"
    { (yyval.type) = type_concat((yyvsp[(1) - (3)].type), (yyvsp[(3) - (3)].type)); ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 284 "grammar.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 285 "grammar.y"
    { 
		(yyval.type) = type_concat(type_name((yyvsp[(1) - (3)].string)), (yyvsp[(3) - (3)].type)); 
		free((yyvsp[(1) - (3)].string));
	;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 289 "grammar.y"
    { 
		(yyval.type) = type_name((yyvsp[(1) - (1)].string)); 
		free((yyvsp[(1) - (1)].string));
	;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 296 "grammar.y"
    { (yyval.stmt) = (yyvsp[(2) - (3)].stmt); ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 300 "grammar.y"
    { (yyval.stmt) = stmt_append((yyvsp[(2) - (2)].stmt), (yyvsp[(1) - (2)].stmt)); ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 301 "grammar.y"
    { (yyval.stmt) = stmt_block(); ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 305 "grammar.y"
    {
		(yyval.stmt) = stmt_for((yyvsp[(3) - (9)].stmt), (yyvsp[(5) - (9)].stmt), (yyvsp[(7) - (9)].stmt), (yyvsp[(9) - (9)].stmt));
	;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 308 "grammar.y"
    {
		(yyval.stmt) = stmt_foreach(var_alloc((yyvsp[(4) - (8)].string), 0, (yyvsp[(3) - (8)].type), (yyvsp[(6) - (8)].expr)), (yyvsp[(8) - (8)].stmt));
	;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 311 "grammar.y"
    {
		(yyval.stmt) = stmt_until((yyvsp[(3) - (5)].stmt), (yyvsp[(5) - (5)].stmt));
	;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 314 "grammar.y"
    {
		(yyval.stmt) = stmt_while((yyvsp[(3) - (5)].stmt), (yyvsp[(5) - (5)].stmt));
	;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 317 "grammar.y"
    {
		(yyval.stmt) = stmt_dountil((yyvsp[(2) - (7)].stmt), (yyvsp[(5) - (7)].stmt));
	;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 320 "grammar.y"
    {
		(yyval.stmt) = stmt_dowhile((yyvsp[(2) - (7)].stmt), (yyvsp[(5) - (7)].stmt));
	;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 323 "grammar.y"
    { 
		(yyval.stmt) = stmt_decl(var_alloc((yyvsp[(2) - (3)].string), 0, (yyvsp[(1) - (3)].type), 0)); 
	;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 326 "grammar.y"
    { (yyval.stmt) = (yyvsp[(1) - (1)].stmt); ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 327 "grammar.y"
    { (yyval.stmt) = (yyvsp[(1) - (2)].stmt); ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 328 "grammar.y"
    { (yyval.stmt) = 0; ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 329 "grammar.y"
    { (yyval.stmt) = stmt_return((yyvsp[(2) - (3)].expr)); ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 330 "grammar.y"
    { (yyval.stmt) = stmt_return(0); ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 334 "grammar.y"
    { 
		(yyval.stmt) = stmt_decl(var_alloc((yyvsp[(2) - (4)].string), 0, (yyvsp[(1) - (4)].type), (yyvsp[(4) - (4)].expr))); 
	;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 337 "grammar.y"
    { 
		(yyval.stmt) = stmt_expr((yyvsp[(1) - (1)].expr)); 
	;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 340 "grammar.y"
    { (yyval.stmt) = 0; ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 344 "grammar.y"
    {
		(yyval.stmt) = stmt_conditional((yyvsp[(3) - (5)].stmt), (yyvsp[(5) - (5)].stmt), 0);
	;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 347 "grammar.y"
    {
		(yyval.stmt) = stmt_conditional((yyvsp[(3) - (7)].stmt), (yyvsp[(5) - (7)].stmt), (yyvsp[(7) - (7)].stmt));
	;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 350 "grammar.y"
    { (yyval.stmt) = (yyvsp[(1) - (1)].stmt); ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 353 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 356 "grammar.y"
    { 
		(yyval.expr) = expr_binary(op_assign, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 359 "grammar.y"
    { 
		(yyval.expr) = expr_binary(op_mul_assign, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 362 "grammar.y"
    { 
		(yyval.expr) = expr_binary(op_div_assign, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 365 "grammar.y"
    { 
		(yyval.expr) = expr_binary(op_mod_assign, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 368 "grammar.y"
    { 
		(yyval.expr) = expr_binary(op_sub_assign, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 371 "grammar.y"
    { 
		(yyval.expr) = expr_binary(op_add_assign, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 374 "grammar.y"
    { 
		(yyval.expr) = expr_binary(op_bitand_assign, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 377 "grammar.y"
    { 
		(yyval.expr) = expr_binary(op_bitor_assign, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 380 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 384 "grammar.y"
    { (yyval.expr) = expr_binary(op_or, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 385 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 389 "grammar.y"
    { (yyval.expr) = expr_binary(op_and, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 390 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 394 "grammar.y"
    { (yyval.expr) = expr_binary(op_bitor, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 395 "grammar.y"
    { (yyval.expr) = expr_binary(op_bitxor, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 396 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 400 "grammar.y"
    { (yyval.expr) = expr_binary(op_bitand, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 401 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 405 "grammar.y"
    { (yyval.expr) = expr_binary(op_equal, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 406 "grammar.y"
    { 
		(yyval.expr) = expr_binary(op_notequal, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 409 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 413 "grammar.y"
    { (yyval.expr) = expr_binary(op_greater, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 414 "grammar.y"
    { (yyval.expr) = expr_binary(op_less, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 415 "grammar.y"
    { (yyval.expr) = expr_binary(op_ge, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 416 "grammar.y"
    { (yyval.expr) = expr_binary(op_le, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 417 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 421 "grammar.y"
    { (yyval.expr) = expr_binary(op_lshift, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 422 "grammar.y"
    { (yyval.expr) = expr_binary(op_rshift, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 423 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 427 "grammar.y"
    { (yyval.expr) = expr_binary(op_plus, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 428 "grammar.y"
    { (yyval.expr) = expr_binary(op_minus, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 429 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 433 "grammar.y"
    { (yyval.expr) = expr_binary(op_mul, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 434 "grammar.y"
    { (yyval.expr) = expr_binary(op_div, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 435 "grammar.y"
    { (yyval.expr) = expr_binary(op_mod, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 436 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 440 "grammar.y"
    { (yyval.expr) = expr_unary(op_inc, (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 441 "grammar.y"
    { (yyval.expr) = expr_unary(op_dec, (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 442 "grammar.y"
    { (yyval.expr) = expr_unary(op_plus, (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 443 "grammar.y"
    { (yyval.expr) = expr_unary(op_minus, (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 444 "grammar.y"
    { (yyval.expr) = expr_unary(op_bang, (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 445 "grammar.y"
    { (yyval.expr) = expr_unary(op_tilde, (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 446 "grammar.y"
    { (yyval.expr) = expr_unary(op_star, (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 447 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 451 "grammar.y"
    { (yyval.expr) = expr_call((yyvsp[(1) - (4)].expr), (yyvsp[(3) - (4)].expr)); ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 452 "grammar.y"
    { (yyval.expr) = expr_call((yyvsp[(1) - (3)].expr), 0); ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 453 "grammar.y"
    { (yyval.expr) = expr_index((yyvsp[(1) - (4)].expr), (yyvsp[(3) - (4)].expr)); ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 454 "grammar.y"
    { (yyval.expr) = expr_member((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].string)); ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 455 "grammar.y"
    { (yyval.expr) = expr_unary(op_postinc, (yyvsp[(1) - (2)].expr)); ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 456 "grammar.y"
    { (yyval.expr) = expr_unary(op_postdec, (yyvsp[(1) - (2)].expr)); ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 457 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 461 "grammar.y"
    { (yyval.expr) = expr_string((yyvsp[(1) - (1)].string)); ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 462 "grammar.y"
    { (yyval.expr) = expr_string((yyvsp[(1) - (1)].string)); ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 463 "grammar.y"
    { (yyval.expr) = expr_string((yyvsp[(1) - (1)].string)); ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 464 "grammar.y"
    { (yyval.expr) = expr_string("FIXME"); ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 465 "grammar.y"
    { (yyval.expr) = expr_string("FIXME"); ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 466 "grammar.y"
    { (yyval.expr) = expr_call(0, (yyvsp[(3) - (4)].expr)); ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 467 "grammar.y"
    { (yyval.expr) = expr_call(0, 0); ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 468 "grammar.y"
    { (yyval.expr) = (yyvsp[(2) - (3)].expr); ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 472 "grammar.y"
    { 
		(yyval.expr) = (yyvsp[(3) - (3)].expr);
		(yyval.expr)->next = (yyvsp[(1) - (3)].expr);
	;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 476 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;



/* Line 1455 of yacc.c  */
#line 3198 "grammar.y.c"
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



