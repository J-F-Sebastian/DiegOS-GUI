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

	void print(void);

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

	/*
	 * Sets the coordinates of the upper left and lower right points
	 * of a rectangle.
	 *
	 * PARAMETERS IN
	 * upleft   - upper left coordinates
	 * lowright - lower right coordinates
	 */
	void set(Point &upleft, Point &lowright);

	/*
	 * Move the rectangle. Adds deltax and deltay to both
	 * upper left and lower right corners.
	 * Area and form factor are not affected by this operation.
	 *
	 * PARAMETERS IN
	 * deltax   - x value (can be negative)
	 * deltay   - y value (can be negative)
	 */
	void move(int deltax, int deltay);

	/*
	 * Move the rectangle. Adds deltax and deltay to both
	 * upper left and lower right corners.
	 * The rectangle cannot exceed boundaries explicited in clipping,
	 * i.e. the move operation is clipped to the clipping boundaries.
	 * Area and form factor are not affected by this operation.
	 *
	 * PARAMETERS IN
	 * deltax   - x value (can be negative)
	 * deltay   - y value (can be negative)
	 * clipping - clipping boundaries, this rectangle cannot move outside these limits
	 */
	void moveClipped(int deltax, int deltay, Rectangle &clipping);
	/*
	 * Zoom the rectangle. Subtracts deltax and deltay from
	 * upper left corner and adds deltax and deltay to lower right corner.
	 * Area and form factor may be affected by this operation.
	 *
	 * PARAMETERS IN
	 * deltax   - x value (can be negative)
	 * deltay   - y value (can be negative)
	 */
	void zoom(int deltax, int deltay);

	/*
	 * Compute the intersection with the input rectangle
	 * and store the result internally.
	 *
	 * PARAMETERS IN
	 * other - the rectangle to intersect with
	 */
	void intersection(Rectangle &other);

	/*
	 * Compute the union with the input rectangle
	 * and store the result internally.
	 * Union equals a rectangle including both this
	 * and other rectangles.
	 *
	 * PARAMETERS IN
	 * other - the rectangle to join with
	 */
	void join(Rectangle &other);

	/*
	 * Move the rectangle to be centered within the
	 * other rectangle.
	 * The other rectangle must include the object's rectangle.
	 * Area and form factor are not affected by this operation.
	 * The coordinates will be altered to have the centers of the
	 * 2 rectangles be the same.
	 *
	 * PARAMETERS IN
	 * other - the rectangle to use for centering
	 */
	void center(Rectangle &other);

	/*
	 * Check if the intersection with the other rectangle
	 * is void or not.
	 *
	 * PARAMETERS IN
	 * other - the rectangle to intersect with
	 *
	 * RETURNS
	 * true if the intersection is not void, i.e. the intersection
	 *      would be a rectangle with a not null area
	 * false in any other case
	 */
	bool intersect(Rectangle &other);

	/*
	 * Check if the other rectangle is included in this or not.
	 *
	 * PARAMETERS IN
	 * other - the rectangle to check
	 *
	 * RETURNS
	 * true if other rectangle is completely included in this rectangle
	 * false in any other case
	 */
	bool includes(Rectangle &other);

	/*
	 * Check if the other point is included in this rectangle.
	 *
	 * PARAMETERS IN
	 * other - the point to be check
	 *
	 * RETURNS
	 * true if the point is included in this rectangle
	 * false in any other case
	 */
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

	/*
	 * Perform clipping of this rectangle using clipping as boundaries.
	 * The result of the operation equals computing the intersection
	 * between this and clipping.
	 * Clipping is assumed to either contain this rectangle or to have a
	 * not null intersection, otherwise the result of clipping would be
	 * a void rectangle.
	 *
	 * PARAMETERS IN
	 * clipping - the clipping rectangle
	 */
	void clip(Rectangle &clipping);

	int width(void) const;
	int height(void) const;
	void width(int w);
	void height(int h);

	inline bool operator==(const Rectangle &other)
	{
		return ((ul == other.ul) && (lr == other.lr)) ? true : false;
	}

	inline bool operator!=(const Rectangle &other)
	{
		return !((*this) == other);
	}

	inline Rectangle &operator=(const Rectangle &other)
	{
		ul = other.ul;
		lr = other.lr;
		return (*this);
	}

	void print(void);

	/* Upper left corner */
	Point ul;
	/* Lower right corner */
	Point lr;
};

#endif