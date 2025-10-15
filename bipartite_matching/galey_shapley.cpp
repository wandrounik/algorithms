#include <iostream>
#include <queue>
#include <vector>

class Graph {
private:
    std::vector<std::vector<int>> men_preferences;
    std::vector<std::vector<int>> women_rank;

    std::vector<int> men_pairs;
    std::vector<int> women_pairs;

public:
    Graph(std::vector<std::vector<int>>&& mpreferences, std::vector<std::vector<int>>&& wpreferences) {
        men_preferences = std::move(mpreferences);

        men_pairs.assign(men_preferences.size(), -1);
        women_pairs.assign(wpreferences.size(), -1);

        women_rank.assign(wpreferences.size(), std::vector<int>(men_preferences.size()));
        for(size_t i = 0; i < wpreferences.size(); ++i) {
            for(size_t j = 0, rank = 0; j < wpreferences[i].size(); ++j) {
                women_rank[i][ wpreferences[i][j] ] = rank++;
            }
        }
    }

    void solve() {
        std::queue<size_t> free_men;
        for(size_t i = 0; i < men_preferences.size(); ++i) {
            free_men.push(i);
        }

        std::vector<int> next_candidate_idx(men_preferences.size(), 0);
        while(!free_men.empty()) {
            const auto man_idx = free_men.front();
            free_men.pop();

            while(true) {
                const auto woman_idx = men_preferences[man_idx][ next_candidate_idx[ man_idx ]++ ];
                const auto current_man_idx = women_pairs[woman_idx];

                std::cout << "man_idx: " << man_idx << ", woman_idx: " << woman_idx << ", current_man_idx: " << current_man_idx << std::endl;

                if(current_man_idx == -1) {  // woman is free
                    std::cout << "woman was free" << std::endl;

                    men_pairs[man_idx] = woman_idx;
                    women_pairs[woman_idx] = man_idx;
                    break;
                }

                if(women_rank[woman_idx][man_idx] < women_rank[woman_idx][current_man_idx]) {
                    std::cout << "win a woman" << std::endl;

                    men_pairs[man_idx] = woman_idx;
                    women_pairs[woman_idx] = man_idx;

                    men_pairs[current_man_idx] = -1;
                    free_men.push(current_man_idx);
                    break;
                }

                std::cout << "nothing happened" << std::endl;
            }
        }
    }

    void print() const {
        for(size_t i = 0; i < men_pairs.size(); ++i) {
            std::cout << "Man: " << i << ", Woman: " << men_pairs[i] << std::endl;
        }
    }
};

int main() {
    std::vector<std::vector<int>> men_preferences {
        { 1, 2, 0 },
        { 1, 0, 2 },
        { 1, 0, 2 }
    };

    std::vector<std::vector<int>> women_preferences {
        { 0, 1, 2 },
        { 1, 2, 0 },
        { 2, 0, 1 }
    };

    Graph graph(std::move(men_preferences), std::move(women_preferences));

    graph.solve();
    graph.print();

    return 0;
}