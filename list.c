#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
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
bool check[1000][201] = {false};
int tradecheck[1000] = {0};
int numtradebanks = 0;
void printcurrency(int tradenum)
{
    struct timeval t1;
    gettimeofday(&t1, NULL);
    srand((t1.tv_sec * t1.tv_usec));

    int i, j, k, l;
    char a[50], b[50];
    FILE *fp = fopen("currency.txt", "r");
    FILE *fp1 = fopen("dict.txt", "a");
    k = rand() % 200;
    if (k == 0)
        k = 200;
    if (check[tradenum][k] == false)
    {
        for (j = 0; j < k - 1; j++)

            fscanf(fp, "%s", a);
        fscanf(fp, "%s", b);
        fprintf(fp1, "%s\n", b);
        check[tradenum][k] = true;
    }
    else
    {

        int y = (k + 1) % 201;

        for (;;)
        {
            if (y == 0)
                y = 1;
            if (check[tradenum][y] == false)
            {
                check[tradenum][y] = true;
                break;
            }
            y = (y + 1) % 201;
        }
        for (j = 0; j < y - 1; j++)
            fscanf(fp, "%s", a);
        fscanf(fp, "%s", b);
        fprintf(fp1, "%s\n", b);
    }

    fclose(fp);
    fclose(fp1);
    return;
}

void printradebank()
{
    struct timeval t1;
    gettimeofday(&t1, NULL);
    srand((t1.tv_sec * t1.tv_usec));

    int i, j, k;
    char a[50], b[50];
    FILE *fp = fopen("tradebanks.txt", "r");
    FILE *fp1 = fopen("dict.txt", "a");
    k = rand() % 1000;

    if (check[k][0] == false)
    {
        for (j = 0; j < k - 1; j++)
            fscanf(fp, "%s", a);
        fscanf(fp, "%s", b);
        fprintf(fp1, "%s\n", b);
        check[k][0] = true;
    }
    else
    {

        int y = (k + 1) % 1000;

        for (;;)
        {
            if (y == 0)
                y = 1;
            if (check[y][0] == false)
            {
                check[y][0] = true;
                break;
            }
            y = (y + 1) % 1000;
        }
        for (j = 0; j < y - 1; j++)
            fscanf(fp, "%s", a);
        fscanf(fp, "%s", b);
        fprintf(fp1, "%s\n", b);
    }

    fclose(fp);
    fclose(fp1);
    return;
}
void printnum()
{
    struct timeval t1;
    int x;
    gettimeofday(&t1, NULL);
    srand((t1.tv_sec * t1.tv_usec));
    FILE *fp = fopen("dict.txt", "a");
    if (fp == NULL)
    {
        printf("ERROR");
        return;
    }
    x = rand() % 400;
    if (x == 0)
        x = 230;
    fprintf(fp, "%d\n", x);
    fclose(fp);
}
int main()
{
    int n, j, i, currnum, currencyID, currnum2;
    FILE *fp;
    struct timeval t1;

    printf("Key for using the file:-\nPress 1 to get name of Tradebank\nPress 2 to get currency variable currency names and add number of currencies you need\nPress 3 to Get conversion rates per table\n");
    printf("Press 4 to delete a tradebank\nPress 5 to delete currencies from a tradebank\nPress 6 to delete a cureency conversion rate\nPress 7 to detect cycle in  graph\n");
    printf("Press 8 to find the second best path\nPress 9 to implement the main function\nPress 10 to exit\n");
    gettimeofday(&t1, NULL);
    srand((t1.tv_sec * t1.tv_usec));
    int x = 0;
    char tradename[50], currname1[50], currname2[50];
    for (;;)
    {
        printf("Enter Operation Number:");
        scanf("%d", &j);
        fp = fopen("dict.txt", "w");
        fclose(fp);
        if (j == 1)
        {
            printf("Enter number of tradebanks needed  ");
            scanf("%d", &n);
            if (numtradebanks + n > 1000)
            {
                printf("The number of tradebanks  have exceeded\n");
                goto end;
            }
            for (x = 0; x < n; x++)
                printradebank();
            numtradebanks += n;
        }
        else if (j == 2)
        {
            printf("Enter  tradebanks ID and number of currencies needed: ");
            scanf("%d%d", &n, &currnum);
            if (tradecheck[n] + currnum > 200)
            {
                printf("The currencies have exceeded\n");
                goto end;
            }
            for (x = 0; x < currnum; x++)
                printcurrency(n);
            tradecheck[n] += currnum;
        }
        else if (j == 3)
        {
            printf("Enter number of currencies:");
            scanf("%d", &n);
            for (i = 0; i < n; i++)
                printnum();
        }
        else if (j == 5)
        {
            printf("Tradebank ID:\n ");
            scanf("%d", &n);
            printf("Currency ID");
            scanf("%d", &currnum);
            check[n][currnum] = false;
        }
        else if (j == 4)
        {
            printf("Tradebank ID:\n ");
            scanf("%d", &n);
            check[n][0] = false;
        }
        else if (j == 6)
        {
            printf("Tradebank ID:\n");
            scanf("%s", tradename);
            printf("Currency ID1 and Currency ID2");
            scanf("%s%s", currname1, currname2);
        }
        else if (j == 7)
        {
            printf("Tradebank ID:\n");
            scanf("%s", tradename);
        }

        else if (j == 8)
        {
            printf("Tradebank ID:\n");
            scanf("%s", tradename);
        }
        else if (j == 9)
        {
            printf("Tradebank ID:\n");
            scanf("%s", tradename);
            printf("Currency ID1 and Currency ID2");
            scanf("%s%s", currname1, currname2);
        }
        else if (j == 10)
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
