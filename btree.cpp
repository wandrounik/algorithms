#include <iostream>
#include <vector>
#include <algorithm>

class BTree {
    struct Node {
        std::vector<int> keys;       // отсортированы по возрастанию
        std::vector<Node*> children; // для внутренних узлов: size == keys.size() + 1
        bool is_leaf;

        explicit Node(bool leaf = false) : is_leaf(leaf) {}
    };

private:
    Node* root = nullptr;
    size_t t; // min_degree (минимальная степень)

private:
    void inorderTraversalPrint(const Node* node) const {
        if (!node) return;
        size_t m = node->keys.size();
        for (size_t i = 0; i < m; ++i) {
            if (!node->is_leaf) inorderTraversalPrint(node->children[i]);
            std::cout << node->keys[i] << ' ';
        }
        if (!node->is_leaf) inorderTraversalPrint(node->children[m]);
    }

    // Разделить переполненного ребёнка parent->children[i]
    void splitChild(Node* parent, size_t i) {
        Node* full = parent->children[i];         // full содержит 2*t - 1 ключей
        Node* right = new Node(full->is_leaf);    // правый узел после сплита

        // Сохраняем средний ключ до resize
        int middle_key = full->keys[t - 1];

        // Правый узел получает последние (t-1) ключей
        right->keys.assign(std::begin(full->keys) + t, std::end(full->keys));
        full->keys.resize(t - 1); // левый оставляет первые (t-1) ключей

        // Переносим детей, если это внутренний узел: правый получает последние t детей
        if (!full->is_leaf) {
            right->children.assign(std::begin(full->children) + t, std::end(full->children));
            full->children.resize(t); // левый оставляет первые t детей
        }

        // Поднимаем средний ключ в родителя
        parent->keys.insert(std::begin(parent->keys) + i, middle_key);
        parent->children.insert(std::begin(parent->children) + i + 1, right);
    }

    // Рекурсивная вставка «сначала вниз, потом split на обратном пути».
    void insertRecursive(Node* node, int key) {
        // Если ключ уже есть в текущем узле — игнорируем дубликат.
        auto it = std::lower_bound(std::begin(node->keys), std::end(node->keys), key);
        size_t idx = static_cast<size_t>(it - std::begin(node->keys));
        if (it != std::end(node->keys) && (*it == key)) return;

        if (node->is_leaf) {
            node->keys.insert(it, key);
            return;
        }

        // Спускаемся в подходящего ребёнка
        insertRecursive(node->children[idx], key);

        // Если ребёнок переполнен — сплитим его (после рекурсии)
        if (node->children[idx]->keys.size() > 2 * t - 1) {
            splitChild(node, idx);
        }
    }

public:
    explicit BTree(size_t min_degree) : t(min_degree) {
        root = new Node(true);
    }

    bool search(int key) const {
        const Node* cur = root;
        while (cur) {
            auto it = std::lower_bound(std::begin(cur->keys), std::end(cur->keys), key);
            size_t idx = static_cast<size_t>(it - std::begin(cur->keys));
            if (it != std::end(cur->keys) && (*it == key)) return true;
            if (cur->is_leaf) return false;
            cur = cur->children[idx];
        }
        return false;
    }

    // Вставляем вниз, а потом, если root переполнен, сплитим наверху
    void insert(int key) {
        insertRecursive(root, key);
        if (root->keys.size() > 2 * t - 1) {
            // Создаём новый корень и сплитим старый корень как ребёнка[0]
            Node* new_root = new Node(false);
            new_root->children.push_back(root);
            splitChild(new_root, 0);
            root = new_root;
        }
    }

    void print() const {
        inorderTraversalPrint(root);
        std::cout << '\n';
    }
};

// Пример использования
int main() {
    const std::vector<int> keys = { 100, 4, 243, 2, 15, 7, 3, 78, 8, 9, 10 };

    BTree tree(3); // t = 3, максимум ключей в узле = 2*t - 1 = 5
    for (int k : keys) tree.insert(k);

    tree.print(); // проверка порядка

    const std::vector<int> q = { 8, 2, 10, 99, 1, 12 };
    for (int x : q) {
        std::cout << "search(" << x << ") = " << (tree.search(x) ? "true" : "false") << '\n';
    }
    return 0;
}
