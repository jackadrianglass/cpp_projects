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
#include "analyze.h"

/* isRecursive:
 * 1 - cGen will recurse on sibling
 * 0 - cGen won't recurse on sibling
 */
static int isRecursive = 1;

/* prototype for internal recursive code generator */
static void cGen(TreeNode *tree);

static int tmp;

/* stack used for call */
int top = 0;
static int param_idx = 0; //< used for bottom up indexing

#define stack_size 10
TreeNode *param_stack[stack_size][stack_size];
static int which_stack = -1;

static bool return_created = false;
static bool ignore_param = false;
static bool use_fp_for_param = true;

/* stack routines*/
int pushParam(TreeNode *param)
{
  if (top == SIZE)
    return 1;

  param_stack[which_stack][top++] = param;
  return 0;
}

void resetParamStack() {
  top = 0;
  param_idx = 0;
  memset(param_stack[which_stack], 0, stack_size * sizeof(TreeNode*));
}

/* emit one instruction to get the address of a var,
 * store the address in ax1,
 * we can access the var by ax1[0]
 */
void emitGetAddr(TreeNode *node) {
  if(node->isGlobal) {
    emitRM("LDA", ax1, -1 - (st_lookup(node->attr.name, 0)), gp, "get global address");
  } else if (var_lookup(node->attr.name, node->scope)->declNode->isParameter) {
    if(use_fp_for_param) {
      emitRM("LDA", ax1, 2 + (st_lookup(node->attr.name, node->scope)), fp, "get param variable address");
    } else {
      emitRM("LDA", ax1, 2 + (st_lookup(node->attr.name, node->scope)), sp, "get param variable address");
    }
  } else {
    emitRM("LDA", ax1, -1 - (st_lookup(node->attr.name, node->scope)), fp, "get local address");
  }
}

static void genFuncEnd(TreeNode* node) {
  //> Restore function state and return to caller
  emitComment("Prep to exit function");
  emitRO("ADD", sp, fp, zero, "copy fp value into sp"); //< removes all locals

  emitRM("LD", ax1, 0, fp, "load return address into ax1");
  emitRM("LD", fp, 1, fp, "load control link into fp");
  emitRM("LDA", sp, node->param_size + 2, sp, "move sp above function frame");

  emitRO("ADD", pc, ax1, zero, "load return address into pc (jump)");
}

/* Procedure genDec generates code at an declaration node */
static void genDec(TreeNode *node)
{
  switch(node->kind.dec) {
    case VarK: {
      emitRM("LDA", sp, -1, sp, "move sp down to allocate sizeof(int)");
      break;
    }
    case ArrayK: {
      emitRM("LDA", sp, -node->arraySize, sp, "move sp down to allocate sizeof([])");
      break;
    }
    case FunK: {
      char name[64];
      //> Store the function start point
      BucketList func = fun_lookup(node->attr.name, node->scope);
      func->fun_start = emitSkip(0);
      sprintf(name, ">>> Begin Function %s", node->attr.name);
      emitComment(name);

      // No need to call parameter list as they're all allocated already
      cGen(node->child[1]); //< function body
      if(!return_created){
        genFuncEnd(node);     //< implicit return
      } else {
        return_created = false;
      }
      sprintf(name, ">>> Ending Function %s", node->attr.name);
      emitComment(name);
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
  emitRM_Abs("JEQ", ax2, current_location, "if(false) jump to else");
  emitRestore();
  
  cGen(node->child[2]); //< else

  //> emit skip to end block
  current_location = emitSkip(0);
  emitBackup(saved_location2);
  emitRM_Abs("JEQ", zero, current_location, "if: end of true block so jump to end");
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
  emitRM_Abs("JEQ", ax2, current_location, "while: false so jump to end");
  emitRestore();
}

void genParam(TreeNode* node, BucketList func) {
  TreeNode* decl = func->declNode->child[0];
  while(decl != NULL) {
    pushParam(decl);
    decl = decl->sibling;
  }

  cGen(node->child[0]);
  resetParamStack();
}

void genParamStore(TreeNode* node) {
  if(node->isParameter && !ignore_param){
    TreeNode* decl = param_stack[which_stack][param_idx++];
    if(decl == NULL) {
      emitComment("Bug in parameter assignment");
      return;
    }
    use_fp_for_param = false;
    emitGetAddr(decl);
    use_fp_for_param = true;
    emitRM("ST", ax2, 0, ax1, "store value of expression into parameter location");
  }
}

static void genCallStmt(TreeNode* node) {
  which_stack++;
  BucketList func = fun_lookup(node->attr.name, node->scope);
  if(strcmp("output", node->attr.name) == 0) {
    ignore_param = true;
    genParam(node, func);
    ignore_param = false;
    emitRO("OUT", ax2, 0, 0, "output using ax2");
    which_stack--;
    return;
  } else if(strcmp("input", node->attr.name) == 0) {
    emitRO("IN", ax2, 0, 0, "input using ax2");
    which_stack--;
    return;
  }

  char name[64];
  sprintf(name, "Start prep to jump to function %s", node->attr.name);
  emitComment(name);

  emitRO("LDA", sp, -(node->param_size + 2), sp, "Move stack pointer down");

  genParam(node, func);

  emitRM("ST", fp, 1, sp, "store control point");
  emitRO("ADD", fp, sp, zero, "set fp to sp");
  emitRM("LDA", ax2, 2, pc, "calculate return address");
  emitRM("ST", ax2, 0, fp, "store return address");

  memset(name, '\0', 64 * sizeof(char));
  sprintf(name, "Jump to function %s", node->attr.name);
  emitRM("JEQ", zero, func->fun_start, zero, name);
  which_stack--;
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
      genParamStore(node);
      break;
    }
    case ReturnK: {
      cGen(node->child[0]);
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
  emitRO("SUB", ax2, ax1, ax2, "sub for comparison");
  emitRM(inst, ax2, 2, pc, "jump if true");
  emitRM("LDC", ax2, 0, zero, "load false into register");
  emitRM("LDA", pc, 1, pc, "unconditional jump");
  emitRM("LDC", ax2, 1, zero, "load true into register");
}

static void genOpExpr(TreeNode* node) {
  cGen(node->child[0]);
  emitRM("ST", ax2, tmp++, mp, "store left child of expr");
  cGen(node->child[1]);
  emitRM("LD", ax1, --tmp, mp, "load left child of expr");
  switch(node->attr.op) {
    case PLUS: {
      emitRO("ADD", ax2, ax1, ax2, "operator +");
      break;
    }
    case MINUS: {
      emitRO("SUB", ax2, ax1, ax2, "operator -");
      break;
    }
    case TIMES: {
      emitRO("MUL", ax2, ax1, ax2, "operator *");
      break;
    }
    case OVER: {
      emitRO("DIV", ax2, ax1, ax2, "operator /");
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
  static bool is_assignment = false;
  switch(node->kind.exp) {
    case OpK: {
      genOpExpr(node);
      break;
    }
    case ConstK: {
      emitRM("LDC", ax2, node->attr.val, zero, "load constant");
      break;
    }
    case IdK: {
      emitGetAddr(node);
      if(!is_assignment){
        emitRM("LD", ax2, 0, ax1, "load value at memory address");
      }
      break;
    }
    case AssignK: {
      is_assignment = true;
      cGen(node->child[0]);
      is_assignment = false;
      //> store the address of the assignee which is located in ax1
      emitRM("ST", ax1, tmp++, mp, "store address of assignee");

      cGen(node->child[1]);
      emitRM("LD", ax1, --tmp, mp, "pop stored address of assignee"); //< restore addr

      emitRM("ST", ax2, 0, ax1, "operator =");
      break;
    }
  }
  genParamStore(node);

} /* genExp */

/* Procedure cGen recursively generates code by
 * tree traversal
 */
static void cGen(TreeNode *tree)
{
  if(tree == NULL) { return; }
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
  emitRM("LD", gp, 0, zero, "load maxaddress from location 0");
  int num_param = syntaxTree->kind.dec != FunK ? -(syntaxTree->param_size) : 0;
  emitRM("LDA", fp, num_param - 2, gp, "copy gp to fp &allocating global variables(if any)");
  emitRO("ADD", sp, 0, fp, "copy fp to sp");
  emitRM("ST", zero, 0, zero, "clear location 0");
  emitComment("End of standard prelude.");

  int loc = emitSkip(8);

  /* generate code for TINY program */
  cGen(syntaxTree);
  /* finish */

  /* Fill up jump-to-main code */
  emitBackup(loc);
  fun = fun_lookup("main", 0);

  emitRM("LDA", ax1, 2, pc, "prep to save return address");
  emitRM("ST", ax1, 0, fp, "save return address");
  emitRM("JEQ", zero, fun->fun_start, zero, "jump to main()");
  emitRO("HALT", 0, 0, 0, "End of execution.");
}
