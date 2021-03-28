#include <iostream>
#include <iomanip>
#include <memory>
#include <list>
#include <array>


constexpr size_t SIZE = 11;

std::array<bool, SIZE> isVisited{ false };

const std::array<std::list<char>, SIZE> graph = { std::list<char>({ 'B', 'D', 'G' }),
                                                  std::list<char>({ 'A', 'C' }),
                                                  std::list<char>({ 'B' }),
                                                  std::list<char>({ 'A', 'E', 'F' }),
                                                  std::list<char>({ 'D' }),
                                                  std::list<char>({ 'D' }),
                                                  std::list<char>({ 'A', 'H', 'J', 'K' }),
                                                  std::list<char>({ 'G', 'I' }),
                                                  std::list<char>({ 'H' }),
                                                  std::list<char>({ 'G' }),
                                                  std::list<char>({ 'G' }) }; 


size_t index(char vertex)
{
    return vertex - 'A';
}

template <typename T>
class MyTree final
{
public:
    explicit MyTree(T&& root)
        : _root(create_subtree(std::forward<T>(root)))
    {}

    void print() const;

private:
    struct Node final
    {
        explicit Node(T&& n)
            : name(std::move(n))
        {}

        T name;
        std::list<std::unique_ptr<Node>> leaves;
    };

private:
    
    std::unique_ptr<Node> create_subtree(T&& name);

    void preorder_traversal(const std::unique_ptr<Node>& node) const;
    void postorder_traversal(const std::unique_ptr<Node>& node) const;

private:
    std::unique_ptr<Node> _root;
};


template <typename T>
std::unique_ptr<typename MyTree<T>::Node> MyTree<T>::create_subtree(T&& name)
{
    if(isVisited[index(name)])
        return nullptr;

    isVisited[index(name)] = true;

    auto root_subtree = std::make_unique<Node>(std::forward<T>(name));

    auto leaves = graph[index(name)];

    for(auto it = std::begin(leaves); it != std::end(leaves); ++it)
    {
        if(!isVisited[index(*it)])
        {
            root_subtree->leaves.push_back(create_subtree(std::forward<T>(*it)));
        }
    }

    return std::move(root_subtree);
}

template <typename T>
void MyTree<T>::preorder_traversal(const std::unique_ptr<typename MyTree<T>::Node>& node) const
{
    std::cout << std::setw(3) << node->name;

    const auto& leaves = node->leaves;

    for(auto it = std::cbegin(leaves); it != std::cend(leaves); ++it)
    {
        preorder_traversal(*it);
    }
}

template <typename T>
void MyTree<T>::postorder_traversal(const std::unique_ptr<typename MyTree<T>::Node>& node) const
{
    const auto& leaves = node->leaves;

    for(auto it = std::cbegin(leaves); it != std::cend(leaves); ++it)
    {
        postorder_traversal(*it);
    }

    std::cout << std::setw(3) << node->name;
}

template <typename T>
void MyTree<T>::print() const
{
    if(_root == nullptr)
        return;

    //preorder_traversal(_root);
    postorder_traversal(_root);

    std::cout << std::endl;
}


int main()
{
    MyTree<char> mytree('A');

    mytree.print();
}
