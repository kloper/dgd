#ifndef lint
static char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define yyclearin (yychar=(-1))
#define yyerrok (yyerrflag=0)
#define YYRECOVERING (yyerrflag!=0)
#define YYPREFIX "yy"
#line 1 "./src/dgd_format_parser.y"
 /* -*- C -*- */

/* 
 * $Id$
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * This file is part of Depression Glass library.
 *
 * Copyright (c) 2003. Dimitry Kloper <dimka@cs.technion.ac.il> . 
 *
 * dgd_format_parser.y -- implementation of format line parser
 *
 */
   
#include <stdio.h>

#include "dgd_format_lexer.h"
#include "dgd_compile_cache.h"
   
char* dgd_format_string = NULL;
cache_t *cache = NULL;

#define YYDEBUG 1

static int yylex();
static void yylexreset();
static void yyerror( char* reason );

#line 44 "./src/dgd_format_parser.y"
typedef union {
   lexer_value_t  lex;
   cache_item_t*  ring;
} YYSTYPE;
#line 59 "./src/dgd_format_parser.c"
#define LEX_EOF 257
#define LEX_ZERO 258
#define LEX_MINUS 259
#define LEX_SPACE 260
#define LEX_PLUS 261
#define LEX_PERIOD 262
#define LEX_NUMBER 263
#define LEX_REFERENCE 264
#define LEX_PERCENT 265
#define LEX_LEFTBRACE 266
#define LEX_WORD 267
#define LEX_COLON 268
#define LEX_EQUAL 269
#define LEX_COMMA 270
#define LEX_STAR 271
#define LEX_DOLLAR 272
#define LEX_DIEZ 273
#define LEX_BACKSLASH 274
#define LEX_LEFT_BRACE 275
#define LEX_RIGHT_BRACE 276
#define LEX_MOD_HALFHALF 277
#define LEX_MOD_HALF 278
#define LEX_MOD_LONGLONG 279
#define LEX_MOD_LONG 280
#define LEX_MOD_INTMAXT 281
#define LEX_MOD_SIZET 282
#define LEX_MOD_PTRDIFFT 283
#define LEX_MOD_LONGDOUBLE 284
#define LEX_T_DEC 285
#define LEX_T_OCT 286
#define LEX_T_UNSIGNED 287
#define LEX_T_HEX_LOW 288
#define LEX_T_HEX_HIGH 289
#define LEX_T_SCI_LOW 290
#define LEX_T_SCI_HIGH 291
#define LEX_T_FLOAT_LOW 292
#define LEX_T_FLOAT_HIGH 293
#define LEX_T_SCIORFLOAT_LOW 294
#define LEX_T_SCIORFLOAT_HIGH 295
#define LEX_T_SCIHEX_LOW 296
#define LEX_T_SCIHEX_HIGH 297
#define LEX_T_CHAR 298
#define LEX_T_STRING 299
#define LEX_T_PTR 300
#define LEX_T_REPORT 301
#define YYERRCODE 256
short yylhs[] = {                                        -1,
    1,    1,    2,    2,    3,    3,    3,    3,    4,    4,
    5,    6,    6,    6,    6,    6,    6,    7,    7,    7,
    7,    7,    7,    7,    7,    8,    8,    9,   10,   11,
   11,   11,   11,   11,   11,   11,   11,   12,   12,   13,
   14,   15,   15,   15,   15,   15,   16,   16,   16,   16,
   16,   17,   17,   17,   18,   18,   18,   18,   19,   19,
   19,   19,   20,    0,    0,    0,    0,
};
short yylen[] = {                                         2,
    1,    2,    1,    3,    1,    1,    3,    3,    1,    1,
    3,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    3,    1,
    1,    1,    1,    1,    1,    1,    0,    1,    0,    2,
    2,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    2,    1,    2,    2,    3,    2,    2,
    2,    0,    3,    2,    2,    2,    0,
};
short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    2,   65,   66,   47,
   49,   50,   51,    0,   52,    0,   48,    0,    0,    0,
    0,   64,   56,    0,   54,   59,   60,   61,    0,   30,
   31,   32,   33,   34,   35,   36,   38,   26,   27,   28,
   44,   45,   46,    0,    0,   42,   43,   63,   58,    0,
    0,   12,   13,   14,   15,   16,   17,   40,   18,   19,
   20,   21,   22,   23,   24,   25,   41,    0,   29,    9,
   10,    4,    0,    0,    0,    0,    0,   11,    7,    8,
};
short yydgoto[] = {                                       4,
    5,   51,   72,   73,   74,   58,   67,   41,   42,   43,
   44,   45,   46,   47,   48,   18,   19,   20,   21,    6,
};
short yysindex[] = {                                   -256,
 -254, -256, -256,    0,   -7, -256,    0,    0,    0,    0,
    0,    0,    0, -257,    0, -248,    0,   -7,   -7,   -7,
  -72,    0,    0, -206,    0,    0,    0,    0, -260,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  -39, -153,    0,    0,    0,    0, -208,
 -179,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -259,    0,    0,
    0,    0, -217, -172, -259, -259, -259,    0,    0,    0,
};
short yyrindex[] = {                                     62,
 -258,   62,   62,    0,  -99,   62,    0,    0,    0,    0,
    0,    0,    0, -126,    0, -214,    0,  -99,  -99,  -99,
  -56,    0,    0, -170,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -177,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, -176, -174,    0,    0,    0,    0,    0,    0,
};
short yygindex[] = {                                     48,
    0,    0,  -21,   29,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   76,    0,
};
#define YYTABLESIZE 266
short yytable[] = {                                       1,
    1,    1,    1,    1,    1,   23,   50,   70,    1,    7,
    2,   71,    1,   24,    1,   25,    1,    3,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,   53,   53,   53,   53,   53,   53,    8,
    9,   75,   76,   22,   79,   80,   53,   49,   53,   68,
   53,   67,   53,   53,   53,   53,   53,   53,   53,   53,
   53,   53,   53,   53,   53,   53,   53,   53,   53,   53,
   53,   53,   53,   53,   53,   53,   53,   57,   57,   57,
   57,   57,   57,   26,   27,   28,   69,   77,    3,    5,
   57,    6,   57,   78,   57,    0,   57,   57,   57,   57,
   57,   57,   57,   57,   57,   57,   57,   57,   57,   57,
   57,   57,   57,   57,   57,   57,   57,   57,   57,   57,
   57,   55,   55,   55,   55,   55,   59,   60,   61,   62,
   63,   64,   65,   66,    0,    0,   55,    0,   55,    0,
   55,   55,   55,   55,   55,   55,   55,   55,   55,   55,
   55,   55,   55,   55,   55,   55,   55,   55,   55,   55,
   55,   55,   55,   55,   55,   62,    0,   62,   62,   62,
   62,   62,   62,   62,   62,   62,   62,   62,   62,   62,
   62,   62,   62,   62,   62,   62,   62,   62,   62,   62,
   62,   62,   29,    0,   30,   31,   32,   33,   34,   35,
   36,   37,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   38,   39,   40,   37,   37,
   37,   37,   37,   39,   39,   39,   39,   39,   39,   39,
   39,    0,    0,    0,   37,   52,   53,   54,   55,   56,
   10,   11,   12,   13,   14,   15,    0,    0,    0,    0,
    0,   57,    0,   16,    0,   17,
};
short yycheck[] = {                                     258,
  259,  260,  261,  262,  263,  263,  267,  267,  265,  264,
  267,  271,  271,  271,  273,  264,  275,  274,  277,  278,
  279,  280,  281,  282,  283,  284,  285,  286,  287,  288,
  289,  290,  291,  292,  293,  294,  295,  296,  297,  298,
  299,  300,  301,  258,  259,  260,  261,  262,  263,    2,
    3,  269,  270,    6,   76,   77,  271,  264,  273,  268,
  275,    0,  277,  278,  279,  280,  281,  282,  283,  284,
  285,  286,  287,  288,  289,  290,  291,  292,  293,  294,
  295,  296,  297,  298,  299,  300,  301,  258,  259,  260,
  261,  262,  263,   18,   19,   20,  276,  270,  276,  276,
  271,  276,  273,   75,  275,   -1,  277,  278,  279,  280,
  281,  282,  283,  284,  285,  286,  287,  288,  289,  290,
  291,  292,  293,  294,  295,  296,  297,  298,  299,  300,
  301,  258,  259,  260,  261,  262,  290,  291,  292,  293,
  294,  295,  296,  297,   -1,   -1,  273,   -1,  275,   -1,
  277,  278,  279,  280,  281,  282,  283,  284,  285,  286,
  287,  288,  289,  290,  291,  292,  293,  294,  295,  296,
  297,  298,  299,  300,  301,  275,   -1,  277,  278,  279,
  280,  281,  282,  283,  284,  285,  286,  287,  288,  289,
  290,  291,  292,  293,  294,  295,  296,  297,  298,  299,
  300,  301,  275,   -1,  277,  278,  279,  280,  281,  282,
  283,  284,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  298,  299,  300,  285,  286,
  287,  288,  289,  290,  291,  292,  293,  294,  295,  296,
  297,   -1,   -1,   -1,  301,  285,  286,  287,  288,  289,
  258,  259,  260,  261,  262,  263,   -1,   -1,   -1,   -1,
   -1,  301,   -1,  271,   -1,  273,
};
#define YYFINAL 4
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 301
#if YYDEBUG
char *yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"LEX_EOF","LEX_ZERO","LEX_MINUS",
"LEX_SPACE","LEX_PLUS","LEX_PERIOD","LEX_NUMBER","LEX_REFERENCE","LEX_PERCENT",
"LEX_LEFTBRACE","LEX_WORD","LEX_COLON","LEX_EQUAL","LEX_COMMA","LEX_STAR",
"LEX_DOLLAR","LEX_DIEZ","LEX_BACKSLASH","LEX_LEFT_BRACE","LEX_RIGHT_BRACE",
"LEX_MOD_HALFHALF","LEX_MOD_HALF","LEX_MOD_LONGLONG","LEX_MOD_LONG",
"LEX_MOD_INTMAXT","LEX_MOD_SIZET","LEX_MOD_PTRDIFFT","LEX_MOD_LONGDOUBLE",
"LEX_T_DEC","LEX_T_OCT","LEX_T_UNSIGNED","LEX_T_HEX_LOW","LEX_T_HEX_HIGH",
"LEX_T_SCI_LOW","LEX_T_SCI_HIGH","LEX_T_FLOAT_LOW","LEX_T_FLOAT_HIGH",
"LEX_T_SCIORFLOAT_LOW","LEX_T_SCIORFLOAT_HIGH","LEX_T_SCIHEX_LOW",
"LEX_T_SCIHEX_HIGH","LEX_T_CHAR","LEX_T_STRING","LEX_T_PTR","LEX_T_REPORT",
};
char *yyrule[] = {
"$accept : fmt",
"cmdbegin : LEX_PERCENT",
"cmdbegin : LEX_PERCENT LEX_REFERENCE",
"subcmd : LEX_WORD",
"subcmd : LEX_WORD LEX_COLON subcmdparams",
"subcmdparams : identifier",
"subcmdparams : pair",
"subcmdparams : identifier LEX_COMMA subcmdparams",
"subcmdparams : pair LEX_COMMA subcmdparams",
"identifier : LEX_WORD",
"identifier : LEX_STAR",
"pair : identifier LEX_EQUAL identifier",
"intformat : LEX_T_DEC",
"intformat : LEX_T_OCT",
"intformat : LEX_T_UNSIGNED",
"intformat : LEX_T_HEX_LOW",
"intformat : LEX_T_HEX_HIGH",
"intformat : LEX_T_REPORT",
"doubleformat : LEX_T_SCI_LOW",
"doubleformat : LEX_T_SCI_HIGH",
"doubleformat : LEX_T_FLOAT_LOW",
"doubleformat : LEX_T_FLOAT_HIGH",
"doubleformat : LEX_T_SCIORFLOAT_LOW",
"doubleformat : LEX_T_SCIORFLOAT_HIGH",
"doubleformat : LEX_T_SCIHEX_LOW",
"doubleformat : LEX_T_SCIHEX_HIGH",
"stringformat : LEX_T_CHAR",
"stringformat : LEX_T_STRING",
"ptrformat : LEX_T_PTR",
"extformat : LEX_LEFT_BRACE subcmd LEX_RIGHT_BRACE",
"intmodifier : LEX_MOD_HALFHALF",
"intmodifier : LEX_MOD_HALF",
"intmodifier : LEX_MOD_LONGLONG",
"intmodifier : LEX_MOD_LONG",
"intmodifier : LEX_MOD_INTMAXT",
"intmodifier : LEX_MOD_SIZET",
"intmodifier : LEX_MOD_PTRDIFFT",
"intmodifier :",
"doublemodifier : LEX_MOD_LONGDOUBLE",
"doublemodifier :",
"modintformat : intmodifier intformat",
"moddoubleformat : doublemodifier doubleformat",
"format : modintformat",
"format : moddoubleformat",
"format : stringformat",
"format : ptrformat",
"format : extformat",
"mode : LEX_ZERO",
"mode : LEX_DIEZ",
"mode : LEX_MINUS",
"mode : LEX_SPACE",
"mode : LEX_PLUS",
"width : LEX_NUMBER",
"width : LEX_STAR",
"width : LEX_STAR LEX_REFERENCE",
"precision : LEX_PERIOD",
"precision : LEX_PERIOD LEX_NUMBER",
"precision : LEX_PERIOD LEX_STAR",
"precision : LEX_PERIOD LEX_STAR LEX_REFERENCE",
"modes : mode modes",
"modes : width modes",
"modes : precision modes",
"modes :",
"cmd : cmdbegin modes format",
"fmt : cmd fmt",
"fmt : LEX_WORD fmt",
"fmt : LEX_BACKSLASH fmt",
"fmt :",
};
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 500
#define YYMAXDEPTH 500
#endif
#endif
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short yyss[YYSTACKSIZE];
YYSTYPE yyvs[YYSTACKSIZE];
#define yystacksize YYSTACKSIZE
#line 318 "./src/dgd_format_parser.y"

static lexer_state_t   lexer_state;
static int             lexer_state_init = 0;
static default_cache_t parser_cache;

static 
int yylex() {
   if( !lexer_state_init ) {
      init_lexer_state( &lexer_state );
      lexer_state_init = 1;

      dgd_lookup_format( &parser_cache );
      cache = (cache_t*) &parser_cache;
   }

   format_lexer( &lexer_state, dgd_format_string, &(yylval.lex) );

   if( yylval.lex.token == LEX_EOF ) {
      return -1;
   }
   return yylval.lex.token;
}

void yylexreset() {
   format_lexer_set_state( &lexer_state, LEX_STATE_NORMAL );
}

static
void yyerror( char* reason ) {
   fprintf( stderr, "%s\n", reason );
}
#line 370 "./src/dgd_format_parser.c"
#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
int
yyparse(void)
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register char *yys;
    extern char *getenv();

    if (yys = getenv("YYDEBUG"))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if (yyn = yydefred[yystate]) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yyss + yystacksize - 1)
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#ifdef lint
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#ifdef lint
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yyss + yystacksize - 1)
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    yyval = yyvsp[1-yym];
    switch (yyn)
    {
case 1:
#line 104 "./src/dgd_format_parser.y"
{  }
break;
case 2:
#line 106 "./src/dgd_format_parser.y"
{  }
break;
case 3:
#line 111 "./src/dgd_format_parser.y"
{ 
                       cache_item_t *ring = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror();
                       }

                       ring->type            = PARS_T_CALL_BY_NAME;
                       ring->value.call.name = yyvsp[0].value.lexeme;
                       ring->value.call.args = NULL;

                       yyval.ring = ring;                       
                     }
break;
case 4:
#line 125 "./src/dgd_format_parser.y"
{ 
                       cache_item_t *args_ring = yyvsp[0].ring; 
                       cache_item_t *ring      = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror();
                       }

                       ring->type            = PARS_T_CALL_BY_NAME;
                       ring->value.call.name = yyvsp[-2].value.lexeme;
                       ring->valie.call.args = args_ring;

                       yyval.ring = ring;                                  
                     }
break;
case 5:
#line 143 "./src/dgd_format_parser.y"
{ 
                       cache_item_t *key_ring   = yyvsp[0].ring;
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror();
                       }

                       dgd_ring_push_back( &key_ring, &value_ring );
                       ring->value.ring = key_ring;

                       yyval.ring = ring;  
                     }
break;
case 6:
#line 156 "./src/dgd_format_parser.y"
{
                       yyval.ring = yyvsp[0].ring;  
                     }
break;
case 7:
#line 160 "./src/dgd_format_parser.y"
{
                       dgd_ring_push_back( &(yyvsp[-2].ring), yyvsp[0].ring );
                     }
break;
case 8:
#line 165 "./src/dgd_format_parser.y"
{
                       dgd_ring_push_back( &(yyvsp[-2].ring), yyvsp[0].ring );
                     }
break;
case 9:
#line 172 "./src/dgd_format_parser.y"
{ 
                       cache_item_t *ring = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror();
                       }

                       ring->type         = PARS_T_LEXEME;
                       ring->value.lexeme = yyvsp[0].value.lexeme;
                       yyval.ring = ring;
                     }
break;
case 10:
#line 184 "./src/dgd_format_parser.y"
{ 
                       cache_item_t *ring = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror();
                       }

                       ring->type         = PARS_T_NEXT_ARG;
                       yyval.ring = ring;
                     }
break;
case 11:
#line 198 "./src/dgd_format_parser.y"
{
                       cache_item_t *key_ring   = yyvsp[-2].ring;
                       cache_item_t *value_ring = yyvsp[0].ring;
                       cache_item_t *ring       = dgd_cache_alloc( cache, 1 );

                       if( ring == NULL ) {
                         yyerror();
                       }

                       ring->type         = PARS_T_PARAM;
                       dgd_ring_push_back( &key_ring, &value_ring );
                       ring->value.ring = key_ring;

                       yyval.ring = ring;  
                     }
break;
case 63:
#line 306 "./src/dgd_format_parser.y"
{
	     yylexreset();
	  }
break;
#line 637 "./src/dgd_format_parser.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yyss + yystacksize - 1)
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
