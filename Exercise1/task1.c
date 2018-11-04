// Georgios Valkanos ALMA Grad Student
#include <stdio.h>

// Constants.
#define ARRAY_SIZE 1000000
#define BSIZE 1<<15

typedef long long L;

// Global Variables.
int n = 0;
L buildings[ARRAY_SIZE];
char buffer[BSIZE];
L bpos = 0L, bsize = 0L;

L readLong() 
{
  L d = 0L, x = 0L; char c;
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

// Find the range between the max elements.
int findMax(int start, int end) {
  L m = 0L; int maxIndex = 0;
  for (int i=start; i<=end; i++) 
    if ( buildings[i] > m ) m = buildings[(maxIndex = i)];
    
  return maxIndex;
}

L calculate(int left1, int right2) {
  // If the array has one element return its height.
  if (left1 == right2) return buildings[left1];

  int right1 = 0, left2 = 0;
  int maxIndex = findMax(left1, right2);
  right1 = maxIndex-1; left2 = maxIndex+1;
  
  // Charge the right side to MAX height.
  L sum1 = buildings[maxIndex] * (right2  - right1);
  if (left1 <= right1) {
    sum1 += calculate(left1, right1);
  }
  
  // Charge the left side to MAX height.
  L sum2 = buildings[maxIndex] * (left2-left1);
  if (left2 <= right2 ){
    sum2 += calculate(left2, right2);
  }
  
  // Return the minimum of both.
  return (sum1>sum2?sum2:sum1);
}

int main() {
  // Read the input.
  readValueFromFile();

  // Print the solution.
  printf("%lld\n", calculate(0, n-1));
  
  return 0;
}
