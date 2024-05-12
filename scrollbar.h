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

#ifndef _SCROLLBAR_H_
#define _SCROLLBAR_H_

#include "view.h"

class ScrollBar : public View
{
public:
	ScrollBar(Rectangle &viewLimits, unsigned refElements, unsigned refVisible, unsigned refPosition);

	virtual void handleEvent(Event *evt) override;

	void setRefElements(unsigned newval);
	void setRefVisible(unsigned newval);
	void setRefPosition(unsigned newval);

	unsigned getRefElements(void) const;
	unsigned getRefVisible(void) const;
	unsigned getRefPosition(void) const;

protected:
	/*
	 * Updates coordinates of the active pad sliding the pad
	 * left/right or up/down
	 */
	virtual void updateActivePad(Point &newpos) = 0;

	/*
	 * Compute the active pad coordinates using the attributes of the class
	 * as a reference.
	 * Call this method when attributes are updated and the scrollbar need to
	 * adapt to the new parameters
	 */
	virtual void computeActivePad(void) = 0;

	/*
	 * Compute the attributes of the class using the active pad coordinates
	 * as a reference.
	 * Call this method when the pad slides and attributes need to be updated.
	 */
	virtual void computeAttributes(void) = 0;

	/*
	 * Total number of elements linked to the scroll bar, number of visible elements,
	 * first visible element (or selectable, or driving the sliding actions).
	 * The pad size and position are computed with these numbers.
	 */
	unsigned refElements, refVisible, refPosition;
	Rectangle activePad;
	Point lastPressure;
};

class VScrollBar : public ScrollBar
{
public:
	VScrollBar(Rectangle &viewLimits, unsigned refElements, unsigned refVisible, unsigned refPosition);

protected:
	virtual void drawView(void) override;
	virtual void updateActivePad(Point &newpos) override;
	virtual void computeActivePad(void) override;
	virtual void computeAttributes(void) override;
};

class HScrollBar : public ScrollBar
{
public:
	HScrollBar(Rectangle &viewLimits, unsigned refElements, unsigned refVisible, unsigned refPosition);

protected:
	virtual void drawView(void) override;
	virtual void updateActivePad(Point &newpos) override;
	virtual void computeActivePad(void) override;
	virtual void computeAttributes(void) override;
};

#endif