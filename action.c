#include "main.h"


action actions[NUM_OF_ACTIONS] =
{
		{"add\0",'R',1,0},
		{"sub\0",'R',2, 0},
		{"and\0",'R', 3, 0},
		{"or\0",'R',4,0},
		{"nor\0",'R',5,0},
		{"move\0",'R',1,1},
		{"mvhi\0",'R',2,1},
		{"mvlo\0",'R',3,1},
		{"addi\0",'I',-1,10},
		{"subi\0",'I',-1,11},
		{"andi\0",'I',-1,12},
		{"ori\0",'I',-1,13},
		{"nori\0",'I',-1,14},
		{"bne\0",'I',-1,15},
		{"beq\0",'I',-1,16},
		{"blt\0",'I',-1,17},
		{"bgt\0",'I',-1,18},
		{"lb\0",'I',-1, 19},
		{"sb\0",'I',-1,20},
		{"lw\0",'I',-1,21},
		{"sw\0",'I', -1, 22},
		{"lh\0",'I', -1, 23},
		{"sh\0",'I',-1,24},
		{"jmp\0",'J',-1, 30},
		{"la\0",'J',-1,31},
		{"call\0",'J', -1,32},
		{"stop\0",'J',-1, 63}
};

static char *hendleRType(int, int, int, int, int);
static char *hendleIType(int, int, int, int);
static char *hendleITypeWithLabel(int, char *, int, int, int);
static char *hendleJType(int, int);
static char *hendleJTypewithLbael(char *, int);
static char *hendleStopCommend();
static char* myItoa(int, int);
static int getFunctByName(char *);
static char* complateToTwo(char*,int);

static char *printBits(size_t const size, void const *const ptr)
{

	unsigned char *b = (unsigned char *)ptr;
	unsigned char byte;
	int i, j;
	int index = 0;
	char *res; 
	res = (char*)malloc(size * 8 - 1);
	for (i = size - 1; i >= 0; i--)
	{
		for (j = 7; j >= 0; j--)
		{
			byte = (b[i] >> j) & 1;
			res[index++] = '0' + byte;
		}
	}
	res[index] = '\0';
	return res;
}


int getOpcodeByName(char *name)
{
	int i;
	for (i = 0; i < NUM_OF_ACTIONS; i++)
		if ((strcmp(actions[i].name, name)) == 0)
			return actions[i].opcode;
	return -1;
}

char getTypeByName(char *name)
{
	int i;
	for (i = 0; i < NUM_OF_ACTIONS; i++)
		if ((strcmp(actions[i].name, name)) == 0)
			return actions[i].type;
	return 'a';	
}

static int getFunctByName(char *name)
{
	int i;
	for (i = 0; i < NUM_OF_ACTIONS; i++)
		if ((strcmp(actions[i].name, name)) == 0)
			return actions[i].funct;
	return -1;
}

char *getBinCode(int address, char *name, int argNum, int whichChar, ...)
{
	va_list p;
	va_start(p, whichChar);
	
	switch (getTypeByName(name))
	{
	case 'R':
		if (argNum == 3)
		{
			int rs = va_arg(p, int);
			int rt = va_arg(p, int);
			int rd = va_arg(p, int);
			return hendleRType(getFunctByName(name), rd, rt, rs, getOpcodeByName(name));
		}
		if (argNum == 2)
		{
			int rs = va_arg(p, int);
			int rd = va_arg(p, int);
			return hendleRType(getFunctByName(name), rd, 0, rs, getOpcodeByName(name));
		}
		break;

	case 'I':

		if (whichChar == -1)
		{
			int rs = va_arg(p, int);
			int immed = va_arg(p, int);
			int rt = va_arg(p, int);
			return hendleIType(immed, rt, rs, getOpcodeByName(name));
		}
		else
		{
			int rs = va_arg(p, int);
			int rt = va_arg(p, int);
			char *immed = va_arg(p, char *);
			return hendleITypeWithLabel(address, immed, rt, rs, getOpcodeByName(name));
		}
		break;

	case 'J':
		if (getOpcodeByName(name) == 63)
		{
			return hendleStopCommend();
		}
		else
		{
			if (whichChar == -1)
			{
				int varAddress = va_arg(p, int);
				return hendleJType(varAddress, getOpcodeByName(name));
			}
			else
			{
				char *varAddress = va_arg(p, char *);
				return hendleJTypewithLbael(varAddress, getOpcodeByName(name));
			}
		}
		break;
	}

	return 0;
}

static char *hendleRType(int funct, int rd, int rt, int rs, int opcode)
{
	struct
	{
		unsigned int a : 6;
		unsigned int b : 5;
		unsigned int c : 5;
		unsigned int d : 5;
		unsigned int e : 5;
		unsigned int f : 6;
	} x;
	x.a = 0;
	x.b = funct;
	x.c = rd;
	x.d = rt;
	x.e = rs;
	x.f = opcode;

	return printBits(sizeof(x), &x);
}

static char *hendleIType(int immed, int rt, int rs, int opcode)
{
	struct
	{
		unsigned int immed : 16;
		unsigned int rt : 5;
		unsigned int rs : 5;
		unsigned int opcode : 6;
	} x;
	x.immed = immed;
	x.rt = rt;
	x.rs = rs;
	x.opcode = opcode;
	return printBits(sizeof(x), &x);
}

static char *hendleITypeWithLabel(int address, char *immed, int rt, int rs, int opcode)
{
	struct
	{
		unsigned int rt : 5;
		unsigned int rs : 5;
		unsigned int opcode : 6;
	} x;

	x.rt = rt;
	x.rs = rs;
	x.opcode = opcode;
	
	return strcat(printBits((sizeof(x) - 2), &x), "?");
}

static char *hendleJType(int address, int opcode)
{
	struct
	{
		unsigned int address : 25;
		unsigned int reg : 1;
		unsigned int opcode : 6;
	} x;
	x.address = address;
	x.reg = 1;
	x.opcode = opcode;
	return printBits(sizeof(x), &x);
}

static char *hendleStopCommend()
{
	struct
	{
		unsigned int zero : 25;
		unsigned int reg : 1;
		unsigned int opcode : 6;
		
	} x;
	x.reg = 0;
	x.opcode = 63;
	x.zero = 0;
	return printBits(sizeof(x), &x);
}

static char *hendleJTypewithLbael(char *address, int opcode)
{

	char *res;
	struct
	{
		unsigned int reg : 1;
		unsigned int opcode : 6;
	} x;
	x.reg = 0;
	x.opcode = opcode;
	res = printBits(sizeof(x) - 3, &x) + 1;
	return strcat(res, "?");
}

char* presentNumberAsBinary(int size, int number)
{
	char *result = number<0? (complateToTwo(myItoa(number*-1,size),size)) : (myItoa(number,size));
	return result;
	
}



int getSizeBytsByGuidance(char * guidance)
{
	if(strcmp(guidance, ".db")==0)
	{
		return 8;
	}
	else if(strcmp(guidance, ".dh")==0)
	{
		return 16;
	}
	else if(strcmp(guidance, ".dw")==0)
	{
		return 32;
	}else{
		return 0;
	}
}

char** getBinCodesForGuidanceAsciz(char * word)
{
	int byteSize = 9;
	int i;
	char **result;
	int n;
	int counter = strlen(word)+1;
	
    	result = (char **)malloc(counter);
	for (i = 0; i < sizeof(word); i++)
	{
		n = word[i];
		result[i] = myItoa((int)n,byteSize);
	}
	result[i] = '\0';
	return result;
}

static char* myItoa(int val, int size){
	int tmpVal = val;
	char *res; 
	int i;	
	res = (char *)malloc(size);
	if(res)
	{
		for(i = size-1; i>-1; --i, tmpVal /= 2){
			res[i] = "01"[tmpVal % 2];
		}
		res[size]='\0';
		return res;		
	}else{
		printf("Error: Failed to allocate space");
		exit(0);
	}
	
}

static char* complateToTwo(char* code,int byteSize)
{
	
	int flag = 0;
	int i;
	char *result;
	result  = (char*)malloc(sizeof(code));
	if(result)
	{
		for(i = byteSize-1; i >-1; i--){
			if(flag == 1)
			{
				result[i] = (code[i] == '1')? '0':'1';
			}else{
				result[i] = code[i];

				flag = code[i] =='1'? 1:0;
			}
		}
		result[byteSize]= '\0';
		return result;
	}else{
		printf("Error: Failed to allocate space");
		exit(0);
	}
}


