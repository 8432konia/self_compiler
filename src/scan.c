#include "token-list.h"

void set_linenum(void);
int get_linenum(void);
int which_keyword(void);
void cscan(void);
char ctable[257] = { SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL,
		SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL,
		SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL,
		SNULL, SNULL, SNULL, SNULL, SNULL, SLNOT, SDQUOTE, SNULL, SNULL, SMOD,
		SBAND, SSQUOTE, SLPAREN, SRPAREN, SSTAR, SPLUS, SCOMMA, SMINUS, SDOT,
		SDIV, SDIGIT, SDIGIT, SDIGIT, SDIGIT, SDIGIT, SDIGIT, SDIGIT, SDIGIT,
		SDIGIT, SDIGIT, SCOLON, SSEMI, SLESS, SASSIGN, SGREAT, SQUEST, SNULL,
		SALPHA, SALPHA, SALPHA, SALPHA, SALPHA, SALPHA, SALPHA, SALPHA, SALPHA,
		SALPHA, SALPHA, SALPHA, SALPHA, SALPHA, SALPHA, SALPHA, SALPHA, SALPHA,
		SALPHA, SALPHA, SALPHA, SALPHA, SALPHA, SALPHA, SALPHA, SALPHA, SLSQP,
		SNULL, SRSQP, SBEOR, SALPHA, SNULL, SALPHA, SALPHA, SALPHA, SALPHA,
		SALPHA, SALPHA, SALPHA, SALPHA, SALPHA, SALPHA, SALPHA, SALPHA, SALPHA,
		SALPHA, SALPHA, SALPHA, SALPHA, SALPHA, SALPHA, SALPHA, SALPHA, SALPHA,
		SALPHA, SALPHA, SALPHA, SALPHA, SLBRACE, SBOR, SRBRACE, SBNOT, SNULL,
		SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL,
		SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL,
		SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL,
		SNULL, SNULL, SKANA, SKANA, SKANA, SKANA, SKANA, SKANA, SKANA, SKANA,
		SKANA, SKANA, SKANA, SKANA, SKANA, SKANA, SKANA, SKANA, SKANA, SKANA,
		SKANA, SKANA, SKANA, SKANA, SKANA, SKANA, SKANA, SKANA, SKANA, SKANA,
		SKANA, SKANA, SKANA, SKANA, SKANA, SKANA, SKANA, SKANA, SKANA, SKANA,
		SKANA, SKANA, SKANA, SKANA, SKANA, SKANA, SKANA, SKANA, SKANA, SKANA,
		SKANA, SKANA, SKANA, SKANA, SKANA, SKANA, SKANA, SKANA, SKANA, SKANA,
		SKANA, SKANA, SKANA, SKANA, SKANA, SKANA, SNULL, SNULL, SNULL, SNULL,
		SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL,
		SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL,
		SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SNULL, SEOF };

int cbuf = '\0';
int cnt,strln;
unsigned long linecnt=0;
FILE *fp;

int init_scan(char *filename) {
	memset(string_attr, '\0', MAXSTRSIZE);
	if ((fp = fopen(filename, "r")) == NULL) {
		return -1; /*error*/
	}
	cbuf = getc(fp);
	return 1;
}
void end_scan() {
	fclose(fp);
}
int scan() {
	int token;
	char *p;
	int cnt = 0, i,substrln;
	if (linecnt <= 0) {
		linecnt = 1;
	}
	START: while ((token = ctable[cbuf]) == SNULL) {
		cscan();
	}
	switch (token) {
	case SALPHA:/*alphabet*/
		p = string_attr;
		strln=0;
		do {
			strln++;
			if(strln>MAXSTRSIZE){
				error("This name is larger.");
			}
			*(p++) = cbuf;
			cscan();
		} while (ctable[cbuf] == SALPHA || ctable[cbuf] == SDIGIT);

		*(p++) = '\0';
		p = NULL;
		token = which_keyword();
		break;
	case SDIGIT:/*number*/
		num_attr=0;
		strln=0;
		p=string_attr;
		while ('0' <= cbuf && cbuf <= '9') {
			strln++;
			if(strln>MAXSTRSIZE){
				error("This number is larger.");
			}
			*(p++)=cbuf;
			num_attr = (num_attr * 10) + (cbuf - '0');
			cscan();
		}
		*(p++)='\0';
		p=NULL;
		if(num_attr<0||num_attr>32767){
			error("The number is larger than 32767");
			return -1;
		}
		token = TNUMBER;
		break;
	case SSQUOTE:/*'*/
		strln=0;
		substrln=strln;
		cscan();
		p = string_attr;
		while (1) {
			if (ctable[cbuf] == SEOF||cbuf=='\n') {
				*(p++)='\0';
				error("non-regulated string.(EOF or newline befor  \')");
				break;
			}
			if (ctable[cbuf] == SSQUOTE) {
				for (cnt = 0; ctable[cbuf] == SSQUOTE; cnt++) {
					cscan();
				}
				for (i = cnt / 2; i > 0; i--) {
					strln+=2;
					substrln++;
					if(strln>MAXSTRSIZE){
						*(p++)='\0';
						error("This string is larger than regulation size.");
						break;
					}
					*(p++) = '\'';
					*(p++) = '\'';
				}
				if (cnt % 2) {
					break;
				}
			}else{
				strln++;
				substrln++;
				if(strln>MAXSTRSIZE){
					error("This string is larger than regulation size.");
				}
					*(p++) = cbuf;
					cscan();
			}
		}
		*(p++)='\0';
		p = NULL;
		strln=substrln;
		token = TSTRING;
		break;
	case SPLUS:/*+*/
		token = TPLUS;
		cscan();
		break;
	case SMINUS:/*-*/
		token = TMINUS;
		cscan();
		break;
	case SSTAR:/***/
		token = TSTAR;
		cscan();
		break;
	case SASSIGN:/*=*/
		token = TEQUAL;
		cscan();
		break;
	case SLESS:/*< <= <>*/
		cscan();
		if (ctable[cbuf] == SGREAT) {
			token = TNOTEQ;
			cscan();
		} else if (ctable[cbuf] == SASSIGN) {
			token = TLEEQ;
			cscan();
		} else
			token = TLE;
		break;
	case SGREAT:/*> >=*/
		cscan();
		if (ctable[cbuf] == SASSIGN) {
			token = TGREQ;
			cscan();
		} else
			token = TGR;
		break;
	case SLPAREN:/*(*/
		cscan();
		token = TLPAREN;
		break;
	case SRPAREN:/*)*/
		cscan();
		token = TRPAREN;
		break;
	case SLSQP:/*[*/
		token = TLSQPAREN;
		cscan();
		break;
	case SRSQP:/*]*/
		token = TRSQPAREN;
		cscan();
		break;
	case SCOLON:/*: :=*/
		cscan();
		if (ctable[cbuf] == SASSIGN) {
			token = TASSIGN;
			cscan();
		} else {
			token = TCOLON;
		}
		break;
	case SSEMI:/*;*/
		cscan();
		token = TSEMI;
		break;
	case SDOT:/*.*/
		cscan();
		token = TDOT;
		break;
	case SCOMMA:/*,*/
		cscan();
		token = TCOMMA;
		break;
	case SDIV:/*/**/
		cscan();
		if (ctable[cbuf] == SSTAR) {
			cscan();
			do {
				while (ctable[cbuf] != SSTAR && ctable[cbuf] != SEOF) {
					cscan();
				}
				if (ctable[cbuf] == SEOF) {
					error("\'/*\' are not found");
					return -1;
				}
				cscan();
			} while (ctable[cbuf] != SDIV);
			cscan();
			goto START;
		} else {
			error("undefined Symbol.");
			return -1;
		}

		break;
	case SLBRACE:/*{}*/
		cscan();
		while (ctable[cbuf] != SRBRACE && ctable[cbuf] != SEOF) {
			cscan();
		}
		if (ctable[cbuf] == SEOF) {
			error("\'{\' are not found");
			return -1;
		}
		cscan();
		goto START;
		break;
	case SEOF:/*EOF*/
		return -1;
	default:
		error("undefined Symbol.");
		return -1;
		break;
	}
	return token;
}

void cscan() {
	static int flag = 0;
	cbuf = getc(fp);
	if (cbuf < 0 || 255 < cbuf)
		cbuf = 256;
	if (flag) {
		set_linenum();
		flag = 0;
	}
	if (cbuf == '\n') {
		flag = 1;
	}
}

int which_keyword() {
	int l, r, c, i;
	l = 0;
	r = KEYWORDSIZE - 1;
	while (l <= r) {
		c = (l + r) >> 1;
		i = strcmp(string_attr, key[c].keyword);
		if (i < 0)
			r = c - 1;
		else if (i > 0)
			l = c + 1;
		else
			return (key[c].keytoken);
	}
	return (TNAME);
}

void set_linenum() {
	linecnt++;
}
int get_linenum() {
	return linecnt;
}
