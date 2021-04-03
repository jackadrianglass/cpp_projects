/****************************************************/
/* File: analyze.c                                  */
/* Semantic analyzer                                */
/****************************************************/

#include "globals.h"
#include "symtab.h"
#include "analyze.h"

static int curr_scope = 0;
static int scope_count = 0;
static bool done_nothing = true;


/* counter for variable memory locations */
static int location[MAX_SCOPE] = {0};

bool is_main(TreeNode* t);

static void genericError(TreeNode *t, char *msg);
static void typeError(TreeNode *t, char *msg);

static void insertNode(TreeNode *t);
static void insertDecl(TreeNode *t);
static void symbolCheckExpr(TreeNode* t);

static void checkNode(TreeNode *t);
static void typeCheckStmt(TreeNode* t);
static void typeCheckExpr(TreeNode* t);
static void typeCheckDecl(TreeNode* t);

/* Procedure traverse is a generic recursive 
 * syntax tree traversal routine:
 * it applies preProc in preorder and postProc 
 * in postorder to tree pointed to by t
 */
static void traverse(TreeNode *t,
                     void (*preProc)(TreeNode *),
                     void (*postProc)(TreeNode *))
{
  if (t == NULL){return;}
  preProc(t);
  for (int i = 0; i < MAXCHILDREN; i++){
    traverse(t->child[i], preProc, postProc);
  }
  postProc(t);
  traverse(t->sibling, preProc, postProc);
}

/* nullProc is a do-nothing procedure to 
 * generate preorder-only or postorder-only
 * traversals from traverse
 */
static void nullProcPre(TreeNode *t){ 
  if(t == NULL) return; 
  // set to function scope
  if(t->nodekind == DecK && t->kind.dec == FunK){ 
    scope_count++;
    curr_scope = scope_count;
  }
}

static void nullProcPost(TreeNode *t){ 
  if(t == NULL) return; 
  // reset to global scope
  if(t->nodekind == DecK && t->kind.dec == FunK){
    curr_scope = 0;
    if(done_nothing){
      scope_count--;
    }
  }
}

/* Function buildSymtab constructs the symbol 
 * table by preorder traversal of the syntax tree
 */
void buildSymtab(TreeNode *syntaxTree)
{
  scope_count = 0;
  curr_scope = 0;
  reset_sym_table();
  traverse(syntaxTree, insertNode, nullProcPost);
}

/* Procedure typeCheck performs type checking 
 * by a postorder syntax tree traversal
 */
void typeCheck(TreeNode *syntaxTree)
{
  scope_count = 0;
  curr_scope = 0;
  traverse(syntaxTree, nullProcPre, checkNode);
}


/* Procedure insertNode inserts 
 * identifiers stored in t into 
 * the symbol table 
 */
static void insertNode(TreeNode *t)
{
  if(t == NULL){ return; }
  switch(t->nodekind) {
    case StmtK: {
      // do nothing
      break;
    }
    case ExpK: {
      symbolCheckExpr(t);
      break;
    }
    case DecK: {
      insertDecl(t);
      break;
    }
  }
}

static void symbolCheckExpr(TreeNode* t) {
  switch(t->kind.exp) {
    case OpK:     // fallthrough
    case AssignK: // fallthrough
    case ConstK: {
      // do nothing
      break;
    }
    case IdK: {
      if(st_lookup(t->attr.name, curr_scope) == -1) {
        genericError(t, "Use of undeclared var");
        return;
      }
      break;
    }
  }
}


static void insertDecl(TreeNode *t) {
  switch(t->kind.dec){
    case FunK: {
      scope_count++;
      curr_scope = scope_count;
      if(scope_count >= MAX_SCOPE) {
        genericError(t, "Scope count exceeded max scope allowance, consider increasing max scope");
        return;
      }
      st_insert(t, t->lineno, -1, curr_scope);
      done_nothing = true;
      break;
    }
    case VarK: {
      if(curr_scope == 0){
        t->isGlobal = true;
      } else {
        done_nothing = false;
      }
      st_insert(t, t->lineno, location[curr_scope], curr_scope);
      location[curr_scope]++;
      break;
    }
    case ArrayK: {
      if(curr_scope == 0){
        t->isGlobal = true;
      } else {
        done_nothing = false;
      }
      st_insert(t, t->lineno, location[curr_scope], curr_scope);
      location[curr_scope] += t->arraySize;
      break;
    }
  }
}
/* Procedure checkNode performs
 * type checking at a single tree node
 */
static void checkNode(TreeNode *t)
{
  if(t == NULL) return;
  switch(t->nodekind){
    case StmtK: {
      typeCheckStmt(t);
      break;
    }
    case ExpK: {
      typeCheckExpr(t);
      break;
    }
    case DecK: {
      typeCheckDecl(t);
      // reset to global scope
      if(t->kind.dec == FunK){
        curr_scope = 0;
        if(done_nothing){
          scope_count--;
        }
      }
      break;
    }
  }
  if(t->sibling == NULL && curr_scope == 0 && !is_main(t)) {
    genericError(t, "Incorrect or missing declaration of int main(void)");
  }
}

static bool isOutput(TreeNode* t) {
  return strcmp(t->attr.name, "output") == 0
      && t->param_size == 1
      && t->child[0] != NULL
      && t->child[0]->type == Integer;
}

bool is_main(TreeNode* t) {
  return t->nodekind == DecK
      && t->kind.dec == FunK
      && t->type == Integer
      && strcmp(t->attr.name, "main") == 0
      && t->param_size == 0
      && t->child[0] == NULL;
}

static void typeCheckStmt(TreeNode* t){
  switch(t->kind.stmt) {
    case IfK: {
      if(t->child[0]->type != Boolean) {
        typeError(t, "If statement requires an expression of type bool");
        return;
      }
      break;
    }
    case WhileK: {
      if(t->child[0]->type != Boolean) {
        typeError(t, "While statement requires an expression of type bool");
        return;
      }
      break;
    }
    case CallK: {
      BucketList call = fun_lookup(t->attr.name, curr_scope);
      if(call == NULL) {
        if(!isOutput(t)) {
          genericError(t, "Call to unknown function");
        } else {
          t->type = Void;
        }
        return;
      }

      TreeNode* decl = call->declNode;
      if(decl->param_size > 0) {
        TreeNode* decl_child = decl->child[0];
        TreeNode* node_child = t->child[0];
        do{
          if((node_child == NULL && decl_child != NULL)
            || (node_child != NULL && decl_child == NULL)){
            typeError(t, "Incorrect number of parameters passed into function");
            return;
          }
          if(node_child->type != decl_child->type){
            typeError(t, "Type mismatch to a parameter of ");
            return;
          }
          decl_child = decl_child->child[0];
          node_child = node_child->child[0];
        } while(decl_child != NULL || node_child != NULL);
      }

      t->type = decl->type;
      break;
    }
    case ReturnK: {
      BucketList fn = fun_lookup_scope(curr_scope);
      TreeNode* decl = fn->declNode;
      if(decl->type == Void && t->child[0] != NULL) {
        typeError(t, "Void function cannot return a value");
      } else if(t->child[0] == NULL) {
        typeError(t, "A non void function must return a value");
      } else if(decl->type != t->child[0]->type) {
        typeError(t, "Must return an expression that matches the functions return type");
      }
      break;
    }
    case CompoundK: {
      // do nothing
      break;
    }
  }
}

bool isRelop(TreeNode* t) {
  return t->attr.op == EQ
      || t->attr.op == NQ
      || t->attr.op == LT
      || t->attr.op == GT
      || t->attr.op == LE
      || t->attr.op == GE;
}

bool areChildrenSameType(TreeNode* t) {
  return t->child[0] != NULL
      && t->child[1] != NULL
      && t->child[0]->type == t->child[1]->type;
}

static void typeCheckExpr(TreeNode* t){
  switch(t->kind.exp) {
    case OpK: {
      if(!areChildrenSameType(t)) {
        typeError(t, "Operators must be applied to expressions of the same type");
        return;
      }

      if(isRelop(t)) {
        t->type = Boolean;
      } else {
        t->type = t->child[0]->type;
      }
      break;
    }
    case ConstK: {
      t->type = Integer;
      break;
    }
    case IdK: {
      BucketList var = var_lookup(t->attr.name, curr_scope);
      if(var == NULL) {
        genericError(t, "Reference to unknown identifier");
        return;
      }

      t->type = var->declNode->type;
      add_reference_to(var, t->lineno);
      break;
    }
    case AssignK: {
      if(!areChildrenSameType(t)){
        typeError(t, "Must assign an expression to an identifier of the same type");
      }
      t->type = t->child[0]->type;
      break;
    }
  }
}

static void typeCheckDecl(TreeNode* t){
  switch(t->kind.dec) {
    case ArrayK: // fallthrough
    case VarK: {
      if(t->type != Integer) {
        typeError(t, "Identifiers must be of type Integer");
        return;
      }
      break;
    }
    case FunK: {
      // do nothing
      break;
    }
  }
}

static void genericError(TreeNode *t, char *msg) {
  fprintf(listing, "Error at line %d: %s\n", t->lineno, msg);
  Error = true;
}

static void typeError(TreeNode *t, char *message)
{
  fprintf(listing, "Type error at line %d: %s\n", t->lineno, message);
  Error = TRUE;
}
