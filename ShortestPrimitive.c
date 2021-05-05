#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define WHITE 1  // visited vertices
#define GRAY 0   // about to be visited
#define BLACK -1 // not visited
#define INFTY 1000000000
#define UNKNOWN -1
#define NOTVERTEX -1

typedef int DistType; // Data type of distances
typedef struct MinHeap *HeapPtr;
typedef struct HeapNode *HeapNdPtr;

typedef struct CurrIn Node;
typedef struct CurrIn *PtrToNext;
typedef int ElemType;
typedef int Vertex;
typedef long long Weight;

struct HeapNode
{
    DistType Dist;
    int VertexID;
    int PrevID;
};

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
HeapPtr InitHeap(PtrToNext *AdjList, int NumVertices, int HtSize);

struct CurrIn
{
    ElemType VertexID; // Name of the vertex
    ElemType ConvRate; // Edge weight
    PtrToNext Next;    // Pointer to Next Node
};

PtrToNext *InitAdjList(ElemType NumVertices);
PtrToNext *InsertWted(Vertex A, Vertex B, Weight wt, PtrToNext *AdjList);
void FreeAdjList(PtrToNext *AdjList, ElemType NumVertices);
void PrintPaths(HeapPtr Heap);
void DecreaseKey(HeapPtr Heap, int index, DistType NewWt);
void PrintDist(HeapPtr Heap, int NumVertices);
HeapPtr swap(HeapPtr Heap, int index1, int index2);

int main(void)
{
    int v, e;
    scanf("%d%d", &v, &e);

    PtrToNext *AdjList = InitAdjList(v);
    //printf("after init graph\n");

    int A, B;
    long long wt;
    for (int i = 0; i < e; i++)
    {
        scanf("%d%d%lld", &A, &B, &wt);
        AdjList = InsertWted(A, B, wt, AdjList);
    }

    //Dijkstra Heap implementation code
    HeapPtr Heap = InitHeap(AdjList, v, v);
    int startID = 1, count = 0;
    count = Heap->Pos[startID - 1];

    DecreaseKey(Heap, count, 0);
    //printf("root\n");
    while (!IsEmptyHeap(Heap))
    {
        HeapNdPtr Vertex = ExtractMin(Heap);
        PtrToNext Curr = AdjList[Vertex->VertexID - 1]->Next;
        while (Curr != NULL)
        {

            int index = Heap->Pos[Curr->VertexID - 1]; // contains the indexes of the nodes in the heap
            //printf( "Curr Convrate = %d, Vertex->Dist = %d, Heap->arr[index] = %d\n", Curr->ConvRate, Vertex->Dist, Heap->arr[index]->Dist);
            if (Curr->ConvRate + Vertex->Dist < Heap->arr[index]->Dist) //updating dist
                DecreaseKey(Heap, index, Curr->ConvRate + Vertex->Dist);

            Curr = Curr->Next;
        }
    }

    //PrintPaths(Heap);
    PrintDist(Heap, v);
    /////////////////////////////

    FreeAdjList(AdjList, v);

    return 0;
}

PtrToNext *InitAdjList(ElemType NumVertices)
{
    PtrToNext *AdjList = (PtrToNext *)malloc(NumVertices * sizeof(struct CurrIn *));

    // Init the graph
    for (int i = 0; i < NumVertices; i++)
    {
        AdjList[i] = (PtrToNext)malloc(sizeof(struct CurrIn));
        AdjList[i]->ConvRate = 0;
        AdjList[i]->Next = NULL;
        AdjList[i]->VertexID = i + 1;
    }

    return AdjList;
}

PtrToNext *InsertWted(ElemType A, ElemType B, Weight wt, PtrToNext *AdjList)
{
    PtrToNext tmp = (PtrToNext)malloc(sizeof(struct CurrIn));
    PtrToNext curr = AdjList[A - 1];
    tmp->Next = curr->Next; // insert at beginning of Adjlist of A
    tmp->VertexID = B;
    tmp->ConvRate = wt;
    curr->Next = tmp;

    return AdjList;
}

void FreeAdjList(PtrToNext *AdjList, ElemType NumVertices)
{
    for (int i = 0; i < NumVertices; i++)
    {
        PtrToNext Curr = AdjList[i], Next;

        while (Curr != NULL)
        {
            Next = Curr->Next;
            free(Curr);
            Curr = Next;
        }
    }

    free(AdjList);
}

void PrintPaths(HeapPtr Heap)
{
}

void PrintDist(HeapPtr Heap, int NumVertices)
{
    if (NumVertices <= 1)
    {
        printf("0\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < NumVertices; i++)
        printf("%d ", Heap->arr[Heap->Pos[i]]->Dist);

    printf("\n");
}
//////////////////////////////////Heap operations

void MinHeapify(HeapPtr Heap, int parIndex)
{
    int lIndex = (2*parIndex) + 1;
    int rIndex = (2*parIndex) + 2;
    int min = parIndex;

    if (lIndex < Heap->NumElems && Heap->arr[lIndex]->Dist < Heap->arr[min]->Dist)
        min = lIndex;

    if (rIndex < Heap->NumElems && Heap->arr[rIndex]->Dist < Heap->arr[min]->Dist)
        min = rIndex;

    if (min != parIndex)
    {
        //printf( "index1 pts to %p, index2 pts to %p\n", Heap->arr[parIndex], Heap->arr[min] );
        Heap = swap(Heap, parIndex, min);
        //printf( "index1 pts to %p, index2 pts to %p\n", Heap->arr[parIndex], Heap->arr[min] );
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
    //static int alx = 0;
    //printf("it is %d and the count is %d\n", alx++, Heap->NumElems);
    if (Heap->NumElems == 0)
        return NULL;
    
    Heap = swap(Heap, 0, Heap->NumElems - 1);

    Heap->NumElems--;
    HeapNdPtr min = Heap->arr[Heap->NumElems];
    MinHeapify(Heap, 0);

    return min;
}

void DecreaseKey(HeapPtr Heap, int index, DistType NewWt)
{
    int position = index;
    Heap->arr[position]->Dist = NewWt; // Updating of weight

    //Essentially, we proceed in the opposite direction to normal Heapify
    int parIndex;
    while (position > 0)
    {
        parIndex = (position - 1) / 2;

        if (Heap->arr[position]->Dist < Heap->arr[parIndex]->Dist)
        {
            Heap = swap(Heap, position, parIndex);
            position = parIndex; // moving up
        }

        else //then all elems of heap in right place
            break;
    }
}

HeapPtr swap(HeapPtr Heap, int index1, int index2)
{
    int tmp_pos = Heap->Pos[Heap->arr[index1]->VertexID -1];
    Heap->Pos[Heap->arr[index1]->VertexID -1] = Heap->Pos[Heap->arr[index2]->VertexID -1];
    Heap->Pos[Heap->arr[index2]->VertexID -1] = tmp_pos;

    //printf( "index1 pts to %p, index2 pts to %p\n", Heap->arr[index1], Heap->arr[index2] );
    HeapNdPtr tmp = Heap->arr[index1];
    Heap->arr[index1] = Heap->arr[index2];
    Heap->arr[index2] = tmp;
    //printf( "index1 pts to %p, index2 pts to %p\n", Heap->arr[index1], Heap->arr[index2] );

    return Heap;
}

bool IsEmptyHeap(HeapPtr Heap)
{
    if (Heap->NumElems == 0)
        return true;

    return false;
}

HeapPtr InitHeap(PtrToNext *AdjList, int NumVertices, int HtSize)
{
    HeapPtr Heap = (HeapPtr)malloc(sizeof(struct MinHeap));
    assert(Heap != NULL);

    Heap->NumElems = NumVertices;
    Heap->arr = (HeapNdPtr *)malloc(NumVertices * sizeof(HeapNdPtr));
    assert(Heap->arr != NULL);

    for (int i = 0; i < NumVertices; i++)
    {
        //printf("ok %d %d\n%lu\n", i, NumVertices, sizeof(Heap->arr[i]));
        Heap->arr[i] = (HeapNdPtr)malloc(sizeof(struct HeapNode));
        assert(Heap->arr[i] != NULL);
    }

    Heap->Pos = (int *)malloc(NumVertices * sizeof(int));
    assert(Heap->Pos != NULL);
    //printf("before for loop\n");
    int count = 0;
    for (int i = 0; i < HtSize; i++)
    {
        //printf("in for loop %d\n", i);
        if (AdjList[i]->VertexID != NOTVERTEX)
        {
            AdjList[i]->ConvRate = BLACK;
            //printf("first step %d\n", count);
            //printf("%p\n", Heap->arr);
            Heap->arr[count]->PrevID = 0;
            Heap->arr[count]->VertexID = AdjList[i]->VertexID;
            //printf("yes\n");
            Heap->arr[count]->Dist = INFTY;
            Heap->Pos[count] = count;
            count++;
        }
    }

    return Heap;
}
