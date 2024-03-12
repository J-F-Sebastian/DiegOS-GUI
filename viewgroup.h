/*
 * DiegOS Operating System source code
 *
 * Copyright (C) 2012 - 2022 Diego Gallizioli
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

#ifndef _VIEWGROUP_H_
#define _VIEWGROUP_H_

#include "view.h"
#include "list.h"

class ViewGroup : public View
{
public:
	explicit ViewGroup(Rectangle &limits, View *parent = nullptr);
	virtual ~ViewGroup();

	/*
	 * Validate the new location coordinates stored in loc
	 * and then invokes changeLimits to apply them.
	 * The new location must be included in the owner limits
	 * if owner is valid.
	 *
	 * PARAMETERS IN
	 * const Rectangle &loc - the new location
	 */
	virtual bool setLocation(const Rectangle &loc) override;

	virtual void setExposed(bool exposed) override;

	virtual void draw(void) override;
	virtual void reDraw(void) override;

	virtual void handleEvent(Event *evt) override;
	virtual bool executeCommand(MessageEvent *cmd) override;
	virtual bool validateCommand(const uint16_t command) override;

	void forEachExecuteCommand(MessageEvent *cmd);

	virtual void setForeground(void) override;
	virtual void setBackground(void) override;

	void maximize(void);
	void minimize(void);
	void restore(void);

	void insert(View *newView);
	void insertBefore(View *newView, View *target);
	bool remove(View *target);
	View *actualView(void);

protected:
	bool focusNext(bool forward);
	void selectNext(bool forward);

	bool focusView(View *target);
	void selectView(View *target);
	void setForeground(View *target);
	void toTheTop(View *target);

	bool thisViewIsMine(View *who);

	virtual void computeExposure(void) override;

	Rectangle lastLimits;
	View *actual;

	/*
	 * List of views owned by this group; the list is Z-ordered from foreground to background,
	 * is populated by calling insert or insertBefore, and can be reordered when the selected object
	 * has the VIEW_OPT_TOPSELECT option set.
	 */
	List<View *> viewList;

	/*
	 * Store the previously used resize flags to restore them later.
	 * This is useful for resizing/zooming operations.
	 */
	unsigned char lastrflags;
};

#endif