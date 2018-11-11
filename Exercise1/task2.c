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

double globalMax=0, globalMin=1000000000;
int checkOverlap (long t) {
  double max=0, min = L;
  
  for(int i=0;i<n;i++) {
    if (A[i]->startTime > t || max > L) break;
    if(!A[i]->dead ) {
      double dis = A[i]->speed*(t-A[i]->startTime);
      max = (max<dis)?dis:max;
    }
  }
  for(int i=0;i<n;i++) {
    if(B[i]->startTime > t || min < 0) break;
    if(!B[i]->dead) {
      double dis = L - B[i]->speed*(t-B[i]->startTime);
      min = (min>dis)?dis:min;
    }
  }
  if (max>=min) {
    globalMin = min;
    globalMax = max;
    return 1;
  }
  return 0;
}

int binarySearch(int left, int right, int (* fun) (long) ) {  
  if (left == right) { return left;}  
  int mid = (left + right)/2;  
  if (fun(mid)) return binarySearch(left, mid, fun);  
  return binarySearch(mid+1,right, fun);  
}

void printFirstCollision(int *a, int n_a, int *b, int n_b){
  int amin = a[0], bmin = b[0]; double min = findIntersection(A[a[0]], B[b[0]]);
  for(int i=0;i<n_a;i++) {
    for(int j=0;j<n_b;j++) {
      double x = findIntersection(A[a[i]], B[b[j]]);
      if (x < min && ((double) L/A[a[i]]->speed) + A[a[i]]->startTime >= B[b[j]]->startTime) {
	amin = a[i]; bmin=b[j]; min=x;
	//printf("AAAAAAAAAA\n");
      }
    }
  }
  printf("%d %d\n", amin+1, bmin+1);
  A[amin]->dead = B[bmin]->dead = 1;
}

long prevCol = 0;
void calculate() {
  long t = binarySearch(0, 1<<31-1, checkOverlap);
  //printf(">>%ld\n",t);
  int indexA=0,indexB=0;
  int tempA[100000], tempB[100000];
  //printf("%f %f\n",min,max);
  for(int i=0;i<n;i++) {
    if (A[i]->startTime > t) break;
    if(!A[i]->dead) {
      double dis = A[i]->speed*(t-A[i]->startTime);
      if (dis>=globalMin && dis<=globalMax) {
	tempA[indexA]=i;indexA++;//printf("A: %d\n",i+1);
      }
    }
  }
  for(int i=0;i<n;i++) {
    if(B[i]->startTime > t) break;
    if(!B[i]->dead) {
      double dis = L - B[i]->speed*(t-B[i]->startTime);
      if (dis>=globalMin && dis<=globalMax) {
	tempB[indexB]=i;indexB++;
      }
    }
  }
  //printf("---%d %d\n",indexA,indexB);
  printFirstCollision(tempA, indexA, tempB, indexB);
  prevCol = t-1;
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
