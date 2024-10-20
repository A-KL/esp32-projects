#include <iostream>

struct RadioStation 
{
    const char* Name;
    const char* Url;

    friend std::ostream& operator<<(std::ostream& os, const RadioStation& station);
};