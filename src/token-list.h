/* token-list.h  */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define MAXSTRSIZE 1024

/* Token */
#define	TNAME		1	/* 名前：英字で始まる英数字列 */
#define	TPROGRAM	2	/* program：キーワード */
#define	TVAR		3	/* var：キーワード */
#define	TARRAY		4	/* array：キーワード */
#define	TOF			5	/* of：キーワード */
#define	TBEGIN		6	/* begin：キーワード */
#define	TEND		7  	/* end：キーワード */
#define	TIF			8  	/* if：キーワード */
#define	TTHEN		9	/* then：キーワード */
#define	TELSE		10	/* else：キーワード */
#define	TPROCEDURE	11	/* procedure：キーワード */
#define	TRETURN	12	/* return：キーワード */
#define	TCALL		13	/* call：キーワード */
#define	TWHILE		14	/* while：キーワード */
#define	TDO			15 	/* do：キーワード */
#define	TNOT		16	/* not：キーワード */
#define	TOR			17	/* or：キーワード */
#define	TDIV		18 	/* div：キーワード */
#define	TAND		19 	/* and：キーワード */
#define	TCHAR		20	/* char：キーワード */
#define	TINTEGER	21	/* integer：キーワード */
#define	TBOOLEAN	22 	/* boolean：キーワード */
#define	TREADLN	23	/* readln：キーワード */
#define	TWRITELN	24	/* writeln：キーワード */
#define	TTRUE		25	/* true：キーワード */
#define	TFALSE		26	/* false：キーワード */
#define	TNUMBER	27	/* 符号なし整数 */
#define	TSTRING	28	/* 文字列 */
#define	TPLUS		29	/* +：記号 */
#define	TMINUS		30 	/* - ：記号 */
#define	TSTAR		31 	/* *：記号 */
#define	TEQUAL		32 	/* = ：記号 */
#define	TNOTEQ		33 	/* <>：記号 */
#define	TLE			34 	/* <：記号 */
#define	TLEEQ		35 	/* <=：記号 */
#define	TGR			36	/* >：記号 */
#define	TGREQ		37	/* >=：記号 */
#define	TLPAREN	38 	/* (：記号 */
#define	TRPAREN	39 	/* )：記号 */
#define	TLSQPAREN	40	/* [：記号 */
#define	TRSQPAREN	41 	/* ]：記号 */
#define	TASSIGN	42	/* :=：記号 */
#define	TDOT		43 	/* . ：記号 */
#define	TCOMMA		44	/* ,：記号 */
#define	TCOLON		45	/* :：記号 */
#define	TSEMI		46	/* ;：記号 */
#define	TREAD		47	/* read：キーワード */
#define	TWRITE		48	/* write：キーワード */
#define	TBREAK 	49 /* break : キーワード*/

/* ASCIIcode */
#define SNULL      1
#define SLNOT      2
#define SDQUOTE    3
#define SMOD       4
#define SBAND      5
#define SSQUOTE    6
#define SLPAREN    7
#define SRPAREN    8
#define SSTAR      9
#define SPLUS     10
#define SCOMMA    11
#define SMINUS    12
#define SDOT      13
#define SDIV      14
#define SDIGIT    15
#define SCOLON    16
#define SSEMI     17
#define SLESS     18
#define SASSIGN   19
#define SGREAT    20
#define SQUEST    21
#define SALPHA    22
#define SLSQP     23
#define SRSQP     24
#define SBEOR     25
#define SLBRACE   26
#define SBOR      27
#define SRBRACE   28
#define SBNOT     29
#define SEOF      30
#define SIDENT    31
#define SSTR      32
#define SCONST    33
#define SLAND     34
#define SLOR      35
#define SEQUAL    36
#define SDEC      37
#define SINC      38
#define SPOINTTO  39
#define SLSHIFT   40
#define SLESSEQ   41
#define SRSHIFT   42
#define SGREATEQ  43
#define SNOTEQ    44
#define SBREAK    45
#define SCASE     46
#define SCHAR     47
#define SCONTINUE 48
#define SDEFAULT  49
#define SDO       50
#define SELSE     51
#define SEXTERN   52
#define SFOR      53
#define SGOTO     54
#define SIF       55
#define SINT      56
#define SRETURN   57
#define SSIZEOF   58
#define SSTRUCT   59
#define SSWITCH   60
#define SWHILE    61
#define SKANA	    62

#define NUMOFTOKEN	49

/* token-list.c */
#define KEYWORDSIZE	28

extern struct KEY {
	char * keyword;
	int keytoken;
} key[KEYWORDSIZE];

extern char *tokenstr[NUMOFTOKEN + 1];

extern void error(char *mes);

/* scan.c */
extern int init_scan(char *filename);
extern int scan(void);
extern int num_attr;
extern char string_attr[MAXSTRSIZE];
extern int strln;
extern FILE *fp;
extern int get_linenum(void);
extern void end_scan(void);

/*synax.c*/
extern int token;
extern int parse_program();

/*cross_reference.c*/
#define ERROR 0
#define NORMAL 1

#define 	TPINT		21
#define 	TPCHAR		20
#define 	TPBOOL		22
#define 	TPARRAY		4
#define 	TPARRAYINT	31
#define 	TPARRAYCHAR	30
#define 	TPARRAYBOOL	32
#define 	TPPROC		11

extern void init_id(void);
extern int search_id(char *np, char *proc);
extern int id_con(char *np, char *proc);
extern void type_id(int type);
extern void print_idtab(void);
extern void release_idtab(void);
extern int mode;
