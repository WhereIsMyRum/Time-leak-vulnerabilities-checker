#include <string>
#include <map>
#include <iterator>
#include <iostream>

using namespace std;

template <class T>
time_leak::Element<T>::Element(std::string id)
{
    this->id = id;
    this->analyzed = false;
}

template <class T>
const string time_leak::Element<T>::GetId()
{
    return this->id;
}

template <class T>
const bool time_leak::Element<T>::WasAnalyzed()
{
    return this->analyzed;
}

template <class T>
map<string, T *> time_leak::Element<T>::GetInElements()
{
    return this->inElements;
}

template <class T>
map<string, T *> time_leak::Element<T>::GetOutElements()
{
    return this->outElements;
}

template <class T>
const map<string, T *> time_leak::Element<T>::GetElementsBasedOnDirection(bool direction)
{
    return direction ? this->inElements : this->outElements;
}

template <class T>
void time_leak::Element<T>::AddInElement(T *element)
{
    this->inElements.insert(pair<std::string, T *>(element->GetId(), element));
}

template <class T>
void time_leak::Element<T>::AddOutElement(T *element)
{
    this->outElements.insert(pair<std::string, T *>(element->GetId(), element));
}

template <class T>
void time_leak::Element<T>::SetAnalyzed(bool analyzed)
{
    this->analyzed = analyzed;
}

template <class T>
void time_leak::Element<T>::Print()
{
    typename map<std::string, T *>::iterator iterator;
    if (this->inElements.size() > 0)
    {
        for (iterator = this->inElements.begin(); iterator != this->inElements.end(); ++iterator)
        {
            std::cout << this->id << " <- ";
            iterator->second->Print();
        }
    }
    else
    {
        std::cout << this->id << endl;
    }
}

template <class T>
bool time_leak::Element<T>::AllDirectionsAnalyzed(bool direction)
{
    bool allAnalyzed = true;
    std::map<std::string, T *> elements;
    typename std::map<std::string, T *>::iterator iterator;

    elements = this->GetElementsBasedOnDirection(!direction);

    if (elements.size() > 0)
    {
        for (iterator = elements.begin(); iterator != elements.end(); ++iterator)
        {
            if (!iterator->second->WasAnalyzed())
            {
                allAnalyzed = false;
                break;
            }
        }
    }
    return allAnalyzed;
}

template <class T>
void time_leak::Element<T>::Traverse(std::map<std::string, T *> elements, time_leak::ElementUniqueFifo<T *> &queue)
{
    typename std::map<std::string, T *>::iterator iterator;

    for (iterator = elements.begin(); iterator != elements.end(); ++iterator)
    {
        queue.Push(iterator->second);
    }
}
