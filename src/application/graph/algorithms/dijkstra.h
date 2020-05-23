#ifndef SRC_DIJKSTRA_H
#define SRC_DIJKSTRA_H

#include "../Graph.h"
#include "../MutablePriorityQueue.h"
#include <cfloat>
#include "../../exceptions.h"

template<class T>
void Graph<T>::dijkstra(Vertex<T> * s) {
    for (typename vector<Vertex<T> *>::const_iterator it = vertexSet.begin(); it != vertexSet.end(); it++) {
        (*it)->dist = DBL_MAX;
        (*it)->path = NULL;
    }

    if (s == NULL) return;

    s->dist = 0;

    MutablePriorityQueue<Vertex<T>> vertexQueue;

    vertexQueue.insert((Vertex<T> *) s);

    while(!vertexQueue.empty()) {
        s = vertexQueue.extractMin();

        for (Edge<T> * a : s->adj) {
            if (a->dest->getDist() > s->getDist() + a->weight) {
                double oldDist = a->dest->getDist();
                a->dest->dist = s->getDist() + a->weight;
                a->dest->path = s;
                if (oldDist == DBL_MAX) {
                    vertexQueue.insert((Vertex<T> *) a->dest);
                } else {
                    vertexQueue.decreaseKey(a->dest);
                }
            }
        }
    }
}

template<class T>
pair<vector<pair<Vertex<T>*, Edge<T> *>>, double> Graph<T>::getPathToFromDijkstra(Vertex<T> * s, Vertex<T> * d) const {
    typedef pair<Vertex<T>*, Edge<T>*> vertexEdgePair;
    // Note, this requires dijkstra algorithm to have been executed starting on vertex *s
    vector<vertexEdgePair> path;
    Vertex<T> * prevVertex, * currVertex = d;

    if (currVertex->dist == DBL_MAX) throw ImpossibleToReach(); // Impossible to reach

    while ((prevVertex = currVertex->path) != NULL) {
        bool found = false;
        for (Edge<T> * edge : prevVertex->adj) {
            if (edge->dest == currVertex) {
                path.push_back(pair<Vertex<T>*, Edge<T>*>(currVertex, edge));
                found = true;
                break;
            }
        }
        if (!found) throw exception();
        currVertex = prevVertex;
    }

    //THIS METHOD ISN'T SUPPOSED TO INSERT THE START VERTEX
    //path.push_back(pair<Vertex<T>*, Edge<T>*>(s, NULL)); // inserts the start vertex, which isn't inserted in the loop

    reverse(path.begin(), path.end());

    return pair<vector<pair<Vertex<T>*, Edge<T> *>>, double>(path, d->dist);
}


template<class T>
vector<Vertex<T>*> Graph<T>::backtrackingDijkstra(Vertex<T> * origin, Vertex<T>* destination, vector<POI> pois, int km){

    vector<Vertex<T>*> passingVertex;

    dijkstra(origin);

    passingVertex.push_back(origin);

    if (destination->dist > km) {
        destination->dist = DBL_MAX;
        return passingVertex;
    }

    int dist = destination->dist;

    for (POI poi: pois){
        for (Vertex<T> *v: vertexSet){
            if (v->info.getCategory() == poi.getType()){
                int temp_dist = v->dist;
                dijkstra(v);
                temp_dist += destination->dist;
                if (v->dist > km){
                    continue;
                }
                passingVertex.push_back(v);
                dist = temp_dist;
            }
        }
    }
    passingVertex.push_back(destination);
    return passingVertex;
}



#endif //SRC_DIJKSTRA_H
