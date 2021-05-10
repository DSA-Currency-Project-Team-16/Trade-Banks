#ifndef _GRAPH_
#define _GRAPH_
#include <stdbool.h>
#define tablesize 307

typedef char ElemType;
typedef struct TradeBank *PtrToGraph;
typedef struct Node *PtrToNext;
typedef struct AllGraph TradeBankList;
typedef struct AllGraph *TradeBankListPtr;
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

void PrintGraph(AllGraph *list);
void AddGraph(ElemType TradeBank[50], AllGraph *list);

PtrToNext delete_edge(ElemType TradeBank[50], ElemType origin[50], ElemType destiny[50], struct AllGraph *list, int flag);
void delete_TradeBank(ElemType TradeBank[50], struct AllGraph *list);
void delete_currency(ElemType TradeBank[50], ElemType VertexID[50], struct AllGraph *list);

//void ShortestPath(struct AllGraph *Pointer, ElemType Source[], ElemType Dest[]);
//PathPtr Second_Shortest_Path_Of_Graph(PtrToGraph TradeBank, char *Currency1, char *Currency2, long long ShortestPathLength, long long *SecondShortestCost, TradeBankListPtr ListOfTradeBanks);
#endif