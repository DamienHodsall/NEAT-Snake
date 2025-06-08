#include <gtest/gtest.h>

#include "Coordinate.hpp"
#include "LinkedList.cpp"

TEST(LinkedListTests, integers)
{
    int data[5] = {6, 9, 4, 2, 0};
    LL::LinkedList<int> test(&data[0]);

    test.push_back(&data[1]);
    test.push_front(&data[3]);
    test.push_front(&data[2]);
    test.push_back(&data[4]);
    test.pop_back();

    LL::Node<int>* cur = test.head;

    do
    {
        std::cout << *(cur->data) << std::endl;
    } while (cur = cur->next);
};

TEST(LinkedListTests, coordinates)
{
    Coordinate coord1(1, 2);
    Coordinate coord2(2, 2);
    Coordinate coord3(3, 2);
    Coordinate coord4(4, 2);

    LL::LinkedList<Coordinate> test(&coord1);

    test.push_back(&coord2);
    test.push_front(&coord3);
    test.pop_back();
    test.push_back(&coord4);
    LL::Node<Coordinate>* cur = test.head;

    do
    {
        std::cout << *(cur->data) << std::endl;
    } while (cur = cur->next);
}

TEST(LinkedListTests, contains)
{
    Coordinate c1(1, 0);
    Coordinate c2(2, 0);
    Coordinate c3(3, 0);
    Coordinate c4(4, 0);

    LL::LinkedList<Coordinate> test(&c1);

    test.push_back(&c2);
    test.push_front(&c3);

    EXPECT_TRUE(test.contains(&c1));
    EXPECT_TRUE(test.contains(&c2));
    EXPECT_TRUE(test.contains(&c3));
    EXPECT_FALSE(test.contains(&c4));
}

TEST(LinkedListTests, empty)
{
    LL::LinkedList<float> test(nullptr);

    float tmps[2] = {0.5, 0.6};

    for (int i = 0; i < 2; i++)
    {
        float* tmp = new float(tmps[i]);
        test.push_back(tmp);
    }

    EXPECT_FLOAT_EQ(*(test.head->data), 0.5);
    EXPECT_FLOAT_EQ(*(test.head->next->data), 0.6);
}

TEST(LinkedListTests, insert_in_order)
{
    LL::LinkedList<float> test(nullptr);

    float tmps[3] = {0.5, 0.6, 0.7};

    for (int i = 0; i < 3; i++)
    {
        float* tmp = new float(tmps[i]);
        test.insert(tmp, i);
    }

    EXPECT_FLOAT_EQ(*(test.head->data), 0.5);
    EXPECT_FLOAT_EQ(*(test.head->next->data), 0.6);
    EXPECT_FLOAT_EQ(*(test.head->next->next->data), 0.7);
}

TEST(LinkedListTests, insert_out_of_order)
{
    LL::LinkedList<float> test(nullptr);

    float tmps[3] = {0.5, 0.6, 0.7};

    for (int i = 0; i < 3; i++)
    {
        float* tmp = new float(tmps[i]);
        test.insert(tmp, i % 2);
    }

    EXPECT_FLOAT_EQ(*(test.head->data), 0.7);
    EXPECT_FLOAT_EQ(*(test.head->next->data), 0.5);
    EXPECT_FLOAT_EQ(*(test.head->next->next->data), 0.6);
}

TEST(LinkedListTests, delete_node)
{
    LL::LinkedList<float> test(nullptr);

    float tmps[3] = {0.5, 0.6, 0.7};

    for (int i = 0; i < 3; i++)
    {
        float* tmp = new float(tmps[i]);
        test.push_back(tmp);
    }

    test.remove(test.head);

    EXPECT_FLOAT_EQ(*(test.head->data), 0.6);
    EXPECT_FLOAT_EQ(*(test.head->next->data), 0.7);

    std::cout << test << std::endl;
}

TEST(LinkedListTests, print)
{
    LL::LinkedList<float> test(nullptr);
    std::cout << test << std::endl;

    float tmps[3] = {0.5, 0.6, 0.7};

    for (int i = 0; i < 3; i++)
    {
        float* tmp = new float(tmps[i]);
        test.push_back(tmp);
    }

    std::cout << test << std::endl;
}

TEST(LinkedListTests, printCoordinate)
{
    LL::LinkedList<Coordinate> test(nullptr);

    Coordinate c1(1, 0);
    Coordinate c2(2, 0);
    Coordinate c3(3, 0);
    Coordinate c4(4, 0);

    test.push_back(&c1);
    test.push_back(&c2);
    test.push_back(&c3);
    test.push_back(&c4);

    std::cout << test << std::endl;
}

TEST(LinkedListTests, printList)
{
    LL::LinkedList<LL::LinkedList<float>> test(nullptr);

    float tmps[3][3] = {{0.5, 0.6, 0.7}, {1.5, 1.6, 1.7}, {2.5, 2.6, 2.7}};

    for (int i = 0; i < 3; i++)
    {
        LL::LinkedList<float>* tmp = new LL::LinkedList<float>(nullptr);
        for (int j = 0; j < 3; j++)
        {
            float* tmpf = new float(tmps[i][j]);
            tmp->push_back(tmpf);
        }
        test.push_back(tmp);
    }

    std::cout << test << std::endl;
}

TEST(LinkedListTests, emptyConstructor)
{
    LL::LinkedList<float> test = LL::LinkedList<float>();

    float* tmp = new float(0.3);
    test.push_back(tmp);
    tmp = new float(0.6);
    test.push_front(tmp);
    // k so don't do this or it clears the list?
    // test.push_front(nullptr);
    // ond really don't do this cause it segfaults???
    // test.push_back(nullptr);

    std::cout << test << " " << test.length << std::endl;
}

TEST(LinkedListTests, get)
{
    LL::LinkedList<float> test(nullptr);

    float tmps[3] = {0.5, 0.6, 0.7};

    for (int i = 0; i < 3; i++)
    {
        float* tmp = new float(tmps[i]);
        test.push_back(tmp);
    }

    EXPECT_EQ(test.head->next, test.get(1));
}

TEST(LinkedListTests, rangeBasedLoop)
{
    LL::LinkedList<float> test;

    float tmps[3] = {0.5, 0.6, 0.7};

    for (int i = 0; i < 3; i++)
    {
        float* tmp = new float(tmps[i]);
        test.push_back(tmp);
    }

    int i = 0;
    for (float& val : test)
        EXPECT_FLOAT_EQ(tmps[i++], val);
}
