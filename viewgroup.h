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
	bool remove(View *target);
	View *actualView(void);

	/*
	 * Call function f passing every child view as parameter.
	 * This way the effect of f() are applied to all views.
	 *
	 * PARAMETERS IN
	 * void f(View *) - a function call, either a pointer or a lambda function
	 */
	template <typename FV>
	void forEachView(FV &&f)
	{
		View *head = listHead;
		while (head)
		{
			f(head);
			head = head->getNext();
		}
	}

	/*
	 * Call function f passing every child view as parameter.
	 * This way the effect of f() are applied to all views.
	 * The function f is expected to return false if other views need to be
	 * processed, and is expected to return true if the view passed as parameter
	 * is the one required for processing.
	 * Whenever f() returns true, forEachViewUntilTrue breaks and returns a pointer
	 * to the current view, i.e. the view which made f() return true.
	 * If the loop inside forEachViewUntilTrue never breaks the function returns nullptr.
	 *
	 * PARAMETERS IN
	 * bool f(View *) - a function call, either a pointer or a lambda function.
	 *                  The function is expected to return true or false, false if further views
	 *                  need to be processed, true if processing is complete.
	 *
	 * RETURNS
	 * A valid pointer to a view if f() returned true for this specific View object.
	 * nullptr if f() never returned true.
	 */
	template <typename FB>
	View *forEachViewUntilTrue(FB &&f)
	{
		View *head = listHead;
		while (head)
		{
			if (f(head))
				break;

			head = head->getNext();
		}

		return head;
	}

protected:
	bool focusNext(bool forward);
	void selectNext(bool forward);

	bool focusView(View *target);
	bool selectView(View *target);
	void toForeground(View *target);
	void toBackground(View *target);

	bool thisViewIsMine(View *who);

	virtual void computeExposure(void) override;

	Rectangle lastLimits;

	/*
	 * The selected child view
	 */
	View *actual;
	/*
	 * siblings list, views collection of this group.
	 * listHead points to the foreground view.
	 */
	View *listHead;
	/*
	 * Number of siblings in the views collection
	 */
	unsigned listSize;
	/*
	 * Store the previously used resize flags to restore them later.
	 * This is useful for resizing/zooming operations.
	 */
	unsigned char lastrflags;
};

#endif