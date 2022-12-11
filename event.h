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

#ifndef _EVENT_H_
#define _EVENT_H_

#include <cstdint>

struct KeyDownEvent
{
    uint16_t keyCode;
    uint16_t modifier;
};

enum
{
    POS_EVT_RIGHT = 1 << 0,
    POS_EVT_MIDDLE = 1 << 1,
    POS_EVT_LEFT = 1 << 2,
    POS_EVT_TAP = 1 << 3,
};

enum
{
    // Single click
    POS_EVT_PRESSED = 1 << 0,
    // Double click
    POS_EVT_DOUBLE = 1 << 1,
    // Button pressed while moving
    POS_EVT_LONG = 1 << 2
};

struct PositionalEvent
{
    int x, y;
    /*
     * Right --------
     * Middle ------ \
     * Left ------- \ \
     * Tap ------- \ \ \
     *            \ \ \ \
     *    7 6 5 4 3 2 1 0
     */
    uint8_t buttons;
    /*
     * Pressed ---------------
     * Double Pressure ------ \
     * Long Pressure ------- \ \
     *                      \ \ \
     *            7 6 5 4 3 2 1 0
     */
    uint8_t status;
};

/*
 * Commands
 */
enum
{
    /* Quit the application */
    CMD_QUIT,
    /* View will be closed and removed */
    CMD_CLOSE,
    /* View will be zoomed, i.e. will resize to the extents of the parent */
    CMD_MAXIMIZE,
    /* View will be resized to the minimum extent */
    CMD_MINIMIZE,
    /* View will return to the original size */
    CMD_RESTORE,
    /* A view is signalling an input event, data are ready to be handled */
    CMD_INPUT,
    /* A view is being moved around */
    CMD_DRAGGING,
    /* A view is being resized */
    CMD_RESIZING,
    /* Move the view to the top of the list */
    CMD_FOREGROUND,
    /* Select a view */
    CMD_SELECT,
    /* Focus a view */
    CMD_FOCUS,
    /* Update the view with data found in the message */
    CMD_UPDATE,
    /* Redraw the view */
    CMD_REDRAW,
    /* Draw all views */
    CMD_DRAW
};

/*
 * Sub Commands
 */
enum
{
    /* OK */
    RESPONSE_OK,
    /* Cancel the operation */
    RESPONSE_CANCEL,
    /* Apply data or action */
    RESPONSE_APPLY,
    /* Move to the previous item (can be anything) */
    RESPONSE_PREV,
    /* Move to the next item (can be anything) */
    RESPONSE_NEXT
};

#define BROADCAST_OBJECT (void *)(-1)
struct MessageEvent
{
    /* Command ID */
    uint16_t command;
    /* Sub command ID */
    uint16_t subCommand;
    /*
     * The object sending this event, i.e. the originator.
     * This value must be assigned anytime to a valid value.
     * It cannot be set to BROADCAST_OBJECT.
     */
    void *senderObject;
    /*
     * The object receiving this event, i.e. the destination.
     * It can be set to BROADCAST_OBJECT to send the event to all objects
     * in a group.
     * The destination is supposed to be the object that process the message.
     */
    void *destObject;
    /*
     * The target of the command, i.e. the object the action is to be applied upon.
     * destObject is supposed to process the command and apply it to targetObject.
     */
    void *targetObject;
    /*
     * Optional payload, up to the message implementation.
     */
    void *payload;
};

class Event
{
public:
    enum EventType
    {
        EVT_UNKNOWN,
        EVT_POS,
        EVT_KBD,
        EVT_CMD
    };

    Event() { myEventData.what = EVT_UNKNOWN; }
    explicit Event(const Event &other) { myEventData = other.myEventData; }
    explicit Event(const PositionalEvent &pos);
    explicit Event(const KeyDownEvent &kbd);
    explicit Event(const MessageEvent &cmd);

    enum EventType getEventType(void);
    bool isEventPositional(void);
    bool isEventKey(void);
    bool isEventUnknown(void);

    struct PositionalEvent *getPositionalEvent(void);
    struct KeyDownEvent *getKeyDownEvent(void);
    struct MessageEvent *getMessageEvent(void);

    void setPositionalEvent(const PositionalEvent &pos);
    void setKeyDownEvent(const KeyDownEvent &kbd);
    void setMessageEvent(const MessageEvent &cmd);

    bool testPositionalEventPos(char bitmap);
    bool testPositionalEventStatus(char bitmap);

    void clear(void);

private:
    struct EventData
    {
        EventType what;
        union
        {
            PositionalEvent position;
            KeyDownEvent keyDown;
            MessageEvent message;
        };
    } myEventData;
};

#endif