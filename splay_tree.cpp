#include <iostream>
#include <iomanip>

#include <queue>


template <typename T>
class SplayTree final
{
public:

    SplayTree()
        : _root(nullptr)
    {}

    ~SplayTree()
    {
        destroy_tree(_root);
    }

    SplayTree(const SplayTree&) = delete;
    SplayTree& operator=(const SplayTree&) = delete;

    SplayTree(SplayTree&&) = default;
    SplayTree& operator=(SplayTree&&) = default;

public:

    bool isEmpty() const;

    void insert(T&& data);

    T max() const;
    T min() const;

    void print() const;

    void print_layers() const;

    void merge(SplayTree<T> &tree);

    SplayTree<T> split(T&& data);

private:

    struct Node final
    {
        explicit Node(T&& d, Node* p = nullptr)
            : data(std::move(d)),
              parent(p),
              left(nullptr),
              right(nullptr)
        {}

        T data;
        Node* parent;

        Node* left;
        Node* right;
    };

private:

    void inorder_traversal(Node* node) const;
    void preorder_traversal(Node* node) const;
    void postorder_traversal(Node* node) const;

    void destroy_tree(Node* &root);
    void inner_destroy_tree(Node* node);

    Node* inner_insert(Node* &node, T&& data, Node* parent);

    Node* inner_max(Node* node) const;
    Node* inner_min(Node* node) const;

    Node* lower_bound(T&& data) const;
    Node* upper_bound(T&& data) const;

private:
    Node* inner_lower_bound(Node* node, T&& data) const;
    Node* inner_upper_bound(Node* node, T&& data) const;

private:

    void splay(Node* node);

    void zig_rotation(Node* node);
    void zag_rotation(Node* node);

    void zig_zig_rotation(Node* node);
    void zag_zag_rotation(Node* node);

    void zig_zag_rotation(Node* node);
    void zag_zig_rotation(Node* node);

private:
    
    Node* _root;
};

template <typename T>
void SplayTree<T>::splay(typename SplayTree<T>::Node* node)
{
    while(node != _root)
    {
        if(node->parent->parent == nullptr)
        {
            if(node->parent->data > node->data)
            {
                zig_rotation(node);
            }
            else
            {
                zag_rotation(node);
            }
        }
        else
        {
            if(node->parent->parent->data > node->parent->data)
            {
                if(node->parent->data > node->data)
                {
                    zig_zig_rotation(node);
                }
                else
                {
                    zig_zag_rotation(node);
                }
            }
            else
            {
                if(node->parent->data < node->data)
                {
                    zag_zag_rotation(node);
                }
                else
                {
                    zag_zig_rotation(node);
                }
            }
        }
    }
}

template <typename T>
void SplayTree<T>::zig_rotation(typename SplayTree<T>::Node* node)
{
    auto p = node->parent;
    auto v = node;
    auto tmp = v->right;

    v->right = p;
    v->parent = p->parent;

    p->parent = v;
    p->left = tmp;
    
    if(tmp != nullptr)
        tmp->parent = p;

    if(p == _root)
        _root = v;
}

template <typename T>
void SplayTree<T>::zag_rotation(typename SplayTree<T>::Node* node)
{
    auto p = node->parent;
    auto v = node;
    auto tmp = v->left;

    v->left = p;
    v->parent = p->parent;

    p->parent = v;
    p->right = tmp;

    if(tmp != nullptr)
        tmp->parent = p;

    if(p == _root)
        _root = v;
}

template <typename T>
void SplayTree<T>::zig_zig_rotation(typename SplayTree<T>::Node* node)
{
    zig_rotation(node->parent);
    zig_rotation(node);
}

template <typename T>
void SplayTree<T>::zag_zag_rotation(typename SplayTree<T>::Node* node)
{
    zag_rotation(node->parent);
    zag_rotation(node);
}

template <typename T>
void SplayTree<T>::zig_zag_rotation(typename SplayTree<T>::Node* node)
{
    zag_rotation(node);
    zig_rotation(node);
}

template <typename T>
void SplayTree<T>::zag_zig_rotation(typename SplayTree<T>::Node* node)
{
    zig_rotation(node);
    zag_rotation(node);
}

template <typename T>
void SplayTree<T>::inorder_traversal(typename SplayTree<T>::Node* node) const
{
    if(node != nullptr)
    {
        inorder_traversal(node->left);

        std::cout << std::setw(3) << node->data;

        inorder_traversal(node->right);
    }
}

template <typename T>
void SplayTree<T>::preorder_traversal(typename SplayTree<T>::Node* node) const
{
    if(node != nullptr)
    {
        std::cout << std::setw(3) << node->data;

        preorder_traversal(node->left);
        preorder_traversal(node->right);
    }
}

template <typename T>
void SplayTree<T>::postorder_traversal(typename SplayTree<T>::Node* node) const
{
    if(node != nullptr)
    {
        postorder_traversal(node->left);
        postorder_traversal(node->right);

        std::cout << std::setw(3) << node->data;
    }
}

template <typename T>
bool SplayTree<T>::isEmpty() const
{
    return _root == nullptr;
}

template <typename T>
typename SplayTree<T>::Node* SplayTree<T>::inner_min(typename SplayTree<T>::Node* node) const
{
    if(node->left == nullptr)
        return node;

    return inner_min(node->left);
}

template <typename T>
T SplayTree<T>::min() const
{
    if(isEmpty())
        throw 42;

    const auto tmp = inner_min(_root);
    
    return tmp->data;
}

template <typename T>
typename SplayTree<T>::Node* SplayTree<T>::inner_max(typename SplayTree<T>::Node* node) const
{
    if(node->right == nullptr)
        return node;

    return inner_max(node->right);
}

template <typename T>
T SplayTree<T>::max() const
{
    if(isEmpty())
        throw 42;

    const auto tmp = inner_max(_root);

    return tmp->data;
}

template <typename T>
void SplayTree<T>::merge(SplayTree<T> &tree)
{
    auto max = inner_max(_root);
    splay(max);

    max->right = tree._root;
    tree._root = nullptr;
}

template <typename T>
void SplayTree<T>::print() const
{
    inorder_traversal(_root);
    //preorder_traversal(_root);
    //postorder_traversal(_root);

    std::cout << std::endl;
}

template <typename T>
void SplayTree<T>::print_layers() const
{
    if(isEmpty())
        return;

    std::queue<Node*> first_layer;
    std::queue<Node*> second_layer;

    first_layer.push(_root); 

    while(!second_layer.empty() || !first_layer.empty())
    {
        while(!first_layer.empty())
        {
            const auto tmp = first_layer.front();
            first_layer.pop();

            std::cout << std::setw(3) << tmp->data;

            if(tmp->left != nullptr)
                second_layer.push(tmp->left);

            if(tmp->right != nullptr)
                second_layer.push(tmp->right);
        }

        std::cout << std::endl;
        while(!second_layer.empty())
        {
            const auto tmp = second_layer.front();
            second_layer.pop();

            std::cout << std::setw(3) << tmp->data;

            if(tmp->left != nullptr)
                first_layer.push(tmp->left);

            if(tmp->right != nullptr)
                first_layer.push(tmp->right);
        }

        std::cout << std::endl;
    }
}

template <typename T>
void SplayTree<T>::insert(T&& data)
{
    auto new_node = inner_insert(_root, std::forward<T>(data), nullptr);

    splay(new_node);
}

template <typename T>
typename SplayTree<T>::Node* SplayTree<T>::inner_insert(typename SplayTree<T>::Node* &node, T&& data, typename SplayTree<T>::Node* parent)
{
    if(node == nullptr)
    {
        node = new Node(std::forward<T>(data), parent);
        return node;
    }

    if(node->data > data)
    {
        return inner_insert(node->left, std::forward<T>(data), node);
    }
    else if(node->data < data)
    {
        return inner_insert(node->right, std::forward<T>(data), node);
    }

    return node;
}

template <typename T>
void SplayTree<T>::destroy_tree(typename SplayTree<T>::Node* &root)
{
    inner_destroy_tree(root);

    root = nullptr;
}

template <typename T>
void SplayTree<T>::inner_destroy_tree(typename SplayTree<T>::Node* node)
{
    if(node != nullptr)
    {
        inner_destroy_tree(node->left);
        inner_destroy_tree(node->right);

        delete node;
    }    
}

template <typename T>
typename SplayTree<T>::Node* SplayTree<T>::lower_bound(T&& data) const
{
    if(isEmpty())
        return nullptr;

    return inner_lower_bound(_root, std::forward<T>(data));
}

template <typename T>
typename SplayTree<T>::Node* SplayTree<T>::inner_lower_bound(typename SplayTree<T>::Node* node, T&& data) const
{
    // TODO: Not implemented yet
    return nullptr;
}

template <typename T>
typename SplayTree<T>::Node* SplayTree<T>::upper_bound(T&& data) const
{
    if(isEmpty())
        return nullptr;

    return inner_upper_bound(_root, std::forward<T>(data));
}

template <typename T>
typename SplayTree<T>::Node* SplayTree<T>::inner_upper_bound(typename SplayTree<T>::Node* node, T&& data) const
{
    // TODO: Not implemented yet
    return nullptr;
}

int main()
{
    SplayTree<int> bigTree;
    bigTree.insert(42);
    bigTree.insert(37);
    bigTree.insert(40);

    SplayTree<int> smallTree;
    smallTree.insert(15);
    smallTree.insert(25);
    smallTree.insert(33);
    smallTree.insert(10);

    bigTree.print();
    smallTree.print();

    smallTree.merge(bigTree);

    bigTree.print();
    smallTree.print();
}
