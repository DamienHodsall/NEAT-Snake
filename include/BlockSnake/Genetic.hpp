#ifndef INCLUDE_BLOCKSNAKE_GENETIC_HPP_
#define INCLUDE_BLOCKSNAKE_GENETIC_HPP_

#include <cstdlib>
#include <ctime>

#include "LinkedList.cpp"

namespace Genetic
{

class Connection;

class Node
{
   public:
    int innovation;
    LL::LinkedList<Connection> in;
    LL::LinkedList<Connection> out;
    float bias;
    float activation;

    Node(int innovation, float bias, Connection* ini, Connection* outi);
    Node(int innovation, float bias, Connection* outi);
    Node(int innovation, Connection* ini, float bias);
    Node(int innovation, float bias);

    float activate();

    friend std::ostream& operator<<(std::ostream&, Node&);
};

class Connection
{
   public:
    int innovation;
    int layer;
    Node* in;
    Node* out;
    bool enabled;
    float weight;

    Connection(int innov, int layer, float weight, Node* ini, Node* outi);

    friend std::ostream& operator<<(std::ostream&, Connection&);
};

class Network
{
   public:
    LL::LinkedList<LL::LinkedList<Node>> nodes;
    LL::LinkedList<LL::LinkedList<Connection>> connections;
    int* innovation;

    Network(int*);
    Network(int*, int, int);

    void addNode(Connection*);
    void addConnection(Node*, Node*, float);
    void mutate();
    LL::LinkedList<float> compute(LL::LinkedList<float>);

    friend std::ostream& operator<<(std::ostream&, Network&);
};

}  // namespace Genetic

#endif  // INCLUDE_BLOCKSNAKE_GENETIC_HPP_
