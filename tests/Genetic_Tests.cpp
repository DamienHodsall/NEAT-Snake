#include <gtest/gtest.h>

#include "Genetic.hpp"

TEST(GeneticTests, print)
{
    int innovation = 0;
    LL::LinkedList<Genetic::Event> history;
    Genetic::Network test = Genetic::Network(&innovation, &history);
    Genetic::Node* inNode = test.nodes.head->data->head->data;
    Genetic::Node* outNode = test.nodes.tail->data->head->data;
    test.addConnection(inNode, outNode, 0.5);
    test.addConnection(inNode, outNode, 0.75);
    test.addNode(test.connections.head->data->head->data);
    test.addNode(test.connections.head->data->head->next->data);

    std::cout << test;
}

TEST(GeneticTests, simple)
{
    int innovation = 0;
    LL::LinkedList<Genetic::Event> history;
    Genetic::Network test = Genetic::Network(&innovation, &history);
    test.addConnection(test.nodes.head->data->head->data,
                       test.nodes.tail->data->head->data, 0.5);

    float* input = new float(0.75);
    LL::LinkedList<float> inputs(input);
    LL::LinkedList<float> outputs = test.compute(inputs);

    EXPECT_FLOAT_EQ(0.5 * 0.75, *(outputs.head->data));
}

TEST(GeneticTests, nodeLists)
{
    int innovation = 0;
    LL::LinkedList<Genetic::Event> history;
    Genetic::Network test = Genetic::Network(&innovation, &history);

    Genetic::Node* inNode = test.nodes.head->data->head->data;
    Genetic::Node* outNode = test.nodes.tail->data->head->data;
    test.addConnection(inNode, outNode, 0.5);

    EXPECT_EQ(inNode->out.head->data, outNode->in.head->data);
}

TEST(GeneticTests, networkI1C2O1)
{
    int innovation = 0;
    LL::LinkedList<Genetic::Event> history;
    Genetic::Network test = Genetic::Network(&innovation, &history);
    test.addConnection(test.nodes.head->data->head->data,
                       test.nodes.tail->data->head->data, 0.5);
    test.addConnection(test.nodes.head->data->head->data,
                       test.nodes.tail->data->head->data, 0.75);

    float* input = new float(0.75);
    LL::LinkedList<float> inputs(input);
    LL::LinkedList<float> outputs = test.compute(inputs);

    EXPECT_FLOAT_EQ((0.5 + 0.75) * 0.75, *(outputs.head->data));
}

TEST(GeneticTests, networkI2C1O1)
{
    int innovation = 0;
    LL::LinkedList<Genetic::Event> history;
    Genetic::Network test = Genetic::Network(&innovation, &history, 2, 1);
    test.addConnection(test.nodes.head->data->head->data,
                       test.nodes.tail->data->head->data, 0.5);
    test.addConnection(test.nodes.head->data->head->next->data,
                       test.nodes.tail->data->head->data, 0.90);

    float* input = new float(0.75);
    LL::LinkedList<float> inputs(input);
    float* tmp = new float(0.25);
    inputs.push_back(tmp);
    LL::LinkedList<float> outputs = test.compute(inputs);

    EXPECT_FLOAT_EQ(0.75 * 0.5 + 0.25 * 0.90, *(outputs.head->data));
}

TEST(GeneticTests, networkI1C1H1C1O1)
{
    int innovation = 0;
    LL::LinkedList<Genetic::Event> history;
    Genetic::Network test = Genetic::Network(&innovation, &history);
    test.addConnection(test.nodes.head->data->head->data,
                       test.nodes.tail->data->head->data, 0.25);
    test.addNode(test.connections.head->data->head->data);

    float* input = new float(0.90);
    LL::LinkedList<float> inputs(input);
    LL::LinkedList<float> outputs = test.compute(inputs);

    EXPECT_FLOAT_EQ(0.90 * 0.25, *(outputs.head->data));
}

TEST(GeneticTests, networkI1C1H2C1O1)
{
    int innovation = 0;
    LL::LinkedList<Genetic::Event> history;
    Genetic::Network test = Genetic::Network(&innovation, &history);
    test.addConnection(test.nodes.head->data->head->data,
                       test.nodes.tail->data->head->data, 0.25);
    test.addNode(test.connections.head->data->head->data);
    test.addConnection(test.nodes.head->data->head->data,
                       test.nodes.head->next->data->head->data, 0.35);
    test.addNode(test.connections.head->data->head->next->data);

    float* input = new float(0.90);
    LL::LinkedList<float> inputs(input);
    LL::LinkedList<float> outputs = test.compute(inputs);

    EXPECT_FLOAT_EQ(0.90 * 0.35 + 0.90 * 0.25, *(outputs.head->data));
}

TEST(GeneticTests, mutation)
{
    int innovation = 0;
    LL::LinkedList<Genetic::Event> history;
    Genetic::Network test = Genetic::Network(&innovation, &history);
    test.addConnection(test.nodes.head->data->head->data,
                       test.nodes.tail->data->head->data, 0.25);
    std::cout << test.nodes.length << std::endl;

    for (int i = 0; i < 1000; i++)
        test.mutate();

    float* data = new float(0.5);
    LL::LinkedList<float> input(data);
    std::cout << test << std::endl;

    LL::Node<LL::LinkedList<Genetic::Node>>* cur = test.nodes.head;
    std::cout << cur->data->length;
    while (cur = cur->next)
    {
        std::cout << ", " << cur->data->length;
    }
    std::cout << std::endl;
    // LL::LinkedList<float> output = test.compute(input);
    // std::cout << output << std::endl;
}

TEST(GeneticTests, copy)
{
    int innovation = 0;
    LL::LinkedList<Genetic::Event> history;
    Genetic::Network test = Genetic::Network(&innovation, &history);
    Genetic::Network copy(test);

    for (int i = 0; i < 64; i++)
        test.mutate();

    for (int i = 0; i < 64; i++)
        copy.mutate();

    LL::LinkedList<float> input(new float(0.5));
    copy.compute(input);

    std::cout << test << std::endl;
    std::cout << copy << std::endl;
}

TEST(GeneticTests, naivePopulation)
{
    // go with 1000 for a realistic size
    const int pop_size = 50;
    int innovation = 0;
    LL::LinkedList<Genetic::Event> history;
    Genetic::Network* population[pop_size];
    population[0] = new Genetic::Network(&innovation, &history, 3, 4);
    for (int i = 1; i < pop_size; i++)
        population[i] = new Genetic::Network(*population[0]);

    LL::LinkedList<float> input(new float(0.5));
    input.push_back(new float(0.6));
    input.push_back(new float(0.7));

    // 150 generations
    for (int i = 0; i < 50; i++)
    {
        for (int j = 0; j < pop_size; j++)
        {
            // a typical game lasts 5000 steps
            for (int k = 0; k < 500; k++)
            {
                population[j]->compute(input);
                // std::cout << i << " " << j << " " << k << std::endl;
            }
            population[j]->mutate();
        }
    }

    // for (int i = 0; i < pop_size; i++)
    // {
    //      std::cout << i << std::endl;
    //      std::cout << *population[i] << std::endl;
    // }

    for (int i = 0; i < pop_size; i++)
        std::cout << population[i]->genome.length << std::endl;
    // std::cout << *population[6] << std::endl;

    // std::cout << *population[0] << std::endl;
    // std::cout << *population[1] << std::endl;
}
