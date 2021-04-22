#include <iostream>
#include <iomanip>
#include <vector>

#include "max_heap.hpp"


namespace {

    int getRandomNumber(int min, int max)
    {
        static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0); 

        return static_cast<int>(rand() * fraction * (max - min + 1) + min);
    }
}


template <typename T, size_t SIZE>
class CartesianTree final
{
public:

    explicit CartesianTree(std::vector<T>&& values)
        : _root(nullptr)
    {
        create_tree(std::move(values));
    }

    CartesianTree(const CartesianTree&) = delete;
    CartesianTree& operator=(const CartesianTree&) = delete;

    CartesianTree(CartesianTree&&) = default;
    CartesianTree& operator=(CartesianTree&&) = default;

    ~CartesianTree()
    {
        destroy_tree(_root);
    }

public:

    bool isEmpty() const;

    void insert(T&& data);

    void erase(T&& data);

    void print() const;

    static CartesianTree merge(CartesianTree& smallTree, CartesianTree& largeTree);

    static void split(CartesianTree& tree, T&& key, CartesianTree& small, CartesianTree& large);

private:

    struct Node final
    {
        explicit Node(const T& d = T())
            : data(d),
              priority(getRandomNumber(0, 100)),
              left(nullptr),
              right(nullptr)
        {}

        bool operator<(Node& node)
        {
            return priority < node.priority;
        }

        T data;
        int priority;

        Node* left;
        Node* right;
    };

private:

    CartesianTree(Node* root)
        : _root(root)
    {}

private:

    void create_tree(std::vector<T>&& values);

    void destroy_tree(Node* root);

    void insert_node(Node* &treeNode, const Node& node);

    void inorder_traversal(const Node* node) const;

    static Node* inner_merge(Node* left, Node* right);

    static void inner_split(Node* node, T&& key, Node* &small, Node* &large);

private:

    Node* _root;
};



template <typename T, size_t SIZE>
bool CartesianTree<T, SIZE>::isEmpty() const
{
    return _root == nullptr;
}

template <typename T, size_t SIZE>
void CartesianTree<T, SIZE>::insert(T&& data)
{
    Node* node = nullptr;

    Node* small = nullptr;
    Node* large = nullptr;

    try
    {
        inner_split(_root, std::forward<T>(data), small, large);

        node = new Node(std::forward<T>(data));
        small = inner_merge(small, node);

        _root = inner_merge(small, large);
    }
    catch(...)
    {
        delete node;
    }
}

template <typename T, size_t SIZE>
void CartesianTree<T, SIZE>::erase(T&& data)
{
    Node* equal = nullptr;

    Node* small = nullptr;
    Node* large = nullptr;

    inner_split(_root, std::forward<T>(data), small, large);
    inner_split(large, std::forward<T>(data) + 1, equal, large);

    destroy_tree(equal);

    _root = inner_merge(small, large);
}

template <typename T, size_t SIZE>
CartesianTree<T, SIZE> CartesianTree<T, SIZE>::merge(CartesianTree& smallTree, CartesianTree& largeTree)
{
    auto root = inner_merge(smallTree._root, largeTree._root);

    smallTree._root = nullptr;
    largeTree._root = nullptr;

    return CartesianTree(root);
}

template <typename T, size_t SIZE>
typename CartesianTree<T, SIZE>::Node* CartesianTree<T, SIZE>::inner_merge(Node* left, Node* right)
{
    if(!left || !right)
        return right == nullptr ? left : right;

    if(*left < *right)
    {
        right->left = inner_merge(left, right->left);
        return right;
    }

    left->right = inner_merge(left->right, right);
    return left;
}

template <typename T, size_t SIZE>
void CartesianTree<T, SIZE>::split(CartesianTree& tree, T&& key, CartesianTree& small, CartesianTree& large)
{
    inner_split(tree._root, std::forward<T>(key), small._root, large._root);

    tree._root = nullptr;
}

template <typename T, size_t SIZE>
void CartesianTree<T, SIZE>::inner_split(Node* node, T&& key, Node* &small, Node* &large)
{
    if(node == nullptr)
    {
        small = nullptr;
        large = nullptr;

        return;
    }

    if(node->data < key)
    {
        inner_split(node->right, std::forward<T>(key), node->right, large);

        small = node;
    }
    else
    {
        inner_split(node->left, std::forward<T>(key), small, node->left);

        large = node;
    }
}

template <typename T, size_t SIZE>
void CartesianTree<T, SIZE>::print() const
{
    if(isEmpty())
    {
        std::cout << "empty...\n";
        return;
    }

    inorder_traversal(_root);
}

template <typename T, size_t SIZE>
void CartesianTree<T, SIZE>::inorder_traversal(const typename CartesianTree<T, SIZE>::Node* node) const
{
    if(node == nullptr)
        return;

    inorder_traversal(node->left);

    std::cout << "data: " << node->data << " \tpriority: " << node->priority << std::endl;

    inorder_traversal(node->right);
}

template <typename T, size_t SIZE>
void CartesianTree<T, SIZE>::create_tree(std::vector<T>&& values)
{
    MaxHeap<Node, SIZE> heap;

    for(auto v : values)
        heap.insert(Node(std::move(v)));

    while(!heap.isEmpty())
    {
        insert_node(_root, heap.get_max());

        heap.remove(0);
    }
}

template <typename T, size_t SIZE>
void CartesianTree<T, SIZE>::destroy_tree(typename CartesianTree<T, SIZE>::Node* root)
{
    if(root != nullptr)
    {
        destroy_tree(root->left);
        destroy_tree(root->right);

        delete root;
    }
}

template <typename T, size_t SIZE>
void CartesianTree<T, SIZE>::insert_node(typename CartesianTree<T, SIZE>::Node* &treeNode, const typename CartesianTree<T, SIZE>::Node& node)
{
    if(treeNode == nullptr)
    {
        treeNode = new Node(node.data);
        treeNode->priority = node.priority;
        return;
    } 

    if(treeNode->data > node.data)
    {
        insert_node(treeNode->left, node);
    }
    else if(treeNode->data < node.data)
    {
        insert_node(treeNode->right, node);
    }
}


int main()
{
    srand(4541);

    CartesianTree<size_t, 5> smallTree({ 3, 4, 1, 2, 0 });
    CartesianTree<size_t, 5> largeTree({ 5, 7, 6, 9, 8 });

    std::cout << "Before merge:\n";

    std::cout << "\nsmall tree:\n";
    smallTree.print();

    std::cout << "\nlarge tree:\n";
    largeTree.print();


    auto mergedTree = CartesianTree<size_t, 5>::merge(smallTree, largeTree);


    std::cout << "\nAfter merge:\n";

    std::cout << "\nsmall tree:\n";
    smallTree.print();

    std::cout << "\nlarge tree:\n";
    largeTree.print();

    std::cout << "\nmerged tree:\n";
    mergedTree.print();


    std::cout << "\nAfter split:\n";

    CartesianTree<size_t, 5>::split(mergedTree, 6, smallTree, largeTree);

    std::cout << "\nsmall tree:\n";
    smallTree.print();

    std::cout << "\nlarge tree:\n";
    largeTree.print();

    std::cout << "\nmerged tree:\n";
    mergedTree.print();


    std::cout << "\nAfter insert:\n";
    largeTree.insert(13);
    largeTree.insert(11);
    largeTree.insert(15);

    std::cout << "\nlarge tree:\n";
    largeTree.print();


    std::cout << "\nAfter erase:\n";
    largeTree.erase(9);

    std::cout << "\nlarge tree:\n";
    largeTree.print();
}
