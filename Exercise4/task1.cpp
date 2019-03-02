#include <iostream>
#include <list>
#include <stack>

using namespace std;
#define BSIZE 1<<15

int bpos, bsize; char buffer[BSIZE];

class Graph {
private:
  int V, E, components;
  bool * is_topological_root;
  list<int> *connective_parts;
  int *strong_connective_components;
  list<int> *adjacent_list;
public:
  // Main Constructor.
  Graph (int vertices): V(vertices) {
    adjacent_list = new list<int>[V];
    strong_connective_components = new int[V];
    connective_parts = new list<int>[V];
    components = 0;
  }
  // Add edge to the graph.
  void addEdge (int v1, int v2) {
    this->adjacent_list[v1].push_back(v2);
    this->E ++;
  }

  void fillOrder(int v, bool visited[], stack<int> &Stack) {
    visited[v] = true;

    for(list<int>::iterator i = adjacent_list[v].begin(); i != adjacent_list[v].end(); ++i)
      if(!visited[*i]) fillOrder(*i, visited, Stack);

    Stack.push(v);
  }

  void DFSUtil(int v, bool visited[], int component, list<int> &connective_part, int * scc ) {
    visited[v] = true;
    connective_part.push_back(v);
    scc[v] = component;

    for (list<int>::iterator i = adjacent_list[v].begin(); i != adjacent_list[v].end(); ++i)
      if (!visited[*i]) DFSUtil(*i, visited, component, connective_part, scc);
  }

  // Transpose Graph.
  Graph getTranspose() {
    Graph g(V);
    for (int v = 0; v < V; v++) {
      for(list<int>::iterator i = adjacent_list[v].begin(); i != adjacent_list[v].end(); ++i) {
        g.adjacent_list[*i].push_back(v);
      }
    }
    return g;
  }

  // Strong Connective Components.
  void findComponents() {
    stack<int> Stack;

    bool *visited = new bool[V];
    for(int i = 0; i < V; i++) visited[i] = false;

    for(int i = 0; i < V; i++)
      if(!visited[i]) this->fillOrder(i, visited, Stack);

    Graph gr = this->getTranspose();

    for(int i = 0; i < V; i++) visited[i] = false;

    while (Stack.empty() == false) {
      int v = Stack.top();
      Stack.pop();

      if (visited[v] == false) {
        gr.DFSUtil(v, visited, this->components, connective_parts[ this->components ], this->strong_connective_components);
        this->components++;
      }
    }
    this->is_topological_root = new bool[components];
    for (int i=0;i < components; i++) this->is_topological_root[i] = true;
  }

  int parse_graph () {
    // Iterate each connective component.
    for(list<int> * i = connective_parts; (*i).size() != 0; ++i) {
      // Iterate vertices of one connective component.
      for(list<int>::iterator j = (*i).begin(); j != (*i).end(); ++j) {
        // Iterate neighbors of each vertex
        for(list<int>::iterator k = this->adjacent_list[*j].begin(); k != this->adjacent_list[*j].end(); ++k) {
          // Check if the edge is not internal inside the component.
          if (strong_connective_components[*k] != strong_connective_components[*j]) {
            is_topological_root[ strong_connective_components[*k] ] = false;
          }
        }
      }
    }
    int count = 0;
    for(int i = 0; i<components; i++) count += is_topological_root[i];
    if (count != 1) return 0;
    for(int i = 0; i<components; i++) if(is_topological_root[i]) return connective_parts[i].size();
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
  int N = readlong();
  Graph mGraph(N);
  for (int i = 0; i<N; i++) {
    for (int j=readlong(); j>0; j--) {
      mGraph.addEdge(readlong()-1, i);
    }
  }
  mGraph.findComponents();
  cout << mGraph.parse_graph() << endl;
  return 0;
}
