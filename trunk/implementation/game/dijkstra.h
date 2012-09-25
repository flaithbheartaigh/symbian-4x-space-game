#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <map>
#include <list>

typedef int vertex_t;
typedef double weight_t;

struct edge {
    const vertex_t target;
    const weight_t weight;
    edge(vertex_t arg_target, weight_t arg_weight)
        : target(arg_target), weight(arg_weight) 
    {}
};

typedef std::map<vertex_t, std::list<edge> > adjacency_map_t;

void DijkstraComputePaths(vertex_t source, const adjacency_map_t &adjacency_map, std::map<vertex_t, weight_t> &min_distance, std::map<vertex_t, vertex_t> &previous);

std::list<vertex_t> DijkstraGetShortestPathTo(vertex_t target, const std::map<vertex_t, vertex_t> &previous);

#endif