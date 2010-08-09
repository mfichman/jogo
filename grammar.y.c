
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
     TOK_STRING = 261,
     TOK_NUMBER = 262,
     TOK_CLASS = 263,
     TOK_INTERFACE = 264,
     TOK_STRUCT = 265,
     TOK_MODULE = 266,
     TOK_IMPORT = 267,
     TOK_DEF = 268,
     TOK_INIT = 269,
     TOK_DESTROY = 270,
     TOK_PUBLIC = 271,
     TOK_PRIVATE = 272,
     TOK_PROTECTED = 273,
     TOK_STATIC = 274,
     TOK_NATIVE = 275,
     TOK_WHILE = 276,
     TOK_ELSE = 277,
     TOK_UNTIL = 278,
     TOK_IF = 279,
     TOK_FOREACH = 280,
     TOK_DO = 281,
     TOK_FOR = 282,
     TOK_EQUAL = 283,
     TOK_NOTEQUAL = 284,
     TOK_SCOPE = 285,
     TOK_OR = 286,
     TOK_AND = 287,
     TOK_GE = 288,
     TOK_LE = 289,
     TOK_LSHIFT = 290,
     TOK_RSHIFT = 291,
     TOK_MUL_ASSIGN = 292,
     TOK_DIV_ASSIGN = 293,
     TOK_SUB_ASSIGN = 294,
     TOK_ADD_ASSIGN = 295,
     TOK_MOD_ASSIGN = 296,
     TOK_BITOR_ASSIGN = 297,
     TOK_BITAND_ASSIGN = 298,
     TOK_INC = 299,
     TOK_DEC = 300
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
#line 202 "grammar.y.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 214 "grammar.y.c"

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
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   351

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  68
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  38
/* YYNRULES -- Number of rules.  */
#define YYNRULES  125
/* YYNRULES -- Number of states.  */
#define YYNSTATES  266

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   300

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    63,     2,     2,     2,    62,    55,     2,
      48,    49,    60,    58,     2,    59,    67,    61,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    52,    46,
      57,    47,    56,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    65,     2,    66,    54,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    50,    53,    51,    64,     2,     2,     2,
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
      45
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     8,    13,    18,    23,    26,    29,    32,
      35,    38,    41,    44,    45,    48,    51,    54,    57,    58,
      61,    64,    67,    70,    73,    76,    77,    80,    83,    86,
      89,    90,    94,    99,   107,   113,   118,   123,   131,   136,
     139,   141,   143,   145,   146,   148,   149,   151,   152,   154,
     156,   160,   162,   166,   169,   170,   180,   189,   195,   201,
     209,   217,   221,   223,   226,   229,   234,   236,   237,   243,
     251,   253,   255,   259,   263,   267,   271,   275,   279,   283,
     287,   289,   293,   295,   299,   301,   305,   309,   311,   315,
     317,   321,   325,   327,   331,   335,   339,   343,   345,   349,
     353,   355,   359,   363,   365,   369,   373,   377,   379,   382,
     385,   388,   391,   394,   397,   400,   402,   405,   410,   414,
     417,   420,   422,   424,   426,   428
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      69,     0,    -1,     8,    86,    46,    70,    -1,     9,    86,
      46,    71,    -1,    10,    86,    46,    72,    -1,    11,    86,
      46,    73,    -1,    74,    70,    -1,    75,    70,    -1,    76,
      70,    -1,    77,    70,    -1,    78,    70,    -1,    79,    70,
      -1,     1,    70,    -1,    -1,    74,    71,    -1,    75,    71,
      -1,    80,    71,    -1,     1,    71,    -1,    -1,    74,    72,
      -1,    75,    72,    -1,    76,    72,    -1,    77,    72,    -1,
      79,    72,    -1,     1,    72,    -1,    -1,    74,    73,    -1,
      75,    73,    -1,    79,    73,    -1,     1,    73,    -1,    -1,
      12,    86,    46,    -1,    13,    85,     3,    46,    -1,    82,
      83,    85,     5,    47,    92,    46,    -1,    82,    83,    85,
       5,    46,    -1,    14,    81,    82,    87,    -1,    15,    48,
      49,    87,    -1,     5,    81,    82,    83,    84,    85,    87,
      -1,     5,    81,    85,    46,    -1,    48,    49,    -1,    16,
      -1,    17,    -1,    18,    -1,    -1,    19,    -1,    -1,    20,
      -1,    -1,     4,    -1,    86,    -1,     3,    30,    86,    -1,
       3,    -1,    50,    88,    51,    -1,    89,    88,    -1,    -1,
      27,    48,    90,    46,    90,    46,    90,    49,    87,    -1,
      25,    48,    85,     5,    52,    92,    49,    87,    -1,    23,
      48,    90,    49,    87,    -1,    21,    48,    90,    49,    87,
      -1,    26,    87,    23,    48,    90,    49,    46,    -1,    26,
      87,    21,    48,    90,    49,    46,    -1,    85,     5,    46,
      -1,    91,    -1,    90,    46,    -1,     1,    46,    -1,    85,
       5,    47,    92,    -1,    92,    -1,    -1,    24,    48,    90,
      49,    87,    -1,    24,    48,    90,    49,    87,    22,    91,
      -1,    87,    -1,    93,    -1,   103,    47,    93,    -1,   103,
      37,    93,    -1,   103,    38,    93,    -1,   103,    41,    93,
      -1,   103,    39,    93,    -1,   103,    40,    93,    -1,   103,
      43,    93,    -1,   103,    42,    93,    -1,    94,    -1,    94,
      31,    95,    -1,    95,    -1,    95,    32,    96,    -1,    96,
      -1,    96,    53,    97,    -1,    96,    54,    97,    -1,    97,
      -1,    97,    55,    98,    -1,    98,    -1,    98,    28,    99,
      -1,    98,    29,    99,    -1,    99,    -1,    99,    56,   100,
      -1,    99,    57,   100,    -1,    99,    33,   100,    -1,    99,
      34,   100,    -1,   100,    -1,   100,    35,   101,    -1,   100,
      36,   101,    -1,   101,    -1,   101,    58,   102,    -1,   101,
      59,   102,    -1,   102,    -1,   102,    60,   103,    -1,   102,
      61,   103,    -1,   102,    62,   103,    -1,   103,    -1,    44,
     103,    -1,    45,   103,    -1,    58,   103,    -1,    59,   103,
      -1,    63,   103,    -1,    64,   103,    -1,    60,   103,    -1,
     104,    -1,   104,    81,    -1,   104,    65,    92,    66,    -1,
     104,    67,     5,    -1,   104,    44,    -1,   104,    45,    -1,
     105,    -1,     6,    -1,     7,    -1,     5,    -1,    48,    92,
      49,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   120,   120,   124,   128,   132,   139,   140,   141,   142,
     143,   144,   145,   146,   150,   151,   152,   153,   154,   158,
     159,   160,   161,   162,   163,   164,   168,   169,   170,   171,
     172,   176,   183,   190,   193,   199,   206,   212,   219,   225,
     229,   230,   231,   232,   236,   237,   241,   242,   246,   247,
     251,   252,   256,   260,   261,   265,   268,   271,   274,   277,
     280,   283,   286,   287,   288,   292,   295,   298,   302,   305,
     308,   311,   314,   317,   320,   323,   326,   329,   332,   335,
     338,   342,   343,   347,   348,   352,   353,   354,   358,   359,
     363,   364,   367,   371,   372,   373,   374,   375,   379,   380,
     381,   385,   386,   387,   391,   392,   393,   394,   398,   399,
     400,   401,   402,   403,   404,   405,   409,   410,   411,   412,
     413,   414,   418,   419,   420,   421
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOK_TYPE", "TOK_PRIMITIVE", "TOK_IDENT",
  "TOK_STRING", "TOK_NUMBER", "TOK_CLASS", "TOK_INTERFACE", "TOK_STRUCT",
  "TOK_MODULE", "TOK_IMPORT", "TOK_DEF", "TOK_INIT", "TOK_DESTROY",
  "TOK_PUBLIC", "TOK_PRIVATE", "TOK_PROTECTED", "TOK_STATIC", "TOK_NATIVE",
  "TOK_WHILE", "TOK_ELSE", "TOK_UNTIL", "TOK_IF", "TOK_FOREACH", "TOK_DO",
  "TOK_FOR", "TOK_EQUAL", "TOK_NOTEQUAL", "TOK_SCOPE", "TOK_OR", "TOK_AND",
  "TOK_GE", "TOK_LE", "TOK_LSHIFT", "TOK_RSHIFT", "TOK_MUL_ASSIGN",
  "TOK_DIV_ASSIGN", "TOK_SUB_ASSIGN", "TOK_ADD_ASSIGN", "TOK_MOD_ASSIGN",
  "TOK_BITOR_ASSIGN", "TOK_BITAND_ASSIGN", "TOK_INC", "TOK_DEC", "';'",
  "'='", "'('", "')'", "'{'", "'}'", "':'", "'|'", "'^'", "'&'", "'>'",
  "'<'", "'+'", "'-'", "'*'", "'/'", "'%'", "'!'", "'~'", "'['", "']'",
  "'.'", "$accept", "translation_unit", "class", "interface", "struct",
  "module", "import", "def", "variable", "constructor", "destructor",
  "function", "prototype", "argument_list", "access", "storage", "native",
  "type", "qualified_name", "block", "statement_list", "statement",
  "clause", "conditional", "expression", "assignment", "logical_or",
  "logical_and", "bitwise_or", "bitwise_and", "equality", "relation",
  "shift", "addition", "multiplication", "unary", "postfix", "primary", 0
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
     295,   296,   297,   298,   299,   300,    59,    61,    40,    41,
     123,   125,    58,   124,    94,    38,    62,    60,    43,    45,
      42,    47,    37,    33,   126,    91,    93,    46
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    68,    69,    69,    69,    69,    70,    70,    70,    70,
      70,    70,    70,    70,    71,    71,    71,    71,    71,    72,
      72,    72,    72,    72,    72,    72,    73,    73,    73,    73,
      73,    74,    75,    76,    76,    77,    78,    79,    80,    81,
      82,    82,    82,    82,    83,    83,    84,    84,    85,    85,
      86,    86,    87,    88,    88,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    90,    90,    90,    91,    91,
      91,    92,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    94,    94,    95,    95,    96,    96,    96,    97,    97,
      98,    98,    98,    99,    99,    99,    99,    99,   100,   100,
     100,   101,   101,   101,   102,   102,   102,   102,   103,   103,
     103,   103,   103,   103,   103,   103,   104,   104,   104,   104,
     104,   104,   105,   105,   105,   105
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     4,     4,     4,     2,     2,     2,     2,
       2,     2,     2,     0,     2,     2,     2,     2,     0,     2,
       2,     2,     2,     2,     2,     0,     2,     2,     2,     2,
       0,     3,     4,     7,     5,     4,     4,     7,     4,     2,
       1,     1,     1,     0,     1,     0,     1,     0,     1,     1,
       3,     1,     3,     2,     0,     9,     8,     5,     5,     7,
       7,     3,     1,     2,     2,     4,     1,     0,     5,     7,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     3,     1,     3,     1,     3,     3,     1,     3,     1,
       3,     3,     1,     3,     3,     3,     3,     1,     3,     3,
       1,     3,     3,     1,     3,     3,     3,     1,     2,     2,
       2,     2,     2,     2,     2,     1,     2,     4,     3,     2,
       2,     1,     1,     1,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,    51,     0,     0,     0,
       0,     1,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,    40,    41,    42,     2,     0,     0,
       0,     0,     0,     0,    45,     0,     0,     3,     0,     0,
       0,     0,     4,     0,     0,     0,     0,     0,     0,     5,
       0,     0,     0,    12,     0,    43,     0,    48,     0,    49,
      43,     0,     6,     7,     8,     9,    10,    11,    44,     0,
      17,     0,    14,    15,    16,    24,    19,    20,    21,    22,
      23,    29,    26,    27,    28,    39,    45,    31,     0,     0,
       0,     0,     0,    47,    32,     0,    35,    36,     0,    38,
      46,     0,     0,   124,   122,   123,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    70,     0,     0,     0,    62,    66,    71,    80,    82,
      84,    87,    89,    92,    97,   100,   103,   107,   115,   121,
      34,     0,     0,    64,    67,    67,    67,     0,     0,    67,
     108,   109,     0,   110,   111,   114,   112,   113,     0,    52,
      53,    63,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   119,   120,
       0,     0,   116,     0,    37,     0,     0,     0,     0,     0,
       0,     0,     0,   125,    61,     0,    81,   107,    83,    85,
      86,    88,    90,    91,    95,    96,    93,    94,    98,    99,
     101,   102,   104,   105,   106,    73,    74,    76,    77,    75,
      79,    78,    72,     0,   118,    33,     0,     0,     0,     0,
       0,    67,    67,    67,    65,   117,    58,    57,    68,     0,
       0,     0,     0,     0,     0,     0,     0,    67,    69,     0,
      60,    59,     0,    56,     0,    55
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,    27,    37,    42,    49,    28,    29,    30,    31,
      32,    33,    40,    55,    34,    69,   101,   195,    59,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -138
static const yytype_int16 yypact[] =
{
     258,    19,    19,    19,    19,    31,     9,     0,     6,    11,
      13,  -138,    19,   260,   121,   280,   290,  -138,   260,     7,
      19,    95,     7,    17,  -138,  -138,  -138,  -138,   260,   260,
     260,   260,   260,   260,    60,   121,     7,  -138,   121,   121,
     121,   280,  -138,   280,   280,   280,   280,   280,   290,  -138,
     290,   290,   290,  -138,    28,    -2,    45,  -138,    91,  -138,
      -2,    78,  -138,  -138,  -138,  -138,  -138,  -138,  -138,    95,
    -138,    95,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,
    -138,  -138,  -138,  -138,  -138,  -138,    60,  -138,    61,   105,
     105,   158,   119,   151,  -138,    37,  -138,  -138,    56,  -138,
    -138,    95,   130,  -138,  -138,  -138,   162,   169,   177,   181,
     105,   183,   109,   109,   109,   109,   109,   109,   109,   109,
     233,  -138,   198,    37,   210,  -138,  -138,  -138,   231,   250,
      58,   268,    89,    33,   110,    93,   251,   267,    99,  -138,
    -138,   109,   105,  -138,    65,    65,    65,    95,    -3,    65,
    -138,  -138,   275,  -138,  -138,  -138,  -138,  -138,    84,  -138,
    -138,  -138,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,  -138,  -138,
     109,   320,  -138,   281,  -138,   321,   279,   282,   283,   324,
     285,   286,   284,  -138,  -138,   109,   250,  -138,    58,   268,
     268,    89,    33,    33,   110,   110,   110,   110,    93,    93,
     251,   251,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,
    -138,  -138,  -138,   269,  -138,  -138,   289,   105,   105,   105,
     287,    65,    65,    65,  -138,  -138,  -138,  -138,   315,   109,
     291,   292,   296,   -13,   294,   298,   299,    65,  -138,   105,
    -138,  -138,   297,  -138,   105,  -138
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -138,  -138,    -5,   139,   115,   176,   168,   207,   -11,     4,
    -138,   189,  -138,   -19,   -50,   252,  -138,   -15,    72,   -89,
     224,  -138,  -137,    96,  -112,   135,  -138,   186,   187,    16,
     185,    76,   117,    97,   125,    23,  -138,  -138
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -68
static const yytype_int16 yytable[] =
{
      96,    97,   152,    60,    45,    86,    58,   196,   197,   198,
      89,   108,   202,    53,    24,    25,    26,    71,   200,    46,
     201,   148,     6,    62,    63,    64,    65,    66,    67,   193,
      45,    11,    45,    45,    45,    45,    45,    95,   102,    12,
       6,    57,   103,   104,   105,    46,    13,    46,    46,    46,
      46,    46,    14,   194,    91,    54,    92,    15,   106,    16,
     107,   108,   109,   110,   111,    61,   169,   170,     6,    57,
     103,   104,   105,     7,     8,     9,    10,    85,   233,    68,
     120,   112,   113,   -67,    17,   114,   142,    95,   -54,   171,
     172,    87,    56,   244,    88,   115,   116,   117,     6,    57,
     118,   119,   140,   141,   250,   251,   252,    94,   120,   112,
     113,   164,   165,   114,   103,   104,   105,   167,   168,   192,
     262,   -18,    35,   115,   116,   117,    36,    90,   118,   119,
     204,   205,   199,    20,    21,   150,   151,   254,   153,   154,
     155,   156,   157,   188,   189,   173,   174,    54,   246,   247,
     248,   175,   176,   112,   113,    95,    75,   114,    76,    77,
      78,    79,    80,    98,   190,    99,   191,   115,   116,   117,
     263,   100,   118,   119,    70,   265,   143,    72,    73,    74,
     209,   210,    38,    43,    50,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     222,   223,   224,    38,    47,    52,    38,    38,    38,    43,
     144,    43,    43,    43,    43,    43,    50,   145,    50,    50,
      50,    39,    44,    51,    81,   146,    82,    83,    84,   147,
      47,   149,    47,    47,    47,    47,    47,    52,   158,    52,
      52,    52,    39,   212,   213,    39,    39,    39,    44,   159,
      44,    44,    44,    44,    44,    51,   161,    51,    51,    51,
     -13,    18,   162,   -43,   -43,    19,     1,     2,     3,     4,
     218,   219,    20,    21,    22,    23,    24,    25,    26,   -43,
     -25,    41,   163,   -43,   -43,    19,   214,   215,   216,   217,
     -30,    48,    20,    21,    22,    19,    24,    25,    26,   -43,
     220,   221,    20,    21,   180,   181,   182,   183,   184,   185,
     186,   177,   178,   179,   187,   225,   226,   227,   228,   229,
     230,   231,   232,   166,   203,   234,   236,   235,   237,   240,
     243,   238,   239,   241,   242,   245,   205,   253,    93,   249,
     255,   256,   257,   259,   260,   261,   264,   160,   206,   258,
     208,   211
};

static const yytype_uint16 yycheck[] =
{
      89,    90,   114,    22,    15,    55,    21,   144,   145,   146,
      60,    24,   149,    18,    16,    17,    18,    36,    21,    15,
      23,   110,     3,    28,    29,    30,    31,    32,    33,   141,
      41,     0,    43,    44,    45,    46,    47,    50,     1,    30,
       3,     4,     5,     6,     7,    41,    46,    43,    44,    45,
      46,    47,    46,   142,    69,    48,    71,    46,    21,    46,
      23,    24,    25,    26,    27,    48,    33,    34,     3,     4,
       5,     6,     7,     1,     2,     3,     4,    49,   190,    19,
      95,    44,    45,    46,    12,    48,   101,    50,    51,    56,
      57,    46,    20,   205,     3,    58,    59,    60,     3,     4,
      63,    64,    46,    47,   241,   242,   243,    46,   123,    44,
      45,    53,    54,    48,     5,     6,     7,    28,    29,   138,
     257,     0,     1,    58,    59,    60,     5,    49,    63,    64,
      46,    47,   147,    12,    13,   112,   113,   249,   115,   116,
     117,   118,   119,    44,    45,    35,    36,    48,   237,   238,
     239,    58,    59,    44,    45,    50,    41,    48,    43,    44,
      45,    46,    47,     5,    65,    46,    67,    58,    59,    60,
     259,    20,    63,    64,    35,   264,    46,    38,    39,    40,
     164,   165,    14,    15,    16,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,    35,    15,    16,    38,    39,    40,    41,
      48,    43,    44,    45,    46,    47,    48,    48,    50,    51,
      52,    14,    15,    16,    48,    48,    50,    51,    52,    48,
      41,    48,    43,    44,    45,    46,    47,    48,     5,    50,
      51,    52,    35,   167,   168,    38,    39,    40,    41,    51,
      43,    44,    45,    46,    47,    48,    46,    50,    51,    52,
       0,     1,    31,     3,     4,     5,     8,     9,    10,    11,
     173,   174,    12,    13,    14,    15,    16,    17,    18,    19,
       0,     1,    32,     3,     4,     5,   169,   170,   171,   172,
       0,     1,    12,    13,    14,     5,    16,    17,    18,    19,
     175,   176,    12,    13,    37,    38,    39,    40,    41,    42,
      43,    60,    61,    62,    47,   180,   181,   182,   183,   184,
     185,   186,   187,    55,    49,     5,     5,    46,    49,     5,
      46,    49,    49,    48,    48,    66,    47,    22,    86,    52,
      49,    49,    46,    49,    46,    46,    49,   123,   162,   253,
     163,   166
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     8,     9,    10,    11,    69,     3,    86,    86,    86,
      86,     0,    30,    46,    46,    46,    46,    86,     1,     5,
      12,    13,    14,    15,    16,    17,    18,    70,    74,    75,
      76,    77,    78,    79,    82,     1,     5,    71,    74,    75,
      80,     1,    72,    74,    75,    76,    77,    79,     1,    73,
      74,    75,    79,    70,    48,    81,    86,     4,    85,    86,
      81,    48,    70,    70,    70,    70,    70,    70,    19,    83,
      71,    81,    71,    71,    71,    72,    72,    72,    72,    72,
      72,    73,    73,    73,    73,    49,    82,    46,     3,    82,
      49,    85,    85,    83,    46,    50,    87,    87,     5,    46,
      20,    84,     1,     5,     6,     7,    21,    23,    24,    25,
      26,    27,    44,    45,    48,    58,    59,    60,    63,    64,
      85,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
      46,    47,    85,    46,    48,    48,    48,    48,    87,    48,
     103,   103,    92,   103,   103,   103,   103,   103,     5,    51,
      88,    46,    31,    32,    53,    54,    55,    28,    29,    33,
      34,    56,    57,    35,    36,    58,    59,    60,    61,    62,
      37,    38,    39,    40,    41,    42,    43,    47,    44,    45,
      65,    67,    81,    92,    87,    85,    90,    90,    90,    85,
      21,    23,    90,    49,    46,    47,    95,   103,    96,    97,
      97,    98,    99,    99,   100,   100,   100,   100,   101,   101,
     102,   102,   103,   103,   103,    93,    93,    93,    93,    93,
      93,    93,    93,    92,     5,    46,     5,    49,    49,    49,
       5,    48,    48,    46,    92,    66,    87,    87,    87,    52,
      90,    90,    90,    22,    92,    49,    49,    46,    91,    49,
      46,    46,    90,    87,    49,    87
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
#line 1379 "grammar.y.c"
	break;
      case 4: /* "TOK_PRIMITIVE" */

/* Line 1000 of yacc.c  */
#line 29 "grammar.y"
	{ type_free((yyvaluep->type)); (yyvaluep->type) = 0; };

/* Line 1000 of yacc.c  */
#line 1388 "grammar.y.c"
	break;
      case 5: /* "TOK_IDENT" */

/* Line 1000 of yacc.c  */
#line 33 "grammar.y"
	{ free((yyvaluep->string)); };

/* Line 1000 of yacc.c  */
#line 1397 "grammar.y.c"
	break;
      case 6: /* "TOK_STRING" */

/* Line 1000 of yacc.c  */
#line 33 "grammar.y"
	{ free((yyvaluep->string)); };

/* Line 1000 of yacc.c  */
#line 1406 "grammar.y.c"
	break;
      case 7: /* "TOK_NUMBER" */

/* Line 1000 of yacc.c  */
#line 33 "grammar.y"
	{ free((yyvaluep->string)); };

/* Line 1000 of yacc.c  */
#line 1415 "grammar.y.c"
	break;
      case 70: /* "class" */

/* Line 1000 of yacc.c  */
#line 30 "grammar.y"
	{ unit_free((yyvaluep->unit)); (yyvaluep->unit) = 0; };

/* Line 1000 of yacc.c  */
#line 1424 "grammar.y.c"
	break;
      case 71: /* "interface" */

/* Line 1000 of yacc.c  */
#line 30 "grammar.y"
	{ unit_free((yyvaluep->unit)); (yyvaluep->unit) = 0; };

/* Line 1000 of yacc.c  */
#line 1433 "grammar.y.c"
	break;
      case 72: /* "struct" */

/* Line 1000 of yacc.c  */
#line 30 "grammar.y"
	{ unit_free((yyvaluep->unit)); (yyvaluep->unit) = 0; };

/* Line 1000 of yacc.c  */
#line 1442 "grammar.y.c"
	break;
      case 73: /* "module" */

/* Line 1000 of yacc.c  */
#line 30 "grammar.y"
	{ unit_free((yyvaluep->unit)); (yyvaluep->unit) = 0; };

/* Line 1000 of yacc.c  */
#line 1451 "grammar.y.c"
	break;
      case 76: /* "variable" */

/* Line 1000 of yacc.c  */
#line 31 "grammar.y"
	{ var_free((yyvaluep->var)); (yyvaluep->var) = 0; };

/* Line 1000 of yacc.c  */
#line 1460 "grammar.y.c"
	break;
      case 77: /* "constructor" */

/* Line 1000 of yacc.c  */
#line 32 "grammar.y"
	{ func_free((yyvaluep->func)); (yyvaluep->func) = 0; };

/* Line 1000 of yacc.c  */
#line 1469 "grammar.y.c"
	break;
      case 78: /* "destructor" */

/* Line 1000 of yacc.c  */
#line 32 "grammar.y"
	{ func_free((yyvaluep->func)); (yyvaluep->func) = 0; };

/* Line 1000 of yacc.c  */
#line 1478 "grammar.y.c"
	break;
      case 79: /* "function" */

/* Line 1000 of yacc.c  */
#line 32 "grammar.y"
	{ func_free((yyvaluep->func)); (yyvaluep->func) = 0; };

/* Line 1000 of yacc.c  */
#line 1487 "grammar.y.c"
	break;
      case 80: /* "prototype" */

/* Line 1000 of yacc.c  */
#line 32 "grammar.y"
	{ func_free((yyvaluep->func)); (yyvaluep->func) = 0; };

/* Line 1000 of yacc.c  */
#line 1496 "grammar.y.c"
	break;
      case 81: /* "argument_list" */

/* Line 1000 of yacc.c  */
#line 29 "grammar.y"
	{ type_free((yyvaluep->type)); (yyvaluep->type) = 0; };

/* Line 1000 of yacc.c  */
#line 1505 "grammar.y.c"
	break;
      case 85: /* "type" */

/* Line 1000 of yacc.c  */
#line 29 "grammar.y"
	{ type_free((yyvaluep->type)); (yyvaluep->type) = 0; };

/* Line 1000 of yacc.c  */
#line 1514 "grammar.y.c"
	break;
      case 86: /* "qualified_name" */

/* Line 1000 of yacc.c  */
#line 29 "grammar.y"
	{ type_free((yyvaluep->type)); (yyvaluep->type) = 0; };

/* Line 1000 of yacc.c  */
#line 1523 "grammar.y.c"
	break;
      case 87: /* "block" */

/* Line 1000 of yacc.c  */
#line 28 "grammar.y"
	{ stmt_free((yyvaluep->stmt)); (yyvaluep->stmt) = 0; };

/* Line 1000 of yacc.c  */
#line 1532 "grammar.y.c"
	break;
      case 88: /* "statement_list" */

/* Line 1000 of yacc.c  */
#line 28 "grammar.y"
	{ stmt_free((yyvaluep->stmt)); (yyvaluep->stmt) = 0; };

/* Line 1000 of yacc.c  */
#line 1541 "grammar.y.c"
	break;
      case 89: /* "statement" */

/* Line 1000 of yacc.c  */
#line 28 "grammar.y"
	{ stmt_free((yyvaluep->stmt)); (yyvaluep->stmt) = 0; };

/* Line 1000 of yacc.c  */
#line 1550 "grammar.y.c"
	break;
      case 90: /* "clause" */

/* Line 1000 of yacc.c  */
#line 28 "grammar.y"
	{ stmt_free((yyvaluep->stmt)); (yyvaluep->stmt) = 0; };

/* Line 1000 of yacc.c  */
#line 1559 "grammar.y.c"
	break;
      case 91: /* "conditional" */

/* Line 1000 of yacc.c  */
#line 28 "grammar.y"
	{ stmt_free((yyvaluep->stmt)); (yyvaluep->stmt) = 0; };

/* Line 1000 of yacc.c  */
#line 1568 "grammar.y.c"
	break;
      case 92: /* "expression" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1577 "grammar.y.c"
	break;
      case 93: /* "assignment" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1586 "grammar.y.c"
	break;
      case 94: /* "logical_or" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1595 "grammar.y.c"
	break;
      case 95: /* "logical_and" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1604 "grammar.y.c"
	break;
      case 96: /* "bitwise_or" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1613 "grammar.y.c"
	break;
      case 97: /* "bitwise_and" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1622 "grammar.y.c"
	break;
      case 98: /* "equality" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1631 "grammar.y.c"
	break;
      case 99: /* "relation" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1640 "grammar.y.c"
	break;
      case 100: /* "shift" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1649 "grammar.y.c"
	break;
      case 101: /* "addition" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1658 "grammar.y.c"
	break;
      case 102: /* "multiplication" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1667 "grammar.y.c"
	break;
      case 103: /* "unary" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1676 "grammar.y.c"
	break;
      case 104: /* "postfix" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1685 "grammar.y.c"
	break;
      case 105: /* "primary" */

/* Line 1000 of yacc.c  */
#line 27 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1694 "grammar.y.c"
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
#line 120 "grammar.y"
    {
        unit_name((yyvsp[(4) - (4)].unit), (yyvsp[(2) - (4)].type)); 
		parser_class(parser, (yyvsp[(4) - (4)].unit));
    ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 124 "grammar.y"
    {
        unit_name((yyvsp[(4) - (4)].unit), (yyvsp[(2) - (4)].type)); 
		parser_interface(parser, (yyvsp[(4) - (4)].unit));
    ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 128 "grammar.y"
    {
        unit_name((yyvsp[(4) - (4)].unit), (yyvsp[(2) - (4)].type)); 
		parser_struct(parser, (yyvsp[(4) - (4)].unit));
    ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 132 "grammar.y"
    {
        unit_name((yyvsp[(4) - (4)].unit), (yyvsp[(2) - (4)].type));
		parser_module(parser, (yyvsp[(4) - (4)].unit));
    ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 139 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 140 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 141 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_var((yyval.unit), (yyvsp[(1) - (2)].var)); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 142 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_ctor((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 143 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_dtor((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 144 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_func((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 145 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 146 "grammar.y"
    { (yyval.unit) = unit_alloc(UNIT_TYPE_CLASS); ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 150 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 151 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 152 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_func((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 153 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 154 "grammar.y"
    { (yyval.unit) = unit_alloc(UNIT_TYPE_INTERFACE); ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 158 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 159 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 160 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_var((yyval.unit), (yyvsp[(1) - (2)].var)); ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 161 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_ctor((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 162 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_func((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 163 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 164 "grammar.y"
    { (yyval.unit) = unit_alloc(UNIT_TYPE_STRUCT); ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 168 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 169 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 170 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_func((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 171 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 172 "grammar.y"
    { (yyval.unit) = unit_alloc(UNIT_TYPE_MODULE); ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 176 "grammar.y"
    {
        //parser_import(parser, $2);
		//$$ = 0;
    ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 183 "grammar.y"
    {
        //parser_def(parser, $2, $3);
		//$$ = 0;
    ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 190 "grammar.y"
    {
		(yyval.var) = var_alloc((yyvsp[(4) - (7)].string), (yyvsp[(1) - (7)].flag)|(yyvsp[(2) - (7)].flag), (yyvsp[(3) - (7)].type), (yyvsp[(6) - (7)].expr));
    ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 193 "grammar.y"
    {
		(yyval.var) = var_alloc((yyvsp[(4) - (5)].string), (yyvsp[(1) - (5)].flag)|(yyvsp[(2) - (5)].flag), (yyvsp[(3) - (5)].type), 0);
    ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 199 "grammar.y"
    { 
		(yyval.func) = func_alloc("@init", (yyvsp[(2) - (4)].type), 0, (yyvsp[(4) - (4)].stmt));
		(yyval.func)->flags = (yyvsp[(3) - (4)].flag);
	;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 206 "grammar.y"
    { 
		(yyval.func) = func_alloc("@destroy", 0, 0, (yyvsp[(4) - (4)].stmt)); 
	;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 212 "grammar.y"
    {
		(yyval.func) = func_alloc((yyvsp[(1) - (7)].string), (yyvsp[(2) - (7)].type), (yyvsp[(6) - (7)].type), (yyvsp[(7) - (7)].stmt));
		(yyval.func)->flags = (yyvsp[(3) - (7)].flag)|(yyvsp[(4) - (7)].flag)|(yyvsp[(5) - (7)].flag);
    ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 219 "grammar.y"
    {
		(yyval.func) = func_alloc((yyvsp[(1) - (4)].string), (yyvsp[(2) - (4)].type), (yyvsp[(3) - (4)].type), 0);
    ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 225 "grammar.y"
    { (yyval.type) = 0; ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 229 "grammar.y"
    { (yyval.flag) = UNIT_FLAG_PUBLIC; ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 230 "grammar.y"
    { (yyval.flag) = UNIT_FLAG_PRIVATE; ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 231 "grammar.y"
    { (yyval.flag) = UNIT_FLAG_PROTECTED; ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 232 "grammar.y"
    { (yyval.flag) = 0; ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 236 "grammar.y"
    { (yyval.flag) = UNIT_FLAG_STATIC; ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 237 "grammar.y"
    { (yyval.flag) = 0; ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 241 "grammar.y"
    { (yyval.flag) = UNIT_FLAG_NATIVE; ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 242 "grammar.y"
    { (yyval.flag) = 0; ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 246 "grammar.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 247 "grammar.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 251 "grammar.y"
    { (yyval.type) = type_concat((yyvsp[(1) - (3)].type), (yyvsp[(3) - (3)].type)); ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 252 "grammar.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 256 "grammar.y"
    { (yyval.stmt) = (yyvsp[(2) - (3)].stmt); ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 260 "grammar.y"
    { (yyval.stmt) = stmt_append((yyvsp[(2) - (2)].stmt), (yyvsp[(1) - (2)].stmt)); ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 261 "grammar.y"
    { (yyval.stmt) = stmt_block(); ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 265 "grammar.y"
    {
		(yyval.stmt) = stmt_for((yyvsp[(3) - (9)].stmt), (yyvsp[(5) - (9)].stmt), (yyvsp[(7) - (9)].stmt), (yyvsp[(9) - (9)].stmt));
	;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 268 "grammar.y"
    {
		(yyval.stmt) = stmt_foreach(var_alloc((yyvsp[(4) - (8)].string), 0, (yyvsp[(3) - (8)].type), (yyvsp[(6) - (8)].expr)), (yyvsp[(8) - (8)].stmt));
	;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 271 "grammar.y"
    {
		(yyval.stmt) = stmt_until((yyvsp[(3) - (5)].stmt), (yyvsp[(5) - (5)].stmt));
	;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 274 "grammar.y"
    {
		(yyval.stmt) = stmt_while((yyvsp[(3) - (5)].stmt), (yyvsp[(5) - (5)].stmt));
	;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 277 "grammar.y"
    {
		(yyval.stmt) = stmt_dountil((yyvsp[(2) - (7)].stmt), (yyvsp[(5) - (7)].stmt));
	;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 280 "grammar.y"
    {
		(yyval.stmt) = stmt_dowhile((yyvsp[(2) - (7)].stmt), (yyvsp[(5) - (7)].stmt));
	;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 283 "grammar.y"
    { 
		(yyval.stmt) = stmt_decl(var_alloc((yyvsp[(2) - (3)].string), 0, (yyvsp[(1) - (3)].type), 0)); 
	;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 286 "grammar.y"
    { (yyval.stmt) = (yyvsp[(1) - (1)].stmt); ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 287 "grammar.y"
    { (yyval.stmt) = (yyvsp[(1) - (2)].stmt); ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 288 "grammar.y"
    { (yyval.stmt) = 0; ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 292 "grammar.y"
    { 
		(yyval.stmt) = stmt_decl(var_alloc((yyvsp[(2) - (4)].string), 0, (yyvsp[(1) - (4)].type), (yyvsp[(4) - (4)].expr))); 
	;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 295 "grammar.y"
    { 
		(yyval.stmt) = stmt_expr((yyvsp[(1) - (1)].expr)); 
	;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 298 "grammar.y"
    { (yyval.stmt) = 0; ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 302 "grammar.y"
    {
		(yyval.stmt) = stmt_conditional((yyvsp[(3) - (5)].stmt), (yyvsp[(5) - (5)].stmt), 0);
	;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 305 "grammar.y"
    {
		(yyval.stmt) = stmt_conditional((yyvsp[(3) - (7)].stmt), (yyvsp[(5) - (7)].stmt), (yyvsp[(7) - (7)].stmt));
	;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 308 "grammar.y"
    { (yyval.stmt) = (yyvsp[(1) - (1)].stmt); ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 311 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 314 "grammar.y"
    { 
		(yyval.expr) = expr_binary(op_assign, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 317 "grammar.y"
    { 
		(yyval.expr) = expr_binary(op_mul_assign, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 320 "grammar.y"
    { 
		(yyval.expr) = expr_binary(op_div_assign, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 323 "grammar.y"
    { 
		(yyval.expr) = expr_binary(op_mod_assign, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 326 "grammar.y"
    { 
		(yyval.expr) = expr_binary(op_sub_assign, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 329 "grammar.y"
    { 
		(yyval.expr) = expr_binary(op_add_assign, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 332 "grammar.y"
    { 
		(yyval.expr) = expr_binary(op_bitand_assign, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 335 "grammar.y"
    { 
		(yyval.expr) = expr_binary(op_bitor_assign, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 338 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 342 "grammar.y"
    { (yyval.expr) = expr_binary(op_or, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 343 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 347 "grammar.y"
    { (yyval.expr) = expr_binary(op_and, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 348 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 352 "grammar.y"
    { (yyval.expr) = expr_binary(op_bitor, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 353 "grammar.y"
    { (yyval.expr) = expr_binary(op_bitxor, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 354 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 358 "grammar.y"
    { (yyval.expr) = expr_binary(op_bitand, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 359 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 363 "grammar.y"
    { (yyval.expr) = expr_binary(op_equal, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 364 "grammar.y"
    { 
		(yyval.expr) = expr_binary(op_notequal, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
	;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 367 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 371 "grammar.y"
    { (yyval.expr) = expr_binary(op_greater, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 372 "grammar.y"
    { (yyval.expr) = expr_binary(op_less, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 373 "grammar.y"
    { (yyval.expr) = expr_binary(op_ge, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 374 "grammar.y"
    { (yyval.expr) = expr_binary(op_le, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 375 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 379 "grammar.y"
    { (yyval.expr) = expr_binary(op_lshift, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 380 "grammar.y"
    { (yyval.expr) = expr_binary(op_rshift, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 381 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 385 "grammar.y"
    { (yyval.expr) = expr_binary(op_plus, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 386 "grammar.y"
    { (yyval.expr) = expr_binary(op_minus, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 387 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 391 "grammar.y"
    { (yyval.expr) = expr_binary(op_mul, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 392 "grammar.y"
    { (yyval.expr) = expr_binary(op_div, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 393 "grammar.y"
    { (yyval.expr) = expr_binary(op_mod, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 394 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 398 "grammar.y"
    { (yyval.expr) = expr_unary(op_inc, (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 399 "grammar.y"
    { (yyval.expr) = expr_unary(op_dec, (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 400 "grammar.y"
    { (yyval.expr) = expr_unary(op_plus, (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 401 "grammar.y"
    { (yyval.expr) = expr_unary(op_minus, (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 402 "grammar.y"
    { (yyval.expr) = expr_unary(op_bang, (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 403 "grammar.y"
    { (yyval.expr) = expr_unary(op_tilde, (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 404 "grammar.y"
    { (yyval.expr) = expr_unary(op_star, (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 405 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 409 "grammar.y"
    { (yyval.expr) = expr_call((yyvsp[(1) - (2)].expr), 0); ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 410 "grammar.y"
    { (yyval.expr) = expr_index((yyvsp[(1) - (4)].expr), (yyvsp[(3) - (4)].expr)); ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 411 "grammar.y"
    { (yyval.expr) = expr_member((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].string)); ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 412 "grammar.y"
    { (yyval.expr) = expr_unary(op_postinc, (yyvsp[(1) - (2)].expr)); ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 413 "grammar.y"
    { (yyval.expr) = expr_unary(op_postdec, (yyvsp[(1) - (2)].expr)); ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 414 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 418 "grammar.y"
    { (yyval.expr) = expr_string((yyvsp[(1) - (1)].string)); ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 419 "grammar.y"
    { (yyval.expr) = expr_string((yyvsp[(1) - (1)].string)); ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 420 "grammar.y"
    { (yyval.expr) = expr_string((yyvsp[(1) - (1)].string)); ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 421 "grammar.y"
    { (yyval.expr) = (yyvsp[(2) - (3)].expr); ;}
    break;



/* Line 1455 of yacc.c  */
#line 2939 "grammar.y.c"
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



