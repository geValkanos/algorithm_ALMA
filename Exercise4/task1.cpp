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
  list<int> *adjastency_list;
public:
  // Main Constructor.
  Graph (int vertices): V(vertices) {
    adjastency_list = new list<int>[V];
    strong_connective_components = new int[V];
    connective_parts = new list<int>[V];
    components = 0;
  }
  // Add edge to the graph.
  void addEdge (int v1, int v2) {
    this->adjastency_list[v1].push_back(v2);
    this->E ++;
    // cout << "added" << endl;
  }
  void fillOrder(int v, bool visited[], stack<int> &Stack) {
    // Mark the current node as visited and print it
    visited[v] = true;

    // Recur for all the vertices adjacent to this vertex
    list<int>::iterator i;
    for(i = adjastency_list[v].begin(); i != adjastency_list[v].end(); ++i)
      if(!visited[*i])
	fillOrder(*i, visited, Stack);

    // All vertices reachable from v are processed by now, push v
    Stack.push(v);
  }

  // A recursive function to print DFS starting from v
  void DFSUtil(int v, bool visited[], int component, list<int> &connective_part, int * scc ) {
    // Mark the current node as visited and print it
    visited[v] = true;
    connective_part.push_back(v);
    scc[v] = component;
    // Recur for all the vertices adjacent to this vertex
    list<int>::iterator i;
    for (i = adjastency_list[v].begin(); i != adjastency_list[v].end(); ++i)
      if (!visited[*i])
	DFSUtil(*i, visited, component, connective_part, scc);
  }

  // Transpose Graph.
  Graph getTranspose() {
    Graph g(V);
    for (int v = 0; v < V; v++) {
      // Recur for all the vertices adjacent to this vertex
      list<int>::iterator i;
      for(i = adjastency_list[v].begin(); i != adjastency_list[v].end(); ++i) {
	g.adjastency_list[*i].push_back(v);
      }
    }
    return g;
  }
  // Strong Connective Components.
  void findComponents() {
    stack<int> Stack;

    // Mark all the vertices as not visited (For first DFS)
    bool *visited = new bool[V];
    for(int i = 0; i < V; i++) visited[i] = false;

    // Fill vertices in stack according to their finishing times
    for(int i = 0; i < V; i++)
      if(visited[i] == false)
	this->fillOrder(i, visited, Stack);

    // Create a reversed graph
    Graph gr = this->getTranspose();

    // Mark all the vertices as not visited (For second DFS)
    for(int i = 0; i < V; i++)
      visited[i] = false;

    // Now process all vertices in order defined by Stack
    while (Stack.empty() == false) {
      // Pop a vertex from stack
      int v = Stack.top();
      Stack.pop();

      // Print Strongly connected component of the popped vertex
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
	for(list<int>::iterator k = this->adjastency_list[*j].begin(); k != this->adjastency_list[*j].end(); ++k) {
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
