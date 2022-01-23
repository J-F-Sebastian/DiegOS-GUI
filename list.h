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

#ifndef _LIST_H_
#define _LIST_H_
#include <iostream>
/*
 * class ListNode objects are items of a linked list.
 * next points to the beginning of the list (the head), prev points to the
 * end of the list (the tail).
 * This class can be used to link objects in a list or remove them from a list,
 * however you need additional logic to manage the list as such.
 */
template <class T>
class ListNode
{
public:
  ListNode() : next(nullptr), prev(nullptr), obj() {}
  explicit ListNode(T object) : next(nullptr), prev(nullptr), obj(object) {}

  ~ListNode()
  {
    unlink();
  }

  /*
   * link this object next to the target, i.e. this object will be the object
   * pointed to by next attribute of target.
   */
  void linkNext(ListNode *target)
  {
    if (!target)
      return;

    prev = target;
    next = target->next;
    if (next)
      next->prev = this;
    target->next = this;
  }

  /*
   * link this object past the target, i.e. this object will be the object
   * pointed to by prev attribute of target.
   */
  void linkPrev(ListNode *target)
  {
    if (!target)
      return;

    next = target;
    prev = target->prev;
    if (prev)
      prev->next = this;
    target->prev = this;
  }

  /*
   * Cut this object off the list it belongs to. The object is not destroyed or deallocated,
   * it is only isolated from other linked objects.
   */
  void unlink(void)
  {
    if (next)
      next->prev = prev;
    if (prev)
      prev->next = next;
    next = prev = nullptr;
  }

  ListNode *next, *prev;
  T obj;
};

/*
 * Class List implements the management of a doubly linked list, it allows adding
 * and removing from head or tail, removing objects from the list, and browsing it
 * with iterators.
 */
template <class T>
class List
{
public:
  class iterator
  {
  public:
    explicit iterator() : cursor(nullptr), instance(nullptr)
    {
    }

    explicit iterator(List &list, ListNode<T> *begin) : instance(&list)
    {
      if (begin && (begin != list.head))
        cursor = begin;
      else if (list.count())
        cursor = list.head->prev;
      else
        cursor = list.tail;
    }

    explicit iterator(List &list, T *begin = nullptr) : instance(&list)
    {
      if (list.count())
      {
        cursor = list.head->prev;
        if (begin)
        {
          ListNode<T> *find = cursor;
          while ((find != list.tail) && (find->obj != *begin))
            find = find->prev;

          cursor = find;
        }
      }
      else
      {
        cursor = list.tail;
      }
    }

    iterator(const iterator &other) : cursor(other.cursor), instance(other.instance) {}

    ~iterator() {}

    bool operator!=(iterator const &b) { return ((instance != b.instance) || (cursor != b.cursor)); }
    bool operator==(iterator const &b) { return ((instance == b.instance) && (cursor == b.cursor)); }
    // Postfix increment
    iterator operator++(int)
    {
      iterator old(*this);
      if (cursor != instance->tail)
        cursor = cursor->prev;
      return old;
    }
    // Prefix increment
    iterator &operator++()
    {
      if (cursor != instance->tail)
        cursor = cursor->prev;
      return *this;
    }
    // Postfix decrement
    iterator operator--(int)
    {
      iterator old(*this);
      if (cursor != instance->head)
        cursor = cursor->next;
      return old;
    }
    // Prefix decrement
    iterator &operator--()
    {
      if (cursor != instance->head)
        cursor = cursor->next;
      return *this;
    }

    iterator &operator=(iterator const &b)
    {
      if (this != &b)
      {
        cursor = b.cursor;
        instance = b.instance;
      }
      return *this;
    }

    T &operator*() { return (cursor->obj); }

    bool checkInstance(List<T> *tgt)
    {
      return (tgt == instance) ? true : false;
    }

  private:
    ListNode<T> *cursor;
    List<T> *instance;
  };

  class riterator
  {
  public:
    explicit riterator() : cursor(nullptr), instance(nullptr)
    {
    }

    explicit riterator(List &list, ListNode<T> *begin) : instance(&list)
    {
      if (begin && (begin != list.tail))
        cursor = begin;
      else if (list.count())
        cursor = list.tail->next;
      else
        cursor = list.head;
    }

    explicit riterator(List &list, T *begin = nullptr) : instance(&list)
    {
      if (list.count())
      {
        cursor = list.tail->next;
        if (begin)
        {
          ListNode<T> *find = cursor;
          while ((find != list.head) && (find->obj != *begin))
            find = find->next;

          cursor = find;
        }
      }
      else
        cursor = list.head;
    }

    riterator(const riterator &other) : cursor(other.cursor), instance(other.instance) {}
    ~riterator() {}

    bool operator!=(riterator const &b) { return ((instance != b.instance) || (cursor != b.cursor)); }
    bool operator==(riterator const &b) { return ((instance == b.instance) && (cursor == b.cursor)); }
    // Postfix increment
    riterator operator++(int)
    {
      riterator old(*this);
      if (cursor != instance->head)
        cursor = cursor->next;
      return old;
    }
    // Prefix increment
    riterator &operator++()
    {
      if (cursor != instance->head)
        cursor = cursor->next;
      return *this;
    }
    // Postfix decrement
    riterator operator--(int)
    {
      riterator old(*this);
      if (cursor != instance->tail)
        cursor = cursor->prev;
      return old;
    }
    // Prefix decrement
    riterator &operator--()
    {
      if (cursor != instance->tail)
        cursor = cursor->prev;
      return *this;
    }

    riterator &operator=(riterator const &b)
    {
      if (this != &b)
      {
        cursor = b.cursor;
        instance = b.instance;
      }
      return *this;
    }

    T &operator*() { return (cursor->obj); }

    bool checkInstance(List<T> *tgt)
    {
      return (tgt == instance) ? true : false;
    }

  private:
    ListNode<T> *cursor;
    List<T> *instance;
  };

  List() : items(0)
  {
    head = new ListNode<T>();
    tail = new ListNode<T>();
    head->linkNext(tail);
  }

  ~List()
  {
    clear();
    // Great we did not mess up
    if (head && tail)
    {
      delete head;
      delete tail;
      head = tail = nullptr;
    }
  }

  unsigned count(void) { return items; }

  bool empty(void) { return (items == 0); }

  // Link the new item to the tail of head, so head is always first in line
  void addHead(T newObject)
  {
    ListNode<T> *temp = new ListNode<T>(newObject);
    temp->linkPrev(head);
    items += 1;
  }

  // Link the new item to the head of tail, so tail is always last in line
  void addTail(T newObject)
  {
    ListNode<T> *temp = new ListNode<T>(newObject);
    temp->linkNext(tail);
    items += 1;
  }

  // Link the new item next to the target, so the new item is closer to the head
  // than the target.
  // If target is not part of the list return false, else return true.
  bool insert(T newObject, T targetObject)
  {
    // Look for targetObject, if not found return
    ListNode<T> *cursor = head->prev;
    while (cursor != tail)
    {
      if (cursor->obj == targetObject)
        break;
      cursor = cursor->prev;
    }
    if (cursor == tail)
      return false;

    // Link in the new object
    ListNode<T> *temp = new ListNode<T>(newObject);
    temp->linkNext(cursor);
    items += 1;
    return true;
  }

  void popHead(void)
  {
    if (items)
    {
      ListNode<T> *temp = head->prev;
      delete temp;
      items -= 1;
    }
  }

  void popTail(void)
  {
    if (items)
    {
      ListNode<T> *temp = tail->next;
      delete temp;
      items -= 1;
    }
  }

  T &getHead(void) { return (items) ? (head->prev->obj) : (head->obj); }
  T &getTail(void) { return (items) ? (tail->next->obj) : (tail->obj); }

  iterator begin(void) { return iterator(*this); }
  iterator end(void) { return iterator(*this, tail); }
  riterator rbegin(void) { return riterator(*this); }
  riterator rend(void) { return riterator(*this, head); }

  iterator erase(iterator target)
  {
    if (!target.checkInstance(this))
      return end();

    if (target == begin())
    {
      popHead();
      return begin();
    }
    else if (target == end()--)
    {
      popTail();
      return end();
    }
    else if (target != end())
    {
      ListNode<T> *find = head->prev;
      while ((find != tail) && (find->obj != *target))
        find = find->prev;

      target++;
      delete (find);
      std::cout << "ddd " << count() << std::endl;
      items -= 1;
      return target;
    }
    return end();
  }

  void clear(void)
  {
    ListNode<T> *temp;
    while (items--)
    {
      temp = head->prev;
      delete temp;
    }
    items = 0;
  }

private:
  ListNode<T> *head, *tail;
  unsigned items;
};

#endif