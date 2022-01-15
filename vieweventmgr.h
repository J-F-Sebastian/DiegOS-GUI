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

#ifndef _VIEWEVENTMGR_H_
#define _VIEWEVENTMGR_H_

#include "event.h"

class ViewEventManager
{
public:
    virtual ~ViewEventManager() {}

    /*
     * Waits for input, and return the read status.
     * The wait action depends on the value set in timeoutms
     *
     * - If timeoutms is positive, wait for new events up to the specified milliseconds
     * - If timeoutms is null, poll for new events and return immediately
     * - If timeoutms is negative, wait for new events forever
     *
     * NO MEMORY MANAGEMENT is performed on evt, its value MUST BE a valid pointer to a Event class object.
     *
     * PARAMETER IN
     * Event *evt - pointer to an Event class to store the received event
     * int timeoutms - wait time in milliseconds (see above description)
     *
     * RETURN
     * true if an event is stored into evt, false in any other case
     */
    virtual bool wait(Event *evt, int timeoutms) = 0;

    /*
     * Polls for input and return the read status.
     * 
     * RETURN
     * true if at least one event is in the queue and can be read, false in any other case
     */
    virtual bool poll(void) = 0;

    /*
     * Copy the event from evt to the events queue. The queue is platform dependent, its management
     * is not bound to the View library.
     * NO MEMORY MANAGEMENT is performed on evt, its value MUST BE a valid pointer to a Event class object.
     * Contents of the object pointed by evt MUST BE copied internally, after the call evt can be deallocated.
     * The memory management of the events' queue (if it is a queue) is up to the specific implementation.
     * 
     * PARAMETER IN
     * Event *evt
     */
    virtual bool put(Event *evt) = 0;

protected:
    ViewEventManager()
    {
    }
};

#endif