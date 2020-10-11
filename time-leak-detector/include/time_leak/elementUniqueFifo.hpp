#ifndef ELEMENT_UNIQUE_FIFO
#define ELEMENT_UNIQUE_FIFO

#include <queue>
#include <set>

namespace time_leak
{
    template <class T>
    class ElementUniqueFifo
    {
    private:
        std::queue<T> Queue;
        std::set<T> Set;

    public:
        void Push(T t);
        T Pop();
        int Size();
        T Front();
        void Shift();
        void Clear();
    };

    template <class T, class T2>
    void ForwardQueue(T &queue1, T2 &queue2, bool direction);

} // namespace time_leak

#include "../../src/time_leak/elementUniqueFifo.tpp"

#endif