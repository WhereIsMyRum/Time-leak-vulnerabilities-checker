#include <string>
#include <map>
#include <iterator>

using namespace std;

template <class T>
time_leak::Element<T>::Element(std::string id)
{
    this->id = id;
    this->analyzed = false;
}

template <class T>
string time_leak::Element<T>::GetId()
{
    return this->id;
}

template <class T>
void time_leak::Element<T>::AddInElement(T *element)
{
    this->inElements.insert(pair<std::string, T*>(element->GetId(), element));
}

template <class T>
void time_leak::Element<T>::AddOutElement(T *element)
{
    this->outElements.insert(pair<std::string, T*>(element->GetId(), element));
}

template <class T>
bool time_leak::Element<T>::WasAnalyzed()
{
    return this->analyzed;
}

template <class T>
void time_leak::Element<T>::SetAnalyzed(bool analyzed)
{
    this->analyzed = analyzed;
}

template <class T>
int time_leak::Element<T>::GetInElementsSize()
{
    return this->inElements.size();
}


template <class T>
void time_leak::Element<T>::Print()
{
    typename map<std::string, T*>::iterator iterator;
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


