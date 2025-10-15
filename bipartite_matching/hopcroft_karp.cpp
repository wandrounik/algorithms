#include <iostream>
#include <queue>
#include <vector>

class Graph {
private:
    std::vector<int> pairL;
    std::vector<int> pairR;

    std::vector<std::vector<int>> adjacency_list;

private:
    bool bfs(std::vector<int>& levels) {
        std::queue<int> queue;
        for(int i = 0; i < pairL.size(); ++i) {
            if(pairL[i] == -1) {
                queue.push(i);
                levels[i] = 0;
            } else {
                levels[i] = -1;
            }
        }

        bool finished = false;
        while(!queue.empty() && !finished) {
            auto size = queue.size();
            while(size-- > 0) {
                const auto from = queue.front();
                queue.pop();

                for(const auto to : adjacency_list[from]) {
                    if(pairR[to] == -1) {
                        finished = true;
                        continue;
                    }

                    if(levels[ pairR[to] ] == -1) {
                        levels[ pairR[to] ] = levels[from] + 1;
                        queue.push(pairR[to]);
                    }
                }
            }
        }
        return finished;
    }

    bool dfs(int from, const std::vector<int>& levels, std::vector<bool>& visited) {
        if(visited[from]) return false;
        visited[from] = true;

        for(const auto to : adjacency_list[from]) {
            if((pairR[to] == -1) || ((levels[ pairR[to] ] == levels[from] + 1) && dfs(pairR[to], levels, visited))) {
                pairL[from] = to;
                pairR[to] = from;
                return true;
            }
        }
        return false;
    }

public:
    Graph(size_t n, size_t m, const std::vector<std::vector<int>>& edges) {
        pairL.assign(n, -1);
        pairR.assign(m, -1);

        adjacency_list.assign(n, {});
        for(const auto& edge : edges) {
            adjacency_list[ edge[0] - 1 ].push_back( edge[1] - 1 );
        }
    }

    size_t solve() {
        std::vector<int> levels(pairL.size());

        size_t counter = 0;
        while(bfs(levels)) {
            std::vector<bool> visited(pairL.size(), false); 
            for(int u = 0; u < pairL.size(); ++u) {
                if((pairL[u] == -1) && dfs(u, levels, visited)) {
                    ++counter;
                }
            }
        }
        return counter;
    }

    void print() const {
        for(size_t i = 0; i < pairL.size(); ++i) {
            if(pairL[i] != -1) {
                std::cout << "L: " << i + 1 << ", R: " << pairL[i] + 1 << std::endl;
            } else {
                std::cout << "L: " << i + 1 << ", R: " << pairL[i] << std::endl;
            }
        }
    }
};

int main() {
    std::vector<std::vector<int>> edges {
        { 1, 2 },
        { 1, 3 },
        { 2, 1 },
        { 2, 2 },
        { 3, 1 },
        { 3, 3 },
        { 3, 4 },
        { 3, 5 },
        { 3, 6 },
        { 4, 2 },
        { 4, 3 },
        { 4, 7 },
        { 5, 4 },
        { 5, 5 },
        { 5, 6 },
        { 5, 7 },
        { 6, 3 },
        { 6, 7 },
        { 7, 5 },
        { 7, 8 }
    };

    Graph graph(7, 8, edges);

    graph.solve();
    graph.print();

    return 0;
}