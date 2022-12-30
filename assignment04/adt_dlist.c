#include <stdlib.h> // malloc

#include "adt_dlist.h"

/* internal insert function
	inserts data into a new node
	return	1 if successful
			0 if memory overflow
*/
static int _insert( LIST *pList, NODE *pPre, void *dataInPtr)
{
	NODE *new=(NODE*)malloc(sizeof(NODE));
	if(new==NULL)
		return 0;
	new->llink=NULL;
	new->rlink=NULL;
	new->dataPtr=dataInPtr;   //void pointer인데 cast 안해줘도 됨?
	if(pPre==NULL)
	{
		new->rlink=pList->head;
		if(pList->count==0)
		{
			pList->rear=new;
		}
		else
		{
			pList->head->llink = new;
		}
		pList->head=new;			
	}

	else
	{
		new->llink=pPre;
		if(pPre->rlink==NULL)
		{
			pList->rear = new;
		}
		else
		{
			pPre->rlink->llink = new;
			new->rlink=pPre->rlink;
		}
		pPre->rlink=new;
	}
	pList->count++;
}

/* internal delete function
	deletes data from a list and saves the (deleted) data to dataOut
*/
static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, void **dataOutPtr)
{
	*dataOutPtr=pLoc->dataPtr;
	if (pPre==NULL)	//맨 앞
	{
		pList->head=pLoc->rlink;
		pLoc->rlink->llink = pLoc->llink;   
	} 
	else	//중간,맨 끝
	{
		if(pLoc->rlink != NULL){
			pPre->rlink=pLoc->rlink;
			pLoc->rlink->llink = pPre;
		}else{
			pPre->rlink=pLoc->rlink;
			pList->rear = pPre;
		}
		

	}

	free(pLoc);
	pList->count--;

}

/* internal search function
	searches list and passes back address of node
	containing target and its logical predecessor
	return	1 found
			0 not found
*/
static int _search( LIST *pList, NODE **pPre, NODE **pLoc, void *pArgu)
{
	*pPre=NULL;
	*pLoc=pList->head;
	
	while((*pLoc)!=NULL)
	{
		//if ((cmpName(pArgu, (*pLoc)->dataPtr))==0)
		if (((*pList->compare)(pArgu, (*pLoc)->dataPtr))==0)

		{
			return 1;
			break;
		}
		else if (((*pList->compare)(pArgu, (*pLoc)->dataPtr))<0)
		{
			break;
		} 
		//else if (cmpName(pArgu, (*pLoc)->dataPtr)>0)
		{
			*pPre=*pLoc;
			*pLoc=(*pLoc)->rlink;   //디음 노드로 traverse
		}
	}

	return 0;

}


//................
LIST *createList( int (*compare)(const void *, const void *))
{
	LIST *myList=(LIST *)malloc(sizeof(LIST));
	myList->count=0;
	myList->head=NULL;
	myList->rear=NULL;
	myList->compare=compare; //compare의 초기화?
	return myList;



}
//........
void destroyList( LIST *pList, void (*callback)(void *))
{
	NODE *Del=NULL;
	NODE *Next=NULL;

	if (pList->count==0)
	{
		free(pList);  
	}

	for(Del=pList->head;Del!=NULL;Del=Next)
	{
		Next=Del->rlink;
		callback(Del->dataPtr);
		free(Del);
	}
	free(pList);


};
//........
int addNode( LIST *pList, void *dataInPtr, void (*callback)(const void *, const void *))
{
	NODE *pPre=NULL;
	NODE *pAdd=NULL; 

	//how to make it overflow? return 0
	int exist=_search(pList, &pPre, &pAdd, dataInPtr);
	if (exist==0)
	{
		if(!_insert(pList, pPre, dataInPtr))
			return 0;  //is it pPre or pADD
		return 1;
	}
	else
	{
		callback(pAdd->dataPtr, dataInPtr);  //dataOutPtr
		return 2;
	}

};

int removeNode( LIST *pList, void *keyPtr, void **dataOutPtr)
{

	NODE *pPre;
	NODE *pRem;

	int exist=_search(pList, &pPre, &pRem,keyPtr);

	if (exist==1)
	{	_delete(pList,pPre,pRem,dataOutPtr);
		 return 1;
	}
	else return 0;
};

int searchList( LIST *pList, void *pArgu, void **dataOutPtr)
{

	NODE *pPre;
	NODE *pSearch;
	//dataOutPtr=stores the found data;
	int exist=_search(pList, &pPre, &pSearch,pArgu);
	if (exist) 
	{
		*dataOutPtr=pSearch->dataPtr;
		
	}
	else return 0;

};

int countList( LIST *pList)
{
	return pList->count;

};

//int emptyList( LIST *pList);

void traverseList( LIST *pList, void (*callback)(const void *))
{
	NODE *pTemp=pList->head;
	while(pTemp!=NULL)
	{
		callback(pTemp->dataPtr);
		pTemp=pTemp->rlink;

	}

};

void traverseListR( LIST *pList, void (*callback)(const void *))
{
	NODE *pTemp=pList->rear;
	while(pTemp!=NULL)
	{
		callback(pTemp->dataPtr);
		pTemp=pTemp->llink;

	}

};
