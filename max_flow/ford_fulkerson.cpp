#include <vector>
#include <iostream>
#include <limits>

class Graph {
private:
    std::vector<std::vector<int>> _graph;

private:
    size_t dfs(size_t from, size_t finish, std::vector<bool>& visited, int flow) {
        if(visited[from]) return 0;
        if(from == finish) return flow;

        visited[from] = true;

        for(size_t to = 0; to < _graph[from].size(); ++to) {
            if(_graph[from][to] == 0) continue;
            if(auto augmented_flow = dfs(to, finish, visited, std::min(flow, _graph[from][to]))) {
                _graph[from][to] -= augmented_flow;
                _graph[to][from] += augmented_flow;
                return augmented_flow;
            }
        }
        return 0;
    }

public:
    explicit Graph(size_t size) {
        _graph.resize(size, std::vector<int>(size));
    }

    void add_edge(size_t from, size_t to, size_t capacity) {
        _graph[from][to] += capacity;
    }

    size_t max_flow(size_t from, size_t to) {
        size_t total_flow = 0;
        std::vector<bool> visited(_graph.size(), false);

        while(auto flow = dfs(from, to, visited, std::numeric_limits<int>::max())) {
            total_flow += flow;
            visited.assign(visited.size(), false);
        }
        return total_flow;
    }

    int get_capacity(size_t from, size_t to) {
        return _graph[from][to];
    }
};

int main() {
    std::vector<std::vector<size_t>> edges = {
        { 0, 1, 6 },
        { 0, 2, 14 },
        { 1, 2, 1 },
        { 1, 3, 5 },
        { 2, 3, 7 },
        { 2, 4, 10 },
        { 3, 4, 1 },
        { 3, 5, 11 },
        { 4, 5, 12 }
    };

    Graph graph(6);
    for(const auto& edge : edges) {
        graph.add_edge(edge[0], edge[1], edge[2]);
    }

    std::cout << "Max flow: " << graph.max_flow(0, 5) << std::endl << std::endl;

    for(const auto& edge : edges) {
        std::cout << "Flow from " << edge[0] << " to " << edge[1] << ": " << edge[2] - graph.get_capacity(edge[0], edge[1]) << std::endl;
    }
    
    return 0;
}