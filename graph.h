#ifndef _GRAPH_
#define _GRAPH_

#define tablesize 307

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

TradeBank *CreateGraph(int numVertex, ElemType *TradeBank);
PtrToNext CreateNode(ElemType VertexID[50]);

void InsertNode(ElemType TradeBank[50], ElemType VertexID[50], AllGraph *list);

void InsertEdge(ElemType TradeBank[50], ElemType C1[50], ElemType C2[50], int ConvRate, AllGraph *list);

void PrintGraph(AllGraph list);
void AddGraph(ElemType TradeBank[50], int numVertex, AllGraph *list);

#endif