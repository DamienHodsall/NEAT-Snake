#include "SnakeList.hpp"

bool SnakeList::move(Direction dir)
{
    Coordinate* top;

    switch (dir)
    {
        case Left:
            top = head->data->Left();
            push_front(top);
            break;

        case Right:
            top = head->data->Right();
            push_front(top);
            break;

        case Up:
            top = head->data->Up();
            push_front(top);
            break;

        case Down:
            top = head->data->Down();
            push_front(top);
            break;

        default:
            return false;
            break;
    }

    // if dir was a valid dirction then the default case wouldn't run and it would have been a succes so return true
    return true;
}
