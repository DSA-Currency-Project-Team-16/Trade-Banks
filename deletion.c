#include <stdio.h>
#include <stdlib.h>

typedef struct TradeBank *PtrToGraph;
typedef struct CurrIn *PtrToNext;
typedef struct CurrOut *PtrToNext;
typedef char ElemType;

struct TradeBank
{
    ElemType TradeBank[50];
    int NumVertex;       // Number of vertices in the graph
    PtrToNext *GraphPtr; // Ptr to each graph (tradebank).
};

struct CurrIn
{
    ElemType VertexID[50]; // Name of the vertex
    int ConvRate; // Edge weight
    PtrToNext Next;    // Pointer to Next Node
};

struct CurrOut
{
    ElemType VertexID[50]; // Name of the vertex
    int ConvRate; // Edge weight
    PtrToNext Next;    // Pointer to Next Node
};

struct AllGraph
{
    int NumBanks;         // number of trade banks present
    PtrToGraph *GraphPtr; // pointer to list of trade banks
};

void delete_TradeBank(ElemType TradeBank[50])
{
    

}
void delete_edge(ElemType TradeBank[50],ElemType origin[50],ElemType destiny[50])//delete a currency conversion from a specified Trade Bank
{

}
void delete_currency(ElemType VertexID[50])
{

}