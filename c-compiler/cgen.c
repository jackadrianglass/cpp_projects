/****************************************************/
/* File: cgen.c                                     */
/* The code generator implementation                */
/* for the C- compiler                              */
/* (generates code for the TM machine)              */
/*                                                  */
/*                                                  */
/****************************************************/

#include "globals.h"
#include "symtab.h"
#include "code.h"
#include "cgen.h"

/* to store local var of main 
*/
static int main_locals = 0;

/* isRecursive:
 * 1 - cGen will recurse on sibling
 * 0 - cGen won't recurse on sibling
 */
static int isRecursive = 1;

/* prototype for internal recursive code generator */
static void cGen(TreeNode *tree);

static int tmp;

/* stack used for call */
#define stack_size 10
static int param_idx = 0; //< used for bottom up indexing
TreeNode *param_stack[stack_size];
int top = 0;

static bool return_created = false;

/* stack routines*/
int pushParam(TreeNode *param)
{
  if (top == SIZE)
    return 1;

  param_stack[top++] = param;
  return 0;
}

TreeNode *popParam()
{
  if (top == 0)
    return NULL;

  TreeNode* return_val = param_stack[--top];
  param_stack[top] = NULL;
  return return_val;
}

void resetParamStack() {
  top = 0;
  param_idx = 0;
  memset(param_stack, 0, stack_size * sizeof(TreeNode*));
}

/* emit one instruction to get the address of a var,
 * store the address in ac2,
 * we can access the var by ac2[0]
 */
void emitGetAddr(TreeNode *node)
{

  if(node->scope == 0) {
    if (node->isArray) {
      emitRM("LDA", ac2, -(st_lookup(node->attr.name, 0)), gp, "get global array address");
    } else {
      emitRM("LDA", ac2, -1 - (st_lookup(node->attr.name, 0)), gp, "get global address");
    }
  } else {
    if (node->isParameter) {
      if (node->isArray) {
        emitRM("LD", ac2, 2 + (st_lookup(node->attr.name, node->scope)), fp, "get param array address");
      } else {
        emitRM("LDA", ac2, 2 + (st_lookup(node->attr.name, node->scope)), fp, "get param variable address");
      }
    } else {
      if (node->isArray) {
        emitRM("LDA", ac2, -(st_lookup(node->attr.name, node->scope)), fp, "get local array address");
      } else {
        emitRM("LDA", ac2, -1 - (st_lookup(node->attr.name, node->scope)), fp, "get local address");
      }
    }
  }
}

static void genFuncEnd(TreeNode* node) {
  //> Restore function state and return to caller
  emitRO("ADD", sp, fp, zero, "copy fp value into sp"); //< removes all locals

  emitRM("LD", fp, -1, fp, "load control link into fp");
  emitRM("LDC", ac2, node->param_size, zero, "load param size");
  emitRO("ADD", sp, sp, ac2, "move sp to above parameter list");

  emitRM("LD", pc, sp, zero, "load return address into pc (jump)");
}

/* Procedure genDec generates code at an declaration node */
static void genDec(TreeNode *node)
{
  switch(node->kind.dec) {
    case VarK: {
      emitRM("LDC", ac1, 1, zero, "load 1 into ac1");
      emitRO("SUB", sp, sp, ac1, "move sp down to allocate");
      break;
    }
    case ArrayK: {
      emitRM("LDC", ac1, node->arraySize, zero, "load array size into ac1");
      emitRO("SUB", sp, sp, ac1, "move sp down to allocate");
      break;
    }
    case FunK: {
      //> Store the function start point
      BucketList func = fun_lookup(node->attr.name, node->scope);
      func->fun_start = emitSkip(0);
      emitComment("Begin Function");
      emitComment(node->attr.name);

      // No need to call parameter list as they're all allocated already
      cGen(node->child[1]); //< function body
      if(!return_created){
        genFuncEnd(node);     //< implicit return
      } else {
        return_created = false;
      }
      emitComment("End Function");
      break;
    }
  }
} /* genDec */

static void genIfStmt(TreeNode* node) {
  cGen(node->child[0]); //< condition
  //> location of the if() skip to else block
  int saved_location1 = emitSkip(1); 

  cGen(node->child[1]); //< if
  //> location of end of the true block so skip to end
  int saved_location2 = emitSkip(1);

  //> emit skip to else block
  int current_location = emitSkip(0);
  emitBackup(saved_location1);
  emitRM_Abs("JEQ", ac1, current_location, "if: false so jump to else");
  emitRestore();

  cGen(node->child[2]); //< else

  //> emit skip to end block
  current_location = emitSkip(0);
  emitBackup(saved_location2);
  emitRM_Abs("JEQ", ac1, current_location, "if: true so jump to end");
  emitRestore();
}

static void genWhileStmt(TreeNode* node) {
  int condition_location = emitSkip(0);
  cGen(node->child[0]); //< condition

  int jump_location = emitSkip(1); //< jump to end
  cGen(node->child[1]);

  //> jump back to start
  emitRM_Abs("JEQ", zero, condition_location, "jump to begin of while");

  //> back patch jump to end
  int current_location = emitSkip(0);
  emitBackup(jump_location);
  emitRM_Abs("JNE", ac1, current_location, "while: false so jump to end");
  emitRestore();
}

static void genCallStmt(TreeNode* node) {
  emitRM("LDC", ac1, node->param_size + 1, zero, "Load number of parameters");
  emitRO("SUB", sp, sp, ac1, "Move stack pointer down");

  BucketList func = fun_lookup(node->attr.name, node->scope);
  TreeNode* decl = func->declNode->child[0];
  while(decl != NULL) {
    pushParam(decl);
    decl = decl->sibling;
  }

  cGen(node->child[0]);

  resetParamStack();

  emitRM("ST", fp, -1, sp, "store control point");
  emitRO("ADD", fp, sp, zero, "set fp to sp");
  emitRM("LDC", ac1, 2, zero, "load 2 into ac1");
  emitRO("ADD", ac1, ac1, pc, "calculate return address");
  emitRM("ST", ac1, 0, fp, "store return address");
  emitRM("LDC", pc, func->fun_start, zero, "jump to function");
}

/* Procedure genStmt generates code at a statement node */
static void genStmt(TreeNode *node)
{
  switch(node->kind.stmt) {
    case IfK: {
      genIfStmt(node);
      break;
    }
    case WhileK: {
      genWhileStmt(node);
      break;
    }
    case CallK: {
      genCallStmt(node);
      break;
    }
    case ReturnK: {
      cGen(node->child[0]); //< put return val in ac1
      genFuncEnd(node);
      return_created = true;
      break;
    }
    case CompoundK: {
      cGen(node->child[0]);
      cGen(node->child[1]);
      break;
    }
  }
} /* genStmt */

static void genCmpJump(char* inst) {
  emitRO("SUB", ac1, ac2, ac1, "sub for comparison");
  emitRM(inst, ac1, 2, pc, "jump if true");
  emitRM("LDC", ac1, 1, zero, "load false into register");
  emitRM("LDA", pc, 1, pc, "unconditional jump");
  emitRM("LDC", ac1, 0, zero, "load true into register");
}

static void genOpExpr(TreeNode* node) {
  cGen(node->child[0]);
  emitRM("ST", ac1, tmp--, mp, "operator: push left");
  cGen(node->child[1]);
  emitRM("LD", ac2, ++tmp, mp, "operator: load right");
  switch(node->attr.op) {
    case PLUS: {
      emitRO("ADD", ac1, ac2, ac1, "operator +");
      break;
    }
    case MINUS: {
      emitRO("SUB", ac1, ac2, ac1, "operator -");
      break;
    }
    case TIMES: {
      emitRO("MUL", ac1, ac2, ac1, "operator *");
      break;
    }
    case OVER: {
      emitRO("DIV", ac1, ac2, ac1, "operator /");
      break;
    }
    case EQ: {
      emitComment("operator ==");
      genCmpJump("JEQ");
      break;
    }
    case NQ: {
      emitComment("operator !=");
      genCmpJump("JNE");
      break;
    }
    case LT: {
      emitComment("operator <");
      genCmpJump("JLT");
      break;
    }
    case LE: {
      emitComment("operator <=");
      genCmpJump("JLE");
      break;
    }
    case GT: {
      emitComment("operator >");
      genCmpJump("JGT");
      break;
    }
    case GE: {
      emitComment("operator >=");
      genCmpJump("JGE");
      break;
    }
  }
}

/* Procedure genExp generates code at an expression node */
static void genExp(TreeNode *node)
{
  switch(node->kind.exp) {
    case OpK: {
      genOpExpr(node);
      break;
    }
    case ConstK: {
      emitRM("LDC", ac1, node->attr.val, zero, "load constant");
      break;
    }
    case IdK: {
      emitGetAddr(node);
      emitRM("LD", ac1, 0, ac2, "load value at memory address");
      break;
    }
    case AssignK: {
      cGen(node->child[0]);
      //> store the address of the assignee which is located in ac2
      emitRM("ST", ac2, tmp--, mp, "operator: push left");

      cGen(node->child[1]);
      emitRM("LD", ac2, ++tmp, mp, "operator: load right"); //< restore addr

      emitRM("ST", ac1, 0, ac2, "assignment to store value");
      break;
    }
  }

  if(node->isParameter){
    TreeNode* decl = param_stack[param_idx++];
    if(decl == NULL) {
      emitComment("Bug in parameter assignment");
      return;
    }
    emitGetAddr(decl);
    emitRM("ST", ac1, 0, ac2, "store value of expression into parameter location");
  }
} /* genExp */

/* Procedure cGen recursively generates code by
 * tree traversal
 */
static void cGen(TreeNode *tree)
{
  if(tree != NULL) { return; }
  switch (tree->nodekind)
  {
  case StmtK:
    genStmt(tree);
    break;
  case ExpK:
    genExp(tree);
    break;
  case DecK:
    genDec(tree);
    break;
  }
  if(isRecursive) {
    cGen(tree->sibling);
  }
}

/**********************************************/
/* the primary function of the code generator */
/**********************************************/
/* Procedure codeGen generates code to a code
 * file by traversal of the syntax tree. The
 * second parameter (codefile) is the file name
 * of the code file, and is used to print the
 * file name as a comment in the code file
 */
void codeGen(TreeNode *syntaxTree, char *codefile)
{
  char *s = malloc(strlen(codefile) + 7);
  BucketList fun; /*function bucket */

  strcpy(s, "File: ");
  strcat(s, codefile);
  emitComment("C- Compilation to TM Code");
  emitComment(s);
  /* generate standard prelude */
  emitComment("Standard prelude:");
  emitRM("LD", mp, 0, zero, "load maxaddress from location 0");
  emitRM("LDA", fp, syntaxTree->kind.dec != FunK ? -(syntaxTree->param_size) : 0, mp, "copy gp to sp &allocating global variables(if any)");
  emitRM("ST", zero, 0, zero, "clear location 0");
  emitComment("End of standard prelude.");

  /*jump to main */
  if (TraceCode)
    emitComment("Jump to main()");
  int loc = emitSkip(6); /*A call consumes 5 instructions, and we need halt after main()*/

  /*defining Input & output fuction as if they were in-built(global) */
  /* if only necessary  i,e. if they are used in program */
  fun = fun_lookup("input", 0);
  if (fun != NULL)
  {
    if (TraceCode)
      emitComment("Begin input()");
    fun->fun_start = emitSkip(0);
    emitRO("IN", ac2, 0, 0, "read input into ac2");
    emitRM("LDA", fp, 1, fp, "pop prepare");
    emitRM("LD", pc, -1, fp, "pop return addr");
    if (TraceCode)
      emitComment("End input()");
  }

  fun = fun_lookup("output", 0);
  if (fun != NULL)
  {
    if (TraceCode)
      emitComment("Begin output()");
    fun->fun_start = emitSkip(0);
    emitRM("LD", ac2, 1, fp, "load param into ac2");
    emitRO("OUT", ac2, 0, 0, "output using ac2");
    emitRM("LDA", fp, 1, fp, "pop prepare");
    emitRM("LD", pc, -1, fp, "pop return addr");
    if (TraceCode)
      emitComment("End output()");
  }

  /* generate code for TINY program */
  cGen(syntaxTree);
  /* finish */

  /* Fill up jump-to-main code */
  emitBackup(loc);
  fun = fun_lookup("main", 0);
  if (fun == NULL)
  {
    fprintf(stderr, "main not found\n");
  }

  emitRM("LDA", ac2, 3, pc, "store returned PC");
  emitRM("LDA", fp, -1, fp, "push prepare");
  emitRM("ST", ac2, 0, fp, "push returned PC");
  emitRM("LDC", pc, fun->fun_start, 0, "jump to function");
  emitRM("LDA", fp, main_locals, fp, "release local var");

  emitComment("End of execution.");
  emitRO("HALT", 0, 0, 0, "");
}
