
#include "token-list.h"
int token;

static int block();
static int variable_declaration();
static int variable_row();
static int variable_name();
static int type();
static int standard_type();
static int array_type();
static int subprogran_declaration();
static int procedure();
static int parameter();
static int compound_statement();
static int sentence();
static int branching_statement();
static int assign_statement();
static int call_statement();
static int line_sentence();
static int substitution();
static int variable();
static int expression();
static int simple_formula();
static int term();
static int factor();
static int input_statement();
static int output_statement();
static int output_specification();


/* string of each token */
char string_attr[MAXSTRSIZE];
int num_attr;
static char sub_str[MAXSTRSIZE];
static char procname[MAXSTRSIZE];
static char sub_proc[MAXSTRSIZE];

int parse_program() {
	mode = 0;
	int i;

	for (i = 0; i < MAXSTRSIZE; i++) {
		sub_proc[i] = '\0';
	}
	for (i = 0; i < MAXSTRSIZE; i++) {
		sub_str[i] = '\0';
	}

	if (token != TPROGRAM) {
		error("Keyword 'program' is not found");
		return (ERROR);
	}
	token = scan();
	if (token != TNAME) {
		error("Program name is not found");
		return (ERROR);
	}
	token = scan();
	if (token != TSEMI) {
		error("Semicolon is not found");
		return (ERROR);
	}
	token = scan();

	if (block() == ERROR) {
		return (ERROR);
	}
	if (token != TDOT) {
		error("Period is not found at the end of program");
		return (ERROR);
	}
	print_idtab();
	return (NORMAL);
}
int block() {
	while (token == TVAR || token == TPROCEDURE) {
		if (token == TVAR) {
			if (variable_declaration() == ERROR) {
				return (ERROR);
			}
		} else if (token == TPROCEDURE) {
			if (subprogran_declaration() == ERROR) {
				return (ERROR);
			}
			print_idtab();
		}
	}
	if (compound_statement() == ERROR) {
		return (ERROR);
	}
	return (NORMAL);
}
int variable_declaration() {
	if (token != TVAR) {
		error("Keyword 'var' is not found");
		return (ERROR);
	}
	token = scan();

	if (variable_row() == ERROR) {
		return (ERROR);
	}
	if (token != TCOLON) {
		error("Colon is not found");
		return (ERROR);
	}
	token = scan();

	if (type() == ERROR) {
		return (ERROR);
	}
	if (token != TSEMI) {
		error("Semicolon is not found");
		return (ERROR);
	}
	token = scan();

	while (token == TNAME) {
		if (variable_row() == ERROR) {
			return (ERROR);
		}
		if (token != TCOLON) {
			error("Colon is not found");
			return (ERROR);
		}
		token = scan();

		if (type() == ERROR) {
			return (ERROR);
		}
		if (token != TSEMI) {
			error("Semicolon is not found");
			return (ERROR);
		}
		token = scan();

	}
	while (token == TNAME)
		;

	return (NORMAL);
}
int variable_row() {
	if (variable_name() == ERROR) {
		return (ERROR);
	}
	id_con(sub_str, sub_proc);
	while (token == TCOMMA) {
		token = scan();
		if (variable_name() == ERROR) {
			return (ERROR);
		}
		id_con(sub_str, sub_proc);
	}
	return (NORMAL);
}
int variable_name() {
	if (token != TNAME) {
		error("Name is not found");
		return (ERROR);
	}
	strcpy(sub_str, string_attr);
	token = scan();
	return (NORMAL);
}
int type() {
	int type;
	if (token == TARRAY) {
		if (array_type() == ERROR) {
			return (ERROR);
		}
	} else {
		if ((type = standard_type()) == ERROR) {
			return (ERROR);
		}
		type_id(type);
	}
	return (NORMAL);
}
int standard_type() {
	int type;

	if ((TCHAR <= token) && (token <= TBOOLEAN)) {
		type = token;
		token = scan();
		return (type);
	}
	error("Syntax of standardtype");
	return (ERROR);
}
int array_type() {
	int type;

	if (token != TARRAY) {
		error("Keyword 'array' is not found");
		return (ERROR);
	}
	token = scan();

	if (token != TLSQPAREN) {
		error("Left square bracket is not found");
		return (ERROR);
	}
	token = scan();

	if (token != TNUMBER) {
		error("Number is not found");
		return (ERROR);
	}
	token = scan();

	if (token != TRSQPAREN) {
		error("Right square bracket is not found");
		return (ERROR);
	}
	token = scan();

	if (token != TOF) {
		error("Keyword 'of' is not found");
		return (ERROR);
	}
	token = scan();

	if ((type = standard_type()) == ERROR) {
		return (ERROR);
	}
	type_id(type);
	return (NORMAL);
}
int subprogran_declaration() {
	int i;

	if (token != TPROCEDURE) {
		error("Keyword 'procedure' is not found");
		return (ERROR);
	}
	token = scan();

	if (procedure() == ERROR) {
		return (ERROR);
	}
	strcpy(sub_proc, procname);
	id_con(sub_proc, '\0');
	type_id(TPPROC);

	if (token == TLPAREN) {
		if (parameter() == ERROR) {
			return (ERROR);
		}
	}
	if (token != TSEMI) {
		error("Semicolon is not found");
		return (ERROR);
	}
	token = scan();

	if (token == TVAR) {
		mode = 1;
		if (variable_declaration() == ERROR) {
			return (ERROR);
		}
	}
	if (compound_statement() == ERROR) {
		return (ERROR);
	}
	if (token != TSEMI) {
		error("Semicolon is not found");
		return (ERROR);
	}
	token = scan();

	for (i = 0; i < MAXSTRSIZE; i++) {
		sub_proc[i] = '\0';
	}

	mode = 0;
	return (NORMAL);
}
int procedure() {
	if (token != TNAME) {
		error("Name is not found");
		return (ERROR);
	}
	strcpy(procname, string_attr);
	fflush(stdout);
	token = scan();
	return (NORMAL);
}
int parameter() {
	mode = 2;
	if (token != TLPAREN) {
		error("Left parenthesis is not found");
		return (ERROR);
	}
	token = scan();

	if (variable_row() == ERROR) {
		return (ERROR);
	}

	if (token != TCOLON) {
		error("Colon is not found");
		return (ERROR);
	}
	token = scan();

	if (type() == ERROR) {
		return (ERROR);
	}

	while (token == TSEMI) {
		token = scan();

		if (variable_row() == ERROR) {
			return (ERROR);
		}
		if (token != TCOLON) {
			error("Colon is not found");
			return (ERROR);
		}
		token = scan();

		if (type() == ERROR) {
			return (ERROR);
		}
	}
	if (token != TRPAREN) {
		error("Right parenthesis is not found");
		return (ERROR);
	}
	token = scan();
	mode = 1;

	return (NORMAL);
}
int compound_statement() {

	if (token != TBEGIN) {
		error("Right parenthesis is not found");
		return (ERROR);
	}
	token = scan();

	if (sentence() == ERROR) {
		return (ERROR);
	}

	while ((token == TSEMI)) {
		token = scan();
		if (token == TEND) {
			break;
		}
		if (token != TBEGIN) {
		}
		if (sentence() == ERROR) {
			return (ERROR);
		}
	}

	if (token != TEND) {
		error("Keyword 'end' is not found");
		return (ERROR);
	}
	token = scan();
	return (NORMAL);
}
int sentence() {
	switch (token) {
	case TNAME:
		if (substitution() == ERROR) {
			return (ERROR);
		}
		break;
	case TIF:
		if (branching_statement() == ERROR) {
			return (ERROR);
		}
		break;
	case TWHILE:
		if (assign_statement() == ERROR) {
			return (ERROR);
		}
		break;
	case TCALL:
		if (call_statement() == ERROR) {
			return (ERROR);
		}
		break;
	case TRETURN:
		if (token != TRETURN) {
			error("Keyword 'end' is not found");
			return (ERROR);
		}
		token = scan();
		break;
	case TREAD:
	case TREADLN:
		if (input_statement() == ERROR) {
			return (ERROR);
		}
		break;
	case TWRITE:
	case TWRITELN:
		if (output_statement() == ERROR) {
			return (ERROR);
		}
		break;
	case TBEGIN:
		if (compound_statement() == ERROR) {
			return (ERROR);
		}
		break;
	}
	return (NORMAL);
}
int branching_statement() {
	int type;

	if (token != TIF) {
		error("Keyword 'if' is not found");
		return (ERROR);
	}
	token = scan();

	if ((type = expression()) == ERROR) {
		return (ERROR);
	}
//	if (type != TPBOOL) {
//		error("Type mismatch (Not boolean)");
//		return (ERROR);
//	}
	if (token != TTHEN) {
		error("Keyword 'then' is not found");
		return (ERROR);
	}
	token = scan();
	if (token == TBEGIN) {
	}
	if (sentence() == ERROR) {
		return (ERROR);
	}
	if (token == TELSE) {
		token = scan();

		if (token == TBEGIN) {
		}
		if (sentence() == ERROR) {
			return (ERROR);
		}
	}
	return (NORMAL);
}
int assign_statement() {
	int type;

	if (token != TWHILE) {
		error("Keyword 'while' is not found");
		return (ERROR);
	}
	token = scan();

	if ((type = expression()) == ERROR) {
		return (ERROR);
	}

	if (token != TDO) {
		error("Type mismatch (Not boolean)");
		return (ERROR);
	}
	token = scan();

	if (token == TBEGIN) {
	}

	if (sentence() == ERROR) {
		return (ERROR);
	}
	return (NORMAL);
}
int call_statement() {

	if (token != TCALL) {
		error("Keyword 'call' is not found");
		return (ERROR);
	}
	token = scan();

	if (procedure() == ERROR) {
		return (ERROR);
	}
	if (token == TLPAREN) {
		token = scan();
		if (line_sentence() == ERROR) {
			return (ERROR);
		}
		if (token != TRPAREN) {
			error("Right parenthesis is not found");
			return (ERROR);
		}
		token = scan();
	}
	return (NORMAL);
}
int line_sentence() {
	if (expression() == ERROR) {
		return (ERROR);
	}
	while (token == TCOMMA) {
		token = scan();
		if (expression() == ERROR) {
			return (ERROR);
		}
	}
	return (NORMAL);
}
int substitution() {
	int type, type2;

	if ((type = variable()) == ERROR) {
		return (ERROR);
	}
	if (token != TASSIGN) {
		error("Assignment is not found");
		return (ERROR);
	}
	token = scan();
	if ((type2 = expression()) == ERROR) {
		return (ERROR);
	}

	return (NORMAL);
}
int variable() {
	int type, type2;

	if (variable_name() == ERROR) {
		return (ERROR);
	}
	type = search_id(sub_str, sub_proc);

	if (token == TLSQPAREN) {

		type = type - 10;
		token = scan();
		if ((type2 = expression()) == ERROR) {
			return (ERROR);
		}

		if (token != TRSQPAREN) {
			error("Right square bracket is not found");
			return (ERROR);
		}
		token = scan();
	}

	return (type);
}
int expression() {
	int type = 0, type2 = 0;
	if ((type = simple_formula()) == ERROR) {
		return (ERROR);
	}
	while ((TEQUAL <= token) && (token <= TGREQ)) {
		token = scan();

		if ((type2 = simple_formula()) == ERROR) {
			return (ERROR);
		}
	}
	if (type == type2) {
		return (TPBOOL);
	}
	return (type);
}
int simple_formula(){
	int type, type2 = 0;
	int flag = 0;

	if ((token == TPLUS) || (token == TMINUS)) {
		token = scan();
		flag = 1;
	}
	if ((type = term()) == ERROR) {
		return (ERROR);
	}
	flag = 0;

	while ((token == TPLUS) || (token == TMINUS) || (token == TOR)) {
		if ((token == TPLUS) || (token == TMINUS))
			flag = 1;
		token = scan();
		if ((type2 = term()) == ERROR) {
			return (ERROR);
		}
		flag = 0;
	}
	return (type);
}
int term() {
	int type, type2;
	int flag;
	if ((type = factor()) == ERROR) {
		return (ERROR);
	}

	while ((token == TSTAR) || (token == TDIV) || (token == TAND)) {
		if ((token == TSTAR) || (token == TDIV))
			flag = 1;
		token = scan();
		if ((type2 = factor()) == ERROR) {
			return (ERROR);
		}
		flag = 0;
	}
	return (type);
}
int factor() {
	int type,type2;
	switch (token) {
	case TNAME:
		if ((type=variable()) == ERROR) {
			return (ERROR);
		}
		break;
	case TNUMBER:
	case TFALSE:
	case TTRUE:
	case TSTRING:
		if ((token == TTRUE) || (token == TFALSE)) {
			type=TPBOOL;
		} else if (token == TNUMBER) {
			type=TPINT;
		} else if (token == TSTRING) {
			type=TPCHAR;
		}
		token = scan();
		break;
	case TNOT:
		token = scan();
		if ((type=factor()) == ERROR) {
			return (ERROR);
		}
		break;
	case TINTEGER:
	case TBOOLEAN:
	case TCHAR:
		if ((type=standard_type()) == ERROR) {
			return (ERROR);
		}
		if (token != TLPAREN) {
			error("Left parenthesis is not found");
			return (ERROR);
		}
		token = scan();
		if ((type2=expression()) == ERROR) {
			return (ERROR);
		}
		if((TPCHAR > type2) || (type2 > TPBOOL)){
			error("Type mismatch (Not standard)");
			return(ERROR);
		}
		if (token != TRPAREN) {
			error("Right parenthesis is not found");
			return (ERROR);
		}
		token = scan();
		break;
	case TLPAREN:
		token = scan();
		if (expression() == ERROR) {
			return (ERROR);
		}
		if (token != TRPAREN) {
			error("Right parenthesis is not found");
			return (ERROR);
		}
		token = scan();
		break;
	default:
		error("Syntax of factor");
		return (ERROR);
	}
	return (NORMAL);
}
int input_statement() {
	if (token != TREAD && token != TREADLN) {
		error("Syntax of input");
		return (ERROR);
	}
	token = scan();

	if (token == TLPAREN) {
		do {
			token = scan();
			if (variable() == ERROR) {
				return (ERROR);
			}
		} while (token == TCOMMA);
		if (token != TRPAREN) {
			error("Right parenthesis is not found4");
			return (ERROR);
		}
		token = scan();
	}
	return (NORMAL);
}
int output_statement() {
	if (token != TWRITE && token != TWRITELN) {
		error("Syntax of output");
		return (ERROR);
	}
	token = scan();

	if (token == TLPAREN) {
		do {
			token = scan();
			if (output_specification() == ERROR) {
				return (ERROR);
			}
		} while (token == TCOMMA);
		if (token != TRPAREN) {
			error("Right parenthesis is not found5");
			return (ERROR);
		}
		token = scan();
	}
	return (NORMAL);
}
int output_specification() {
	if (token == TSTRING && strlen(string_attr) != 1) {
		if ((strlen(string_attr) != 2) && (strstr(string_attr, "''") == NULL)) {
			token = scan();
			return (NORMAL);
		}
	}
	if (expression() != ERROR) {
		if (token == TCOLON) {
			token = scan();
			if (token != TNUMBER) {
				error("Number is not found");
				return (ERROR);
			}
			token = scan();
		}
		return (NORMAL);
	}
	return (ERROR);
}
