#ifndef LEX_H_INCLUDED
#define LEX_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "token.h"

#ifdef _WIN32
#define LINE_FEED_COUNT 1
#else
#define LINE_FEED_COUNT 0
#endif // _WIN32

#define INITIAL_STATE -1
#define BEGIN_LOGIC_AND_STATE -2
#define BEGIN_LOGIC_OR_STATE -3
#define BEGIN_STRING_STATE -4
#define END_STRING_STATE -5
#define BEGIN_REAL_NUMBER_STATE -6
#define ERROR_STATE -7

Token* getNextToken(FILE* fileToAnalyze);
void ungetToken(Token* token);
Token* getTokenFromFile(FILE* fileToAnalyze);
int mygetChar(FILE* fileToAnalyze);
void myUngetchar(int c);
void setStateByToken(char token, int* state);
void setErrorIfExists(int* state);
void comprobeAndSetKeyWord(Token* token);

#endif // LEX_H_INCLUDED
