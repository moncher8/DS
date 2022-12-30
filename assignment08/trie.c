#include <stdio.h>
#include <stdlib.h>	// malloc
#include <string.h>	// strdup
#include <ctype.h>	// isupper, tolower

#define MAX_DEGREE	27 // 'a' ~ 'z' and EOW
#define EOW			'$'	// end of word 

// used in the following functions: trieInsert, trieSearch, triePrefixList
#define getIndex(x)		(((x) == EOW) ? MAX_DEGREE-1 : ((x) - 'a'))
//getIndex is for subtrees[MAX_Degree]어디로 분기해야 할까?


// TRIE type definition
typedef struct trieNode {
	int 			index; // -1 (non-word), 0, 1, 2, ...
	struct trieNode	*subtrees[MAX_DEGREE];
} TRIE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations
TRIE *trieCreateNode(void);
void trieDestroy( TRIE *root);
int trieInsert( TRIE *root, char *str, int dic_index);
int trieSearch( TRIE *root, char *str);
void trieList( TRIE *root, char *dic[]);
void triePrefixList( TRIE *root, char *str, char *dic[]);
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	TRIE *trie;
	char *dic[100000];

	int ret;
	char str[100];
	FILE *fp;
	int index = 0;
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n", argv[0]);
		return 1;
	}
	
	fp = fopen( argv[1], "rt");
	if (fp == NULL)
	{
		fprintf( stderr, "File open error: %s\n", argv[1]);
		return 1;
	}
	
	trie = trieCreateNode();
	
	while (fscanf( fp, "%s", str) != EOF)
	{
		ret = trieInsert( trie, str, index);

		if (ret)  
			dic[index++] = strdup( str);
	}
	
	fclose( fp);
	
	printf( "\nQuery: ");
	while (fscanf( stdin, "%s", str) != EOF)
	{
		// wildcard search(trailing search)
		if (str[strlen(str)-1] == '*')
		{
			str[strlen(str)-1] = 0;
			triePrefixList( trie, str, dic);
		}
		// keyword search
		else
		{
			ret = trieSearch( trie, str);
			if (ret == -1) printf( "[%s] not found!\n", str);
			else printf( "[%s] found!\n", dic[ret]);
		}
		
		printf( "\nQuery: ");
	}
	
	for (int i = 0; i < index; i++)
		free( dic[i]);
	
	trieDestroy( trie);
	
	return 0;
}
///////////////////////my function defintion/////////////////////
TRIE *trieCreateNode(void)//TRIE index==-1 and subtrees는 다 NULL로 초기화
{
	TRIE *root=(TRIE*)malloc(sizeof(TRIE));
	root->index=-1;
	for(int i=0;i<MAX_DEGREE;i++)
		root->subtrees[i]=NULL;
	return root;
}


/* Deletes all data in trie and recycles memory
*/
void trieDestroy( TRIE *root)
{
	for(int i=0;i<root->index;i++){
		free(root->subtrees[i]);
	}
}

// used in the following functions: trieInsert, trieSearch, triePrefixList
//getIndex is for subtrees[MAX_Degree]어디로 분기해야 할까?

/* Inserts new entry into the trie
	return	1 success
			0 failure
*/
// 주의! 엔트리를 중복 삽입하지 않도록 체크해야 함
// 대소문자를 소문자로 통일하여 삽입
// 영문자와 EOW 외 문자를 포함하는 문자열은 삽입하지 않음
int trieInsert( TRIE *root, char *str, int dic_index) //dic_index 마지막 node 사전의 index
{
	//중복 삽입 금지
	for(int i=0;i<strlen(str);i++){
		if(isupper(str[i]))
			str[i]=tolower(str[i]);
		int index=getIndex(str[i]);
		if (root->subtrees[index]==NULL){
			root->subtrees[index]=trieCreateNode();
			root=root->subtrees[index];
			  //????
		}
		else{
			root=root->subtrees[index];
			continue;
		}
		if (index>26 ||index<0)
			return 0;
	}
	root->index=dic_index;
	return 1;
}

/* Retrieve trie for the requested key
	return	index in dictionary (trie) if key found
			-1 key not found
*/
int trieSearch( TRIE *root, char *str)
{
	for(int i=0;i<strlen(str);i++){
		int index=getIndex(str[i]);
		root=root->subtrees[index];
		if(root==NULL)
			return -1;
	}
	if(root->index==-1) 
		return -1;
	else
		return root->index;
}

/* prints all entries in trie using preorder traversal
*/
void trieList( TRIE *root, char *dic[])
{
	if(root==NULL)
		return;
	if(root->index!=-1) 
		printf("%s\n",dic[root->index]);  //(root->index!=-1)
	for (int i=0;i<MAX_DEGREE;i++){
		if(root->subtrees[i]==NULL)
			continue;
		else
			trieList(root->subtrees[i],dic);
	}
	//words.ordered txt의 배열==dic[]
}

/* prints all entries starting with str (as prefix) in trie
	ex) "abb" -> "abbas", "abbasid", "abbess", ...
	this function uses trieList function
*/
void triePrefixList( TRIE *root, char *str, char *dic[])
{
	//prefix까지 내려간 담에
	//for문으로 trielist 호출
	for (int i=0;i<strlen(str);i++){
		int index=getIndex(str[i]);
		if(root->subtrees[index]==NULL)
			return;
		root=root->subtrees[index];
	}
	trieList(root,dic);
}