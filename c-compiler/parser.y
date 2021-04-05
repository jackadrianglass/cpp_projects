%{
    #include <stdio.h>
    #include "scan.h"
    #include "parse.h"
    #include "util.h"
    static int yylex();
    int yyerror(char* s);

    static TreeNode* savedTree = NULL;
    static ExpType savedType;
    static TokenType savedTokenType;

    static const char* savedName;
    static TreeNode* getSiblingNode(TreeNode* a, TreeNode* b);
    static int getNumSiblings(TreeNode* firstSibling);

    static void debug(const char * msg);
    static int debug_enabled = 0;
%}
%define api.value.type {TreeNode*}
%define parse.error verbose
%token BOOL INT VOID
%token IF ELSE WHILE RETURN
%token TRUE FALSE NOT
%token ASSIGN EQ NQ LT GT LE GE
%token AND OR PLUS MINUS TIMES OVER
%token LPAREN RPAREN SEMI COMMA LBRACK RBRACK LCBRACK RCBRACK
%token NUM ID
%token ERROR

%%
program: declaration_list {
    debug("in program");
    savedTree = $1;
    $$ = savedTree;
};

declaration_list: declaration declaration_list {
    debug("in declaration list 1");
    $$ = $1;
    $$->sibling = $2;
}
| declaration {
    debug("in declaration list 2");
    $$ = $1;
};

declaration: var_declaration            {$$ = $1;}
                | fun_declaration       {$$ = $1;}
;

var_declaration: type_specifier ID SEMI {
    debug("in var decl 1");
    $$ = newDecNode(VarK);
    $$->type = savedType;
    $$->attr.name = copyString(tokenString);
    debug(tokenString);
}
| type_specifier ID LBRACK NUM RBRACK SEMI {
    debug("in var decl 1");
    $$ = newDecNode(ArrayK);
    $$->type = savedType;
    $$->attr.name = copyString(previousTokenString);
    $$->arraySize = atoi(tokenString);
};

type_specifier: INT     {savedType = Integer;}
                | VOID  {savedType = Void;}
                | BOOL  {savedType = Boolean;}
;

fun_declaration: type_specifier ID {
    debug("in funk decl 1");
    $$ = newDecNode(FunK);
    $$->attr.name = copyString(tokenString);
    $$->type = savedType;
} LPAREN params RPAREN compound_stmt {
    debug("in funk decl 2");
    $$ = $3;
    $$->child[0] = $5;
    $$->param_size = getNumSiblings($$->child[0]);
    $$->child[1] = $7;

    savedName = NULL;
};

params: param_list {
    debug("in params 1");
    $$ = $1;
} | VOID {
    debug("in params 2");
    $$ = NULL;
};

param_list: param_list COMMA param   {
    debug("in param list 1");
    $$ = getSiblingNode($1, $3);
} | param {
    debug("in param list 2");
    $$ = $1;
};

param: type_specifier ID {
    debug("in param 1");
    $$ = newDecNode(VarK);
    $$->attr.name = copyString(tokenString);
    $$->type = savedType;
    $$->isParameter = true;
} | type_specifier ID LBRACK RBRACK {
    debug("in param 1");
    $$ = newDecNode(ArrayK);
    $$->attr.name = copyString(tokenString);
    $$->type = savedType;
    $$->isParameter = true;
    $$->isArray = true;
};

compound_stmt: LCBRACK local_declarations statement_list RCBRACK {
    debug("in compound stmt");
    $$ = newStmtNode(CompoundK);
    $$->child[0] = $2;
    $$->child[1] = $3;
};

local_declarations: local_declarations var_declaration {
    debug("in local decl 1");
    $$ = getSiblingNode($1, $2);
} | %empty {
    debug("in local decl 2");
    $$ = NULL;
};

statement_list: statement_list statement {
    debug("in stmt list 1");
    $$ = getSiblingNode($1, $2);
} | %empty {
    debug("in stmt list 2");
    $$ = NULL;
};

statement: expression_stmt      {$$ = $1;}
            | compound_stmt     {$$ = $1;}
            | selection_stmt    {$$ = $1;}
            | iteration_stmt    {$$ = $1;}
            | return_stmt       {$$ = $1;}
;

expression_stmt: expression SEMI {
    $$ = $1;
} | SEMI {
    $$ = NULL;
};

selection_stmt: IF LPAREN expression RPAREN statement {
    $$ = newStmtNode(IfK);
    $$->lineno = $3->lineno;
    $$->child[0] = $3;
    $$->child[1] = $5;
} | IF LPAREN expression RPAREN statement ELSE statement {
    $$ = newStmtNode(IfK);
    $$->lineno = $3->lineno;
    $$->child[0] = $3;
    $$->child[1] = $5;
    $$->child[2] = $7;
};

iteration_stmt: WHILE LPAREN expression RPAREN statement {
    $$ = newStmtNode(WhileK);
    $$->lineno = $3->lineno;
    $$->child[0] = $3;
    $$->child[1] = $5;
};

return_stmt: RETURN SEMI {
    $$ = newStmtNode(ReturnK);
} | RETURN expression SEMI {
    $$ = newStmtNode(ReturnK);
    $$->lineno = $2->lineno;
    $$->child[0] = $2;
};

expression: var ASSIGN expression {
    debug("in expr 1");
    $$ = newExpNode(AssignK);
    $$->child[0] = $1;
    $$->child[1] = $3;
    $$->attr.op = ASSIGN;
} | simple_expression {
    debug("in expr 2");
    $$ = $1;
};

var: ID {
    debug("in var 1");
    $$ = newExpNode(IdK);
    $$->attr.name = copyString(tokenString);
} | ID {
    debug("in var 2");
    $$ = newExpNode(IdK);
    $$->attr.name = copyString(tokenString);
    $$->isArray = 1;
} LBRACK expression RBRACK {
    debug("in var 3");
    $$ = $2;
    $$->child[0] = $4;
};

simple_expression: simple_expression relop {
    debug("in simple expr 1");
    $$ = newExpNode(OpK);
    $$->attr.op = savedTokenType;
} additive_expression {
    debug("in simple expr 2");
    $$ = $3;
    $$->child[0] = $1;
    $$->child[1] = $4;
}
| additive_expression {
    debug("in simple expr 3");
    $$ = $1;
};

relop: LE       {savedTokenType = LE;}
        | LT    {savedTokenType = LT;}
        | GT    {savedTokenType = GT;}
        | GE    {savedTokenType = GE;}
        | EQ    {savedTokenType = EQ;}
        | NQ    {savedTokenType = NQ;}
;

additive_expression: additive_expression addop {
    debug("in add expr 1");
    $$ = newExpNode(OpK);
    $$->attr.op = savedTokenType;
} term {
    debug("in add expr 2");
    $$ = $3;
    $$->child[0] = $1;
    $$->child[1] = $4;
} | term {
    debug("in add expr 3");
    $$ = $1;
};

addop: PLUS     {savedTokenType = PLUS;}
        | MINUS {savedTokenType = MINUS;}
;

term: term mulop {
    debug("in term 1");
    $$ = newExpNode(OpK);
    $$->attr.op = savedTokenType;
} factor {
    debug("in term 2");
    $$ = $3;
    $$->child[0] = $1;
    $$->child[1] = $4;
} | factor {
    debug("in term 3");
    $$ = $1;
};

mulop: TIMES    {savedTokenType = TIMES;}
        | OVER  {savedTokenType = OVER;}
;

factor: LPAREN expression RPAREN    { $$ = $2; }
        | var                       { $$ = $1; }
        | call                      { $$ = $1; }
        | NUM                       {
            $$ = newExpNode(ConstK);
            $$->attr.val = atoi(tokenString);
        }
;

call: ID {
    debug("in call 1");
    $$ = newStmtNode(CallK);
    $$->attr.name = copyString(tokenString);
} LPAREN args RPAREN {
    debug("in call 2");
    $$ = $2;
    $$->child[0] = $4;
    $$->param_size = getNumSiblings($$->child[0]);
};

args: arg_list  {
    $$ = $1;
}
| %empty {
    $$ = NULL;
};

arg_list: arg_list COMMA expression {
    $$ = getSiblingNode($1, $3);
}
| expression {
    $$ = $1;
    $$->isParameter = true;
};


%%
void debug(const char* msg) {
    if(debug_enabled) {
        printf("%s", msg);
        printf("%s", "\n");
    }
}
TreeNode* getSiblingNode(TreeNode* a, TreeNode* b) {
    if(a != NULL) {
        TreeNode* tmp = a;
        while(tmp->sibling != NULL){
            tmp = tmp->sibling;
        }
        tmp->sibling = b;
        return a;
    } else {
        return b;
    }
}

static int getNumSiblings(TreeNode* firstSibling) {
    if(firstSibling == NULL){
        return 0;
    }
    int count = 0;
    TreeNode* tmp = firstSibling;
    do{
        count++;
        tmp = tmp->sibling;
    } while(tmp != NULL);
    return count;
}
int yyerror(char* msg) {
    fprintf(listing, "Syntax error at line %d: %s\n", lineno, msg);
    fprintf(listing, "Current token: ");
    printToken(yychar, currTokenString, lineno);
    fprintf(listing, "\n");
    Error = TRUE;
    return 0;
}

static int yylex() {
    return getToken();
}

TreeNode* parse() {
    yyparse();
    return savedTree;
}
