#include "main.h"


void addToTablelList(t_ptr *listHead, rowTable row)
{
	t_ptr curr = NULL,prev = NULL;
	t_ptr newNode;
	
	newNode = (t_ptr)malloc(sizeof(tableList));
	if(!newNode)
	{
		printf("\nthere are no pleace in the memory\n");
		exit(0);
	}
	newNode->data = row;
	newNode->next = NULL;
	
	curr = *listHead;

	while(curr)
	{
		prev = curr;
		curr = prev->next;
	}
	
	if(curr == *listHead)
		*listHead = newNode;
	else
		prev->next = newNode;
}

void printTableList(t_ptr h)
{
	if(h == NULL)
        	printf("\nList is empty.\n");
        else
	{
	printf("\n|\tAddress\t|\tSource Code\t|\tMachine Code\t|\tCommand Code\t|\tLable\t\n");
	
	while(h)
	{
		printf("---------------------------------------------------------");
		printf("\n|\t%d\t| %s | %s | %s | %s |\n", h->data.address, h->data.sourceCode, h->data.machineCode, h->data.commend , h->data.label);
		h=h->next;
	}
	printf("\n");
	}
}

void freeTableList(t_ptr * hptr)
{
	t_ptr p;
	while(*hptr)
	{
		p = *hptr;
		*hptr = (*hptr)->next;
		free(p);
	}
}



