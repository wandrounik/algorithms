#include <iostream>
#include <iomanip>
#include <array>

#include <memory>



template <typename T, size_t SIZE>
class PerTree final
{
public:

    PerTree(const std::array<T, SIZE>& values)
        : _root(nullptr)
    {
        create_tree(values);
    }

public:

    void print() const;

    PerTree set(size_t index, T&& value);

private:

    struct Node final
    {
        explicit Node(T&& d = T())
            : data(std::move(d)),
              left(nullptr),
              right(nullptr)
        {}

        ~Node()
        {
            std::cout << "Deleted node: " << data << std::endl;
        }

        T data;

        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
    };

private:

    PerTree(std::shared_ptr<Node> root)
        : _root(root)
    {}

private:

    void create_tree(const std::array<T, SIZE>& values);
    void inner_create_tree(const std::array<T, SIZE>& values, size_t l, size_t r, std::shared_ptr<Node> &node);

    void inorder_traversal(std::shared_ptr<Node> node) const;

    void inner_set(size_t index, T&& value, size_t l, size_t r, std::shared_ptr<Node> &node, std::shared_ptr<Node> old);

private:

    std::shared_ptr<Node> _root;
};

template <typename T, size_t SIZE>
PerTree<T, SIZE> PerTree<T, SIZE>::set(size_t index, T&& value)
{
    std::shared_ptr<Node> root = nullptr;

    inner_set(index, std::forward<T>(value), 0, SIZE-1, root, _root);

    return PerTree(root);
}

template <typename T, size_t SIZE>
void PerTree<T, SIZE>::inner_set(size_t index, T&& value, size_t l, size_t r, std::shared_ptr<typename PerTree<T, SIZE>::Node> &node, std::shared_ptr<typename PerTree<T, SIZE>::Node> old)
{
    node = std::make_shared<Node>();

    if(l == r)
    {
        node->data = std::move(value);
        return;
    }

    const auto m = (l + r)/2;

    if(index <= m)
    {
        inner_set(index, std::forward<T>(value), l, m, node->left, old->left);
        node->right = old->right;
    }
    else
    {
        inner_set(index, std::forward<T>(value), m+1, r, node->right, old->right);
        node->left = old->left;
    }

    node->data = node->left->data + node->right->data;
}

template <typename T, size_t SIZE>
void PerTree<T,SIZE>::create_tree(const std::array<T, SIZE>& values)
{
    inner_create_tree(values, 0, values.size() - 1, _root);
}

template <typename T, size_t SIZE>
void PerTree<T, SIZE>::inner_create_tree(const std::array<T, SIZE>& values, size_t l, size_t r, std::shared_ptr<typename PerTree<T, SIZE>::Node> &node)
{
    node = std::make_shared<Node>();

    if(l == r)
    {
        node->data = values[l];
        return;
    }

    const auto m = (l+r)/2;

    inner_create_tree(values, l, m, node->left);
    inner_create_tree(values, m+1, r, node->right);

    node->data = node->left->data + node->right->data;
}

template <typename T, size_t SIZE>
void PerTree<T, SIZE>::print() const
{
    inorder_traversal(_root);

    std::cout << std::endl;
}

template <typename T, size_t SIZE>
void PerTree<T, SIZE>::inorder_traversal(std::shared_ptr<typename PerTree<T, SIZE>::Node> node) const
{
    if(node != nullptr)
    {
        inorder_traversal(node->left);

        std::cout << std::setw(3) << node->data;

        inorder_traversal(node->right);
    }
}


int main()
{
    PerTree<size_t, 4> mytree({ 3, 5, 1, 6 });

    mytree.print();

    auto mytree2 = mytree.set(2, 4);

    mytree.print();
    mytree2.print();
}
