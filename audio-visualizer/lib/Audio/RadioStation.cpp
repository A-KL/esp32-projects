#include "RadioStation.h"

std::ostream& operator<<(std::ostream& os, const RadioStation& station)
{
    os << station.Name;
    return os;
}