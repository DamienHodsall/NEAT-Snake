#include "Game.hpp"

Coordinate::Coordinate(int xi, int yi) : x(xi), y(yi) {};

Coordinate::Coordinate(Direction dir)
{
    switch (dir)
    {
        case Left:
            x = -1;
            y = 0;
            break;
        case Right:
            x = 1;
            y = 0;
            break;
        case Up:
            x = 0;
            y = -1;
            break;
        case Down:
            x = 0;
            y = 1;
            break;
        default:
            x = 0;
            y = 0;
            break;
    }
}

std::ostream& operator<<(std::ostream& os, Coordinate& coord)
{
    os << "(" << coord.x << "," << coord.y << ")";
    return os;
};

bool operator==(Coordinate lhs, Coordinate rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
};

Coordinate operator+(Coordinate& lhs, Coordinate& rhs)
{
    return Coordinate(lhs.x + rhs.x, lhs.y + rhs.y);
};

Coordinate operator+(Coordinate& lhs, Coordinate rhs)
{
    return Coordinate(lhs.x + rhs.x, lhs.y + rhs.y);
};

bool Snake::move(Direction dir)
{
    if (dir == none)
        return false;

    emplace_front(front() + dir);

    return true;
};

Game::Game(int w, int h)
    : width(w), height(h), time(0), snake(), apple(-1, -1), next_apple(0)
{
    snake.emplace_front(width / 2 - 1, height / 2);
    snake.move(Right);
    snake.move(Right);
};

void Game::generateApple()
{
    apple = Coordinate(rand() % width, rand() % height);

    while (collideApple())
        apple = Coordinate(rand() % width, rand() % height);
};

bool Game::collideApple()
{
    for (Coordinate segment : snake)
        if (segment == apple)
            return true;

    return false;
};

bool Game::collideWall(Coordinate head)
{
    return (head.x < 0) || (head.x >= width) || (head.y < 0) ||
           (head.y >= height);
};

bool Game::collideWall()
{
    return collideWall(snake.front());
}

bool Game::collideSnake(Coordinate head)
{
    Snake::iterator segment = snake.begin();
    while (++segment != snake.end())
        if (head == *segment)
            return true;

    return false;
};

bool Game::collideSnake()
{
    return collideSnake(snake.front());
}

bool Game::iterate(Direction action)
{
    bool moved = snake.move(action);

    if (snake.front() == apple)
    {
        apple = Coordinate(-1, -1);
        next_apple = time + 3;
    }
    else
    {
        snake.pop_back();
    }

    if ((time > next_apple) && (apple == Coordinate(-1, -1)))
        generateApple();

    time++;

    return moved && !collideWall() && !collideSnake();
}
