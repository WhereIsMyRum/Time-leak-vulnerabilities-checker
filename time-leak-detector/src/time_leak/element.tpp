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
