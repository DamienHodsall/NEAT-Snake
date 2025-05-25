#ifndef INCLUDE_BLOCKSNAKE_SNAKELIST_HPP_
#define INCLUDE_BLOCKSNAKE_SNAKELIST_HPP_

#include <ostream>

#include "Coordinate.hpp"

enum Direction
{
    None,
    Left,
    Right,
    Up,
    Down
};

class Node
{
   public:
    Node(Node*, Node*, Coordinate);

    Coordinate coord;
    Node* next;
    Node* prev;
};

class SnakeList
{
   public:
    Node* head;
    Node* tail;
    int length;

    SnakeList(Coordinate);

    void push(Node*);
    void pop();

    bool move(Direction);

    friend std::ostream& operator<<(std::ostream&, SnakeList&);
};

#endif  // INCLUDE_BLOCKSNAKE_SNAKELIST_HPP_
