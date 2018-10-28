#include "token-list.h"

/* keyword list */
struct KEY key[KEYWORDSIZE] = {
		{"and", 	TAND	},
		{"array",	TARRAY	},
		{"begin",	TBEGIN	},
		{"boolean",	TBOOLEAN},
		{"break",	TBREAK  },
		{"call",	TCALL	},
		{"char",	TCHAR	},
		{"div",		TDIV	},
		{"do",		TDO	},
		{"else",	TELSE	},
		{"end",		TEND	},
		{"false",	TFALSE	},
		{"if",		TIF	},
		{"integer",	TINTEGER},
		{"not",		TNOT	},
		{"of",		TOF	},
		{"or",		TOR	},
		{"procedure", TPROCEDURE},
		{"program",	TPROGRAM},
		{"read",	TREAD	},
		{"readln",	TREADLN },
		{"return", 	TRETURN },
		{"then",	TTHEN	},
		{"true",	TTRUE	},
		{"var",		TVAR	},
		{"while",	TWHILE	},
		{"write",	TWRITE  },
		{"writeln",	TWRITELN}
};

/* string of each token */
char *tokenstr[NUMOFTOKEN + 1] = { "", "NAME", "program", "var", "array", "of",
		"begin", "end", "if", "then", "else", "procedure", "return", "call",
		"while", "do", "not", "or", "div", "and", "char", "integer", "boolean",
		"readln", "writeln", "true", "false", "NUMBER", "STRING", "+", "-",
		"*", "=", "<>", "<", "<=", ">", ">=", "(", ")", "[", "]", ":=", ".",
		",", ":", ";", "read", "write","break"  };

int num_attr;
char string_attr[MAXSTRSIZE];

int main(int nc, char *np[]) {
	if (nc < 2) {
		printf("Don\'t select the file.\n");
		return -1;
	}

	if (init_scan(np[1]) < 0) {
		printf("file : %s don\'t open \n", np[1]);
		return -1;
	}
	token = scan();
	parse_program();

	end_scan();
	return 0;
}

void error(char *mes) {
	printf("\n[line:%d] ERROR: %s\n",get_linenum(),mes);
	end_scan();
	fflush(stdout);
	exit(-1);
}

