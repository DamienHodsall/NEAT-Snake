#include "Genetic.hpp"
#include <iostream>

namespace Genetic
{

Node::Node(int innovationi, float biasi, Connection* ini, Connection* outi)
    : innovation(innovationi), bias(biasi), in(ini), out(outi), activation(0) {
      };
Node::Node(int innovationi, float biasi, Connection* outi)
    : innovation(innovationi), bias(biasi), in(), out(outi), activation(0) {};
Node::Node(int innovationi, Connection* ini, float biasi)
    : innovation(innovationi), bias(biasi), in(ini), out(), activation(0) {};
Node::Node(int innovationi, float biasi)
    : innovation(innovationi), bias(biasi), in(), out(), activation(0) {};

float Node::activate()
{
    if (in.length == 0)
        return bias;

    float sum = bias;
    LL::Node<Connection>* cur = in.head;

    do
    {
        sum += cur->data->weight * cur->data->in->activation;
    } while (cur = cur->next);

    activation = std::fmax(sum, 0);

    return activation;
}

std::ostream& operator<<(std::ostream& os, Node& node)
{
    os << "{";
    os << "I:" << node.innovation;
    os << ", b:" << node.bias;
    os << ", i:[";
    if (node.in.length > 0)
    {
        LL::Node<Connection>* ccur = node.in.head;
        os << ccur->data->innovation;
        while (ccur = ccur->next)
        {
            os << "," << ccur->data->innovation;
        }
    }
    os << "]";
    os << ", o:[";
    if (node.out.length > 0)
    {
        LL::Node<Connection>* ccur = node.out.head;
        os << ccur->data->innovation;
        while (ccur = ccur->next)
        {
            os << "," << ccur->data->innovation;
        }
    }
    os << "]";
    os << ", a:" << node.activation;
    os << "}";

    return os;
}

Connection::Connection(int innovationi, int layeri, float weighti, Node* ini,
                       Node* outi)
    : innovation(innovationi),
      layer(layeri),
      weight(weighti),
      in(ini),
      out(outi),
      enabled(true) {};

std::ostream& operator<<(std::ostream& os, Connection& con)
{
    os << "{";
    os << "I:" << con.innovation;
    os << ", l:" << con.layer;
    os << ", i:" << con.in->innovation;
    os << ", o:" << con.out->innovation;
    os << ", e:" << con.enabled;
    os << ", w:" << con.weight;
    os << "}";

    return os;
}

Event::Event(Node* nodei)
    : innovation(nodei->innovation),
      a(-1),
      b(nodei->in.head->data->innovation),
      weight(&nodei->bias)
{
}
Event::Event(Connection* coni)
    : innovation(coni->innovation),
      a(coni->in->innovation),
      b(coni->out->innovation),
      weight(&coni->weight)
{
}

std::ostream& operator<<(std::ostream& os, Event& event)
{
    os << "{";
    os << "I:" << event.innovation;
    os << ", a:" << event.a;
    os << ", b:" << event.b;
    os << ", w:" << *event.weight;
    os << "}";

    return os;
}

Network::Network(int* history)
    : nodes(), connections(), innovation(history), genome()
{
    Node* input = new Node((*innovation)++, 0.0);
    LL::LinkedList<Node>* inlist = new LL::LinkedList<Node>(input);
    nodes.push_back(inlist);

    Node* output = new Node((*innovation)++, 0.0);
    LL::LinkedList<Node>* outlist = new LL::LinkedList<Node>(output);
    nodes.push_back(outlist);

    LL::LinkedList<Connection>* conlist = new LL::LinkedList<Connection>();
    connections.push_back(conlist);
}

Network::Network(int* history, int n_inputs, int n_outputs)
    : nodes(), connections(), innovation(history), genome()
{
    LL::LinkedList<Node>* inlist = new LL::LinkedList<Node>();
    for (int i = 0; i < n_inputs; i++)
    {
        Node* input = new Node((*innovation)++, 0.0);
        inlist->push_back(input);
    }
    nodes.push_back(inlist);

    LL::LinkedList<Node>* outlist = new LL::LinkedList<Node>();
    for (int i = 0; i < n_outputs; i++)
    {
        Node* output = new Node((*innovation)++, 0.0);
        outlist->push_back(output);
    }
    nodes.push_back(outlist);

    LL::LinkedList<Connection>* conlist = new LL::LinkedList<Connection>();
    connections.push_back(conlist);
}

Network::Network(const Network& rhs)
    : nodes(), connections(), innovation(new int(0)), genome()
{
    LL::LinkedList<Node>* inlist = new LL::LinkedList<Node>();
    for (int i = 0; i < rhs.nodes.head->data->length; i++)
    {
        Node* input = new Node((*innovation)++, 0.0);
        inlist->push_back(input);
    }
    nodes.push_back(inlist);

    LL::LinkedList<Node>* outlist = new LL::LinkedList<Node>();
    for (int i = 0; i < rhs.nodes.tail->data->length; i++)
    {
        Node* output = new Node((*innovation)++, 0.0);
        outlist->push_back(output);
    }
    nodes.push_back(outlist);

    LL::LinkedList<Connection>* conlist = new LL::LinkedList<Connection>();
    connections.push_back(conlist);

    bool skip = false;

    if (rhs.genome.length == 0)
    {
        innovation = rhs.innovation;
        return;
    }

    // then addConnection and addNode until finished
    LL::Node<Event>* cureve = rhs.genome.head;
    do
    {
        if (skip)
        {
            skip = false;
        }
        else
        {
            *innovation = cureve->data->innovation;

            if (cureve->data->a < 0)
            {
                // find equivalent Connection in new Network
                bool breaker = false;
                LL::Node<LL::LinkedList<Connection>>* cur = connections.head;
                LL::Node<Connection>* incur = cur->data->head;
                do
                {
                    incur = cur->data->head;
                    do
                    {
                        if (incur->data->innovation == cureve->data->b)
                        {
                            breaker = true;
                            break;
                        }
                    } while (incur = incur->next);
                    if (breaker)
                        break;
                } while (cur = cur->next);

                addNode(incur->data, *cureve->data->weight);
                skip = true;
            }
            else
            {
                // find equivalent in/out Nodes in new Network
                bool breaker = false;
                LL::Node<LL::LinkedList<Node>>* cur = nodes.head;
                LL::Node<Node>* incur = cur->data->head;
                do
                {
                    incur = cur->data->head;
                    do
                    {
                        if (incur->data->innovation == cureve->data->a)
                        {
                            breaker = true;
                            break;
                        }
                    } while (incur = incur->next);
                    if (breaker)
                        break;
                } while (cur = cur->next);

                breaker = false;
                LL::Node<Node>* outcur = cur->data->head;
                do
                {
                    outcur = cur->data->head;
                    do
                    {
                        if (outcur->data->innovation == cureve->data->b)
                        {
                            breaker = true;
                            break;
                        }
                    } while (outcur = outcur->next);
                    if (breaker)
                        break;
                } while (cur = cur->next);

                addConnection(incur->data, outcur->data, *cureve->data->weight);
            }
        }
    } while (cureve = cureve->next);

    innovation = rhs.innovation;
}

void Network::addNode(Connection* con)
{
    addNode(con, 0);
}

void Network::addNode(Connection* con, float weight)
{
    Node* node = new Node((*innovation)++, weight);
    node->in.push_back(con);

    LL::Node<LL::LinkedList<Node>>* cur = nodes.head;
    int layer = 0;

    // find the layer that contains con->in
    while (cur && !(cur->data->contains(con->in)))
    {
        cur = cur->next;
        layer++;
    }

    // move the layer just past con->in
    cur = cur->next;
    layer++;

    // insert the new node into a new layer if the con->in and con->out nodes are sequential
    // otherwise add it to the layer just past the con->in node
    if (cur->data->contains(con->out))
    {
        LL::LinkedList<Node>* nodelist = new LL::LinkedList<Node>(node);
        nodes.insert(nodelist, layer);

        // increment the layer of all connections in layers con->out
        LL::Node<LL::LinkedList<Connection>>* ccur = connections.head;
        LL::Node<Connection>* inccur = ccur->data->head;
        do
        {
            if (ccur->data->head->data->layer >= layer)
            {
                inccur = ccur->data->head;
                do
                {
                    inccur->data->layer++;
                } while (inccur = inccur->next);
            }
        } while (ccur = ccur->next);
    }
    else
    {
        cur->data->push_back(node);
    }

    genome.push_back(new Event(node));

    addConnection(node, con->out, 1.0);

    // remove the old in connection from the con->out node
    // (deletes the pointer not the data)

    LL::Node<Connection>* ccur = con->out->in.head;

    do
    {
        if (ccur->data == con)
        {
            con->out->in.remove(ccur);
            break;
        }
    } while (ccur = ccur->next);

    // modify the connection between con->in and con->out to be con->in-node and make a new connection with weight 1.0 new-node

    con->out = node;
}

void Network::addConnection(Node* input, Node* output, float weight)
{
    LL::Node<LL::LinkedList<Node>>* cur = nodes.head;
    int layer = 0;

    // get the layer of the input node
    while (cur && !(cur->data->contains(input)))
    {
        cur = cur->next;
        layer++;
    }

    Connection* con =
        new Connection((*innovation)++, layer, weight, input, output);

    // add the connection to the output connections of the input node
    LL::Node<Node>* incur = cur->data->head;
    do
    {
        if (incur->data == input)
        {
            incur->data->out.push_back(con);

            break;
        }
    } while (incur = incur->next);

    LL::Node<LL::LinkedList<Connection>>* ccur = connections.head;

    if (!(ccur->data->length > 0))
    {
        ccur->data->push_back(con);
    }
    else
    {
        // add the connection to the correct layer
        do
        {
            if (ccur->data->head->data->layer == layer)
            {  // the 8th layer of hell is this and it's pointers istfg
                ccur->data->push_back(con);

                break;
            }
        } while (ccur = ccur->next);

        // if the it got through all connection lists without finding the layer then make the layer
        if (!ccur)
        {
            LL::LinkedList<Connection>* conlist =
                new LL::LinkedList<Connection>(con);
            connections.push_back(conlist);
        }
    }

    // add the connection to the input connections of the output node
    cur = nodes.tail;
    while (cur && !(cur->data->contains(output)))
    {
        cur = cur->prev;  // traverse to the correct layer
    }

    incur = cur->data->head;

    do
    {
        if (incur->data == output)
        {
            incur->data->in.push_back(con);

            break;
        }
    } while (incur = incur->next);

    genome.push_back(new Event(con));
}

void Network::mutate()
{
    // TODO some random stuff ig
    if (rand() < 0.8 * RAND_MAX)
    {
        // mutate a weight
        if (rand() % 2)
        {
            // mutate a node weight
            int layer = 1 + rand() % (nodes.length - 2);
            LL::Node<LL::LinkedList<Node>>* nodelayer = nodes.get(layer);
            LL::Node<Node>* node =
                nodelayer->data->get(rand() % nodelayer->data->length);
            float dbias = 1.0 - 2.0 * ((0.1 * rand()) / RAND_MAX);
            node->data->bias += dbias;
        }
        else
        {
            // mutate a connection weight
            int layer = rand() % (connections.length);
            LL::Node<LL::LinkedList<Connection>>* conlayer =
                connections.get(layer);
            if (conlayer->data->length > 0)
            {
                LL::Node<Connection>* node =
                    conlayer->data->get(rand() % conlayer->data->length);
                float dweight = 1.0 - 2.0 * ((0.1 * rand()) / RAND_MAX);
                node->data->weight += dweight;
            }
        }
    }

    if (rand() < 0.05 * RAND_MAX)
    {
        // add a connection
        // choose a starting node then choose a random node that is after it
        int start = rand() % (nodes.length - 1);
        LL::Node<LL::LinkedList<Node>>* startlayer = nodes.get(start);
        LL::Node<Node>* startnode =
            startlayer->data->get(rand() % startlayer->data->length);

        int end = start + 1 + rand() % (nodes.length - start - 1);
        LL::Node<LL::LinkedList<Node>>* endlayer = nodes.get(end);
        LL::Node<Node>* endnode =
            endlayer->data->get(rand() % endlayer->data->length);

        addConnection(startnode->data, endnode->data,
                      1.0 - 2.0 * (float)rand() / RAND_MAX);
    }

    if (rand() < 0.03 * RAND_MAX)
    {
        // add a node
        LL::Node<LL::LinkedList<Connection>>* conlayer =
            connections.get(rand() % connections.length);
        if (conlayer->data->length > 0)
        {
            LL::Node<Connection>* con =
                conlayer->data->get(rand() % conlayer->data->length);
            addNode(con->data);
        }
    }
}

LL::LinkedList<float> Network::compute(LL::LinkedList<float> weights)
{
    // assign the given inputs to the input layer
    LL::Node<LL::LinkedList<Node>>* cur = nodes.head;
    LL::Node<Node>* incur = cur->data->head;
    LL::Node<float>* cur_weight = weights.head;
    do
    {
        incur->data->activation = *(cur_weight->data);
    } while ((incur = incur->next) && (cur_weight = cur_weight->next));

    // call Node.activate() on each node in each layer IN ORDER
    while (cur = cur->next)
    {
        incur = cur->data->head;
        do
        {
            incur->data->activate();
        } while (incur = incur->next);
    }

    // add the output node activations to the return list
    LL::LinkedList<float> output;
    incur = nodes.tail->data->head;
    do
    {
        float* tmp = new float(incur->data->activation);
        output.push_back(tmp);
    } while (incur = incur->next);

    return output;
}

std::ostream& operator<<(std::ostream& os, Network& net)
{
    os << "genome:\n" << net.genome << "\n";
    // os << "nodes:\n" << net.nodes << "\n";
    // os << "connections:\n" << net.connections << "\n";

    return os;
}

Population::Population(int pop_size, int n_inputs, int n_outputs)
    : innovation(0),
      history(),
      nets(new Network(&innovation, n_inputs, n_outputs))
{
    Network* tmp = nets.head->data;

    for (int i = 1; i < pop_size; i++)
    {
        nets.push_back(new Network(*tmp));
    }
}

Population::Population(int pop_size, Network* tmp)
    : innovation(0), history(), nets(new Network(*tmp))
{
    for (int i = 1; i < pop_size; i++)
    {
        nets.push_back(new Network(*tmp));
    }
}

}  // namespace Genetic
