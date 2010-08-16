
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

#define PARSER_CHECK(x) if (!((x))) YYERROR;


/* Line 189 of yacc.c  */
#line 91 "grammar.y.c"

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
#line 18 "grammar.y"
 expr_t *expr; 

/* Line 214 of yacc.c  */
#line 19 "grammar.y"
 stmt_t *stmt; 

/* Line 214 of yacc.c  */
#line 20 "grammar.y"
 type_t *type; 

/* Line 214 of yacc.c  */
#line 21 "grammar.y"
 unit_t *unit; 

/* Line 214 of yacc.c  */
#line 22 "grammar.y"
 var_t *var; 

/* Line 214 of yacc.c  */
#line 23 "grammar.y"
 func_t *func; 

/* Line 214 of yacc.c  */
#line 24 "grammar.y"
 import_t *import; 

/* Line 214 of yacc.c  */
#line 25 "grammar.y"
 def_t *def; 

/* Line 214 of yacc.c  */
#line 26 "grammar.y"
 char *string; 

/* Line 214 of yacc.c  */
#line 27 "grammar.y"
 int null; 

/* Line 214 of yacc.c  */
#line 28 "grammar.y"
 int flag; 


/* Line 214 of yacc.c  */
#line 218 "grammar.y.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 230 "grammar.y.c"

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
#define YYLAST   477

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  72
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  41
/* YYNRULES -- Number of rules.  */
#define YYNRULES  149
/* YYNRULES -- Number of states.  */
#define YYNSTATES  313

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
       2,     2,     2,    67,     2,     2,     2,    66,    60,     2,
      50,    51,    53,    63,    52,    64,    71,    65,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    57,    48,
      62,    49,    61,    54,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    69,     2,    70,    59,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    55,    58,    56,    68,     2,     2,     2,
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
     188,   190,   192,   193,   195,   196,   198,   201,   204,   206,
     209,   212,   216,   218,   222,   224,   228,   231,   235,   236,
     246,   255,   261,   267,   275,   283,   287,   289,   292,   296,
     299,   304,   306,   307,   313,   321,   323,   325,   329,   331,
     335,   339,   343,   347,   351,   355,   359,   363,   365,   369,
     371,   375,   377,   381,   385,   387,   391,   393,   397,   401,
     403,   407,   411,   415,   419,   421,   425,   429,   431,   435,
     439,   441,   445,   449,   453,   455,   458,   461,   464,   467,
     470,   473,   476,   478,   483,   487,   492,   496,   499,   502,
     504,   506,   510,   514,   519,   523,   527,   529,   531,   535
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      73,     0,    -1,     9,    91,    48,    74,    -1,    10,    91,
      48,    75,    -1,    11,    91,    48,    76,    -1,    12,    91,
      48,    77,    -1,    78,    74,    -1,    79,    74,    -1,    80,
      74,    -1,    81,    74,    -1,    82,    74,    -1,    83,    74,
      -1,    84,    74,    -1,     1,    74,    -1,    -1,    78,    75,
      -1,    79,    75,    -1,    85,    75,    -1,     1,    75,    -1,
      -1,    78,    76,    -1,    79,    76,    -1,    80,    76,    -1,
      81,    76,    -1,    83,    76,    -1,     1,    76,    -1,    -1,
      78,    77,    -1,    79,    77,    -1,    83,    77,    -1,     1,
      77,    -1,    -1,    13,    91,    48,    -1,    14,    90,     3,
      48,    -1,    88,    89,    90,     5,    49,    97,    48,    -1,
      88,    89,    90,     5,    48,    -1,    88,    89,    90,     6,
      49,    97,    48,    -1,    15,    86,    88,    92,    -1,    16,
      50,    51,    92,    -1,     5,    86,    88,    89,    90,    92,
      -1,     5,    86,    88,    89,    92,    -1,     5,    86,    88,
      89,    21,    90,    48,    -1,     5,    86,    88,    89,    21,
      48,    -1,     5,    86,    90,    48,    -1,     5,    86,    48,
      -1,    50,    87,    51,    -1,    50,    51,    -1,    90,     5,
      52,    87,    -1,    90,     5,    -1,    17,    -1,    18,    -1,
      19,    -1,    -1,    20,    -1,    -1,     4,    -1,     4,    53,
      -1,     4,    54,    -1,    91,    -1,    91,    53,    -1,    91,
      54,    -1,     3,    32,    91,    -1,     3,    -1,     6,    32,
      91,    -1,     6,    -1,    55,    93,    56,    -1,    93,    94,
      -1,    93,     1,    48,    -1,    -1,    28,    50,    95,    48,
      95,    48,    95,    51,    92,    -1,    26,    50,    90,     5,
      57,    97,    51,    92,    -1,    24,    50,    95,    51,    92,
      -1,    22,    50,    95,    51,    92,    -1,    27,    92,    24,
      50,    95,    51,    48,    -1,    27,    92,    22,    50,    95,
      51,    48,    -1,    90,     5,    48,    -1,    96,    -1,    95,
      48,    -1,    29,    97,    48,    -1,    29,    48,    -1,    90,
       5,    49,    97,    -1,    97,    -1,    -1,    25,    50,    95,
      51,    92,    -1,    25,    50,    95,    51,    92,    23,    96,
      -1,    92,    -1,    98,    -1,    98,    54,    99,    -1,    99,
      -1,   109,    49,    99,    -1,   109,    39,    99,    -1,   109,
      40,    99,    -1,   109,    43,    99,    -1,   109,    41,    99,
      -1,   109,    42,    99,    -1,   109,    45,    99,    -1,   109,
      44,    99,    -1,   100,    -1,   100,    33,   101,    -1,   101,
      -1,   101,    34,   102,    -1,   102,    -1,   102,    58,   103,
      -1,   102,    59,   103,    -1,   103,    -1,   103,    60,   104,
      -1,   104,    -1,   104,    30,   105,    -1,   104,    31,   105,
      -1,   105,    -1,   105,    61,   106,    -1,   105,    62,   106,
      -1,   105,    35,   106,    -1,   105,    36,   106,    -1,   106,
      -1,   106,    37,   107,    -1,   106,    38,   107,    -1,   107,
      -1,   107,    63,   108,    -1,   107,    64,   108,    -1,   108,
      -1,   108,    53,   109,    -1,   108,    65,   109,    -1,   108,
      66,   109,    -1,   109,    -1,    46,   109,    -1,    47,   109,
      -1,    63,   109,    -1,    64,   109,    -1,    67,   109,    -1,
      68,   109,    -1,    53,   109,    -1,   110,    -1,   110,    50,
     112,    51,    -1,   110,    50,    51,    -1,   110,    69,    97,
      70,    -1,   110,    71,     5,    -1,   110,    46,    -1,   110,
      47,    -1,   111,    -1,     5,    -1,    90,    71,     6,    -1,
      90,    71,     5,    -1,    90,    50,   112,    51,    -1,    90,
      50,    51,    -1,    50,    97,    51,    -1,     7,    -1,     8,
      -1,    97,    52,   112,    -1,    97,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   135,   135,   139,   143,   147,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   166,   167,   168,   169,   170,
     174,   175,   176,   177,   178,   179,   180,   184,   185,   186,
     187,   188,   192,   196,   200,   205,   209,   216,   223,   229,
     234,   242,   247,   255,   259,   266,   267,   271,   276,   283,
     284,   285,   286,   290,   291,   295,   296,   297,   298,   301,
     305,   312,   313,   314,   318,   325,   332,   333,   334,   341,
     344,   348,   351,   354,   357,   360,   364,   365,   366,   367,
     371,   375,   378,   382,   385,   388,   391,   394,   397,   401,
     404,   407,   410,   413,   416,   419,   422,   425,   429,   432,
     436,   439,   443,   446,   449,   453,   456,   460,   463,   466,
     470,   473,   476,   479,   482,   486,   489,   492,   496,   499,
     502,   506,   509,   512,   515,   519,   520,   521,   522,   523,
     524,   525,   526,   530,   533,   536,   539,   543,   546,   549,
     553,   557,   561,   565,   568,   571,   572,   573,   577,   581
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
  "','", "'*'", "'?'", "'{'", "'}'", "':'", "'|'", "'^'", "'&'", "'>'",
  "'<'", "'+'", "'-'", "'/'", "'%'", "'!'", "'~'", "'['", "']'", "'.'",
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
     295,   296,   297,   298,   299,   300,   301,   302,    59,    61,
      40,    41,    44,    42,    63,   123,   125,    58,   124,    94,
      38,    62,    60,    43,    45,    47,    37,    33,   126,    91,
      93,    46
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    72,    73,    73,    73,    73,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    75,    75,    75,    75,    75,
      76,    76,    76,    76,    76,    76,    76,    77,    77,    77,
      77,    77,    78,    79,    80,    80,    80,    81,    82,    83,
      83,    84,    84,    85,    85,    86,    86,    87,    87,    88,
      88,    88,    88,    89,    89,    90,    90,    90,    90,    90,
      90,    91,    91,    91,    91,    92,    93,    93,    93,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      95,    95,    95,    96,    96,    96,    97,    98,    98,    99,
      99,    99,    99,    99,    99,    99,    99,    99,   100,   100,
     101,   101,   102,   102,   102,   103,   103,   104,   104,   104,
     105,   105,   105,   105,   105,   106,   106,   106,   107,   107,
     107,   108,   108,   108,   108,   109,   109,   109,   109,   109,
     109,   109,   109,   110,   110,   110,   110,   110,   110,   110,
     111,   111,   111,   111,   111,   111,   111,   111,   112,   112
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
       2,     2,     1,     4,     3,     4,     3,     2,     2,     1,
       1,     3,     3,     4,     3,     3,     1,     1,     3,     1
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
      39,     0,   140,   146,   147,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    65,     0,     0,     0,
       0,     0,    85,    66,     0,    76,    81,    86,    88,    97,
      99,   101,   104,   106,   109,   114,   117,   120,   124,   132,
     139,     0,     0,     0,    41,    67,    82,    82,    82,     0,
       0,    82,    79,     0,   125,   126,     0,   131,   127,   128,
     129,   130,     0,     0,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   137,   138,     0,     0,     0,    34,    36,
       0,     0,     0,     0,     0,     0,     0,     0,    78,   145,
      75,     0,   144,   149,     0,   142,   141,    87,    98,   124,
     100,   102,   103,   105,   107,   108,   112,   113,   110,   111,
     115,   116,   118,   119,   121,   122,   123,    90,    91,    93,
      94,    92,    96,    95,    89,   134,     0,     0,   136,     0,
       0,     0,     0,     0,    82,    82,    82,    80,     0,   143,
     133,   135,    72,    71,    83,     0,     0,     0,     0,   148,
       0,     0,     0,     0,    82,    84,     0,    74,    73,     0,
      70,     0,    69
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,    30,    41,    47,    54,    31,    32,    33,    34,
      35,    36,    37,    44,    60,    93,    38,    75,   171,    64,
     122,   123,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     244
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -213
static const yytype_int16 yypact[] =
{
     100,    44,    44,    44,    44,    23,    -5,     2,    12,    18,
      24,    39,  -213,    44,    44,   418,   133,   439,   285,  -213,
    -213,   418,     4,    44,   116,     4,    52,  -213,  -213,  -213,
    -213,   418,   418,   418,   418,   418,   418,   418,    84,   133,
       4,  -213,   133,   133,   133,   439,     4,  -213,   439,   439,
     439,   439,   439,   285,  -213,   285,   285,   285,  -213,    38,
     159,    70,    78,   123,    83,   159,   105,  -213,  -213,  -213,
    -213,  -213,  -213,  -213,  -213,   116,  -213,    59,  -213,  -213,
    -213,  -213,   159,  -213,  -213,  -213,  -213,  -213,  -213,  -213,
    -213,  -213,  -213,   109,   124,    84,  -213,  -213,  -213,    95,
    -213,  -213,   107,   107,   178,  -213,   148,    84,  -213,   146,
      22,  -213,  -213,  -213,  -213,   158,   190,  -213,    33,   116,
      94,   107,  -213,   241,  -213,   268,   268,  -213,  -213,   193,
    -213,   195,  -213,  -213,  -213,   202,   209,   234,   245,   107,
     260,    11,   268,   268,   268,   268,  -213,   268,   268,   268,
     268,    -2,  -213,  -213,   216,  -213,  -213,   265,  -213,   289,
     286,   150,   263,   201,    -6,   174,   172,    48,   428,    21,
    -213,   -39,   276,   277,  -213,  -213,   268,   268,   268,   116,
      69,   268,  -213,   278,  -213,  -213,   279,  -213,  -213,  -213,
    -213,  -213,   229,    77,   287,  -213,   268,   268,   268,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   268,  -213,  -213,   187,   268,   322,  -213,  -213,
      -1,   282,   290,   291,   344,   307,   308,   317,  -213,  -213,
    -213,   268,  -213,   318,   320,  -213,  -213,  -213,   286,  -213,
     150,   263,   263,   201,    -6,    -6,   174,   174,   174,   174,
     172,   172,    48,    48,  -213,  -213,  -213,  -213,  -213,  -213,
    -213,  -213,  -213,  -213,  -213,  -213,   321,   299,  -213,   324,
     107,   107,   107,   326,   268,   268,   268,  -213,   268,  -213,
    -213,  -213,  -213,  -213,   351,   268,   327,   333,   343,  -213,
     -15,   357,   347,   361,   268,  -213,   107,  -213,  -213,   359,
    -213,   107,  -213
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -213,  -213,   429,   214,   137,   226,   295,   337,   122,   152,
    -213,   311,  -213,  -213,     6,   256,    43,   -62,   -24,   403,
    -101,  -213,  -213,  -169,   111,  -120,  -213,   181,  -213,   215,
     217,   101,   212,   104,   -56,   108,    92,    16,  -213,  -213,
    -212
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -83
static const yytype_int16 yytable[] =
{
      63,   113,   114,   192,   279,   172,   173,   231,   232,   233,
     137,   193,   237,   276,     6,    62,   132,     7,   133,   134,
     130,   183,   152,    12,   186,     6,    62,    13,     7,   204,
     205,    65,   194,   110,    14,    94,     6,    62,   180,     7,
     112,     6,    62,   120,     7,   118,    77,     6,   193,   193,
       7,   104,    82,   106,    59,   206,   207,   142,   143,   182,
      15,   144,     6,    62,   145,     7,    16,   223,   224,   194,
     194,   225,    17,   243,   147,   148,   299,   112,   149,   150,
       6,    62,   132,     7,   133,   134,   121,    18,   112,    92,
     226,   235,   227,   236,   121,    94,   129,     6,    62,   151,
       7,   212,    66,    95,    74,   243,   277,   105,   102,     1,
       2,     3,     4,   213,   214,   296,   297,   298,    96,     6,
      62,   287,     7,   142,   143,   107,    99,   144,   242,   109,
     145,    97,    98,   -19,    39,   309,   100,   101,    40,    50,
     147,   148,   128,   111,   149,   150,    23,    24,   256,   257,
     258,   259,   230,   230,   230,   234,   103,   230,   184,   185,
     108,   187,   112,   188,   189,   190,   191,    50,   243,    51,
      50,    50,    50,    50,    50,   301,    27,    28,    29,   292,
     293,   294,    81,   115,   116,    83,    84,    85,    86,    87,
       6,    62,   132,     7,   133,   134,   117,    51,   119,   152,
      51,    51,    51,    51,    51,   310,   124,   125,   199,   200,
     312,   208,   209,   249,   249,   249,   249,   249,   249,   249,
     249,   249,   249,   249,   249,   249,   249,   249,   264,   265,
     266,   202,   203,   142,   143,   210,   211,   144,   275,   126,
     145,   174,   131,   175,     6,    62,   132,     7,   133,   134,
     147,   148,   176,    76,   149,   150,    78,    79,    80,   177,
     230,   230,   230,   135,   195,   136,   137,   138,   139,   140,
     141,     6,    62,   132,     7,   133,   134,   240,   241,    88,
     230,    89,    90,    91,   178,   -31,    53,   142,   143,   -82,
      46,   144,   245,   246,   145,   179,   112,   146,    23,    24,
     251,   252,   262,   263,   147,   148,   254,   255,   149,   150,
     181,    42,    48,    55,   142,   143,   260,   261,   144,   196,
     198,   145,   197,   201,   228,   229,   238,   278,    52,    57,
     239,   147,   148,   280,    42,   149,   150,    42,    42,    42,
      48,   281,   282,    48,    48,    48,    48,    48,    55,   283,
      55,    55,    55,    43,    49,    56,    52,   284,   285,    52,
      52,    52,    52,    52,    57,   286,    57,    57,    57,   291,
     288,   289,   290,   241,   300,   127,    43,   247,   302,    43,
      43,    43,    49,   295,   303,    49,    49,    49,    49,    49,
      56,   304,    56,    56,    56,   307,   267,   268,   269,   270,
     271,   272,   273,   274,     8,     9,    10,    11,   306,   308,
     311,   305,   248,   253,     0,   250,    19,    20,   -14,    21,
       0,   -52,   -52,    22,   -52,     0,    61,     0,     0,     0,
       0,    23,    24,    25,    26,    27,    28,    29,   -52,   -26,
      45,     0,   -52,   -52,    46,   -52,     0,     0,     0,     0,
      58,     0,    23,    24,    25,     0,    27,    28,    29,   -52,
      67,    68,    69,    70,    71,    72,    73,   215,   216,   217,
     218,   219,   220,   221,     0,     0,     0,   222
};

static const yytype_int16 yycheck[] =
{
      24,   102,   103,     5,     5,   125,   126,   176,   177,   178,
      25,    50,   181,   225,     3,     4,     5,     6,     7,     8,
     121,   141,   123,     0,   144,     3,     4,    32,     6,    35,
      36,    25,    71,    95,    32,    59,     3,     4,   139,     6,
      55,     3,     4,    21,     6,   107,    40,     3,    50,    50,
       6,    75,    46,    77,    50,    61,    62,    46,    47,    48,
      48,    50,     3,     4,    53,     6,    48,    46,    47,    71,
      71,    50,    48,   193,    63,    64,   288,    55,    67,    68,
       3,     4,     5,     6,     7,     8,   110,    48,    55,    51,
      69,    22,    71,    24,   118,   119,   120,     3,     4,   123,
       6,    53,    50,    60,    20,   225,   226,    48,    65,     9,
      10,    11,    12,    65,    66,   284,   285,   286,    48,     3,
       4,   241,     6,    46,    47,    82,     3,    50,    51,     5,
      53,    53,    54,     0,     1,   304,    53,    54,     5,    17,
      63,    64,    48,    48,    67,    68,    13,    14,   204,   205,
     206,   207,   176,   177,   178,   179,    51,   181,   142,   143,
      51,   145,    55,   147,   148,   149,   150,    45,   288,    17,
      48,    49,    50,    51,    52,   295,    17,    18,    19,   280,
     281,   282,    45,     5,     6,    48,    49,    50,    51,    52,
       3,     4,     5,     6,     7,     8,    48,    45,    52,   300,
      48,    49,    50,    51,    52,   306,    48,    49,    58,    59,
     311,    37,    38,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,    30,    31,    46,    47,    63,    64,    50,    51,    49,
      53,    48,     1,    48,     3,     4,     5,     6,     7,     8,
      63,    64,    50,    39,    67,    68,    42,    43,    44,    50,
     284,   285,   286,    22,    48,    24,    25,    26,    27,    28,
      29,     3,     4,     5,     6,     7,     8,    48,    49,    53,
     304,    55,    56,    57,    50,     0,     1,    46,    47,    48,
       5,    50,     5,     6,    53,    50,    55,    56,    13,    14,
     199,   200,   210,   211,    63,    64,   202,   203,    67,    68,
      50,    16,    17,    18,    46,    47,   208,   209,    50,    54,
      34,    53,    33,    60,    48,    48,    48,     5,    17,    18,
      51,    63,    64,    51,    39,    67,    68,    42,    43,    44,
      45,    51,    51,    48,    49,    50,    51,    52,    53,     5,
      55,    56,    57,    16,    17,    18,    45,    50,    50,    48,
      49,    50,    51,    52,    53,    48,    55,    56,    57,    70,
      52,    51,    51,    49,    23,   119,    39,   196,    51,    42,
      43,    44,    45,    57,    51,    48,    49,    50,    51,    52,
      53,    48,    55,    56,    57,    48,   215,   216,   217,   218,
     219,   220,   221,   222,     1,     2,     3,     4,    51,    48,
      51,   300,   197,   201,    -1,   198,    13,    14,     0,     1,
      -1,     3,     4,     5,     6,    -1,    23,    -1,    -1,    -1,
      -1,    13,    14,    15,    16,    17,    18,    19,    20,     0,
       1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    -1,
      21,    -1,    13,    14,    15,    -1,    17,    18,    19,    20,
      31,    32,    33,    34,    35,    36,    37,    39,    40,    41,
      42,    43,    44,    45,    -1,    -1,    -1,    49
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     9,    10,    11,    12,    73,     3,     6,    91,    91,
      91,    91,     0,    32,    32,    48,    48,    48,    48,    91,
      91,     1,     5,    13,    14,    15,    16,    17,    18,    19,
      74,    78,    79,    80,    81,    82,    83,    84,    88,     1,
       5,    75,    78,    79,    85,     1,     5,    76,    78,    79,
      80,    81,    83,     1,    77,    78,    79,    83,    74,    50,
      86,    91,     4,    90,    91,    86,    50,    74,    74,    74,
      74,    74,    74,    74,    20,    89,    75,    86,    75,    75,
      75,    76,    86,    76,    76,    76,    76,    76,    77,    77,
      77,    77,    51,    87,    90,    88,    48,    53,    54,     3,
      53,    54,    88,    51,    90,    48,    90,    88,    51,     5,
      89,    48,    55,    92,    92,     5,     6,    48,    89,    52,
      21,    90,    92,    93,    48,    49,    49,    87,    48,    90,
      92,     1,     5,     7,     8,    22,    24,    25,    26,    27,
      28,    29,    46,    47,    50,    53,    56,    63,    64,    67,
      68,    90,    92,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,    90,    97,    97,    48,    48,    50,    50,    50,    50,
      92,    50,    48,    97,   109,   109,    97,   109,   109,   109,
     109,   109,     5,    50,    71,    48,    54,    33,    34,    58,
      59,    60,    30,    31,    35,    36,    61,    62,    37,    38,
      63,    64,    53,    65,    66,    39,    40,    41,    42,    43,
      44,    45,    49,    46,    47,    50,    69,    71,    48,    48,
      90,    95,    95,    95,    90,    22,    24,    95,    48,    51,
      48,    49,    51,    97,   112,     5,     6,    99,   101,   109,
     102,   103,   103,   104,   105,   105,   106,   106,   106,   106,
     107,   107,   108,   108,   109,   109,   109,    99,    99,    99,
      99,    99,    99,    99,    99,    51,   112,    97,     5,     5,
      51,    51,    51,     5,    50,    50,    48,    97,    52,    51,
      51,    70,    92,    92,    92,    57,    95,    95,    95,   112,
      23,    97,    51,    51,    48,    96,    51,    48,    48,    95,
      92,    51,    92
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
#line 37 "grammar.y"
	{ type_free((yyvaluep->type)); (yyvaluep->type) = 0; };

/* Line 1000 of yacc.c  */
#line 1457 "grammar.y.c"
	break;
      case 4: /* "TOK_PRIMITIVE" */

/* Line 1000 of yacc.c  */
#line 37 "grammar.y"
	{ type_free((yyvaluep->type)); (yyvaluep->type) = 0; };

/* Line 1000 of yacc.c  */
#line 1466 "grammar.y.c"
	break;
      case 5: /* "TOK_IDENT" */

/* Line 1000 of yacc.c  */
#line 41 "grammar.y"
	{ free((yyvaluep->string)); };

/* Line 1000 of yacc.c  */
#line 1475 "grammar.y.c"
	break;
      case 6: /* "TOK_CONST" */

/* Line 1000 of yacc.c  */
#line 41 "grammar.y"
	{ free((yyvaluep->string)); };

/* Line 1000 of yacc.c  */
#line 1484 "grammar.y.c"
	break;
      case 7: /* "TOK_STRING" */

/* Line 1000 of yacc.c  */
#line 35 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1493 "grammar.y.c"
	break;
      case 8: /* "TOK_NUMBER" */

/* Line 1000 of yacc.c  */
#line 35 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1502 "grammar.y.c"
	break;
      case 74: /* "class" */

/* Line 1000 of yacc.c  */
#line 38 "grammar.y"
	{ unit_free((yyvaluep->unit)); (yyvaluep->unit) = 0; };

/* Line 1000 of yacc.c  */
#line 1511 "grammar.y.c"
	break;
      case 75: /* "interface" */

/* Line 1000 of yacc.c  */
#line 38 "grammar.y"
	{ unit_free((yyvaluep->unit)); (yyvaluep->unit) = 0; };

/* Line 1000 of yacc.c  */
#line 1520 "grammar.y.c"
	break;
      case 76: /* "struct" */

/* Line 1000 of yacc.c  */
#line 38 "grammar.y"
	{ unit_free((yyvaluep->unit)); (yyvaluep->unit) = 0; };

/* Line 1000 of yacc.c  */
#line 1529 "grammar.y.c"
	break;
      case 77: /* "module" */

/* Line 1000 of yacc.c  */
#line 38 "grammar.y"
	{ unit_free((yyvaluep->unit)); (yyvaluep->unit) = 0; };

/* Line 1000 of yacc.c  */
#line 1538 "grammar.y.c"
	break;
      case 80: /* "variable" */

/* Line 1000 of yacc.c  */
#line 39 "grammar.y"
	{ var_free((yyvaluep->var)); (yyvaluep->var) = 0; };

/* Line 1000 of yacc.c  */
#line 1547 "grammar.y.c"
	break;
      case 81: /* "constructor" */

/* Line 1000 of yacc.c  */
#line 40 "grammar.y"
	{ func_free((yyvaluep->func)); (yyvaluep->func) = 0; };

/* Line 1000 of yacc.c  */
#line 1556 "grammar.y.c"
	break;
      case 82: /* "destructor" */

/* Line 1000 of yacc.c  */
#line 40 "grammar.y"
	{ func_free((yyvaluep->func)); (yyvaluep->func) = 0; };

/* Line 1000 of yacc.c  */
#line 1565 "grammar.y.c"
	break;
      case 83: /* "function" */

/* Line 1000 of yacc.c  */
#line 40 "grammar.y"
	{ func_free((yyvaluep->func)); (yyvaluep->func) = 0; };

/* Line 1000 of yacc.c  */
#line 1574 "grammar.y.c"
	break;
      case 84: /* "native" */

/* Line 1000 of yacc.c  */
#line 40 "grammar.y"
	{ func_free((yyvaluep->func)); (yyvaluep->func) = 0; };

/* Line 1000 of yacc.c  */
#line 1583 "grammar.y.c"
	break;
      case 85: /* "prototype" */

/* Line 1000 of yacc.c  */
#line 40 "grammar.y"
	{ func_free((yyvaluep->func)); (yyvaluep->func) = 0; };

/* Line 1000 of yacc.c  */
#line 1592 "grammar.y.c"
	break;
      case 86: /* "parameter_signature" */

/* Line 1000 of yacc.c  */
#line 39 "grammar.y"
	{ var_free((yyvaluep->var)); (yyvaluep->var) = 0; };

/* Line 1000 of yacc.c  */
#line 1601 "grammar.y.c"
	break;
      case 87: /* "parameter_list" */

/* Line 1000 of yacc.c  */
#line 39 "grammar.y"
	{ var_free((yyvaluep->var)); (yyvaluep->var) = 0; };

/* Line 1000 of yacc.c  */
#line 1610 "grammar.y.c"
	break;
      case 90: /* "type" */

/* Line 1000 of yacc.c  */
#line 37 "grammar.y"
	{ type_free((yyvaluep->type)); (yyvaluep->type) = 0; };

/* Line 1000 of yacc.c  */
#line 1619 "grammar.y.c"
	break;
      case 91: /* "qualified_name" */

/* Line 1000 of yacc.c  */
#line 37 "grammar.y"
	{ type_free((yyvaluep->type)); (yyvaluep->type) = 0; };

/* Line 1000 of yacc.c  */
#line 1628 "grammar.y.c"
	break;
      case 92: /* "block" */

/* Line 1000 of yacc.c  */
#line 36 "grammar.y"
	{ stmt_free((yyvaluep->stmt)); (yyvaluep->stmt) = 0; };

/* Line 1000 of yacc.c  */
#line 1637 "grammar.y.c"
	break;
      case 93: /* "statement_list" */

/* Line 1000 of yacc.c  */
#line 36 "grammar.y"
	{ stmt_free((yyvaluep->stmt)); (yyvaluep->stmt) = 0; };

/* Line 1000 of yacc.c  */
#line 1646 "grammar.y.c"
	break;
      case 94: /* "statement" */

/* Line 1000 of yacc.c  */
#line 36 "grammar.y"
	{ stmt_free((yyvaluep->stmt)); (yyvaluep->stmt) = 0; };

/* Line 1000 of yacc.c  */
#line 1655 "grammar.y.c"
	break;
      case 95: /* "clause" */

/* Line 1000 of yacc.c  */
#line 36 "grammar.y"
	{ stmt_free((yyvaluep->stmt)); (yyvaluep->stmt) = 0; };

/* Line 1000 of yacc.c  */
#line 1664 "grammar.y.c"
	break;
      case 96: /* "conditional" */

/* Line 1000 of yacc.c  */
#line 36 "grammar.y"
	{ stmt_free((yyvaluep->stmt)); (yyvaluep->stmt) = 0; };

/* Line 1000 of yacc.c  */
#line 1673 "grammar.y.c"
	break;
      case 97: /* "expression" */

/* Line 1000 of yacc.c  */
#line 35 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1682 "grammar.y.c"
	break;
      case 98: /* "nullable" */

/* Line 1000 of yacc.c  */
#line 35 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1691 "grammar.y.c"
	break;
      case 99: /* "assignment" */

/* Line 1000 of yacc.c  */
#line 35 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1700 "grammar.y.c"
	break;
      case 100: /* "logical_or" */

/* Line 1000 of yacc.c  */
#line 35 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1709 "grammar.y.c"
	break;
      case 101: /* "logical_and" */

/* Line 1000 of yacc.c  */
#line 35 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1718 "grammar.y.c"
	break;
      case 102: /* "bitwise_or" */

/* Line 1000 of yacc.c  */
#line 35 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1727 "grammar.y.c"
	break;
      case 103: /* "bitwise_and" */

/* Line 1000 of yacc.c  */
#line 35 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1736 "grammar.y.c"
	break;
      case 104: /* "equality" */

/* Line 1000 of yacc.c  */
#line 35 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1745 "grammar.y.c"
	break;
      case 105: /* "relation" */

/* Line 1000 of yacc.c  */
#line 35 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1754 "grammar.y.c"
	break;
      case 106: /* "shift" */

/* Line 1000 of yacc.c  */
#line 35 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1763 "grammar.y.c"
	break;
      case 107: /* "addition" */

/* Line 1000 of yacc.c  */
#line 35 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1772 "grammar.y.c"
	break;
      case 108: /* "multiplication" */

/* Line 1000 of yacc.c  */
#line 35 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1781 "grammar.y.c"
	break;
      case 109: /* "unary" */

/* Line 1000 of yacc.c  */
#line 35 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1790 "grammar.y.c"
	break;
      case 110: /* "postfix" */

/* Line 1000 of yacc.c  */
#line 35 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1799 "grammar.y.c"
	break;
      case 111: /* "primary" */

/* Line 1000 of yacc.c  */
#line 35 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1808 "grammar.y.c"
	break;
      case 112: /* "argument_list" */

/* Line 1000 of yacc.c  */
#line 35 "grammar.y"
	{ expr_free((yyvaluep->expr)); (yyvaluep->expr) = 0; };

/* Line 1000 of yacc.c  */
#line 1817 "grammar.y.c"
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
#line 135 "grammar.y"
    {
        unit_name((yyvsp[(4) - (4)].unit), (yyvsp[(2) - (4)].type)); 
		parser_class(parser, (yyvsp[(4) - (4)].unit));
    ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 139 "grammar.y"
    {
        unit_name((yyvsp[(4) - (4)].unit), (yyvsp[(2) - (4)].type)); 
		parser_interface(parser, (yyvsp[(4) - (4)].unit));
    ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 143 "grammar.y"
    {
        unit_name((yyvsp[(4) - (4)].unit), (yyvsp[(2) - (4)].type)); 
		parser_struct(parser, (yyvsp[(4) - (4)].unit));
    ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 147 "grammar.y"
    {
        unit_name((yyvsp[(4) - (4)].unit), (yyvsp[(2) - (4)].type));
		parser_module(parser, (yyvsp[(4) - (4)].unit));
    ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 154 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_import((yyval.unit), (yyvsp[(1) - (2)].import)); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 155 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_def((yyval.unit), (yyvsp[(1) - (2)].def)); ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 156 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_var((yyval.unit), (yyvsp[(1) - (2)].var)); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 157 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_ctor((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 158 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_dtor((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 159 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_func((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 160 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_func((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 161 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 162 "grammar.y"
    { (yyval.unit) = unit_alloc(UNIT_TYPE_CLASS); ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 166 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_import((yyval.unit), (yyvsp[(1) - (2)].import)); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 167 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_def((yyval.unit), (yyvsp[(1) - (2)].def)); ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 168 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_func((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 169 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 170 "grammar.y"
    { (yyval.unit) = unit_alloc(UNIT_TYPE_INTERFACE); ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 174 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_import((yyval.unit), (yyvsp[(1) - (2)].import)); ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 175 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_def((yyval.unit), (yyvsp[(1) - (2)].def)); ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 176 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_var((yyval.unit), (yyvsp[(1) - (2)].var)); ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 177 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_ctor((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 178 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_func((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 179 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 180 "grammar.y"
    { (yyval.unit) = unit_alloc(UNIT_TYPE_STRUCT); ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 184 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_import((yyval.unit), (yyvsp[(1) - (2)].import)); ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 185 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_def((yyval.unit), (yyvsp[(1) - (2)].def)); ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 186 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); unit_func((yyval.unit), (yyvsp[(1) - (2)].func)); ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 187 "grammar.y"
    { (yyval.unit) = (yyvsp[(2) - (2)].unit); ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 188 "grammar.y"
    { (yyval.unit) = unit_alloc(UNIT_TYPE_MODULE); ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 192 "grammar.y"
    { (yyval.import) = import_alloc((yyvsp[(2) - (3)].type)); ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 196 "grammar.y"
    { (yyval.def) = def_alloc((yyvsp[(2) - (4)].type), (yyvsp[(3) - (4)].type)); ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 200 "grammar.y"
    {
		// TODO: Set symbol table for class-level
		(yyval.var) = var_alloc((yyvsp[(4) - (7)].string), (yyvsp[(1) - (7)].flag)|(yyvsp[(2) - (7)].flag), (yyvsp[(3) - (7)].type), (yyvsp[(6) - (7)].expr));
		free((yyvsp[(4) - (7)].string));
    ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 205 "grammar.y"
    {
		(yyval.var) = var_alloc((yyvsp[(4) - (5)].string), (yyvsp[(1) - (5)].flag)|(yyvsp[(2) - (5)].flag), (yyvsp[(3) - (5)].type), 0);
		free((yyvsp[(4) - (5)].string));
    ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 209 "grammar.y"
    {
		(yyval.var) = var_alloc((yyvsp[(4) - (7)].string), (yyvsp[(1) - (7)].flag)|(yyvsp[(2) - (7)].flag)|UNIT_FLAG_CONST, (yyvsp[(3) - (7)].type), (yyvsp[(6) - (7)].expr));
		free((yyvsp[(4) - (7)].string));
	;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 216 "grammar.y"
    { 
		(yyval.func) = func_alloc("@init", (yyvsp[(2) - (4)].var), 0, (yyvsp[(4) - (4)].stmt));
		(yyval.func)->flags = (yyvsp[(3) - (4)].flag);
	;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 223 "grammar.y"
    { 
		(yyval.func) = func_alloc("@destroy", 0, 0, (yyvsp[(4) - (4)].stmt)); 
	;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 229 "grammar.y"
    {
		(yyval.func) = func_alloc((yyvsp[(1) - (6)].string), (yyvsp[(2) - (6)].var), (yyvsp[(5) - (6)].type), (yyvsp[(6) - (6)].stmt));
		(yyval.func)->flags = (yyvsp[(3) - (6)].flag)|(yyvsp[(4) - (6)].flag);
		free((yyvsp[(1) - (6)].string));
    ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 234 "grammar.y"
    {
		(yyval.func) = func_alloc((yyvsp[(1) - (5)].string), (yyvsp[(2) - (5)].var), 0, (yyvsp[(5) - (5)].stmt));
		(yyval.func)->flags = (yyvsp[(3) - (5)].flag)|(yyvsp[(4) - (5)].flag);
		free((yyvsp[(1) - (5)].string));
	;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 242 "grammar.y"
    {
		(yyval.func) = func_alloc((yyvsp[(1) - (7)].string), (yyvsp[(2) - (7)].var), (yyvsp[(6) - (7)].type), 0); 
		(yyval.func)->flags = (yyvsp[(3) - (7)].flag)|(yyvsp[(4) - (7)].flag)|UNIT_FLAG_NATIVE;
		free((yyvsp[(1) - (7)].string));
	;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 247 "grammar.y"
    {
		(yyval.func) = func_alloc((yyvsp[(1) - (6)].string), (yyvsp[(2) - (6)].var), 0, 0);
		(yyval.func)->flags = (yyvsp[(3) - (6)].flag)|(yyvsp[(4) - (6)].flag)|UNIT_FLAG_NATIVE;
		free((yyvsp[(1) - (6)].string));
	;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 255 "grammar.y"
    {
		(yyval.func) = func_alloc((yyvsp[(1) - (4)].string), (yyvsp[(2) - (4)].var), (yyvsp[(3) - (4)].type), 0);
		free((yyvsp[(1) - (4)].string));
    ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 259 "grammar.y"
    {
		(yyval.func) = func_alloc((yyvsp[(1) - (3)].string), (yyvsp[(2) - (3)].var), 0, 0);
		free((yyvsp[(1) - (3)].string));
	;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 266 "grammar.y"
    { (yyval.var) = (yyvsp[(2) - (3)].var); ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 267 "grammar.y"
    { (yyval.var) = 0; ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 271 "grammar.y"
    { 
		(yyval.var) = (yyvsp[(4) - (4)].var);
		(yyval.var)->next = var_alloc((yyvsp[(2) - (4)].string), 0, (yyvsp[(1) - (4)].type), 0); 
		free((yyvsp[(2) - (4)].string));
	;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 276 "grammar.y"
    { 
		(yyval.var) = var_alloc((yyvsp[(2) - (2)].string), 0, (yyvsp[(1) - (2)].type), 0);
		free((yyvsp[(2) - (2)].string));
	;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 283 "grammar.y"
    { (yyval.flag) = UNIT_FLAG_PUBLIC; ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 284 "grammar.y"
    { (yyval.flag) = UNIT_FLAG_PRIVATE; ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 285 "grammar.y"
    { (yyval.flag) = UNIT_FLAG_PROTECTED; ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 286 "grammar.y"
    { (yyval.flag) = 0; ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 290 "grammar.y"
    { (yyval.flag) = UNIT_FLAG_STATIC; ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 291 "grammar.y"
    { (yyval.flag) = 0; ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 295 "grammar.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 296 "grammar.y"
    { (yyval.type) = (yyvsp[(1) - (2)].type); /* TODO: Range */ (yyval.type)->pointer = 1; ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 297 "grammar.y"
    { (yyval.type) = (yyvsp[(1) - (2)].type); /* TODO: Nullable */ ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 298 "grammar.y"
    { 
		PARSER_CHECK(parser_resolve_type((yyval.type) = (yyvsp[(1) - (1)].type)));
	;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 301 "grammar.y"
    { 
		PARSER_CHECK(parser_resolve_type((yyval.type) = (yyvsp[(1) - (2)].type)));
		(yyval.type)->pointer = 1;
	;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 305 "grammar.y"
    { 
		PARSER_CHECK(parser_resolve_type((yyval.type) = (yyvsp[(1) - (2)].type)));
		(yyval.type)->pointer = 1; /* TODO: Nullable */
	;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 312 "grammar.y"
    { (yyval.type) = type_concat((yyvsp[(1) - (3)].type), (yyvsp[(3) - (3)].type)); ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 313 "grammar.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 314 "grammar.y"
    { 
		(yyval.type) = type_concat(type_object((yyvsp[(1) - (3)].string)), (yyvsp[(3) - (3)].type)); 
		free((yyvsp[(1) - (3)].string));
	;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 318 "grammar.y"
    { 
		(yyval.type) = type_object((yyvsp[(1) - (1)].string)); 
		free((yyvsp[(1) - (1)].string));
	;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 325 "grammar.y"
    { 
		(yyval.stmt) = (yyvsp[(2) - (3)].stmt); 
		parser->symbols = (yyval.stmt)->symbols ? symtab_get_parent((yyval.stmt)->symbols) : 0;
	;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 332 "grammar.y"
    { (yyval.stmt) = stmt_append((yyvsp[(1) - (2)].stmt), (yyvsp[(2) - (2)].stmt)); ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 333 "grammar.y"
    { (yyval.stmt) = (yyvsp[(1) - (3)].stmt); ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 334 "grammar.y"
    { 
		(yyval.stmt) = stmt_block(parser->symbols);
		parser->symbols = (yyval.stmt)->symbols;
	;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 341 "grammar.y"
    {
		(yyval.stmt) = stmt_for((yyvsp[(3) - (9)].stmt), (yyvsp[(5) - (9)].stmt), (yyvsp[(7) - (9)].stmt), (yyvsp[(9) - (9)].stmt));
	;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 344 "grammar.y"
    {
		(yyval.stmt) = stmt_foreach(var_alloc((yyvsp[(4) - (8)].string), 0, (yyvsp[(3) - (8)].type), (yyvsp[(6) - (8)].expr)), (yyvsp[(8) - (8)].stmt));
		free((yyvsp[(4) - (8)].string));
	;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 348 "grammar.y"
    {
		(yyval.stmt) = stmt_until((yyvsp[(3) - (5)].stmt), (yyvsp[(5) - (5)].stmt));
	;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 351 "grammar.y"
    {
		(yyval.stmt) = stmt_while((yyvsp[(3) - (5)].stmt), (yyvsp[(5) - (5)].stmt));
	;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 354 "grammar.y"
    {
		(yyval.stmt) = stmt_dountil((yyvsp[(2) - (7)].stmt), (yyvsp[(5) - (7)].stmt));
	;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 357 "grammar.y"
    {
		(yyval.stmt) = stmt_dowhile((yyvsp[(2) - (7)].stmt), (yyvsp[(5) - (7)].stmt));
	;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 360 "grammar.y"
    { 
		(yyval.stmt) = stmt_decl(parser, var_alloc((yyvsp[(2) - (3)].string), 0, (yyvsp[(1) - (3)].type), 0)); 
		free((yyvsp[(2) - (3)].string));
	;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 364 "grammar.y"
    { (yyval.stmt) = (yyvsp[(1) - (1)].stmt); ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 365 "grammar.y"
    { (yyval.stmt) = (yyvsp[(1) - (2)].stmt); ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 366 "grammar.y"
    { (yyval.stmt) = stmt_return((yyvsp[(2) - (3)].expr)); ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 367 "grammar.y"
    { (yyval.stmt) = stmt_return(0); ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 371 "grammar.y"
    { 
		(yyval.stmt) = stmt_decl(parser, var_alloc((yyvsp[(2) - (4)].string), 0, (yyvsp[(1) - (4)].type), (yyvsp[(4) - (4)].expr))); 
		free((yyvsp[(2) - (4)].string));
	;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 375 "grammar.y"
    { 
		(yyval.stmt) = stmt_expr((yyvsp[(1) - (1)].expr)); 
	;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 378 "grammar.y"
    { (yyval.stmt) = 0; ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 382 "grammar.y"
    {
		(yyval.stmt) = stmt_conditional((yyvsp[(3) - (5)].stmt), (yyvsp[(5) - (5)].stmt), 0);
	;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 385 "grammar.y"
    {
		(yyval.stmt) = stmt_conditional((yyvsp[(3) - (7)].stmt), (yyvsp[(5) - (7)].stmt), (yyvsp[(7) - (7)].stmt));
	;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 388 "grammar.y"
    { (yyval.stmt) = (yyvsp[(1) - (1)].stmt); ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 391 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 394 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_binary(parser, op_null_check, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)));
	;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 401 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_binary(parser, op_assign, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr))); 
	;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 404 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_binary(parser, op_mul_assign, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr))); 
	;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 407 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_binary(parser, op_div_assign, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr))); 
	;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 410 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_binary(parser, op_mod_assign, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr))); 
	;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 413 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_binary(parser, op_sub_assign, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr))); 
	;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 416 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_binary(parser, op_add_assign, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr))); 
	;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 419 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_binary(parser, op_bitand_assign, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr))); 
	;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 422 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_binary(parser, op_bitor_assign, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr))); 
	;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 425 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 429 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_binary(parser, op_or, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr))); 
	;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 432 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 436 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_binary(parser, op_and, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr))); 
	;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 439 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 443 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_binary(parser, op_bitor, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr))); 
	;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 446 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_binary(parser, op_bitxor, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr))); 
	;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 449 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 453 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_binary(parser, op_bitand, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr))); 
	;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 456 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 460 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_binary(parser, op_equal, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr))); 
	;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 463 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_binary(parser, op_notequal, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr))); 
	;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 466 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 470 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_binary(parser, op_greater, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr))); 
	;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 473 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_binary(parser, op_less, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr))); 
	;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 476 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_binary(parser, op_ge, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr))); 
	;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 479 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_binary(parser, op_le, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr))); 
	;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 482 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 486 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_binary(parser, op_lshift, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr))); 
	;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 489 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_binary(parser, op_rshift, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr))); 
	;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 492 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 496 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_binary(parser, op_plus, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr))); 
	;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 499 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_binary(parser, op_minus, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr))); 
	;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 502 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 506 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_binary(parser, op_mul, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr))); 
	;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 509 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_binary(parser, op_div, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr))); 
	;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 512 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_binary(parser, op_mod, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr))); 
	;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 515 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 519 "grammar.y"
    { PARSER_CHECK((yyval.expr) = expr_unary(parser, op_inc, (yyvsp[(2) - (2)].expr))); ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 520 "grammar.y"
    { PARSER_CHECK((yyval.expr) = expr_unary(parser, op_dec, (yyvsp[(2) - (2)].expr))); ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 521 "grammar.y"
    { PARSER_CHECK((yyval.expr) = expr_unary(parser, op_plus, (yyvsp[(2) - (2)].expr))); ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 522 "grammar.y"
    { PARSER_CHECK((yyval.expr) = expr_unary(parser, op_minus, (yyvsp[(2) - (2)].expr))); ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 523 "grammar.y"
    { PARSER_CHECK((yyval.expr) = expr_unary(parser, op_bang, (yyvsp[(2) - (2)].expr))); ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 524 "grammar.y"
    { PARSER_CHECK((yyval.expr) = expr_unary(parser, op_tilde, (yyvsp[(2) - (2)].expr))); ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 525 "grammar.y"
    { PARSER_CHECK((yyval.expr) = expr_unary(parser, op_star, (yyvsp[(2) - (2)].expr))); ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 526 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 530 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_call(parser, (yyvsp[(1) - (4)].expr), (yyvsp[(3) - (4)].expr))); 
	;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 533 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_call(parser, (yyvsp[(1) - (3)].expr), 0)); 
	;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 536 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_index(parser, (yyvsp[(1) - (4)].expr), (yyvsp[(3) - (4)].expr))); 
	;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 539 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_member(parser, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].string))); 
		free((yyvsp[(3) - (3)].string)); 
	;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 543 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_unary(parser, op_postinc, (yyvsp[(1) - (2)].expr))); 
	;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 546 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_unary(parser, op_postdec, (yyvsp[(1) - (2)].expr))); 
	;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 549 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 553 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_var(parser, (yyvsp[(1) - (1)].string))); 
		free((yyvsp[(1) - (1)].string)); 
	;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 557 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_static(parser, (yyvsp[(1) - (3)].type), (yyvsp[(3) - (3)].string))); 
		free((yyvsp[(3) - (3)].string)); 
	;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 561 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_static(parser, (yyvsp[(1) - (3)].type), (yyvsp[(3) - (3)].string))); 
		free((yyvsp[(3) - (3)].string)); 
	;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 565 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_ctor(parser, (yyvsp[(1) - (4)].type), (yyvsp[(3) - (4)].expr))); 
	;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 568 "grammar.y"
    { 
		PARSER_CHECK((yyval.expr) = expr_ctor(parser, (yyvsp[(1) - (3)].type), 0)); 
	;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 571 "grammar.y"
    { (yyval.expr) = (yyvsp[(2) - (3)].expr); ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 572 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 573 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 577 "grammar.y"
    { 
		(yyval.expr) = (yyvsp[(3) - (3)].expr);
		(yyval.expr)->next = (yyvsp[(1) - (3)].expr);
	;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 581 "grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;



/* Line 1455 of yacc.c  */
#line 3335 "grammar.y.c"
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



