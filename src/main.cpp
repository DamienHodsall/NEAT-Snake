#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Coordinate.hpp"
#include "Game.hpp"
#include "Genetic.hpp"
#include "SnakeList.hpp"

int main()
{
    int seed = time(0);
    // int seed = 1746775168;
    // int seed = 1746831318;
    std::cout << seed << std::endl;
    srand(seed);

    Game test(8, 8);
    Direction instructions[6] = {Up, Left, Down, Down, Left, Left};
    // test.apple = Coordinate(6, 3);
    // test.snake.move(Right);

    std::cout << test.snake << "; " << test.apple << std::endl;

    while ((test.time < 6) && test.iterate(instructions[test.time]))
    {
         std::cout << test.snake << "; " << test.apple << std::endl;
    }
}
