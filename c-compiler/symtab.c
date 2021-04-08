/****************************************************/
/* File: symtab.c                                   */
/* Symbol table implementation for the C- compiler  */
/* (allows only one symbol table)                   */
/* Symbol table is implemented as a chained         */
/* hash table                                       */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#include "globals.h"

static struct ScopeList
{
  BucketList hashTable[SIZE];
} Scope[MAX_SCOPE] = {0};

void reset_sym_table() {
  memset(Scope, 0, sizeof(Scope));
}
/* SHIFT is the power of two used as multiplier
   in hash function  */
#define SHIFT 4

static int hash(char * const key)
{
  int temp = 0;
  int i = 0;
  while (key[i] != '\0')
  {
    temp = ((temp << SHIFT) + key[i]) % SIZE;
    ++i;
  }
  return temp;
}

/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */
void st_insert(TreeNode* declNode, int lineno, int loc, int scope)
{
  char * const name = declNode->attr.name;
  int h = hash(name);
  int insert_scope = scope;
  if(loc == -1){
    insert_scope = 0;
  }
  BucketList l = Scope[insert_scope].hashTable[h];
  while ((l != NULL) && (strcmp(name, l->declNode->attr.name) != 0)){
    l = l->next;
  }

  if (l != NULL)
  {
    printf("Variable has already been declared in current scope");
    return;
  }

  l = (BucketList)malloc(sizeof(struct BucketListRec));
  l->declNode = declNode;
  l->scope = scope;     /* scope of variable */
  l->lines = (LineList)malloc(sizeof(struct LineListRec));
  l->lines->lineno = lineno;
  l->memloc = loc;
  l->lines->next = NULL;
  l->next = NULL;
  Scope[insert_scope].hashTable[h] = l;
} /* st_insert */

/* Function fun_lookup returns the  bucket 
 * location of a funtion or NULL if not found
 */
BucketList fun_lookup(char *name, int sp)
{
  int h = hash(name);
  BucketList l = Scope[sp].hashTable[h];
  while ((l != NULL) && (strcmp(name, l->declNode->attr.name) != 0))
    l = l->next;

  if(l == NULL){
    if(sp != 0){
      return fun_lookup(name, 0);
    }
    return NULL;
  }
  return l;
}

BucketList fun_lookup_scope(int scope) {
  for(int idx = 0; idx < SIZE; idx++){
    BucketList lookup = Scope[0].hashTable[idx];
    if(lookup != NULL && lookup->scope == scope){
      return lookup;
    }
  }
  if(scope != 0){
    return fun_lookup_scope(0);
  }
  return NULL;
}
/* Function var_lookup returns the  bucket 
 * location of a var or NULL if not found
 */
BucketList var_lookup(char *name, int sp)
{
  int h = hash(name);
  BucketList l = Scope[sp].hashTable[h];
  while((l != NULL) && (strcmp(name, l->declNode->attr.name) != 0)){
    l = l->next;
  }
  if(l == NULL) {
    if(sp != 0) {
      return var_lookup(name, 0);
    }
    return NULL;
  } else {
    return l;
  }
}

/* Function st_lookup returns the memory 
 * location of a variable or -1 if not found
 */
int st_lookup(char *name, int sp)
{
  int h = hash(name);
  BucketList l = Scope[sp].hashTable[h];
  while ((l != NULL) && (strcmp(name, l->declNode->attr.name) != 0)){
    l = l->next;
  }

  if (l == NULL){
    if(sp != 0){
      return st_lookup(name, 0);
    }
    return -1;
  }
  else{
    return l->memloc;
  }
}

void add_reference_to(BucketList var, int lineno) {
  LineList t = var->lines;
  while (t->next != NULL)
    t = t->next;
  t->next = (LineList)malloc(sizeof(struct LineListRec));
  t->next->lineno = lineno;
  t->next->next = NULL;
}

/* Procedure printSymTab prints a formatted 
 * listing of the symbol table contents 
 * to the listing file
 */
void printSymTab(FILE *listing)
{
  for (int j = 0; j < MAX_SCOPE; j++)
  {
    bool first = true;
    for (int i = 0; i < SIZE; ++i)
    {
      if (Scope[j].hashTable[i] == NULL){ continue; }
      if(first){
        fprintf(listing, "Scope | Variable Name | Type | Location | Line Numbers\n");
        fprintf(listing, "------|---------------|------|----------|-------------\n");
        first = false;
      }

      BucketList l = Scope[j].hashTable[i];
      while (l != NULL)
      {
        LineList t = l->lines;
        fprintf(listing, "%-6d|", l->scope);
        fprintf(listing, " %-13s |", l->declNode->attr.name);
        switch(l->declNode->type){
          case Integer: {
            fprintf(listing, " int  |");
            break;
          }
          case Void: {
            fprintf(listing, " void |");
            break;
          }
          case Boolean: {
            fprintf(listing, " bool |");
            break;
          }
        }
        fprintf(listing, " %-8d | ", l->memloc);
        while (t != NULL)
        {
          fprintf(listing, "%-4d ", t->lineno);
          t = t->next;
        }
        fprintf(listing, "\n");
        l = l->next;
      }
    }
    if(!first){
      fprintf(listing, "\n");
    }
  }
} /* printSymTab */
