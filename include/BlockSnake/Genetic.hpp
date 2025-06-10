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

class Event
{
   public:
    int innovation;
    int a;
    int b;
    float* weight;

    Event(Node*);
    Event(Connection*);

    friend std::ostream& operator<<(std::ostream&, Event&);
};

class Network
{
   public:
    LL::LinkedList<LL::LinkedList<Node>> nodes;
    LL::LinkedList<LL::LinkedList<Connection>> connections;
    int* innovation;
    LL::LinkedList<Event> genome;

    Network(int*);
    Network(int*, int, int);
    Network(const Network&);

    void addNode(Connection*);
    void addNode(Connection*, float);
    void addConnection(Node*, Node*, float);
    void mutate();
    LL::LinkedList<float> compute(LL::LinkedList<float>);

    friend std::ostream& operator<<(std::ostream&, Network&);
};

class Population
{
   public:
    int innovation;
    LL::LinkedList<Event> history;
    LL::LinkedList<Network> nets;

    Population(int, int, int);
    Population(int, Network*);
};

}  // namespace Genetic

#endif  // INCLUDE_BLOCKSNAKE_GENETIC_HPP_
