#include <vector>
#include <queue>
#include <iostream>
#include <limits>

class Graph {
private:
    std::vector<std::vector<int>> _graph;

private:
    size_t bfs(size_t start, size_t finish, std::vector<int>& parents) {
        std::queue<std::pair<size_t, int>> queue;    // <vertex, flow>
        queue.emplace(start, std::numeric_limits<int>::max());

        std::vector<bool> visited(_graph.size(), false);

        while(!queue.empty()) {
            const auto from = queue.front().first;
            const auto flow = queue.front().second;
            queue.pop();

            if(from == finish) return flow;
            if(visited[from]) continue;

            visited[from] = true;

            for(size_t to = 0; to < _graph[from].size(); ++to) {
                if(visited[to] || (_graph[from][to] == 0)) continue;

                queue.emplace(to, std::min(flow, _graph[from][to]));
                parents[to] = from;
            }
        }

        return 0;
    }

public:
    explicit Graph(size_t size) {
        _graph.resize(size, std::vector<int>(size));
    }

    void add_edge(size_t from, size_t to, int capacity) {
        _graph[from][to] += capacity;
    }

    size_t max_flow(size_t from, size_t to) {
        size_t total_flow = 0;
        std::vector<int> parents(_graph.size(), -1);

        while(auto flow = bfs(from, to, parents)) {
            total_flow += flow;

            auto it = to;
            while(it != from) {
                _graph[ parents[it] ][ it ] -= flow;
                _graph[ it ][ parents[it] ] += flow;

                it = parents[it];
            }

            parents.assign(parents.size(), -1);
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