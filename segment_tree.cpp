#include <functional>
#include <vector>
#include <iostream>


template <typename T>
class MyArray final
{
public:
    using Operand = std::function<T(T, T)>;
    using RangeOperand = std::function<T(T)>;

    explicit MyArray(std::vector<T> data, Operand operand)
         : _operand(operand),
           _root(create_tree(data, 0, data.size() - 1))
    {
        std::cout << "_root->_data: " << _root->_data << std::endl;
    }

    ~MyArray()
    {
        destroy_tree(_root);
    }

    MyArray(const MyArray&) = delete;
    MyArray& operator=(const MyArray&) = delete;

    MyArray(MyArray&&) = default;
    MyArray& operator=(MyArray&&) = default;

public:
    T query(size_t left, size_t right);

    void update(RangeOperand operand, size_t left, size_t right);

private:
    struct Node
    {
        explicit Node(T d, Node* l = nullptr, Node* r = nullptr) 
            : _data(d),
              _left(l),
              _right(r)
        {}

        T getData() const
        {
            if(_op)
                return _op(_data);

            return _data;
        }

        T _data;
        RangeOperand _op;

        Node* _left;
        Node* _right;
    };

private:
    Node* create_tree(const std::vector<T>& data, size_t left, size_t right);

    void destroy_tree(Node* node);

    T inner_query(Node* node, size_t left_node, size_t right_node, size_t left, size_t right);

    void inner_update(Node* node, size_t left_node, size_t right_node, size_t left, size_t right, const RangeOperand& operand);

private:
    Operand _operand;
    Node* _root;
};

template<typename T>
typename MyArray<T>::Node* MyArray<T>::create_tree(const std::vector<T>& data, size_t left, size_t right)
{
    if(left == right)
        return new Node(data[left]);

    const auto middle = (left + right)/2;

    const auto left_node = create_tree(data, left, middle);
    const auto right_node = create_tree(data, middle+1, right);

    const auto node = new Node(_operand(left_node->_data, right_node->_data), left_node, right_node);
    
    return node;
}

template<typename T>
void MyArray<T>::destroy_tree(typename MyArray<T>::Node* node)
{
    if(node)
    {
        destroy_tree(node->_left);
        destroy_tree(node->_right);
    }

    delete node;
}

template<typename T>
T MyArray<T>::inner_query(typename MyArray<T>::Node* node, size_t left_node, size_t right_node, size_t left, size_t right)
{
   if(left_node >= left && right_node <= right) 
       return node->getData();

   // 0 - for std::plus<>(), 1 - for std::multiplies<>()
   if(right_node < left || left_node > right)
       return 0;

   const auto middle = (left_node + right_node)/2;

   const auto left_node_data = inner_query(node->_left, left_node, middle, left, right);
   const auto right_node_data = inner_query(node->_right, middle+1, right_node, left, right);

   const auto result = _operand(left_node_data, right_node_data);

   if(node->_op)
       return node->_op(result);

   return result;
}

template<typename T>
T MyArray<T>::query(size_t left, size_t right)
{
    return inner_query(_root, 0, 7, left, right);
}

template<typename T>
void MyArray<T>::inner_update(Node* node, size_t left_node, size_t right_node, size_t left, size_t right, const RangeOperand& operand)
{
   if(right_node < left || left_node > right)
       return;

   if(left_node == right_node)
   {
       node->_data = operand(node->_data);
       return;
   }
   
   const auto middle = (left_node + right_node)/2;
   if(node->_op)
   {
       inner_update(node->_left, left_node, middle, left_node, middle, node->_op);
       inner_update(node->_right, middle+1, right_node, middle+1, right_node, node->_op);

       node->_data = _operand(node->_left->getData(), node->_right->getData());
       node->_op = nullptr;
   }

   if(left_node >= left && right_node <= right) 
   {
       node->_op = operand;
       return;
   }

   inner_update(node->_left, left_node, middle, left, right, operand);
   inner_update(node->_right, middle+1, right_node, left, right, operand);

   node->_data = _operand(node->_left->getData(), node->_right->getData());
}

template<typename T>
void MyArray<T>::update(RangeOperand operand, size_t left, size_t right)
{
    inner_update(_root, 0, 7, left, right, operand);
}

int main()
{
    std::function<int(int)> multiplyByTwo = [](int value){ return 2*value; };
    std::function<int(int)> multiplyByThree = [](int value){ return 3*value; };

    MyArray<int> array({0, 1, 2, 3, 4, 5, 6, 7}, std::plus<>());

    std::cout << "r[1,5]: " << array.query(1, 5) << std::endl;
    std::cout << "r[2,6]: " << array.query(2, 6) << std::endl;
    std::cout << "r[1,6]: " << array.query(1, 6) << std::endl;
    std::cout << "r[0,7]: " << array.query(0, 7) << std::endl;
    
    
    array.update(multiplyByTwo, 0, 3);

    std::cout << "\nAfter update: multiplyByTwo[0,3]\n";
    std::cout << "r[0,3]: " << array.query(0, 3) << std::endl;
    std::cout << "r[0,7]: " << array.query(0, 7) << std::endl;
    std::cout << "r[1,6]: " << array.query(1, 6) << std::endl;
    std::cout << "r[0,6]: " << array.query(0, 6) << std::endl;

    array.update(multiplyByThree, 2, 5);

    std::cout << "\nAfter update: multiplyByThree[2,5]\n";
    std::cout << "r[0,3]: " << array.query(0, 3) << std::endl;
    std::cout << "r[2,7]: " << array.query(2, 7) << std::endl;
    std::cout << "r[1,6]: " << array.query(1, 6) << std::endl;

    std::cout << "\n############# RESULT #################\n";
    std::cout << "r[0,0]: " << array.query(0, 0) << std::endl;
    std::cout << "r[1,1]: " << array.query(1, 1) << std::endl;
    std::cout << "r[2,2]: " << array.query(2, 2) << std::endl;
    std::cout << "r[3,3]: " << array.query(3, 3) << std::endl;
    std::cout << "r[4,4]: " << array.query(4, 4) << std::endl;
    std::cout << "r[5,5]: " << array.query(5, 5) << std::endl;
    std::cout << "r[6,6]: " << array.query(6, 6) << std::endl;
    std::cout << "r[7,7]: " << array.query(7, 7) << std::endl;
}
