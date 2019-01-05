// Georgios Valkanos ALMA AL1.18.0002

#include <stdio.h>
#include <stdlib.h>
#define BSIZE 1<<15
#define MOD 1000000103

char buffer[BSIZE];
long bpos = 0L, bsize = 0L;

typedef struct {
  int status;
  int paths[101];
} vertex;

int N,M,X,K, tunnels[180][180][2];  
vertex * v[180][180];

int readlong() {
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

void readInput() {
  N = readlong(); M = readlong(); K = readlong(); X = readlong();
  for (int i=0;i<180;i++) {
    for (int j=0;j<180;j++) {
      tunnels[i][j][0] = tunnels[i][j][1] = -1;
      v[i][j] = (vertex *)malloc(sizeof(vertex));
    }
  }
  for (int i=0; i<K; i++) {
    int s = readlong(), e = readlong();
    tunnels[s/M][s%M][0] = e/M;
    tunnels[s/M][s%M][1] = e%M;
  }
  v[0][0]->status = 1;
  v[0][0]->paths[0] = 1;
  v[0][0]->paths[1] = 1;
}

int * DFS(int x, int y) {
  if (v[x][y]->status == 1) return v[x][y]->paths;
  
  if (tunnels[x][y][0] + tunnels[x][y][1] != -2) {
    int * temp = DFS(tunnels[x][y][0], tunnels[x][y][1]);
    v[x][y]->status = 1;
    v[x][y]->paths[0] = temp[0] + 1;
    v[x][y]->paths[1] = 0;
    for (int i=2; i<=v[x][y]->paths[0]; i++) v[x][y]->paths[i] = temp[i-1];
  } else {
    if (y>0) {
      if (x>0) {
        int * left = DFS(x, y-1);
	int * up = DFS(x-1, y);
	v[x][y]->paths[0] = (up[0] > left[0])?up[0]:left[0];
	for (int i=1; i<=v[x][y]->paths[0]; i++) v[x][y]->paths[i] = (up[i] + left[i])%MOD;
      } else {
	return DFS(x, y-1);
      }
    } else {
      return DFS(x-1, y);
    }
    v[x][y]->status = 1;    
  }
  return v[x][y]->paths;
}

void calculate () {
  int * result = DFS(N-1, M-1), total = 0;
  for (int i=1; i<=X+1; i++) total = (total + result[i])%MOD;
  printf("%d\n", total);
}

int main() {
  readInput();
  calculate();
  return 0;
}
