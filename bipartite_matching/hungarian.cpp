#include <iostream>
#include <limits>
#include <queue>
#include <vector>

class Graph {
private:
    std::vector<std::vector<int>> costs;

    std::vector<int> pairL;
    std::vector<int> pairR;

    std::vector<int> potentialL;
    std::vector<int> potentialR;

private:
    bool bfs(std::vector<int>& levels, std::vector<bool>& visitedL, std::vector<bool>& visitedR) {
        std::queue<size_t> queue;
        for(size_t i = 0; i < pairL.size(); ++i) {
            if(pairL[i] == -1) {
                queue.push(i);
                levels[i] = 0;
            }
        }

        bool finished = false;
        while(!queue.empty() && !finished) {
            auto size = queue.size();
            while(size-- > 0) {
                const auto from = queue.front();
                queue.pop();

                if(visitedL[from]) continue;
                visitedL[from] = true;

                for(size_t to = 0; to < costs[from].size(); ++to) {
                    if(visitedR[to] || (potentialL[from] + potentialR[to] != costs[from][to])) continue;
                    visitedR[to] = true;

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

    bool dfs(size_t from, const std::vector<int>& levels, std::vector<bool>& visited) {
        if(visited[from]) return false;
        visited[from] = true;

        for(size_t to = 0; to < costs[from].size(); ++to) {
            if(potentialL[from] + potentialR[to] != costs[from][to]) continue;

            if((pairR[to] == -1) || ((levels[ pairR[to] ] == levels[from] + 1) && dfs(pairR[to], levels, visited))) {
                pairL[from] = to;
                pairR[to] = from;
                return true;
            }
        }
        return false;
    }

    void update_potentials(const std::vector<bool>& visitedL, const std::vector<bool>& visitedR) {
        int delta = std::numeric_limits<int>::max();
        for(size_t i = 0; i < visitedL.size(); ++i) {
            if(!visitedL[i]) continue;

            for(size_t j = 0; j < visitedR.size(); ++j) {
                if(visitedR[j] || (potentialL[i] + potentialR[j] - costs[i][j] == 0)) continue;

                delta = std::min(delta, potentialL[i] + potentialR[j] - costs[i][j]);
            }
        }

        for(size_t i = 0; i < visitedL.size(); ++i) {
            if(!visitedL[i]) continue;
            potentialL[i] -= delta;
        }

        for(size_t i = 0; i < visitedR.size(); ++i) {
            if(!visitedR[i]) continue;
            potentialR[i] += delta;
        }
    }

public:
    explicit Graph(const std::vector<std::vector<int>>& costs) : costs(costs) {
        const auto n = costs.size();        // L-vertixes
        const auto m = costs[0].size();     // R-vertixes

        pairL.assign(n, -1);
        pairR.assign(m, -1);

        potentialL.assign(n, std::numeric_limits<int>::min());
        potentialR.assign(m, 0);

        for(size_t i = 0; i < n; ++i) {
            for(size_t j = 0; j < m; ++j) {
                potentialL[i] = std::max(potentialL[i], costs[i][j]);
            }
        }
    }

    int max_matching() {
        size_t matches_counter = 0;
        while(matches_counter < std::min(costs.size(), costs[0].size())) {   // Until we get a perfect matching

            std::vector<bool> visitedL(pairL.size(), false);
            std::vector<bool> visitedR(pairR.size(), false);

            std::vector<int> levels(pairL.size(), -1);
            if(bfs(levels, visitedL, visitedR)) {
                std::vector<bool> visited(pairL.size(), false); 
                for(size_t u = 0; u < pairL.size(); ++u) {
                    if((pairL[u] == -1) && dfs(u, levels, visited)) {
                        ++matches_counter;
                    }
                }
            }

            update_potentials(visitedL, visitedR);
        }

        int total_sum = 0;
        for(size_t i = 0; i < pairL.size(); ++i) {
            if(pairL[i] != -1) {
                total_sum += costs[i][pairL[i]];
            }
        }

        return total_sum;
    }

    void print() const {
        for(size_t i = 0; i < pairL.size(); ++i) {
            std::cout << "l: " << i << ", r: " << pairL[i] << ", cost: " << costs[i][ pairL[i] ] << std::endl;
        }
    }
};

int main() {
    /*
    std::vector<std::vector<int>> costs = {
        { 3, 1, 2 },
        { 4, 2, 5 },
        { 5, 3, 1 }
    };
    */

    std::vector<std::vector<int>> costs = {
        {  4, 10, 10, 10,  2,  9,  3 },
        {  6,  8,  5, 12,  9,  7,  2 },
        { 11,  9,  6,  7,  9,  5, 15 },
        {  3,  9,  6,  7,  5,  6,  3 },
        {  2,  6,  5,  3,  2,  4,  2 },
        { 10,  8, 11,  4, 11,  2, 11 },
        {  3,  4,  5,  4,  3,  6,  8 }
    };

    Graph graph(costs);
    std::cout << "Max matching: " << graph.max_matching() << std::endl << std::endl;

    graph.print();
    return 0;
}