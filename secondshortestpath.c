#include "graph.h"
#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#define STRLEN 50
#define NULL_CHAR '\0' //Null character
#define TBLSIZE 200

typedef struct AllGraph TradeBankList;
typedef struct AllGraph *TradeBankListPtr;
//typedef struct Node* PtrToNode;
//typedef struct TradeBank* PtrToTradeBank;
//typedef struct Node* PtrToCurrencyGraph;

typedef struct TradeBank *PtrToGraph;
typedef struct Node *PtrToNext;
typedef char ElemType;

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
struct MinHeap
{
    HeapNdPtr *arr;
    int *Pos;
    int NumElems;
};

typedef struct path *PathPtr;
struct path //linked list
{
    ElemType CurrID[STRLEN];
    PathPtr Next;
};
HeapPtr swap(HeapPtr Heap, int index1, int index2);
int GetHash(ElemType Vertex[]); //returns index of the Vertex
PathPtr Dijkstra(PtrToNext *AdjList, ElemType Source[], ElemType Dest[], int NumVertices, int flag, long long *MinDist);
//if flag == 1, prints the shortest path & dist, if flag == -1, finds the shortest path & dist but doesn't print it, else(flag = 0), finds dist(updates MinDist).
void ShortestPath(struct AllGraph *Pointer, ElemType Source[], ElemType Dest[]);

PathPtr AddPath(PathPtr Path, ElemType Vertex[]);
void FreePath(PathPtr Path);
long long Second_Shortest_Path_Of_Graph(PtrToGraph TradeBank, char *Currency1, char *Currency2, int ShortestPathLength, TradeBankListPtr ListOfTradeBanks);
long long Second_Shortest_Conversion(TradeBankListPtr ListOfTradeBanks, char *Currency1, char *Currency2);
PtrToNext delete_edge(ElemType TradeBank[50], ElemType origin[50], ElemType destiny[50], struct AllGraph list, int flag);
void InsertEdge(ElemType TradeBank[50], ElemType C1[50], ElemType C2[50], int ConvRate, TradeBankListPtr list);
void print_existing_path(PathPtr P);

void print_existing_path(PathPtr P)
{
    while(P != NULL)
    {
        printf("%s -> "P->CurrID)
    }
    printf("\n");
}

//Returns the second shortest path between currency1 and currency2 for a particular TradeBank
//Algorithm has a time complexity of O(x*(E + V)*logV) where x is the number of edges in shortest path
//Worst case O(V * (E + V) * logV)
PathPtr Second_Shortest_Path_Of_Graph(PtrToGraph TradeBank, char *Currency1, char *Currency2, int ShortestPathLength,int* SecondShortestCost,TradeBankListPtr ListOfTradeBanks)
{
    long long cost = INFTY;
    long long mincost = INFTY;
    PathPtr P = Dijkstra(TradeBank->GraphIn, Currency1, Currency2, TradeBank->NumVertex, -1, &cost);
    PathPtr clearpath = P;
    PathPtr returnpath;
    PathPtr finalpath;

    if (P == NULL)
        return NULL;

    while (P->Next != NULL)
    {
        //Removing an edge from the shortest path and finding the shortest path using the removed edge
        PtrToNext edge = delete_edge(TradeBank->TradeBank, P->CurrID, P->Next->CurrID, *ListOfTradeBanks, 1);
        cost = INFTY;
        returnpath = Dijkstra(TradeBank->GraphIn, Currency1, Currency2, TradeBank->NumVertex, -1, &cost);
        if (cost == ShortestPathLength)
        {
            cost = INFTY;
            finalpath = Second_Shortest_Path_Of_Graph(TradeBank, Currency1, Currency2, ShortestPathLength,SecondShortestCost, ListOfTradeBanks);
        }
        else if (cost < mincost)
        {
            mincost = cost;
            FreePath(finalpath);
            finalpath = returnpath;
        }
        else
        {
            if(returnpath != NULL)
                FreePath(returnpath);
        }
        InsertEdge(TradeBank->TradeBank, P->CurrID, P->Next->CurrID, edge->ConvRate, ListOfTradeBanks);
        free(edge);
    }
    if(mincost < (*SecondShortestCost))
    {
        *SecondShortestCost = mincost;
        //FreePath(clearpath);
        return finalpath;
    }
    else
        return NULL;
    //FreePath(clearpath);
    //return mincost;
}

//Prints the second shortest path between Currency1 and Currency2 across all TradeBanks
//Total worst case time complexity is
long long Second_Shortest_Conversion(TradeBankListPtr ListOfTradeBanks, char *Currency1, char *Currency2)
{
    //variables
    PtrToGraph T = ListOfTradeBanks->GraphPtr;
    
    if(T == NULL)
        return INFTY;
    
    long long MinConversionCosts[ListOfTradeBanks->NumBanks];
    int NoOfTradeBanksMinCost = 0, minindex = INFTY;
    long long ShortestPathLength = INFTY, SecondShortestCost = INFTY;
    PtrToGraph StartMinTradeBank, SecondMinTradeBank;

    for (int index = 0; index < ListOfTradeBanks->NumBanks; index++)
    {
        MinConversionCosts[index] = INFTY;
        Dijkstra(T->GraphIn, Currency1, Currency2, T->NumVertex, 0, &MinConversionCosts[index]);

        //Finding if it is shortest or second shortest path and assigning each to a variable
        if (MinConversionCosts[index] < ShortestPathLength)
        {
            ShortestPathLength = MinConversionCosts[index];
            StartMinTradeBank = T;
            minindex = index;
            NoOfTradeBanksMinCost = 1;
        }
        else if (MinConversionCosts[index] == ShortestPathLength)
        {
            NoOfTradeBanksMinCost++;
        }
        else if ((MinConversionCosts[index] > ShortestPathLength) && (MinConversionCosts[index] < SecondShortestCost))
        {
            SecondShortestCost = MinConversionCosts[index];
            SecondMinTradeBank = T;
        }

        T = T->Next;
    }

    if (minindex == INFTY)
    {
        printf("No conversion exsists from %s to %s through any Trade Bank\n", Currency1, Currency2);
        return INFTY;
    }
    PathPtr SecondMinGraphPath;
    PathPtr FinalSecondMinGraphPath;

    //Going through TradeBanks which have their cost as the lowest cost obtained across all graphs(or TradeBanks)
    while ((StartMinTradeBank != NULL) && (NoOfTradeBanksMinCost > 0))
    {
        if (MinConversionCosts[minindex] == ShortestPathLength)
        {
            SecondMinGraphPath = Second_Shortest_Path_Of_Graph(StartMinTradeBank, Currency1, Currency2, ShortestPathLength,&SecondShortestCost,ListOfTradeBanks);
            if(SecondMinGraphPath != NULL)
            {
                FinalSecondMinGraphPath = SecondMinGraphPath;
            }
        }

        minindex++;
        StartMinTradeBank = StartMinTradeBank->Next;
        NoOfTradeBanksMinCost--;
    }
    if(FinalSecondMinGraphPath != NULL)
    {
        print_existing_path(FinalSecondMinGraphPath);
        FreePath(FinalSecondMinGraphPath);
    }
    else
    {
        Dijkstra(T->GraphIn, Currency1, Currency2 , T->NumVertex , 1 , INFTY);
    }
    printf("Conversion from %s to %s second lowest cost = %lld through Trade Bank - %s", Currency1, Currency2, SecondShortestCost, SecondMinTradeBank->TradeBank);
}