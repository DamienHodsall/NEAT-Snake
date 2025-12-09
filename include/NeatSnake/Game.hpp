#ifndef INCLUDE_CHEATSNAKE_GAME_HPP_
#define INCLUDE_CHEATSNAKE_GAME_HPP_

#include <iostream>
#include <list>

enum Direction
{
    none,
    Left,
    Right,
    Up,
    Down
};

class Coordinate
{
   public:
    int x, y;

    Coordinate(int, int);
    Coordinate(Direction dir);

    friend std::ostream& operator<<(std::ostream& os, Coordinate& coord);
    friend bool operator==(Coordinate lhs, Coordinate rhs);
    friend Coordinate operator+(Coordinate& lhs, Coordinate& rhs);
    friend Coordinate operator+(Coordinate& lhs, Coordinate rhs);
};

class Snake : public std::list<Coordinate>
{
   public:
    bool move(Direction dir);
};

class Game
{
   public:
    int width, height, time;
    Snake snake;
    Coordinate apple;

    Game(int width, int height);

    void generateApple();

    bool collideApple();
    bool collideWall(Coordinate head);
    bool collideWall();
    bool collideSnake(Coordinate head);
    bool collideSnake();

    bool iterate(Direction dir);

   private:
    int next_apple;
};

#endif  // INCLUDE_CHEATSNAKE_GAME_HPP_
