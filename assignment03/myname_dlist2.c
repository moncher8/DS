 #include <stdlib.h> // malloc
#include <string.h> // strchr
#include <stdio.h>
#include <string.h> // strdup, strcmp
#include <ctype.h> // toupper

#define QUIT			1
#define FORWARD_PRINT	2
#define BACKWARD_PRINT	3
#define SEARCH			4
#define DELETE			5
#define COUNT			6

// User structure type definition
typedef struct 
{
	char	*name;
	char	sex;
	int		freq;
} tName;

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	tName		*dataPtr;  //이름 구조체를 가리키는 포인터
	struct node	*llink; //predecessor
	struct node	*rlink;  //successor
} NODE;

typedef struct
{
	int		count;  
	NODE	*head;  
	NODE	*rear;
} LIST;  //head 정보

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations
LIST *createList(void);
void destroyList( LIST *pList);
int addNode( LIST *pList, tName *dataInPtr);
int removeNode( LIST *pList, tName *keyPtr, tName **dataOutPtr);
int searchList( LIST *pList, tName *pArgu, tName **dataOutPtr);
int countList( LIST *pList);
int emptyList( LIST *pList);
void traverseList( LIST *pList, void (*callback)(const tName *));
void traverseListR( LIST *pList, void (*callback)(const tName *));
static int _insert( LIST *pList, NODE *pPre, tName *dataInPtr);
static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, tName **dataOutPtr);
static int _search( LIST *pList, NODE **pPre, NODE **pLoc, tName *pArgu);
tName *createName( char *str, char sex, int freq); 
void destroyName( tName *pNode);

/////////////////////////////////////////////////////////////////////////////////
/* Allocates dynamic memory for a list head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/

////////////////////////////////////////////////////////////////////////////////
/* gets user's input
*/
int get_action()
{
	char ch;
	scanf( "%c", &ch);
	ch = toupper( ch);   
	switch( ch)
	{
		case 'Q':
			return QUIT;
		case 'P':
			return FORWARD_PRINT;
		case 'B':
			return BACKWARD_PRINT;
		case 'S':
			return SEARCH;
		case 'D':
			return DELETE;
		case 'C':
			return COUNT;
	}
	return 0; // undefined action
}

// compares two names in name structures
// for createList function
int cmpName( const tName *pName1, const tName *pName2)
{
	int ret = strcmp( pName1->name, pName2->name);
	if (ret == 0) return pName1->sex - pName2->sex;
	else return ret;
}

// prints contents of name structure
// for traverseList and traverseListR functions
void print_name(const tName *dataPtr)
{
	printf( "%s\t%c\t%d\n", dataPtr->name, dataPtr->sex, dataPtr->freq);
}

// increases freq in name structure
// for addNode function
void increase_freq(tName *dataOutPtr, const tName *dataInPtr)
{
	dataOutPtr->freq += dataInPtr->freq;
}

// splits name and sex from string
// ex) "Zoe/M"
void split_name_sex(char *str, char *sex)
{
	char *p;
	
	p = strchr(str, '/');
	if (p == NULL) 
	{
		fprintf( stderr, "unexpected name/sex format\n");
		return;
	}
	else
	{
		*p++ = 0;
		*sex = *p;
	}
}
////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	LIST *list;
	
	char str[1024];
	char name[100];
	char sex;
	int freq;
	
	tName *pName;
	int ret;
	FILE *fp;
	
	if (argc != 2){
		fprintf( stderr, "usage: %s FILE\n", argv[0]);
		return 1;
	}
	
	fp = fopen( argv[1], "rt");
	if (!fp)
	{
		fprintf( stderr, "Error: cannot open file [%s]\n", argv[1]);
		return 2;
	}
	
	// creates an empty list
	//list = createList();
	if (!list)
	{
		printf( "Cannot create list\n");
		return 100;
	}
	
	while(fscanf( fp, "%*d\t%s\t%c\t%d", str, &sex, &freq) != EOF)
	{
		//pName = createName( str, sex, freq);
		
		//ret = addNode( list, pName);
		
		//if (ret == 2) // duplicated
		{
			//destroyName( pName);  //이미 그 이름은 존재, freq만 업테이트함
		}
	}
	
	fclose( fp);
	
	fprintf( stderr, "Select Q)uit, P)rint, B)ackward print, S)earch, D)elete, C)ount: ");
	
	while (1)
	{
		tName *p;
		int action = get_action();
		
		switch( action)
		{
			case QUIT:
				//destroyList( list);
				return 0;
			
			case FORWARD_PRINT:
				//traverseList( list, print_name);
				break;
			
			case BACKWARD_PRINT:
				//traverseListR( list, print_name);
				break;
			
			case SEARCH:
				fprintf( stderr, "Input a name/sex to find: ");
				fscanf( stdin, "%s", str);
				split_name_sex(str, &sex);
				
				//pName = createName( str, sex, 0);

				//if (searchList( list, pName, &p)) print_name( p);
				//else fprintf( stdout, "%s not found\n", str);
				
				//destroyName( pName);
				break;
				
			case DELETE:
				fprintf( stderr, "Input a name/sex to delete: ");
				fscanf( stdin, "%s", str);
				split_name_sex(str, &sex);
				
				//pName = createName( str, sex, 0);

				//if (removeNode( list, pName, &p))
				{
					//fprintf( stdout, "(%s, %c, %d) deleted\n", p->name, p->sex, p->freq);
					//destroyName( p);
				}
				//else fprintf( stdout, "%s not found\n", str);
				
				//destroyName( pName);
				break;
			
			case COUNT:
				//fprintf( stdout, "%d\n", countList( list));
				break;
		}
		
		if (action) fprintf( stderr, "Select Q)uit, P)rint, B)ackward print, S)earch, D)elete, C)ount: ");
	}
	return 0;
}
 
 
////////////////my function declaration//////////////////////////
LIST *createList(void)
{
	LIST *myList=(LIST *)malloc(sizeof(LIST));
	myList->count=0;
	myList->head=NULL;
	myList->rear=NULL;
	return myList;


}; 

/* Deletes all data in list and recycles memory
*/
void destroyList( LIST *pList)
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
		free(Del);
	}
	free(pList);

};

/* Inserts data into list
	return	0 if overflow
			1 if successful
			2 if duplicated key
*/
int addNode( LIST *pList, tName *dataInPtr)
{
	//노드가 추가될 위치,preceding node에 대한 포인터
	//use _search, _insert 함수
	NODE *pPre=NULL;
	NODE *pAdd=NULL;


	//how to make it overflow? return 0
	int exist=_search(pList, &pPre, &pAdd, dataInPtr);
	if (exist==0)
	{
		_insert(pList, pPre, dataInPtr);  //is it pPre or pADD
		return 1;
	}
	else
	{
		increase_freq(pAdd->dataPtr, dataInPtr);  //dataOutPtr
		return 2;
	}



};




/* Removes data from list
	return	0 not found
			1 deleted
*/
int removeNode( LIST *pList, tName *keyPtr, tName **dataOutPtr)
{
	 NODE *pPre;
	 NODE *pRem;

	 int exist=_search(pList, &pPre, &pRem, &dataOutPtr);

	 if (exist==1)
	{	_delete(pList,&pPre,&pRem,&dataOutPtr);
		 return 1;
	}
	else return 0;


};


/* interface to search function
	Argu	key being sought
	dataOut	contains found data
	return	1 successful
			0 not found
*/
int searchList( LIST *pList, tName *pArgu, tName **dataOutPtr)
{
	NODE *pPre;
	NODE *pSearch;
	//dataOutPtr=stores the found data;
	exist=_search(pList, pPre, pSearch,pArgu);
	if (exist) return 1;
	else return 0;

};



/* returns number of nodes in list
*/
int countList( LIST *pList)
{
	return pList->count;
};
//count 

/* returns	1 empty
			0 list has data
*/
int emptyList( LIST *pList); //구현안해도 됨



/* traverses data from list (forward)
*/
void traverseList( LIST *pList, void (*callback)(const tName *))
{
	NODE *pTemp=pList->head;
	while(pTemp!=NULL)
	{
		callback(pTemp->dataPtr);
		pTemp=pTemp->rlink;

	}
	
};




/* traverses data from list (backward)
*/
void traverseListR( LIST *pList, void (*callback)(const tName *))
{

	NODE *pTemp=pList->rear;
	while(pTemp!=NULL)
	{
		callback(pTemp->dataPtr);
		pTemp=pTemp->llink;

	}
};

/* internal insert function
	inserts data into a new node
	return	1 if successful
			0 if memory overflow
*/
static int _insert( LIST *pList, NODE *pPre, tName *dataInPtr)
{
	NODE *new=(NODE*)malloc(sizeof(NODE));
	new->rlink=NULL;
	if(pPre==NULL)
	{
		pList->head=new;
		new->dataPtr=dataInPtr;	
	}

	else
	{
		new->llink=pPre;
		new->rlink=pPre->llink;
		pPre->rlink->llink=new;
		before->rlink=new;   //what is before?
		new->dataPtr=dataInPtr;
		

	}
	pList->count++;

};




/* internal delete function
	deletes data from a list and saves the (deleted) data to dataOut
*/
static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, tName **dataOutPtr)
{
	//pLoc==없애는 노드
	//check the delete par
	*dataOutPtr=pLoc->dataPtr;
	if (pPre==NULL)
	{
		pList->head=pLoc->rlink;   
	} 

	else
	{
		pLoc->llink->rlink=pLoc->rlink;
		pLoc->rlink->llink=pLoc->llink;
	}

	free(pLoc);
	pList->head--;



};


/* internal search function
	searches list and passes back address of node
	containing target and its logical predecessor
	return	1 found
			0 not found
*/
static int _search( LIST *pList, NODE **pPre, NODE **pLoc, tName *pArgu)
{
	*pPre=NULL;
	*pLoc=pList->head;
	
	while(*pLoc!=NULL)
	{
		if (cmpName(pArgu, (*pLoc)->dataPtr)==0) 
		{
			return 0;
			break;
		}
		else if (cmpName(pArgu, (*pLoc)->dataPtr)<0) 
		{
			return 1;
			break;
		} 
		else if (cmpName(pArgu, (*pLoc)->dataPtr)>0)
		{
			pPre=pLoc;
			pLoc=pLoc->rlink;   //디음 노드로 traverse
		}
	}



};
//main 함슈에서 addNode 할 때 어디에 삽입할지를 위한, remove node
////////////////////////////////////////////////////////////////////////////////
/* Allocates dynamic memory for a name structure, initialize fields(name, freq) and returns its address to caller
	return	name structure pointer
			NULL if overflow
*/
tName *createName( char *str, char sex, int freq)
{
	tName *pName=(tName *)malloc(sizeof(tName));

	pName->name=(*str);
	pName->freq=freq;
	pName->sex=sex;

	return pName;

}; 




/* Deletes all data in name structure and recycles memory
*/
void destroyName( tName *pNode)
{
	NODE *names;
	NODE *destroy;
	LIST *pList;
	for (destroy=pList->head;destroy!=NULL;destroy=names)
	{
		destroy->dataPtr=pNode->name;
		//destroy=names->rlink;
		names=destroy->rlink;
		free(destroy);
	}

	free(pNode);

};

