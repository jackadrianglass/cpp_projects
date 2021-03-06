/****************************************************/
/* File: scan.h                                     */
/* The scanner interface for the C- compiler        */
/*                                                  */
/****************************************************/

#ifndef _SCAN_H_
#define _SCAN_H_

#include "globals.h"
/* MAXTOKENLEN is the maximum size of a token */
#define MAXTOKENLEN 40

/* tokenString array stores the lexeme of each token */
extern char tokenString[MAXTOKENLEN+1];

/* previous tokenString array stores the lexeme of previous token */
extern char previousTokenString[MAXTOKENLEN+1];

extern char currTokenString[MAXTOKENLEN+1];
/* function getToken returns the 
 * next token in source file
 */
TokenType getToken();

#endif
