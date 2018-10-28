#include "token-list.h"

struct TYPE {
	int ttype; /* TPINT TPCHAR TPBOOL TPARRAY TPARRAYINT TPARRAYCHAR
	 TPARRAYBOOL TPPROC */
	int arraysize; /* size of array, if TPARRAY */
	struct TYPE *paratp; /* pointer to parameter's type list if ttype is TPPROC */
};

struct LINE {
	int reflinenum;
	struct LINE *nextlinep;
};

struct ID {
	char *name;
	char *procname; /* procedure name within which this name is defined *//* NULL if global name */
	struct TYPE *itp;
	int ispara; /* 1:formal parameter, 0:else(variable) */
	int deflinenum;
	struct LINE *irefp;
	struct ID *nextp;
}*globalidroot, *localidroot; /* Pointers to root of global & local symbol tables */

int mode;

void init_id() {
	globalidroot = NULL;
	localidroot = NULL;
}

int search_id(char *np, char *proc) { //変数の未定義、調査用
	struct ID *p;
	struct LINE *newl;

	for (p = localidroot; p != NULL; p = p->nextp) {
		if (strcmp(np, p->name) == 0 && strcmp(proc, p->procname)) {
			if ((newl = (struct LINE *) malloc(sizeof(struct LINE))) == NULL) {
				error("can not malloc in localidroot'line");
				return ERROR;
			}
			newl->reflinenum = get_linenum();
			newl->nextlinep = p->irefp;
			p->irefp = newl;
			return p->itp->ttype;
		}
	}

	for (p = globalidroot; p != NULL; p = p->nextp) {
		if (strcmp(np, p->name) == 0) {
			if ((newl = (struct LINE *) malloc(sizeof(struct LINE))) == NULL) {
				error("can not malloc in globalidroot'line");
				return ERROR;
			}
			newl->reflinenum = get_linenum();
			newl->nextlinep = p->irefp;
			p->irefp = newl;
			return p->itp->ttype;
		}
	}
	print_idtab();
	mode = 0;
	print_idtab();
	error("Undefined variable");
	return NORMAL;
}

int id_con(char *np, char *proc) { //型の2重定義確認用
	struct ID *p;
	struct TYPE *t, *t1;
	char *cp;

	for (p = globalidroot; p != NULL; p = p->nextp) {
		if (strcmp(np, p->name) == 0) {
			error("Double definition");
			return ERROR;
		}
	}

	if (mode == 1 || mode == 2) {//1:副プログラム内 2:宣言内
		for (p = localidroot; p != NULL; p = p->nextp) {
			if (strcmp(np, p->name) == 0) {
				error("Double definition");
				return ERROR;
			}
		}
	}

	if ((p = (struct ID *) malloc(sizeof(struct ID))) == NULL) {
		error("can not malloc in idroot");
		return ERROR;
	}

	if ((cp = (char *) malloc(strlen(np) + 1)) == NULL) {
		error("can not malloc in idroot");
		return ERROR;
	}
	strcpy(cp, np);
	p->name = cp;

	if ((t = (struct TYPE *) malloc(sizeof(struct TYPE))) == NULL) {
		error("can not malloc in idroot");
		return ERROR;
	}
	if (mode != 0) {
		if ((cp = (char *) malloc(strlen(proc) + 1)) == NULL) {
			error("can not malloc in idroot");
			return ERROR;
		}
		strcpy(cp, proc);
		p->procname = cp;
	}

	p->deflinenum = get_linenum();
	p->irefp = NULL;
	p->itp = t;
	t->ttype = 0;
	t->paratp=NULL;

	if (mode == 1 || mode == 2) {//1:副プログラム内 2:宣言内
		p->nextp = localidroot;
		localidroot = p;
	} else {//グローバル宣言
		p->nextp = globalidroot;
		globalidroot = p;
	}

	if (mode == 2) {//1:副プログラム内 2:宣言内
		for (p = globalidroot; p != NULL; p = p->nextp) {
			if (strcmp(proc, p->name) == 0) {
				for (t = p->itp->paratp; t != NULL; t = t->paratp) {
				}
				if ((t1 = (struct TYPE *) malloc(sizeof(struct TYPE))) == NULL) {
					error("can not malloc in idroot");
					return ERROR;
				}
				t = t1;
				t1->ttype = 0;
				break;
			}
		}
		return 0;
	}
	return 0;
}

void type_id(int type) {
	struct ID *p;
	struct TYPE *t;

	switch (mode) {
	case 2:
		for (p = globalidroot; p != NULL; p = p->nextp) {
			for (t = p->itp->paratp; t != NULL; t = t->paratp) {
				if (t->ttype == 0) {
					t->ttype = type;
					if ((TPARRAYCHAR <= type) && (type <= TPARRAYBOOL))
						t->arraysize = num_attr;
				}
			}
		}
	case 1:
		for (p = localidroot; p != NULL; p = p->nextp) {
			if (p->itp->ttype == 0) {
				p->itp->ttype = type;
				if ((TPARRAYCHAR <= type) && (type <= TPARRAYBOOL))
					p->itp->ttype = num_attr;
			}

		}
		break;
	default:
		for (p = globalidroot; p != NULL; p = p->nextp) {
			if (p->itp->ttype == 0) {
				p->itp->ttype = type;
				if ((TPARRAYCHAR <= type) && (type <= TPARRAYBOOL))
					p->itp->arraysize = num_attr;
			}
		}
	}

}

void print_idtab(void) {
	struct ID *p,*pi;
	struct TYPE *stype;
	struct LINE *sline;
	char type[] = "";

	printf("NAME\t    FORM\t ||DEFINE | REFERENCES");
	if (mode == 1)
		p = localidroot;
	else
		p = globalidroot;

	pi = p;
	do{
		switch (pi->itp->ttype) {
		case TPINT:
		case TPCHAR:
		case TPBOOL:
			sprintf(type, "%s", tokenstr[pi->itp->ttype]);
			break;
		case TPARRAYINT:
		case TPARRAYCHAR:
		case TPARRAYBOOL:
			sprintf(type, "%s[%d] of %s", tokenstr[TPARRAY], p->itp->arraysize,tokenstr[(p->itp->ttype) - 10]);
			break;
		case TPPROC:
			sprintf(type, "%s (", tokenstr[TPPROC]);
			if ((stype = p->itp->paratp) != NULL) {
				strcat(type, tokenstr[stype->ttype]);
				stype = stype->paratp;
			}
			for (; stype != NULL; stype = stype->paratp) {
				strcat(type, ", ");
				strcat(type, tokenstr[stype->ttype]);
			}
			strcat(type, ")");
			break;
		default:;
		}

		printf("\n%-10s %-10s || %4d  |", pi->name, type, pi->deflinenum);
		if ((sline = pi->irefp) != NULL) {
			printf(" %d", sline->reflinenum);
			sline = sline->nextlinep;
		}
		for (; sline != NULL; sline = sline->nextlinep) {
			printf(", %d", sline->reflinenum);
		}
		pi = pi->nextp;
	}while(pi != NULL);
}

void release_idtab(void) {
	struct ID *p, *q;

	if (mode == 1)p = localidroot;
	else p = globalidroot;

	for (; p != NULL; p = q) {
		free(p->name);
		if (p->procname == NULL)
			free(p->procname);
		q = p->nextp;
		free(p);
	}
}
