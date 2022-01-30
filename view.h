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

#ifndef _VIEW_H_
#define _VIEW_H_

#include "list.h"
#include "geometry.h"
#include "event.h"
#include "palettegroup.h"
#include "viewrender.h"

/*
 * resize flags; there are 4 flags for resizing, one for each coordinate
 * of a rectangle.
 * Resizable means that the lower right point coordinates of a rectangle
 * will be updated.
 * The upper left point will remain unchanged, width and height will change.
 * Bounded means that all coordinates will be updated.
 * In the present implementation the "resize to" values are passed in with
 * a Point class, so a Bounded view will not change its width and height,
 * but only its position (as the same x and y values will be added to both
 * upper left and lower right points).
 */
enum
{
	VIEW_RESIZE_UX = (1 << 0),
	VIEW_RESIZE_UY = (1 << 1),
	VIEW_RESIZE_LX = (1 << 2),
	VIEW_RESIZE_LY = (1 << 3),
	VIEW_RESIZEABLE = (VIEW_RESIZE_LX | VIEW_RESIZE_LY),
	VIEW_BOUNDED = (VIEW_RESIZE_UX | VIEW_RESIZE_UY | VIEW_RESIZE_LX | VIEW_RESIZE_LY)
};

/*
 * State flags
 */
enum
{
	/* View is visible */
	VIEW_STATE_VISIBLE = (1 << 0),
	/* View is selected, i.e. it is the actual selection */
	VIEW_STATE_SELECTED = (1 << 1),
	/* View has the focus, i.e. it will receive inputs from user */
	VIEW_STATE_FOCUSED = (1 << 2),
	/* View is disabled, no events are processed for this view */
	VIEW_STATE_DISABLED = (1 << 3),
	/* View is moving around or resizing */
	VIEW_STATE_DRAGGING = (1 << 4),
	/* View is running the event loop */
	VIEW_STATE_EVLOOP = (1 << 5),
	/* View is to be drawn on video */
	VIEW_STATE_EXPOSED = (1 << 6)
};

/*
 * Option flags
 */
enum
{
	/* View can be selected or get the focus */
	VIEW_OPT_SELECTABLE = (1 << 0),
	/* View will be moved to foreground when selected */
	VIEW_OPT_TOPSELECT = (1 << 1),
	/* View will preprocess events */
	VIEW_OPT_PREPROCESS = (1 << 2),
	/* View will postprocess events */
	VIEW_OPT_POSTPROCESS = (1 << 3),
	/* View can be tiled inside its owner boundaries */
	VIEW_OPT_TILEABLE = (1 << 4),
	/* View can be centered w.r.t. owner's X axis */
	VIEW_OPT_CENTERX = (1 << 5),
	/* View can be centered w.r.t. owner's Y axis */
	VIEW_OPT_CENTERY = (1 << 6),
	/* View can be centered w.r.t. owner boundaries */
	VIEW_OPT_CENTERED = (VIEW_OPT_CENTERX | VIEW_OPT_CENTERY),
	/* View need validation */
	VIEW_OPT_VALIDATE = (1 << 7),
	VIEW_OPT_ALL = (VIEW_OPT_SELECTABLE |
			VIEW_OPT_TOPSELECT |
			VIEW_OPT_PREPROCESS |
			VIEW_OPT_POSTPROCESS |
			VIEW_OPT_TILEABLE |
			VIEW_OPT_CENTERED |
			VIEW_OPT_VALIDATE)
};

/*
 * Changed flags, used to mark the need to update the view in some way.
 */
enum
{
	/* View need to be redrawn */
	VIEW_CHANGED_REDRAW = (1 << 0),
	/* View carries updated data */
	VIEW_CHANGED_DATA = (1 << 1)
};

/*
 * Class View is the basic interface to a graphical - or text view.
 * Coordinates are referred to the owner's origin.
 */
class View
{
public:
	virtual ~View();

	/*
	 * Compute new limits using resize flags.
	 * Increments stored in delta are added to the view's borders
	 * according to flags stored in rflags, the resulting rectangle
	 * is stored in newrect.
	 * 
	 * PARAMETERS IN
	 * const Point &delta - delta values
	 * 
	 * PARAMETERS OUT
	 * Rectangle &newrect - the resulting rectangle after computation
	 */
	void calcLimits(const Point &delta, Rectangle &newrect);

	/*
	 * Compute minimum and maximum width and height and store
	 * the results in min and max.
	 * The default computation returns (0,0) in min and
	 * (width, height) in max if owner is nil; if owner is valid
	 * the width and height are those of the owner.
	 * 
	 * PARAMETERS OUT
	 * Point &min - minimum width and height
	 * Point &max - maximum width and height
	 */
	virtual void sizeLimits(Point &min, Point &max);

	/*
	 * Validate the new location coordinates stored in loc
	 * and then invokes changeLimits to apply them.
	 * The new location must be included in the owner borders
	 * if owner is valid.
	 * 
	 * PARAMETERS IN
	 * const Rectangle &loc - the new location
	 * 
	 * RETURN
	 * true if the new location was validated
	 * false if the new location was rejected
	 */
	virtual bool setLocation(const Rectangle &loc);

	/*
	 * Compute the new location coordinates adding
	 * delta to the borders rectangle, validates the new location
	 * and then invokes changeLimits to apply it.
	 * The new location must be included in the owner borders
	 * if owner is valid.
	 * 
	 * PARAMETERS IN
	 * const Point &delta - delta coordinates to be added to the origin
	 * 
	 * RETURN
	 * true if the new location was validated
	 * false if the new location was rejected
	 */
	virtual bool moveLocation(const Point &delta);

	/*
	 * Copy the upper left point of borders into origin.
	 * Origin is relative to the owner's origin.
	 * 
	 * PARAMETERS OUT
	 * Point &origin - view's origin
	 */
	void getOrigin(Point &origin) const;

	/*
	 * Copy the extents of the view, i.e. set extent to
	 * a rectangle with upper left set to (0,0) and lower right
	 * set to (width - 1, height - 1).
	 * NOTE: width and height are computed in pixels and include
	 * both vertical and horizontal lines.
	 * 
	 * PARAMETERS OUT
	 * Rectangle &extent - reference to a Rectangle set to the extents of the view
	 */
	void getExtent(Rectangle &extent);

	/*
	 * Convert rectangle coordinates from global (root owner view)
	 * to local (this view).
	 *
	 * PARAMETERS IN/OUT
	 * Rectangle &rect - reference to a Rectangle in global coordinates,
	 *                   will be converted to local coordinates
	 */
	void localize(Rectangle &rect);

	/*
	 * Convert rectangle coordinates from local (this view)
	 * to global (root owner view).
	 * This method is complementary to localize().
	 *
	 * PARAMETERS IN/OUT
	 * Rectangle &rect - reference to a Rectangle in local coordinates,
	 *                   will be converted to global coordinates
	 */
	void globalize(Rectangle &rect);

	/*
	 * Set the parent view (owner).
	 *
	 * PARAMETERS IN
	 * View *par - pointer to a View instance
	 */
	void setParent(View *par);

	/*
	 * Set the renderer instance.
	 *
	 * PARAMETERS IN
	 * ViewRenderer *rnd - pointer to a ViewRender instance
	 */
	void setRenderer(ViewRender *rnd);

	/*
	 * Set the palette instance.
	 *
	 * PARAMETERS IN
	 * Palette *pal - pointer to a Palette instance
	 */
	void setPalette(Palette *pal);

	/*
	 * Operate on the view resize options flags.
	 */
	void setResizeMode(unsigned char flags);
	bool getResizeMode(unsigned char flags) const;
	void clearResizeMode(unsigned char flags);

	/*
	 * Operate on the view options flags.
	 */
	void setOptions(unsigned char flags);
	bool getOptions(unsigned char flags) const;
	void clearOptions(unsigned char flags);

	/*
	 * Operate on the view state flags.
	 */
	void setState(unsigned char flags);
	bool getState(unsigned char flags) const;
	void clearState(unsigned char flags);

	/*
	 * Operate on the view changed flags.
	 */
	void setChanged(unsigned char flags);
	bool getChanged(unsigned char flags) const;
	void clearChanged(unsigned char flags);

	/*
	 * Draw the graphics of the view unconditionally.
	 * The default method is null.
	 */
	virtual void draw(void) = 0;

	/*
	 * Make use of the Event object to perform tasks.
	 * The default handleEvent will evaluate positional events,
	 * in case the event coordinates are inside the view limits
	 * and the PRESSED bit is set, the view will select herself
	 * and notify with a message its owner.
	 * NO memory management is to be performed on evt.
	 * 
	 * PARAMETERS IN
	 * Event *evt - a pointer to an Event object
	 */
	virtual void handleEvent(Event *evt);

	/*
	 * Send an event to the view's owner.
	 * The method will recursively look for the top owner.
	 * NO memory management is to be performed on evt.
	 * 
	 * PARAMETERS IN
	 * Event *evt - a pointer to an event object
	 */
	virtual void sendEvent(Event *evt);

	/*
	 * Verify the validity of the event for this view and returns 
	 * a boolean according to the evaluation.
	 * A positional event is valid if:
	 * 	1) the pointer is not null
	 * 	2) the event is positional
	 * 	3) the event coordinates are in the view's range
	 *
	 * PARAMETERS IN
	 * Event *evt - the event to be validated
	 * 
	 * RETURN
	 * true if the event is valid for this view, i.e. the event has coordinates
	 *      falling inside the view limits, and the event type is positional (mouse, touchscreen)
	 * false if the event is not valid for this view and should not be processed by
	 *       handleEvent
	 */
	virtual bool isEventPositionValid(Event *evt);

protected:
	explicit View(Rectangle &limits, ViewRender *rnd = nullptr, Palette *pal = nullptr, View *parent = nullptr);

	/*
	 * Apply new limits.
	 * Limits are stored in a rectangle in owners coordinates.
	 * 
	 * PARAMETERS IN
	 * const Rectangle &newrect - new view limits
	 */
	void setLimits(const Rectangle &newrect);

	/*
	 * Copy limits into rect.
	 * Limits are stored in a rectangle in owners coordinates.
	 * 
	 * PARAMETERS OUT
	 * Rectangle &rect - view limits
	 */
	void getLimits(Rectangle &rect);

	/*
	 * Apply new limits and draw the view.
	 * Limits are stored in a rectangle in owners coordinates.
	 * 
	 * PARAMETERS IN
	 * const Rectangle &newrect - the new boundaries
	 */
	void changeLimits(const Rectangle &newrect);

	/*
	 * General explanation: each view has its origin at (0,0) and expresses
	 * coordinates inside its extents, or in other words, points inside a view
	 * can have values ranging from (0,0) to (viewwidth - 1, viewheight - 1).
	 * Now the same coordinates can be referred to the root view, which is supposed to be
	 * the screen or a containing window.
	 * The child view origin will have a pixel coordinates of (x, y) on screen, so
	 * using global coordinates (screen coordinates) a point inside the child view
	 * can have values ranging from (x, y) to (x + viewwidth - 1, y + viewheigth - 1).
	 * 
	 *origin+----------------------------------------+
	 *     |                                        |
	 *     |     (x,y)                              |
	 *     |     origin+---------+                  |
	 *     |           |(0,0)    |                  |
	 *     |           |         |                  |
	 *     |           +---------+                  |
	 *     |                                        |
	 *     |                                        |
	 *     +----------------------------------------+
	 */

	/*
	 * Converts coordinates from global (root parent)
	 * to local (referred to the origin of the calling view).
	 * origin is assumed to be valorized with root parent coordinates,
	 * so that input origin(0,0) is the origin of the root parent, which
	 * is usually the size of the screen.
	 * 
	 * PARAMETERS IN
	 * Point &origin - global coordinates (screen coordinates)
	 * 
	 * PARAMETERS OUT
	* Point &origin - local coordinates (view coordinates)
	 */
	void makeLocal(Point &origin);

	/*
	 * Converts coordinates from local (referred to the origin of the calling view)
	 * to global (screen coordinates).
	 * origin is assumed to be valorized with view coordinates,
	 * so that input origin(0,0) is the upper left view corner.
	 * 
	 * PARAMETERS IN
	 * Point &origin - local coordinates (view coordinates)
	 * 
	 * PARAMETERS OUT
	* Point &origin - global coordinates (screen coordinates)
	 */
	void makeGlobal(Point &origin);

	/*
	 * Verify that coordinates of the positional event are in the view's range.
	 * The method will not check for pointer validity OR positional validity.
	 * Use as a speedup but with caution.
	 *
	 * PARAMETERS IN
	 * Event *evt - the event
	 * 
	 * RETURN
	 * true if the event is in range for this view, i.e. the event has coordinates
	 *      falling inside the view limits
	 * false if the event is outside this view's range
	 */
	bool isEventPositionInRange(Event *evt);

	/*
	 * Verify that the event is a command and the destination object matches this
	 * instance.
	 *
	 * PARAMETERS IN
	 * Event *evt - the event
	 * 
	 * RETURN
	 * true if the event is a command with destObject == this
	 * false in any other case
	 */
	bool isEventCmdForMe(Event *evt);

	View *getParent(void) { return parentView; }
	ViewRender *getRenderer(void) { return renderer; }
	Palette *getPalette(void) { return palette; }

	bool focus(void);
	void select(void);

private:
	/*
	 * The parent or Owner of this view, can be nullptr
	 */
	View *parentView;
	/*
	 * renderer instance, cannot be nullptr
	 */
	ViewRender *renderer;
	/*
	 * Color palette, cannot be nullptr
	 */
	Palette *palette;

	/*
	 * borders is expressed in owners coordinates
	 */
	Rectangle borders;
	/*
	 * extent is the width and height of the view
	 */
	Rectangle extent;
	unsigned char rflags, sflags, oflags, cflags;
};

class ViewGroup : public View
{
public:
	explicit ViewGroup(Rectangle &limits, ViewRender *rnd, PaletteGroup *pals, View *parent = nullptr);
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
	/*
	 * Compute the new location coordinates adding
	 * delta to the limits rectangle, validates the new location
	 * and then invokes changeLimits to apply it.
	 * The new location must be included in the owner limits
	 * if owner is valid.
	 * 
	 * PARAMETERS IN
	 * const Point &delta - delta coordinates to be added to the origin
	 * 
	 * RETURN
	 * true if the new location was validated
	 * false if the new location was rejected
	 */
	virtual bool moveLocation(const Point &delta) override;

	virtual void draw(void) override;

	virtual void handleEvent(Event *evt) override;

	void maximize(void);
	void minimize(void);

	void insert(View *newView);
	void insertBefore(View *newView, View *target);

	void setPalettes(PaletteGroup *pals);

protected:
	bool focusNext(bool forward);
	void selectNext(bool forward);

	bool focusView(View *target);
	void selectView(View *target);
	void toTheTop(View *target);

	bool thisViewIsMine(View *who);

	Rectangle lastLimits;
	View *focused;
	View *selected;
	PaletteGroup *palettes;

private:
	List<View *> viewList;
};

#endif