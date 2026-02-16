#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Genetic.hpp"
#include "play_snake.cpp"

int num_games = 3;

float comp_func(Genetic::Network& net)
{
    float average = 0;

    for (int i = 0; i < num_games; i++)
        average += play_snake(net);

    return average / num_games;
}

int main()
{
    int seed = time(0);
    std::cout << seed << std::endl;  // kinda useless when I'm threading...
    srand(seed);

    // PARAMETERS:
    //      Rectilinear distance to the apple       (0)
    //      apple to the Left/Right/Up/Down         (1,2)
    //      distance to Left/Right/Up/Down wall     (3,4,5,6)
    //      next to snake Left/Right/Up/Down        (7,8,9,10)
    //      bias                                    (11)
    Genetic::Population population(500, 12, 4);
    // Genetic::Population population("champions/2025-09-28 17:38:42/2500.net",
    //                                1000);
    // Genetic::Population population("latest.net", 500);
    population.c3 = 1.5;
    population.ct = 3.0;
    std::cout << "Initialized Population" << std::endl;

    while (population.generation <= 1500)
    {
        std::cout << "\nStarting Generation " << population.generation
                  << " with " << population.species.size() << " species"
                  << std::endl;
        population.mutate();
        population.compete(comp_func);
    }

    Genetic::Network& net = population.nets.back();

    std::string filename("latest.net");

    std::cout << net << std::endl;
    net.save(filename);

    population.save_species();

    // for (auto& spec : population.species)
    //     std::cout << spec.size() << " ";
    // std::cout << "\n" << net << std::endl;
    //
    // if (serialConnection->begin("/dev/ttyACM0", B115200))
    // {
    //     std::cout << "Connected Successfully!" << std::endl;
    // }
    // else
    // {
    //     std::cout << "Bad Connection... :(" << std::endl;
    //     return 0;
    // }
    //
    // // wait for user input
    // // (so I can watch the pretty animation!!!)
    // // std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    // std::string nowhere;
    // std::getline(std::cin, nowhere);
    //
    // show_on_arduino = true;
    // *generation = INT_MAX / 50;
    // play_snake(net);
    //
    // serialConnection->end();
}
