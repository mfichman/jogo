
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

/* Substitute the variable and function names.  */
#define yyparse         parser_parse
#define yylex           parser_lex
#define yyerror         parser_error
#define yylval          parser_lval
#define yychar          parser_char
#define yydebug         parser_debug
#define yynerrs         parser_nerrs


/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "grammar.y"

#include "parser.hpp"


/* Line 189 of yacc.c  */
#line 86 "grammar.tab.c"

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
     TOK_CLASS = 258,
     TOK_INTERFACE = 259,
     TOK_STRUCT = 260,
     TOK_MODULE = 261,
     TOK_IMPORT = 262,
     TOK_DEF = 263,
     TOK_IDENT = 264,
     TOK_INIT = 265,
     TOK_DESTROY = 266,
     TOK_PUBLIC = 267,
     TOK_PRIVATE = 268,
     TOK_PROTECTED = 269,
     TOK_STATIC = 270,
     TOK_NATIVE = 271,
     TOK_INT = 272,
     TOK_UINT = 273,
     TOK_SHORT = 274,
     TOK_USHORT = 275,
     TOK_BYTE = 276,
     TOK_UBYTE = 277,
     TOK_LONG = 278,
     TOK_ULONG = 279,
     TOK_TYPE = 280,
     TOK_EQUAL = 281,
     TOK_NOTEQUAL = 282,
     TOK_SCOPE = 283,
     TOK_OR = 284,
     TOK_AND = 285,
     TOK_GE = 286,
     TOK_LE = 287,
     TOK_LSHIFT = 288,
     TOK_RSHIFT = 289,
     TOK_STRING = 290,
     TOK_NUMBER = 291,
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
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 173 "grammar.tab.c"

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
#define YYLAST   267

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  67
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  35
/* YYNRULES -- Number of rules.  */
#define YYNRULES  114
/* YYNRULES -- Number of states.  */
#define YYNSTATES  211

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
       2,     2,     2,    62,     2,     2,     2,    61,    54,     2,
      48,    49,    59,    57,     2,    58,    66,    60,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    46,
      56,    47,    55,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    64,     2,    65,    53,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    50,    52,    51,    63,     2,     2,     2,
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
      35,    38,    41,    42,    45,    48,    51,    52,    55,    58,
      61,    64,    67,    68,    71,    74,    77,    78,    82,    87,
      95,   101,   106,   110,   118,   123,   126,   128,   130,   132,
     133,   135,   136,   138,   139,   141,   143,   145,   147,   149,
     151,   153,   155,   157,   161,   163,   167,   170,   171,   174,
     178,   184,   188,   192,   196,   200,   204,   208,   212,   216,
     218,   222,   224,   228,   230,   234,   238,   240,   244,   246,
     250,   254,   256,   260,   264,   268,   272,   274,   278,   282,
     284,   288,   292,   294,   298,   302,   306,   308,   311,   314,
     317,   320,   323,   326,   329,   331,   334,   339,   343,   346,
     349,   351,   353,   355,   357
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      68,     0,    -1,     3,    85,    46,    69,    -1,     4,    85,
      46,    70,    -1,     5,    85,    46,    71,    -1,     6,    85,
      46,    72,    -1,    73,    69,    -1,    74,    69,    -1,    75,
      69,    -1,    76,    69,    -1,    77,    69,    -1,    78,    69,
      -1,    -1,    73,    70,    -1,    74,    70,    -1,    79,    70,
      -1,    -1,    73,    71,    -1,    74,    71,    -1,    75,    71,
      -1,    76,    71,    -1,    78,    71,    -1,    -1,    73,    72,
      -1,    74,    72,    -1,    78,    72,    -1,    -1,     7,    85,
      46,    -1,     8,    84,    25,    46,    -1,    81,    82,    84,
       9,    47,    89,    46,    -1,    81,    82,    84,     9,    46,
      -1,    10,    80,    81,    86,    -1,    11,    80,    86,    -1,
       9,    80,    81,    82,    83,    84,    86,    -1,     9,    80,
      84,    46,    -1,    48,    49,    -1,    12,    -1,    13,    -1,
      14,    -1,    -1,    15,    -1,    -1,    16,    -1,    -1,    18,
      -1,    17,    -1,    20,    -1,    19,    -1,    22,    -1,    21,
      -1,    24,    -1,    23,    -1,    85,    -1,    25,    28,    85,
      -1,    25,    -1,    50,    87,    51,    -1,    88,    87,    -1,
      -1,    89,    46,    -1,    84,     9,    46,    -1,    84,     9,
      47,    89,    46,    -1,    99,    47,    89,    -1,    99,    37,
      89,    -1,    99,    38,    89,    -1,    99,    41,    89,    -1,
      99,    39,    89,    -1,    99,    40,    89,    -1,    99,    43,
      89,    -1,    99,    42,    89,    -1,    90,    -1,    90,    29,
      91,    -1,    91,    -1,    91,    29,    92,    -1,    92,    -1,
      92,    52,    93,    -1,    92,    53,    93,    -1,    93,    -1,
      93,    54,    94,    -1,    94,    -1,    94,    26,    95,    -1,
      94,    27,    95,    -1,    95,    -1,    95,    55,    96,    -1,
      95,    56,    96,    -1,    95,    31,    96,    -1,    95,    32,
      96,    -1,    96,    -1,    96,    33,    97,    -1,    96,    34,
      97,    -1,    97,    -1,    97,    57,    98,    -1,    97,    58,
      98,    -1,    98,    -1,    98,    59,    99,    -1,    98,    60,
      99,    -1,    98,    61,    99,    -1,    99,    -1,    44,    99,
      -1,    45,    99,    -1,    57,    99,    -1,    58,    99,    -1,
      62,    99,    -1,    63,    99,    -1,    59,    99,    -1,   100,
      -1,   100,    80,    -1,   100,    64,    89,    65,    -1,   100,
      66,     9,    -1,   100,    44,    -1,   100,    45,    -1,   101,
      -1,    35,    -1,    36,    -1,     9,    -1,    48,    89,    49,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    61,    61,    65,    69,    73,    79,    80,    81,    82,
      83,    84,    85,    89,    90,    91,    92,    96,    97,    98,
      99,   100,   101,   105,   106,   107,   108,   112,   118,   124,
     128,   134,   140,   146,   152,   158,   162,   163,   164,   165,
     169,   170,   174,   175,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   191,   192,   196,   200,   201,   205,   206,
     207,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     223,   224,   228,   229,   233,   234,   235,   239,   240,   244,
     245,   246,   250,   251,   252,   253,   254,   258,   259,   260,
     264,   265,   266,   270,   271,   272,   273,   277,   278,   279,
     280,   281,   282,   283,   284,   288,   289,   290,   291,   292,
     293,   297,   298,   299,   300
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOK_CLASS", "TOK_INTERFACE",
  "TOK_STRUCT", "TOK_MODULE", "TOK_IMPORT", "TOK_DEF", "TOK_IDENT",
  "TOK_INIT", "TOK_DESTROY", "TOK_PUBLIC", "TOK_PRIVATE", "TOK_PROTECTED",
  "TOK_STATIC", "TOK_NATIVE", "TOK_INT", "TOK_UINT", "TOK_SHORT",
  "TOK_USHORT", "TOK_BYTE", "TOK_UBYTE", "TOK_LONG", "TOK_ULONG",
  "TOK_TYPE", "TOK_EQUAL", "TOK_NOTEQUAL", "TOK_SCOPE", "TOK_OR",
  "TOK_AND", "TOK_GE", "TOK_LE", "TOK_LSHIFT", "TOK_RSHIFT", "TOK_STRING",
  "TOK_NUMBER", "TOK_MUL_ASSIGN", "TOK_DIV_ASSIGN", "TOK_SUB_ASSIGN",
  "TOK_ADD_ASSIGN", "TOK_MOD_ASSIGN", "TOK_BITOR_ASSIGN",
  "TOK_BITAND_ASSIGN", "TOK_INC", "TOK_DEC", "';'", "'='", "'('", "')'",
  "'{'", "'}'", "'|'", "'^'", "'&'", "'>'", "'<'", "'+'", "'-'", "'*'",
  "'/'", "'%'", "'!'", "'~'", "'['", "']'", "'.'", "$accept",
  "translation_unit", "class_member_list", "interface_member_list",
  "struct_member_list", "module_member_list", "import", "def", "variable",
  "constructor", "destructor", "function", "prototype", "argument_list",
  "access", "storage", "native", "type", "qualified_name",
  "compound_statement", "statement_list", "statement", "assignment",
  "logical_or", "logical_and", "bitwise_or", "bitwise_and", "equality",
  "relation", "shift", "addition", "multiplication", "unary", "postfix",
  "primary", 0
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
     123,   125,   124,    94,    38,    62,    60,    43,    45,    42,
      47,    37,    33,   126,    91,    93,    46
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    67,    68,    68,    68,    68,    69,    69,    69,    69,
      69,    69,    69,    70,    70,    70,    70,    71,    71,    71,
      71,    71,    71,    72,    72,    72,    72,    73,    74,    75,
      75,    76,    77,    78,    79,    80,    81,    81,    81,    81,
      82,    82,    83,    83,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    85,    85,    86,    87,    87,    88,    88,
      88,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      90,    90,    91,    91,    92,    92,    92,    93,    93,    94,
      94,    94,    95,    95,    95,    95,    95,    96,    96,    96,
      97,    97,    97,    98,    98,    98,    98,    99,    99,    99,
      99,    99,    99,    99,    99,   100,   100,   100,   100,   100,
     100,   101,   101,   101,   101
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     4,     4,     4,     2,     2,     2,     2,
       2,     2,     0,     2,     2,     2,     0,     2,     2,     2,
       2,     2,     0,     2,     2,     2,     0,     3,     4,     7,
       5,     4,     3,     7,     4,     2,     1,     1,     1,     0,
       1,     0,     1,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     3,     2,     0,     2,     3,
       5,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       3,     1,     3,     1,     3,     3,     1,     3,     1,     3,
       3,     1,     3,     3,     3,     3,     1,     3,     3,     1,
       3,     3,     1,     3,     3,     3,     1,     2,     2,     2,
       2,     2,     2,     2,     1,     2,     4,     3,     2,     2,
       1,     1,     1,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,    54,     0,     0,     0,
       0,     1,     0,    39,    16,    39,    26,    53,     0,     0,
       0,     0,     0,    36,    37,    38,     2,    39,    39,    39,
      39,    39,    39,    41,     0,     3,    16,    16,    16,     4,
      39,    39,    39,    39,    39,     5,    26,    26,    26,     0,
      45,    44,    47,    46,    49,    48,    51,    50,     0,    52,
       0,    39,    39,     0,     6,     7,     8,     9,    10,    11,
      40,     0,     0,    13,    14,    15,    17,    18,    19,    20,
      21,    23,    24,    25,    27,     0,    35,    41,     0,    57,
      32,     0,     0,    28,    43,    31,   113,   111,   112,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,    69,    71,    73,    76,    78,    81,    86,    89,    92,
      96,   104,   110,     0,    34,    42,     0,    97,    98,     0,
      99,   100,   103,   101,   102,     0,    55,    56,    58,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   108,   109,     0,     0,   105,
      30,     0,     0,   114,    59,     0,    70,    96,    72,    74,
      75,    77,    79,    80,    84,    85,    82,    83,    87,    88,
      90,    91,    93,    94,    95,    62,    63,    65,    66,    64,
      68,    67,    61,     0,   107,     0,    33,     0,   106,    29,
      60
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,    26,    35,    39,    45,    27,    28,    29,    30,
      31,    32,    38,    61,    33,    71,   126,   107,    59,    90,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   177,   121,   122
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -85
static const yytype_int16 yypact[] =
{
       5,   -21,   -21,   -21,   -21,    13,    26,   -11,    35,    54,
      90,   -85,   -21,   208,    42,   223,    60,   -85,   -21,   176,
     116,   116,   116,   -85,   -85,   -85,   -85,   208,   208,   208,
     208,   208,   208,   153,   116,   -85,    42,    42,    42,   -85,
     223,   223,   223,   223,   223,   -85,    60,    60,    60,   128,
     -85,   -85,   -85,   -85,   -85,   -85,   -85,   -85,   158,   -85,
     140,   134,   134,   152,   -85,   -85,   -85,   -85,   -85,   -85,
     -85,   176,   176,   -85,   -85,   -85,   -85,   -85,   -85,   -85,
     -85,   -85,   -85,   -85,   -85,   188,   -85,   153,   152,    94,
     -85,   240,   204,   -85,   235,   -85,   -85,   -85,   -85,    -2,
      -2,    -2,    -2,    -2,    -2,    -2,    -2,   243,   202,    94,
     210,   225,   226,   -26,   203,    32,    -8,    11,     7,   102,
     201,    18,   -85,    39,   -85,   -85,   176,   -85,   -85,   211,
     -85,   -85,   -85,   -85,   -85,    58,   -85,   -85,   -85,    -2,
      -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,
      -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,
      -2,    -2,    -2,    -2,    -2,   -85,   -85,    -2,   249,   -85,
     -85,    -2,   152,   -85,   -85,    -2,   226,   -85,   -26,   203,
     203,    32,    -8,    -8,    11,    11,    11,    11,     7,     7,
     102,   102,   -85,   -85,   -85,   -85,   -85,   -85,   -85,   -85,
     -85,   -85,   -85,   194,   -85,   215,   -85,   216,   -85,   -85,
     -85
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -85,   -85,   197,   209,    84,   166,   129,   144,   -12,    -3,
     -85,   163,   -85,   -20,    27,   177,   -85,   -19,   119,   -82,
     154,   -85,   -65,   -85,   126,   127,   -33,   123,    -4,   -14,
      -1,     2,   -84,   -85,   -85
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -23
static const yytype_int16 yytable[] =
{
      58,    62,    63,    42,     6,   120,    95,    96,     1,     2,
       3,     4,    43,    11,    72,   127,   128,   120,   130,   131,
     132,   133,   134,   146,   147,   120,   141,   142,    42,    42,
      42,    42,    42,    97,    98,    13,   129,    43,    43,    43,
      43,    43,    99,   100,   150,   151,   101,   148,   149,    18,
      19,    34,    91,    92,    12,   102,   103,   104,   144,   145,
     105,   106,   165,   166,   152,   153,    60,    18,    19,    20,
     192,   193,   194,   120,   120,   120,   120,   120,   120,   120,
     120,    14,   167,   120,   168,   170,   171,   120,    87,    88,
     206,   120,   195,   196,   197,   198,   199,   200,   201,   202,
      15,   169,   203,    96,   174,   175,   205,   172,   179,   180,
     207,    50,    51,    52,    53,    54,    55,    56,    57,     6,
       7,     8,     9,    10,    76,    77,    78,    79,    80,    97,
      98,    17,   184,   185,   186,   187,    16,    49,    99,   100,
     182,   183,   101,    36,    40,    46,    23,    24,    25,   188,
     189,   102,   103,   104,   190,   191,   105,   106,    37,    41,
      47,   154,   155,   156,    60,    36,    36,    36,    70,    40,
      40,    40,    40,    40,    84,    46,    46,    46,    44,    48,
      37,    37,    37,    85,    41,    41,    41,    41,    41,    86,
      47,    47,    47,    50,    51,    52,    53,    54,    55,    56,
      57,     6,    89,    44,    44,    44,    44,    44,   -12,    48,
      48,    48,    81,    82,    83,    18,    19,    20,    21,    22,
      23,    24,    25,   -22,    64,    65,    66,    67,    68,    69,
      18,    19,    20,    21,    93,    23,    24,    25,   157,   158,
     159,   160,   161,   162,   163,    73,    74,    75,   164,   123,
     124,   125,   135,   136,   139,   140,   138,   143,   204,   208,
     173,   209,   210,   137,    94,   176,   181,   178
};

static const yytype_uint8 yycheck[] =
{
      19,    21,    22,    15,    25,    89,    88,     9,     3,     4,
       5,     6,    15,     0,    34,    99,   100,   101,   102,   103,
     104,   105,   106,    31,    32,   109,    52,    53,    40,    41,
      42,    43,    44,    35,    36,    46,   101,    40,    41,    42,
      43,    44,    44,    45,    33,    34,    48,    55,    56,     7,
       8,     9,    71,    72,    28,    57,    58,    59,    26,    27,
      62,    63,    44,    45,    57,    58,    48,     7,     8,     9,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    46,    64,   167,    66,    46,    47,   171,    61,    62,
     172,   175,   157,   158,   159,   160,   161,   162,   163,   164,
      46,   121,   167,     9,    46,    47,   171,   126,   141,   142,
     175,    17,    18,    19,    20,    21,    22,    23,    24,    25,
       1,     2,     3,     4,    40,    41,    42,    43,    44,    35,
      36,    12,   146,   147,   148,   149,    46,    18,    44,    45,
     144,   145,    48,    14,    15,    16,    12,    13,    14,   150,
     151,    57,    58,    59,   152,   153,    62,    63,    14,    15,
      16,    59,    60,    61,    48,    36,    37,    38,    15,    40,
      41,    42,    43,    44,    46,    46,    47,    48,    15,    16,
      36,    37,    38,    25,    40,    41,    42,    43,    44,    49,
      46,    47,    48,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    50,    40,    41,    42,    43,    44,     0,    46,
      47,    48,    46,    47,    48,     7,     8,     9,    10,    11,
      12,    13,    14,     0,    27,    28,    29,    30,    31,    32,
       7,     8,     9,    10,    46,    12,    13,    14,    37,    38,
      39,    40,    41,    42,    43,    36,    37,    38,    47,     9,
      46,    16,     9,    51,    29,    29,    46,    54,     9,    65,
      49,    46,    46,   109,    87,   139,   143,   140
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,    68,    25,    85,    85,    85,
      85,     0,    28,    46,    46,    46,    46,    85,     7,     8,
       9,    10,    11,    12,    13,    14,    69,    73,    74,    75,
      76,    77,    78,    81,     9,    70,    73,    74,    79,    71,
      73,    74,    75,    76,    78,    72,    73,    74,    78,    85,
      17,    18,    19,    20,    21,    22,    23,    24,    84,    85,
      48,    80,    80,    80,    69,    69,    69,    69,    69,    69,
      15,    82,    80,    70,    70,    70,    71,    71,    71,    71,
      71,    72,    72,    72,    46,    25,    49,    81,    81,    50,
      86,    84,    84,    46,    82,    86,     9,    35,    36,    44,
      45,    48,    57,    58,    59,    62,    63,    84,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,     9,    46,    16,    83,    99,    99,    89,
      99,    99,    99,    99,    99,     9,    51,    87,    46,    29,
      29,    52,    53,    54,    26,    27,    31,    32,    55,    56,
      33,    34,    57,    58,    59,    60,    61,    37,    38,    39,
      40,    41,    42,    43,    47,    44,    45,    64,    66,    80,
      46,    47,    84,    49,    46,    47,    91,    99,    92,    93,
      93,    94,    95,    95,    96,    96,    96,    96,    97,    97,
      98,    98,    99,    99,    99,    89,    89,    89,    89,    89,
      89,    89,    89,    89,     9,    89,    86,    89,    65,    46,
      46
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
#line 61 "grammar.y"
    {
        parser_class(parser);
    ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 65 "grammar.y"
    {
        parser_interface(parser);
    ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 69 "grammar.y"
    {
        parser_struct(parser);
    ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 73 "grammar.y"
    {
        parser_module(parser);
    ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 112 "grammar.y"
    {
        parser_import(parser, (yyvsp[(2) - (3)]));
    ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 118 "grammar.y"
    {
        parser_def(parser, (yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]));
    ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 124 "grammar.y"
    {
        parser_varinit(parser, (yyvsp[(1) - (7)]), (yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]));
    ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 128 "grammar.y"
    {
        parser_vardecl(parser, (yyvsp[(1) - (5)]), (yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]));
    ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 134 "grammar.y"
    {
        parser_ctor(parser, (yyvsp[(1) - (4)]), (yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]));
    ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 140 "grammar.y"
    {
        parser_dtor(parser, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]));
    ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 146 "grammar.y"
    {
        parser_func(parser, (yyvsp[(1) - (7)]), (yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)]), (yyvsp[(7) - (7)]));
    ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 152 "grammar.y"
    {
        parser_proto(parser, (yyvsp[(1) - (4)]), (yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]));
    ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 158 "grammar.y"
    { (yyval) = 0; ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 162 "grammar.y"
    { (yyval) = node_str("public"); ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 163 "grammar.y"
    { (yyval) = node_str("private"); ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 164 "grammar.y"
    { (yyval) = node_str("protected"); ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 165 "grammar.y"
    { (yyval) = 0; ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 169 "grammar.y"
    { (yyval) = node_str("static"); ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 170 "grammar.y"
    { (yyval) = 0; ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 174 "grammar.y"
    { (yyval) = node_str("native"); ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 175 "grammar.y"
    { (yyval) = 0; ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 179 "grammar.y"
    { (yyval) = node_str("uint"); ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 180 "grammar.y"
    { (yyval) = node_str("int"); ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 181 "grammar.y"
    { (yyval) = node_str("ushort"); ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 182 "grammar.y"
    { (yyval) = node_str("short"); ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 183 "grammar.y"
    { (yyval) = node_str("ubyte"); ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 184 "grammar.y"
    { (yyval) = node_str("byte"); ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 185 "grammar.y"
    { (yyval) = node_str("ulong"); ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 186 "grammar.y"
    { (yyval) = node_str("long"); ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 187 "grammar.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 191 "grammar.y"
    { (yyval) = node_strcat2((yyval), "::", (yyvsp[(3) - (3)])); ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 192 "grammar.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 196 "grammar.y"
    { (yyval) = (yyvsp[(1) - (3)]); ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 200 "grammar.y"
    { (yyval) = node_prepend((yyvsp[(2) - (2)]), (yyvsp[(1) - (2)])); ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 201 "grammar.y"
    { (yyval) = node_alloc(); ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 205 "grammar.y"
    { (yyval) = (yyvsp[(1) - (2)]); ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 206 "grammar.y"
    { (yyval) = node_binary((yyvsp[(1) - (3)]), (yyvsp[(2) - (3)])); ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 207 "grammar.y"
    { (yyval) = node_trinary((yyvsp[(1) - (5)]), (yyvsp[(2) - (5)]), (yyvsp[(3) - (5)])); ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 211 "grammar.y"
    { (yyval) = node_trinary((yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 212 "grammar.y"
    { (yyval) = node_trinary((yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 213 "grammar.y"
    { (yyval) = node_trinary((yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 214 "grammar.y"
    { (yyval) = node_trinary((yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 215 "grammar.y"
    { (yyval) = node_trinary((yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 216 "grammar.y"
    { (yyval) = node_trinary((yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 217 "grammar.y"
    { (yyval) = node_trinary((yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 218 "grammar.y"
    { (yyval) = node_trinary((yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 219 "grammar.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 223 "grammar.y"
    { (yyval) = node_trinary((yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 224 "grammar.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 228 "grammar.y"
    { (yyval) = node_trinary((yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 229 "grammar.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 288 "grammar.y"
    { (yyval) = node_trinary((yyvsp[(1) - (2)]), node_str("call"), (yyvsp[(2) - (2)])); ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 289 "grammar.y"
    { (yyval) = node_trinary((yyvsp[(1) - (4)]), node_str("[]"), (yyvsp[(2) - (4)])); ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 290 "grammar.y"
    { (yyval) = node_trinary((yyvsp[(1) - (3)]), node_str("."); (yyvsp[(2) - (3)])); ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 291 "grammar.y"
    { (yyval) = node_binary((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 292 "grammar.y"
    { (yyval) = node_binary((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 293 "grammar.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 297 "grammar.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 298 "grammar.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 299 "grammar.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 300 "grammar.y"
    { (yyvsp[(2) - (3)]); ;}
    break;



/* Line 1455 of yacc.c  */
#line 2043 "grammar.tab.c"
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



