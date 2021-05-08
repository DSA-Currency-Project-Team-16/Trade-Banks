#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "graph.h"
#include <string.h>

/*Likith, use dijkstra function for your second shortest path, and free
the path using FreePath after you have finished*/
/*Basic algorithm used = Dijkstra using Heaps. Expected Time complexity of Dijkstra = (E + V)logV
  time complexity of shortest path algorithm = (Number of tradebanks)*Dijkstra */

#define STRLEN 50
#define NULL_CHAR '\0' //Null character
#define TBLSIZE 200    //size of HashTable

typedef int DistType;               // Data type of distances
typedef struct MinHeap *HeapPtr;    //Points to whole Heap
typedef struct HeapNode *HeapNdPtr; // To individual nodes of heap
typedef char ElemType;              //used for Currency names

struct HeapNode
{
    DistType Dist;
    ElemType VertexID[STRLEN];
    ElemType PrevID[STRLEN];
};

typedef struct path *PathPtr;
struct path //linked list
{
    ElemType CurrID[STRLEN];
    PathPtr Next;
};

PathPtr AddPath(PathPtr Path, ElemType Vertex[]);
void FreePath(PathPtr Path);

struct MinHeap
{
    HeapNdPtr *arr;
    int *Pos;
    int NumElems;
};

void MinHeapify(HeapPtr Heap, int parIndex);
void MinHeapBuild(HeapPtr Heap);
HeapNdPtr ExtractMin(HeapPtr Heap);
bool IsEmptyHeap(HeapPtr Heap);
HeapPtr InitHeap(PtrToNext *AdjList, int NumVertices, int HtSize, ElemType Source[]);
void DecreaseKey(HeapPtr Heap, int index, DistType NewWt);

DistType FindDist(HeapPtr Heap, int startID, int DestID);
PathPtr FindPath(HeapPtr Heap, ElemType Source[], ElemType Dest[], PathPtr Path);
PathPtr PrintPath(HeapPtr Heap, ElemType Source[], ElemType Dest[], PathPtr Path);
void PrintDist(HeapPtr Heap, int StartID, int DestID);

HeapPtr swap(HeapPtr Heap, int index1, int index2);
int GetHash(ElemType Vertex[]); //returns index of the Vertex
PtrToNext Dijkstra(PtrToNext *AdjList, ElemType Source[], ElemType Dest[], int NumVertices, int flag, long long *MinDist);
//if flag == 1, prints the shortest path & dist, if flag == -1, finds the shortest path & dist but doesn't print it, else(flag = 0), finds dist(updates MinDist).
void ShortestPath(struct AllGraph *Pointer, ElemType Source[], ElemType Dest[]);

PtrToNext Dijkstra(PtrToNext *AdjList, ElemType Source[], ElemType Dest[], int NumVertices, int flag, long long *MinDist)
{
    //Dijkstra Heap implementation code
    HeapPtr Heap = InitHeap(AdjList, NumVertices, NumVertices, Source);
    int startID = 0;
    while (startID < TBLSIZE && strcmp(Heap->arr[startID]->VertexID, Source) != 0) //goes to the source vertex
        startID++;

    if (startID >= TBLSIZE) //Source Vertex Not found
    {
        if (flag == -1)
            *MinDist = 20 * INFTY + 5;

        else if (flag == 1)
            printf("The source currency(A) does not exist\n");

        //if flag == 0, does nothing

        return NULL;
    }
    int count = Heap->Pos[startID];

    DecreaseKey(Heap, count, 0);
    HeapNdPtr Vertex = NULL;

    while (!IsEmptyHeap(Heap))
    {
        Vertex = ExtractMin(Heap);
        if (strcmp(Vertex->VertexID, Dest) == 0) // Dest = name of currency we were looking for
            break;

        PtrToNext Curr = AdjList[GetHash(Vertex->VertexID)]->Next;
        while (Curr != NULL)
        {

            int index = Heap->Pos[GetHash(Curr->VertexID)]; // contains the indexes of the nodes in the heap

            if (Curr->ConvRate + Vertex->Dist < Heap->arr[index]->Dist) //updating dist
                DecreaseKey(Heap, index, Curr->ConvRate + Vertex->Dist);

            Curr = Curr->Next;
        }
    }

    if (IsEmptyHeap) //if Heap is empty, 2 chances, either the last vertex extracted was Dest/Dest isn't in the TradeBank
    {
        if (strcmp(Dest, Vertex->VertexID) != 0)
        {
            if (flag == 1)
                printf("The currency B does not exist\n");

            else if (flag == -1)
                *MinDist = 20*INFTY + 5;    //meaning that the dest vertex doesn't exist
            
            // flag = 0, do  nothing

            return NULL;
        }       
    }

    PathPtr Path = NULL;

    if (flag == 1)
    {
        Path = PrintPath(Heap, Source, Dest, Path);
        PrintDist(Heap, startID, GetHash(Dest));
        FreeHeap(Heap, NumVertices);

        return Path;
    }

    else if (flag == -1)
    {
        Path = FindPath(Heap, Source, Dest, Path);
        *MinDist = FindDist(Heap, Source, GetHash(Dest));
        FreeHeap(Heap, NumVertices);

        return Path;
    }

    //if flag == 0
    if (Heap->arr[Heap->Pos[GetHash(Dest)]] < *MinDist)
        *MinDist = Heap->arr[Heap->Pos[GetHash(Dest)]];

    FreeHeap(Heap, NumVertices);
    return Path;
}

void ShortestPath(struct AllGraph *Pointer, ElemType Source[], ElemType Dest[])
{
    PtrToGraph CurrBank = Pointer->GraphPtr; //points to current tradebank
    DistType MinDist = 20 * INFTY + 5, temp;
    temp = MinDist;
    ElemType BankName[STRLEN] = {'\0'};

    while (CurrBank != NULL)
    {
        Dijkstra(CurrBank->GraphIn, Source, Dest, CurrBank->NumVertex, 0, &MinDist);
        if (temp != MinDist) //meaning that mindist was updated
        {
            strcmp(BankName, CurrBank->TradeBank);
            temp = MinDist;
        }

        CurrBank = CurrBank->Next;
    }

    CurrBank = Pointer->GraphPtr;
    while (strcmp(CurrBank->TradeBank, BankName)) //traversing the list of tradebanks
        CurrBank = CurrBank->Next;

    //Prints shortest path and shortest path distance
    Dijkstra(CurrBank->GraphIn, Source, Dest, CurrBank->NumVertex, 1, &MinDist);
}

//////////////////////////////////Heap operations

void MinHeapify(HeapPtr Heap, int parIndex)
{
    int lIndex = (2 * parIndex) + 1;
    int rIndex = (2 * parIndex) + 2;
    int min = parIndex;

    if (lIndex < Heap->NumElems && Heap->arr[lIndex]->Dist < Heap->arr[min]->Dist)
        min = lIndex;

    if (rIndex < Heap->NumElems && Heap->arr[rIndex]->Dist < Heap->arr[min]->Dist)
        min = rIndex;

    if (min != parIndex)
    {

        Heap = swap(Heap, parIndex, min);
        MinHeapify(Heap, min);
    }
}

void MinHeapBuild(HeapPtr Heap)
{
    for (int i = Heap->NumElems / 2; i >= 1; i--)
        MinHeapify(Heap, i);
}

HeapNdPtr ExtractMin(HeapPtr Heap)
{
    if (Heap->NumElems == 0)
        return NULL;

    Heap = swap(Heap, 0, Heap->NumElems - 1);

    Heap->NumElems--;
    HeapNdPtr min = Heap->arr[Heap->NumElems]; // the element which is currently in the priority queue
    MinHeapify(Heap, 0);

    return min;
}

void DecreaseKey(HeapPtr Heap, int index, DistType NewWt)
{
    Heap->arr[index]->Dist = NewWt; // Updating of weight

    //Essentially, we proceed in the opposite direction to normal Heapify
    int parIndex;
    while (index > 0)
    {
        parIndex = (index - 1) / 2;

        if (Heap->arr[index]->Dist < Heap->arr[parIndex]->Dist)
        {
            Heap = swap(Heap, index, parIndex);
            index = parIndex; // moving up
        }

        else //then all elems of heap in right place
            break;
    }
}

HeapPtr swap(HeapPtr Heap, int index1, int index2)
{
    int pos1 = GetHash(Heap->arr[index1]->VertexID); //positions of the elements in the heaps
    int pos2 = GetHash(Heap->arr[index2]->VertexID);

    int tmp_pos = Heap->Pos[pos1];
    Heap->Pos[pos1] = Heap->Pos[pos2];
    Heap->Pos[pos2] = tmp_pos;

    HeapNdPtr tmp = Heap->arr[index1];
    Heap->arr[index1] = Heap->arr[index2];
    Heap->arr[index2] = tmp;

    return Heap;
}

bool IsEmptyHeap(HeapPtr Heap)
{
    if (Heap->NumElems == 0)
        return true;

    return false;
}

HeapPtr InitHeap(PtrToNext *AdjList, int NumVertices, int HtSize, ElemType Source[]) //initialises heap
{
    HeapPtr Heap = (HeapPtr)malloc(sizeof(struct MinHeap));
    assert(Heap != NULL);

    Heap->NumElems = NumVertices;
    Heap->arr = (HeapNdPtr *)malloc(NumVertices * sizeof(HeapNdPtr));
    assert(Heap->arr != NULL);

    for (int i = 0; i < NumVertices; i++)
    {
        Heap->arr[i] = (HeapNdPtr)malloc(sizeof(struct HeapNode));
        assert(Heap->arr[i] != NULL);
    }

    Heap->Pos = (int *)malloc(NumVertices * sizeof(int));
    assert(Heap->Pos != NULL);

    int count = 0;
    for (int i = 0; i < HtSize; i++)
    {
        if (AdjList[i]->VertexID[0] != NULL_CHAR) //checks if the element of hashtbl is a currency or not
        {
            strcpy(Heap->arr[count]->PrevID, Source);
            strcpy(Heap->arr[count]->VertexID, AdjList[i]->VertexID);

            Heap->arr[count]->Dist = INFTY;
            Heap->Pos[count] = count;
            count++;
        }
    }

    return Heap;
}

void FreeHeap(HeapPtr Heap, int NumVertices)
{
    for (int i = 0; i < NumVertices; i++)
        free(Heap->arr[i]);

    free(Heap->Pos);
    free(Heap->arr);
    free(Heap);
}

///////////////////////////////////////////////////////

PathPtr AddPath(PathPtr Path, ElemType Vertex[])
{
    PathPtr tmp = (PathPtr)malloc(sizeof(struct path));
    assert(tmp != NULL);

    strcpy(tmp->CurrID, Vertex);
    tmp->Next = NULL;

    if (Path == NULL)
        return tmp;

    //else
    PathPtr Curr = Path;
    while (Curr->Next != NULL) //goto end of LL
        Curr = Curr->Next;

    Curr->Next = tmp;

    return Path;
}

void FreePath(PathPtr Path)
{
    PathPtr curr = Path;

    while (Path != NULL)
    {
        Path = Path->Next;
        free(curr);
        curr = Path;
    }
}

PathPtr PrintPath(HeapPtr Heap, ElemType Source[], ElemType Dest[], PathPtr Path)
{
    int index = GetHash(Dest);
    while (strcmp(Dest, Source)) // if the vertex is not the same as source
        PrintPath(Heap, Source, Heap->arr[index]->PrevID, Path);

    printf("%s ->", Dest);
    Path = AddPath(Path, Dest); //adding of node to LList

    return Path;
}

void PrintDist(HeapPtr Heap, int startID, int DestID)
{
    printf("Shortest Distance = %d\n", Heap->arr[Heap->Pos[DestID]]->Dist); //If DistType is changed,the type specifier
                                                                            //Must also be  changed
}

PathPtr FindPath(HeapPtr Heap, ElemType Source[], ElemType Dest[], PathPtr Path)
{
    int index = GetHash(Dest);
    while (strcmp(Dest, Source)) // if the vertex is not the same as source
        PrintPath(Heap, Source, Heap->arr[index]->PrevID, Path);

    Path = AddPath(Path, Dest); //adding of node to LList

    return Path;
}

DistType FindDist(HeapPtr Heap, int startID, int DestID)
{
    return (Heap->arr[Heap->Pos[DestID]]->Dist);
}

/*check for segfault around line 119, in the while loops for shortest path*/
