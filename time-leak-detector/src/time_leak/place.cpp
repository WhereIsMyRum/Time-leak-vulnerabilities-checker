#include <string>
#include <iostream>

#include "../../include/time_leak/place.h"

using namespace std;

time_leak::Place::Place(string id)
    : Element(id)
{
}