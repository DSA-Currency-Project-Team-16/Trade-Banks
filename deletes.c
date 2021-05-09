#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define tablesize 307

typedef struct TradeBank *PtrToGraph;
typedef struct Node *PtrToNext;
typedef char ElemType;
typedef struct Node Node;

struct TradeBank // stores all details of a TradeBank
{
    ElemType TradeBank[50]; // name of the TradeBank
    int NumVertex;          // Number of vertices in the graph
    PtrToNext *GraphIn;     //  adjecency list  with considering in edges
    PtrToNext *GraphOut;    //  adjecency list  with considering out edges
    PtrToGraph Next;        // list of Ptr's to each node of graph (tradebank).
    bool *check;
};

struct Node // stores details of Vertex (currency)
{
    ElemType VertexID[50]; // name of currency
    int ConvRate;          // conversion rate -- weight of edge
    PtrToNext Next;        // points to next connected vertex's list
};

struct AllGraph
{
    int NumBanks;        // number of trade banks present
    PtrToGraph GraphPtr; // pointer to list of trade banks
};
int hash_search(ElemType string[50], PtrToNext *G);
PtrToNext delete_edge(ElemType TradeBank[50], ElemType origin[50], ElemType destiny[50], struct AllGraph *list, int flag);
void delete_TradeBank(ElemType TradeBank[50], struct AllGraph *list);
void delete_currency(ElemType TradeBank[50], ElemType VertexID[50], struct AllGraph *list);

int hash_search(ElemType string[50], PtrToNext *G)
{
    int key = hash_fun(string);
    if (strcmp(G[key]->VertexID, string) == 0)
        return key;
    else
    {
        while (G[key]->VertexID[0] != '\0' || G[key]->ConvRate == del)
        {
            // printf("------\n");
            if ((strcmp(G[key]->VertexID, string) == 0))
                return key;
            key++;
            key = key % tablesize;
        }
        return -1;
    }
}
void delete_TradeBank(ElemType TradeBank[50], struct AllGraph *list) // deletes a TradeBank
{
    int n = list->NumBanks;
    int c = 0;
    PtrToGraph temp = list->GraphPtr;
    PtrToGraph prev = temp;
    while (temp != NULL)
    {
        if (strcmp(temp->TradeBank, TradeBank) == 0) // checks whether matches to given TradeBank or not
        {
            free(temp->check);
            c = 1;
            for (int i = 0; i < tablesize; i++)
            {
                Node *t1 = temp->GraphIn[i], *next;
                while (t1 != NULL)
                {

                    next = t1->Next;
                    free(t1);
                    t1 = next;
                }
                // free(next);

                Node *t2 = temp->GraphOut[i], *next1;
                while (t2 != NULL)
                {
                    next1 = t2->Next;
                    free(t2);
                    t2 = next1;
                }
                // free(next1);
            }
            free(temp->GraphIn);
            free(temp->GraphOut);
            if (temp == prev)
            {
                list->GraphPtr = temp->Next;
                break;
            }
            prev->Next = temp->Next;
            free(temp); // clears the entire trade bank
            list->NumBanks = list->NumBanks - 1;
            break;
        }
        prev = temp;
        temp = temp->Next;
    }
    if (c == 0)
    {
        printf(" There is no TradeBank with this name \n ");
    }
}
PtrToNext delete_edge(ElemType TradeBank[50], ElemType origin[50], ElemType destiny[50], struct AllGraph *list, int flag) //delete a currency conversion from a specified Trade Bank
{
    int n = list->NumBanks;
    int c = 0;
    PtrToGraph temp = list->GraphPtr;
    while (temp != NULL)
    {
        if (strcmp(temp->TradeBank, TradeBank) == 0) // checks whether matches to given TradeBank or not
        {
            int key2 = hash_search(destiny, temp->GraphOut);
            if (key2 == -1)
            {
                printf("Edge does not exist!\n");
                return NULL;
            }
            else if (key2 != -1)
            {
                Node *temp2 = temp->GraphOut[key2];
                Node *prev, *prev3;
                while (strcmp(temp2->VertexID, origin) != 0 && temp2 != NULL)
                {
                    c = 1;
                    prev = temp2;
                    temp2 = temp2->Next;
                }
                if (strcmp(temp2->VertexID, origin) == 0)
                {
                    prev3 = temp2->Next;
                    free(temp2); // clears the particular edge in GraphOut
                    prev->Next = prev3;
                }
            }
            int key1 = hash_search(origin, temp->GraphIn);
            if (key1 == -1)
            {
                printf("Edge does not exist!\n");
                return NULL;
            }
            else if (key1 != -1)
            {
                Node *temp1 = temp->GraphIn[key1];
                Node *prev1, *prev2;
                while (temp1 != NULL && strcmp(temp1->VertexID, destiny) != 0) // checks whether matches to given VertexID or not
                {
                    c = 1;
                    prev1 = temp1;
                    temp1 = temp1->Next;
                }
                if (temp1 == NULL)
                {
                    printf("Edge does not exist!\n");
                    return NULL;
                }
                else if (strcmp(temp1->VertexID, destiny) == 0)
                {
                    prev2 = temp1->Next;
                    prev1->Next = prev2;
                    if (flag == 1)
                    {
                        return temp1; // as deleted edge is required  in 2nd shortes path this had done to made when required
                    }
                    if (flag == 0)
                    {
                        free(temp1); // clears the particular edge in GraphIn
                    }
                    
                }
                else
                {
                    printf("Edge does not exist!\n");
                    return NULL;
                }
            }
            
            break;
        }
        temp = temp->Next;
    }
    if (c == 0)
    {
        printf(" There is no edge in this TradeBank b/w given vertices \n ");
    }
    return NULL;
}
void delete_currency(ElemType TradeBank[50], ElemType VertexID[50], struct AllGraph *list) // deletes the nodes(currencies)  along with all the edges connected to them
{
    int n = list->NumBanks;
    int c = 0;
    PtrToGraph tem = list->GraphPtr;

    while (strcmp(tem->TradeBank, TradeBank) != 0 && tem->Next != NULL)
    {
        tem = tem->Next;
    }
    int key = hash_search(VertexID, tem->GraphIn);
    if (key != -1)
    {
        Node *temp = tem->GraphIn[key];
        if (temp != NULL)
        {
            c = 1;
            Node *temp1 = temp;
            Node *tempo = temp, *next;
            while (temp != NULL)
            {
                int key1 = hash_search(temp->VertexID, tem->GraphIn);
                if (key1 != -1)
                {
                    Node *t = tem->GraphOut[key1];
                    while (t->Next != NULL)
                    {
                        if (strcmp(t->Next->VertexID, VertexID) == 0)
                        {
                            Node *te = t->Next->Next;
                            free(t->Next); //  clears side connected edges in GraphOut  which can be find through connections in GraphIn
                            t->Next = te;
                            break;
                        }
                        t = t->Next;
                    }
                }
                temp = temp->Next;
            }
            if (tempo->Next != NULL)
            {
                tempo = tempo->Next;
                while (tempo != NULL)
                {
                    next = tempo->Next;
                    free(tempo);
                    tempo = next;
                }
            }
            tem->GraphIn[key]->Next = NULL;
            tem->GraphIn[key]->VertexID[0] = '\0';
            tem->GraphIn[key]->ConvRate = -2; // these indicates that the node is deleted
        }
        Node *temp2 = tem->GraphOut[key];
        if (temp2->Next != NULL)
        {
            c = 1;
            Node *temp3 = temp2;
            Node *tempo = temp2, *next;
            while (temp2 != NULL)
            {
                int key1 = hash_search(temp2->VertexID, tem->GraphOut);
                if (key1 != -1)
                {
                    Node *t = tem->GraphIn[key1];
                    while (t->Next != NULL)
                    {
                        if (strcmp(t->Next->VertexID, VertexID) == 0)
                        {
                            Node *te = t->Next->Next;
                            free(t->Next); //  clears side connected edges in GraphIn  which can be find through connections in GraphOut
                            t->Next = te;
                            break;
                        }
                        t = t->Next;
                    }
                }
                temp2 = temp2->Next;
            }
            if (tempo->Next != NULL)
            {
                tempo = tempo->Next;
                while (tempo != NULL)
                {
                    next = tempo->Next;
                    free(tempo);
                    tempo = next;
                }
            }
            tem->GraphOut[key]->Next = NULL;
            tem->GraphOut[key]->VertexID[0] = '\0';
            tem->GraphOut[key]->ConvRate = -2; // these indicates that the node is deleted
        }
    }
    if (c == 0)
    {
        printf(" There is no currency of this name in any TradeBank  \n ");
    }
    else if (c == 1)
    {
        tem->NumVertex--;
    }
}
