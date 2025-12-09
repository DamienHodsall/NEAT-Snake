#include <list>

#include "Genetic.hpp"
#include "play_snake.cpp"

#include "Serial.h"

Serial* serialConnection = new Serial();

int main(int argc, char* argv[])
{
    int seed = time(0);
    std::cout << seed << std::endl;
    srand(seed);

    Genetic::Population championPop(argv[1], 1);
    rmdir(championPop.folder.c_str());
    Genetic::Network& champion = championPop.nets.front();

    std::cout << champion << std::endl;
    champion.visualize("watchme.dot");

    if (serialConnection->begin("/dev/ttyACM0", B115200))
    {
        std::cout << "Connected Successfully!" << std::endl;
    }
    else
    {
        std::cout << "Bad Connection... :(" << std::endl;
        return 0;
    }

    std::cout << play_snake(champion, serialConnection, 50) * 100 << std::endl;

    serialConnection->end();
}
