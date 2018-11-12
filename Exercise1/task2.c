// Valkanos Georgios ALMA - AL1180002
#include <stdio.h>  
#include <stdlib.h>  
  
// Constants.  
#define ARRAY_SIZE 200000  
#define BSIZE 1<<15  
  
// Typedef and types types.  
typedef long long Long;  
typedef struct {  
  Long startTime;  
  Long speed;  
  int dead;  
} particle;  
  
// Global Variables.  
int n = 0, k=0; Long L=0;  
particle *A[ARRAY_SIZE], *B[ARRAY_SIZE];  
char buffer[BSIZE];  
long bpos = 0L, bsize = 0L;  
int globalA=0, globalB=0;    
  
long readLong()   
{  
  long d = 0L, x = 0L; char c;  
  while(1) {  
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
  
// Read the input.  
void readValueFromFile() {  
  n = readLong(); L = readLong(); k = readLong();  
  for(int i=0;i<n;i++) { A[i] = (particle *) malloc(sizeof(particle)); A[i]->startTime = readLong(); A[i]->speed = readLong(); A[i]->dead = 0; }  
  for(int i=0;i<n;i++) { B[i] = (particle *) malloc(sizeof(particle)); B[i]->startTime = readLong(); B[i]->speed = readLong(); B[i]->dead = 0; }  
}


double fasterParticle (particle **A, int * global, double (* fun) (particle *)) {
  double min=1000000000;
  long speed=0;  
  for(int i=0;i<n;i++) {  
    if(A[i]->startTime > min ) break;  
    if(speed < A[i]->speed && !A[i]->dead) {  
      double t = fun(A[i]);
      if (minA>t) { speed = A[i]->speed; minA = t; *global = i+1;}  
    }  
  }  
  return min;
}

int checkOverlap (double d) {
  
  double timeA(particle * x) { return (d/x->speed) + x->startTime; }
  double timeB(particle * x) { return ((L-d)/x->speed) + x->startTime; }

  return (fasterParticle(A, &globalA, timeA)>=fasterParticle(B, &globalB, timeB))?1:0;
}  
  
int binarySearch(int left, int right, int (* fun) (double) ) {    
  if (left == right) { fun(left); return left;}    
  int mid = (left + right)/2;    
  if (fun(mid)) return binarySearch(left, mid, fun);    
  return binarySearch(mid+1,right, fun);    
}  
  
void calculate() {  
  binarySearch(0, L, checkOverlap);  
  printf("%d %d\n", globalA, globalB);  
  A[globalA-1]->dead = B[globalB-1]->dead = 1;    
}  
  
int main() {
  readValueFromFile();  
  for(int i=0;i<k;i++) calculate();  
  return 0;  
}  
