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

#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

/*
 * This class models a 2-D point on video, whose coordinates are
 * x and y and are relative to the upper left corner of the screen
 * or of a view port.
 * Upper left corner is referred by Point(0,0).
 * This class is generic - it is not specialized for graphics or text.
 */
class Point
{
public:
	Point();
	Point(int x, int y);
	Point(const Point &other);

	void set(int x, int y);
	void move(int deltax, int deltay);
	void add(Point &other);
	void sub(Point &other);
	void neg(void)
	{
		x = -x;
		y = -y;
	}

	inline bool operator==(const Point &other)
	{
		return ((x == other.x) && (y == other.y)) ? true : false;
	};

	inline bool operator!=(const Point &other)
	{
		return !((*this) == other);
	}

	inline void operator+=(const Point &other)
	{
		x += other.x;
		y += other.y;
	}

	inline void operator-=(const Point &other)
	{
		x -= other.x;
		y -= other.y;
	}

	inline void operator*=(int num)
	{
		x *= num;
		y *= num;
	}

	inline Point &operator=(const Point &other)
	{
		x = other.x;
		y = other.y;
		return (*this);
	}

	int x, y;
};

/*
 * This class models a 2-D rectangle, described by 2 points referring to
 * the upper left and lower right corners.
 * Upper left corner is referred by ul, lower right corner is referred by lr.
 * This class is generic - it is not specialized for graphics or text.
 * The class implements some CSG functions like intersection and union.
 */
class Rectangle
{
public:
	Rectangle();
	Rectangle(Point &upleft, Point &lowright);
	Rectangle(int upleftx, int uplefty, int lowrightx, int lowrighty);
	Rectangle(Rectangle &other);

	void set(Point &upleft);
	void set(Point &upleft, Point &lowright);
	void move(int deltax, int deltay);
	void zoom(int deltax, int deltay);
	void intersection(Rectangle &other);
	void join(Rectangle &other);
	void center(Rectangle &other);
	bool intersect(Rectangle &other);
	bool includes(Rectangle &other);
	bool includes(Point &point);
	/*
	 * computes the difference between other and this rectangle.
	 * Output is stored in other.
	 * 
	 * PARAMETERS IN
	 * Rectangle &other - the rectangle to subtract 
	 * 
	 * PRAMETERS OUT
	 * Rectangle &other - the result of the difference this -= other
	 */
	void delta(Rectangle &other);
	int width(void) const;
	int height(void) const;
	void width(int w);
	void height(int h);

	inline bool operator==(const Rectangle &other)
	{
		return ((ul == other.ul) && (lr == other.lr)) ? true : false;
	}

	inline Rectangle &operator=(const Rectangle &other)
	{
		ul = other.ul;
		lr = other.lr;
		return (*this);
	}

	/* Upper left corner */
	Point ul;
	/* Lower right corner */
	Point lr;
};

#endif