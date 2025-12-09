#include <gtest/gtest.h>
#include <thread>

#include "Game.hpp"
#include "Serial.h"

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

TEST(SerialTests, sendBoo)
{
    unsigned char boo[] = {0xe0, 0x09, 0x00, 0x90, 0x09, 0x00,
                           0xe2, 0x29, 0x55, 0x95, 0x5e, 0x22};
    unsigned char oob[] = {0x22, 0x3d, 0x54, 0xd5, 0x4a, 0x22,
                           0x80, 0x28, 0x04, 0x80, 0x48, 0x03};
    unsigned char test[] = {0x88, 0x08, 0x40, 0x82, 0x08, 0x10,
                            0x80, 0x88, 0x04, 0x80, 0x28, 0x01};
    unsigned char tset[] = {0x40, 0x0a, 0x00, 0x90, 0x08, 0x80,
                            0x84, 0x08, 0x20, 0x81, 0x08, 0x08};
    unsigned char count[] = {0xb7, 0x7b, 0xef, 0xdf, 0xdf, 0xef,
                             0xfb, 0xff, 0x7f, 0xf7, 0xff, 0x80};
    unsigned char icount[] = {0x48, 0x84, 0x10, 0x20, 0x20, 0x10,
                              0x04, 0x00, 0x80, 0x08, 0x00, 0x40};
    unsigned char blank[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    unsigned char bright[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                              0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

    Serial* s = new Serial();
    s->begin("/dev/ttyACM0", B115200);

    s->writeBytes(&boo, 12);

    s->end();
}

TEST(SerialTests, sendGame)
{
    Serial* s = new Serial();
    s->begin("/dev/ttyACM0", B115200);

    Game game(12, 8);
    Direction instructions[] = {Up, Left, Down, Down, Left, Left};

    do
    {
        std::cout << game.time << std::endl;
        std::cout << game.snake << "; " << game.apple << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        int crapFrame[12][8];
        for (int i = 0; i < 12; i++)
            for (int j = 0; j < 8; j++)
                crapFrame[i][j] = 0;

        crapFrame[game.apple.x][game.apple.y] = 1;

        for (Coordinate& segment : game.snake)
            crapFrame[segment.x][segment.y] = 1;

        for (auto& col : crapFrame)
        {
            for (int& pixel : col)
            {
                std::cout << pixel << " ";
            }
            std::cout << std::endl;
        }

        unsigned char frame[12];
        for (int i = 0; i < 12; i++)
            frame[i] = 0;
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 12; j++)
            {
                frame[(12 * i + j) / 8] |= crapFrame[j][i]
                                           << (7 - (12 * i + j) % 8);
            }
        }

        s->writeBytes(&frame, 12);
        std::cout << "got here" << std::endl;

    } while ((game.time < 6) && game.iterate(instructions[game.time]));

    std::cout << game.snake << "; " << game.apple << std::endl;

    s->end();
}
