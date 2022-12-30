#include <stdlib.h> // malloc, atoi, rand
#include <stdio.h>
#include <assert.h>
#include <time.h> // time

#define RANDOM_INPUT	1
#define FILE_INPUT		2

////////////////////////////////////////////////////////////////////////////////
// TREE type definition
typedef struct node
{
	int			data;
	struct node	*left;
	struct node	*right;
} NODE;

typedef struct
{
	NODE	*root;
} TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a tree head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
TREE *BST_Create( void);

/* Deletes all data in tree and recycles memory
*/
void BST_Destroy( TREE *pTree);

/* internal function (not mandatory)
*/
static void _destroy( NODE *root);

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int BST_Insert( TREE *pTree, int data);

/* internal function (not mandatory)
*/
static void _insert( NODE *root, NODE *newPtr);

NODE *_makeNode( int data);

/* Deletes a node with dltKey from the tree
	return	1 success
			0 not found
*/
int BST_Delete( TREE *pTree, int dltKey);

/* internal function
	success is 1 if deleted; 0 if not
	return	pointer to root
*/
static NODE *_delete( NODE *root, int dltKey, int *success);

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
int *BST_Retrieve( TREE *pTree, int key);

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
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
	
	if (argc != 2)
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


	
	while (1)
	{
		fprintf( stdout, "Input a number to delete: "); 
		int num;
		if (scanf( "%d", &num) == EOF) break;
		
		int ret = BST_Delete( tree, num);
		if (!ret)
		{
			fprintf( stdout, "%d not found\n", num);
			continue;
		}
		
		// print tree with right-to-left inorder traversal
		fprintf( stdout, "Tree representation:\n");
		printTree(tree);
		
		if (BST_Empty( tree))
		{
			fprintf( stdout, "Empty tree!\n");
			break;
		}
	}
	
	BST_Destroy( tree);

	return 0;
}
///////////////////////////////////////my function defintion///////////////////////////////////////////////////////////

TREE *BST_Create(void) //빈 트리에 head node만 생성 return 주소
{
	TREE *newTree;
	newTree=(TREE*)malloc(sizeof(TREE));
	newTree->root=NULL;       
	return newTree;
} 
/* Deletes all data in tree and recycles memory
*/
void BST_Destroy( TREE *pTree)
{
	if(pTree)
	{
		_destroy(pTree->root);
		free(pTree);
		
	}	

}

/* internal function (not mandatory)
*/

static void _destroy( NODE *root)
{
	if(root==NULL)
		return;
	if(root)
	{
		_destroy(root->left);
		_destroy(root->right);
		free(root);
	}
	return; 
	//root->data는 어떻게 삭제? dataPtr이 아닌데	
} 


/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int BST_Insert( TREE *pTree, int data) //data 값을 주면서 node
{
	NODE *newNode;//=(NODE*)malloc(sizeof(NODE));
	newNode=_makeNode(data);
	//NODE *temp=pTree->root;
	if (pTree->root==NULL) 
	{
		pTree->root=newNode;
		return 1;
	}

	else
	{
		_insert(pTree->root,newNode);
		return 1;
	}

	return 0;
	
}

static void _insert( NODE *root, NODE *newPtr)
{//node를 적절한 위치에 삽입하기 위한 위치 return
 
	if(root->data>newPtr->data)
	{
		if(root->left == NULL)
			root->left = newPtr;
		else
			_insert(root->left,newPtr);
	}
	else if (root->data<=newPtr->data)
	{	
		if(root->right==NULL)
		{
			root->right=newPtr;
		}
		else
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


void BST_Traverse( TREE *pTree)
{
	_traverse(pTree->root);
	
}
static void _traverse( NODE *root)
{
	if(root->left!=NULL)
	{
		_traverse(root->left);
	}
	printf("%d ",root->data);
	if(root->right!=NULL)
	{	_traverse(root->right);
	}
	
}
/* Print tree using inorder r3ight-to-left inorder traversal
*/

void printTree( TREE *pTree)
{
	_inorder_print(pTree->root,0);
} //level 만큼 tab
/* internal traversal function
*/ 

static void _inorder_print( NODE *root, int level)
{
	if(root==NULL)
	{
		return;
	}
	else if(root->right==NULL && root->left==NULL)
	{
		for (int i=0;i<level;i++)
		{
			printf("\t");
		}
		printf("%d\n",root->data);
	}
	
	else if (root->left==NULL)
	{
		_inorder_print(root->right,level+1);

		for (int i=0;i<level;i++)
		{
			printf("\t");
		}
		printf("%d\n",root->data);
	}

	else if(root->right==NULL)
	{	
		 
		for (int i=0;i<level;i++)
		{
			printf("\t");
		}
		printf("%d\n",root->data);
		_inorder_print(root->left,level+1);

	}	
	else
	{	_inorder_print(root->right,level+1);
		for (int i=0;i<level;i++)
		{
			printf("\t");
		}
		printf("%d\n",root->data);
		_inorder_print(root->left,level+1);
	}
}


int BST_Empty( TREE *pTree)
{
	if (pTree->root==NULL)
	{
		return 1;
	}
	else return 0;
}



/* Deletes a node with dltKey from the tree
	return	1 success
			0 not found
*/


int BST_Delete( TREE *pTree, int dltKey)
{
	int success;
	if(dltKey==pTree->root->data)
		pTree->root=_delete(pTree->root,dltKey,&success);
	else
		_delete(pTree->root,dltKey,&success);
	return success;
}



/* internal function
	*success is 1 if deleted; 0 if not
	return	pointer to root

*/

static NODE *_delete( NODE *root, int dltKey, int *success)
 //삭제할 때 경우의 수가 여러 개 있음
//root노드가 바뀌더라도 밖에서 바로 알 수 있게 root node is a pointer
{
	
	if (root==NULL) 
	{	
		(*success)=0;
		return root;
	}
	if (root->data>dltKey)
	{
		root->left=_delete(root->left,dltKey,success);
	}
	else if (root->data<dltKey)
	{
		root->right=_delete(root->right,dltKey,success);

	}
	else //삭제하는 경우
	{
		//자녀가 없음
		if(root->right==NULL&&root->left==NULL)
		{
			free(root);
			(*success)=1;
			return NULL;
		}
		//자녀가 한개
		else if (root->right==NULL || root->left==NULL)
		{
			NODE *temp;
			if(root->right==NULL) 
				temp=root->left;
			else
				temp=root->right;
			free(root);
			(*success)=1;
			return temp;
		}
		//two children 
		//node의 자녀가 두개면 오른쪽 서브트리에서 가장 작은 수 
		else 
		{
			NODE *exchange=root->right;
			while(exchange->left)
				exchange=exchange->left;
			int holdPtr=root->data;
			root->data=exchange->data;
			exchange->data=holdPtr;
			root->right=_delete(root->right,exchange->data,success);
		}
	
		
	}
	return root;
}