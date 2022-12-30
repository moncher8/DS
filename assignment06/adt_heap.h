typedef struct
{
	void **heapArr;
	int	last;
	int	capacity;
	int (*compare) (void *arg1, void *arg2);
} HEAP;

/* Allocates memory for heap and returns address of heap head structure
if memory overflow, NULL returned
*/
HEAP *heap_Create( int capacity, int (*compare) (void *arg1, void *arg2));

/* Free memory for heap
*/
void heap_Destroy( HEAP *heap);

/* Inserts data into heap
return 1 if successful; 0 if heap full
*/
int heap_Insert( HEAP *heap, void *dataPtr);
//capacity 기존에 2배씩 증가; realloc

/* Deletes root of heap and passes data back to caller
return 1 if successful; 0 if heap empty
*/
int heap_Delete( HEAP *heap, void **dataOutPtr);

/*
return 1 if the heap is empty; 0 if not
*/
int heap_Empty(  HEAP *heap);
//HEAP.last를 index를 가르키기 -1이 empty, 채워지면서 0,1,2 

/* Print heap array */
void heap_Print( HEAP *heap, void (*print_func) (void *data));

