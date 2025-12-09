#include <list>
#include <thread>

#include "Game.hpp"
#include "Genetic.hpp"

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

float play_snake(Genetic::Network& net, Serial* serialConnection, int period)
{
    Game game(8 + rand() % 8, 8 + rand() % 8);

    if (serialConnection)
    {
        game = Game(12, 8);
    }

    std::list<Direction> directions = {Left, Right, Up, Down};
    int area = game.width * game.height;
    std::list<Direction>::iterator dir;
    std::list<float> parameters;
    std::list<float> output;
    std::list<float>::iterator out;
    std::vector<std::pair<float, Direction>> scores;
    Coordinate head(-1, -1);
    Direction action = Right;
    bool timeout = false;
    float memory = 0;

    do
    {
        if (serialConnection)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(period));
            int crapFrame[12][8];

            for (int i = 0; i < 12; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    crapFrame[i][j] = 0;
                }
            }

            if (!(game.apple == Coordinate(-1, -1)))
                crapFrame[game.apple.x][game.apple.y] = 1;

            for (Coordinate& segment : game.snake)
            {
                crapFrame[segment.x][segment.y] = 1;
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

            serialConnection->writeBytes(&frame, 12);

            std::cout << game.apple << " | " << game.snake << std::endl;
        }

        if (game.time > 3 * area * area)
        {
            std::cout << "timeout" << std::endl;
            timeout = true;
            break;
        }

        parameters.clear();
        head = game.snake.front();

        // appleDistance
        if (game.apple == Coordinate(-1, -1))
            parameters.push_back((float)(abs(head.x - game.apple.x) +
                                         abs(head.y - game.apple.y)) /
                                 (float)area);
        else
            parameters.push_back(0.0);

        // appleDirection
        if (game.apple == Coordinate(-1, -1))
        {
            parameters.push_back(0);
            parameters.push_back(0);
        }
        else
        {
            parameters.push_back((head.x > game.apple.x) -
                                 (head.x < game.apple.x));
            parameters.push_back((head.y > game.apple.y) -
                                 (head.y < game.apple.y));
        }

        // wallDistance
        parameters.push_back((float)head.x / (float)game.width);
        parameters.push_back((float)head.y / (float)game.height);
        parameters.push_back(1 - (float)head.x / (float)game.width);
        parameters.push_back(1 - (float)head.y / (float)game.height);

        // nextToSnake
        for (Direction dir : directions)
        {
            parameters.push_back(game.collideSnake(head + dir));
        }

        // snakeSize
        // parameters.push_back((float)game.snake.size() / (float)area);

        // bias
        parameters.push_back(1.0);

        output = net.compute(parameters);

        scores.clear();

        out = output.begin();
        dir = directions.begin();
        while ((out != output.end()) && (dir != directions.end()))
        {
            scores.emplace_back(*out, *dir);
            out++;
            dir++;
        }

        std::sort(scores.begin(), scores.end(), [](auto& left, auto& right)
                  { return left.first < right.first; });
        action = scores.front().second;
    } while (game.iterate(action));

    float board_percent = (float)game.snake.size() / (float)area;

    if (serialConnection)
    {
        std::cout << "died here: " << game.snake.front() << std::endl;
        return board_percent;
    }

    return board_percent - 0.020 * game.collideSnake() -
           0.020 * game.collideWall() - 0.025 * timeout;
}

float play_snake(Genetic::Network& net)
{
    return play_snake(net, nullptr, 0);
}
