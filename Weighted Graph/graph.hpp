#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <vector>
#include <queue>              
#include <limits>                   

struct Edge {
    int src;
    int dest;
    int weight;

    Edge(int _src, int _dest, int _w) : src{_src}, dest(_dest), weight(_w) {}
};

class Graph {
public:
    Graph(std::size_t size);
    void addVertex();
    void addEdge(int source, int destination, int weight);
    void dfs(int src);
    void dijkstra(int src);
    void belmanFord(int src);

private:
    void dfs(int src, std::vector<bool>& visited);
    std::vector<Edge> createEdges() const;

private:
    std::size_t m_size;
    std::vector<std::vector<Edge>> m_graph;
};

#endif
