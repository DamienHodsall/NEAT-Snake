#include <gtest/gtest.h>

#include "Coordinate.hpp"
#include "SnakeList.hpp"

TEST(SnakeListTests, single_value_boundary)
{
    SnakeList list(Coordinate(3,4));

    ASSERT_EQ(list.head, list.tail);
}
