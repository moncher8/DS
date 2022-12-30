#define SHOW_STEP 0 // 제출시 0
#define BALANCING 1 // 제출시 1

#include <stdlib.h>
#include <stdio.h>
#include <string.h> //strcmp, strdup

#define max(x, y)	(((x) > (y)) ? (x) : (y))

////////////////////////////////////////////////////////////////////////////////
// AVL_TREE type definition
typedef struct node
{
	char		*data;
	struct node	*left;
	struct node	*right;
	int			height;
} NODE;

typedef struct
{ 
	NODE	*root;
	int		count;  // number of nodes
} AVL_TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations    

/* Allocates dynamic memory for a AVL_TREE head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
AVL_TREE *AVL_Create( void);

/* Deletes all data in tree and recycles memory
*/
void AVL_Destroy( AVL_TREE *pTree);
static void _destroy( NODE *root);

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int AVL_Insert( AVL_TREE *pTree, char *data);

/* internal function
	This function uses recursion to insert the new data into a leaf node
	return	pointer to new root
*/
static NODE *_insert( NODE *root, NODE *newPtr);

static NODE *_makeNode( char *data);

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
char *AVL_Retrieve( AVL_TREE *pTree, char *key);

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve( NODE *root, char *key);

/* Prints tree using inorder traversal
*/
void AVL_Traverse( AVL_TREE *pTree);
static void _traverse( NODE *root);

/* Prints tree using inorder right-to-left traversal
*/
void printTree( AVL_TREE *pTree);
/* internal traversal function
*/
static void _infix_print( NODE *root, int level);

/* internal function
	return	height of the (sub)tree from the node (root)
*/
static int getHeight( NODE *root);

/* internal function
	Exchanges pointers to rotate the tree to the right
	updates heights of the nodes
	return	new root
*/
static NODE *rotateRight( NODE *root);

/* internal function
	Exchanges pointers to rotate the tree to the left
	updates heights of the nodes
	return	new root
*/
static NODE *rotateLeft( NODE *root);

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	AVL_TREE *tree;
	char str[1024];
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n", argv[0]);
		return 0;
	}
	
	// creates a null tree
	tree = AVL_Create();
	
	if (!tree)
	{
		fprintf( stderr, "Cannot create tree!\n");
		return 100;
	} 

	FILE *fp = fopen( argv[1], "rt");
	if (fp == NULL)
	{
		fprintf( stderr, "Cannot open file! [%s]\n", argv[1]);
		return 200;
	}

	while(fscanf( fp, "%s", str) != EOF)
	{

#if SHOW_STEP
		fprintf( stdout, "Insert %s>\n", str);
#endif		
		// insert function call
		AVL_Insert( tree, str);

#if SHOW_STEP
		fprintf( stdout, "Tree representation:\n");
		printTree( tree);
#endif
	}
	
	fclose( fp);


#if SHOW_STEP
	fprintf( stdout, "\n");

	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	AVL_Traverse( tree);
	fprintf( stdout, "\n");

	// print tree with right-to-left infix traversal
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
#endif
	fprintf( stdout, "Height of tree: %d\n", tree->root->height);
	fprintf( stdout, "# of nodes: %d\n", tree->count);
	
	// retrieval
	char *key;
	fprintf( stdout, "Query: ");
	while( fscanf( stdin, "%s", str) != EOF)
	{
		key = AVL_Retrieve( tree, str);
		
		if (key) fprintf( stdout, "%s found!\n", key);
		else fprintf( stdout, "%s NOT found!\n", str);
		
		fprintf( stdout, "Query: ");
	}
	
	// destroy tree
	AVL_Destroy( tree);

	return 0;
}



//////////////////my function definition//////////////////////////

/* Allocates dynamic memory for a AVL_TREE head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
AVL_TREE *AVL_Create( void)
{
	AVL_TREE *newTree;
	newTree=(AVL_TREE*)malloc(sizeof(AVL_TREE));
	newTree->root=NULL;
	newTree->count=0;
	return newTree;
}

/* Deletes all data in tree and recycles memory
*/
void AVL_Destroy( AVL_TREE *pTree)
{
	if(pTree){
		_destroy(pTree->root);
		free(pTree);
	}
}
static void _destroy( NODE *root)
{
	if(root==NULL)
		return;
	if(root){
		_destroy(root->left);
		free(root->data);
		_destroy(root->right);
		free(root);
	}
	return;
 
}

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int AVL_Insert( AVL_TREE *pTree, char *data)
{
	NODE *newNode;
	newNode=_makeNode(data);
	if(newNode)
	{
		pTree->root=_insert(pTree->root,newNode);
		(pTree->count)++;
		return 1;
	}
	return 0;
}

/* internal function
	This function uses recursion to insert the new data into a leaf node
	return	pointer to new root
*/
static NODE *_insert( NODE *root, NODE *newPtr)
{
	if (root==NULL)
		root=newPtr;
	if (strcmp(newPtr->data, root->data)<0)
		root->left = _insert(root->left, newPtr);
	else if (strcmp(newPtr->data, root->data)>0)
		root->right = _insert(root->right, newPtr);
	else
		return root;
	//if equal keys
	root->height=getHeight(root);
	int balance=getHeight(root->left)-getHeight(root->right);
	
	//not balanced cases
	//LL
	if(balance>1 && strcmp(newPtr->data,root->data)<0)
		return rotateRight(root);
	//RR
	if(balance<-1 && strcmp(newPtr->data,root->data)>0)
		return rotateLeft(root);
	//LR
	if (balance>1 && strcmp(newPtr->data,root->left->data)>0){
		root->left=rotateLeft(root->left);
		return rotateRight(root);
	}
	//Rl
	if(balance<-1 && strcmp(newPtr->data,root->left->data)<0){
		root->right=rotateRight(root->right);
		return rotateLeft(root);
	}
	return root;

}

static NODE *_makeNode( char *data)
{	
	NODE *newPtr;
	newPtr=(NODE*)malloc(sizeof(NODE));
	char *address=strdup(data);
	newPtr->data=address;
	newPtr->left=NULL;
	newPtr->right=NULL;
	newPtr->height=1;    
	
	return newPtr;
	
}

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
char *AVL_Retrieve( AVL_TREE *pTree, char *key)
{
	NODE *rkey;
	rkey=_retrieve(pTree->root,key);
	if(rkey)
		return rkey->data;
	else
		return NULL;
}

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve( NODE *root, char *key)
{	if (root){
		if(strcmp(key,root->data)>0)
			_retrieve(root->right,key);	
		else if(strcmp(key,root->data)<0)
			_retrieve(root->left,key);	
		else
			return root;
	}
	else
		return root;
	
}

/* Prints tree using inorder traversal
*/
void AVL_Traverse( AVL_TREE *pTree)
{
	_traverse(pTree->root);
	return;
}


static void _traverse( NODE *root)
{
	if(root->left!=NULL)
		_traverse(root->left);
	printf("%s ",root->data);
	if(root->right!=NULL)
		_traverse(root->right);
}

/* Prints tree using inorder right-to-left traversal
*/
void printTree( AVL_TREE *pTree)
{
	_infix_print(pTree->root,0); //infix print에서 level=1/0?
}
/* internal traversal function
*/
static void _infix_print( NODE *root, int level)
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
		printf("%s\n",root->data);
	}
	
	else if (root->left==NULL)
	{
		_infix_print(root->right,level+1);

		for (int i=0;i<level;i++)
		{
			printf("\t");
		}
		printf("%s\n",root->data);
	}

	else if(root->right==NULL)
	{	
		 
		for (int i=0;i<level;i++)
		{
			printf("\t");
		}
		printf("%s\n",root->data);
		_infix_print(root->left,level+1);

	}	
	else
	{	_infix_print(root->right,level+1);
		for (int i=0;i<level;i++)
		{
			printf("\t");
		}
		printf("%s\n",root->data);
		_infix_print(root->left,level+1);
	}
}

/* internal function
	return	height of the (sub)tree from the node (root)
*/
static int getHeight( NODE *root)
{
	if (root)
		return max(getHeight(root->left),getHeight(root->right))+1;
	else
		return 0;
	//return root->height;
}

/* internal function
	Exchanges pointers to rotate the tree to the right
	updates heights of the nodes
	return	new root
*/
static NODE *rotateRight( NODE *root)
{
	NODE *newroot;
	NODE *temp;
	newroot=root->left;
	temp=newroot->right;

	newroot->right=root;
	root->left=temp;

	newroot->height=getHeight(root);
	root->height=getHeight(root);

	return newroot;

}

/* internal function
	Exchanges pointers to rotate the tree to the left
	updates heights of the nodes
	return	new root
*/
static NODE *rotateLeft( NODE *root)
{
	NODE *newroot;
	NODE *temp;
	newroot=root->right;
	temp=newroot->left;

	newroot->left=root;
	root->right=temp;

	root->height=getHeight(root);
	newroot->height=getHeight(newroot);
	return newroot;

}