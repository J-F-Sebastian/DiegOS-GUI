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

// #include "list.h"

template <class T>
void ListNode<T>::linkNext(ListNode *target)
{
	if (!target)
		return;

	prev = target;
	next = target->next;
	if (next)
		next->prev = this;
	target->next = this;
}

template <class T>
void ListNode<T>::linkPrev(ListNode *target)
{
	if (!target)
		return;

	next = target;
	prev = target->prev;
	if (prev)
		prev->next = this;
	target->prev = this;
}

template <class T>
void ListNode<T>::unlink()
{
	if (next)
		next->prev = prev;
	if (prev)
		prev->next = next;
	next = prev = 0;
}

template <class T>
List<T>::~List()
{
	ListNode<T> *temp;
	while (head != tail)
	{
		temp = head->getNext();
		delete head;
		head = temp;
	}
	// If eventsListHead is not null, we had at least one event in queue or we
	// need to delete the last event in queue
	if (head)
	{
		delete head;
		head = tail = nullptr;
		items = 0;
	}
}

template <class T>
void List<T>::addHead(T newObject)
{
	ListNode<T> *temp = new ListNode<T>(newObject);
	temp->linkNext(head);
	head = temp;
	items++;
}

template <class T>
void List<T>::addTail(T newObject)
{
	ListNode<T> *temp = new ListNode<T>(newObject);
	temp->linkNext(tail);
	items++;
}

template <class T>
void List<T>::popHead()
{
	if (items)
	{
		ListNode<T> *temp = head;
		head = head->getPrev();
		delete temp;
		items -= 1;
	}
}

template <class T>
void List<T>::popTail()
{
	if (items)
	{
		ListNode<T> *temp = tail->getNext();
		delete temp;
		items -= 1;
	}
}

template <class T>
class List<T>::iterator List<T>::erase(ListNode<T> *item)
{
	if (item == head)
	{
		popHead();
		iterator it(*this);
		return it;
	}
	else if (item == tail->getNext())
	{
		popTail();
		iterator it(tail);
		return it;
	}
	else if (item != tail)
	{
		iterator it(item);
		it++;
		item->unlink();
		delete item;
		items -= 1;
		return it;
	}
	iterator it(tail);
	return it;
}