// Valkanos Georgios ALMA AL1.18.0002
#include <stdio.h>

#define PEOPLE_SIZE 500000
#define BSIZE 1<<15

char buffer[BSIZE];
long bpos = 0L, bsize = 0L;

int people[ PEOPLE_SIZE ], n, A[PEOPLE_SIZE], B[PEOPLE_SIZE];
int seq[ PEOPLE_SIZE + 2], last = 2;
int parse = 0;
int (* condition) (int);

// Read the input from stdin.
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

void readInput() {
  n = readLong();
  for ( int i = 0; i < n; i++) people[i] = readLong();
}


int conditionA ( int index ) { return seq[index] >= people[parse]; }
int conditionB ( int index ) { return seq[index] <= people[parse]; }

// Binary Search.
int binarySearch(int left, int right ) {
  if (left == right) return left;
  int mid = (left + right)/2;
  if (condition(mid)) return binarySearch(left, mid);
  return binarySearch(mid+1,right);
}

int calculate() {
  // Value's initialization.
  parse = 1; condition = conditionA;
  seq[0] = 0; seq[1] = people[0]; seq[2] = 1<<30; A[0] = 1;
  // Define binary search condition.
  for (parse = 1; parse < n; parse++) {
    int x = binarySearch(0, last);
    if (x == 1) A[parse] = 0;
    else A[parse] = x;
    if ( last == x) seq[last] = seq[last++];
    seq[x] = people[parse]; 
   }
  parse = 0;
  return 0;
}

int calculate2() {
  seq[0] = 1<<30; seq[1] = people[n-1];
  seq[2] = 0; last = 2;
  B[n-1] = 1; parse = 2; condition = conditionB;
  for (parse = n-2; parse >= 0; parse--) {
    int x = binarySearch(0, last);
    B[parse] = x; seq[x] = people[parse]; 
    if (last == x) seq[++last] = 0;    
  }
  parse = 0;
  return 0;
}

int main() {
  readInput();
  calculate();
  calculate2();
  int maxA = A[0], totalMax = B[0]; int temp = 0;
  for (int i=1;i<n;i++) {
    if (maxA < A[i]) maxA = A[i];
    temp = maxA + B[i];
    if (totalMax < temp) totalMax = temp;
  };
  printf("%d\n", totalMax);
  return 0;
};

