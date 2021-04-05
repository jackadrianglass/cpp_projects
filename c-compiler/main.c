/****************************************************/
/* File: main.c                                     */
/* Main program for C- compiler                     */
/*                                                  */
/****************************************************/

#include <stdbool.h>
#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"
#include "analyze.h"
#include "symtab.h"
#include "cgen.h"

/* allocate global variables */
int lineno = 0;
FILE *source;
FILE *listing;
FILE *code;

/* allocate and set tracing flags */
bool EchoSource = false;
bool TraceScan = false;
bool TraceParse = false;
bool TraceAnalyze = true;
bool TraceCode = true;

bool Error = false;

void parse_args(int argc, char** argv) {
  char pgm[120]; /* source code file name */
  if (argc < 3 || argc > 4)
  {
    fprintf(stderr, "usage: %s <in_filename> <code_filename> <listing_filename (optional)>\n", argv[0]);
    exit(1);
  }

  strcpy(pgm, argv[1]);
  if (strchr(pgm, '.') == NULL) {
    strcat(pgm, ".CM");
  }
  source = fopen(pgm, "r");
  if (source == NULL)
  {
    fprintf(stderr, "File %s not found\n", pgm);
    exit(1);
  }

  strcpy(pgm, argv[2]);
  code = fopen(pgm, "w");
  if(code == NULL) {
    fprintf(stderr, "Unable to create %s\n", pgm);
    exit(1);
  }

  if(argc == 4){
    strcpy(pgm, argv[2]);
    listing = fopen(pgm, "w");
    if (listing == NULL)
    {
      fprintf(stderr, "Unable to create %s\n", pgm);
      exit(1);
    }
  } else {
    listing = stdout;
  }
  fprintf(listing, "\nC- COMPILATION: %s\n", pgm);
}

int main(int argc, char** argv)
{
  parse_args(argc, argv);

  // yydebug = 1; //enable it when you want to debug your parser
  TreeNode* syntaxTree = parse();
  fclose(source);

  if (TraceParse)
  {
    printTree(syntaxTree);
  }

  buildSymtab(syntaxTree);
  typeCheck(syntaxTree);
  if (TraceAnalyze)
  {
    fprintf(listing, "\nSymbol table:\n\n");
    printSymTab(listing);
  }

  codeGen(syntaxTree, argv[2]);
}
