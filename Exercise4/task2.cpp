#include <iostream>
#include <list>
#include <tuple>
#include <utility>
#include <vector>
#include <queue>

using namespace std;
#define BSIZE 1<<15
#define INF 0x3f3f3f3f

int bpos, bsize; char buffer[BSIZE];
vector<int> *distances;

typedef pair<int,int> iPair;
typedef list<iPair> iList;

class Graph {
private:
  iList *adj;
  int E, V;
public:
  // Main Constructor.
  Graph (int vertices): V(vertices) { this->adj = new iList[V]; this->E = 0;}

  // Copy Constructor.
  Graph (Graph * obj) { *this = *obj; }

  // Add new edge to the graph.
  void addEdge (int v1, int v2, int w) {
    this->adj[v1].push_back(make_pair(v2, w));
    this->E++;
  }

  // Get graph vertex number.
  int getVertices () { return this->V; }

  // Get graph edge number.
  int getEdges () { return this->E; }

  // Get graph adj list.
  iList *getAdj () { return this->adj; }

  // Create layers of super-graph.
  void createLayer (Graph *g) {
    iList *adj = new iList[ this->V + g->getVertices() ];

    iList *g_adj = g->getAdj();
    for (int i = 0; i < this->V; i++) { adj[i] = this->adj[i]; }
    this->adj = adj;

    for (int i = 0; i < g->getVertices(); i++) {
      for (iList::iterator j = g_adj[i].begin(); j != g_adj[i].end(); ++j) {
        // cout << "A " << i << " " << j.first << endl;
        this->addEdge(j->first + this->V - g->getVertices(), this->V + i, j->second);
        this->addEdge(this->V + i, j->first + this->V, j->second);
      }
    }

    this->E += 2*g->getEdges(); this->V += g->getVertices();
  }
  // Dijkstra
  void dijkstra(int src) {
    priority_queue< iPair, vector <iPair> , greater<iPair> > pq;

    vector<int> dist(V, INF);
    pq.push(make_pair(0, src));
    dist[src] = 0;

    while (!pq.empty()) {
      int u = pq.top().second;
      pq.pop();
      iList::iterator i;
      for (i = adj[u].begin(); i != adj[u].end(); ++i) {
        int v = i->first;
        int weight = i->second;
        if (dist[v] > dist[u] + weight) {
          dist[v] = dist[u] + weight;
          pq.push(make_pair(dist[v], v));
        }
      }
    }
    distances[src] = dist;
  }
  // Print Graph.
  void printGraph () {
    for (int i = 0; i < this->V; i++) {
      cout << i << ": ";
      for (iList::iterator j = this->adj[i].begin(); j != this->adj[i].end(); j++ ) {
        cout << "(" << j->first << "," << j->second << ") ";
      }
      cout << endl;
    }
  }

  // Overload assignment operator.
  void operator = (Graph const &g) {
    this->V = g.V;
    this->E = g.E;
    this->adj = new iList[V];
    iList *g_adj = g.adj;
    for (int i = 0; i < g.V; i++) {
      for (iList::iterator j = g_adj[i].begin(); j != g_adj[i].end(); ++j) {
        this->addEdge(i, j->first, j->second);
      }
    }
  }
};

int readlong() {
  int d = 0, x = 0;
  char c;
  while (1) {
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

int main () {
  int N = readlong(), M = readlong(), K = readlong(), Q = readlong();
  Graph g(N); tuple<int, int, int> * questions = new tuple<int, int, int>[Q];
  for (int i = 0; i < M; i++) {
    int v1 = readlong() - 1; int v2 = readlong() - 1; int w = readlong();
    //x.vertex = readlong() - 1; x.weight = readlong();
    g.addEdge(v1, v2, w);
  }
  for (int i = 0; i < Q; i++) {
    int src = readlong()-1; int dst = readlong()-1; int ill = readlong();
    questions[i] = make_tuple(src, dst, ill); }
  // Create the supergraph.
  Graph g1(&g);
  for (int i = 0; i < K; i++) { g.createLayer(&g1); }
  distances = new vector<int>[N];
  for (int i = 0; i < N; i++) { g.dijkstra(i); }

  for (int i = 0; i < Q; i++) {
    int src = get<0>(questions[i]);
    int dst = get<1>(questions[i]);
    int illegalities = get<2>(questions[i]);
    bool flag = true; int max = INF;
    for (int j = dst; j<=dst+N*illegalities; j+=N) {
      if (distances[src][j] < max) max = distances[src][j];
    }
    if (max == INF) cout << "IMPOSSIBLE" << endl;
    else cout << max << endl;
  }
  return 0;
}
