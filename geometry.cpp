/*
 * DiegOS Operating System source code
 *
 * Copyright (C) 2012 - 2019 Diego Gallizioli
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

#include "geometry.h"

Point::Point() : x(0), y(0)
{
}

Point::Point(int x, int y) : x(x), y(y)
{
}

Point::Point(const Point &other) : x(other.x), y(other.y)
{
}

void Point::set(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Point::move(int deltax, int deltay)
{
	x += deltax;
	y += deltay;
}

void Point::add(Point &other)
{
	x += other.x;
	y += other.y;
}

void Point::sub(Point &other)
{
	x -= other.x;
	y -= other.y;
}

void Point::print()
{
	std::cout << "POINT " << std::hex << (intptr_t)this << " (" << std::dec << x << "," << y << ")" << std::endl;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

Rectangle::Rectangle() : ul(0, 0), lr(0, 0) {}

Rectangle::Rectangle(Point &upleft, Point &lowright) : ul(upleft), lr(lowright) {}

Rectangle::Rectangle(int upleftx, int uplefty, int lowrightx, int lowrighty) : ul(upleftx, uplefty),
									       lr(lowrightx, lowrighty) {}

Rectangle::Rectangle(Rectangle &other) : ul(other.ul), lr(other.lr)
{
}

void Rectangle::set(Point &upleft, Point &lowright)
{
	ul = upleft;
	lr = lowright;
}

void Rectangle::move(int deltax, int deltay)
{
	ul.move(deltax, deltay);
	lr.move(deltax, deltay);
}

void Rectangle::moveClipped(int deltax, int deltay, Rectangle &clipping)
{
	/* Should we return some status here? Clipping is supposed to be alwaing including this rectangle !!! */
	if (!clipping.includes(*this))
		return;

	move(deltax, deltay);

	if (!clipping.includes(*this))
	{
		if ((lr.x > clipping.lr.x) || (lr.y > clipping.lr.y))
		{
			if (lr.x > clipping.lr.x)
				deltax = clipping.lr.x - lr.x;
			if (lr.y > clipping.lr.y)
				deltay = clipping.lr.y - lr.y;

			move(deltax, deltay);
		}
		else if ((ul.x < clipping.ul.x) || (ul.y < clipping.ul.y))
		{
			if (ul.x < clipping.ul.x)
				deltax = clipping.ul.x - ul.x;
			if (ul.y < clipping.ul.y)
				deltay = clipping.ul.y - ul.y;

			move(deltax, deltay);
		}
	}
}

void Rectangle::zoom(int deltax, int deltay)
{
	ul.move(-deltax, -deltay);
	lr.move(deltax, deltay);
}

void Rectangle::intersection(Rectangle &other)
{
	if (intersect(other))
	{
		if (ul.x < other.ul.x)
			ul.x = other.ul.x;
		if (ul.y < other.ul.y)
			ul.y = other.ul.y;
		if (lr.x > other.lr.x)
			lr.x = other.lr.x;
		if (lr.y > other.lr.y)
			lr.y = other.lr.y;
	}
	else
	{
		ul = lr = {0, 0};
	}
}

void Rectangle::join(Rectangle &other)
{
	if (ul.x > other.ul.x)
		ul.x = other.ul.x;
	if (ul.y > other.ul.y)
		ul.y = other.ul.y;
	if (lr.x < other.lr.x)
		lr.x = other.lr.x;
	if (lr.y < other.lr.y)
		lr.y = other.lr.y;
}

void Rectangle::center(Rectangle &other)
{
	if (other.includes(*this))
	{
		int w = width();
		int h = height();
		ul.x += (other.width() - width()) / 2;
		ul.y += (other.height() - height()) / 2;
		lr.x = ul.x + w;
		lr.y = ul.y + h;
	}
}

bool Rectangle::intersect(Rectangle &other)
{
	if (lr.x < other.ul.x)
		return false;

	if (ul.x > other.lr.x)
		return false;

	if (lr.y < other.ul.y)
		return false;

	if (ul.y > other.lr.y)
		return false;

	return true;
}

bool Rectangle::includes(Rectangle &other)
{
	if ((ul.x <= other.ul.x) &&
	    (ul.y <= other.ul.y) &&
	    (lr.x >= other.lr.x) &&
	    (lr.y >= other.lr.y))
		return true;

	return false;
}

bool Rectangle::includes(Point &point)
{
	if ((ul.x <= point.x) &&
	    (ul.y <= point.y) &&
	    (lr.x >= point.x) &&
	    (lr.y >= point.y))
		return true;

	return false;
}

void Rectangle::delta(Rectangle &other)
{
	Rectangle temp = *this;
	temp.ul.sub(other.ul);
	temp.lr.sub(other.lr);
	other = temp;
}

void Rectangle::clip(Rectangle &clipping)
{
	intersection(clipping);
}

bool Rectangle::superpose(Rectangle &other)
{
	return (width() == other.width()) && (height() == other.height()) ? true : false;
}

int Rectangle::width() const
{
	int retval = lr.x - ul.x + 1;

	return (retval >= 0) ? retval : -retval;
}

int Rectangle::height() const
{
	int retval = lr.y - ul.y + 1;

	return (retval >= 0) ? retval : -retval;
}

void Rectangle::width(int w)
{
	if (w > 0)
		lr.x = ul.x + w;
}

void Rectangle::height(int h)
{
	if (h > 0)
		lr.y = ul.y + h;
}

void Rectangle::print()
{
	std::cout << "RECTANGLE " << std::hex << (intptr_t)this << std::dec << std::endl;
	std::cout << "UL ";
	ul.print();
	std::cout << "LR ";
	lr.print();
	std::cout << "W " << width() << " H " << height() << std::endl;
}