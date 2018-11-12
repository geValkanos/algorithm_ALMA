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

double findIntersection(particle * a, particle * b){
  // L = v_a(t-t_sa) + v_b(t-t_sb) 
  double m = (double) (L + a->startTime*a->speed + b->startTime*b->speed) / (a->speed+b->speed);
  //printf("time: %.15f\n",m);
  return m;
}

int globalA=0, globalB=0;
int checkOverlap (int d) {
  double minA=1000000000, minB = 1000000000;
  long speedA=0, speedB= 0;
  for(int i=0;i<n;i++) {
    if(A[i]->startTime > minA ) break;
    if(speedA < A[i]->speed && !A[i]->dead) {
      double t = (double) (d/A[i]->speed) + A[i]->startTime;
      if (minA>t) { speedA = A[i]->speed; minA = t; globalA = i+1;}
    }
  }
  for(int i=0;i<n;i++) {
    if(B[i]->startTime > minB ) break;
    if(speedB < B[i]->speed && !B[i]->dead) { //x = L - v(t-t_s)
      double t = (double) ((L-d)/B[i]->speed) + B[i]->startTime;
      if (minB>t) { speedB = B[i]->speed; minB = t; globalB = i+1;}
    }
  }
  if (minA>=minB) {
    return 1;
  }
  return 0;
}

int binarySearch(int left, int right, int (* fun) (int) ) {  
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
  // Read the input.
  readValueFromFile();
  //printf("L:%lld\n",L);
  //  printf("%d-%d --- %d-%d\n",A[3]->speed,A[3]->startTime,B[5]->speed, B[5]->startTime);
  // Print the solution.
  for(int i=0;i<k;i++) calculate();

  return 0;
}
