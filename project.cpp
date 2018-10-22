#include <stdio.h>

// Constants.
#define ARRAY_SIZE 1000000
#define BSIZE 1<<15

// Global Variables.
int n = 0;
int buildings[ARRAY_SIZE];
char buffer[BSIZE];
long bpos = 0L, bsize = 0L;

long readLong() 
{
  long d = 0L, x = 0L;
  char c;
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
  n = readLong();
  for(int i=0;i<n;i++) buildings[i] = readLong();
}

// Find the max element.
long findMaxRange(int start, int end, int * indexStart, int * indexEnd) {
  long m = 0L;
  *indexStart = start; *indexEnd = start;
  for (int i=start; i<=end; i++){
    if ( buildings[i] > m ) {
      *indexStart = i; *indexEnd = i;
      m = buildings[i];
    }
    else if ( buildings[i] == m ){
      *indexEnd = i;
    }
  }
  return m;
}

long findMax(int indexStart, int indexEnd){
  long m = 0L;
  for (int i = indexStart; i <= indexEnd; i++)
    if ( buildings[i] > m ) m = buildings[i];
  return m * (indexEnd-indexStart);
}

long  calculate(int left1, int right1, int left2, int right2, long max) {
  long newMax = 0;
  if (left1 == right2) return buildings[left1];
  //printf("%d-%d-%d-%d\n",left1,right1,left2, right2);
  // Charge the right side to MAX height.
  long sum1 = max * (right2  - right1);
  //printf("%ld\n",max * (left2-left1));printf("==============\n");  
  int indexStart = 0, indexEnd = 0;
  if (left1 <= right1) {
    newMax = findMaxRange(left1, right1, &indexStart, &indexEnd); 
    sum1 += calculate(left1, indexStart-1, indexEnd+1, right1, newMax);
  }
  // Charge the left side to MAX height.
  long sum2 = max * (left2-left1);
  if (left2 <= right2 ){
    newMax = findMaxRange(left2, right2, &indexStart, &indexEnd);
    sum2 += calculate(left2, indexStart-1, indexEnd+1, right2, newMax);
  }
  return (sum1>sum2?sum2:sum1);
}

int main() {
  // Read the input.
  readValueFromFile();

  // Calculate the max element of the array.
  int indexStart = 0, indexEnd = 0;
  long max = findMaxRange(0, n-1, &indexStart, &indexEnd);
  printf("%ld\n",max);
  // Find in which range to put the highest building.
  printf("%ld\n", calculate(0, indexStart-1, indexEnd + 1, n-1, max));
  
  return 0;
}
