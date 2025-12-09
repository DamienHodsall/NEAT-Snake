#include <gtest/gtest.h>
#include <fstream>
#include <numeric>

#include "Genetic.hpp"

template <typename T>
std::ostream& operator<<(std::ostream& os, std::list<T>& l)
{
    if (l.empty())
        return os << "[]";

    os << "[";
    typename std::list<T>::iterator it = l.begin();
    os << *it;
    it = std::next(it);
    while (it != l.end())
    {
        os << "," << *it;
        it = std::next(it);
    }
    os << "]";

    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T>& l)
{
    if (l.empty())
        return os << "[]";

    os << "[";
    typename std::vector<T>::iterator it = l.begin();
    os << *it;
    it = std::next(it);
    while (it != l.end())
    {
        os << "," << *it;
        it = std::next(it);
    }
    os << "]";

    return os;
}

// std::cout << "got here" << std::endl;

TEST(GeneticTests, print)
{
    Genetic::Population population(1, 1, 1);
    Genetic::Network& test = population.nets.front();
    Genetic::Node* inNode = &test.nodes.front().front();
    Genetic::Node* outNode = &test.nodes.back().front();
    population.history.clear();  // fake round of mutation
    std::cout << test << std::endl;

    test.addConnection(inNode, outNode, 0.5);
    test.addNode(&test.connections.front().front());
    population.history.clear();  // fake round of mutation
    test.addConnection(inNode, outNode, 0.75);
    test.addNode(&test.connections.front().back());

    std::cout << test << std::endl;
}

TEST(GeneticTests, compute)
{
    Genetic::Population population(1, 3, 4);
    Genetic::Network& test = population.nets.front();
    std::list<float> input = {0.5, 0.6, 0.7};
    std::list<float> output = test.compute(input);

    std::cout << "before mutation:" << std::endl;
    std::cout << test << std::endl;
    std::cout << output << std::endl;

    for (int i = 0; i < 50; i++)
        population.mutate();

    output = test.compute(input);
    std::cout << "after mutation:" << std::endl;
    std::cout << test << std::endl;
    std::cout << output << std::endl;
}

TEST(GeneticTests, copyNetwork)
{
    // initial population is copies of first member
    Genetic::Population pop(2, 1, 1);
    Genetic::Network& test = pop.nets.front();
    Genetic::Network& copy = pop.nets.back();

    std::cout << test << std::endl;
    std::cout << copy << std::endl;

    for (int i = 0; i < 64; i++)
        pop.mutate();
    std::cout << "got here" << std::endl;

    std::list<float> input = {0.5};
    copy.compute(input);

    std::cout << test << std::endl;
    std::cout << copy << std::endl;
}

TEST(GeneticTests, mutation)
{
    Genetic::Population population(25, 1, 1);

    for (int i = 0; i < 125; i++)
        population.mutate();

    std::cout << population.nets.front() << std::endl;
    std::cout << population.history << std::endl;
    std::cout << population.innovation << std::endl;
}

TEST(GeneticTests, Speciation)
{
    Genetic::Population pop(50, 3, 4);
    pop.ct = 1.0;

    for (int i = 0; i < 25; i++)
    {
        std::cout << pop.history << std::endl;
        std::cout << pop.species << std::endl;
        // std::cout << pop.species.size() << " | " << pop.history << std::endl;

        pop.mutate();
    }

    std::cout << pop.history << std::endl;
    std::cout << pop.species << std::endl;
    // std::cout << pop.species.size() << " | " << pop.history << std::endl;
}

TEST(GeneticTests, PopulationHistory)
{
    Genetic::Population pop(15, 2, 2);

    std::cout << pop.history << std::endl;

    for (int i = 0; i < 10; i++)
    {
        pop.mutate();
        std::cout << pop.history << std::endl;
    }
}

TEST(GeneticTests, Compatibility)
{
    Genetic::Population pop(3, 2, 2);
    Genetic::Network& net1 = pop.nets.front();
    Genetic::Network& net2 = *(++pop.nets.begin());
    Genetic::Network& net3 = pop.nets.back();
    std::cout << pop.compatibility(net1, net2) << std::endl;

    // for (int i = 0; i < 50; i++)
    //     pop.mutate();

    // designed networks that differ by a single connection
    pop.history.clear();
    net1.addConnection(&net1.nodes.front().front(), &net1.nodes.back().front(),
                       0.3);
    net1.addNode(&net1.connections.front().front());
    net1.connections.back().front().weight = 0.4;
    net1.addConnection(&net1.nodes.front().back(), &net1.nodes.back().back(),
                       0.5);
    net1.addConnection(&net1.nodes.front().back(),
                       &(++net1.nodes.begin())->front(), 0.7);

    net2.addConnection(&net2.nodes.front().front(), &net2.nodes.back().front(),
                       0.3);
    net2.addNode(&net2.connections.front().front());
    net2.connections.back().front().weight = 0.4;
    net2.addConnection(&net2.nodes.front().back(), &net2.nodes.back().back(),
                       0.5);

    // random network that just happens to have the same topolopy as net1...
    // and a topology that eventually breaks because mutations are only supposed to happen as a population
    for (int i = 0; i < 40; i++)
        net3.mutate();

    std::cout << pop.history << std::endl;
    std::cout << net1.genome << std::endl;
    std::cout << net2.genome << std::endl;
    std::cout << net3.genome << std::endl;

    for (Genetic::Gene& gene : net1.genome)
        std::cout << gene.innovation << " ";
    std::cout << std::endl;

    for (Genetic::Gene& gene : net2.genome)
        std::cout << gene.innovation << " ";
    std::cout << std::endl;

    for (Genetic::Gene& gene : net3.genome)
        std::cout << gene.innovation << " ";
    std::cout << std::endl;

    std::cout << pop.compatibility(net1, net2) << std::endl;
    std::cout << pop.compatibility(net1, net3) << std::endl;
    std::cout << pop.compatibility(net2, net3) << std::endl;
}

float play_snake(Genetic::Network& net)
{
    std::list<float> input = {0.5, 0.6, 0.7};

    float sum = 0;
    std::list<float> output;
    for (int i = 0; i < 500; i++)
    {
        output = net.compute(input);
        sum += std::accumulate(output.begin(), output.end(), 0.0);
    }

    return sum;
}

TEST(GeneticTests, PopulationClass)
{
    Genetic::Population population(50, 3, 4);
    population.ct = 1.0;

    for (int i = 0; i < 25; i++)
    {
        std::cout << "gen " << i << std::endl;
        std::cout << population.history << std::endl;
        std::cout << population.species << std::endl;
        // for (Genetic::Network& net : population.nets)
        //     std::cout << net;

        population.mutate();

        population.compete(play_snake);
    }

    // std::cout << (float)(population.innovation) / 50.0 << std::endl;
    // std::cout << std::thread::hardware_concurrency() << std::endl;
    // std::cout << population.innovation << std::endl;
    // std::cout << population.history.size() << std::endl;
}

TEST(GeneticTests, PopFromFile)
{
    Genetic::Population pop("../tests/test.net", 5);

    std::cout << pop.nets << std::endl;
}

TEST(GeneticTests, SaveNetwork)
{
    Genetic::Population pop(1, 3, 4);

    for (int i = 0; i < 25; i++)
    {
        pop.mutate();
    }

    pop.nets.front().save("test.net");

    std::cout << pop.nets.front() << std::endl;

    std::ifstream check("test.net");
    std::string line;

    while (std::getline(check, line))
        std::cout << line << std::endl;
}

TEST(GeneticTests, visualizeNetwork)
{
    Genetic::Population pop(1, 2, 2);
    Genetic::Network& net1 = pop.nets.front();

    net1.addConnection(&net1.nodes.front().front(), &net1.nodes.back().front(),
                       0.3);
    net1.addNode(&net1.connections.front().front());
    net1.connections.back().front().weight = 0.4;
    net1.addConnection(&net1.nodes.front().back(), &net1.nodes.back().back(),
                       0.5);
    net1.addConnection(&net1.nodes.front().back(),
                       &(++net1.nodes.begin())->front(), 0.7);

    net1.visualize("test.dot");
}
