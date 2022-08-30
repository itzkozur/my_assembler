#ifndef action_h
#define action_h
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdarg.h>

#define MAX_LINE 80
#define MAX_LABEL 30
#define MAX_CHAR 10
#define NUM_OF_ACTIONS 50
#define MAX_SIZE 80
#define MAX_CODE 32



typedef struct{
	char name[30];
	int value;
	char attributes[2][10];
	int attrNum;
} symbol;

typedef struct node * ptr;
typedef struct node {
	symbol data;
	ptr next;
} symbolList;

typedef struct{
	int address;
	char sourceCode[MAX_SIZE];
	char machineCode[MAX_CODE];
	char label[MAX_SIZE] ;
	char commend[MAX_SIZE] ;
} rowTable;

typedef struct t_node * t_ptr;
typedef struct t_node {
	rowTable data;
	t_ptr next;
} tableList;


typedef struct
{
	char name[MAX_CHAR];
	char type;
	int funct;
	int opcode;
} action;

void addToTableList(t_ptr *, rowTable);
void freeTableList(t_ptr *);
void printTableList(t_ptr);
int getOpcodeByName(char *);
char getTypeByName(char *);
char *getBinCode(int address, char *commendName, int howManyVariables, int whichVariableisChar, ...);
int getSizeBytsByGuidance(char *);
char* presentNumberAsBinary(int, int);

void addToSymbolList(ptr *, symbol);
void freeSymbolList(ptr *);
void printSymbolList(ptr);
int getValueBySmboleName(ptr, char *);
int isSymbolExists(ptr listHead, char * symbol);
void addAttributeToSymbol(ptr listHead, char * symbol, char* attr);
int hasExternAttribute(ptr listHead, char *);
int what_command(char[], int, int, t_ptr *, ptr *);
int printbinary2Hexadecimal(t_ptr, char[], int, int);
int run2(char[], int, ptr, t_ptr);
int updateSymbol_Table(char[], char[], int, ptr);
void compliteTable(ptr, t_ptr);
void addGuidanceToTables(ptr *, t_ptr *, int, char *, char *, char *, int, ...);
void addNumbersGuidanceToTables(ptr *, t_ptr *, char *, char *, char *, int, int *);
int isCommend(char[]);
int printEntry(ptr ,char *);
int printExtern( char * , ptr ,t_ptr);
void findExtern(ptr  ,ptr );
int validLenLine(char *);



#endif
