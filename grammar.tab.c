
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


/* Line 189 of yacc.c  */
#line 79 "grammar.tab.c"

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
#line 15 "grammar.y"
 expr_t *expr; 

/* Line 214 of yacc.c  */
#line 16 "grammar.y"
 statement_t *statement; 

/* Line 214 of yacc.c  */
#line 17 "grammar.y"
 type_t *type; 

/* Line 214 of yacc.c  */
#line 18 "grammar.y"
 const char *string; 


/* Line 214 of yacc.c  */
#line 176 "grammar.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 188 "grammar.tab.c"

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
#define YYLAST   304

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  68
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  37
/* YYNRULES -- Number of rules.  */
#define YYNRULES  118
/* YYNRULES -- Number of states.  */
#define YYNSTATES  253

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
      35,    38,    41,    42,    45,    48,    51,    52,    55,    58,
      61,    64,    67,    68,    71,    74,    77,    78,    82,    87,
      95,   101,   106,   110,   118,   123,   126,   128,   130,   132,
     133,   135,   136,   138,   139,   141,   143,   147,   149,   153,
     156,   157,   167,   176,   182,   188,   196,   204,   210,   214,
     216,   219,   225,   233,   235,   237,   241,   245,   249,   253,
     257,   261,   265,   269,   271,   275,   277,   281,   283,   287,
     291,   293,   297,   299,   303,   307,   309,   313,   317,   321,
     325,   327,   331,   335,   337,   341,   345,   347,   351,   355,
     359,   361,   364,   367,   370,   373,   376,   379,   382,   384,
     387,   392,   396,   399,   402,   404,   406,   408,   410
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      69,     0,    -1,     8,    86,    46,    70,    -1,     9,    86,
      46,    71,    -1,    10,    86,    46,    72,    -1,    11,    86,
      46,    73,    -1,    74,    70,    -1,    75,    70,    -1,    76,
      70,    -1,    77,    70,    -1,    78,    70,    -1,    79,    70,
      -1,    -1,    74,    71,    -1,    75,    71,    -1,    80,    71,
      -1,    -1,    74,    72,    -1,    75,    72,    -1,    76,    72,
      -1,    77,    72,    -1,    79,    72,    -1,    -1,    74,    73,
      -1,    75,    73,    -1,    79,    73,    -1,    -1,    12,    86,
      46,    -1,    13,    85,     3,    46,    -1,    82,    83,    85,
       5,    47,    91,    46,    -1,    82,    83,    85,     5,    46,
      -1,    14,    81,    82,    87,    -1,    15,    81,    87,    -1,
       5,    81,    82,    83,    84,    85,    87,    -1,     5,    81,
      85,    46,    -1,    48,    49,    -1,    16,    -1,    17,    -1,
      18,    -1,    -1,    19,    -1,    -1,    20,    -1,    -1,     4,
      -1,    86,    -1,     3,    30,    86,    -1,     3,    -1,    50,
      88,    51,    -1,    89,    88,    -1,    -1,    27,    48,    91,
      46,    91,    46,    91,    49,    87,    -1,    25,    48,    85,
       5,    52,    91,    49,    87,    -1,    23,    48,    91,    49,
      87,    -1,    21,    48,    91,    49,    87,    -1,    26,    87,
      23,    48,    91,    49,    46,    -1,    26,    87,    21,    48,
      91,    49,    46,    -1,    85,     5,    47,    91,    46,    -1,
      85,     5,    46,    -1,    90,    -1,    91,    46,    -1,    24,
      48,    91,    49,    87,    -1,    24,    48,    91,    49,    87,
      22,    90,    -1,    87,    -1,    92,    -1,   102,    47,    92,
      -1,   102,    37,    92,    -1,   102,    38,    92,    -1,   102,
      41,    92,    -1,   102,    39,    92,    -1,   102,    40,    92,
      -1,   102,    43,    92,    -1,   102,    42,    92,    -1,    93,
      -1,    93,    31,    94,    -1,    94,    -1,    94,    32,    95,
      -1,    95,    -1,    95,    53,    96,    -1,    95,    54,    96,
      -1,    96,    -1,    96,    55,    97,    -1,    97,    -1,    97,
      28,    98,    -1,    97,    29,    98,    -1,    98,    -1,    98,
      56,    99,    -1,    98,    57,    99,    -1,    98,    33,    99,
      -1,    98,    34,    99,    -1,    99,    -1,    99,    35,   100,
      -1,    99,    36,   100,    -1,   100,    -1,   100,    58,   101,
      -1,   100,    59,   101,    -1,   101,    -1,   101,    60,   102,
      -1,   101,    61,   102,    -1,   101,    62,   102,    -1,   102,
      -1,    44,   102,    -1,    45,   102,    -1,    58,   102,    -1,
      59,   102,    -1,    63,   102,    -1,    64,   102,    -1,    60,
     102,    -1,   103,    -1,   103,    81,    -1,   103,    65,    91,
      66,    -1,   103,    67,     5,    -1,   103,    44,    -1,   103,
      45,    -1,   104,    -1,     6,    -1,     7,    -1,     5,    -1,
      48,    91,    49,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   103,   103,   107,   111,   115,   122,   123,   124,   125,
     126,   127,   128,   132,   133,   134,   135,   139,   140,   141,
     142,   143,   144,   148,   149,   150,   151,   155,   162,   169,
     173,   180,   187,   194,   201,   208,   212,   213,   214,   215,
     219,   220,   224,   225,   229,   230,   234,   235,   239,   243,
     244,   248,   251,   254,   257,   260,   263,   266,   269,   270,
     271,   275,   278,   281,   284,   287,   288,   291,   294,   297,
     300,   303,   306,   309,   313,   314,   318,   319,   323,   324,
     325,   329,   330,   334,   335,   338,   342,   343,   344,   345,
     346,   350,   351,   352,   356,   357,   358,   362,   363,   364,
     365,   369,   370,   371,   372,   373,   374,   375,   376,   380,
     381,   382,   383,   384,   385,   389,   390,   391,   392
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
  "'.'", "$accept", "translation_unit", "class_member_list",
  "interface_member_list", "struct_member_list", "module_member_list",
  "import", "def", "variable", "constructor", "destructor", "function",
  "prototype", "argument_list", "access", "storage", "native", "type",
  "qualified_name", "block", "statement_list", "statement", "conditional",
  "expression", "assignment", "logical_or", "logical_and", "bitwise_or",
  "bitwise_and", "equality", "relation", "shift", "addition",
  "multiplication", "unary", "postfix", "primary", 0
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
      70,    70,    70,    71,    71,    71,    71,    72,    72,    72,
      72,    72,    72,    73,    73,    73,    73,    74,    75,    76,
      76,    77,    78,    79,    80,    81,    82,    82,    82,    82,
      83,    83,    84,    84,    85,    85,    86,    86,    87,    88,
      88,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    90,    90,    90,    91,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    93,    93,    94,    94,    95,    95,
      95,    96,    96,    97,    97,    97,    98,    98,    98,    98,
      98,    99,    99,    99,   100,   100,   100,   101,   101,   101,
     101,   102,   102,   102,   102,   102,   102,   102,   102,   103,
     103,   103,   103,   103,   103,   104,   104,   104,   104
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     4,     4,     4,     2,     2,     2,     2,
       2,     2,     0,     2,     2,     2,     0,     2,     2,     2,
       2,     2,     0,     2,     2,     2,     0,     3,     4,     7,
       5,     4,     3,     7,     4,     2,     1,     1,     1,     0,
       1,     0,     1,     0,     1,     1,     3,     1,     3,     2,
       0,     9,     8,     5,     5,     7,     7,     5,     3,     1,
       2,     5,     7,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     3,     1,     3,     1,     3,     3,
       1,     3,     1,     3,     3,     1,     3,     3,     3,     3,
       1,     3,     3,     1,     3,     3,     1,     3,     3,     3,
       1,     2,     2,     2,     2,     2,     2,     2,     1,     2,
       4,     3,     2,     2,     1,     1,     1,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,    47,     0,     0,     0,
       0,     1,     0,    39,    16,    39,    26,    46,     0,     0,
       0,     0,     0,    36,    37,    38,     2,    39,    39,    39,
      39,    39,    39,    41,     0,     3,    16,    16,    16,     4,
      39,    39,    39,    39,    39,     5,    26,    26,    26,     0,
      39,     0,    44,     0,    45,    39,     0,     6,     7,     8,
       9,    10,    11,    40,     0,     0,    13,    14,    15,    17,
      18,    19,    20,    21,    23,    24,    25,    35,    41,    27,
       0,     0,    50,    32,     0,     0,    43,    28,    31,   117,
     115,   116,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,    50,
      59,     0,    64,    73,    75,    77,    80,    82,    85,    90,
      93,    96,   100,   108,   114,     0,    34,    42,     0,     0,
       0,     0,     0,     0,     0,   101,   102,     0,   103,   104,
     107,   105,   106,     0,    48,    49,    60,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   112,   113,     0,     0,   109,    30,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   118,    58,
       0,    74,   100,    76,    78,    79,    81,    83,    84,    88,
      89,    86,    87,    91,    92,    94,    95,    97,    98,    99,
      66,    67,    69,    70,    68,    72,    71,    65,     0,   111,
       0,    33,     0,     0,     0,     0,     0,     0,     0,     0,
     110,    29,    54,    53,    61,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,    62,     0,    56,    55,     0,
      52,     0,    51
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,    26,    35,    39,    45,    27,    28,    29,    30,
      31,    32,    38,    50,    33,    64,   128,   106,    54,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -100
static const yytype_int16 yypact[] =
{
      76,     5,     5,     5,     5,    24,    -2,   -13,    -7,     2,
      35,  -100,     5,   213,    14,   232,    70,  -100,    42,     5,
     119,    42,    42,  -100,  -100,  -100,  -100,   213,   213,   213,
     213,   213,   213,    74,    42,  -100,    14,    14,    14,  -100,
     232,   232,   232,   232,   232,  -100,    70,    70,    70,    46,
       4,    63,  -100,   113,  -100,     4,    85,  -100,  -100,  -100,
    -100,  -100,  -100,  -100,   119,   119,  -100,  -100,  -100,  -100,
    -100,  -100,  -100,  -100,  -100,  -100,  -100,  -100,    74,  -100,
      91,    85,    94,  -100,   138,   100,   139,  -100,  -100,  -100,
    -100,  -100,   124,   130,   146,   148,    85,   169,    44,    44,
      44,    44,    44,    44,    44,    44,   231,  -100,   196,    94,
    -100,   211,  -100,   228,   242,    25,   223,    97,   -19,   105,
      92,    -8,   160,    29,  -100,   109,  -100,  -100,   119,    44,
      44,    44,   119,    13,    44,  -100,  -100,   230,  -100,  -100,
    -100,  -100,  -100,   222,  -100,  -100,  -100,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    44,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    44,    44,    44,    44,
      44,    44,    44,  -100,  -100,    44,   272,  -100,  -100,    44,
      85,   233,   234,   235,   275,   237,   238,   241,  -100,  -100,
      44,   242,  -100,    25,   223,   223,    97,   -19,   -19,   105,
     105,   105,   105,    92,    92,    -8,    -8,  -100,  -100,  -100,
    -100,  -100,  -100,  -100,  -100,  -100,  -100,  -100,   215,  -100,
     243,  -100,    85,    85,    85,   236,    44,    44,    44,   244,
    -100,  -100,  -100,  -100,   269,    44,   245,   246,   247,  -100,
     -21,   248,   250,   252,    44,  -100,    85,  -100,  -100,   251,
    -100,    85,  -100
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -100,  -100,   192,    75,   212,   187,   133,   168,     1,    90,
    -100,   145,  -100,   -17,   -32,   214,  -100,   -18,   239,   -56,
     190,  -100,    61,   -99,    95,  -100,   155,   156,    89,   152,
     118,     8,   114,   115,   -92,  -100,  -100
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -23
static const yytype_int16 yytable[] =
{
      83,   137,    53,    94,    55,    56,   135,   136,     6,   138,
     139,   140,   141,   142,   154,   155,    42,    65,    78,    34,
      23,    24,    25,    81,    11,    88,    19,    20,    12,    82,
     181,   182,   183,    13,   185,   187,   186,   156,   157,    14,
     133,    42,    42,    42,    42,    42,    84,    85,    15,    89,
      90,    91,   162,   163,   164,   192,   192,   192,   192,   192,
     192,   192,   192,   192,   192,   192,   192,   192,   192,   192,
     207,   208,   209,   173,   174,    18,   218,    49,   149,   150,
     220,    16,    19,    20,     1,     2,     3,     4,    98,    99,
      49,   229,   100,    63,   175,    77,   176,     6,    52,    89,
      90,    91,   101,   102,   103,    43,   177,   104,   105,    79,
     180,    66,    67,    68,   184,    92,    80,    93,    94,    95,
      96,    97,     6,    52,   221,   152,   153,   236,   237,   238,
      43,    43,    43,    43,    43,    82,   241,    87,    98,    99,
     158,   159,   100,   125,    82,   249,   126,    36,    40,    46,
     160,   161,   101,   102,   103,   178,   179,   104,   105,   127,
      44,    48,   199,   200,   201,   202,   232,   233,   234,    36,
      36,    36,   129,    40,    40,    40,    40,    40,   130,    46,
      46,    46,    37,    41,    47,    44,    44,    44,    44,    44,
     250,    48,    48,    48,   131,   252,   132,   165,   166,   167,
     168,   169,   170,   171,    37,    37,    37,   172,    41,    41,
      41,    41,    41,   -12,    47,    47,    47,   134,    18,    57,
      58,    59,    60,    61,    62,    19,    20,    21,    22,    23,
      24,    25,   -22,    74,    75,    76,   143,    18,   194,   195,
       7,     8,     9,    10,    19,    20,    21,   144,    23,    24,
      25,    17,    69,    70,    71,    72,    73,   146,    51,   147,
     210,   211,   212,   213,   214,   215,   216,   217,   189,   190,
     197,   198,   203,   204,   148,   205,   206,   219,   151,   188,
     225,   230,   222,   223,   224,   226,   227,   228,   235,   231,
     239,   240,    86,   244,   242,   243,   247,   246,   248,   145,
     251,   245,   191,   196,   193
};

static const yytype_uint8 yycheck[] =
{
      56,   100,    20,    24,    21,    22,    98,    99,     3,   101,
     102,   103,   104,   105,    33,    34,    15,    34,    50,     5,
      16,    17,    18,    55,     0,    81,    12,    13,    30,    50,
     129,   130,   131,    46,    21,   134,    23,    56,    57,    46,
      96,    40,    41,    42,    43,    44,    64,    65,    46,     5,
       6,     7,    60,    61,    62,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    44,    45,     5,   175,    48,    53,    54,
     179,    46,    12,    13,     8,     9,    10,    11,    44,    45,
      48,   190,    48,    19,    65,    49,    67,     3,     4,     5,
       6,     7,    58,    59,    60,    15,   123,    63,    64,    46,
     128,    36,    37,    38,   132,    21,     3,    23,    24,    25,
      26,    27,     3,     4,   180,    28,    29,   226,   227,   228,
      40,    41,    42,    43,    44,    50,   235,    46,    44,    45,
      35,    36,    48,     5,    50,   244,    46,    14,    15,    16,
      58,    59,    58,    59,    60,    46,    47,    63,    64,    20,
      15,    16,   154,   155,   156,   157,   222,   223,   224,    36,
      37,    38,    48,    40,    41,    42,    43,    44,    48,    46,
      47,    48,    14,    15,    16,    40,    41,    42,    43,    44,
     246,    46,    47,    48,    48,   251,    48,    37,    38,    39,
      40,    41,    42,    43,    36,    37,    38,    47,    40,    41,
      42,    43,    44,     0,    46,    47,    48,    48,     5,    27,
      28,    29,    30,    31,    32,    12,    13,    14,    15,    16,
      17,    18,     0,    46,    47,    48,     5,     5,   149,   150,
       1,     2,     3,     4,    12,    13,    14,    51,    16,    17,
      18,    12,    40,    41,    42,    43,    44,    46,    19,    31,
     165,   166,   167,   168,   169,   170,   171,   172,    46,    47,
     152,   153,   158,   159,    32,   160,   161,     5,    55,    49,
       5,    66,    49,    49,    49,    48,    48,    46,    52,    46,
      46,    22,    78,    46,    49,    49,    46,    49,    46,   109,
      49,   240,   147,   151,   148
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     8,     9,    10,    11,    69,     3,    86,    86,    86,
      86,     0,    30,    46,    46,    46,    46,    86,     5,    12,
      13,    14,    15,    16,    17,    18,    70,    74,    75,    76,
      77,    78,    79,    82,     5,    71,    74,    75,    80,    72,
      74,    75,    76,    77,    79,    73,    74,    75,    79,    48,
      81,    86,     4,    85,    86,    81,    81,    70,    70,    70,
      70,    70,    70,    19,    83,    81,    71,    71,    71,    72,
      72,    72,    72,    72,    73,    73,    73,    49,    82,    46,
       3,    82,    50,    87,    85,    85,    83,    46,    87,     5,
       6,     7,    21,    23,    24,    25,    26,    27,    44,    45,
      48,    58,    59,    60,    63,    64,    85,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,     5,    46,    20,    84,    48,
      48,    48,    48,    87,    48,   102,   102,    91,   102,   102,
     102,   102,   102,     5,    51,    88,    46,    31,    32,    53,
      54,    55,    28,    29,    33,    34,    56,    57,    35,    36,
      58,    59,    60,    61,    62,    37,    38,    39,    40,    41,
      42,    43,    47,    44,    45,    65,    67,    81,    46,    47,
      85,    91,    91,    91,    85,    21,    23,    91,    49,    46,
      47,    94,   102,    95,    96,    96,    97,    98,    98,    99,
      99,    99,    99,   100,   100,   101,   101,   102,   102,   102,
      92,    92,    92,    92,    92,    92,    92,    92,    91,     5,
      91,    87,    49,    49,    49,     5,    48,    48,    46,    91,
      66,    46,    87,    87,    87,    52,    91,    91,    91,    46,
      22,    91,    49,    49,    46,    90,    49,    46,    46,    91,
      87,    49,    87
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
#line 13 "grammar.y"
	{ type_free((yyvaluep->type_t *)); };

/* Line 1000 of yacc.c  */
#line 1335 "grammar.tab.c"
	break;
      case 4: /* "TOK_PRIMITIVE" */

/* Line 1000 of yacc.c  */
#line 13 "grammar.y"
	{ type_free((yyvaluep->type_t *)); };

/* Line 1000 of yacc.c  */
#line 1344 "grammar.tab.c"
	break;
      case 5: /* "TOK_IDENT" */

/* Line 1000 of yacc.c  */
#line 14 "grammar.y"
	{ free((yyvaluep->const char *)); };

/* Line 1000 of yacc.c  */
#line 1353 "grammar.tab.c"
	break;
      case 6: /* "TOK_STRING" */

/* Line 1000 of yacc.c  */
#line 14 "grammar.y"
	{ free((yyvaluep->const char *)); };

/* Line 1000 of yacc.c  */
#line 1362 "grammar.tab.c"
	break;
      case 7: /* "TOK_NUMBER" */

/* Line 1000 of yacc.c  */
#line 14 "grammar.y"
	{ free((yyvaluep->const char *)); };

/* Line 1000 of yacc.c  */
#line 1371 "grammar.tab.c"
	break;
      case 85: /* "type" */

/* Line 1000 of yacc.c  */
#line 13 "grammar.y"
	{ type_free((yyvaluep->type_t *)); };

/* Line 1000 of yacc.c  */
#line 1380 "grammar.tab.c"
	break;
      case 86: /* "qualified_name" */

/* Line 1000 of yacc.c  */
#line 13 "grammar.y"
	{ type_free((yyvaluep->type_t *)); };

/* Line 1000 of yacc.c  */
#line 1389 "grammar.tab.c"
	break;
      case 87: /* "block" */

/* Line 1000 of yacc.c  */
#line 12 "grammar.y"
	{ statement_free((yyvaluep->statement_t *)); };

/* Line 1000 of yacc.c  */
#line 1398 "grammar.tab.c"
	break;
      case 88: /* "statement_list" */

/* Line 1000 of yacc.c  */
#line 12 "grammar.y"
	{ statement_free((yyvaluep->statement_t *)); };

/* Line 1000 of yacc.c  */
#line 1407 "grammar.tab.c"
	break;
      case 89: /* "statement" */

/* Line 1000 of yacc.c  */
#line 12 "grammar.y"
	{ statement_free((yyvaluep->statement_t *)); };

/* Line 1000 of yacc.c  */
#line 1416 "grammar.tab.c"
	break;
      case 90: /* "conditional" */

/* Line 1000 of yacc.c  */
#line 12 "grammar.y"
	{ statement_free((yyvaluep->statement_t *)); };

/* Line 1000 of yacc.c  */
#line 1425 "grammar.tab.c"
	break;
      case 91: /* "expression" */

/* Line 1000 of yacc.c  */
#line 11 "grammar.y"
	{ expr_free((yyvaluep->expr_t *)); };

/* Line 1000 of yacc.c  */
#line 1434 "grammar.tab.c"
	break;
      case 92: /* "assignment" */

/* Line 1000 of yacc.c  */
#line 11 "grammar.y"
	{ expr_free((yyvaluep->expr_t *)); };

/* Line 1000 of yacc.c  */
#line 1443 "grammar.tab.c"
	break;
      case 93: /* "logical_or" */

/* Line 1000 of yacc.c  */
#line 11 "grammar.y"
	{ expr_free((yyvaluep->expr_t *)); };

/* Line 1000 of yacc.c  */
#line 1452 "grammar.tab.c"
	break;
      case 94: /* "logical_and" */

/* Line 1000 of yacc.c  */
#line 11 "grammar.y"
	{ expr_free((yyvaluep->expr_t *)); };

/* Line 1000 of yacc.c  */
#line 1461 "grammar.tab.c"
	break;
      case 95: /* "bitwise_or" */

/* Line 1000 of yacc.c  */
#line 11 "grammar.y"
	{ expr_free((yyvaluep->expr_t *)); };

/* Line 1000 of yacc.c  */
#line 1470 "grammar.tab.c"
	break;
      case 96: /* "bitwise_and" */

/* Line 1000 of yacc.c  */
#line 11 "grammar.y"
	{ expr_free((yyvaluep->expr_t *)); };

/* Line 1000 of yacc.c  */
#line 1479 "grammar.tab.c"
	break;
      case 97: /* "equality" */

/* Line 1000 of yacc.c  */
#line 11 "grammar.y"
	{ expr_free((yyvaluep->expr_t *)); };

/* Line 1000 of yacc.c  */
#line 1488 "grammar.tab.c"
	break;
      case 98: /* "relation" */

/* Line 1000 of yacc.c  */
#line 11 "grammar.y"
	{ expr_free((yyvaluep->expr_t *)); };

/* Line 1000 of yacc.c  */
#line 1497 "grammar.tab.c"
	break;
      case 99: /* "shift" */

/* Line 1000 of yacc.c  */
#line 11 "grammar.y"
	{ expr_free((yyvaluep->expr_t *)); };

/* Line 1000 of yacc.c  */
#line 1506 "grammar.tab.c"
	break;
      case 100: /* "addition" */

/* Line 1000 of yacc.c  */
#line 11 "grammar.y"
	{ expr_free((yyvaluep->expr_t *)); };

/* Line 1000 of yacc.c  */
#line 1515 "grammar.tab.c"
	break;
      case 101: /* "multiplication" */

/* Line 1000 of yacc.c  */
#line 11 "grammar.y"
	{ expr_free((yyvaluep->expr_t *)); };

/* Line 1000 of yacc.c  */
#line 1524 "grammar.tab.c"
	break;
      case 102: /* "unary" */

/* Line 1000 of yacc.c  */
#line 11 "grammar.y"
	{ expr_free((yyvaluep->expr_t *)); };

/* Line 1000 of yacc.c  */
#line 1533 "grammar.tab.c"
	break;
      case 103: /* "postfix" */

/* Line 1000 of yacc.c  */
#line 11 "grammar.y"
	{ expr_free((yyvaluep->expr_t *)); };

/* Line 1000 of yacc.c  */
#line 1542 "grammar.tab.c"
	break;
      case 104: /* "primary" */

/* Line 1000 of yacc.c  */
#line 11 "grammar.y"
	{ expr_free((yyvaluep->expr_t *)); };

/* Line 1000 of yacc.c  */
#line 1551 "grammar.tab.c"
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
#line 103 "grammar.y"
    {
		(yyval.void) = 0;
        parser_class(parser, (yyvsp[(2) - (4)].type_t *));
    ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 107 "grammar.y"
    {
		(yyval.void) = 0;
        parser_interface(parser, (yyvsp[(2) - (4)].type_t *));
    ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 111 "grammar.y"
    {
		(yyval.void) = 0;
        parser_struct(parser, (yyvsp[(2) - (4)].type_t *));
    ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 115 "grammar.y"
    {
		(yyval.void) = 0;
        parser_module(parser, (yyvsp[(2) - (4)].type_t *));
    ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 155 "grammar.y"
    {
        parser_import(parser, (yyvsp[(2) - (3)].type_t *));
		(yyval.void) = 0;
    ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 162 "grammar.y"
    {
        parser_def(parser, (yyvsp[(2) - (4)].type_t *), (yyvsp[(3) - (4)].type_t *));
		(yyval.void) = 0;
    ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 169 "grammar.y"
    {
        parser_varinit(parser, (yyvsp[(1) - (7)].void), (yyvsp[(2) - (7)].void), (yyvsp[(3) - (7)].type_t *), (yyvsp[(4) - (7)].const char *), (yyvsp[(6) - (7)].expr_t *));
		(yyval.void) = 0;
    ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 173 "grammar.y"
    {
        parser_vardecl(parser, (yyvsp[(1) - (5)].void), (yyvsp[(2) - (5)].void), (yyvsp[(3) - (5)].type_t *), (yyvsp[(4) - (5)].const char *));
		(yyval.void) = 0;
    ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 180 "grammar.y"
    {
        parser_ctor(parser, (yyvsp[(2) - (4)].void), (yyvsp[(3) - (4)].void), (yyvsp[(4) - (4)].statement_t *));
		(yyval.void) = 0;
    ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 187 "grammar.y"
    {
        parser_dtor(parser, (yyvsp[(2) - (3)].void), (yyvsp[(3) - (3)].statement_t *));
		(yyval.void) = 0;
    ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 194 "grammar.y"
    {
        parser_func(parser, (yyvsp[(1) - (7)].const char *), (yyvsp[(2) - (7)].void), (yyvsp[(3) - (7)].void), (yyvsp[(4) - (7)].void), (yyvsp[(5) - (7)].void), (yyvsp[(6) - (7)].type_t *), (yyvsp[(7) - (7)].statement_t *));
		(yyval.void) = 0;
    ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 201 "grammar.y"
    {
        parser_proto(parser, (yyvsp[(1) - (4)].const char *), (yyvsp[(2) - (4)].void), (yyvsp[(3) - (4)].type_t *));
		(yyval.void) = 0;
    ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 208 "grammar.y"
    { (yyval.void) = 0; ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 212 "grammar.y"
    { (yyval.void) = expr_string("public"); ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 213 "grammar.y"
    { (yyval.void) = expr_string("private"); ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 214 "grammar.y"
    { (yyval.void) = expr_string("protected"); ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 215 "grammar.y"
    { (yyval.void) = 0; ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 219 "grammar.y"
    { (yyval.void) = expr_string("static"); ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 220 "grammar.y"
    { (yyval.void) = 0; ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 224 "grammar.y"
    { (yyval.void) = expr_string("native"); ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 225 "grammar.y"
    { (yyval.void) = 0; ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 229 "grammar.y"
    { (yyval.type_t *) = (yyvsp[(1) - (1)].type_t *); ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 230 "grammar.y"
    { (yyval.type_t *) = (yyvsp[(1) - (1)].type_t *); ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 234 "grammar.y"
    { (yyval.type_t *) = type_concat((yyvsp[(1) - (3)].type_t *), (yyvsp[(3) - (3)].type_t *)); ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 235 "grammar.y"
    { (yyval.type_t *) = (yyvsp[(1) - (1)].type_t *); ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 239 "grammar.y"
    { (yyval.statement_t *) = (yyvsp[(2) - (3)].statement_t *); ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 243 "grammar.y"
    { (yyval.statement_t *) = expr_prepend((yyvsp[(2) - (2)].statement_t *), (yyvsp[(1) - (2)].statement_t *)); ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 244 "grammar.y"
    { (yyval.statement_t *) = expr_list(); ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 248 "grammar.y"
    {
		(yyval.statement_t *) = statement_for((yyvsp[(3) - (9)].expr_t *), (yyvsp[(5) - (9)].expr_t *), (yyvsp[(7) - (9)].expr_t *), (yyvsp[(9) - (9)].statement_t *));
	;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 251 "grammar.y"
    {
		(yyval.statement_t *) = statement_foreach((yyvsp[(3) - (8)].type_t *), (yyvsp[(4) - (8)].const char *), (yyvsp[(6) - (8)].expr_t *), (yyvsp[(8) - (8)].statement_t *));
	;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 254 "grammar.y"
    {
		(yyval.statement_t *) = statement_until((yyvsp[(3) - (5)].expr_t *), (yyvsp[(5) - (5)].statement_t *));
	;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 257 "grammar.y"
    {
		(yyval.statement_t *) = statement_while((yyvsp[(3) - (5)].expr_t *), (yyvsp[(5) - (5)].statement_t *));
	;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 260 "grammar.y"
    {
		(yyval.statement_t *) = statement_dountil((yyvsp[(5) - (7)].expr_t *), (yyvsp[(2) - (7)].statement_t *));
	;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 263 "grammar.y"
    {
		(yyval.statement_t *) = statement_dowhile((yyvsp[(5) - (7)].expr_t *), (yyvsp[(2) - (7)].statement_t *));
	;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 266 "grammar.y"
    { 
		(yyval.statement_t *) = expr_binary(op_assign, expr_pair((yyvsp[(1) - (5)].type_t *), (yyvsp[(2) - (5)].const char *)), (yyvsp[(4) - (5)].expr_t *)); 
	;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 269 "grammar.y"
    { (yyval.statement_t *) = expr_pair((yyvsp[(1) - (3)].type_t *), (yyvsp[(2) - (3)].const char *)); ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 270 "grammar.y"
    { (yyval.statement_t *) = (yyvsp[(1) - (1)].statement_t *); ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 271 "grammar.y"
    { (yyval.statement_t *) = (yyvsp[(1) - (2)].expr_t *); ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 275 "grammar.y"
    {
		(yyval.statement_t *) = statement_conditional((yyvsp[(3) - (5)].expr_t *), (yyvsp[(5) - (5)].statement_t *), 0);
	;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 278 "grammar.y"
    {
		(yyval.statement_t *) = statement_conditional((yyvsp[(3) - (7)].expr_t *), (yyvsp[(5) - (7)].statement_t *), (yyvsp[(7) - (7)].statement_t *));
	;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 281 "grammar.y"
    { (yyval.statement_t *) = (yyvsp[(1) - (1)].statement_t *); ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 284 "grammar.y"
    { (yyval.expr_t *) = (yyvsp[(1) - (1)].expr_t *); ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 287 "grammar.y"
    { (yyval.expr_t *) = expr_binary(op_assign, (yyvsp[(1) - (3)].expr_t *), (yyvsp[(3) - (3)].expr_t *)); ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 288 "grammar.y"
    { 
		(yyval.expr_t *) = expr_binary(op_mul_assign, (yyvsp[(1) - (3)].expr_t *), (yyvsp[(3) - (3)].expr_t *)); 
	;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 291 "grammar.y"
    { 
		(yyval.expr_t *) = expr_binary(op_div_assign, (yyvsp[(1) - (3)].expr_t *), (yyvsp[(3) - (3)].expr_t *)); 
	;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 294 "grammar.y"
    { 
		(yyval.expr_t *) = expr_binary(op_mod_assign, (yyvsp[(1) - (3)].expr_t *), (yyvsp[(3) - (3)].expr_t *)); 
	;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 297 "grammar.y"
    { 
		(yyval.expr_t *) = expr_binary(op_sub_assign, (yyvsp[(1) - (3)].expr_t *), (yyvsp[(3) - (3)].expr_t *)); 
	;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 300 "grammar.y"
    { 
		(yyval.expr_t *) = expr_binary(op_add_assign, (yyvsp[(1) - (3)].expr_t *), (yyvsp[(3) - (3)].expr_t *)); 
	;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 303 "grammar.y"
    { 
		(yyval.expr_t *) = expr_binary(op_bitand_assign, (yyvsp[(1) - (3)].expr_t *), (yyvsp[(3) - (3)].expr_t *)); 
	;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 306 "grammar.y"
    { 
		(yyval.expr_t *) = expr_binary(op_bitor_assign, (yyvsp[(1) - (3)].expr_t *), (yyvsp[(3) - (3)].expr_t *)); 
	;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 309 "grammar.y"
    { (yyval.expr_t *) = (yyvsp[(1) - (1)].expr_t *); ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 313 "grammar.y"
    { (yyval.expr_t *) = expr_binary(op_or, (yyvsp[(1) - (3)].expr_t *), (yyvsp[(3) - (3)].expr_t *)); ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 314 "grammar.y"
    { (yyval.expr_t *) = (yyvsp[(1) - (1)].expr_t *); ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 318 "grammar.y"
    { (yyval.expr_t *) = expr_binary(op_and, (yyvsp[(1) - (3)].expr_t *), (yyvsp[(3) - (3)].expr_t *)); ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 319 "grammar.y"
    { (yyval.expr_t *) = (yyvsp[(1) - (1)].expr_t *); ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 323 "grammar.y"
    { (yyval.expr_t *) = expr_binary(op_bitor, (yyvsp[(1) - (3)].expr_t *), (yyvsp[(3) - (3)].expr_t *)); ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 324 "grammar.y"
    { (yyval.expr_t *) = expr_binary(op_bitxor, (yyvsp[(1) - (3)].expr_t *), (yyvsp[(3) - (3)].expr_t *)); ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 325 "grammar.y"
    { (yyval.expr_t *) = (yyvsp[(1) - (1)].expr_t *); ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 329 "grammar.y"
    { (yyval.expr_t *) = expr_binary(op_bitand, (yyvsp[(1) - (3)].expr_t *), (yyvsp[(3) - (3)].expr_t *)); ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 330 "grammar.y"
    { (yyval.expr_t *) = (yyvsp[(1) - (1)].expr_t *); ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 334 "grammar.y"
    { (yyval.expr_t *) = expr_binary(op_equal, (yyvsp[(1) - (3)].expr_t *), (yyvsp[(3) - (3)].expr_t *)); ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 335 "grammar.y"
    { 
		(yyval.expr_t *) = expr_binary(op_notequal, (yyvsp[(1) - (3)].expr_t *), (yyvsp[(3) - (3)].expr_t *)); 
	;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 338 "grammar.y"
    { (yyval.expr_t *) = (yyvsp[(1) - (1)].expr_t *); ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 342 "grammar.y"
    { (yyval.expr_t *) = expr_binary(op_greater, (yyvsp[(1) - (3)].expr_t *), (yyvsp[(3) - (3)].expr_t *)); ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 343 "grammar.y"
    { (yyval.expr_t *) = expr_binary(op_less, (yyvsp[(1) - (3)].expr_t *), (yyvsp[(3) - (3)].expr_t *)); ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 344 "grammar.y"
    { (yyval.expr_t *) = expr_binary(op_ge, (yyvsp[(1) - (3)].expr_t *), (yyvsp[(3) - (3)].expr_t *)); ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 345 "grammar.y"
    { (yyval.expr_t *) = expr_binary(op_le, (yyvsp[(1) - (3)].expr_t *), (yyvsp[(3) - (3)].expr_t *)); ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 346 "grammar.y"
    { (yyval.expr_t *) = (yyvsp[(1) - (1)].expr_t *); ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 350 "grammar.y"
    { (yyval.expr_t *) = expr_binary(op_lshift, (yyvsp[(1) - (3)].expr_t *), (yyvsp[(3) - (3)].expr_t *)); ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 351 "grammar.y"
    { (yyval.expr_t *) = expr_binary(op_rshift, (yyvsp[(1) - (3)].expr_t *), (yyvsp[(3) - (3)].expr_t *)); ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 352 "grammar.y"
    { (yyval.expr_t *) = (yyvsp[(1) - (1)].expr_t *); ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 356 "grammar.y"
    { (yyval.expr_t *) = expr_binary(op_plus, (yyvsp[(1) - (3)].expr_t *), (yyvsp[(3) - (3)].expr_t *)); ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 357 "grammar.y"
    { (yyval.expr_t *) = expr_binary(op_minus, (yyvsp[(1) - (3)].expr_t *), (yyvsp[(3) - (3)].expr_t *)); ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 358 "grammar.y"
    { (yyval.expr_t *) = (yyvsp[(1) - (1)].expr_t *); ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 362 "grammar.y"
    { (yyval.expr_t *) = expr_binary(op_mul, (yyvsp[(1) - (3)].expr_t *), (yyvsp[(3) - (3)].expr_t *)); ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 363 "grammar.y"
    { (yyval.expr_t *) = expr_binary(op_div, (yyvsp[(1) - (3)].expr_t *), (yyvsp[(3) - (3)].expr_t *)); ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 364 "grammar.y"
    { (yyval.expr_t *) = expr_binary(op_mod, (yyvsp[(1) - (3)].expr_t *), (yyvsp[(3) - (3)].expr_t *)); ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 365 "grammar.y"
    { (yyval.expr_t *) = (yyvsp[(1) - (1)].expr_t *); ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 369 "grammar.y"
    { (yyval.expr_t *) = expr_unary(op_inc, (yyvsp[(2) - (2)].expr_t *)); ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 370 "grammar.y"
    { (yyval.expr_t *) = expr_unary(op_dec, (yyvsp[(2) - (2)].expr_t *)); ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 371 "grammar.y"
    { (yyval.expr_t *) = expr_unary(op_plus, (yyvsp[(2) - (2)].expr_t *)); ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 372 "grammar.y"
    { (yyval.expr_t *) = expr_unary(op_minus, (yyvsp[(2) - (2)].expr_t *)); ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 373 "grammar.y"
    { (yyval.expr_t *) = expr_unary(op_bang, (yyvsp[(2) - (2)].expr_t *)); ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 374 "grammar.y"
    { (yyval.expr_t *) = expr_unary(op_tilde, (yyvsp[(2) - (2)].expr_t *)); ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 375 "grammar.y"
    { (yyval.expr_t *) = expr_unary(op_star, (yyvsp[(2) - (2)].expr_t *)); ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 376 "grammar.y"
    { (yyval.expr_t *) = (yyvsp[(1) - (1)].expr_t *); ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 380 "grammar.y"
    { (yyval.expr_t *) = expr_call((yyvsp[(1) - (2)].expr_t *), (yyvsp[(2) - (2)].void)); ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 381 "grammar.y"
    { (yyval.expr_t *) = expr_index((yyvsp[(1) - (4)].expr_t *), (yyvsp[(3) - (4)].expr_t *)); ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 382 "grammar.y"
    { (yyval.expr_t *) = expr_member((yyvsp[(1) - (3)].expr_t *), (yyvsp[(3) - (3)].const char *)); ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 383 "grammar.y"
    { (yyval.expr_t *) = expr_unary(op_postinc, (yyvsp[(1) - (2)].expr_t *)); ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 384 "grammar.y"
    { (yyval.expr_t *) = expr_unary(op_preinc, (yyvsp[(1) - (2)].expr_t *)); ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 385 "grammar.y"
    { (yyval.expr_t *) = (yyvsp[(1) - (1)].expr_t *); ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 389 "grammar.y"
    { (yyval.expr_t *) = (yyvsp[(1) - (1)].const char *); ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 390 "grammar.y"
    { (yyval.expr_t *) = (yyvsp[(1) - (1)].const char *); ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 391 "grammar.y"
    { (yyval.expr_t *) = (yyvsp[(1) - (1)].const char *); ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 392 "grammar.y"
    { (yyval.expr_t *) = (yyvsp[(2) - (3)].expr_t *); ;}
    break;



/* Line 1455 of yacc.c  */
#line 2598 "grammar.tab.c"
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



