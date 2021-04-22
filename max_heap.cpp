#include <iostream>
#include <iomanip>
#include <array>


template <typename T, size_t SIZE>
class MaxHeap final
{
public:

    MaxHeap()
        : _current_size(0)
    {}

    void insert(T&& data);

    void remove(int index);

    void print() const;

private:

    int parent(int index) const;

    int left(int index) const;
    int right(int index) const;

    void heapify();

    void inner_print(int index) const;

private:
    
    size_t _current_size;

    std::array<T, SIZE> _heap;
};

template <typename T, size_t SIZE>
void MaxHeap<T, SIZE>::print() const 
{
    inner_print(0);

    std::cout << std::endl;
}

template <typename T, size_t SIZE>
void MaxHeap<T, SIZE>::inner_print(int index) const
{
    if(index >= _current_size)
        return;

    inner_print(left(index));

    std::cout << std::setw(3) << _heap[index];

    inner_print(right(index));
}

template <typename T, size_t SIZE>
void MaxHeap<T, SIZE>::heapify()
{
    const auto i_end = parent(_current_size);

    for(int i = 0; i < i_end; ++i)
    {
        const auto left_index = left(i);
        const auto right_index = right(i);

        if(_heap[i] < _heap[left_index])
            std::swap(_heap[i], _heap[left_index]);

        if(_heap[i] < _heap[right_index])
            std::swap(_heap[i], _heap[right_index]);
    }
}

template <typename T, size_t SIZE>
void MaxHeap<T, SIZE>::insert(T&& data)
{
    if(_current_size == SIZE)
        throw 42;

    _heap[_current_size++] = data;

    heapify();
}

template <typename T, size_t SIZE>
void MaxHeap<T, SIZE>::remove(int index)
{
    if(index > _current_size || _current_size == 0)
        throw 42;

    std::swap(_heap[_current_size - 1], _heap[index]);

    --_current_size;

    heapify();
}

template <typename T, size_t SIZE>
int MaxHeap<T, SIZE>::parent(int index) const
{
    return (index - 2 + (index & 1))/2;
}

template <typename T, size_t SIZE>
int MaxHeap<T, SIZE>::left(int index) const
{
    return 2*index + 1;
}

template <typename T, size_t SIZE>
int MaxHeap<T, SIZE>::right(int index) const
{
    return 2*index + 2;
}


int main()
{
    MaxHeap<int, 7> myheap;

    myheap.insert(10);
    myheap.insert(8);
    myheap.insert(7);
    myheap.insert(5);
    myheap.insert(6);
    myheap.insert(4);
    myheap.insert(5);

    myheap.print();

    myheap.remove(1);

    myheap.print();

    myheap.insert(9);

    myheap.print();
}
