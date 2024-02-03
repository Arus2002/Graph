#include "graphWithList.hpp"
#include <iostream>
#include <stdexcept>
#include <queue>
#include <stack>

Graph::Graph(std::size_t size) 
    : m_size{size}
    , m_vertexes{m_size} {}

void Graph::addVertex() {
    m_vertexes.push_back(std::forward_list<int>());
    m_size++;
}

void Graph::addEdge(int src, int dest) {
    if (src >= m_size || dest >= m_size) {
        return;
    }
    m_vertexes[src].push_front(dest);
    //m_vertexes[dest].push_front(src);
}

void Graph::BFS(int start) const {
    if (start >= m_size) {
        return;
    }

    std::queue<int> q;
    std::vector<bool> visited(m_size, false);

    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int currentVertex = q.front();
        std::cout << currentVertex << " ";
        q.pop();

        for (const auto& neighbor : m_vertexes[currentVertex]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }

    std::cout << std::endl;
}

void Graph::DFSHelper(int start, std::vector<bool>& visited) {
    visited[start] = true;
    std::cout << start << " ";

    for (const auto& itr: m_vertexes[start]) {
        if (!visited[itr]) {
            DFSHelper(itr, visited);
        }
    }
}

void Graph::DFSRec(int start) {
    if (start >= m_size) {
        return;
    }

    std::vector<bool> visited(m_size, false);
    DFSHelper(start, visited);

    std::cout << std::endl;
}

std::size_t Graph::componentsCount() const {
    if (m_size == 0) {
        return -1;
    }

    std::vector<bool> visited(m_size, false);
    std::queue<int> q;

    q.push(0);
    visited[0] = true;

    int count = 0;

    for (int i = 0; i < m_size; ++i) {
        if (!visited[i]) {
            ++count;
            while (!q.empty()) {
                int tmp = q.front();
                q.pop();
                for (const auto& neighbor : m_vertexes[tmp]) {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
        }
    }

    return count;
}

std::vector<int> Graph::shortestPath(int src, int dest) const {
    if (src >= m_size || dest >= m_size || src == dest) {
        return {};
    }

    std::vector<int> res;
    std::vector<int> parent(m_size, -1);
    std::vector<bool> visited(m_size, false);
    std::queue<int> q;

    q.push(src);
    visited[src] = true;

    while (!q.empty()) {
        int tmp = q.front();
        q.pop();
        for (auto neighbor : m_vertexes[tmp]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
                parent[neighbor] = tmp;
                if (neighbor == dest) {
                    while (neighbor != -1) {
                        res.push_back(neighbor);
                        neighbor = parent[neighbor];
                    }
                    return res;
                }
            }
        }
        
    }
    return res;
}

int Graph::nodesCountAtAGivenLevel(int level) {
    if (level >= m_size) {
        return -1;
    }

    std::queue<int> q;
    std::vector<bool> visited(m_size, false);

    visited[0] = true;
    q.push(0);


    while (!q.empty()) {
        int currentVertex = q.front();
        q.pop();
        --level;

        if (level == 0) {
            return std::distance(m_vertexes[currentVertex].begin(), m_vertexes[currentVertex].end());
        }

        for (const auto& neighbor : m_vertexes[currentVertex]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
    return -1;
}

bool Graph::hasCycleUndirectedHelper(std::vector<int>& visited, int source, int parent) const {
    visited[source] = true;

    for (const auto& neighbour: m_vertexes[source]) {
        if (!visited[neighbour]) {
            if (hasCycleUndirectedHelper(visited, neighbour, source)) {
                return true;
            }
        }
        else if (parent != neighbour) {
                return true;
        }
    }
    return false;
}

bool Graph::hasCycleUndirected() const {
    int parent = -1;
    std::vector<int> visited(m_size);

    for (int i = 0; i < m_size; ++i) {
        if (!visited[i]) {
            if (hasCycleUndirectedHelper(visited, i, -1)) {
                return true;
            }
        }
    }
    return false;
}


bool Graph::hasCycleDirectedHelper(std::vector<int>& visited, int source, std::vector<int>& onStack) const {
    visited[source] = true;
    onStack[source] = true;

    for (const auto& neighbour: m_vertexes[source]) {
        if (!visited[neighbour]) {
            if (hasCycleDirectedHelper(visited, neighbour, onStack)) {
                return true;
            }
        }
        else if (onStack[neighbour]) {
                return true;
        }
    }

    onStack[source] = false;
    return false;
}

bool Graph::hasCycleDirected() const {
    std::vector<int> visited(m_size);
    std::vector<int> onStack(m_size); 

    for (int i = 0; i < m_size; ++i) {
        if (!visited[i]) {
            if (hasCycleDirectedHelper(visited, i, onStack)) {
                return true;
            }
        }
    }
    return false;
}

void Graph::topSortWithDfsHelper(int start, std::vector<bool>& visited, std::forward_list<int>& res) const {
    visited[start] = true;

    for (const auto& itr: m_vertexes[start]) {
        if (!visited[itr]) {
            topSortWithDfsHelper(itr, visited, res);
        }
    }
    
    res.push_front(start);
}

std::forward_list<int> Graph::topSortWithDfs() const {
    std::vector<bool> visited(m_size);
    std::forward_list<int> res;

    for (int i = 0; i < m_size; ++i) {
        if (!visited[i]) {
            topSortWithDfsHelper(i, visited, res);
        }
    }

    return res;
}

void Graph::topSortWithKahn() const {
    //count inDegrees
    std::vector<int> inDegree(m_size, 0);
    for (int i = 0; i < m_size; ++i) {
        for (const auto& itr: m_vertexes[i]) {
            ++inDegree[itr];
        }
    }

    //find start element(element with indegreee=0)
    std::queue<int> q;
    for (int i = 0; i < inDegree.size(); ++i) {
        if (inDegree[i] == 0) {
            q.push(i);
        }
    }

    //Kahn's algorithm
    std::vector<int> topSort;
    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        topSort.push_back(curr);
        for (const auto& neighbour: m_vertexes[curr]) {
            --inDegree[neighbour];
            if (inDegree[neighbour] == 0) {
                q.push(neighbour);
            }
        }
    }

    if (topSort.size() != m_size) {
        std::cout << "There's cycle!" << std::endl;
        return;
    }

    for (int i = 0 ; i < topSort.size(); ++i) {
        std::cout << topSort[i] << " ";
    }
    std::cout << std::endl;
}

void Graph::DFSIter(int start) const {
    if (start >= m_size) {
        return;
    }

    std::stack<int> s;
    std::vector<bool> visited(m_size, false);
    
    s.push(start);

    while (!s.empty()) {
        int tmp = s.top();
        s.pop();
        if (!visited[tmp]) {
            visited[tmp] = true;
            std::cout << tmp << " ";

            for (const auto& itr: m_vertexes[tmp]) {
                if (!visited[itr]) {
                    s.push(itr);
                }
            }
        }
        std::cout << std::endl;
    }
}

std::vector<std::forward_list<int>> Graph::transpoze() const {
    std::vector<std::forward_list<int>> res(m_size);

    for (int i = 0; i < m_size; ++i) {
        for (const auto& itr2: m_vertexes[i]) {
            res[itr2].push_front(i);
        }
    } 

    return res;
}

void Graph::SCCKosarajuHelper1(int start, std::vector<bool>& visited, std::stack<int>& res) {
    visited[start] = true;

    for (const auto& itr: m_vertexes[start]) {
        if (!visited[itr]) {
            SCCKosarajuHelper1(itr, visited, res);
        }
    }
    
    res.push(start);
}

void Graph::SCCKosarajuHelper3(int start, std::vector<bool>& visited, std::vector<int>& component, std::vector<std::forward_list<int>>& transpozedGraph) {
    visited[start] = true;
    component.push_back(start);
    
    for (const auto& itr: transpozedGraph[start]) {
        if (!visited[itr]) {
            SCCKosarajuHelper3(itr, visited, component, transpozedGraph);
        }
    }
}

int Graph::SCCKosaraju() {
    std::vector<bool> visited(m_size);
    std::stack<int> onStack;

    for (int i = 0; i < m_size; ++i) {
        if (!visited[i]) {
            SCCKosarajuHelper1(i, visited, onStack);
        }
    }
    
    std::vector<std::forward_list<int>> transpozedGraph = transpoze();
    
    std::fill(visited.begin(), visited.end(), false);

    int SCC = 0;

    while (!onStack.empty()) {
        int curr = onStack.top();
        onStack.pop();
        if (!visited[curr]) {
            std::vector<int> component;
            ++SCC;
            SCCKosarajuHelper3(curr, visited, component, transpozedGraph);
            std::cout << "Strongly Connected Component:";
            for (int vertex : component) {
                std::cout << " " << vertex;
            }
            std::cout << std::endl;
        }
    }

    return SCC;
}

std::vector<std::vector<int>> Graph::SCCTarjan() {
    std::vector<int> ids(m_size, -1);
    std::vector<int> lls(m_size, -1);
    std::stack<int> stack;
    std::vector<bool> onStack(m_size, false);
    std::vector<std::vector<int>> result;
    int id = 0;
    for (int i = 0; i < m_size; ++i) {
        if (ids[i] == -1) {
            SCCTarjanHelper(i, ids, lls, stack, onStack, result, id);
        }
    }

    return result;
}

void Graph::SCCTarjanHelper(int src, std::vector<int>& ids, std::vector<int>& lls, std::stack<int>& stack, std::vector<bool>& onStack, std::vector<std::vector<int>>& result, int& id) {
    ids[src] = lls[src] = id;
    ++id;
    onStack[src] = true;
    stack.push(src);

    for (const auto& vertex: m_vertexes[src]) {
        if (ids[vertex] == -1) {
            SCCTarjanHelper(vertex, ids, lls, stack, onStack, result, id);
        }
        if (onStack[vertex]) {
            lls[src] = std::min(lls[src], lls[vertex]);
        }
    }

    if (lls[src] == ids[src]) {
        std::vector<int> component;
        while (true) {
            int curr = stack.top();
            stack.pop();
            onStack[curr] = false;
            component.push_back(curr);
            if (curr == src) {
                break;
            }
        }
        result.push_back(component);
    }
}