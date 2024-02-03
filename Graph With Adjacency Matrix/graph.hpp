#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <forward_list>

class Graph {
public:
    Graph(std::size_t size);
    Graph();

    void addVertex();
    void addEdge(std::size_t u, std::size_t v);
    void dfsIter(std::size_t start) const;
    void dfsRec(std::size_t start) const;
    void bfs(std::size_t start) const;
    std::size_t componentsCount() const;
    std::vector<int> shortestPath(int src, int dest) const;
    bool hasCycleUndirected();
    bool hasCycleDirected();
    std::vector<std::vector<int>> transpoze() const;
    std::forward_list<int> topSortWithDfs();
    void topSortWithKahn() const;
    int SCCKosaraju();
    std::vector<std::vector<int>> SCCTarjan();

private:
    void dfsHelper(std::size_t start, std::vector<bool>& visited) const;
    bool hasCycleUndirectedHelper(int start, std::vector<bool>& visited, int parent);
    bool hasCycleDirectedHelper(int start, std::vector<bool>& visited, std::vector<bool>& onStack);
    void topSortWithDfsHelper(int start, std::vector<bool>& visited, std::forward_list<int>& res);
    void SCCKosarajuHelper1(int start, std::vector<bool>& visited, std::stack<int>& res);
    void SCCKosarajuHelper2(int start, std::vector<bool>& visited, std::vector<int>& component, std::vector<std::vector<int>>& transpozed);
    void SCCTarjanHelper(int src, std::vector<int>& ids, std::vector<int>& lls, std::stack<int>& stack, std::vector<bool>& onStack, std::vector<std::vector<int>>& result, int& id);

private:
    std::vector<std::vector<int>> m_graph;
    std::size_t m_size;    
};

#endif