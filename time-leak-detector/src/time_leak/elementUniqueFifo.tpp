
#include <queue>
#include <set>

using namespace std;

template <class T>
void time_leak::ElementUniqueFifo<T>::Push(T t)
{
    if (this->Set.find(t) == this->Set.end())
    {
        this->Queue.push(t);
        this->Set.insert(t);
    }
}

template <class T>
T time_leak::ElementUniqueFifo<T>::Pop()
{
    T t = this->Queue.front();
    this->Queue.pop();
    return t;
}

template <class T>
int time_leak::ElementUniqueFifo<T>::Size()
{
    return this->Queue.size();
}
template <class T>
T time_leak::ElementUniqueFifo<T>::Front()
{
    return this->Queue.front();
}

template <class T>
void time_leak::ElementUniqueFifo<T>::Shift()
{
    T t = this->Pop();
    this->Queue.push(t);
}

template <class T>
void time_leak::ElementUniqueFifo<T>::Clear()
{
    this->Queue.empty();
    this->Set.clear();
}

template <class T, class T2>
bool time_leak::ForwardQueue(T &queue1, T2 &queue2, bool direction)
{
    bool changesMade = false;
    if (queue1.Size() > 0)
    {
        if (queue1.Front()->AllDirectionsAnalyzed(direction))
        {
            auto element = queue1.Pop();
            changesMade = element->Analyze();
            element->Traverse(element->GetElementsBasedOnDirection(direction), queue2);
        }
        else
        {
            auto element = queue1.Front();
            auto elements = element->GetElementsBasedOnDirection(!direction);
            for (auto iterator = elements.begin(); iterator != elements.end(); ++iterator) 
            {
                if (!iterator->second->WasAnalyzed())
                {
                    iterator->second->Analyze();
                    break;
                }
            }
            queue1.Shift();
        }
    }

    return changesMade;
}