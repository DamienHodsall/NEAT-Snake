#include "Coordinate.hpp"

Coordinate::Coordinate(int xi, int yi) : x(xi), y(yi) {};

Coordinate* Coordinate::Left()
{
    return new Coordinate(x - 1, y);
}

Coordinate* Coordinate::Right()
{
    return new Coordinate(x + 1, y);
}

Coordinate* Coordinate::Up()
{
    return new Coordinate(x, y - 1);
}

Coordinate* Coordinate::Down()
{
    return new Coordinate(x, y + 1);
}

std::ostream& operator<<(std::ostream& os, Coordinate& coord)
{
    os << "(" << coord.x << "," << coord.y << ")";
    return os;
}

bool operator==(Coordinate& left, Coordinate& right)
{
    return (left.x == right.x) && (left.y == right.y);
}
