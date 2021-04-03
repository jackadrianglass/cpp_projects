/****************************************************/
/* File: globals.h                                  */
/* Global types and vars for C-                     */
/* must come before other include files             */
/*                                                  */
/****************************************************/

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

/* MAXRESERVED = the number of reserved words */
#define MAXRESERVED 10

typedef int TokenType;
extern FILE *source;  /* source code text file */
extern FILE *listing; /* listing output text file */
extern FILE *code;    /* code text file for TM simulator */

extern int lineno; /* source line number for listing */

typedef enum
{
   StmtK,
   ExpK,
   DecK
} NodeKind;

typedef enum
{
   IfK,
   WhileK,
   CallK,
   ReturnK,
   CompoundK
} StmtKind;

typedef enum
{
   OpK,
   ConstK,
   IdK,
   AssignK
} ExpKind;

typedef enum
{
   VarK,
   ArrayK,
   FunK
} DecKind;

/* ExpType is used for type checking */
typedef enum
{
   Void,
   Integer,
   Boolean
} ExpType;

#define MAXCHILDREN 3

typedef struct treeNode
{
   struct treeNode* child[MAXCHILDREN];
   struct treeNode* sibling;
   int lineno;
   NodeKind nodekind;
   union
   {
      StmtKind stmt;
      ExpKind exp;
      DecKind dec;
   } kind;
   union
   {
      TokenType op;
      int val;
      char *name;
   } attr;

   // items for semantic analysis
   int value; // array index
   bool isArray;
   int arraySize;
   /*
   If isParameter is TRUE, then this node declares an actual parameter
   to a function.
   */
   bool isParameter;
   bool isGlobal;     // If isGlobal is TRUE, then the variable is a global
   int param_size;   // parameter count and local variable count
   int local_size;
   int scope;        // scope of the node
   ExpType type; /* for type checking of exps */
} TreeNode;

/* Yacc/Bison generates internally its own values
 * for the tokens. Other files can access these values
 * by including the tab.h file generated using the
 * Yacc/Bison option -d ("generate header")
 *
 * The YYPARSER flag prevents inclusion of the tab.h
 * into the Yacc/Bison output itself
 */

#ifndef YYPARSER

/* the name of the following file may change */
#include "parser.y.h"

/* ENDFILE is implicitly defined by Yacc/Bison,
 * and not included in the tab.h file
 */
#define ENDFILE 0

#endif

/**************************************************/
/***********   Flags for tracing       ************/
/**************************************************/

/* EchoSource = TRUE causes the source program to
 * be echoed to the listing file with line numbers
 * during parsing
 */
extern bool EchoSource;

/* TraceScan = TRUE causes token information to be
 * printed to the listing file as each token is
 * recognized by the scanner
 */
extern bool TraceScan;

/* TraceParse = TRUE causes the syntax tree to be
 * printed to the listing file in linearized form
 * (using indents for children)
 */
extern bool TraceParse;

/* TraceAnalyze = TRUE causes symbol table inserts
 * and lookups to be reported to the listing file
 */
extern bool TraceAnalyze;

/* TraceCode = TRUE causes comments to be written
 * to the TM code file as code is generated
 */
extern bool TraceCode;

/* Error = TRUE prevents further passes if an error occurs */
extern bool Error;
#endif
