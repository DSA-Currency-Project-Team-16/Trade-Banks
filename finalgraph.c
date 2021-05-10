#include "finalgraph.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <sys/time.h>
#define y 33
#define del -2
#define STRLEN 50
#define NULL_CHAR '\0' //Null character
#define tablesize 307  //size of HashTable
#define del -2
#define y 33
#define INFTY 100000000
typedef long long DistType;         // Data type of distances
typedef struct MinHeap *HeapPtr;    //Points to whole Heap
typedef struct HeapNode *HeapNdPtr; // To individual nodes of heap
typedef char ElemType;              //used for Currency names

struct HeapNode
{
    DistType Dist;
    ElemType VertexID[STRLEN];
    ElemType PrevID[STRLEN];
};

typedef struct path *PathPtr;
struct path //linked list
{
    ElemType CurrID[STRLEN];
    PathPtr Next;
};

PathPtr AddPath(PathPtr Path, ElemType Vertex[]);
void FreePath(PathPtr Path);

struct MinHeap
{
    HeapNdPtr *arr;
    int *Pos;
    int NumElems;
};
void FreeHeap(HeapPtr Heap, int NumVertices); //frees the heap
void MinHeapify(HeapPtr Heap, int parIndex, PtrToNext *AdjList);
void MinHeapBuild(HeapPtr Heap, PtrToNext *AdjList);
HeapNdPtr ExtractMin(HeapPtr Heap, PtrToNext *AdjList);
bool IsEmptyHeap(HeapPtr Heap);
HeapPtr InitHeap(PtrToNext *AdjList, int NumVertices, int HtSize, ElemType Source[]);
void DecreaseKey(HeapPtr Heap, int index, DistType NewWt, PtrToNext *AdjList);

DistType FindDist(HeapPtr Heap, int DestID);
PathPtr FindPath(HeapPtr Heap, ElemType Source[], ElemType Dest[], PathPtr Path, PtrToNext *AdjList);
PathPtr PrintPath(HeapPtr Heap, ElemType Source[], ElemType Dest[], PathPtr Path, PtrToNext *AdjList);
void PrintDist(HeapPtr Heap, int StartID, int DestID);

HeapPtr swap(HeapPtr Heap, int index1, int index2, PtrToNext *AdjList);
int hash(ElemType string[50], PtrToNext *G); //returns index of the Vertex
PathPtr Dijkstra(PtrToNext *AdjList, ElemType Source[], ElemType Dest[], int NumVertices, int flag, DistType *MinDist);
//if flag == 1, prints the shortest path & dist, if flag == -1, finds the shortest path & dist but doesn't print it, else(flag = 0), finds dist(updates MinDist).
void ShortestPath(struct AllGraph *Pointer, ElemType Source[], ElemType Dest[]);
PathPtr Second_Shortest_Path_Of_Graph(PtrToGraph TradeBank, char *Currency1, char *Currency2, long long ShortestPathLength, long long *SecondShortestCost, TradeBankListPtr ListOfTradeBanks);
long long Second_Shortest_Conversion(TradeBankListPtr ListOfTradeBanks, char *Currency1, char *Currency2);
void print_existing_path(PathPtr P);
int hash_fun(ElemType string[50]) // Hash function maps the given string to a position in the hash table
{                                 // using Horner's rule and returns the position.
    int sum = 0;                  // takes name of currency(string) as input
    int l = strlen(string);
    for (int i = 0; i < l; i++)
    {
        sum = (sum * y + string[i]) % tablesize;
    }
    return sum % tablesize;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////

int hash_search(ElemType string[50], PtrToNext *G) // Search function finds the location of the given string in the given hashtable
{                                                  // and returns the position if found.
    int key = hash_fun(string);                    // otherwise returns -1
    if (strcmp(G[key]->VertexID, string) == 0)     // takes name of currency(string) and pointer to hashtable(PtrToNext) as inputs
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

PtrToNext CreateNode(ElemType VertexID[50], int ConvRate) // Creates a Node(allocates memory) and initialises pointer to NULL and returns it
{                                                         // takes name of currency(string) and conversion rate as inputs
    Node *N = malloc(sizeof(Node));
    strcpy(N->VertexID, VertexID);
    N->ConvRate = ConvRate;
    N->Next = NULL;
    return N;
}

void AddGraph(ElemType TradeBank[50], AllGraph *list) // Creates a graph using CreateGraph function and adds it to the list of TradeBanks
{                                                     // takes name of TradeBank(string) and list of graphs as inputs
    PtrToGraph B = CreateGraph(TradeBank);
    B->Next = list->GraphPtr;
    list->GraphPtr = B;
    list->NumBanks++;
    printf("TradeBank %s has been successfully created!\n", B->TradeBank);
}

PtrToGraph CreateGraph(ElemType TradeBank[50]) // Creates a graph and initialises the head pointer(to the linked list associated with each vertex)
{                                              //  to NULL and returns a pointer to graph
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
        B->GraphIn[i]->ConvRate = -1;      // to differentitate vertex and node(of linked list)
        B->GraphIn[i]->VertexID[0] = '\0'; // useful in other functions
        B->GraphOut[i]->Next = NULL;
        B->GraphOut[i]->ConvRate = -1;
        B->GraphOut[i]->VertexID[0] = '\0';
    }
    B->check = (bool *)malloc(sizeof(bool) * 200);
    for (int i = 0; i < 200; i++)
        B->check[i] = false;
    return B;
}

void InsertNode(ElemType TradeBank[50], ElemType VertexID[50], AllGraph *list) // Adds a currency to the given TradeBank
{
    AllGraph *temp = list;
    while (temp->GraphPtr) // To traverse the list of graphs
    {
        if (strcmp(temp->GraphPtr->TradeBank, TradeBank) == 0) // to check whether the given TradeBank exists
        {
            if (hash_search(VertexID, temp->GraphPtr->GraphIn) >= 0) // To check whether the given currency already exists
            {
                printf("Currency already exists..\n");
                return;
            }
            int key = hash_fun(VertexID);

            while (temp->GraphPtr->GraphIn[key]->VertexID[0] != '\0') // Traversing the hashtable until a vacant position is found
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

void InsertEdge(ElemType TradeBank[50], ElemType C1[50], ElemType C2[50], int ConvRate, AllGraph *list) // Adds a currency conversion between the given two currencies
{
    AllGraph *temp = list;
    while (temp->GraphPtr) // To traverse the list of graphs
    {
        if (strcmp(temp->GraphPtr->TradeBank, TradeBank) == 0) // to check whether the given TradeBank exists
        {
            int key1 = hash_search(C1, temp->GraphPtr->GraphIn); // To check whether the given currency(source) exists in the TradeBank
            if (key1 == -1)
            {
                printf("INVALID OPERATION !!\n");
                return;
            }

            int key2 = hash_search(C2, temp->GraphPtr->GraphOut); // To check whether the given currency(destination) exists in the TradeBank
            if (key2 == -1)
            {
                printf("INVALID OPERATION !!\n");
                return;
            }

            PtrToNext nodeIn = CreateNode(C2, ConvRate); // Creates a Node(to insert it in the linkedlist of edges of that particular vertex)
            // nodeIn->ConvRate = ConvRate;

            PtrToNext nodeOut = CreateNode(C1, ConvRate); // Creates a Node(to insert it in the linkedlist of edges of that particular vertex)
            // nodeOut->ConvRate = ConvRate;

            PtrToNext t1 = temp->GraphPtr->GraphIn[key1]->Next;
            while (t1) // To check whether the given currency conversion already exists in the TradeBank
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

            nodeIn->Next = temp->GraphPtr->GraphIn[key1]->Next; // Inserts the currency conversion in the linkedlist associated with the source vertex
            temp->GraphPtr->GraphIn[key1]->Next = nodeIn;
            temp->GraphPtr->GraphIn[key1]->ConvRate = 0;

            nodeOut->Next = temp->GraphPtr->GraphOut[key2]->Next; // Inserts the currency conversion in the linkedlist associated with the destination vertex
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

void PrintGraph(AllGraph *list)
{
    while (list->GraphPtr)
    {
        printf("TradeBank: %s (NumVertex: %d)\n", list->GraphPtr->TradeBank, list->GraphPtr->NumVertex);

        for (int i = 0; i < tablesize; i++)
        {
            if (list->GraphPtr->GraphIn[i]->VertexID[0] != '\0')
            {
                printf("Currency: %s ", list->GraphPtr->GraphIn[i]->VertexID);
                Node *temp = list->GraphPtr->GraphIn[i]->Next;
                while (temp)
                {
                    printf("%s(%d) ", temp->VertexID, temp->ConvRate);
                    temp = temp->Next;
                }
                printf("\n");
                free(temp);
            }
        }
        printf("\n");

        for (int i = 0; i < tablesize; i++)
        {
            if (list->GraphPtr->GraphOut[i]->VertexID[0] != '\0')
            {
                printf("Currency: %s ", list->GraphPtr->GraphOut[i]->VertexID);
                Node *temp = list->GraphPtr->GraphOut[i]->Next;
                while (temp)
                {
                    printf("%s(%d) ", temp->VertexID, temp->ConvRate);
                    temp = temp->Next;
                }
                free(temp);
                printf("\n");
            }
        }
        printf("\n");

        list->GraphPtr = list->GraphPtr->Next;

        printf("\n");
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

                Node *t2 = temp->GraphOut[i], *next1;
                while (t2 != NULL)
                {
                    next1 = t2->Next;
                    free(t2);
                    t2 = next1;
                }
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
        printf("There is no TradeBank with this name \n ");
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
    if (strcmp(tem->TradeBank, TradeBank) == 0)
    {
        int key = hash_search(VertexID, tem->GraphIn);
        if (key != -1)
        {
            Node *temp = tem->GraphIn[key];
            // printf("   %d\n", key);
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
    else
        printf("TradeBank does not exist!\n");
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int tradecheck[1000] = {0}; //check number of currencies in each tradebank
int numtradebanks = 0;      //check how many tradebank names havebeen taken
bool tradenamecheck[1000] = {false};
void printcurrency(PtrToGraph bank)
{
    struct timeval t1;
    gettimeofday(&t1, NULL);
    srand((t1.tv_sec * t1.tv_usec)); //defined for more and better randomisation

    int i, j, k, l;
    char a[50], b[50];
    FILE *fp = fopen("currency.txt", "r"); //opens the currency.txt file to take input from
    FILE *fp1 = fopen("dict.txt", "a");    //output file
    if (fp == NULL)
    {
        printf("ERROR"); //checks if file is present or not if not then prints ERROR
        return;
    }
    if (fp1 == NULL)
    {
        printf("ERROR"); //checks if file is present or not if not then prints ERROR
        return;
    }
    k = rand() % 200;
    if (k == 0)
        k = 200;
    if (bank->check[k] == false) //checks for a specific tradebank if the name has been taken or not
    {
        for (j = 0; j < k - 1; j++)

            fscanf(fp, "%s", a);
        fscanf(fp, "%s", b);
        fprintf(fp1, "%s\n", b);
        bank->check[k] = true;
    }
    else //if the name has been taken then it traverses until a currency not taken is encountered
    {

        int y1 = (k + 1) % 201;

        for (;;)
        {
            if (y1 == 0)
                y1 = 1;
            if (bank->check[y1] == false) //traverses until a false case is encountered
            {
                bank->check[y1] = true;
                break;
            }
            y1 = (y1 + 1) % 201;
        }
        for (j = 0; j < y1 - 1; j++)
            fscanf(fp, "%s", a);
        fscanf(fp, "%s", b);
        fprintf(fp1, "%s\n", b); //prints the obtained output in the output file
    }

    fclose(fp); //closes the opened file
    fclose(fp1);
    return;
}

void printradebank()
{
    struct timeval t1;
    gettimeofday(&t1, NULL); //defined for more and better randomisation
    srand((t1.tv_sec * t1.tv_usec));

    int i, j, k;
    char a[50], b[50];
    FILE *fp = fopen("tradebanks.txt", "r"); //opens the tradebank txt file just like the currency.txt file for reading and taking the input
    FILE *fp1 = fopen("dict.txt", "a");
    k = rand() % 1000;
    if (fp == NULL)
    {
        printf("ERROR"); //checks if file is present or not if not then prints ERROR
        return;
    }
    if (fp1 == NULL)
    {
        printf("ERROR"); //checks if file is present or not if not then prints ERROR
        return;
    }
    if (tradenamecheck[k] == false) //cheks if the name has been taken by the user or not
    {
        for (j = 0; j < k - 1; j++)
            fscanf(fp, "%s", a);
        fscanf(fp, "%s", b);
        fprintf(fp1, "%s\n", b);
        tradenamecheck[k] = true; //marks the name as taken i.e. true
    }
    else
    {

        int y1 = (k + 1) % 1000;

        for (;;) //if the name is not taken it iterates over the file and reads until such a tradebankname is countered which has not been read
        {
            if (y1 == 0)
                y1 = 1;
            if (tradenamecheck[y1] == false)
            {
                tradenamecheck[y1] = true; //marks the tradebank name as taken
                break;
            }
            y1 = (y1 + 1) % 1000;
        }
        for (j = 0; j < y1 - 1; j++)
            fscanf(fp, "%s", a);
        fscanf(fp, "%s", b);
        fprintf(fp1, "%s\n", b); //prints the output encountered in the file
    }

    fclose(fp); //closes all the files opened
    fclose(fp1);
    return;
}
void printnum() //to generate random numbers
{
    struct timeval t1;
    int x;
    gettimeofday(&t1, NULL); //declared for better randomisation
    srand((t1.tv_sec * t1.tv_usec));
    FILE *fp = fopen("dict.txt", "a");
    if (fp == NULL)
    {
        printf("ERROR"); //checks if file is present or not if not then prints ERROR
        return;
    }
    x = rand() % 400;
    if (x == 0)
        x = 230;
    fprintf(fp, "%d\n", x);
    fclose(fp); //closes the input file
}
//BONUS PART FOR DETECTING CYCLES
// it is a directed graph so no DSU can be used ,hence i have implemented DFS to do .Used backtracking approach to solve the above problem

bool visited[200] = {false}; //bool array marking all nodes as unvisited
/*
dfs_check is a supplementry function which performs dfs on each node given input by dfs_main and tries to check for cycle by recursively running dfs on it
until either an vertice occured earlier has occured or everything backtracks to false and it returns false as the output to it ,i have further explained 
 usage of each variable line by line.
*/
bool dfs_check(PtrToGraph P, PtrToNext N, PtrToNext *Adjlist, PtrToNext *List, int hashval)
{
    if (hashval == -1)
        return false;
    PtrToNext p = Adjlist[hashval]; //AdjList basically as the name says all stores neighbouring vertices for all
    if (p == NULL)                  //p is the pointer in hashtable of next neighbour of a given vertex
        return false;
    if (p->VertexID[0] == '\0')
        return false;
    if (visited[p->ConvRate] == true) //here it checks whether the vertex has been visited or not,if no then it continues else returns true;
        return true;
    visited[p->ConvRate] = true; //marks the given node as visited
    PtrToNext next = p->Next;
    bool check = false;
    for (;;)
    {
        if (next == NULL)
            break;
        if (next->VertexID[0] == '\0') //checks if the  further neghbours of the vertice exist or not
            break;
        check = dfs_check(P, next, Adjlist, List, hash(next->VertexID, Adjlist)); //recursive function for checking for further .
        if (check == true)                                                        //if on further iterations a cycle is found then simply a cycle exists and return it ASAP
            return true;
        next = next->Next; //iterate further neighbours
    }
    visited[p->ConvRate] = false; //after all posibilities no cycle is found hence backtrack it and set it to false
    return false;                 //no cycle found after all checking,return false
}
bool dfs_main(int v, PtrToGraph P, PtrToNext *AdjList, PtrToNext *List) //the main graph dfs function which iterates over v and checks for each of the v
{
    bool check = false;
    int i, k;
    Node *p;                //node to point to the vertex
    Node *prev;             //just a check node
    int hashval;            //hashvalue of the node
    for (i = 0; i < v; i++) //iterative loop to run on dfs on nodes until a cycle is found
    {
        visited[i] = true; //mark the node as visited by dfs
        p = List[i];

        if (p == NULL) //checking whether the neighbour exists ,if not then go to the next node and unvisit it
        {
            visited[prev->ConvRate] = false;
            continue;
        }
        if (p->VertexID[0] == '\0')
        {
            visited[prev->ConvRate] = false;
            continue;
        }
        prev = p;
        p = p->Next; //first neighbour of node

        if (p == NULL) //checking whether the neighbour exists ,if not then go to the next node and unvisit it
        {
            visited[prev->ConvRate] = false;
            continue;
        }
        if (p->VertexID[0] == '\0')
        {
            visited[prev->ConvRate] = false;
            continue;
        }
        hashval = hash(p->VertexID, AdjList); //hashvalue of the next neighbour of the function
        for (;;)
        {
            if (p == NULL) //checking whether the neighbour exists ,if not then go to the next node and unvisit it
                break;
            if (hashval == -1)
                break;
            if (p->VertexID[0] == '\0')
                break;
            check = dfs_check(P, p, AdjList, List, hashval); //call the supplementry dfs function to run dfs on its neighbour and check further
            if (check == true)                               //if a cycle is found then return true ASAP else iterate and go to the next neighbour of the given node
                return true;
            p = p->Next;
        }
        visited[i] = false; //no cycle found pertaining to this node so unvisit it and goto the next node
    }
    return false; //if no cycle has been found then just return false
}
PtrToGraph searchbank(AllGraph *A, char b[50]) //A supplementry function to find pointer to the tradebank if it exists in the set of graphs,if not then return NULL
{
    PtrToGraph p = A->GraphPtr;
    int i, n = A->NumBanks;
    if (p == NULL)
        return NULL;
    for (i = 0; i < n; i++)
    {
        if (strcmp(p->TradeBank, b) == 0) //checking each tradebank names in the graph iteratively
            return p;
        p = p->Next;
    }
    return NULL; //if no such tradebank exists then return NULL
}
int main()
{
    int n, j, i, currnum, currencyID, currnum2;
    FILE *fp;
    struct timeval t1;
    AllGraph *A = malloc(sizeof(A)); //the allgraph variable which contains the whole graph
    A->NumBanks = 0;
    //TEST CASES PLEASE DELETE
    AddGraph("bank1", A);
    InsertNode("bank1", "abc", A);
    InsertNode("bank1", "def", A);
    InsertNode("bank1", "ghi", A);
    InsertNode("bank1", "jkl", A);
    InsertNode("bank1", "mno", A);
    InsertNode("bank1", "pqr", A);
    InsertEdge("bank1", "abc", "ghi", 3, A);
    InsertEdge("bank1", "ghi", "pqr", 7, A);
    InsertEdge("bank1", "mno", "jkl", 1, A);
    InsertEdge("bank1", "pqr", "abc", 8, A);
    AddGraph("bank2", A);
    InsertNode("bank2", "srija", A);
    InsertNode("bank2", "divya", A);
    InsertNode("bank2", "keerthana", A);
    InsertNode("bank2", "amara", A);
    InsertNode("bank2", "kavya", A);
    InsertNode("bank2", "nikki", A);
    InsertNode("bank2", "shriya", A);
    InsertEdge("bank2", "divya", "amara", 509, A);
    InsertEdge("bank2", "amara", "srija", 24, A);
    InsertEdge("bank2", "kavya", "nikki", 100, A);
    InsertEdge("bank2", "srija", "shriya", 4574, A);
    InsertEdge("bank2", "srija", "divya", 1389, A);
    //InsertEdge("bank2", "srija", "keerthana", 94958, A);
    InsertEdge("bank2", "srija", "amara", 949, A);
    InsertEdge("bank2", "amara", "keerthana", 949, A);
    AddGraph("bank3", A);
    InsertNode("bank3", "cat", A);
    InsertNode("bank3", "rat", A);
    InsertNode("bank3", "tom", A);
    InsertNode("bank3", "jerry", A);
    InsertNode("bank3", "jerry", A);
    InsertEdge("bank3", "cat", "tom", 57878, A);
    InsertEdge("bank3", "rat", "jerry", 4385, A);
    AddGraph("string", A);
    InsertNode("string", "a", A);
    InsertNode("string", "b", A);
    InsertNode("string", "c", A);
    InsertNode("string", "d", A);
    InsertNode("string", "e", A);
    InsertNode("string", "f", A);
    InsertNode("string", "g", A);
    InsertNode("string", "h", A);
    InsertEdge("string", "e", "b", 1, A);
    InsertEdge("string", "b", "c", 12, A);
    InsertEdge("string", "e", "b", 123, A);
    InsertEdge("string", "a", "e", 1, A);
    InsertEdge("string", "e", "f", 1, A);
    InsertEdge("string", "f", "a", 1, A);
    InsertEdge("string", "e", "h", 1, A);
    InsertEdge("string", "h", "e", 123, A);
    InsertEdge("string", "a", "c", 1234, A);
    InsertEdge("string", "c", "h", 12, A);
    InsertEdge("string", "d", "a", 1, A);

    //     PrintGraph(*A);
    //     delete_currency("bank3","tom", A);
    //     delete_currency("bank3","jerry", A);
    printf("Key for using the file:-\nPress 1 to add a Tradebank\nPress 2 to Add currencies for a given tradebank\nPress 3 to Get conversion rates for each tradebank(Add edge)\n");
    printf("Press 4 to delete a tradebank\nPress 5 to delete currency from a tradebank\nPress 6 to delete a currency conversion rate(edge)\nPress 7 to detect cycle in  graph\n");
    printf("Press 8 to find the second best path\nPress 9 to find the best path \nPress 10 to add edge weight manually\nPress 11 to exit the input programme\n");
    gettimeofday(&t1, NULL);
    srand((t1.tv_sec * t1.tv_usec));
    int x = 0;

    char a[50], tradename[50], currname1[50], currname2[50];
    for (;;) //all functionalities have been implemented along the given number
    {
        printf("Enter Operation Number:");
        scanf("%d", &j);
        fp = fopen("dict.txt", "w");
        fclose(fp);
        if (j == 1)
        {
            char tradebankname[50];

            if (numtradebanks + 1 > 1000)
            {
                printf("The number of tradebanks  have exceeded\n");
                goto end;
            }
            printradebank();
            numtradebanks++;
            fp = fopen("dict.txt", "r");
            fscanf(fp, "%s", tradebankname);
            fclose(fp);
            AddGraph(tradebankname, A);
        }
        else if (j == 2)
        {

            if (A->NumBanks == 0)
            {
                printf("Add a tradebank:)\n");
                continue;
            }
            printf("Enter  tradebanks ID and number of currencies needed: ");
            scanf("%s%d", a, &currnum);
            PtrToGraph pp = searchbank(A, a);
            char currnames[currnum][50];
            if (pp == NULL)
            {
                printf("The tradebank does not exist\n");
                goto end;
            }
            if (tradecheck[n] + currnum > 200)
            {
                printf("The currencies have exceeded\n");
                goto end;
            }

            for (x = 0; x < currnum; x++)
                printcurrency(pp);
            tradecheck[n] += currnum;
            fp = fopen("dict.txt", "r");
            for (int i = 0; i < currnum; i++)
            {
                fscanf(fp, "%s", currnames[i]);
                InsertNode(a, currnames[i], A);
            }
            fclose(fp);
        }
        else if (j == 3)
        {
            char tradebankname[50];
            int value;
            //     printf("Enter number of currencies:");
            //     scanf("%d", &n);
            //     for (i = 0; i < n; i++)
            printf("Enter tradebank name Currency ID1 and Currency ID2: ");
            scanf("%s%s%s", tradebankname, currname1, currname2);
            printnum();
            fp = fopen("dict.txt", "r");
            fscanf(fp, "%d", &value);
            fclose(fp);
            InsertEdge(tradebankname, currname1, currname2, value, A);
        }
        else if (j == 5)
        {
            char tradebankname[50], currname[50];
            printf("Tradebank ID:\n ");
            scanf("%s", tradebankname);
            printf("Currency ID");
            scanf("%s", currname);
            fp = fopen("tradebanks.txt", "r");
            for (int i = 0; i < 1000; i++)
            {
                char name[50];
                fscanf(fp, "%s", name);
                if (strcmp(name, tradebankname) == 0)
                {
                    tradecheck[i]--;
                    break;
                }
            }
            fclose(fp);
            delete_currency(tradebankname, currname, A);
        }
        else if (j == 4)
        {
            printf("Tradebank ID:\n ");

            scanf("%s", tradename);
            numtradebanks--;
            fp = fopen("tradebanks.txt", "r");
            for (int i = 0; i < 1000; i++)
            {
                char name[50];
                fscanf(fp, "%s", name);
                if (strcmp(name, tradename) == 0)
                {
                    tradenamecheck[i] = false;
                    break;
                }
            }
            fclose(fp);
            delete_TradeBank(tradename, A);
            A->NumBanks--;
        }
        else if (j == 6)
        {
            printf("Tradebank ID:\n");
            scanf("%s", tradename);
            printf("Currency ID1 and Currency ID2: ");
            scanf("%s%s", currname1, currname2);
            delete_edge(tradename, currname1, currname2, A, 0);
        }
        else if (j == 7) //functionality for searching for cycle
        {
            PtrToGraph P;
            printf("Tradebank ID: ");
            scanf("%s", tradename);
            P = searchbank(A, tradename); //Searching for the pointer to a given tradebank entered by the user

            if (P == NULL)
            {
                printf("No such tradebank exists\n");
                continue;
            }

            int i, v = P->NumVertex, count = 0;

            PtrToNext *Adjlist = P->GraphIn; //Adjacency list for the given tradebank

            PtrToNext List[v]; //This list is important as it compresses the given hashtable and stores pointers to all the nodes in a hashtable

            for (i = 0; i < 307; i++) //iteratively finding the nodes and compressing / storing them in the List array
            {
                if (Adjlist[i]->VertexID[0] != '\0')
                {
                    List[count] = Adjlist[i];
                    Adjlist[i]->ConvRate = count;
                    count++;
                }
            }
            if (count == 0)
            {
                printf("Add nodes in the tradebank\n");
                continue;
            }
            bool check = dfs_main(count, P, Adjlist, List); //dfs function ran on the code to check for cycles ,if found it returns true else returns false
            if (check == true)
                printf("Yes cycle exists in the given tradebank\n");
            else
                printf("No cycle found in the given tradebank\n");
        }

        else if (j == 8)
        {
            printf("Currency ID1 and Currency ID2");
            scanf("%s%s", currname1, currname2);
            Second_Shortest_Conversion(A, currname1, currname2);
        }
        else if (j == 9)
        {
            printf("Currency ID1 and Currency ID2");
            scanf("%s%s", currname1, currname2);
            ShortestPath(A, currname1, currname2);
        }
        else if (j == 10)
        {
            int edgeweight;
            printf("Enter Tradebank ID:");
            scanf("%s", tradename);
            printf("Enter Currency ID1 and Currency ID2: ");
            scanf("%s%s", currname1, currname2);
            printf("Enter an edgeweight between 0 and 500: ");
            scanf("%d", &edgeweight);
            if (edgeweight <= 0 || edgeweight > 500)
            {
                printf("Perform the operation again\n");
                continue;
            }
            InsertEdge(tradename, currname1, currname2, edgeweight, A);
        }
        else if (j == 11)
        {
            printf("Printed graph is-\n");
            PrintGraph(A);
        }
        else if (j == 12)
        {
            printf("Thanks for using the file :)\n");
            return 0;
        }
        else
        {
            printf("Enter Operation Number correctly again:)\n");
        }
    end:;
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int hash(ElemType string[50], PtrToNext *G) //linear probing
{
    int sum = 0;
    int l = strlen(string); //calculates hashvalue
    for (int i = 0; i < l; i++)
    {
        sum = (sum * y + string[i]) % tablesize;
    }
    int key = sum % tablesize;
    int initalkey = key;
    if (strcmp(G[key]->VertexID, string) == 0)
        return key;
    else
    {
        while (G[key]->VertexID[0] != '\0' || G[key]->ConvRate == del) // linear probing
        {
            if ((strcmp(G[key]->VertexID, string) == 0))
                return key;
            key++;
            key = key % tablesize;
            if (key == initalkey)
                break;
        }
        //printf( "ok %s\n", string);
        return -1;
    }
}
PathPtr Dijkstra(PtrToNext *AdjList, ElemType Source[], ElemType Dest[], int NumVertices, int flag, long long *MinDist)
{
    //Dijkstra Heap implementation code
    HeapPtr Heap = InitHeap(AdjList, NumVertices, tablesize, Source);
    int hashvalue = hash(Source, AdjList);
    if (hashvalue == -1)
    {
        if (flag == 1)
            printf("No Path Exists\n");
        if (flag == -1)
            return NULL;

        return NULL;
    }
    hashvalue = hash(Dest, AdjList);
    if (hashvalue == -1)
    {
        if (flag == 1)
            printf("No Path Exists\n");
        if (flag == -1)
            return NULL;
        *MinDist = INFTY;
        return NULL;
    }
    int startID = AdjList[hash(Source, AdjList)]->ConvRate; // contains position in heap

    if (startID < 0) //Source Vertex Not found
    {
        if (flag == -1)
            *MinDist = 20 * INFTY + 5;

        else if (flag == 1)
            printf("The source currency(PtrToList) does not exist\n");

        //if flag == 0, does nothing

        return NULL;
    }
    int count = startID;

    DecreaseKey(Heap, count, 0, AdjList);
    HeapNdPtr Vertex = NULL;

    while (!IsEmptyHeap(Heap))
    {
        Vertex = ExtractMin(Heap, AdjList);
        if (strcmp(Vertex->VertexID, Dest) == 0) // Dest = name of currency we were looking for
            break;

        PtrToNext Curr = AdjList[hash(Vertex->VertexID, AdjList)]->Next;
        while (Curr != NULL)
        {

            int index = AdjList[hash(Curr->VertexID, AdjList)]->ConvRate; // contains the indexes of the nodes in the heap

            if (Curr->ConvRate + Vertex->Dist < Heap->arr[index]->Dist) //updating dist
            {
                strcpy(Heap->arr[index]->PrevID, Vertex->VertexID);
                DecreaseKey(Heap, index, Curr->ConvRate + Vertex->Dist, AdjList);
            }

            Curr = Curr->Next;
        }
    }

    if (IsEmptyHeap) //if Heap is empty, 2 chances, either the last vertex extracted was Dest/Dest isn't in the TradeBank
    {
        if (strcmp(Dest, Vertex->VertexID) != 0)
        {
            if (flag == 1)
                printf("The currency B does not exist\n");

            else if (flag == -1)
                *MinDist = 20 * INFTY + 5; //meaning that the dest vertex doesn't exist

            // flag = 0, do  nothing
            FreeHeap(Heap, NumVertices);
            return NULL;
        }
    }

    PathPtr Path = NULL;
    if (flag == 1)
    {
        // hashvalue = hash(Dest,AdjList);
        // if (hashvalue  == -1)
        // {
        //     *MinDist = INFTY;
        //     return NULL;
        // }
        if (*MinDist >= INFTY)
        {
            printf("There exists no path from %s to %s via any bank\n", Source, Dest);
            return NULL;
        }
        Path = PrintPath(Heap, Source, Dest, Path, AdjList);
        printf("End of Path\n");
        //printf("%lld\n", *MinDist);
        PrintDist(Heap, startID, AdjList[hash(Dest, AdjList)]->ConvRate);
        FreeHeap(Heap, NumVertices);

        return Path;
    }

    else if (flag == -1)
    {
        //printf("End of Path\n");
        //printf("%lld\n", *MinDist);
        //PrintDist(Heap, startID, AdjList[hash_fun(Dest)]->ConvRate);
        //printf("2\n");
        hashvalue = hash(Dest, AdjList);
        // if (hashvalue  == -1)
        // {
        //     *MinDist = INFTY;
        //     return NULL;
        // }
        if (Heap->arr[AdjList[hash(Dest, AdjList)]->ConvRate]->Dist < *MinDist)
            *MinDist = Heap->arr[AdjList[hash(Dest, AdjList)]->ConvRate]->Dist;
        //printf("3\n");
        if (*MinDist >= INFTY)
        {
            //printf("There exists no path from %s to %s via any bank\n", Source, Dest);
            FreeHeap(Heap, NumVertices);
            //printf("4\n");
            return NULL;
        }
        Path = FindPath(Heap, Source, Dest, Path, AdjList);
        FreeHeap(Heap, NumVertices);

        return Path;
    }

    //if flag == 0
    if (Heap->arr[AdjList[hash(Dest, AdjList)]->ConvRate]->Dist < *MinDist)
        *MinDist = Heap->arr[AdjList[hash(Dest, AdjList)]->ConvRate]->Dist;

    FreeHeap(Heap, NumVertices);
    return Path;
}

void ShortestPath(struct AllGraph *Pointer, ElemType Source[], ElemType Dest[])
{
    PtrToGraph CurrBank = Pointer->GraphPtr; //points to current tradebank
    DistType MinDist = 20 * INFTY + 5, temp;
    temp = MinDist;
    ElemType BankName[STRLEN] = {'\0'};

    while (CurrBank != NULL)
    {
        Dijkstra(CurrBank->GraphIn, Source, Dest, CurrBank->NumVertex, 0, &MinDist);
        if (temp != MinDist) //meaning that mindist was updated
        {
            strcpy(BankName, CurrBank->TradeBank);
            temp = MinDist;
        }

        CurrBank = CurrBank->Next;
    }

    CurrBank = Pointer->GraphPtr;
    while (CurrBank != NULL && strcmp(CurrBank->TradeBank, BankName)) //traversing the list of tradebanks
        CurrBank = CurrBank->Next;

    if (CurrBank == NULL)
    {
        printf("It is not possible to convert from %s to %s via any bank\n", Source, Dest);
        return;
    }
    //Prints shortest path and shortest path distance
    if (BankName[0] != NULL_CHAR)
    {
        printf("Use This bank : %s\n", CurrBank->TradeBank);
    }
    Dijkstra(CurrBank->GraphIn, Source, Dest, CurrBank->NumVertex, 1, &MinDist);
}

//////////////////////////////////Heap operations

void MinHeapify(HeapPtr Heap, int parIndex, PtrToNext *AdjList)
{
    int lIndex = (2 * parIndex) + 1;
    int rIndex = (2 * parIndex) + 2;
    int min = parIndex;

    if (lIndex < Heap->NumElems && Heap->arr[lIndex]->Dist < Heap->arr[min]->Dist)
        min = lIndex;

    if (rIndex < Heap->NumElems && Heap->arr[rIndex]->Dist < Heap->arr[min]->Dist)
        min = rIndex;

    if (min != parIndex)
    {
        Heap = swap(Heap, parIndex, min, AdjList);
        MinHeapify(Heap, min, AdjList);
    }
}

void MinHeapBuild(HeapPtr Heap, PtrToNext *AdjList)
{
    for (int i = Heap->NumElems / 2; i >= 1; i--)
        MinHeapify(Heap, i, AdjList);
}

HeapNdPtr ExtractMin(HeapPtr Heap, PtrToNext *AdjList)
{
    if (Heap->NumElems == 0)
        return NULL;

    Heap = swap(Heap, 0, Heap->NumElems - 1, AdjList);

    Heap->NumElems--;
    HeapNdPtr min = Heap->arr[Heap->NumElems]; // the element which is currently in the priority queue
    MinHeapify(Heap, 0, AdjList);

    return min;
}

void DecreaseKey(HeapPtr Heap, int index, DistType NewWt, PtrToNext *AdjList)
{
    Heap->arr[index]->Dist = NewWt; // Updating of weight

    //Essentially, we proceed in the opposite direction to normal Heapify
    int parIndex;
    while (index > 0)
    {
        parIndex = (index - 1) / 2;

        if (Heap->arr[index]->Dist < Heap->arr[parIndex]->Dist)
        {
            Heap = swap(Heap, index, parIndex, AdjList);
            index = parIndex; // moving up
        }

        else //then all elems of heap in right place
            break;
    }
}

HeapPtr swap(HeapPtr Heap, int index1, int index2, PtrToNext *AdjList)
{
    int temp_rate = AdjList[hash(Heap->arr[index1]->VertexID, AdjList)]->ConvRate; //swapping of positions in the AdjList
    AdjList[hash(Heap->arr[index1]->VertexID, AdjList)]->ConvRate = AdjList[hash(Heap->arr[index2]->VertexID, AdjList)]->ConvRate;
    AdjList[hash(Heap->arr[index2]->VertexID, AdjList)]->ConvRate = temp_rate;

    int tmp_pos = Heap->Pos[index1];
    Heap->Pos[index1] = Heap->Pos[index2];
    Heap->Pos[index2] = tmp_pos;

    HeapNdPtr tmp = Heap->arr[index1];
    Heap->arr[index1] = Heap->arr[index2];
    Heap->arr[index2] = tmp;

    return Heap;
}

bool IsEmptyHeap(HeapPtr Heap)
{
    if (Heap->NumElems == 0)
        return true;

    return false;
}

HeapPtr InitHeap(PtrToNext *AdjList, int NumVertices, int HtSize, ElemType Source[]) //initialises heap
{
    HeapPtr Heap = (HeapPtr)malloc(sizeof(struct MinHeap));
    assert(Heap != NULL);

    Heap->NumElems = NumVertices;
    Heap->arr = (HeapNdPtr *)malloc(NumVertices * sizeof(HeapNdPtr));
    assert(Heap->arr != NULL);

    for (int i = 0; i < NumVertices; i++)
    {
        Heap->arr[i] = (HeapNdPtr)malloc(sizeof(struct HeapNode));
        assert(Heap->arr[i] != NULL);
    }

    Heap->Pos = (int *)malloc(NumVertices * sizeof(int));
    assert(Heap->Pos != NULL);

    int count = 0;
    for (int i = 0; i < HtSize; i++)
    {
        if (AdjList[i]->VertexID[0] != NULL_CHAR) //checks if the element of hashtbl is a currency or not
        {
            Heap->arr[count]->PrevID[0] = NULL_CHAR;
            strcpy(Heap->arr[count]->VertexID, AdjList[i]->VertexID);

            AdjList[i]->ConvRate = count; //stores pos of heap elem in the adj list
            Heap->arr[count]->Dist = INFTY;
            Heap->Pos[count] = count;
            count++;
        }
    }

    return Heap;
}

void FreeHeap(HeapPtr Heap, int NumVertices)
{
    for (int i = 0; i < NumVertices; i++)
        free(Heap->arr[i]);

    free(Heap->Pos);
    free(Heap->arr);
    free(Heap);
}

///////////////////////////////////////////////////////

PathPtr AddPath(PathPtr Path, ElemType Vertex[])
{
    PathPtr tmp = (PathPtr)malloc(sizeof(struct path));
    assert(tmp != NULL);

    strcpy(tmp->CurrID, Vertex);
    tmp->Next = NULL;

    if (Path == NULL)
        return tmp;

    //else
    PathPtr Curr = Path;
    while (Curr->Next != NULL) //goto end of LL
        Curr = Curr->Next;

    Curr->Next = tmp;

    return Path;
}

void FreePath(PathPtr Path)
{
    PathPtr curr = Path;

    while (Path != NULL)
    {
        Path = Path->Next;
        free(curr);
        curr = Path;
    }
}

PathPtr PrintPath(HeapPtr Heap, ElemType Source[], ElemType Dest[], PathPtr Path, PtrToNext *AdjList) //recursive func to print path
{
    //printf("%s\n", Dest);
    int index = AdjList[hash(Dest, AdjList)]->ConvRate;    //position of Dest in Heap
    if (Dest[0] != NULL_CHAR && strcmp(Dest, Source) != 0) // if the vertex is not the same as source
        Path = PrintPath(Heap, Source, Heap->arr[index]->PrevID, Path, AdjList);

    printf("%s ->", Dest);
    Path = AddPath(Path, Dest); //adding of node to LList

    return Path;
}

void PrintDist(HeapPtr Heap, int startID, int DestID)
{
    printf("Shortest Distance = %lld\n", Heap->arr[DestID]->Dist); //If DistType is changed,the type specifier
                                                                   //Must also be  changed
}

PathPtr FindPath(HeapPtr Heap, ElemType Source[], ElemType Dest[], PathPtr Path, PtrToNext *AdjList)
{
    int index = AdjList[hash(Dest, AdjList)]->ConvRate;
    if (Dest[0] != NULL_CHAR && strcmp(Dest, Source) != 0) // if the vertex is not the same as source
        Path = FindPath(Heap, Source, Heap->arr[index]->PrevID, Path, AdjList);

    Path = AddPath(Path, Dest); //adding of node to LList

    return Path;
}

DistType FindDist(HeapPtr Heap, int DestID)
{
    return (Heap->arr[DestID]->Dist);
}

/*check for segfault around line 119, in the while loops for shortest path*/
void print_existing_path(PathPtr P)
{
    while (P != NULL)
    {
        printf("%s -> ", P->CurrID);
        P = P->Next;
    }
    printf("End of Path\n");
}
void InsertEdgeSecondShortestPath(ElemType TradeBank[50], ElemType C1[50], ElemType C2[50], int ConvRate, AllGraph *list) // Adds a currency conversion between the given two currencies
{
    AllGraph *temp = list;
    while (temp->GraphPtr) // To traverse the list of graphs
    {
        if (strcmp(temp->GraphPtr->TradeBank, TradeBank) == 0) // to check whether the given TradeBank exists
        {
            int key1 = hash_search(C1, temp->GraphPtr->GraphIn); // To check whether the given currency(source) exists in the TradeBank
            if (key1 == -1)
            {
                //printf("INVALID OPERATION !!\n");
                return;
            }

            int key2 = hash_search(C2, temp->GraphPtr->GraphOut); // To check whether the given currency(destination) exists in the TradeBank
            if (key2 == -1)
            {
                //printf("INVALID OPERATION !!\n");
                return;
            }

            PtrToNext nodeIn = CreateNode(C2, ConvRate); // Creates a Node(to insert it in the linkedlist of edges of that particular vertex)
            // nodeIn->ConvRate = ConvRate;

            PtrToNext nodeOut = CreateNode(C1, ConvRate); // Creates a Node(to insert it in the linkedlist of edges of that particular vertex)
            // nodeOut->ConvRate = ConvRate;

            PtrToNext t1 = temp->GraphPtr->GraphIn[key1]->Next;
            while (t1) // To check whether the given currency conversion already exists in the TradeBank
            {
                if (strcmp(t1->VertexID, C2) == 0)
                {
                    //printf("Edge already exists !!\n\n");
                    //t1 = NULL;
                    //free(t1);
                    return;
                }
                t1 = t1->Next;
            }
            //t1 = NULL;
            //free(t1);

            nodeIn->Next = temp->GraphPtr->GraphIn[key1]->Next; // Inserts the currency conversion in the linkedlist associated with the source vertex
            temp->GraphPtr->GraphIn[key1]->Next = nodeIn;
            temp->GraphPtr->GraphIn[key1]->ConvRate = 0;

            nodeOut->Next = temp->GraphPtr->GraphOut[key2]->Next; // Inserts the currency conversion in the linkedlist associated with the destination vertex
            temp->GraphPtr->GraphOut[key2]->Next = nodeOut;
            temp->GraphPtr->GraphOut[key2]->ConvRate = 0;

            //printf("Currency conversion of rate %d has been added from the currency %s to the currency %s in the TradeBank %s!\n", ConvRate, C1, C2, temp->GraphPtr->TradeBank);
            return;
        }

        temp->GraphPtr = temp->GraphPtr->Next;
    }
    //printf("TradeBank does not exist!!\n");
    //temp = NULL;
    //free(temp);
}
PathPtr Second_Shortest_Path_Of_Graph(PtrToGraph TradeBank, char *Currency1, char *Currency2, long long ShortestPathLength, long long *SecondShortestCost, TradeBankListPtr ListOfTradeBanks)
{
    //variables
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
        PtrToNext edge = delete_edge(TradeBank->TradeBank, P->CurrID, P->Next->CurrID, ListOfTradeBanks, 1);
        cost = INFTY;

        returnpath = Dijkstra(TradeBank->GraphIn, Currency1, Currency2, TradeBank->NumVertex, -1, &cost);
        if (cost < INFTY && returnpath != NULL)
        {
            if (cost == ShortestPathLength)
            {
                cost = INFTY;
                finalpath = Second_Shortest_Path_Of_Graph(TradeBank, Currency1, Currency2, ShortestPathLength, SecondShortestCost, ListOfTradeBanks);
            }
            else if (cost < mincost)
            {
                mincost = cost;
                //FreePath(finalpath);
                finalpath = returnpath;
            }
            else
            {
                if (returnpath != NULL)
                    FreePath(returnpath);
            }
        }
        InsertEdgeSecondShortestPath(TradeBank->TradeBank, P->CurrID, P->Next->CurrID, edge->ConvRate, ListOfTradeBanks);
        free(edge);
        // printf("mincost =  %lld \n",mincost);
        P = P->Next;
        //printf("7\n");
    }
    if (mincost < (*SecondShortestCost))
    {
        *SecondShortestCost = mincost;
        //printf("1\n");
        //FreePath(clearpath);
        //printf("111111\n");
        return finalpath;
    }
    else if (finalpath != NULL)
    {
        return finalpath;
    }
    else
        return NULL;
}

//Prints the second shortest path between Currency1 and Currency2 across all TradeBanks
long long Second_Shortest_Conversion(TradeBankListPtr ListOfTradeBanks, char *Currency1, char *Currency2)
{
    //variables
    PtrToGraph T = ListOfTradeBanks->GraphPtr;

    if (T == NULL)
        return INFTY;
    //printf("%d\n",ListOfTradeBanks->NumBanks);
    long long MinConversionCosts[ListOfTradeBanks->NumBanks];
    int NoOfTradeBanksMinCost = 0, minindex = INFTY;
    long long ShortestPathLength = INFTY, SecondShortestCost = INFTY;
    PtrToGraph StartMinTradeBank, SecondMinTradeBank = NULL;

    for (int index = 0; index < ListOfTradeBanks->NumBanks; index++)
    {
        MinConversionCosts[index] = INFTY;
        Dijkstra(T->GraphIn, Currency1, Currency2, T->NumVertex, 0, &MinConversionCosts[index]);
        //printf("%lld  -  %d\n", MinConversionCosts[index], index);
        //Finding if it is shortest or second shortest path and assigning each to a variable
        if (MinConversionCosts[index] < ShortestPathLength)
        {
            SecondShortestCost = ShortestPathLength;
            ShortestPathLength = MinConversionCosts[index];
            StartMinTradeBank = T;
            minindex = index;
            NoOfTradeBanksMinCost = 1;
            //printf("--1--\n");
        }
        else if (MinConversionCosts[index] == ShortestPathLength)
        {
            NoOfTradeBanksMinCost++;
            //printf("--2--\n");
        }
        else if ((MinConversionCosts[index] > ShortestPathLength) && (MinConversionCosts[index] < SecondShortestCost))
        {
            SecondShortestCost = MinConversionCosts[index];
            SecondMinTradeBank = T;
            //printf("--3--\n");
        }

        T = T->Next;
    }

    if (minindex == INFTY)
    {
        printf("No conversion exists from %s to %s through any Trade Bank\n", Currency1, Currency2);
        return INFTY;
    }
    PathPtr SecondMinGraphPath;
    PathPtr FinalSecondMinGraphPath = NULL;
    //printf("----%lld---\n",SecondShortestCost);
    //Going through TradeBanks which have their cost as the lowest cost obtained across all graphs(or TradeBanks)
    while ((StartMinTradeBank != NULL) && (NoOfTradeBanksMinCost > 0))
    {
        if (MinConversionCosts[minindex] == ShortestPathLength)
        {
            //printf("--4--\n");
            SecondMinGraphPath = Second_Shortest_Path_Of_Graph(StartMinTradeBank, Currency1, Currency2, ShortestPathLength, &SecondShortestCost, ListOfTradeBanks);
            if (SecondMinGraphPath != NULL)
            {
                //printf("--5--");
                FinalSecondMinGraphPath = SecondMinGraphPath;
                SecondMinTradeBank = StartMinTradeBank;
            }
        }

        minindex++;
        StartMinTradeBank = StartMinTradeBank->Next;
        NoOfTradeBanksMinCost--;
    }
    if (FinalSecondMinGraphPath != NULL)
    {
        //printf("1\n");
        if (SecondMinTradeBank == NULL)
        {
            printf("No path found\n");
            return 0;
        }
        printf("Use : %s\n", SecondMinTradeBank->TradeBank);
        print_existing_path(FinalSecondMinGraphPath);
        //printf("%s")
        printf("Second Shortest distance = %lld\n", SecondShortestCost);
        FreePath(FinalSecondMinGraphPath);
    }
    else
    {
        printf("2\n");
        if (SecondMinTradeBank == NULL)
        {
            printf("No path found\n");
            return 0;
        }
        printf("Use : %s\n", SecondMinTradeBank->TradeBank);
        printf("Second ");
        ShortestPathLength = INFTY;
        //PrintGraph(*ListOfTradeBanks);
        Dijkstra(SecondMinTradeBank->GraphIn, Currency1, Currency2, SecondMinTradeBank->NumVertex, 1, &ShortestPathLength);
    }
}