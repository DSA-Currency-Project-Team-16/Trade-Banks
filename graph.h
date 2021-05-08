#pragma once

#define WHITE 1  // visited vertices
#define GRAY 0   // about to be visited
#define BLACK -1 // not visited
#define INFTY 100000000
#define UNKNOWN -1
#define NOTVERTEX -1

typedef struct TradeBank *PtrToGraph;
typedef struct CurrIn *PtrToNext;
typedef struct CurrOut *PtrToNext;
typedef int ElemType;

struct TradeBank
{
    int NumVertex;       // Number of vertices in the graph
    PtrToNext *GraphPtr; // Ptr to each graph (tradebank).
};

struct CurrIn
{
    ElemType VertexID; // Name of the vertex
    ElemType ConvRate; // Edge weight
    PtrToNext Next;    // Pointer to Next Node
};

struct CurrOut
{
    ElemType VertexID; // Name of the vertex
    ElemType ConvRate; // Edge weight
    PtrToNext Next;    // Pointer to Next Node
};

struct AllGraph
{
    int NumBanks;         // number of trade banks present
    PtrToGraph *GraphPtr; // pointer to list of trade banks
};

// Let's say A->B (A nad B are vertices and -> is a directed edge) is present in the graph
// Then, in CurrOut HashTable(adjacency list), the neighbor of B will be A
// Similarly, in CurrIn HashTable(adjacency list), neighbor of A will be B  