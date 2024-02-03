#include "graph.hpp"

Graph::Graph(std::size_t size)
    : m_size{size}
    , m_graph(m_size) {} 

void Graph::addVertex() {
    ++m_size;
    m_graph.push_back({});
}

void Graph::addEdge(int source, int destination, int weight) {
    if (source >= m_size || source < 0) {
        return;
    }

    if (destination >= m_size || destination < 0) {
        return;
    }

    m_graph[source].push_back({source, destination, weight});
}

void Graph::dfs(int src, std::vector<bool>& visited) {
    visited[src] = true;
    std::cout << src << " ";

    for (const auto& itr: m_graph[src]) {
        if (!visited[itr.dest]) {
            dfs(itr.dest, visited);
        }
    }
}

void Graph::dfs(int src) {
    std::vector<bool> visited(m_size, false);
    dfs(src, visited);
    std::cout << std::endl;
}

void Graph::dijkstra(int src) {
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    std::vector<int> distance(m_size, std::numeric_limits<int>::max());

    distance[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        int curr = pq.top().second;
        pq.pop(); 
        for (const Edge& edge : m_graph[curr]) {
            int v = edge.dest;
            int w = edge.weight;
            if (distance[curr] + w < distance[v]) {
                distance[v] = distance[curr] + w;
                pq.push({distance[v], v});
            }
        }
    }

    std::cout << "Shortest distances from vertex " << src << ":" << std::endl;
        for (int i = 0; i < m_size; ++i) {
            std::cout << "To vertex " << i << ": " << distance[i] << std::endl;
        }
}

std::vector<Edge> Graph::createEdges() const {
    std::vector<Edge> edges;
    for (int i = 0; i < m_size; ++i) {
        for (int j = 0; j < m_graph[i].size(); ++j) {
            edges.push_back(m_graph[i][j]);
        }
    }

    return edges;
}

void Graph::belmanFord(int src) {
    std::vector<int> distance(m_size, std::numeric_limits<int>::max());
    std::vector<Edge> edges = createEdges();
    distance[src] = 0;

    for (int i = 0; i < m_size - 1; ++i) {
        for (const Edge& edge : edges) {
            int u = edge.src;
            int v = edge.dest;
            int w = edge.weight;

            if (distance[u] != std::numeric_limits<int>::max() && distance[u] + w < distance[v]) {
                distance[v] = distance[u] + w;
            }
        }
    }

    for (const Edge& edge : edges) {
        int u = edge.src;
        int v = edge.dest;
        int w = edge.weight;

        if (distance[u] != std::numeric_limits<int>::max() && distance[u] + w < distance[v]) {
            std::cout << "Graph contains a negative weight cycle!" << std::endl;
            return;
        }
    }

    std::cout << "Shortest distances from vertex " << src << ":" << std::endl;
    for (int i = 0; i < m_size; ++i) {
        std::cout << "To vertex " << i << ": " << distance[i] << std::endl;
    }

}