#include "SnakeList.hpp"

Node::Node(Node* nxt, Node* prv, Coordinate pos)
    : next(nxt), prev(prv), coord(pos) {};

SnakeList::SnakeList(Coordinate start)
{
    head = new Node(nullptr, nullptr, start);
    tail = head;
    length = 1;
}

void SnakeList::push(Node* point)
{
    head->prev = point;
    point->next = head;
    head = point;
    length++;
}

void SnakeList::pop()
{
    if (!tail)
        return;
    
    tail = tail->prev;
    delete (tail->next);
    tail->next = nullptr;
    length--;
}

bool SnakeList::move(Direction dir)
{
    Node* top = new Node(nullptr, nullptr, head->coord);

    switch (dir) {
        case Left:
            top->coord = head->coord.Left();
            push(top);
            break;

        case Right:
            top->coord = head->coord.Right();
            push(top);
            break;

        case Up:
            top->coord = head->coord.Up();
            push(top);
            break;

        case Down:
            top->coord = head->coord.Down();
            push(top);
            break;

        default:
            return false;
            break;
    }

    // if dir was a valid dirction then the default case wouldn't run and it would have been a succes so return true
    return true;
}

std::ostream& operator<<(std::ostream& os, SnakeList &list)
{
    Node* cur = list.head;
    if (!cur)
        return os << "Empty SnakeList";
    
    os << cur->coord;
    while (cur != list.tail)
    {
        cur = cur->next;
        os << ", " << cur->coord;
    }

    return os;
}
