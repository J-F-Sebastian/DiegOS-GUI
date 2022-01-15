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

struct CharScanType
{
    uint8_t charCode;
    uint8_t scanCode;
};

struct KeyDownEvent
{
    union
    {
        uint16_t keyCode;
        CharScanType charScan;
    };
};

struct MouseEvent
{
    int x, y;
    /*
     * Right  -----\
     * Middle ----\ \
     * Left -----\ \ \
     *            \ \ \
     *   7 6 5 4 3 2 1 0
     */
    uint8_t buttons;
    bool pressed;
    bool doubleClick;
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
    CMD_FOCUS
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

#define BROADCAST_OBJECT (void *)(-1UL)
struct MessageEvent
{
    /* Command ID */
    uint16_t command;
    /* Sub command ID */
    uint16_t subCommand;
    /* The object sending this event, i.e. the originator */
    void *senderObject;
    /*
     * The object receiving this event, i.e. the destination.
     * It can be set to BROADCAST_OBJECT to send the event to all objects
     * in the group tree.
     */
    void *destObject;
    /* The target of the command, i.e. the object the action is to be applied upon. */
    void *targetObject;
};

class Event
{
public:
    enum EventType
    {
        EVT_UNKNOWN,
        EVT_MOUSE,
        EVT_KBD,
        EVT_CMD
    };

    Event() { myEventData.what = EVT_UNKNOWN; }
    explicit Event(const MouseEvent &mouse);
    explicit Event(const KeyDownEvent &kbd);
    explicit Event(const MessageEvent &cmd);

    enum EventType getEventType(void);
    bool isEventPositional(void);

    struct MouseEvent *getMouseEvent(void);
    struct KeyDownEvent *getKeyDownEvent(void);
    struct MessageEvent *getMessageEvent(void);

    void setMouseEvent(const MouseEvent &mouse);
    void setKeyDownEvent(const KeyDownEvent &kbd);
    void setMessageEvent(const MessageEvent &cmd);

private:
    struct EventData
    {
        EventType what;
        union
        {
            MouseEvent mouse;
            KeyDownEvent keyDown;
            MessageEvent message;
        };
    } myEventData;
};

#endif