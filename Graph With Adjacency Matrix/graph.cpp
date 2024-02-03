#include "graph.hpp"

Graph::Graph(std::size_t size)
    : m_size{size}
    , m_graph{size, std::vector<int>(m_size)} 
{}

Graph::Graph() = default;

void Graph::addVertex() {
    m_graph.push_back(std::vector<int>(m_size));
    ++m_size;
}

void Graph::addEdge(std::size_t u, std::size_t v) {
    if (u >= m_size || u < 0) {
        throw std::out_of_range("Out of range!");
    }
    if (v >= m_size || v < 0) {
        throw std::out_of_range("Out of range!");
    }
    m_graph[u][v] = 1;
}

void Graph::bfs(std::size_t start) const {
    std::queue<int> q;
    std::vector<bool> visited(m_size, false);
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int curr = q.front();
        std::cout << curr << " ";
        q.pop();
        for (int i = 0; i < m_size; ++i) {
            if (!visited[i] && m_graph[curr][i] == 1) {
                q.push(i);
                visited[i] = true;
            }
        }
    }
    std::cout << std::endl;
}

void Graph::dfsIter(std::size_t start) const {
    std::stack<int> s;
    std::vector<bool> visited(m_size, false);
    s.push(start);
    visited[start] = true;

    while (!s.empty()) {
        int curr = s.top();
        std::cout << curr << " ";
        s.pop();
        for (int i = 0; i < m_size; ++i) {
            if (!visited[i] && m_graph[curr][i] == 1) {
                s.push(i);
                visited[i] = true;
            }
        }
    }
    std::cout << std::endl;
}

void Graph::dfsHelper(std::size_t start, std::vector<bool>& visited) const {
    visited[start] = true;
    std::cout << start << " ";

    for (int i = 0; i < m_size; ++i) {
        if (!visited[i] && m_graph[start][i] == 1) {
            dfsHelper(i, visited);
        }
    }
}

void Graph::dfsRec(std::size_t start) const {
    std::vector<bool> visited(m_size, false);
    dfsHelper(start, visited);
    std::cout << std::endl;
}

std::size_t Graph::componentsCount() const {
    std::stack<int> s;
    std::vector<bool> visited(m_size, false);
    s.push(0);
    visited[0] = true;

    int count = 0;
    for (int i = 1; i < m_size; ++i) {
        for (int j = 0; j < m_graph[i].size(); ++j) {
            if (!visited[i] && m_graph[i][j] == 1) {
                while (!s.empty()) {
                    int curr = s.top();
                    s.pop();
                    for (int i = 0; i < m_size; ++i) {
                        if (!visited[i] && m_graph[curr][i] == 1) {
                            s.push(i);
                            visited[i] = true;
                        }
                    }
                }
            ++count;
            }
        }   
    }
    return count;
}

bool Graph::hasCycleUndirectedHelper(int start, std::vector<bool>& visited, int parent) {
    visited[start] = true;

    for (int i = 0; i < m_size; ++i) {
        if (m_graph[start][i] == 1) {
            if (!visited[i]) {
                if (hasCycleUndirectedHelper(i, visited, start)) {
                    return true;
                }
            }
            else if (parent != i) {
                return true;
            }
        }
    }
    return false;
}

bool Graph::hasCycleUndirected() {
    std::vector<bool> visited(m_size, false);

    if (hasCycleUndirectedHelper(0, visited, -1)) {
        return true;
    }
    return false;
}

bool Graph::hasCycleDirectedHelper(int start, std::vector<bool>& visited, std::vector<bool>& onStack) {
    visited[start] = true;
    onStack[start] = true;

    for (int i = 0; i < m_size; ++i) {
        if (m_graph[start][i] == 1) {
            if (!visited[i]) {
                if (hasCycleUndirectedHelper(i, visited, start)) {
                    return true;
                }
            }
            else if (onStack[i]) {
                return true;
            }
        }
    }

    return false;
}
    
bool Graph::hasCycleDirected() {
    std::vector<bool> visited(m_size, false);
    std::vector<bool> onStack(m_size, false);

    if (hasCycleDirectedHelper(0, visited, onStack)) {
        return true;
    }
    return false;
}

std::vector<std::vector<int>> Graph::transpoze() const {
    std::vector<std::vector<int>> res(m_size, std::vector<int>(m_size));
    for (int i = 0; i < m_size; ++i) {
        for (int j = 0; j < m_size; ++j) {
            if (m_graph[i][j] == 1) {
                res[j][i] = 1;
            }
        }
    }

    return res;
}

void Graph::topSortWithDfsHelper(int start, std::vector<bool>& visited, std::forward_list<int>& res) {
    visited[start] = true;

    for (int i = 0; i < m_size; ++i) {
        if (!visited[i] && m_graph[start][i] == 1) {
            topSortWithDfsHelper(i, visited, res);
        }
    }

    res.push_front(start);
}

std::forward_list<int> Graph::topSortWithDfs() {
    std::forward_list<int> res;
    std::vector<bool> visited(m_size, false);

    for (int i = 0; i < m_size; ++i) {
        if (!visited[i]) {
            topSortWithDfsHelper(i, visited, res);
        }
    }

    return res;
}

void Graph::topSortWithKahn() const {
    std::vector<int> inDegree(m_size);
    std::vector<int> res;

    for (int i = 0; i < m_size; ++i) {
        for (int j = 0; j < m_size; ++j) {
            if (m_graph[i][j] == 1) {
                inDegree[j]++;
            }
        }
    }

    std::queue<int> q;
    for (int i = 0; i < inDegree.size(); ++i) {
        if (inDegree[i] == 0) {
            q.push(i);
        }
    }

    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        res.push_back(curr);
        for (int i = 0; i < m_size; ++i) {
            if (m_graph[curr][i] == 1) {
                --inDegree[i];
                if (inDegree[i] == 0) {
                    q.push(i);
                }
            }
        }
    }

    if (res.size() != m_size) {
        std::cout << 4;
        return;
    }

    for (int i = 0; i < res.size(); ++i) {
        std::cout << res[i] << " ";
    }
    std::cout << std::endl;
}

void Graph::SCCKosarajuHelper1(int start, std::vector<bool>& visited, std::stack<int>& res) {
    visited[start] = true;

    for (int i = 0; i < m_size; ++i) {
        if (!visited[i] & m_graph[start][i]) {
            SCCKosarajuHelper1(i, visited, res);
        }
    }

    res.push(start);
}

void Graph::SCCKosarajuHelper2(int start, std::vector<bool>& visited, std::vector<int>& component, std::vector<std::vector<int>>& transpozed) {
    visited[start] = true;
    component.push_back(start);

    for (int i = 0; i < m_size; ++i) {
        if (!visited[i] & m_graph[start][i]) {
            SCCKosarajuHelper2(i, visited, component, transpozed);
        }
    }
}

int Graph::SCCKosaraju() {
    std::vector<bool> visited(m_size, false);
    std::stack<int> onStack;

    for (int i = 0; i < m_size; ++i) {
        if (!visited[i]) {
            SCCKosarajuHelper1(i, visited, onStack);
        }
    }

    std::vector<std::vector<int>> transpozedGraph = transpoze();
    
    std::fill(visited.begin(), visited.end(), false);

    int SCC = 0;

    while (!onStack.empty()) {
        int curr = onStack.top();
        onStack.pop();
        if (!visited[curr]) {
            std::vector<int> component;
            ++SCC;
            SCCKosarajuHelper2(curr, visited, component, transpozedGraph);
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

    for (int i = 0; i < m_size; ++i) {
        if (m_graph[src][i] == 1) {
            if (ids[i] == -1) {
                SCCTarjanHelper(i, ids, lls, stack, onStack, result, id);
            }
            if (onStack[i]) {
                lls[src] = std::min(lls[src], lls[i]);
            }
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

