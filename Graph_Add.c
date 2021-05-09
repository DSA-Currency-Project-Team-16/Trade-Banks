#include "Graph_Add.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define y 33
#define del -2

int hash_fun(ElemType string[50])     // Hash function maps the given string to a position in the hash table
{                                     // using Horner's rule and returns the position.              
    int sum = 0;                      // takes name of currency(string) as input
    int l = strlen(string);
    for (int i = 0; i < l; i++)
    {
        sum = (sum * y + string[i]) % tablesize;
    }
    return sum % tablesize;
}

int hash_search(ElemType string[50], PtrToNext *G)      // Search function finds the location of the given string in the given hashtable 
{                                                       // and returns the position if found. 
    int key = hash_fun(string);                         // otherwise returns -1
    if (strcmp(G[key]->VertexID, string) == 0)          // takes name of currency(string) and pointer to hashtable(PtrToNext) as inputs
        return key;
    else
    {
        while (G[key]->VertexID[0] != '\0' || G[key]->ConvRate == del)
        {
            if ((strcmp(G[key]->VertexID, string) == 0))
                return key;
            key++;
            key = key % tablesize;
        }
        return -1;
    }
}

PtrToNext CreateNode(ElemType VertexID[50], int ConvRate)      // Creates a Node(allocates memory) and initialises pointer to NULL and returns it
{                                                              // takes name of currency(string) and conversion rate as inputs       
    Node *N = malloc(sizeof(Node));
    strcpy(N->VertexID, VertexID);
    N->ConvRate = ConvRate;
    N->Next = NULL;
    return N;
}

void AddGraph(ElemType TradeBank[50], AllGraph *list)      // Creates a graph using CreateGraph function and adds it to the list of TradeBanks 
{                                                          // takes name of TradeBank(string) and list of graphs as inputs             
    PtrToGraph B = CreateGraph(TradeBank);
    B->Next = list->GraphPtr;
    list->GraphPtr = B;
    list->NumBanks++;
    printf("TradeBank %s has been successfully created!\n", B->TradeBank);
}

PtrToGraph CreateGraph(ElemType TradeBank[50])                      // Creates a graph and initialises the head pointer(to the linked list associated with each vertex)
{                                                                   //  to NULL and returns a pointer to graph
    PtrToGraph B = (PtrToGraph)malloc(sizeof(struct TradeBank));
    B->GraphIn = (PtrToNext *)malloc(sizeof(Node *) * tablesize);
    B->GraphOut = (PtrToNext *)malloc(sizeof(Node *) * tablesize);
    strcpy(B->TradeBank, TradeBank);
    B->NumVertex = 0;
    B->Next = NULL;

    for (int i = 0; i < tablesize; i++)
    {
        B->GraphIn[i] = (PtrToNext)malloc(sizeof(Node));
        B->GraphOut[i] = (PtrToNext)malloc(sizeof(Node));
        B->GraphIn[i]->Next = NULL;
        B->GraphIn[i]->ConvRate = -1;             // to differentitate vertex and node(of linked list)                  
        B->GraphIn[i]->VertexID[0] = '\0';        // useful in other functions
        B->GraphOut[i]->Next = NULL;
        B->GraphOut[i]->ConvRate = -1;
        B->GraphOut[i]->VertexID[0] = '\0';
    }
    B->check = (bool *)malloc(sizeof(bool) * 200);
    for(int i = 0; i < 200; i++)
        B->check[i] = false;
    return B;
}

void InsertNode(ElemType TradeBank[50], ElemType VertexID[50], AllGraph *list)              // Adds a currency to the given TradeBank
{
    AllGraph *temp = list;
    while (temp->GraphPtr)                      // To traverse the list of graphs
    {
        if (strcmp(temp->GraphPtr->TradeBank, TradeBank) == 0)          // to check whether the given TradeBank exists 
        {
            if (hash_search(VertexID, temp->GraphPtr->GraphIn) >= 0)        // To check whether the given currency already exists
            {
                printf("Currency already exists..\n");
                return;
            }
            int key = hash_fun(VertexID);

            while (temp->GraphPtr->GraphIn[key]->VertexID[0] != '\0')       // Traversing the hashtable until a vacant position is found
            {
                if (key == tablesize - 1)
                    key = 0;
                else
                    key++;
            }
            strcpy(temp->GraphPtr->GraphIn[key]->VertexID, VertexID);
            strcpy(temp->GraphPtr->GraphOut[key]->VertexID, VertexID);

            temp->GraphPtr->GraphIn[key]->ConvRate = -1;
            temp->GraphPtr->GraphOut[key]->ConvRate = -1;

            temp->GraphPtr->NumVertex++;
            printf("Currency %s has been successfully added in TradeBank %s!\n", temp->GraphPtr->GraphIn[key]->VertexID, temp->GraphPtr->TradeBank);
            temp = NULL;
            free(temp);
            return;
        }

        temp->GraphPtr = temp->GraphPtr->Next;
    }
    printf("TradeBank does not exist!!\n");
}

void InsertEdge(ElemType TradeBank[50], ElemType C1[50], ElemType C2[50], int ConvRate, AllGraph *list)         // Adds a currency conversion between the given two currencies
{
    AllGraph *temp = list;
    while (temp->GraphPtr)                      // To traverse the list of graphs        
    {
        if (strcmp(temp->GraphPtr->TradeBank, TradeBank) == 0)          // to check whether the given TradeBank exists 
        {
            int key1 = hash_search(C1, temp->GraphPtr->GraphIn);        // To check whether the given currency(source) exists in the TradeBank
            if (key1 == -1) {
                printf("INVALID OPERATION !!\n");
                return;
            }

            int key2 = hash_search(C2, temp->GraphPtr->GraphOut);       // To check whether the given currency(destination) exists in the TradeBank
            if (key2 == -1) {
                printf("INVALID OPERATION !!\n");
                return;
            }

            PtrToNext nodeIn = CreateNode(C2, ConvRate);                // Creates a Node(to insert it in the linkedlist of edges of that particular vertex)

            PtrToNext nodeOut = CreateNode(C1, ConvRate);               // Creates a Node(to insert it in the linkedlist of edges of that particular vertex)

            PtrToNext t1 = temp->GraphPtr->GraphIn[key1]->Next;
            while (t1)                                                  // To check whether the given currency conversion already exists in the TradeBank
            {
                if (strcmp(t1->VertexID, C2) == 0)
                {
                    printf("Edge already exists !!\n\n");
                    t1 = NULL;
                    free(t1);
                    return;
                }
                t1 = t1->Next;
            }
            t1 = NULL;
            free(t1);

            nodeIn->Next = temp->GraphPtr->GraphIn[key1]->Next;         // Inserts the currency conversion in the linkedlist associated with the source vertex
            temp->GraphPtr->GraphIn[key1]->Next = nodeIn;
            temp->GraphPtr->GraphIn[key1]->ConvRate = 0;

            nodeOut->Next = temp->GraphPtr->GraphOut[key2]->Next;       // Inserts the currency conversion in the linkedlist associated with the destination vertex
            temp->GraphPtr->GraphOut[key2]->Next = nodeOut;
            temp->GraphPtr->GraphOut[key2]->ConvRate = 0;

            printf("Currency conversion of rate %d has been added from the currency %s to the currency %s in the TradeBank %s!\n", ConvRate, C1, C2, temp->GraphPtr->TradeBank);
            return;
        }

        temp->GraphPtr = temp->GraphPtr->Next;
    }
    printf("TradeBank does not exist!!\n");
    temp = NULL;
    free(temp);
}