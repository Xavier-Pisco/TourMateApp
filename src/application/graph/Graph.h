#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <iostream>
#include "../containers/Position.h"
#include "MutablePriorityQueue.h"

using namespace std;

template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;


template <class T>
class Vertex {
	T info;                // contents
	vector<Edge<T> *> adj;  // list of outgoing edges
	bool visited;          // auxiliary field used by dfs and bfs
	int indegree;          // auxiliary field used by topsort
	bool processing;       // auxiliary field used by isDAG
	int graphViewerID; // auxiliary for GraphViewer
	double dist;
	Vertex<T> * path;
    int queueIndex;

	void addEdge(Edge<T> * edge);
	bool removeEdgeTo(Vertex<T> *d);
public:
	Vertex(T in);
	~Vertex();
	T &getInfo();
	void setPoi(string poi);
	double getDist() { return dist; }
    vector<Edge<T>> getAdj() { return adj; }
    bool operator<(Vertex<T> & vertex) const; // // required by MutablePriorityQueue
    friend class Graph<T>;
	friend class MutablePriorityQueue<Vertex<T>>;
    friend class GraphViewerCustom;
    friend class MapContainer;
};

template <class T>
class Edge {
protected:
	Vertex<T> * dest;      // destination vertex
	double weight;
	bool infoXML = false;
public:
	Edge(Vertex<T> *d, double w);
	bool hasInfoXML() const;
	friend class Graph<T>;
	friend class Vertex<T>;
    friend class GraphViewerCustom;
};

template<class T>
bool Edge<T>::hasInfoXML() const {
    return hasInfoXML;
}

template <class T>
class Graph {
	vector<Vertex<T> *> vertexSet;    // vertex set
	vector<Vertex<T> *> originalVertexSet;
    pair<double, double> minCoords = {0, 0}, maxCoords = {0, 0};

	void dfsVisit(Vertex<T> *v,  vector<Vertex<T>*> & res) const;
	bool dfsIsDAG(Vertex<T> *v) const;

public:
    ~Graph();
    Vertex<T> *findVertex(const T &in) const;
    const vector<Vertex<T>*> &getVertexSet() const;
    void setMaxMinCoords(pair<double, double> mn, pair<double, double> mx);
    void setVertexSet(vector<Vertex<T>*> v);
    void setOriginalVertexSet();
	int getNumVertex() const;
	bool addVertex(const T &in);
    bool addVertex(Vertex<T> * in);
	bool addVertex(const T &in, const Position &p);
	bool removeVertex(const T &in);
	bool addEdge(const T &sourc, const T &dest, double w);
    bool addEdge(Vertex<T> *sourcVertex, Edge<T> * edge);
	bool removeEdge(const T &sourc, const T &dest);
    vector<Vertex<T>*> dfs() const;
    vector<Vertex<T>*> bfs(Vertex<T> * source) const;
	vector<T> topsort() const;
	int maxNewChildren(const T &source, T &inf) const;
	bool isDAG() const;
	void dijkstra(Vertex<T> * origin);
    pair<vector<pair<Vertex<T>*, Edge<T>*>>, double> getPathToFromDijkstra(Vertex<T> * s, Vertex<T> * d) const;
	friend class GraphViewerCustom;
	friend class MapContainer;
};

template<class T>
Vertex<T>::Vertex(T in): info(in) {}

template<class T>
Vertex<T>::~Vertex() {
    for (Edge<T> * e : adj) delete e;
}

template<class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w) {}


template<class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
}

template<class T>
const vector<Vertex<T>*> &Graph<T>::getVertexSet() const {
    return vertexSet;
}

template<class T>
Graph<T>::~Graph() {
    for (Vertex<T> * v : vertexSet) delete v;
}

template<class T>
void Graph<T>::setMaxMinCoords(pair<double, double> mn, pair<double, double> mx) {
    this->minCoords = mn;
    this->maxCoords = mx;
}

template<class T>
void Graph<T>::setVertexSet(vector<Vertex<T>*> v) {
    if (originalVertexSet.empty()) setOriginalVertexSet();
    vertexSet = v;
}

template<class T>
void Graph<T>::setOriginalVertexSet() {
    originalVertexSet.erase(originalVertexSet.begin(), originalVertexSet.end());
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        originalVertexSet.push_back(*it);
    }
}

template<class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
    return this->dist < vertex.dist;
}

/**
 * Auxiliary function to find a vertex with a given content.
 */
template<class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
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
template<class T>
bool Graph<T>::addVertex(const T &in) {
    if (findVertex(in) != NULL) return false;
    auto vertex = new Vertex<T>(in);
    vertexSet.push_back(vertex);
    return true;
}

template<class T>
bool Graph<T>::addVertex(Vertex<T> * vertex) {
    vertexSet.push_back(vertex);
    return true;
}


template<class T>
bool Graph<T>::addVertex(const T &in, const Position &p) {
    if (findVertex(in) != NULL) return false;
    auto vertex = new Vertex<T>(in);
    vertex->position = p;
    vertexSet.push_back(vertex);
    return true;
}

/**
 * Adds an edge to a graph (this), given the contents of the source (sourc) and
 * destination (dest) vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template<class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
	Vertex<T> * sourcVertex = findVertex(sourc);
	Vertex<T> * destVertex = findVertex(dest);
	if (sourcVertex == NULL || destVertex == NULL) return false;
	sourcVertex->addEdge(Edge<T>(sourcVertex, w));
	return true;
}

template<class T>
bool Graph<T>::addEdge(Vertex<T> *sourcVertex, Edge<T> * edge) {
    if (sourcVertex == NULL) return false;
    sourcVertex->addEdge(edge);
    return true;
}

/**
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template<class T>
void Vertex<T>::addEdge(Edge<T> * edge) {
    for (Edge<T> * e : adj) {
        if (e->dest->info.getID() == edge->dest->info.getID()) {
            return;
        }
    }
    adj.push_back(edge);
}

/**
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
template<class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
	Vertex<T> * sourcVertex = findVertex(sourc);
    Vertex<T> * destVertex = findVertex(dest);
    if (sourcVertex == NULL || destVertex == NULL) return false;
    return sourcVertex->removeEdgeTo(destVertex);
}

/**
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */
template<class T>
bool Vertex<T>::removeEdgeTo(Vertex<T> *d) {
    for (auto it = adj.begin(); it != adj.end(); it++) {
        if (it->dest->info == d->info) {
            adj.erase(it);
            return true;
        }
    }
	return false;
}

template<class T>
void Vertex<T>::setPoi(string poi) {
    this->info.setPoi(poi);
}

template<class T>
T &Vertex<T>::getInfo() {
    return info;
}

/**
 *  Removes a vertex with a given content (in) from a graph (this), and
 *  all outgoing and incoming edges.
 *  Returns true if successful, and false if such vertex does not exist.
 */
template<class T>
bool Graph<T>::removeVertex(const T &in) {
    Vertex<T> * destVertex = findVertex(in);
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
