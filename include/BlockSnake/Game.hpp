#ifndef INCLUDE_BLOCKSNAKE_GAME_HPP_
#define INCLUDE_BLOCKSNAKE_GAME_HPP_

#include <cstdlib>
#include <ctime>

#include "Coordinate.hpp"
#include "SnakeList.hpp"

class Game
{
   public:
    int height, width, time;
    SnakeList snake;
    Coordinate apple;

    Game(int, int);

    void generateApple();

    // these would be better on SnakeList but I couldn't get that to work well
    bool collideApple();
    bool collideWall();
    bool collideSnake();

    bool iterate(Direction);
};

#endif  // INCLUDE_BLOCKSNAKE_GAME_HPP_
