#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include "Position.h"
#include <iostream>

using namespace std;

template <class T, class P> class Edge;
template <class T, class P> class Graph;
template <class T, class P> class Vertex;


/****************** Provided structures  ********************/

template <class T, class P>
class Vertex {
	T info;                // contents
	vector<Edge<T, P> > adj;  // list of outgoing edges
	bool visited;          // auxiliary field used by dfs and bfs
	int indegree;          // auxiliary field used by topsort
	bool processing;       // auxiliary field used by isDAG
	int graphViewerID; // auxiliary for GraphViewer
	Position position = Position(0,0);
	string POIType = "";

	void addEdge(Vertex<T, P> *dest, P w);
	bool removeEdgeTo(Vertex<T, P> *d);
public:
	Vertex(T in);
	void coutPosition(){ cout.precision(9); std::cout << fixed << position.getX() << "," << fixed << position.getY();	};
	void coutType() { cout << POIType; };
	friend class Graph<T, P>;
	friend class Application;
};

template <class T, class P>
class Edge {
	Vertex<T, P> * dest;      // destination vertex
	P weight;         // edge weight
public:
	Edge(Vertex<T, P> *d, P w);
	friend class Graph<T, P>;
	friend class Vertex<T, P>;
	friend class Application;
};

template <class T, class P>
class Graph {
	vector<Vertex<T, P> *> vertexSet;    // vertex set

	void dfsVisit(Vertex<T, P> *v,  vector<T> & res) const;
	bool dfsIsDAG(Vertex<T, P> *v) const;
public:
    Vertex<T, P> *findVertex(const T &in) const;
	int getNumVertex() const;
	bool addVertex(const T &in);
	bool addVertex(const T &in, const Position &p);
	bool removeVertex(const T &in);
	bool addEdge(const T &sourc, const T &dest, P w);
	bool removeEdge(const T &sourc, const T &dest);
	vector<T> dfs() const;
	vector<T> bfs(const T &source) const;
	vector<T> topsort() const;
	int maxNewChildren(const T &source, T &inf) const;
	bool isDAG() const;
	friend class Application;
	void setPOIType(const T&in, const string& poiType);
};

/****************** Provided constructors and functions ********************/

template <class T, class P>
Vertex<T, P>::Vertex(T in): info(in) {}

template <class T, class P>
Edge<T, P>::Edge(Vertex<T, P> *d, P w): dest(d), weight(w) {}


template <class T, class P>
int Graph<T, P>::getNumVertex() const {
	return vertexSet.size();
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template <class T, class P>
Vertex<T, P> * Graph<T, P>::findVertex(const T &in) const {
	for (auto v : vertexSet) {
        if (v->info == in) {
            return v;
        }
    }
	return NULL;
}

/****************** 1a) addVertex ********************/

/*
 *  Adds a vertex with a given content/info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T, class P>
bool Graph<T, P>::addVertex(const T &in) {
    if (findVertex(in) != NULL) return false;
	auto vertex = new Vertex<T, P>(in);
	vertexSet.push_back(vertex);
    return true;
}


template <class T, class P>
bool Graph<T, P>::addVertex(const T &in, const Position &p) {
    if (findVertex(in) != NULL) return false;
    auto vertex = new Vertex<T, P>(in);
    vertex->position = p;
    vertexSet.push_back(vertex);
    return true;
}

/****************** 1b) addEdge ********************/

/*
 * Adds an edge to a graph (this), given the contents of the source (sourc) and
 * destination (dest) vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T, class P>
bool Graph<T, P>::addEdge(const T &sourc, const T &dest, P w) {
	Vertex<T, P> * sourcVertex = findVertex(sourc);
	Vertex<T, P> * destVertex = findVertex(dest);
	if (sourcVertex == NULL || destVertex == NULL) return false;
	sourcVertex->addEdge(destVertex, w);
	return true;
}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T, class P>
void Vertex<T, P>::addEdge(Vertex<T, P> *d, P w) {
    adj.push_back(Edge<T, P>(d, w));
}


/****************** 1c) removeEdge ********************/

/*
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T, class P>
bool Graph<T, P>::removeEdge(const T &sourc, const T &dest) {
	Vertex<T, P> * sourcVertex = findVertex(sourc);
    Vertex<T, P> * destVertex = findVertex(dest);
    if (sourcVertex == NULL || destVertex == NULL) return false;
    return sourcVertex->removeEdgeTo(destVertex);
}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T, class P>
bool Vertex<T, P>::removeEdgeTo(Vertex<T, P> *d) {
    for (auto it = adj.begin(); it != adj.end(); it++) {
        if (it->dest->info == d->info) {
            adj.erase(it);
            return true;
        }
    }
	return false;
}


/****************** 1d) removeVertex ********************/

/*
 *  Removes a vertex with a given content (in) from a graph (this), and
 *  all outgoing and incoming edges.
 *  Returns true if successful, and false if such vertex does not exist.
 */
template <class T, class P>
bool Graph<T, P>::removeVertex(const T &in) {
    Vertex<T, P> * destVertex = findVertex(in);
    bool removed = false;
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        if ((*it)->info == in) {
            delete *it;
            vertexSet.erase(it);
            it--;
            removed = true;
        } else {
            (*it)->removeEdgeTo(destVertex);
        }
    }
	return removed;
}


/****************** 2a) dfs ********************/

/*
 * Performs a depth-first search (dfs) in a graph (this).
 * Returns a vector with the contents of the vertices by dfs order.
 * Follows the algorithm described in theoretical classes.
 */
template <class T, class P>
vector<T> Graph<T, P>::dfs() const {
	vector<T> res;
	for (typename vector<Vertex<T, P> *>::const_iterator it = vertexSet.begin(); it != vertexSet.end(); it++) {
        (*it)->visited = false;
	}
    for (typename vector<Vertex<T, P> *>::const_iterator it = vertexSet.begin(); it != vertexSet.end(); it++) {
        if (!(*it)->visited) {
            dfsVisit(*it, res);
        }
    }
	return res;
}

/*
 * Auxiliary function that visits a vertex (v) and its adjacent not yet visited, recursively.
 * Updates a parameter with the list of visited node contents.
 */
template <class T, class P>
void Graph<T, P>::dfsVisit(Vertex<T, P> *v, vector<T> & res) const {
    v->visited = true;
    res.push_back(v->info);
    for (Edge<T, P> a : v->adj) {
        if (!a.dest->visited) {
            dfsVisit(a.dest, res);
        }
    }
}

/****************** 2b) bfs ********************/

/*
 * Performs a breadth-first search (bfs) in a graph (this), starting
 * from the vertex with the given source contents (source).
 * Returns a vector with the contents of the vertices by dfs order.
 * Follows the algorithm described in theoretical classes.
 */
template <class T, class P>
vector<T> Graph<T, P>::bfs(const T & source) const {
    vector<T> res;
    for (typename vector<Vertex<T, P> *>::const_iterator it = vertexSet.begin(); it != vertexSet.end(); it++) {
        (*it)->visited = false;
    }
    queue<Vertex<T, P> *> vertexQueue;

    Vertex<T, P> * s = findVertex(source);
    if (s == NULL) return vector<T>();

    vertexQueue.push(s);
    s->visited = true;

    while(!vertexQueue.empty()) {
        s = vertexQueue.front();
        vertexQueue.pop();
        res.push_back(s->info);
        for (Edge<T, P> a : s->adj) {
            if (!a.dest->visited) {
                vertexQueue.push(a.dest);
                a.dest->visited = true;
            }
        }
    }
	return res;
}

/****************** 2c) toposort ********************/

/*
 * Performs a topological sorting of the vertices of a graph (this).
 * Returns a vector with the contents of the vertices by topological order.
 * If the graph has cycles, returns an empty vector.
 * Follows the algorithm described in theoretical classes.
 */

template <class T, class P>
vector<T> Graph<T, P>::topsort() const {
	vector<T> res;

	for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        (*it)->indegree = 0;
	}
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        for (Edge<T, P> a : (*it)->adj) a.dest->indegree++;
    }
    queue<Vertex<T, P> *> C; // vertices with indegree == 0 (candidates)
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        if ((*it)->indegree == 0) {
            C.push(*it);
        }
    }

    Vertex<T, P> *v;
    while (!C.empty()) {
        v = C.front();
        C.pop();
        res.push_back(v->info);
        for (Edge<T, P> e : v->adj) {
            e.dest->indegree--;
            if (e.dest->indegree == 0) C.push(e.dest);
        }
    }
    if (res.size() != vertexSet.size()) return vector<T>(); // graph is not a DAG
	return res;
}

/****************** 3a) maxNewChildren (HOME WORK)  ********************/

/*
 * Performs a breadth-first search in a graph (this), starting
 * from the vertex with the given source contents (source).
 * During the search, determines the vertex that has a maximum number
 * of new children (adjacent not previously visited), and returns the
 * contents of that vertex (inf) and the number of new children (return value).
 */

template <class T, class P>
int Graph<T, P>::maxNewChildren(const T & source, T &inf) const {
    int res = 0;
    for (typename vector<Vertex<T, P> *>::const_iterator it = vertexSet.begin(); it != vertexSet.end(); it++) {
        (*it)->visited = false;
    }
    queue<Vertex<T, P> *> vertexQueue;

    Vertex<T, P> * s = findVertex(source);
    if (s == NULL) return 0;

    vertexQueue.push(s);
    s->visited = true;

    int count;
    while(!vertexQueue.empty()) {
        count = 0;
        s = vertexQueue.front();
        vertexQueue.pop();
        for (Edge<T, P> a : s->adj) {
            if (!a.dest->visited) {
                vertexQueue.push(a.dest);
                a.dest->visited = true;
                count++;
            }
        }
        if (count > res) {
            res = count;
            inf = s->info;
        }
    }
	return res;
}

/****************** 3b) isDAG   (HOME WORK)  ********************/

/*
 * Performs a depth-first search in a graph (this), to determine if the graph
 * is acyclic (acyclic directed graph or DAG).
 * During the search, a cycle is found if an edge connects to a vertex
 * that is being processed in the the stack of recursive calls (see theoretical classes).
 * Returns true if the graph is acyclic, and false otherwise.
 */
template <class T, class P>
bool Graph<T, P>::isDAG() const {
    for (typename vector<Vertex<T, P> *>::const_iterator it = vertexSet.begin(); it != vertexSet.end(); it++) {
        (*it)->processing = false;
        (*it)->visited = false;
    }
    for (typename vector<Vertex<T, P> *>::const_iterator it = vertexSet.begin(); it != vertexSet.end(); it++) {
        if (!(*it)->visited) {
            (*it)->processing = true;
            if (!dfsIsDAG(*it)) return false;
            (*it)->processing = false;
        }
    }
	return true;
}

/**
 * Auxiliary function that visits a vertex (v) and its adjacent not yet visited, recursively.
 * Returns false (not acyclic) if an edge to a vertex in the stack is found.
 */
template <class T, class P>
bool Graph<T, P>::dfsIsDAG(Vertex<T, P> *v) const {
    v->visited = true;
    for (Edge<T, P> a : v->adj) {
        if (a.dest->processing) return false;
        if (!a.dest->visited) {
            a.dest->processing = true;
            if (!dfsIsDAG(a.dest)) return false;
            a.dest->processing = false;
        }
    }
	return true;
}

template<class T, class P>
void Graph<T, P>::setPOIType(const T &in, const string &poiType) {
    Vertex<T, P> *v = findVertex(in);
    v->POIType = poiType;
}

#endif /* GRAPH_H_ */
