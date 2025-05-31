#include "Genetic.hpp"
#include <ostream>

namespace Genetic
{

Node::Node(int innovationi, float biasi, Connection* ini, Connection* outi)
    : innovation(innovationi),
      bias(biasi),
      in(LL::LinkedList<Connection>(ini)),
      out(LL::LinkedList<Connection>(outi)),
      activation(0) {};
Node::Node(int innovationi, float biasi, Connection* outi)
    : innovation(innovationi),
      bias(biasi),
      in(LL::LinkedList<Connection>(nullptr)),
      out(LL::LinkedList<Connection>(outi)),
      activation(0) {};
Node::Node(int innovationi, Connection* ini, float biasi)
    : innovation(innovationi),
      bias(biasi),
      in(LL::LinkedList<Connection>(ini)),
      out(LL::LinkedList<Connection>(nullptr)),
      activation(0) {};
Node::Node(int innovationi, float biasi)
    : innovation(innovationi),
      bias(biasi),
      in(LL::LinkedList<Connection>(nullptr)),
      out(LL::LinkedList<Connection>(nullptr)),
      activation(0) {};

float Node::activate()
{
    LL::Node<Connection>* cur = in.head;
    float sum = bias;

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
    LL::Node<Connection>* ccur = node.in.head;
    os << ", i:[";
    if (ccur->data)
    {
        os << ccur->data->innovation;
        while (ccur = ccur->next)
        {
            os << "," << ccur->data->innovation;
        }
    }
    os << "]";
    ccur = node.out.head;
    os << ", o:[";
    if (ccur->data)
    {
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

Network::Network() : nodes(nullptr), connections(nullptr), innovation(-2)
{
    Node* input = new Node(innovation++, 0.0);
    LL::LinkedList<Node>* inlist = new LL::LinkedList<Node>(input);
    nodes.push_back(inlist);

    Node* output = new Node(innovation++, 0.0);
    LL::LinkedList<Node>* outlist = new LL::LinkedList<Node>(output);
    nodes.push_back(outlist);

    LL::LinkedList<Connection>* conlist =
        new LL::LinkedList<Connection>(nullptr);
    connections.push_back(conlist);
}

Network::Network(int n_inputs, int n_outputs)
    : nodes(nullptr), connections(nullptr), innovation(-n_inputs - n_outputs)
{
    LL::LinkedList<Node>* inlist = new LL::LinkedList<Node>(nullptr);
    for (int i = 0; i < n_inputs; i++)
    {
        Node* input = new Node(innovation++, 0.0);
        inlist->push_back(input);
    }
    nodes.push_back(inlist);

    LL::LinkedList<Node>* outlist = new LL::LinkedList<Node>(nullptr);
    for (int i = 0; i < n_outputs; i++)
    {
        Node* output = new Node(innovation++, 0.0);
        outlist->push_back(output);
    }
    nodes.push_back(outlist);

    LL::LinkedList<Connection>* conlist =
        new LL::LinkedList<Connection>(nullptr);
    connections.push_back(conlist);
}

void Network::addNode(Connection* con)
{
    Node* node = new Node(++innovation, 0);

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
                if (inccur->data)
                {
                    do
                    {
                        inccur->data->layer++;
                    } while (inccur = inccur->next);
                }
            }
        } while (ccur = ccur->next);
    }
    else
    {
        cur->data->push_back(node);
    }

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
    node->in.push_back(con);
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
        new Connection(++innovation, layer, weight, input, output);

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

    if (!(ccur->data->head->data))
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
}

void Network::mutate()
{
    // TODO some random stuff ig
    int rand1 = rand();
    if (rand1 < 0.8 * RAND_MAX)
    {
        // mutate a weight
        if (rand() % 2)
        {
            // mutate a node weight
            int layer = 1 + rand() % (nodes.length - 1);
            LL::Node<LL::LinkedList<Node>>* nodelayer = nodes.get(layer);
            LL::Node<Node>* node =
                nodelayer->data->get(rand() % nodelayer->data->length);
            node->data->bias = (float)rand() / RAND_MAX;
        }
        else
        {
            // mutate a connection weight
            int layer = 1 + rand() % (connections.length - 1);
            LL::Node<LL::LinkedList<Connection>>* nodelayer =
                connections.get(layer);
            LL::Node<Connection>* node =
                nodelayer->data->get(rand() % nodelayer->data->length);
            node->data->weight = (float)rand() / RAND_MAX;
        }
    }

    int rand2 = rand();
    if (rand2 < 0.05 * RAND_MAX)
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

        addConnection(startnode->data, endnode->data, (float)rand() / RAND_MAX);
    }

    int rand3 = rand();
    if (rand3 < 0.01 * RAND_MAX)
    {
        // add a node
        LL::Node<LL::LinkedList<Connection>>* conlayer =
            connections.get(rand() % connections.length);
        LL::Node<Connection>* con =
            conlayer->data->get(rand() % conlayer->data->length);
        addNode(con->data);
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
        if (cur->data)
        {
            incur = cur->data->head;
            if (incur->data)
            {
                do
                {
                    incur->data->activate();
                } while (incur = incur->next);
            }
        }
    }

    // add the output node activations to the return list
    LL::LinkedList<float> output(nullptr);
    incur = nodes.tail->data->head;
    do
    {
        float* tmp = new float(incur->data->activation);
        output.push_back(tmp);
    } while (incur = incur->next);

    return output;
}

// std::ostream& operator<<(std::ostream& os, Network& net)
// {
//     os << "innovation: " << net.innovation << "\n";
//     os << "nodes:\n";
//     LL::Node<LL::LinkedList<Node>>* ncur = net.nodes.head;
//     LL::Node<Node>* inncur;
//     LL::Node<Connection>* inccur;
//     os << "\tin :\n";
//     do
//     {
//         inncur = ncur->data->head;
//         do
//         {
//             inccur = inncur->data->in.head;
//             os << "\t\t{";
//             os << "i[" << inccur->data->innovation;
//             while (inccur = inccur->next)
//             {
//                 os << inccur->data->innovation << ",";
//             }
//             os << "]";
//             inccur = inncur->data->out.head;
//             os << " o[" << inccur->data->innovation;
//             while (inccur = inccur->next)
//             {
//                 os << inccur->data->innovation << ",";
//             }
//             os << "] b" << inncur->data->bias;
//             os << " a" << inncur->data->activation;
//             os << "}\n";
//         } while (inncur = inncur->next);
//     } while (ncur != net.nodes.tail);
//
//     // hidden nodes
//     while (ncur != net.nodes.tail)
//     {
//         ncur = ncur->next;
//
//         os << "\th" << ncur->data->head->data->out.head->data->layer << " :\n";
//
//         do
//         {
//             inncur = ncur->data->head;
//             do
//             {
//                 inccur = inncur->data->in.head;
//                 os << "\t\t{";
//                 os << "i[" << inccur->data->innovation;
//                 while (inccur = inccur->next)
//                 {
//                     os << inccur->data->innovation << ",";
//                 }
//                 os << "]";
//                 inccur = inncur->data->out.head;
//                 os << " o[" << inccur->data->innovation;
//                 while (inccur = inccur->next)
//                 {
//                     os << inccur->data->innovation << ",";
//                 }
//                 os << "] b" << inncur->data->bias;
//                 os << " a" << inncur->data->activation;
//                 os << "}\n";
//             } while (inncur = inncur->next);
//         } while (ncur != net.nodes.tail);
//     }
//
//     os << "\tout:\n";
//     inncur = ncur->data->head;
//     do
//     {
//         inccur = inncur->data->in.head;
//         os << "\t\t{";
//         os << "i[" << inccur->data->innovation;
//         while (inccur = inccur->next)
//         {
//             os << inccur->data->innovation << ",";
//         }
//         os << "]";
//         inccur = inncur->data->out.head;
//         os << " o[" << inccur->data->innovation;
//         while (inccur = inccur->next)
//         {
//             os << inccur->data->innovation << ",";
//         }
//         os << "] b" << inncur->data->bias;
//         os << " a" << inncur->data->activation;
//         os << "}\n";
//     } while (inncur = inncur->next);
//
//     os << "connections:\n";
//
//     return os;
// }

std::ostream& operator<<(std::ostream& os, Network& net)
{
    os << "innovation: " << net.innovation << "\n";
    os << "nodes:\n" << net.nodes << "\n";
    os << "connections:\n" << net.connections << "\n";

    return os;
}

}  // namespace Genetic
