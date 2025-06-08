#ifndef INCLUDE_BLOCKSNAKE_SNAKELIST_HPP_
#define INCLUDE_BLOCKSNAKE_SNAKELIST_HPP_

#include "Coordinate.hpp"
#include "LinkedList.cpp"

enum Direction
{
    None,
    Left,
    Right,
    Up,
    Down
};

class SnakeList : public LL::LinkedList<Coordinate>
{
   public:
    bool move(Direction);
};

#endif  // INCLUDE_BLOCKSNAKE_SNAKELIST_HPP_
