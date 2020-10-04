#include <string>

using namespace std;

template <class T>
time_leak::Element<T>::Element(std::string id)
{
    this->id = id;
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


