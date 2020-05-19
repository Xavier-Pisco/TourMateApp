#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <iostream>
#include "../containers/Position.h"
#include "MutablePriorityQueue.h"

using namespace std;

template <class T, class P> class Edge;
template <class T, class P> class Graph;
template <class T, class P> class Vertex;


template <class T, class P>
class Vertex {
	T info;                // contents
	vector<Edge<T, P> > adj;  // list of outgoing edges
	bool visited;          // auxiliary field used by dfs and bfs
	int indegree;          // auxiliary field used by topsort
	bool processing;       // auxiliary field used by isDAG
	int graphViewerID; // auxiliary for GraphViewer
	double dist;
	Vertex<T, P> * path;
    int queueIndex;

	void addEdge(Vertex<T, P> *dest, P inf, double w);
	bool removeEdgeTo(Vertex<T, P> *d);
public:
	Vertex(T in);
	T &getInfo();
	void setPoi(string poi);
	double getDist() { return dist; }
    vector<Edge<T, P>> getAdj() { return adj; }
    bool operator<(Vertex<T, P> & vertex) const; // // required by MutablePriorityQueue
    friend class Graph<T, P>;
	friend class MutablePriorityQueue<Vertex<T, P>>;
    friend class GraphViewerCustom;
    friend class MapContainer;
};

template <class T, class P>
class Edge {
	Vertex<T, P> * dest;      // destination vertex
	P info;
	double weight;
public:
	Edge(Vertex<T, P> *d, P info, double w);
	P getInfo() { return info; }
	friend class Graph<T, P>;
	friend class Vertex<T, P>;
    friend class GraphViewerCustom;
};

template <class T, class P>
class Graph {
	vector<Vertex<T, P> *> vertexSet;    // vertex set
	vector<Vertex<T, P> *> originalVertexSet;
    pair<double, double> minCoords = {0, 0}, maxCoords = {0, 0};

	void dfsVisit(Vertex<T, P> *v,  vector<Vertex<T, P>*> & res) const;
	bool dfsIsDAG(Vertex<T, P> *v) const;

public:
    ~Graph();
    Vertex<T, P> *findVertex(const T &in) const;
    const vector<Vertex<T, P>*> &getVertexSet() const;
    void setMaxMinCoords(pair<double, double> mn, pair<double, double> mx);
    void setVertexSet(vector<Vertex<T, P>*> v);
    void setOriginalVertexSet();
	int getNumVertex() const;
	bool addVertex(const T &in);
    bool addVertex(Vertex<T, P> * in);
	bool addVertex(const T &in, const Position &p);
	bool removeVertex(const T &in);
	bool addEdge(const T &sourc, const T &dest, P info, double w);
    bool addEdge(Vertex<T, P> *sourcVertex, Vertex<T, P> *destVertex, P info, double w);
    bool addEdge(const T &sourc, const T &dest, P info); // this is a temporary solution
	bool removeEdge(const T &sourc, const T &dest);
    vector<Vertex<T, P>*> dfs() const;
    vector<Vertex<T, P>*> bfs(Vertex<T, P> * source) const;
	vector<T> topsort() const;
	int maxNewChildren(const T &source, T &inf) const;
	bool isDAG() const;
	void dijkstra(Vertex<T, P> * origin);
	pair<vector<P>, double> getPathToFromDijkstra(Vertex<T, P> * s, Vertex<T, P> * d);
	friend class GraphViewerCustom;
	friend class MapContainer;
};

template <class T, class P>
Vertex<T, P>::Vertex(T in): info(in) {}

template <class T, class P>
Edge<T, P>::Edge(Vertex<T, P> *d, P info, double w): dest(d), info(info), weight(w) {}


template <class T, class P>
int Graph<T, P>::getNumVertex() const {
	return vertexSet.size();
}

template <class T, class P>
const vector<Vertex<T, P>*> &Graph<T, P>::getVertexSet() const {
    return vertexSet;
}

template <class T, class P>
Graph<T, P>::~Graph() {
    for (Vertex<T, P> * v : vertexSet) delete v;
}

template<class T, class P>
void Graph<T, P>::setMaxMinCoords(pair<double, double> mn, pair<double, double> mx) {
    this->minCoords = mn;
    this->maxCoords = mx;
}

template<class T, class P>
void Graph<T, P>::setVertexSet(vector<Vertex<T, P>*> v) {
    if (originalVertexSet.empty()) setOriginalVertexSet();
    vertexSet = v;
}

template<class T, class P>
void Graph<T, P>::setOriginalVertexSet() {
    originalVertexSet.erase(originalVertexSet.begin(), originalVertexSet.end());
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        originalVertexSet.push_back(*it);
    }
}

template <class T, class P>
bool Vertex<T, P>::operator<(Vertex<T, P> & vertex) const {
    return this->dist < vertex.dist;
}

/**
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

/**
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
bool Graph<T, P>::addVertex(Vertex<T, P> * vertex) {
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

/**
 * Adds an edge to a graph (this), given the contents of the source (sourc) and
 * destination (dest) vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T, class P>
bool Graph<T, P>::addEdge(const T &sourc, const T &dest, P info, double w) {
	Vertex<T, P> * sourcVertex = findVertex(sourc);
	Vertex<T, P> * destVertex = findVertex(dest);
	if (sourcVertex == NULL || destVertex == NULL) return false;
	sourcVertex->addEdge(destVertex, info, w);
	return true;
}

template <class T, class P>
bool Graph<T, P>::addEdge(Vertex<T, P> *sourcVertex, Vertex<T, P> *destVertex, P info, double w) {
    if (sourcVertex == NULL || destVertex == NULL) return false;
    sourcVertex->addEdge(destVertex, info, w);
    return true;
}

template <class T, class P>
bool Graph<T, P>::addEdge(const T &sourc, const T &dest, P info) {
    Vertex<T, P> * sourcVertex = findVertex(sourc);
    Vertex<T, P> * destVertex = findVertex(dest);
    if (sourcVertex == NULL || destVertex == NULL) return false;
    sourcVertex->addEdge(destVertex, info, (double) info);
    return true;
}

/**
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T, class P>
void Vertex<T, P>::addEdge(Vertex<T, P> *d, P inf, double w) {
    for (Edge<T, P> &edge : adj) {
        if (edge.dest->info.getID() == d->info.getID()) {
            return;
        }
    }
    adj.push_back(Edge<T, P>(d, inf, w));
}

/**
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

/**
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

template<class T, class P>
void Vertex<T, P>::setPoi(string poi) {
    this->info.setPoi(poi);
}

template<class T, class P>
T &Vertex<T, P>::getInfo() {
    return info;
}

/**
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

#endif /* GRAPH_H_ */
