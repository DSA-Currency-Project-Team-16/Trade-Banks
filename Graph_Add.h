#ifndef _GRAPH_
#define _GRAPH_

#define tablesize 307
#include <stdbool.h>

typedef char ElemType;
typedef struct TradeBank *PtrToGraph;
typedef struct Node *PtrToNext;

struct TradeBank
{
    ElemType TradeBank[50];
    int NumVertex;
    PtrToNext *GraphIn;
    PtrToNext *GraphOut;
    PtrToGraph Next;
    bool* check;
};
typedef struct TradeBank TradeBank;

struct Node
{
    ElemType VertexID[50];
    int ConvRate;
    PtrToNext Next;
};
typedef struct Node Node;

struct AllGraph
{
    int NumBanks;
    PtrToGraph GraphPtr;
};
typedef struct AllGraph AllGraph;

PtrToGraph CreateGraph(ElemType TradeBank[50]);
PtrToNext CreateNode(ElemType VertexID[50], int ConvRate);

void InsertNode(ElemType TradeBank[50], ElemType VertexID[50], AllGraph *list);

void InsertEdge(ElemType TradeBank[50], ElemType C1[50], ElemType C2[50], int ConvRate, AllGraph *list);

void AddGraph(ElemType TradeBank[50], AllGraph *list);

#endif