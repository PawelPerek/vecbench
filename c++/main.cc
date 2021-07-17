#include <functional>
#include <chrono>
#include <iostream>
#include <memory>
#include <vector>

template <class T>
struct Node
{
private:
    T data;
    std::vector<std::unique_ptr<Node<T>>> children;

public:
    Node();
    Node(T initial);
    void append_child(std::unique_ptr<Node<T>> child);
    void traverse(std::function<T(T)> modifier);
};

template <class T>
Node<T>::Node()
{
    data = 0;
    children = std::vector<std::unique_ptr<Node<T>>>();
}

template <class T>
Node<T>::Node(T initial)
{
    data = initial;
    children = std::vector<std::unique_ptr<Node<T>>>();
}

template <class T>
void Node<T>::traverse(std::function<T(T)> modifier)
{
    data = modifier(data);
    for(const auto& child: children) {
        child -> traverse(modifier);
    }
}

template <class T>
void Node<T>::append_child(std::unique_ptr<Node<T>> child)
{
    children.push_back(std::move(child));
}
const int WIDTH = 100;

auto main() -> int
{
    auto alloc_start = std::chrono::steady_clock::now();
    
    auto root = std::make_unique<Node<uint8_t>>();

    for (int x0 = 0; x0 < WIDTH; x0++)
    {
        auto l0 = std::make_unique<Node<uint8_t>>(x0);
        for (int x1 = 0; x1 < WIDTH; x1++)
        {
            auto l1 = std::make_unique<Node<uint8_t>>(x0 + x1);
             for (int x2 = 0; x2 < WIDTH; x2++)
             {
                 auto l2 = std::make_unique<Node<uint8_t>>(x0 + x1 + x2);
                 l1 -> append_child(std::move(l2));
             }
            l0 -> append_child(std::move(l1));
        }
        root -> append_child(std::move(l0));
    }

    auto alloc_end = std::chrono::steady_clock::now();
    std::cout << std::format("allocated in: {}\n", std::chrono::duration_cast<std::chrono::milliseconds>(alloc_end-alloc_start));

    for(int i = 0; i < 1000; i++) {
        auto iter_start = std::chrono::steady_clock::now();
        root -> traverse([](uint8_t arg) {
            return arg+1;
        });
        auto iter_end = std::chrono::steady_clock::now();

        std::cout << std::format("iterated in: {}\n", std::chrono::duration_cast<std::chrono::milliseconds>(iter_end-iter_start));
    }

    root -> traverse([](uint8_t arg) {
        std::cout << std::format("{}\n", arg);
        return arg;
    });

    return 0;
}
