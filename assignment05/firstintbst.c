#include <stdlib.h> // malloc, atoi, rand
#include <stdio.h>
#include <assert.h>
#include <time.h> // time

#define RANDOM_INPUT   1
#define FILE_INPUT      2

// 왼쪽과 오른쪽 서브트리를 모두 가지고 있는 노드를 삭제하는 경우: 오른쪽 서브트리의 가장 작은 값을 가진 노드가 삭제될 노드의 자리로 위치

////////////////////////////////////////////////////////////////////////////////
// TREE type definition
typedef struct node
{
   int         data;
   struct node   *left;
   struct node   *right;
} NODE;

typedef struct
{
   NODE   *root;
} TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a tree head node and returns its address to caller
   return   head node pointer
         NULL if overflow
*/
// 빈 트리 만들기, datanode는 없고 headnode만, headnode 주소 return
TREE *BST_Create( void);

/* Deletes all data in tree and recycles memory
*/
// datanode 다 해제, headnode까지도, 
void BST_Destroy( TREE *pTree);

/* internal function (not mandatory)
*/
static void _destroy( NODE *root);

/* Inserts new data into the tree
   return   1 success
         0 overflow
*/
//datanode만들고
int BST_Insert( TREE *pTree, int data);

/* internal function (not mandatory)
*/
static void _insert( NODE *root, NODE *newPtr);

//이게 datanode만드는 함수
NODE *_makeNode( int data);

/* Deletes a node with dltKey from the tree
   return   1 success
         0 not found
*/
// 삭제할 키를 받아서 삭제
int BST_Delete( TREE *pTree, int dltKey);

/* internal function
   success is 1 if deleted; 0 if not
   return   pointer to root
*/
//rootnode부터 시작해서 key를 찾아가는.
static NODE *_delete( NODE *root, int dltKey, int *success);

/* Retrieve tree for the node containing the requested key
   return   address of data of the node containing the key
         NULL not found
*/
 
int *BST_Retrieve( TREE *pTree, int key);

/* internal function
   Retrieve node containing the requested key
   return   address of the node containing the key
         NULL not found
*/
static NODE *_retrieve( NODE *root, int key);

/* prints tree using inorder traversal
*/
void BST_Traverse( TREE *pTree);
static void _traverse( NODE *root);

/* Print tree using inorder right-to-left traversal
*/
void printTree( TREE *pTree);
/* internal traversal function
*/
static void _inorder_print( NODE *root, int level);

/* 
   return 1 if the tree is empty; 0 if not
*/
int BST_Empty( TREE *pTree);

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
   int mode; // input mode
   TREE *tree;
   int data;
   
   if (argc != 2) // argument 개수가 맞지 않으면
   {
      fprintf( stderr, "usage: %s FILE or %s number\n", argv[0], argv[0]);
      return 1;
   }
   
   FILE *fp;
   
   if ((fp = fopen(argv[1], "rt")) == NULL)
   {
      mode = RANDOM_INPUT;
   }
   else mode = FILE_INPUT;
   
   // creates a null tree
   tree = BST_Create();
   
   if (!tree)
   {
      printf( "Cannot create a tree!\n");
      return 100;
   }

   if (mode == RANDOM_INPUT)
   {
      int numbers;
      numbers = atoi(argv[1]);
      assert( numbers > 0);

      fprintf( stdout, "Inserting: ");
      
      srand( time(NULL));
      for (int i = 0; i < numbers; i++)
      {
         data = rand() % (numbers*3) + 1; // random number (1 ~ numbers * 3)
         
         fprintf( stdout, "%d ", data);
			
			// insert function call
			int ret = BST_Insert( tree, data);
			if (!ret) break;
		}
	}
	else if (mode == FILE_INPUT)
	{
		fprintf( stdout, "Inserting: ");
		
		while (fscanf( fp, "%d", &data) != EOF)
		{
			fprintf( stdout, "%d ", data);
			
			// insert function call
			int ret = BST_Insert( tree, data);
			if (!ret) break;
		}
		fclose( fp);
	}
	
	fprintf( stdout, "\n");

	if (BST_Empty( tree))
	{
		fprintf( stdout, "Empty tree!\n");
		BST_Destroy( tree);
		return 0;
	}	

	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	BST_Traverse( tree);
	fprintf( stdout, "\n");
	
	// print tree with right-to-left inorder traversal
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
	
	/*
	while (1)
	{
		fprintf( stdout, "Input a number to delete: "); 
		int num;
		if (scanf( "%d", &num) == EOF) break;
		
		//int ret = BST_Delete( tree, num);
		if (!ret) 
		{
			fprintf( stdout, "%d not found\n", num);
			continue;
		}
		
		// print tree with right-to-left inorder traversal
		fprintf( stdout, "Tree representation:\n");
		//printTree(tree);
		
		//if (BST_Empty( tree))
		{
			fprintf( stdout, "Empty tree!\n");
			break;
		}
	}
	
	//BST_Destroy( tree);
*/
	return 0;
}

///////////////////////////////////////my function defintion///////////////////////////////////////////////////////////
TREE *BST_Create( void) //빈 트리에 head node만 생성 return 주소
{
	TREE *newTree;
	newTree=(TREE*)malloc(sizeof(TREE));
	newTree->root=NULL;       //*
	return newTree;
} 


/* Deletes all data in tree and recycles memory
*/
/*void BST_Destroy( TREE *pTree)
{
	NODE *ancestor;
	pTree->root=ancestor;
	//NODE *Del;
	//NODE *next;
	if (ancestor==NULL)
	{
		free(pTree);
	}

	else
	{
		while(ancestor!=NULL)
		{
			_destroy(ancestor->left);
			_destroy(ancestor->right);

		}
		free(ancestor);
		free(pTree);


	}

}

/* internal function (not mandatory)
*/
/*
static void _destroy( NODE *root)
{
	free(root->data);
	free(root);
	
}  //recursively data를 삭제
*/


/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int BST_Insert( TREE *pTree, int data) //data 값을 주면서 node
{
	NODE *newNode;//=(NODE*)malloc(sizeof(NODE));
	newNode=_makeNode(data);
	NODE *temp=pTree->root;
	if (pTree->root==NULL) return 0;

	else
	{
		_insert(temp,newNode);
		return 1;
	}
	
}

/* internal function (not mandatory)
*/
static void _insert( NODE *root, NODE *newPtr)
{//node를 적절한 위치에 삽입하기 위한 위치 return
 
	//NODE *new=(NODE*)malloc(sizeof(NODE));
	
	//new=root;
	if(root->data>newPtr->data)
	{
		_insert(root->left,newPtr);
	}
	else if (root->data<newPtr->data)
	{
		_insert(root->right,newPtr);
	}


	}


NODE *_makeNode( int data)
{
	NODE *newnode=(NODE *)malloc(sizeof(NODE));
	newnode->data=data;
	newnode->left=NULL;
	newnode->right=NULL;
	return newnode;
}


/* Deletes a node with dltKey from the tree
	return	1 success
			0 not found
*/

int BST_Delete( TREE *pTree, int dltKey)
{
	NODE *delNode;
	int pf=0;
	delNode=_delete(pTree->root,dltKey,pf);
	if(pf)
	{
		pTree->root=delNode;
	}
	
	return pf;
}


/* internal function
	*success is 1 if deleted; 0 if not
	return	pointer to root
*/

static NODE *_delete( NODE *root, int dltKey, int *success)
 //삭제할 때 경우의 수가 여러 개 있음
//root노드가 바뀌더라도 밖에서 바로 알 수 있게 root node is a pointer
{
	
	if (root==NULL) return root;
	if (root->data>dltKey)
	{
		root->left=_delete(root->left,dltKey,0);
	}
	else if (root->data<dltKey)
	{
		root->right=_delete(root->right,dltKey,0);

	}
	else //삭제하는 경우
	{
		//자녀가 한개
		if (root->right==NULL)
		{
			NODE *temp=root->left;
			free(root);
			return temp;
		}
		else if(root->left==NULL)
		{
			NODE *temp=root->right;
			free(root);
			return temp;
		}
		//node의 자녀가 두개면 오른쪽 서브트리에서 가장 작은 수 
		else
		{
			NODE *now=root->right;
			while(root&&root->left!=NULL)
			{
				root=root->left;
			}
			NODE *temp=now;
			root->data=temp->data;
			root->right=_delete(root->right,temp->data,1);
			*success=1;
		}
		return root;
	}
}




/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
/*
int *BST_Retrieve( TREE *pTree, int key)
{
	while (pTree->root!=NULL && pTree->root->data!=key)
	{
		if ((pTree->root->data)>key)
		{
			pTree->root=pTree->root->left;
		}
		else if((pTree->root->data)<key)
		{
			pTree->root=pTree->root->right;
		}
		else pTree->root=NULL;

	}
	return pTree->root;
}
*/

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found

	static NODE *_retrieve( NODE *root, int key);
*/


/* prints tree using inorder traversal
*/

void BST_Traverse( TREE *pTree)
{
	NODE *root;
	root=pTree->root;
	while(root!=NULL)
	{
		_traverse(root->left);
		_traverse(root);
		_traverse(root->right);
	}
}
static void _traverse( NODE *root)
{
	printf("%d",root->data);
}


/* Print tree using inorder right-to-left inorder traversal
*/
/*
void printTree( TREE *pTree)
{
	NODE *node=(NODE*)malloc(sizeof(NODE));
	node=pTree->root;
	node=inorder_print(node,0);
} //level 만큼 tab
/* internal traversal function
*/ 
/*
static void _inorder_print( NODE *root, int level)
{
	if(root==NULL)
	{
		return;
	}
	
	_inorder_print(root->right,level);
	for 문level++;
	printf("\t"*level"%d",root->data);
	_inorder_print(root->left,level);
	level++;

	}
	

}*/

/* 
	return 1 if the tree is empty; 0 if not
*/
int BST_Empty( TREE *pTree)
{
	if (pTree->root==NULL)
	{
		return 1;
	}
	else return 0;
}
