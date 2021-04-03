/****************************************************/
/* File: symtab.h                                   */
/* Symbol table interface for the C-  compiler      */
/* (allows only one symbol table)                   */
/* C- Compiler Project				                */
/*       	                                        */
/****************************************************/

#ifndef _SYMTAB_H_
#define _SYMTAB_H_
#include <stdio.h>
#include <stdbool.h>
#include "globals.h"

/* SIZE is the size of the hash table */
#define SIZE 211

/* MAX_SCOPE is maximum allowed scope can be changed for larger programs*/
#define MAX_SCOPE 8

/* the list of line numbers of the source 
 * code in which a variable is referenced
 */
typedef struct LineListRec
{
  int lineno;
  struct LineListRec *next;
} * LineList;

/* The record in the bucket lists for
 * each variable, including name, 
 * assigned memory location, and
 * the list of line numbers in which
 * it appears in the source code
 */
typedef struct BucketListRec
{
  TreeNode* declNode;
  LineList lines;
  int memloc;    /* memory location for variable */
  int scope;     /* scope of the variable */
  int fun_start;
  struct BucketListRec *next;
} * BucketList;

void reset_sym_table();

/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */
void st_insert(TreeNode* declNode, int lineno, int loc, int scope);

/* Function st_lookup returns the memory 
 * location of a variable or -1 if not found
 */
int st_lookup(char *name, int scope);

/* Function fun_lookup returns the  bucket 
 * location of a funtion or NULL if not found
 */
BucketList fun_lookup(char *name, int sp);

BucketList fun_lookup_scope(int scope);

/* Function var_lookup returns the  bucket 
 * location of a var or NULL if not found
 */
BucketList var_lookup(char *name, int sp);

void add_reference_to(BucketList var, int lineno);

/* Procedure printSymTab prints a formatted 
 * listing of the symbol table contents 
 * to the listing file
 */
void printSymTab(FILE *listing);

#endif
