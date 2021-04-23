#include <iostream>


template <typename T>
class RedBlackTree final
{
public:

    RedBlackTree()
        : _root(nullptr)
    {}

    RedBlackTree(const RedBlackTree&) = delete;
    RedBlackTree& operator=(const RedBlackTree&) = delete;

    RedBlackTree(RedBlackTree&&) = default;
    RedBlackTree& operator=(RedBlackTree&&) = default;

    ~RedBlackTree()
    {
        destroy_tree(_root);
    }

public:

    bool isEmpty() const;

    void insert(T&& data);

    void print() const;

private:

    struct Node final
    {
        enum class Color
        {
            RED = 1,
            BLACK = 0
        };

        explicit Node(T&& d, Node* p = nullptr, Node::Color c = Color::RED)
            : data(std::move(d)),
              color(c),
              parent(p),
              left(nullptr),
              right(nullptr)
        {}

        T data;
        Color color;

        Node* parent;

        Node* left;
        Node* right;
    };

private:
    
    void destroy_tree(Node* node);

    void validate_tree(Node* newNode);

    Node* inner_insert(Node* &node, T&& data, Node* parent);

    void inorder_traversal(Node* node) const;
    void preorder_traversal(Node* node) const;

    Node* sibling(Node* node) const;

private:

    void rotate(Node* node);

    void rotate_right(Node* node, bool isNeedRecolor = false);
    void rotate_left(Node* node, bool isNeedRecolor = false);

    void rotate_left_right(Node* node);
    void rotate_right_left(Node* node);

private:

    Node* _root;
};


template <typename T>
void RedBlackTree<T>::destroy_tree(typename RedBlackTree<T>::Node* node)
{
    if(node != nullptr)
    {
        destroy_tree(node->left);
        destroy_tree(node->right);

        delete node;
    }
}

template <typename T>
void RedBlackTree<T>::rotate(typename RedBlackTree<T>::Node* node)
{
    const auto parent = node->parent;
    const auto parent_parent = parent->parent;

    if(node->data < parent->data)
    {
        if(parent->data < parent_parent->data)
            rotate_right(parent, true);
        else
            rotate_right_left(node);
    }
    else
    {
        if(parent->data > parent_parent->data)
            rotate_left(parent, true);
        else
            rotate_left_right(node);
    }
}

template <typename T>
void RedBlackTree<T>::rotate_right(typename RedBlackTree<T>::Node* node, bool isNeedRecolor /*= false*/)
{
    const auto parent = node->parent;
    const auto parent_parent = parent->parent;

    if(isNeedRecolor)
    {
        node->color = Node::Color::BLACK;
        parent->color = Node::Color::RED;
    }

    const auto tmp = node->right;

    parent->left = tmp;
    if(tmp != nullptr)
        tmp->parent = parent;

    node->right = parent;
    parent->parent = node;

    if(parent_parent == nullptr)
    {
        node->parent = nullptr;
        _root = node;
    }
    else
    {
        if(parent_parent->data > parent->data)
        {
            parent_parent->left = node;
        }
        else
        {
            parent_parent->right = node;
        }

        node->parent = parent_parent;
    }
}

template <typename T>
void RedBlackTree<T>::rotate_left(typename RedBlackTree<T>::Node* node, bool isNeedRecolor /*= false*/)
{
    const auto parent = node->parent;
    const auto parent_parent = parent->parent;

    if(isNeedRecolor)
    {
        node->color = Node::Color::BLACK;
        parent->color = Node::Color::RED;
    }

    const auto tmp = node->left;

    parent->right = tmp;
    if(tmp != nullptr)
        tmp->parent = parent;

    node->left = parent;
    parent->parent = node;

    if(parent_parent == nullptr)
    {
        node->parent = nullptr;
        _root = node;
    }
    else
    {
        if(parent_parent->data > parent->data)
        {
            parent_parent->left = node;
        }
        else
        {
            parent_parent->right = node;
        }

        node->parent = parent_parent;
    }
}

template <typename T>
void RedBlackTree<T>::rotate_left_right(typename RedBlackTree<T>::Node* node)
{
    rotate_left(node, false);
    rotate_right(node, true);
}

template <typename T>
void RedBlackTree<T>::rotate_right_left(typename RedBlackTree<T>::Node* node)
{
    rotate_right(node, false);
    rotate_left(node, true);
}

template <typename T>
typename RedBlackTree<T>::Node* RedBlackTree<T>::sibling(typename RedBlackTree<T>::Node* node) const
{
    if(isEmpty() || node == _root)
        throw 42;

    const auto parent = node->parent;

    if(parent->data > node->data)
        return parent->right;

    return parent->left;
}

template <typename T>
bool RedBlackTree<T>::isEmpty() const
{
    return _root == nullptr;
}

template <typename T>
void RedBlackTree<T>::print() const
{
    //inorder_traversal(_root);
    preorder_traversal(_root);

    std::cout << std::endl;
}

template <typename T>
void RedBlackTree<T>::inorder_traversal(typename RedBlackTree<T>::Node* node) const
{
    if(node != nullptr)
    {
        inorder_traversal(node->left);

        if(node->color == Node::Color::BLACK)
            std::cout << 'B';
        else
            std::cout << 'R';

        std::cout << node->data << ' ';

        inorder_traversal(node->right);
    }
}

template <typename T>
void RedBlackTree<T>::preorder_traversal(typename RedBlackTree<T>::Node* node) const
{
    if(node != nullptr)
    {
        if(node->color == Node::Color::BLACK)
            std::cout << 'B';
        else
            std::cout << 'R';

        std::cout << node->data << ' ';

        preorder_traversal(node->left);
        preorder_traversal(node->right);
    }
}

template <typename T>
void RedBlackTree<T>::validate_tree(typename RedBlackTree<T>::Node* newNode)
{
    if(_root == newNode)
    {
        _root->color = Node::Color::BLACK;
        return;
    }

    if(newNode->parent->color == Node::Color::BLACK)
        return;

    const auto parent = newNode->parent;
    const auto parentSibling = sibling(parent);

    if(!parentSibling || parentSibling->color == Node::Color::BLACK)
    {
        rotate(newNode);
    }
    else
    {
        parentSibling->color = Node::Color::BLACK;
        parent->color = Node::Color::BLACK;

        const auto parent_parent = parent->parent;

        if(parent_parent != _root)
        {
            parent_parent->color = Node::Color::RED;

            validate_tree(parent_parent);
        }
    }
}

template <typename T>
void RedBlackTree<T>::insert(T&& data)
{
    const auto node = inner_insert(_root, std::forward<T>(data), nullptr);

    validate_tree(node);
}

template <typename T>
typename RedBlackTree<T>::Node* RedBlackTree<T>::inner_insert(typename RedBlackTree<T>::Node* &node, T&& data, typename RedBlackTree<T>::Node* parent)
{
    if(node == nullptr)
        return node = new Node(std::forward<T>(data), parent);

    if(node->data > data)
        return inner_insert(node->left, std::forward<T>(data), node);

    return inner_insert(node->right, std::forward<T>(data), node);
}


int main()
{
    RedBlackTree<size_t> mytree;

    mytree.insert(10);
    mytree.print();

    mytree.insert(18);
    mytree.print();

    mytree.insert(7);
    mytree.print();

    mytree.insert(15);
    mytree.print();

    mytree.insert(16);
    mytree.print();

    mytree.insert(30);
    mytree.print();

    mytree.insert(25);
    mytree.print();

    mytree.insert(40);
    mytree.print();

    mytree.insert(60);
    mytree.print();

    mytree.insert(2);
    mytree.print();

    mytree.insert(1);
    mytree.print();

    mytree.insert(70);
    mytree.print();
}
