#include <iostream>
#include <iomanip>


template <typename T>
class AVL_Tree final
{
public:

    explicit AVL_Tree(T&& data)
        : _root(new Node(std::forward<T>(data)))
    {}

    ~AVL_Tree()
    {
        destroy_tree(_root);
    }

    AVL_Tree(const AVL_Tree&) = delete;
    AVL_Tree& operator=(const AVL_Tree&) = delete;

    AVL_Tree(AVL_Tree&&) = default;
    AVL_Tree& operator=(AVL_Tree&&) = default;

    void print() const;

    void insert(T&& data);

    void remove(T&& data);

    bool find(T&& data);

private:

    struct Node final
    {
        explicit Node(T&& d)
            : data(std::move(d)),
              height(1),
              left(nullptr),
              right(nullptr)
        {}

        T data;

        int height;

        Node* left;
        Node* right;
    };

private:

    void destroy_tree(Node* &node);

    void inorder_traversal(Node* node) const;
    void preorder_traversal(Node* node) const;
    void postorder_traversal(Node* node) const;

    void inner_insert(Node* &node, T&& data);
    void inner_remove(Node* &node, T&& data);

    void rotate_right(Node* &node);
    void rotate_left(Node* &node);

    void rotate_left_right(Node* &node);
    void rotate_right_left(Node* &node);

    int balance_factor(Node* node) const;
    void balance_tree(Node* &node, T&& data);

    int calculate_height(Node* node) const;

    Node* find_min(Node* node) const;
    Node* find_max(Node* node) const;

    bool inner_find(Node* node, T&& data);

private:

    Node* _root;
};


template<typename T>
void AVL_Tree<T>::destroy_tree(typename AVL_Tree<T>::Node* &node)
{
    if(node != nullptr)
    {
        destroy_tree(node->left);
        destroy_tree(node->right);

        delete node;
    }
}

template<typename T>
void AVL_Tree<T>::inorder_traversal(typename AVL_Tree<T>::Node* node) const
{
    if(node != nullptr)
    {
        inorder_traversal(node->left);

        std::cout << std::setw(3) << node->data;

        inorder_traversal(node->right);
    }
}

template<typename T>
void AVL_Tree<T>::preorder_traversal(typename AVL_Tree<T>::Node* node) const
{
    if(node != nullptr)
    {
        std::cout << std::setw(3) << node->data;

        preorder_traversal(node->left);
        preorder_traversal(node->right);
    }
}

template<typename T>
void AVL_Tree<T>::postorder_traversal(typename AVL_Tree<T>::Node* node) const
{
    if(node != nullptr)
    {
        postorder_traversal(node->left);
        postorder_traversal(node->right);

        std::cout << std::setw(3) << node->data;
    }
}

template<typename T>
void AVL_Tree<T>::print() const
{
    //inorder_traversal(_root);
    preorder_traversal(_root);
    //postorder_traversal(_root);

    std::cout << std::endl;
}

template<typename T>
void AVL_Tree<T>::insert(T&& data)
{
    inner_insert(_root, std::forward<T>(data));
}

template<typename T>
void AVL_Tree<T>::inner_insert(typename AVL_Tree<T>::Node* &node, T&& data)
{
    if(node == nullptr)
    {
        node = new Node(std::forward<T>(data));
        return;
    }

    if(data < node->data)
    {
        inner_insert(node->left, std::forward<T>(data));
    }
    else
    {
        inner_insert(node->right, std::forward<T>(data));
    }

    node->height = calculate_height(node);

    balance_tree(node, std::forward<T>(data));
}

template<typename T>
void AVL_Tree<T>::remove(T&& data)
{
    inner_remove(_root, std::forward<T>(data));
}

template<typename T>
void AVL_Tree<T>::inner_remove(typename AVL_Tree<T>::Node* &node, T&& data)
{
    if(node == nullptr)
        return;

    if(data < node->data)
    {
        inner_remove(node->left, std::forward<T>(data));
    }
    else if(data > node->data)
    {
        inner_remove(node->right, std::forward<T>(data));
    }
    else
    {
        if(!node->left && !node->right)
        {
            delete node;
            node = nullptr;
            return;
        }

        if(node->left != nullptr)
        {
            auto left_max = find_max(node->left);
            
            std::swap(node->data, left_max->data);

            inner_remove(node->left, std::forward<T>(data));
        }
        else
        {
            auto right_min = find_min(node->right); 

            std::swap(node->data, right_min->data);

            inner_remove(node->right, std::forward<T>(data));
        }
    }

    node->height = calculate_height(node);

    balance_tree(node, std::forward<T>(data));
}

template<typename T>
void AVL_Tree<T>::rotate_right(typename AVL_Tree<T>::Node* &node)
{
    std::cout << "right\n";

    const auto tmp = node->left;

    node->left = tmp->right;
    node->height = calculate_height(node);

    tmp->right = node;
    tmp->height = calculate_height(tmp);

    node = tmp;
}

template<typename T>
void AVL_Tree<T>::rotate_left(typename AVL_Tree<T>::Node* &node)
{
    std::cout << "left\n";

    const auto tmp = node->right;

    node->right = tmp->left;
    node->height = calculate_height(node);

    tmp->left = node;
    tmp->height = calculate_height(tmp);

    node = tmp;
}

template<typename T>
void AVL_Tree<T>::rotate_left_right(typename AVL_Tree<T>::Node* &node)
{
    std::cout << "left-right\n";

    rotate_left(node->left);
    rotate_right(node);
}

template<typename T>
void AVL_Tree<T>::rotate_right_left(typename AVL_Tree<T>::Node* &node)
{
    std::cout << "right-left\n";

    rotate_right(node->right);
    rotate_left(node);
}

template<typename T>
int AVL_Tree<T>::balance_factor(typename AVL_Tree<T>::Node* node) const
{
    if(node->left == nullptr && node->right == nullptr)
        return 0;

    if(node->left == nullptr)
        return node->right->height*(-1);

    if(node->right == nullptr)
        return node->left->height;

    return node->left->height - node->right->height;
}

template<typename T>
void AVL_Tree<T>::balance_tree(typename AVL_Tree<T>::Node* &node, T&& data)
{
    const auto balance = balance_factor(node);

    if( -1 <= balance && balance <= 1)
        return;

    if(node->left &&  data < node->left->data && balance > 1)
    {
        rotate_right(node);
    }
    else if(node->right && data > node->right->data && balance < -1)
    {
        rotate_left(node);
    }
    else if(node->left && data > node->left->data && balance > 1)
    {
        rotate_left_right(node);
    }
    else if(node->right && data < node->right->data && balance < -1)
    {
        rotate_right_left(node);
    }

    node->height = calculate_height(node);
}

template<typename T>
int AVL_Tree<T>::calculate_height(typename AVL_Tree<T>::Node* node) const
{
    if(node->left == nullptr && node->right == nullptr)
        return 1;

    if(node->left == nullptr)
        return node->right->height + 1;

    if(node->right == nullptr)
        return node->left->height + 1;

   return std::max(node->left->height, node->right->height) + 1;
}

template<typename T>
typename AVL_Tree<T>::Node* AVL_Tree<T>::find_min(typename AVL_Tree<T>::Node* node) const
{
    if(!node->left)
        return node;

    return find_min(node->left);
}

template<typename T>
typename AVL_Tree<T>::Node* AVL_Tree<T>::find_max(typename AVL_Tree<T>::Node* node) const
{
    if(!node->right)
        return node;

    return find_max(node->right);
}

template<typename T>
bool AVL_Tree<T>::inner_find(AVL_Tree<T>::Node* node, T&& data)
{
    if(node == nullptr)
        return false;

    if(data < node->data)
        return inner_find(node->left, std::forward<T>(data));

    if(data > node->data)
        return inner_find(node->right, std::forward<T>(data));

    return true;
}

template<typename T>
bool AVL_Tree<T>::find(T&& data)
{
    return inner_find(_root, std::forward<T>(data));
}


int main()
{
    AVL_Tree<int> mytree(42);

    mytree.insert(56);
    mytree.insert(37);
    mytree.insert(65);
    mytree.insert(39);
    mytree.insert(15);
    mytree.insert(10);
    mytree.insert(8);
    mytree.insert(83);

    mytree.print();

    mytree.remove(42);

    mytree.print();

    std::cout << "5 is inside: " << std::boolalpha << mytree.find(5) << std::endl;

    mytree.insert(5);

    mytree.print();

    std::cout << "5 is inside: " << std::boolalpha << mytree.find(5) << std::endl;
}
