#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define tablesize 2003

typedef struct TradeBank *PtrToGraph;
typedef struct Node *PtrToNext;
typedef char ElemType;
typedef struct Node Node;

struct TradeBank // stores all details of a TradeBank
{
    ElemType TradeBank[50]; // name of the TradeBank
    int NumVertex;          // Number of vertices in the graph
    Node aIn[tablesize];    //  adjecency list  with considering in edges
    Node aOut[tablesize];   //  adjecency list  with considering out edges
    PtrToNext *NodePtr;     // list of Ptr's to each node of graph (tradebank).
};

struct Node // stores details of Vertex (currency)
{
    ElemType VertexID[50]; // name of currency
    int ConvRate;          // conversion rate -- weight of edge
    PtrToNext Next;        // points to next connected vertex's list
};

struct AllGraph
{
    int NumBanks;         // number of trade banks present
    PtrToGraph *GraphPtr; // pointer to list of trade banks
};
int hash_fun(char string[50]) //  gives a hash key to find in which the edges are stored ( adjecency list)
{
    int sum = 0;
    int l = strlen(string);
    for (int i = 0; i < l; i++)
    {
        sum = (sum * 37 + string[i]) % tablesize;
    }
    return sum % tablesize;
}
void delete_TradeBank(ElemType TradeBank[50], struct AllGraph list) // deletes a TradeBank
{
    int n = list.NumBanks;
    int c = 0;
    for (int i = 0; i < n; i++)
    {
        if (strcmp(list.GraphPtr[i]->TradeBank[50], TradeBank[50]) == 0)
        {
            c = 1;
            for (int j = 0; j < list.GraphPtr[i]->NumVertex; j++)
            {
                list.GraphPtr[i]->NodePtr[j] = NULL;
                free(list.GraphPtr[i]->NodePtr[j]);
            }
            list.GraphPtr[i] = NULL; // this clears hash tables tooo
            free(list.GraphPtr[i]);

            list.NumBanks = list.NumBanks - 1;
            break;
        }
    }
    if (c == 0)
    {
        printf(" There is no TradeBank with this name \n ");
    }
}
void delete_edge(ElemType TradeBank[50], ElemType origin[50], ElemType destiny[50], struct AllGraph list) //delete a currency conversion from a specified Trade Bank
{
    int n = list.NumBanks;
    int c = 0;
    for (int i = 0; i < n; i++)
    {
        if (strcmp(list.GraphPtr[i]->TradeBank[50], TradeBank[50]) == 0)
        {
            int key = hash_fun(origin[50]);
            Node *temp1 = list.GraphPtr[i]->aIn[key].Next;
            Node *prev1;
            while (strcmp(temp1->VertexID[50], destiny[50]) != 0)
            {
                c = 1;
                prev1 = temp1;
                temp1 = temp1->Next;
            }
            prev1->Next = temp1->Next;

            int key = hash_fun(destiny[50]);
            Node *temp2 = list.GraphPtr[i]->aOut[key].Next;
            Node *prev;
            while (strcmp(temp2->VertexID[50], origin[50]) != 0)
            {
                c = 1;
                prev = temp2;
                temp2 = temp2->Next;
            }
            prev->Next = temp2->Next;
        }
    }
    if (c == 0)
    {
        printf(" There is no edge in this TradeBank b/w given vertices \n ");
    }
}
// void delete_currency(ElemType VertexID[50], struct AllGraph list) //  this function will have a bit more complexity  so change using hash tables
// {
//     int n = list.NumBanks;
//     for (int i = 0; i < n; i++)
//     {
//         int m = list.GraphPtr[i]->NumVertex;
//         for (int j = 0; j < m; j++)
//         {
//             if (strcmp(list.GraphPtr[i]->NodePtr[j]->VertexID[50], VertexID[50]) == 0)
//             {
//                 list.GraphPtr[i]->NodePtr[j]=NULL;
//                 free(list.GraphPtr[i]->NodePtr[j]);
//                 list.GraphPtr[i]->NumVertex--;

//             }
//         }
//     }
// }
void delete_currency(ElemType VertexID[50], struct AllGraph list) // deletes the nodes(currencies)  along with all the edges connected to them
{
    int n = list.NumBanks;
    int c = 0;
    int key = hash_fun(VertexID[50]);

    for (int i = 0; i < n; i++)
    {

        Node temp = list.GraphPtr[i]->aIn[key];
        if (temp.Next != NULL)
        {
            c = 1;
            Node *temp1 = temp.Next;
            while (temp.Next != NULL)
            {
                int key1 = hash_fun(temp.Next->VertexID[50]);
                Node t = list.GraphPtr[i]->aOut[key1];
                while (t.Next != NULL)
                {
                    if (strcmp(t.Next->VertexID[50], VertexID[50]) == 0)
                    {
                        t.Next = t.Next->Next;
                        break;
                    }
                }
            }
            temp1 = NULL;
            free(temp1);
        }
        Node temp2 = list.GraphPtr[i]->aOut[key];
        if (temp2.Next != NULL)
        {
            c = 1;
            Node *temp3 = temp2.Next;
            while (temp2.Next != NULL)
            {
                int key1 = hash_fun(temp2.Next->VertexID[50]);
                Node t = list.GraphPtr[i]->aIn[key1];
                while (t.Next != NULL)
                {
                    if (strcmp(t.Next->VertexID[50], VertexID[50]) == 0)
                    {
                        t.Next = t.Next->Next;
                        break;
                    }
                }
            }
            temp3 = NULL;
            free(temp3);
        }
        if(temp.Next != NULL||temp2.Next != NULL)
        {
            list.GraphPtr[i]->NumVertex--;
        }
    }
    if(c==0)
    {
        printf(" There is no currency of this name in any TradeBank  \n ");
    }
}
