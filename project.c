#include <stdio.h>
#include "graph.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// it is a directed graph so no DSU can be used ,hence i have implemented DFS to do .Used backtracking approach to solve the above problem
//the time complexity of the given code is O(V+E) and space complexity is O(V^2)
bool visited[200]={false};
typedef struct Node node;
// node* createnode(int v)//Made for testing purposes only NOT the main graph code :)
// {node *s;
// s=(node*) malloc(sizeof(node));
// s->Next=NULL;
// s->ViD=v;
// return s;
// }
bool dfs_check(node*arr[],int v,node*pp)//this is a suplementry function to complement the main dfs function
{
int i,vertex=pp->ConvRate;//vertex is the node at which everything begines
node *p;//node *p is the pointer to the very next neighbour of the node
if(visited[vertex]==true)
 return true;

 visited[vertex]=true;
 bool check=false;
 p=arr[vertex]->Next;
for(;;)
{
    if(p==NULL) break;
check=dfs_check(arr,vertex,arr[p->ConvRate]);
if(check==true)
return true;
p=p->Next;
}
visited[vertex]=false;
return false;
}
bool dfs_main(int v,int e,node * arr[])//the main graph dfs function which iterates over v and checks for each of the v
{
bool  check=false;int i,k;//visited array for 200 nodes defined by me to implement the dfs function
node *p;

for(i=0;i<v;i++)
{visited[i]=true;p=arr[i]->Next;
for(;;)
{
    if(p==NULL) break;
check=dfs_check(arr,i,arr[p->ConvRate]);
if(check==true) return true;
p=p->Next;

}
visited[i]=false;
}
return false;
}

int main()
{
//printf("Enter name of the tradebank:");
int v,e;
scanf("%d%d",&v,&e);

node * arr1[v],*p,*arr[v];
// int i,j,k;
// for(i=0;i<v;i++)
// {
//    arr1[i]= createnode(i);
    
// }
// char a[50],b[50];
// for(i=0;i<e;i++)
// {
// scanf("%d%d",&j,&k);//j=j-1;k=k-1;
// if(arr1[j]->Next==NULL)
// {
// arr[j]= createnode(k);
// arr1[j]->Next=arr[j];
// }
// else{
//     p=createnode(k);
//     arr[j]->Next=p;
//     arr[j]=p;
// }
// }
bool check=dfs_main( v, e,arr1);
if(check==true)
printf("Yes,this contains a cycle\n");
else
printf("No,this graph does not contain a cycle\n");
}
