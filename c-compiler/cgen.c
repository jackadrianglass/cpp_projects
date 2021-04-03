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

/* getValue:
 * 1 - store value in ax
 * 0 - store address in bx
 */
static int getValue = 1;

/* prototype for internal recursive code generator */
static void cGen(TreeNode *tree);

static int tmp;

/* isRecursive:
 * 1 - cGen will recurse on sibling
 * 0 - cGen won't recurse on sibling
 */
static int isRecursive = 1;

/* stack used for call */
TreeNode *paramStack[10];
int top = 0;

/* stack routines*/
int pushParam(TreeNode *param)
{
  if (top == SIZE)
    return 1;

  paramStack[top++] = param;
  return 0;
}

TreeNode *popParam()
{
  if (top == 0)
    return NULL;

  return paramStack[--top];
}

/* emit one instruction to get the address of a var,
 * store the address in bx,
 * we can access the var by bx[0]
 */
void emitGetAddr(TreeNode *var)
{

  if(var->scope == 0) {
    if (var->isArray) {
      emitRM("LDA", bx, -(st_lookup(var->attr.name, 0)), mp, "get global array address");
    } else {
      emitRM("LDA", bx, -1 - (st_lookup(var->attr.name, 0)), mp, "get global address");
    }
  } else {
    if (var_lookup(var->attr.name, var->scope)->declNode->isParameter) {
      if (var->isArray) {
        emitRM("LD", bx, 2 + (st_lookup(var->attr.name, var->scope)), gp, "get param array address");
      } else {
        emitRM("LDA", bx, 2 + (st_lookup(var->attr.name, var->scope)), gp, "get param variable address");
      }
    } else {
      if (var->isArray) {
        emitRM("LDA", bx, -(st_lookup(var->attr.name, var->scope)), gp, "get local array address");
      } else {
        emitRM("LDA", bx, -1 - (st_lookup(var->attr.name, var->scope)), gp, "get local address");
      }
    }
  }
}

/* Procedure genDec generates code at an declaration node */
static void genDec(TreeNode *tree)
{
  switch(tree->kind.dec) {
    case VarK: {
      break;
    }
    case ArrayK: {
      break;
    }
    case FunK: {
      //> Store the function start point
      BucketList func = fun_lookup(tree->attr.name, tree->scope);
      func->fun_start = emitSkip(0);
      cGen(tree->child[0]); //< parameter list
      cGen(tree->child[1]); //< function body

      //> Restore function state and return to caller
      // todo
      break;
    }
  }
} /* genDec */

/* Procedure genStmt generates code at a statement node */
static void genStmt(TreeNode *tree)
{

  switch(tree->kind.stmt) {
    case IfK: {
      cGen(tree->child[0]); //< condition
      //> location of the if() skip to else block
      int saved_location1 = emitSkip(1); 

      cGen(tree->child[1]); //< if
      //> location of end of the true block so skip to end
      int saved_location2 = emitSkip(1);

      //> emit skip to else block
      int current_location = emitSkip(0);
      emitBackup(saved_location1);
      emitRM_Abs("JEQ", bx, current_location, "if: false so jump to else");
      emitRestore();

      cGen(tree->child[2]); //< else

      //> emit skip to end block
      current_location = emitSkip(0);
      emitBackup(saved_location2);
      emitRM_Abs("JEQ", bx, current_location, "if: true so jump to end");
      emitRestore();
      break;
    }
    case WhileK: {
      int condition_location = emitSkip(0);
      cGen(tree->child[0]); //< condition

      int jump_location = emitSkip(1); //< jump to end
      cGen(tree->child[1]);

      //> jump back to start
      emitRM_Abs("JEQ", zero, condition_location, "jump to begin of while");

      //> back patch jump to end
      int current_location = emitSkip(0);
      emitBackup(jump_location);
      emitRM_Abs("JNE", bx, current_location, "while: false so jump to end");
      emitRestore();
      break;
    }
    case CallK: {
      //jump to the address of this
      //load result into a register?
      break;
    }
    case ReturnK: {
      //put the return value somewhere
      //load the return address and the other stuff
      break;
    }
    case CompoundK: {
      cGen(tree->child[0]);
      cGen(tree->child[1]);
      break;
    }
  }
} /* genStmt */

/* Procedure genExp generates code at an expression node */
static void genExp(TreeNode *tree)
{
  switch(tree->kind.exp) {
    case OpK: {
      cGen(tree->child[0]);
      emitRM("ST", bx, tmp--, mp, "operator: push left");
      cGen(tree->child[1]);
      emitRM("LD", ax, ++tmp, mp, "operator: load right");
      switch(tree->attr.op) {
        case PLUS: {
          emitRO("ADD", bx, ax, bx, "operator +");
          break;
        }
        case MINUS: {
          emitRO("SUB", bx, ax, bx, "operator -");
          break;
        }
        case TIMES: {
          emitRO("MUL", bx, ax, bx, "operator *");
          break;
        }
        case OVER: {
          emitRO("DIV", bx, ax, bx, "operator /");
          break;
        }
        case EQ: {
          emitRO("SUB", bx, ax, bx, "operator ==");
          emitRM("JEQ", bx, 2, pc, "jump if true");
          emitRM("LDC", bx, 0, bx, "load false into register");
          emitRM("LDA", pc, 1, pc, "unconditional jump");
          emitRM("LDC", bx, 1, bx, "load true into register");
          break;
        }
        case NQ: {
          emitRO("SUB", bx, ax, bx, "operator !=");
          emitRM("JNE", bx, 2, pc, "jump if true");
          emitRM("LDC", bx, 0, bx, "load false into register");
          emitRM("LDA", pc, 1, pc, "unconditional jump");
          emitRM("LDC", bx, 1, bx, "load true into register");
          break;
        }
        case LT: {
          emitRO("SUB", bx, ax, bx, "operator <");
          emitRM("JLT", bx, 2, pc, "jump if true");
          emitRM("LDC", bx, 0, bx, "load false into register");
          emitRM("LDA", pc, 1, pc, "unconditional jump");
          emitRM("LDC", bx, 1, bx, "load true into register");
          break;
        }
        case LE: {
          emitRO("SUB", bx, ax, bx, "operator <=");
          emitRM("JLE", bx, 2, pc, "jump if true");
          emitRM("LDC", bx, 0, bx, "load false into register");
          emitRM("LDA", pc, 1, pc, "unconditional jump");
          emitRM("LDC", bx, 1, bx, "load true into register");
          break;
        }
        case GT: {
          emitRO("SUB", bx, ax, bx, "operator >");
          emitRM("JGT", bx, 2, pc, "jump if true");
          emitRM("LDC", bx, 0, bx, "load false into register");
          emitRM("LDA", pc, 1, pc, "unconditional jump");
          emitRM("LDC", bx, 1, bx, "load true into register");
          break;
        }
        case GE: {
          emitRO("SUB", bx, ax, bx, "operator >=");
          emitRM("JGE", bx, 2, pc, "jump if true");
          emitRM("LDC", bx, 0, bx, "load false into register");
          emitRM("LDA", pc, 1, pc, "unconditional jump");
          emitRM("LDC", bx, 1, bx, "load true into register");
          break;
        }
      }
      break;
    }
    case ConstK: {
      emitRM("LDC", bx, tree->attr.val, 0, "load constant");
      break;
    }
    case IdK: {
      emitGetAddr(tree);
      break;
    }
    case AssignK: {
      cGen(tree->child[0]);
      emitRM("ST", bx, tmp--, mp, "operator: push left");
      cGen(tree->child[1]);
      emitRM("LD", ax, ++tmp, mp, "operator: load right");

      emitRM("ST", bx, ax, gp, "assignment to store value");
      break;
    }
  }
} /* genExp */

/* Procedure cGen recursively generates code by
 * tree traversal
 */
static void cGen(TreeNode *tree)
{
  if (tree != NULL)
  {
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
    default:
      break;
    }
    if (isRecursive)
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
    emitRO("IN", ax, 0, 0, "read input into ax");
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
    emitRM("LD", ax, 1, fp, "load param into ax");
    emitRO("OUT", ax, 0, 0, "output using ax");
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

  emitRM("LDA", ax, 3, pc, "store returned PC");
  emitRM("LDA", fp, -1, fp, "push prepare");
  emitRM("ST", ax, 0, fp, "push returned PC");
  emitRM("LDC", pc, fun->fun_start, 0, "jump to function");
  emitRM("LDA", fp, main_locals, fp, "release local var");

  emitComment("End of execution.");
  emitRO("HALT", 0, 0, 0, "");
}
