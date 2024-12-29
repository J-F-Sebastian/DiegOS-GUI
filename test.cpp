/*
 * DiegOS Operating System source code
 *
 * Copyright (C) 2012 - 2024 Diego Gallizioli
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include "list.h"

class base
{
public:
    base() { std::cout << "base" << std::endl; }
    virtual ~base() {};

    virtual void aaa(void) { std::cout << "aaa" << std::endl; }

    void bbb(void) { std::cout << "bbb" << std::endl; }
};

class ext : public base
{
public:
    ext() { std::cout << "ext " << std::endl; }
    virtual void aaa(void)
    {
        std::cout << "aaa+aaa ";
        base::aaa();
    }

    void bbb(void) { std::cout << "bbb+bbb" << std::endl; }
};

int main()
{
    base *uno = new base();
    base *due = new ext();

    uno->aaa();
    uno->bbb();
    due->aaa();
    due->bbb();
    // dynamic_cast<ext *>(due)->bbb();

    List<int> lista;
    lista.addHead(1);
    lista.addHead(2);
    lista.addHead(3);
    lista.addHead(4);
    lista.addHead(5);

    List<int>::iterator it = lista.begin();

    while (it != lista.end())
    {
        std::cout << "Numero " << *it++ << std::endl;
    }

    std::cout << "==========" << std::endl;

    List<int>::riterator rit = lista.rbegin();

    while (rit != lista.rend())
    {
        std::cout << "Numero " << *rit++ << std::endl;
    }

    it = lista.begin();
    *it++ = -1;
    *it++ = -2;
    *it-- = -3;

    std::cout << "Numero Check " << *it << std::endl;

    it = lista.begin();

    while (it != lista.end())
    {
        std::cout << "Numero " << *it++ << std::endl;
    }

    return 0;
}