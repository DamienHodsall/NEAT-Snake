#ifndef INCLUDE_CHEATSNAKE_GENETIC_HPP_
#define INCLUDE_CHEATSNAKE_GENETIC_HPP_

#include <iostream>
#include <list>

namespace Genetic
{
class Connection;

class Node
{
   public:
    int innovation;
    int layer;
    std::list<Connection*> in;
    std::list<Connection*> out;
    float bias;
    float activation;

    Node(int innovation, int layer, float bias, Connection* ini,
         Connection* outi);
    Node(int innovation, int layer, float bias, Connection* outi);
    Node(int innovation, int layer, Connection* ini, float bias);
    Node(int innovation, int layer, float bias);

    float activate();

    friend std::ostream& operator<<(std::ostream&, Node&);
    friend bool operator==(Node&, Node&);
    friend bool operator==(const Node&, const Node&);
    friend bool operator==(const Node&, Node&);
};

class Connection
{
   public:
    int innovation;
    Node* in;
    Node* out;
    bool enabled;
    float weight;

    Connection(int innovation, float weight, Node* ini, Node* outi);

    friend std::ostream& operator<<(std::ostream&, Connection&);
};

class Event
{
   public:
    int innovation;
    int a;
    int b;

    Event(Node*);
    Event(Connection*);

    bool operator==(Event& other);
    bool operator==(const Event& other);

    friend std::ostream& operator<<(std::ostream&, Event&);
};

class Gene : public Event
{
   public:
    float* weight;

    Gene(Node*);
    Gene(Connection*);

    friend std::ostream& operator<<(std::ostream&, Gene&);
};

class Network
{
   public:
    std::list<std::list<Node>> nodes;
    std::list<std::list<Connection>> connections;
    int* innovation;
    bool protect;
    std::list<Gene> genome;
    std::list<Event>* history;
    std::list<std::list<Network*>>::iterator species;

    Network(int* innovation, std::list<Event>* history);
    Network(int* innovation, std::list<Event>* history, int n_in, int n_out);
    Network(const Network&);

    void addNode(Connection* con);
    void addNode(Connection* con, float weight);
    void addConnection(Node* node_start, Node* node_end, float weight);
    void mutate();
    std::list<float> compute(std::list<float>& input);
    void save(std::string filename);
    void visualize(std::string filename);

    friend std::ostream& operator<<(std::ostream&, Network&);
};

class Population
{
   public:
    int innovation;
    int generation;
    std::string folder;
    std::list<Event> history;
    std::list<Network> nets;
    float c1, c2, c3, ct;
    std::list<std::list<Network*>> species;

    Population(int pop_size, int n_in, int n_out);
    Population(std::string filename, int pop_size);

    void mutate();
    void compete(float (*comp_func)(Network&));

    float compatibility(Network& net1, Network& net2);
    float compatibility(std::list<Gene>& genome1, std::list<Gene>& genome2);

   private:
    std::list<std::list<Gene>> reps;
    void speciate_network(Network* net);
};

}  // namespace Genetic

#endif  // INCLUDE_CHEATSNAKE_GENETIC_HPP_
