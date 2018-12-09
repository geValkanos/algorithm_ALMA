// Valkanos Georgios ALMA AL1.18.0002

#include <stdio.h>
#include <math.h>

// Constants.
#define ARRAY_SIZE 1000000
#define BSIZE 1<<15

typedef long long L;

typedef struct { double a,b ;} point;

// Global Variables.
int x=0,y=0, n, a, b, c, stackSize = 0, start = 0;
int sweets[ARRAY_SIZE];point stack[ARRAY_SIZE];
L DP[ARRAY_SIZE];
char buffer[BSIZE]; L s2[ARRAY_SIZE], s[ARRAY_SIZE];
L bpos = 0L, bsize = 0L;

L readLong() 
{
  L d = 0L, x = 0L; char c;int y = 1;
  while(1) {
    if (bpos >= bsize) {
      bpos = 0;
      if (feof(stdin)) return x;
      bsize = fread(buffer, 1, BSIZE, stdin);
    }
    c = buffer[bpos++];
    if (c == '-') { y = -1;} 
    if (c >= '0' && c <= '9') { x = x*10 + (c-'0'); d = 1; }
    else if (d == 1) return y*x;
  }
  return -1;
}

// Read the input.
void readValueFromFile() {
  n = readLong();
  a = readLong(); b = readLong(); c = readLong();
  for(int i=0;i<n;i++) sweets[i] = readLong();
}

L equationConstant(int j) { return DP[j] + a*s2[j] - b*s[j]; }
L equationSlope (int j) { return -2*a*s[j]; }
L quadratic(L x) { return a*x*x + b*x + c ;}

void create_S  () {
  s[0] = sweets[0]; s2[0] = s[0]*s[0];
  for (int i = 1; i<n; i++) { s[i] = s[i-1] + sweets[i]; s2[i] = s[i]*s[i]; }
}

void update_the_stack( int index ) {
  point newLine; newLine.a = equationSlope(index); newLine.b = equationConstant(index);
  for (int i=stackSize; i>0; i--) {
    double x1 = (stack[i].b - newLine.b) / (newLine.a - stack[i].a);
    double y1 = (newLine.a * x1 + newLine.b );
    double x2 = (stack[i-1].b - newLine.b) / (newLine.a - stack[i-1].a);
    double y2 = (newLine.a * x2 + newLine.b );
    if (y2 > y1) {
      stackSize --;
    } else {
      stack[++stackSize] = newLine; break;
    }
  }
}

void update_DP (int index) {
  DP[index] = stack[start].a * s[index] + stack[start].b + a*s2[index] + b*s[index] + c;
  for (int j = start+1; j<=stackSize; j++) {
    L temp = stack[j].a * s[index] + stack[j].b + a*s2[index] + b*s[index] + c;
    if (temp < DP[index]) { break; }
    DP[index]=temp; start++;
  }
  L x = quadratic(s[index]);
  if ( x > DP[index]) DP[index] = x;
}

void calculate() {
  DP[0] = quadratic(sweets[0]); //printf("%lld\n", DP[0]);
  stack[0].a = equationSlope(0); stack[0].b = equationConstant(0); 
  L t1 = quadratic(s[1]); L t2 = DP[0] + quadratic(sweets[1]); DP[1] = (t1>t2)?t1:t2;
  stack[1].a = equationSlope(1); stack[1].b = equationConstant(1); stackSize = 1;
  //  printf("%lld, %lld\n", stack[0].a, stack[0].b);
  for (int i = 2; i < n; i++) {
    update_DP(i);
    update_the_stack(i);
    //for (int k = 0; k<=stackSize; k++) printf("%d: %lld, %lld\n", k,stack[k].a, stack[k].b);;
    //printf("%lld\n", DP[i]);
  }
  printf("%lld\n", DP[n-1]);
}

int main() {
  // Read input.
  readValueFromFile();
  create_S();
  calculate();
  return 0;
}
