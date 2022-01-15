#include <iostream>
#include "list.h"

int main()
{
    List<int> list;

    list.addTail(1);

    List<int>::iterator it = list.begin();

    while (it != list.end())
    {
        std::cout << *it << std::endl;
        ++it;
    }

    std::cout << " *** " << list.count() << std::endl;

    list.clear();

    return 0;
}