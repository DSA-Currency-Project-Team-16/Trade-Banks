#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
int hash_fun(char string[50]);
PtrToNext delete_edge(ElemType TradeBank[50], ElemType origin[50], ElemType destiny[50], struct AllGraph *list, int flag);
void delete_TradeBank(ElemType TradeBank[50], struct AllGraph *list);
void delete_currency(ElemType TradeBank[50], ElemType VertexID[50], struct AllGraph *list);

int hash_fun(char string[50]) //  gives a hash key to find in which the edges are stored ( adjecency list)
{
    int sum = 0;
    int l = strlen(string);
    for (int i = 0; i < l; i++)
    {
        sum = (sum * 33 + string[i]) % tablesize;
    }
    return sum % tablesize;
}
void delete_TradeBank(ElemType TradeBank[50], struct AllGraph *list) // deletes a TradeBank
{
    int n = list->NumBanks;
    int c = 0;
    // for (int i = 0; i < n; i++)
    // {
    PtrToGraph temp = list->GraphPtr;
    PtrToGraph prev = temp;
    while (temp != NULL)
    {
        if (strcmp(temp->TradeBank, TradeBank) == 0) // checks whether matches to given TradeBank or not
        {
            c = 1;
            Node *t1 = temp->GraphIn[0];
            while (t1->Next != NULL)
            {
                delete_currency(temp->TradeBank, t1->VertexID, list); // deletes the  adjecency list we made for GraphIn  in edges
            }
            Node *t2 = temp->GraphOut[0];
            while (t2->Next != NULL)
            {
                delete_currency(temp->TradeBank, t2->VertexID, list); // deletes the  adjecency list we made for GraphOut  in edges
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
            int key = hash_fun(origin);
            Node *temp1 = temp->GraphIn[key]->Next;
            Node *prev1, *prev2;
            while (strcmp(temp1->VertexID, destiny) != 0) // checks whether matches to given VertexID or not
            {
                c = 1;
                prev1 = temp1;
                temp1 = temp1->Next;
            }
            prev2 = temp1->Next;
            if (flag == 1)
            {
                return temp1; // as deleted edge is required  in 2nd shortes path this had done to made when required
            }
            if (flag == 0)
            {
                free(temp1); // clears the particular edge in GraphIn
            }
            prev1->Next = prev2;

            int key = hash_fun(destiny);
            Node *temp2 = temp->GraphOut[key]->Next;
            Node *prev, *prev3;
            while (strcmp(temp2->VertexID, origin) != 0)
            {
                c = 1;
                prev = temp2;
                temp2 = temp2->Next;
            }
            prev2 = temp2->Next;
            free(temp2); // clears the particular edge in GraphOut
            prev->Next = prev3;
        }
        temp = temp->Next;
    }
    if (c == 0)
    {
        printf(" There is no edge in this TradeBank b/w given vertices \n ");
    }
}
void delete_currency(ElemType TradeBank[50], ElemType VertexID[50], struct AllGraph *list) // deletes the nodes(currencies)  along with all the edges connected to them
{
    int n = list->NumBanks;
    int c = 0;
    int key = hash_fun(VertexID);

    PtrToGraph tem = list->GraphPtr;
    while (strcmp(tem->TradeBank, TradeBank) != 0)
    {
        tem = tem->Next;
    }
    // while (tem != NULL)
    // {
    Node *temp = tem->GraphIn[key];
    if (temp->Next != NULL)
    {
        c = 1;
        Node *temp1 = temp->Next;
        while (temp->Next != NULL)
        {
            int key1 = hash_fun(temp->Next->VertexID);
            Node *t = tem->GraphOut[key1];
            while (t->Next != NULL)
            {
                if (strcmp(t->Next->VertexID, VertexID) == 0)
                {
                    Node *te = t->Next->Next;
                    free(t->Next); //  clears side connected edges in GraphOut  which can be find through connections in GraphIn
                    t->Next = t->Next->Next;
                    break;
                }
            }
        }
        temp1 = NULL;
        free(temp1); // clears edges directed inwards
    }
    Node *temp2 = tem->GraphOut[key];
    if (temp2->Next != NULL)
    {
        c = 1;
        Node *temp3 = temp2->Next;
        while (temp2->Next != NULL)
        {
            int key1 = hash_fun(temp2->Next->VertexID);
            Node *t = tem->GraphIn[key1];
            while (t->Next != NULL)
            {
                if (strcmp(t->Next->VertexID, VertexID) == 0)
                {
                    Node *te = t->Next->Next;
                    free(t->Next); //  clears side connected edges in GraphIn  which can be find through connections in GraphOut
                    t->Next = t->Next->Next;
                    break;
                }
            }
        }
        temp3 = NULL;
        free(temp3); // clears edges directed outwards
    }
    // tem = tem->Next;
    // }
    if (c == 0)
    {
        printf(" There is no currency of this name in any TradeBank  \n ");
    }
}
