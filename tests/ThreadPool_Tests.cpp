#include <gtest/gtest.h>
#include <future>
#include <list>

#include "ThreadPool.hpp"

template <typename T>
std::ostream& operator<<(std::ostream& os, std::list<T>& l)
{
    if (l.empty())
        return os << "[]";

    os << "[";

    typename std::list<T>::iterator it = l.begin();
    os << *it;
    it++;
    for (; it != l.end(); it++)
    {
        os << "," << *it;
    }

    os << "]";

    return os;
}

TEST(ThreadPoolTests, gfgexample)
{
    // Create a thread pool with 4 threads
    ThreadPool pool(4);

    // Enqueue tasks for execution
    for (int i = 0; i < 5; i++)
    {
        pool.enqueue(
            [i]
            {
                std::cout << "Task " << i << " is running on thread "
                          << std::this_thread::get_id() << std::endl;
                // Simulate some work
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            });
    }
}

// so this is kinda useless huh...
// void comp_func(int in)
// {
//     std::cout << in << std::endl;
// }
//
// TEST(ThreadPoolTests, biggoExample)
// {
//     ThreadPool pool;
//
//     for (int i = 0; i < 20; i++)
//     {
//         pool.enqueue(comp_func);
//     }
// }

float func(float in)
{
    std::cout << "in: " << in << " threadId: " << std::this_thread::get_id()
              << std::endl;
    return -1.0 * in;
}

TEST(ThreadPoolTests, async_future)
{
    std::list<std::future<float>> data;

    std::list<float> inputs;
    std::list<float> outputs;

    for (int i = 0; i < 30; i++)
        inputs.push_back((float)i / 10.0);

    for (float input : inputs)
    {
        data.emplace_back(std::async(&func, input));
    }

    std::cout << "all in threads" << std::endl;

    for (auto& dat : data)
    {
        outputs.emplace_back(dat.get());
    }

    std::cout << outputs << std::endl;
}
