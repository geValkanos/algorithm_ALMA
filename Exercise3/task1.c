#include <stdio.h>
#include <stdlib.h>
#define BSIZE 1<<15

char buffer[BSIZE];
long bpos = 0L, bsize = 0L;

struct Edge{
  int nod1,nod2,cost;
};
struct sub
{
  int parent,level;
};
struct list{
  int nod; int flag;
  struct list *next;  struct list *same;
};
void read_input();int comp(const void *,const void *);int Union(int,int);int find(int);
int basic();void bridge(int);void findbridge(int);int readlong();
int n,m,gefyres,trash;struct Edge array[500000];struct sub Sets[50000];
int visited[50000],parent[50000],low[50000],disc[50000];int checked[50000]={};int lis[50000];
int tim;struct list *graph[50000];
int main()
{
  read_input();
  qsort(array,m,sizeof(struct Edge),comp);
  basic();
  printf("%d\n%d\n%d\n",gefyres,trash,m-gefyres-trash);
  return 0;
}
int find(int i)
{
  if (Sets[i].parent != i) Sets[i].parent = find(Sets[i].parent);
  return Sets[i].parent;
}
 
int Union(int x, int y)
{
    int xr=find(x);int yr=find(y);
    if (Sets[xr].level < Sets[yr].level) Sets[xr].parent = yr;
    else if (Sets[xr].level > Sets[yr].level) Sets[yr].parent = xr;
    else
      {
        Sets[yr].parent = xr;Sets[xr].level++;
      }
    return 0;
}
int comp(const void* x, const void* y)
{
  struct Edge* a = (struct Edge*)x;
  struct Edge* b = (struct Edge*)y;
  return a->cost > b->cost;
}

void findbridge(int u)
{
  visited[u]=1;
  disc[u]=++tim;low[u]=tim;
  for(struct list *t=graph[u];t->next!=NULL;t=t->next)
    {
      int v=t->next->nod;
      if (!visited[v])
        {
	  parent[v]=u; t->next->flag=0;
	  findbridge(v);
	  if(low[v]<low[u]) low[u]=low[v];
	  if (low[v] > disc[u]) gefyres++;
        }
      else if (t->next->same->flag)
	{
	  if(disc[v]<low[u]) low[u]= disc[v];
	}
    }
}

void bridge(int k)
{
  for (int i=0;i<k;i++)
    {
      parent[lis[i]] = lis[i];
      visited[lis[i]] = 0;
    }
  tim=0;
  for (int i = 0; i <k; i++) if (!visited[lis[i]]) findbridge(lis[i]);
}
int basic()
{
  int i,point;
  for (i= 0; i < n; i++)
    {
      Sets[i].parent=i;
      Sets[i].level=0;
    }
  i=0;
  for(point=1;point<201;point++)
    {
      int j=i,k=0;
      for(k=0;k<n;k++) {graph[k]=(struct list *)malloc(sizeof(struct list));graph[k]->next=NULL;}
      k=0;
      while(i<m && point==array[i].cost)
	{
	  int x=find(array[i].nod1-1);
	  int y=find(array[i].nod2-1);
	  array[i].nod1=x;array[i++].nod2=y;
	  if(x!=y)
	    {
	      if(!checked[x]) lis[k++]=x;
	      if(!checked[y]) lis[k++]=y;
	      struct list *t; t=(struct list *)malloc(sizeof(struct list));
	      t->flag=1;t->nod=y;t->next=graph[x]->next;graph[x]->next=t;
	      t=(struct list *)malloc(sizeof(struct list));
	      t->flag=1;t->nod=x;t->next=graph[y]->next;graph[y]->next=t;
	      graph[x]->next->same=graph[y]->next;
	      graph[y]->next->same=graph[x]->next;
	      checked[x]=1;checked[y]=1;
	    }
	  else trash++;	      
	}
      bridge(k);
      while(j<m && point==array[j].cost)
	{
	  int x=array[j].nod1;
	  int y=array[j++].nod2;
	  checked[x]=0;checked[y]=0;
	  if(x!=y) Union(x,y);	      
	}
    }
  return 0;
}
int readlong() 
{
	long d = 0L, x = 0L;
	char c;
	while (1)  {
		if (bpos >= bsize) {
			bpos = 0;
			if (feof(stdin)) return x;
			bsize = fread(buffer, 1, BSIZE, stdin);
		}
		c = buffer[bpos++];
		if (c >= '0' && c <= '9') { x = x*10 + (c-'0'); d = 1; }
		else if (d == 1) return x;
	}
	return -1;
}
void read_input()
{
  n=readlong();m=readlong();
  for(int i=0;i<m;i++)
    {
      array[i].nod1=readlong();
      array[i].nod2=readlong();
      array[i].cost=readlong();
    }
}
