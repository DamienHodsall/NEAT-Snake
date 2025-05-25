#include "Game.hpp"

Game::Game(int w, int h)
    : width(w),
      height(h),
      time(0),
      snake(Coordinate(width / 2 - 1, height / 2)),
      apple(-1, -1)
{
    snake.move(Right);
    snake.move(Right);
    generateApple();
}

void Game::generateApple()
{
    apple = Coordinate(rand() % width, rand() % height);

    while (collideApple())
        apple = Coordinate(rand() % width, rand() % height);
}

bool Game::collideApple()
{
    // this is the standard form of iteration through the snake
    Node* cur = snake.head;

    do {
        if (cur->coord == apple)
            return true;  // exit the loop early whenever possible
    } while (cur = cur->next);

    return false;
}

bool Game::collideWall()
{
    return ((snake.head->coord.x < 0) || (snake.head->coord.x >= width) ||
            (snake.head->coord.y < 0) || (snake.head->coord.y >= height));
}

bool Game::collideSnake()
{
    Node* cur = snake.head->next;

    do {
        if (cur->coord == snake.head->coord)
            return true;
    } while (cur = cur->next);

    return false;
}

bool Game::iterate(Direction action)
{
    bool moved = snake.move(action);

    if (collideApple())
    {
        generateApple();
    } else {
        snake.pop();
    }

    time++;

    return moved && !collideWall() && !collideSnake();
}

// int Game::run(std::function action_generator): return length
// while (test.iterate(action_generator()))
