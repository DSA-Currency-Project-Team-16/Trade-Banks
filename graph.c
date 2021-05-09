#include "graph.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define y 33

int hash_fun(ElemType string[50])
{
    int sum = 0;
    int l = strlen(string);
    for (int i = 0; i < l; i++)
    {
        sum = (sum * y + string[i]) % tablesize;
    }
    return sum % tablesize;
}

PtrToNext CreateNode(ElemType VertexID[50])
{
    Node *N = malloc(sizeof(Node));
    strcpy(N->VertexID, VertexID);
    N->Next = NULL;
    return N;
}

void AddGraph(ElemType TradeBank[50], int NumVertex, AllGraph *list)
{
    PtrToGraph B = CreateGraph(NumVertex, TradeBank);
    B->Next = list->GraphPtr;
    list->GraphPtr = B;
    list->NumBanks++;
}

PtrToGraph CreateGraph(int NumVertex, ElemType TradeBank[50])
{
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
        B->GraphIn[i]->ConvRate = -1;
        B->GraphIn[i]->VertexID[0] = '\0';
        B->GraphOut[i]->Next = NULL;
        B->GraphOut[i]->ConvRate = -1;
        B->GraphOut[i]->VertexID[0] = '\0';
    }
    return B;
}

void InsertNode(ElemType TradeBank[50], ElemType VertexID[50], AllGraph *list)
{
    AllGraph *temp = list;
    while (temp->GraphPtr)
    {
        if (strcmp(temp->GraphPtr->TradeBank, TradeBank) == 0)
        {

            int key = hash_fun(VertexID);

            while (temp->GraphPtr->GraphIn[key]->ConvRate == 0)
            {
                if (key == tablesize - 1)
                    key = 0;
                else
                    key++;
            }
            strcpy(temp->GraphPtr->GraphIn[key]->VertexID, VertexID);
            strcpy(temp->GraphPtr->GraphOut[key]->VertexID, VertexID);

            temp->GraphPtr->GraphIn[key]->ConvRate = 0;
            temp->GraphPtr->GraphOut[key]->ConvRate = 0;

            temp->GraphPtr->NumVertex++;
            return;
        }

        temp->GraphPtr = temp->GraphPtr->Next;
    }
}

int hash_search(ElemType string[50], PtrToNext *G)
{
    int key = hash_fun(string);
    if (strcmp(G[key]->VertexID, string) == 0)
        return key;
    else
    {
        while (G[key]->ConvRate == 0)
        {
            if ((strcmp(G[key]->VertexID, string) == 0))
                return key;
            key++;
            key = key % tablesize;
        }
        return -1;
    }
}

void InsertEdge(ElemType TradeBank[50], ElemType C1[50], ElemType C2[50], int ConvRate, AllGraph *list)
{
    AllGraph *temp = list;
    while (temp->GraphPtr)
    {
        if (strcmp(temp->GraphPtr->TradeBank, TradeBank) == 0)
        {

            PtrToNext nodeIn = CreateNode(C2);
            nodeIn->ConvRate = ConvRate;

            PtrToNext nodeOut = CreateNode(C1);
            nodeOut->ConvRate = ConvRate;

            int key1 = hash_search(C1, temp->GraphPtr->GraphIn);
            if (key1 == -1)
                printf("INVALID OPERATION !!\n\n");

            int key2 = hash_search(C2, temp->GraphPtr->GraphOut);
            if (key2 == -1)
                printf("INVALID OPERATION !!\n\n");    

            PtrToNext t1 = temp->GraphPtr->GraphIn[key1]->Next;
            while(t1) {
                if(strcmp(t1->VertexID, C2) == 0) {
                    printf("Edge already exists !!\n\n");
                    return;
                }
                t1 = t1->Next;
            }    

            nodeIn->Next = temp->GraphPtr->GraphIn[key1]->Next;
            temp->GraphPtr->GraphIn[key1]->Next = nodeIn;
            temp->GraphPtr->GraphIn[key1]->ConvRate = 0;

            nodeOut->Next = temp->GraphPtr->GraphOut[key2]->Next;
            temp->GraphPtr->GraphOut[key2]->Next = nodeOut;
            temp->GraphPtr->GraphOut[key2]->ConvRate = 0;

            return;
        }

        temp->GraphPtr = temp->GraphPtr->Next;
    }
}

// void PrintGraph(AllGraph list)
// {
//     while (list.GraphPtr)
//     {
//         printf("TradeBank: %s NumVertex: %d\n", list.GraphPtr->TradeBank, list.GraphPtr->NumVertex);

//         for (int i = 0; i < tablesize; i++)
//         {
//             if (list.GraphPtr->GraphIn[i]->VertexID[0] != '\0')
//             {
//                 printf("Currency: %s(%d) ", list.GraphPtr->GraphIn[i]->VertexID, list.GraphPtr->GraphIn[i]->ConvRate);
//                 // printf("Currency: %s ", list.GraphPtr->GraphIn[i]->VertexID);
//                 Node *temp = list.GraphPtr->GraphIn[i]->Next;
//                 while (temp)
//                 {
//                     printf("%s(%d) ", temp->VertexID, temp->ConvRate);
//                     temp = temp->Next;
//                 }
//                 printf("\n");
//                 free(temp);
//             }
//         }
//         printf("\n");

//         for (int i = 0; i < tablesize; i++)
//         {
//             if (list.GraphPtr->GraphOut[i]->VertexID[0] != '\0')
//             {
//                 // printf("Currency: %s ", list.GraphPtr->GraphOut[i]->VertexID);
//                 printf("Currency: %s(%d) ", list.GraphPtr->GraphIn[i]->VertexID, list.GraphPtr->GraphIn[i]->ConvRate);
//                 Node *temp = list.GraphPtr->GraphOut[i]->Next;
//                 while (temp)
//                 {
//                     printf("%s(%d) ", temp->VertexID, temp->ConvRate);
//                     temp = temp->Next;
//                 }
//                 free(temp);
//                 printf("\n");
//             }
//         }
//         printf("\n");

//         list.GraphPtr = list.GraphPtr->Next;

//         printf("\n");
//     }
// }
