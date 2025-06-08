#ifndef INCLUDE_BLOCKSNAKE_LINKEDLIST_HPP_
#define INCLUDE_BLOCKSNAKE_LINKEDLIST_HPP_

// sorry about the cursed cpp in the includes...
// I was having some linker/cmake issues and this worked

#include <ostream>

namespace LL
{

template <typename T>
class Node
{
   public:
    T* data;
    Node<T>* next;
    Node<T>* prev;

    Node(Node<T>*, T*, Node<T>*);
    Node(Node<T>*, T*);
    Node(T*);
};

template <typename T>
class LinkedList
{
   public:
    Node<T>* head;
    Node<T>* tail;
    int length;

    LinkedList();
    LinkedList(T*);
    LinkedList(const LinkedList<T>&);
    void push_front(T*);
    void push_back(T*);
    void insert(T*, int);
    void pop_front();
    void pop_back();
    void remove(Node<T>*);
    bool contains(T*);
    Node<T>* get(int);

    friend std::ostream& operator<<(std::ostream& os, LinkedList& list)
    {
        if (!list.head->data)
            return os << "[]";

        Node<T>* cur = list.head;
        os << "[";
        os << *(cur->data);
        while (cur = cur->next)
        {
            os << "," << *(cur->data);
        }
        os << "]";

        return os;
    }

    // gross iterator stuff
    class Iterator
    {
       public:
        Iterator(Node<T>* node) : cur(node) {};

        T& operator*() { return *cur->data; };

        Iterator& operator++()
        {
            cur = cur->next;
            return *this;
        }

        bool operator!=(const Iterator& rhs) const { return cur != rhs.cur; }

       private:
        Node<T>* cur;
    };

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }
};

// implementation here cause linking bull

template <typename T>
Node<T>::Node(Node<T>* prv, T* dat, Node<T>* nxt)
    : prev(prv), data(dat), next(nxt){};
template <typename T>
Node<T>::Node(Node<T>* prv, T* dat) : prev(prv), data(dat), next(nullptr){};
template <typename T>
Node<T>::Node(T* dat) : prev(nullptr), data(dat), next(nullptr){};

template <typename T>
LinkedList<T>::LinkedList()
{
    head = new Node<T>(nullptr);
    tail = head;
    length = 0;
}

template <typename T>
LinkedList<T>::LinkedList(T* start)
{
    head = new Node<T>(start);
    tail = head;
    length = 1;
};

template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& rhs)
{
    Node<T>* cur = rhs.head;
    head = new Node<T>(new T(*cur->data));
    tail = head;
    length = 1;

    while (cur = cur->next)
        push_back(new T(*cur->data));
}

template <typename T>
void LinkedList<T>::push_front(T* data)
{
    Node<T>* data_node = new Node<T>(data);
    if (head->data)
    {
        head->prev = data_node;
        data_node->next = head;
        head = data_node;
    }
    else
    {
        head = data_node;
        tail = head;
    }
    length++;
};

template <typename T>
void LinkedList<T>::push_back(T* data)
{
    Node<T>* data_node = new Node<T>(data);
    if (tail->data)
    {
        tail->next = data_node;
        data_node->prev = tail;
        tail = data_node;
    }
    else
    {
        tail = data_node;
        head = tail;
    }
    length++;
};

template <typename T>
void LinkedList<T>::insert(T* data, int pos)
{
    Node<T>* cur = head;
    if (head->data)
    {
        int count = 0;
        while ((cur->next) && (count < pos))
        {
            cur = cur->next;
            count++;
        }

        if (count < pos)
        {
            push_back(data);
            return;
        }

        if (cur->prev)
        {
            Node<T>* data_node = new Node<T>(cur->prev, data, cur);

            cur->prev->next = data_node;
            cur->prev = data_node;
        }
        else
        {
            push_front(data);
        }
    }
    else
    {
        Node<T>* data_node = new Node<T>(data);
        head = data_node;
        tail = head;
    }
    length++;
}

template <typename T>
void LinkedList<T>::pop_front()
{
    if (!head->data)
        return;

    head = head->next;
    delete (head->prev);
    head->prev = nullptr;
    length--;
};

template <typename T>
void LinkedList<T>::pop_back()
{
    if (!tail->data)
        return;

    tail = tail->prev;
    delete (tail->next);
    tail->next = nullptr;
    length--;
};

template <typename T>
void LinkedList<T>::remove(Node<T>* node)
{
    if (node == head)
        head = node->next;
    else
        node->prev->next = node->next;

    if (node == tail)
        tail = node->prev;
    else
        node->next->prev = node->prev;

    delete (node);
    length--;
}

template <typename T>
bool LinkedList<T>::contains(T* test)
{
    if (!head->data)
        return false;

    Node<T>* cur = head;
    do
    {
        // this "accidently" checks the pointers but it's all pointers anyway
        if (cur->data == test)
            return true;
    } while (cur = cur->next);

    return false;
}

template <typename T>
Node<T>* LinkedList<T>::get(int pos)
{
    Node<T>* cur = head;
    while ((cur != tail) && (pos--))
    {
        cur = cur->next;
    }

    return cur;
}

}  // namespace LL

#endif  // INCLUDE_BLOCKSNAKE_LINKEDLIST_HPP_
