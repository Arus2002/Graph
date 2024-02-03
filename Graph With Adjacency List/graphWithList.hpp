#ifndef GRAPHWITHLIST_HPP
#define GRAPHWITHLIST_HPP

#include <iostream>
#include <vector>
#include <forward_list>
#include <stack>

class Graph {
public:
    Graph() = default;
    Graph(std::size_t size);
    void addVertex();
    void addEdge(int src, int dest);
    void DFSRec(int start);
    void DFSIter(int start) const;
    void BFS(int start) const;
    std::size_t componentsCount() const;
    std::vector<int> shortestPath(int src, int dest) const;
    int nodesCountAtAGivenLevel(int level);
    bool hasCycleUndirected() const;
    bool hasCycleDirected() const;
    std::vector<std::forward_list<int>> transpoze() const;
    std::forward_list<int> topSortWithDfs() const;
    void topSortWithKahn() const;
    int SCCKosaraju();
    std::vector<std::vector<int>> SCCTarjan();

private:
    void SCCKosarajuHelper1(int start, std::vector<bool>& visited, std::stack<int>& res);
    void SCCTarjanHelper(int src, std::vector<int>& ids, std::vector<int>& lls, std::stack<int>& stack, std::vector<bool>& onStack, std::vector<std::vector<int>>& result, int& id);
    std::stack<int> SCCKosarajuHelper2(int start);
    void SCCKosarajuHelper3(int start, std::vector<bool>& visited, std::vector<int>& component, std::vector<std::forward_list<int>>& graph);
    void DFSHelper(int start, std::vector<bool>& visited);
    void topSortWithDfsHelper(int start, std::vector<bool>& visited, std::forward_list<int>& res) const;
    bool hasCycleUndirectedHelper(std::vector<int>& vec, int source, int parent) const;
    bool hasCycleDirectedHelper(std::vector<int>& visited, int source, std::vector<int>& onStack) const;
private:
    std::size_t m_size;
    std::vector<std::forward_list<int>> m_vertexes;
};

#endif