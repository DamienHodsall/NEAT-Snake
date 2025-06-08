#ifndef INCLUDE_BLOCKSNAKE_COORDINATE_HPP_
#define INCLUDE_BLOCKSNAKE_COORDINATE_HPP_

#include <ostream>

class Coordinate
{
   public:
    int x, y;

    Coordinate(int, int);

    Coordinate* Left();
    Coordinate* Right();
    Coordinate* Up();
    Coordinate* Down();

    friend std::ostream& operator<<(std::ostream&, Coordinate&);
    friend bool operator==(Coordinate&, Coordinate&);
};

#endif  // INCLUDE_BLOCKSNAKE_COORDINATE_HPP_
