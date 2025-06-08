#include "Game.hpp"

Game::Game(int w, int h) : width(w), height(h), time(0), snake(), apple(-1, -1)
{
    snake.push_front(new Coordinate(width / 2 - 1, height / 2));
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
    for (Coordinate pos : snake)
        if (pos == apple)
            return true;

    return false;
}

bool Game::collideWall()
{
    return ((snake.head->data->x < 0) || (snake.head->data->x >= width) ||
            (snake.head->data->y < 0) || (snake.head->data->y >= height));
}

bool Game::collideSnake()
{
    LL::Node<Coordinate>* cur = snake.head;
    while ((cur = cur->next))
        if (snake.head == cur)
            return true;

    return false;
}

bool Game::iterate(Direction action)
{
    bool moved = snake.move(action);

    if (collideApple())
    {
        generateApple();
    }
    else
    {
        snake.pop_back();
    }

    time++;

    return moved && !collideWall() && !collideSnake();
}

// int Game::run(std::function action_generator): return length
// while (test.iterate(action_generator()))
