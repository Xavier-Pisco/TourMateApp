#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <iostream>
#include "../containers/Position.h"
#include "MutablePriorityQueue.h"
#include "../containers/POI.h"
#include <stack>

using namespace std;

template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;
template <class T> class MapContainer;
template <class T> class GraphViewerCustom;

template <class T>
class Vertex {
	T info;                // contents
	vector<Edge<T> *> adj;  // list of outgoing edges
	bool visited;          // auxiliary field used by dfs and bfs
	int indegree;          // auxiliary field used by topsort
	bool processing;       // auxiliary field used by isDAG
	int graphViewerID; // auxiliary for GraphViewer
	Vertex<T> * transposeVertex; // auxiliary for Strongly Connected Components (heps accelerate the process)

	/* fields for dijkstra algorithm */
	double dist;
	Vertex<T> * path;
    int queueIndex;  // for the MutablePriorityQueue

    /* fields for findArticulationPoint */
    int low, num, childNo/* Only for the root */;
    Vertex<T> * parent;


    /* fields for making route */
    bool lessPreferable;



	void addEdge(Edge<T> * edge);
	bool removeEdgeTo(Vertex<T> *d);
public:
	Vertex(T in);
	~Vertex();
	T &getInfo();
	void setPoi(string poi);
	double getDist() { return dist; }
	bool getVisited() {return visited;}
	bool getLessPreferable() {return lessPreferable;}
	void setLessPreferable(bool s) { lessPreferable = s; }
	void setVisited(bool v) { visited = v; }
	void setDist(double d) {dist = d;}
    vector<Edge<T>*> getAdj() { return adj; }
    bool operator<(Vertex<T> & vertex) const; // // required by MutablePriorityQueue
    friend class Graph<T>;
	friend class MutablePriorityQueue<Vertex<T>>;
    friend class GraphViewerCustom<T>;
    friend class MapContainer<T>;
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
    friend class GraphViewerCustom<T>;
};

template<class T>
bool Edge<T>::hasInfoXML() const {
    return infoXML;
}

template <class T>
class Graph {
	vector<Vertex<T> *> vertexSet;    // vertex set
	vector<Vertex<T> *> originalVertexSet;
    pair<double, double> minCoords = {0, 0}, maxCoords = {0, 0};
    int counter; // required for findArticulationPoints

	void dfsVisit(Vertex<T> *v,  stack<Vertex<T>*> & res) const;
    void dfsVisit(Vertex<T> *v,  vector<Vertex<T>*> & res) const;
	bool dfsIsDAG(Vertex<T> *v) const;

    /**
     * @brief auxialiary function for findArticulationPoints
     * @param vx  - the vertex
     * @param v - the articulation point vector
     */
    void findArt(Vertex<T> * vx, vector<Vertex<T>*> &v, Vertex<T> * rootVx, bool root = false);

public:
    ~Graph();
    Vertex<T> *findVertex(const T &in) const;
    Vertex<T> *findVertex(long id) const;
    const vector<Vertex<T>*> &getVertexSet() const;
    void setMaxMinCoords(pair<double, double> mn, pair<double, double> mx);
    const pair<double,double> &getMinCoords() const;
    const pair<double,double> &getMaxCoords() const;
    void setVertexSet(vector<Vertex<T>*> v);
    void setOriginalVertexSet();
	int getNumVertex() const;
	Vertex<T> * addVertex(const T &in);
    bool addVertex(Vertex<T> * in);
	bool addVertex(const T &in, const Position &p);
	bool removeVertex(const T &in);
	bool addEdge(long sourcID, long destID, double w);
    bool addEdge(Vertex<T> *sourcVertex, Edge<T> * edge);
	bool removeEdge(const T &sourc, const T &dest);
    stack<Vertex<T>*> dfsToStack() const;
    vector<Vertex<T>*> dfs() const;
    vector<Vertex<T>*> bfs(Vertex<T> * source) const;
	vector<T> topsort() const;
	int maxNewChildren(const T &source, T &inf) const;
	bool isDAG() const;

	/**
	 * @brief returns a transposed version of the graph
	 */
	Graph<T> * getTranspose() const;

	/**
	 * @brief finds the strongly connected components in the graph, using Kosaraju's algorithm
	 * @return a vector of strongly connected components
	 */
    vector<vector<Vertex<T>*>> stronglyConnectedComponents();

    /**
     * @brief makes dfs in the vertexes from the stack in stack order, return a vector with the results for each vertex
     * @param g - the graph
     * @param s - the stack
     * @return vector with each vertex's strongly connected components
     */
    static vector<vector<Vertex<T>*>> dfsFromStack(Graph<T> * g, stack<Vertex<T> *> &s);

    /**
     * @brief finds articulation points in the graph (if no articulation points, graph is biconnected
     * an articulation point is a point that when removed makes the graph disconnected)
     */
    vector<Vertex<T>*> findArticulationPoints();

    /**
	 * @brief dijkstra algorithm starting on origin
	 * @param origin - the start vertex
	 */
	void dijkstra(Vertex<T> * origin);

	/**
	 * @brief Function that uses backtracking and dijkstra in order to see what points of interest the user can pass between its origin and destination
	 *
	 * This function will always prioritize pois by the order they appear in the pois vector
	 *
	 * @param origin - origin of the route
	 * @param destination - destination of the route
	 * @param pois - User's poiList
	 * @param km - maximum distance the route can have
	 * @return vector with all the poi the user can pass by, starts at origin and (if possible on km) ends at destination
	 */
	vector<Vertex<T>*> backtrackingDijkstra(Vertex<T> * origin, Vertex<T>* destination, vector<POI> pois, int km);

	/**
	 * @brief gets a path from s to d, when dijkstra has been executed starting on s
	 * @param s - the origin vertex
	 * @param d - the destination vertex
	 * @return {{{firstVertex, nullptr}, {secondVertex, edgeThatTakesToSecondVertex}, ...}, length}  we return the edge because, in the case of OSM maps, it contains the name of the road.
	 */
    pair<vector<pair<Vertex<T>*, Edge<T>*>>, double> getPathToFromDijkstra(Vertex<T> * s, Vertex<T> * d) const;

	friend class GraphViewerCustom<T>;
	friend class MapContainer<T>;
	friend class OSMapContainer;
	friend class SimpleMapContainer;
};



template<class T>
const pair<double,double> &Graph<T>::getMinCoords() const {
    return minCoords;
}

template<class T>
const pair<double,double> &Graph<T>::getMaxCoords() const {
    return maxCoords;
}

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
 * Auxiliary function to find a vertex with a given id.
 */
template<class T>
Vertex<T> * Graph<T>::findVertex(long id) const {
    for (auto v : vertexSet) {
        if (v->info.getID() == id) {
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
Vertex<T> * Graph<T>::addVertex(const T &in) {
    if (findVertex(in) != NULL) return nullptr;
    auto vertex = new Vertex<T>(in);
    vertexSet.push_back(vertex);
    return vertex;
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
bool Graph<T>::addEdge(long sourc, long dest, double w) {
	Vertex<T> * sourcVertex = findVertex(sourc);
	Vertex<T> * destVertex = findVertex(dest);
	if (sourcVertex == NULL || destVertex == NULL) return false;
	sourcVertex->addEdge(new Edge<T>(destVertex, w));
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
