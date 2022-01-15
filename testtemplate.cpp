#include <iostream>
#include <random>
using namespace std;

template <class T>
class ListNode
{
  public:
    ListNode(T object) : next(0), prev(0), obj(object){};
    ListNode(T object, ListNode *head) : obj(object) { addFront(head); }
    ~ListNode() { unlink(); }

    bool isHead(void) { return (next) ? false : true; }
    bool isTail(void) { return (prev) ? false : true; }

    void addFront(ListNode *front)
    {
        if (!front)
            return;

        prev = front;
        next = front->next;
        if (next)
            next->prev = this;
        front->next = this;
    }

    void addBack(ListNode *back)
    {
        if (!back)
            return;

        next = back;
        prev = back->prev;
        if (prev)
            prev->next = this;
        back->prev = this;
    }

    void unlink(void)
    {
        if (next)
            next->prev = prev;
        if (prev)
            prev->next = next;
        next = prev = 0;
    }

    ListNode *getNext(void) { return next; }

    ListNode *getPrev(void) { return prev; }

    T getObject(void) { return obj; }

  private:
    ListNode *next, *prev;
    T obj;
};

template <class T>
class List
{
  public:
    List() : head(0), tail(0), items(0) {};

    ~List()
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
            head = tail = 0;
            items = 0;
        }
    }

    void addTop(T newObject)
    {
        if (!items)
        {
            ListNode<T> *temp = new ListNode<T>(newObject);
            head = tail = temp;
        }
        else
        {
            ListNode<T> *temp = new ListNode<T>(newObject, head);
            head = temp;
        }
        items++;
    }

    T popTop()
    {
        T retval;

        if (items)
        {
            ListNode<T> *temp = head;
            head = head->getPrev();
            retval = temp->getObject();
            delete temp;
            items -= 1;
            if (!items)
                tail = 0;
        }

        return retval;
    }

  private:
    ListNode<T> *head, *tail;
    unsigned items;
};

int main()
{
    ListNode<int> uno(1);
    ListNode<int> due(2, &uno);
    ListNode<int> tre(3, &due);
    ListNode<int> neg(-1, &uno);

    ListNode<int> *head = &uno;

    while (head)
    {
        cout << head->getObject() << ",";
        head = head->getNext();
    }
    cout << endl;

    head = &uno;
    due.unlink();

    while (head)
    {
        cout << head->getObject() << ",";
        head = head->getNext();
    }
    cout << endl;

    List<int> myList;
    myList.addTop(1);
    myList.addTop(3);
    myList.addTop(2);
    myList.addTop(4);

    cout << myList.popTop() << "," << myList.popTop() <<  "," << myList.popTop() <<  "," << myList.popTop() << endl;
}