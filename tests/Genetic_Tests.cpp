#include <gtest/gtest.h>

#include "Genetic.hpp"

TEST(GeneticTests, print)
{
    int history = 0;
    Genetic::Network test = Genetic::Network(&history);
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
    int history = 0;
    Genetic::Network test = Genetic::Network(&history);
    test.addConnection(test.nodes.head->data->head->data,
                       test.nodes.tail->data->head->data, 0.5);

    float* input = new float(0.75);
    LL::LinkedList<float> inputs(input);
    LL::LinkedList<float> outputs = test.compute(inputs);

    EXPECT_FLOAT_EQ(0.5 * 0.75, *(outputs.head->data));
}

TEST(GeneticTests, nodeLists)
{
    int history = 0;
    Genetic::Network test = Genetic::Network(&history);

    Genetic::Node* inNode = test.nodes.head->data->head->data;
    Genetic::Node* outNode = test.nodes.tail->data->head->data;
    test.addConnection(inNode, outNode, 0.5);

    EXPECT_EQ(inNode->out.head->data, outNode->in.head->data);
}

TEST(GeneticTests, networkI1C2O1)
{
    int history = 0;
    Genetic::Network test = Genetic::Network(&history);
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
    int history = 0;
    Genetic::Network test = Genetic::Network(&history, 2, 1);
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
    int history = 0;
    Genetic::Network test = Genetic::Network(&history);
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
    int history = 0;
    Genetic::Network test = Genetic::Network(&history);
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
    int history = 0;
    Genetic::Network test = Genetic::Network(&history);
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

TEST(GeneticTests, population)
{
    // go with 1000 for a realistic size
    const int pop_size = 10;
    int history = 0;
    Genetic::Network* population[pop_size];

    for (int i = 0; i < pop_size; i++)
    {
        population[i] = new Genetic::Network(&history, 1, 1);
        for (int j = 0; j < 50; j++)
        {
            population[i]->mutate();
        }
    }

    // for (int i = 0; i < pop_size; i++)
    // {
    //      std::cout << i << std::endl;
    //      std::cout << *population[i] << std::endl;
    // }

    float* data = new float(0.5);
    LL::LinkedList<float> input(data);
    // a typical game lasts 5000 steps
    for (int j = 0; j < 50; j++)
        for (int i = 0; i < pop_size; i++)
        {
            population[i]->compute(input);
        }

    // std::cout << *population[0] << std::endl;
    // std::cout << *population[1] << std::endl;
}
