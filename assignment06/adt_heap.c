#include <stdio.h>
#include <stdlib.h> // malloc, realloc
#include "adt_heap.h"


/* Reestablishes heap by moving data in child up to correct location heap array
*/
static void _reheapUp( HEAP *heap, int index)
{   
    int parindex;
    void* hold;
   
    if(index==0)
        return;
    if (index!=0)
    {
        parindex=(index-1)/2;
        if(heap->compare(heap->heapArr[parindex],heap->heapArr[index])<0)  //index가 parindex보다 크면
        {
            hold=heap->heapArr[parindex];
            heap->heapArr[parindex]=heap->heapArr[index];
            heap->heapArr[index]=hold;
            _reheapUp(heap,parindex);  
        }
        else
            return;

    }
}


/* Reestablishes heap by moving data in root down to its correct location in the heap
*/
static void _reheapDown( HEAP *heap, int index)
{
    void *hold;
    void *rightdata;
    void *leftdata;
    int rightin=(index*2)+2;
    int leftin=(index*2)+1;
    int largerin;

    
    if(rightin>heap->last||leftin>heap->last)
        return;
    rightdata=heap->heapArr[rightin];
    leftdata=heap->heapArr[leftin];
    if (heap->compare(leftdata,rightdata)>0)
        largerin=leftin;
    else
        largerin=rightin;
    if (heap->compare(heap->heapArr[index],heap->heapArr[largerin])>0)
        return;
    if (heap->compare(heap->heapArr[index],heap->heapArr[largerin])<0)
    {
        hold=heap->heapArr[index];
        heap->heapArr[index]=heap->heapArr[largerin];
        heap->heapArr[largerin]=hold;
        _reheapDown(heap,largerin);
    }

}



/* Allocates memory for heap and returns address of heap head structure
if memory overflow, NULL returned
*/
HEAP *heap_Create( int capacity, int (*compare) (void *arg1, void *arg2))
{
    HEAP *newHeap;
    newHeap=(HEAP*)malloc(sizeof(HEAP));
    if (!newHeap)
        return NULL;
    newHeap->last=-1;
    newHeap->compare=compare;
    newHeap->capacity=capacity;
    newHeap->heapArr=(void**)malloc(sizeof(void*)*capacity);
    
    return newHeap;
}

/* Free memory for heap
*/
void heap_Destroy( HEAP *heap)
{
    free(heap->heapArr);
    free(heap);

}


/* Inserts data into heap
return 1 if successful; 0 if heap full
*/
int heap_Insert( HEAP *heap, void *dataPtr)
{   
    if(dataPtr==NULL)
        return 0;
    //if heap is empty
    if(heap->last==-1)  
    {  
        heap->last=0;
        heap->heapArr[heap->last]=dataPtr;
        return 1;
    }
    //if heap's capacity 부족하면 realloc
    if (heap->last>=(heap->capacity-1))
    {
        heap->heapArr=realloc(heap->heapArr,sizeof(void *)*(heap->capacity)*2);
        heap->capacity=(heap->capacity)*2;
    }
    ++(heap->last);
    heap->heapArr[heap->last]=dataPtr;
    _reheapUp(heap,heap->last);
    return 1;
}

/* Deletes root of heap and passes data back to caller
return 1 if successful; 0 if heap empty
*/
int heap_Delete( HEAP *heap, void **dataOutPtr)
{
    if(heap->last==-1)
        return 0;
   *dataOutPtr=heap->heapArr[0];
   heap->heapArr[0]=heap->heapArr[heap->last];
   --(heap->last);
   _reheapDown(heap,0);
   return 1;
}

/* 
return 1 if the heap is empty; 0 if not
*/  
int heap_Empty(HEAP *heap)
{
    if((heap->last)==-1)
        return 1;
    else
        return 0;
}


//HEAP.last를 index를 가르키기 -1이 empty, 채워지면서 0,1,2 
/* Print heap array */
void heap_Print( HEAP *heap, void (*print_func) (void *data))
{
    for(int i=0;i<=heap->last;i++)
    {
        print_func(heap->heapArr[i]);
        //printf("\n");

    }

    printf("\n");

}


