
#include "main.h"


void addToSymbolList(ptr *listHead, symbol s)
{
	ptr curr=NULL,prev=NULL;
	ptr newNode;
	
	newNode = (ptr)malloc(sizeof(symbolList));
	if(!newNode)
	{
		printf("\nthere are no plecae in the memory\n");
		exit(0);
	}
	newNode->data = s;
	newNode->next=NULL;
	
	curr = *listHead;

	while(curr)
	{
		prev = curr;
		curr = curr->next;
	}
	
	if(curr == *listHead)
		*listHead = newNode;
	else
		prev->next = newNode;
}

void printSymbolList(ptr h)
{
	int i;
	if(h == NULL)
        	printf("\nList is empty.\n");
        else
	{
	printf("\n|\tSymbol\t|\tvalue\t|\tattribute\t|\n");
	
	while(h)
	{
		printf("---------------------------------------------------------");
		i = 0;
		printf("\n|\t%s\t|\t%d\t", h->data.name, h->data.value);
		printf("|");
		for(i=0; i< 2; i++){
			if(h->data.attributes[i] != NULL){
				printf("   %s   ",h->data.attributes[i]);
			}
		}
		printf("|\n");
		h=h->next;
	}
	printf("\n");
	}
}

void freeSymbolList(ptr * hptr)
{
	ptr p;
	while(*hptr)
	{
		p = *hptr;
		*hptr = (*hptr)->next;
		free(p);
	}
}

int getValueBySmboleName(ptr listHead, char * name)
{
	if(listHead == NULL)
        	printf("\nList is empty.\n");
	while(listHead)
	{
		if((strcmp(listHead->data.name,name))==0)
			return listHead->data.value;
		else
			listHead = listHead->next;
	}
	return 0;
}

int isSymbolExists(ptr listHead, char * symbol)
{
	if(listHead == NULL){
		return 1;
	}
	while(listHead)
	{
		if((strcmp(listHead->data.name,symbol))==0)
			return 0;
		else
			listHead = listHead->next;
	}
	return 1;
}

int hasExternAttribute(ptr listHead, char *symbol)
{
	int i;
	if(listHead == NULL)
        	printf("\nList is empty.\n");
	while(listHead)
	{
		if((strcmp(listHead->data.name,symbol))==0){
			for(i = 0; i<2; i++)
			{
				if(strcmp(listHead->data.attributes[i],"extern")==0)
				{
					return 1;
				}
			}
			return 0;
		}
		else
			listHead = listHead->next;
	}
	return 0;

}

void addAttributeToSymbol(ptr listHead, char * symbol, char* attr)
{
	if(listHead == NULL)
        	printf("\nList is empty.\n");
	while(listHead)
	{
		if((strcmp(listHead->data.name,symbol))==0)
		{
			strcpy(listHead->data.attributes[1] ,attr);
			listHead->data.attrNum = 2;
			listHead = listHead->next;
		}else{
			listHead = listHead->next;
		}
	}

}


