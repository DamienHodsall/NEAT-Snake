#include "Genetic.hpp"
#include <sys/stat.h>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <cstdlib>
#include <fstream>
#include <future>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <sstream>
#include <vector>

namespace Genetic
{

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> dist(-1, 1);

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

template <typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T>& l)
{
    if (l.empty())
        return os << "[]";

    os << "[";
    typename std::vector<T>::iterator it = l.begin();
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

Node::Node(int innovationi, int layeri, float biasi, Connection* ini,
           Connection* outi)
    : innovation(innovationi),
      layer(layeri),
      bias(biasi),
      in({ini}),
      out({outi}),
      activation(0) {};

Node::Node(int innovationi, int layeri, float biasi, Connection* outi)
    : innovation(innovationi),
      layer(layeri),
      bias(biasi),
      in(),
      out({outi}),
      activation(0) {};

Node::Node(int innovationi, int layeri, Connection* ini, float biasi)
    : innovation(innovationi),
      layer(layeri),
      bias(biasi),
      in({ini}),
      out(),
      activation(0) {};

Node::Node(int innovationi, int layeri, float biasi)
    : innovation(innovationi),
      layer(layeri),
      bias(biasi),
      in(),
      out(),
      activation(0) {};

float Node::activate()
{
    if (in.empty())
        return bias;

    float sum = bias;

    for (Connection* con : in)
        sum += con->weight * con->in->activation;

    activation = std::fmin(std::fmax(sum, -1), 1);
    // activation = sum;

    return activation;
}

std::ostream& operator<<(std::ostream& os, Node& node)
{
    os << "{";
    os << "I:" << node.innovation;
    os << ", l:" << node.layer;
    os << ", b:" << node.bias;
    os << ", i:[";
    if (!node.in.empty())
    {
        std::list<Connection*>::iterator itin = node.in.begin();
        os << (*itin)->innovation;
        itin = std::next(itin);
        while (itin != node.in.end())
        {
            os << "," << (*itin)->innovation;
            itin = std::next(itin);
        }
    }
    os << "]";
    os << ", o:[";
    if (!node.out.empty())
    {
        std::list<Connection*>::iterator itout = node.out.begin();
        os << (*itout)->innovation;
        itout = std::next(itout);
        while (itout != node.out.end())
        {
            os << "," << (*itout)->innovation;
            itout = std::next(itout);
        }
    }
    os << "]";
    os << ", a:" << node.activation;
    os << "}";

    return os;
}

bool operator==(Node& lhs, Node& rhs)
{
    bool innovattion = (lhs.innovation == rhs.innovation);
    bool bias = (lhs.bias == rhs.bias);

    return innovattion && bias;
}

bool operator==(const Node& lhs, const Node& rhs)
{
    bool innovattion = (lhs.innovation == rhs.innovation);
    bool bias = (lhs.bias == rhs.bias);

    return innovattion && bias;
}

bool operator==(const Node& lhs, Node& rhs)
{
    bool innovattion = (lhs.innovation == rhs.innovation);
    bool bias = (lhs.bias == rhs.bias);

    return innovattion && bias;
}

Connection::Connection(int innovationi, float weighti, Node* ini, Node* outi)
    : innovation(innovationi),
      weight(weighti),
      in(ini),
      out(outi),
      enabled(true) {};

std::ostream& operator<<(std::ostream& os, Connection& con)
{
    os << "{";
    os << "I:" << con.innovation;
    os << ", i:" << con.in->innovation;
    os << ", o:" << con.out->innovation;
    os << ", e:" << con.enabled;
    os << ", w:" << con.weight;
    os << "}";

    return os;
}

Event::Event(Node* nodei)
    : innovation(nodei->innovation), a(-1), b(nodei->in.front()->innovation)
{
}
Event::Event(Connection* coni)
    : innovation(coni->innovation),
      a(coni->in->innovation),
      b(coni->out->innovation)
{
}

std::ostream& operator<<(std::ostream& os, Event& event)
{
    os << "{";
    os << "I:" << event.innovation;
    os << ", a:" << event.a;
    os << ", b:" << event.b;
    os << "}";

    return os;
}

Gene::Gene(Node* nodei) : Event(nodei), weight(&(nodei->bias)) {};
Gene::Gene(Connection* coni) : Event(coni), weight(&(coni->weight)) {};

std::ostream& operator<<(std::ostream& os, Gene& gene)
{
    os << "{";
    os << "I:" << gene.innovation;
    os << ", a:" << gene.a;
    os << ", b:" << gene.b;
    os << ", w:" << *(gene.weight);
    os << "}";

    return os;
}

bool Event::operator==(Event& other)
{
    return (this->a == other.a) && (this->b == other.b);
}

bool Event::operator==(const Event& other)
{
    return (this->a == other.a) && (this->b == other.b);
}

Network::Network(int* innovationi, std::list<Event>* historyi)
    : nodes(),
      connections(),
      innovation(innovationi),
      genome(),
      history(historyi)
{
    nodes.emplace_back();
    nodes.front().emplace_back((*innovation)++, 0, 0.0);
    nodes.emplace_back();
    nodes.back().emplace_back((*innovation)++, 1, 0.0);
    connections.emplace_back();
}

Network::Network(int* innovationi, std::list<Event>* historyi, int n_in,
                 int n_out)
    : nodes(),
      connections(),
      innovation(innovationi),
      genome(),
      history(historyi)
{
    nodes.emplace_back();
    for (int i = 0; i < n_in; i++)
    {
        nodes.front().emplace_back((*innovation)++, 0, 0.0);
    }

    nodes.emplace_back();
    for (int i = 0; i < n_out; i++)
    {
        nodes.back().emplace_back((*innovation)++, 1, 0.0);
    }

    connections.emplace_back();
}

Network::Network(const Network& rhs)
    : nodes(),
      connections(),
      innovation(new int(0)),
      genome(),
      history(new std::list<Event>())
{
    nodes.emplace_back();
    for (int i = 0; i < rhs.nodes.front().size(); i++)
    {
        nodes.front().emplace_back((*innovation)++, 0, 0.0);
    }

    nodes.emplace_back();
    for (int i = 0; i < rhs.nodes.back().size(); i++)
    {
        nodes.back().emplace_back((*innovation)++, 1, 0.0);
    }

    connections.emplace_back();

    if (rhs.genome.empty())
    {
        innovation = rhs.innovation;
        history = rhs.history;
        return;
    }

    bool skip = false;

    for (const Gene& event : rhs.genome)
    {
        if (skip)
        {
            skip = false;
        }
        else
        {
            *innovation = event.innovation;
            bool breaker = false;

            if (event.a < 0)
            {
                // find equivalent Connection in new Network
                for (std::list<Connection>& conlist : connections)
                {
                    for (Connection& con : conlist)
                    {
                        if (con.innovation == event.b)
                        {
                            addNode(&con, *(event.weight));
                            skip = true;
                            breaker = true;
                        }
                    }

                    if (breaker)
                        break;
                }
            }
            else
            {
                Node* innode = nullptr;
                bool breaker = false;

                for (std::list<Node>& nodelist : nodes)
                {
                    for (Node& node : nodelist)
                    {
                        if (innode)
                        {
                            // check for out node
                            // always after the in node so use the same loop
                            if (node.innovation == event.b)
                            {
                                addConnection(innode, &node, *(event.weight));
                                breaker = true;
                                break;
                            }
                        }
                        else
                        {
                            if (node.innovation == event.a)
                            {
                                innode = &node;
                            }
                        }
                    }

                    if (breaker)
                        break;
                }
            }
        }
    }

    delete innovation;
    delete history;
    innovation = rhs.innovation;
    history = rhs.history;
}

void Network::addNode(Connection* con)
{
    addNode(con, 0);
}

void Network::addNode(Connection* con, float weight)
{
    Node* node = new Node(*innovation, con->in->layer + 1, weight);
    node->in.emplace_back(con);
    Gene checker(node);
    delete node;
    if (std::find(genome.begin(), genome.end(), checker) != genome.end())
        return;

    bool upto_conin = true;
    bool past_conout = false;
    bool inserted = false;
    int layer = 0;

    std::list<std::list<Node>>::iterator nodelayer = nodes.begin();

    // find the layer that contains con->in
    while ((nodelayer != nodes.end()) &&
           !(std::count(nodelayer->begin(), nodelayer->end(), *con->in) > 0))
    {
        layer++;
        nodelayer = std::next(nodelayer);
    }

    layer++;
    nodelayer = std::next(nodelayer);

    // insert the new node into a new layer if the con->in and con->out nodes are in sequential layers
    // otherwise add it to the layer just past the con->in node
    if (con->in->layer + 1 == con->out->layer)
    {
        std::list<std::list<Node>>::iterator newlayer =
            nodes.emplace(std::find(nodes.begin(), nodes.end(), *nodelayer));
        newlayer->emplace_back((*innovation)++, layer, weight);
        node = &newlayer->back();

        nodelayer = std::next(newlayer);
        do
        {
            if (nodelayer->front().layer >= layer)
            {
                for (Node& upnode : *nodelayer)
                {
                    upnode.layer++;
                }
            }
            nodelayer = std::next(nodelayer);
        } while (nodelayer != nodes.end());
    }
    else
    {
        nodelayer->emplace_back((*innovation)++, layer, weight);
        node = &nodelayer->back();
    }

    node->in.emplace_back(con);
    history->emplace_back(node);

    std::list<Event>::iterator gene =
        std::find(history->begin(), --history->end(), history->back());
    if ((history->size() > 1) && (gene != --history->end()))
    {
        history->pop_back();
        (*innovation)--;
        node->innovation = gene->innovation;
    }

    genome.push_back(node);

    addConnection(node, con->out, 1.0);

    // remove the old in connection from the con->out node
    con->out->in.erase(
        std::find(con->out->in.begin(), con->out->in.end(), con));

    con->out = node;
}

void Network::addConnection(Node* input, Node* output, float weight)
{
    Connection* con = new Connection(*innovation, weight, input, output);
    Gene checker(con);
    delete con;
    if (std::find(genome.begin(), genome.end(), checker) != genome.end())
        return;

    for (Connection* concheck : input->out)
    {
        if (concheck->out->innovation == output->innovation)
            return;
    }

    bool con_placed = false;
    if (connections.front().empty())
    {
        connections.front().emplace_back((*innovation)++, weight, input,
                                         output);
        con = &connections.front().back();
        history->emplace_back(con);
    }
    else
    {
        for (std::list<Connection>& conlayer : connections)
        {
            if (conlayer.front().in->layer == input->layer)
            {
                conlayer.emplace_back((*innovation)++, weight, input, output);
                con = &conlayer.back();
                history->emplace_back(con);
                con_placed = true;
                break;
            }
        }

        if (!con_placed)
        {
            connections.emplace_back();
            connections.back().emplace_back((*innovation)++, weight, input,
                                            output);
            con = &connections.back().back();
            history->emplace_back(con);
        }
    }

    input->out.emplace_back(con);
    output->in.emplace_back(con);

    std::list<Event>::iterator gene =
        std::find(history->begin(), --history->end(), history->back());
    if ((history->size() > 1) && (gene != --history->end()))
    {
        history->pop_back();
        (*innovation)--;
        con->innovation = gene->innovation;
    }

    genome.push_back(con);
}

void Network::mutate()
{
    if (rand() < 0.8 * RAND_MAX)
    {
        int num_mutations = rand() % (genome.size() + 1);
        for (int i = 0; i < num_mutations; i++)
        {
            // mutate a weight
            if (rand() % 2)
            {
                //mutate a node weight
                if (nodes.size() > 2)
                {
                    std::list<std::list<Node>>::iterator nodelayer =
                        nodes.begin();
                    std::advance(nodelayer, 1 + rand() % (nodes.size() - 2));
                    std::list<Node>::iterator node = nodelayer->begin();
                    std::advance(node, rand() % nodelayer->size());
                    node->bias += dist(gen);
                }
            }
            else
            {
                // mutate a connection weight
                std::list<std::list<Connection>>::iterator conlayer =
                    connections.begin();
                std::advance(conlayer, rand() % connections.size());

                if (!(*conlayer).empty())
                {
                    std::list<Connection>::iterator con = conlayer->begin();
                    std::advance(con, rand() % conlayer->size());
                    con->weight += dist(gen);
                }
            }
        }
    }

    if (rand() < 0.05 * RAND_MAX)
    {
        // add a connection
        // choose a starting node then choose a random node that is after it
        int start = rand() % (nodes.size() - 1);
        std::list<std::list<Node>>::iterator nodelayer = nodes.begin();
        std::advance(nodelayer, start);
        std::list<Node>::iterator startnode = nodelayer->begin();
        std::advance(startnode, rand() % nodelayer->size());

        int end = start + 1 + rand() % (nodes.size() - start - 1);
        std::advance(nodelayer, 1 + rand() % (nodes.size() - start - 1));
        std::list<Node>::iterator endnode = nodelayer->begin();
        std::advance(endnode, rand() % nodelayer->size());

        addConnection(&(*startnode), &(*endnode),
                      1.0 - 2.0 * (float)rand() / RAND_MAX);
    }

    if (rand() < 0.03 * RAND_MAX)
    {
        // add a node
        std::list<std::list<Connection>>::iterator conlayer =
            connections.begin();
        std::advance(conlayer, rand() % connections.size());
        if (!conlayer->empty())
        {
            std::list<Connection>::iterator con = conlayer->begin();
            std::advance(con, rand() % conlayer->size());
            addNode(&(*con));
        }
    }
}

std::list<float> Network::compute(std::list<float>& weights)
{
    bool first_layer = true;
    for (std::list<Node>& layer : nodes)
    {
        if (first_layer)
        {
            std::list<Node>::iterator node = layer.begin();
            std::list<float>::iterator weight = weights.begin();
            while ((node != layer.end()) && weight != weights.end())
            {
                node->activation = *weight;
                node++;
                weight++;
            }

            first_layer = false;
        }
        else
        {
            for (Node& node : layer)
            {
                node.activate();
            }
        }
    }

    std::list<float> output;

    for (Node& node : nodes.back())
    {
        output.emplace_back(node.activation);
    }

    return output;
}

void Network::save(std::string filename)
{
    std::ofstream fileout(filename);

    fileout << nodes.front().size() << " " << nodes.back().size() << std::endl;

    for (Gene& gene : genome)
    {
        fileout << gene.innovation << " " << gene.a << " " << gene.b << " "
                << *gene.weight << std::endl;
    }

    fileout.close();
}

void Network::visualize(std::string filename)
{
    std::ofstream fileout(filename);

    fileout << "digraph {" << std::endl;
    fileout << "label=\"G:" << genome.size() << "\\n \"" << std::endl;
    fileout << "labelloc=t" << std::endl;
    fileout << "rankdir=\"LR\";" << std::endl;
    fileout << "ranksep=3;" << std::endl;
    fileout << "nodesep=1.5;" << std::endl;
    fileout << "splines=line;" << std::endl;
    fileout << "node [shape=circle style=filled fixedsize=true height=1];"
            << std::endl;

    int rank = 0;
    for (std::list<Node>& layer : nodes)
    {
        fileout << "{";
        fileout << "rank = same;";
        for (Node& node : layer)
        {
            fileout << " n" << node.innovation << " [label=\""
                    << node.innovation << "\\n"
                    << node.bias << "\"]";
        }
        fileout << "r" << rank << " [style=invis]}" << std::endl;

        for (Node& node : layer)
        {
            fileout << "n" << node.innovation << " -> ";
        }
        fileout << "r" << rank << " [style=invis]" << std::endl;

        rank++;
    }

    for (std::list<Connection>& layer : connections)
    {
        for (Connection& con : layer)
        {
            fileout << "n" << con.in->innovation << " -> n"
                    << con.out->innovation << " [xlabel=\"" << con.innovation
                    << ":" << con.weight << "\"]" << std::endl;
        }
    }

    fileout << "}" << std::endl;

    fileout.close();
}

std::ostream& operator<<(std::ostream& os, Network& net)
{
    os << "genome:" << net.genome.size() << "\n" << net.genome << "\n";
    os << "nodes:"
       << std::accumulate(net.nodes.begin(), net.nodes.end(), 0,
                          [](auto& init, auto& ls) { return init + ls.size(); })
       << "\n"
       << net.nodes << "\n";
    os << "connections:"
       << std::accumulate(net.connections.begin(), net.connections.end(), 0,
                          [](auto& init, auto& ls) { return init + ls.size(); })
       << "\n"
       << net.connections << "\n";

    return os;
}

Population::Population(int pop_size, int n_in, int n_out)
    : innovation(0),
      generation(0),
      folder(),
      history(),
      nets(),
      c1(1.0),
      c2(1.0),
      c3(0.4),
      ct(3.0),
      species()
{
    char foldertime[50];
    time_t timestamp = time(NULL);
    struct tm datetime = *localtime(&timestamp);
    strftime(foldertime, 50, "%Y-%m-%d %H:%M:%S", &datetime);

    folder.append("./champions/");
    folder.append(foldertime);
    mkdir(folder.c_str(), 0777);

    nets.emplace_back(&innovation, &history, n_in, n_out);

    for (int i = 1; i < pop_size; i++)
    {
        nets.emplace_back(nets.front());
    }
}

Population::Population(std::string filename, int pop_size)
    : innovation(0),
      generation(0),
      folder(),
      history(),
      nets(),
      c1(1.0),
      c2(1.0),
      c3(0.4),
      ct(3.0),
      species()
{
    char foldertime[50];
    time_t timestamp = time(NULL);
    struct tm datetime = *localtime(&timestamp);
    strftime(foldertime, 50, "%Y-%m-%d %H:%M:%S", &datetime);

    folder.append("./champions/");
    folder.append(foldertime);
    mkdir(folder.c_str(), 0777);

    std::ifstream refnet(filename);

    if (!refnet.is_open())
    {
        std::cerr << "Could not find file: " << filename << std::endl;
    }
    else
    {
        std::string line;
        std::stringstream linestream;

        Network* net;
        int n_in, n_out, inno, a, b;
        float weight;
        bool skip = false;
        bool surface_nodes = false;

        while (std::getline(refnet, line))
        {
            if (line.find("//") != std::string::npos)
                continue;

            linestream << line;

            if (!surface_nodes)
            {
                linestream >> n_in >> n_out;

                nets.emplace_back(&innovation, &history, n_in, n_out);
                net = &nets.front();

                surface_nodes = true;
            }
            else
            {
                linestream >> inno >> a >> b >> weight;

                if (skip)
                {
                    skip = false;
                }
                else
                {
                    innovation = inno;
                    bool breaker = false;

                    if (a == -1)
                    {
                        for (std::list<Connection>& conlist : net->connections)
                        {
                            for (Connection& con : conlist)
                            {
                                if (con.innovation == b)
                                {
                                    net->addNode(&con, weight);
                                    skip = true;
                                    breaker = true;
                                }
                            }

                            if (breaker)
                                break;
                        }
                    }
                    else
                    {
                        Node* innode = nullptr;
                        bool breaker = false;
                        innovation = inno;

                        for (std::list<Node>& nodelist : net->nodes)
                        {
                            for (Node& node : nodelist)
                            {
                                if (innode)
                                {
                                    if (node.innovation == b)
                                    {
                                        net->addConnection(innode, &node,
                                                           weight);
                                        breaker = true;
                                        break;
                                    }
                                }
                                else
                                {
                                    if (node.innovation == a)
                                    {
                                        innode = &node;
                                    }
                                }
                            }

                            if (breaker)
                                break;
                        }
                    }
                }
            }

            linestream.clear();
        }

        refnet.close();

        for (int i = 1; i < pop_size; i++)
        {
            nets.emplace_back(nets.front());
        }
    }
}

void Population::speciate_network(Network* net)
{
    // add to existing species
    bool new_species = true;
    std::list<std::list<Gene>>::iterator specrep = reps.begin();
    std::list<std::list<Network*>>::iterator spec = species.begin();
    while ((spec != species.end()) && (specrep != reps.end()))
    {
        if (compatibility(net->genome, *specrep) < ct)
        {
            spec->emplace_back(net);
            net->species = spec;
            new_species = false;
            break;
        }
        specrep++;
        spec++;
    }

    // if not, add to new species
    if (new_species)
    {
        species.emplace_back();
        species.back().emplace_back(net);
        net->species = --species.end();
        reps.emplace_back(net->genome);
    }
}

void Population::mutate()
{
    // choose species representatives
    reps.clear();
    for (std::list<Network*>& spec : species)
    {
        std::list<Network*>::iterator specrep = spec.begin();
        std::advance(specrep, rand() % spec.size());
        reps.emplace_back((*specrep)->genome);
    }

    species.clear();
    for (std::list<Gene> rep : reps)
        species.emplace_back();

    history.clear();

    // std::list<std::future<void>> threads;

    for (Network& net : nets)
    {
        if (!net.protect)
            net.mutate();

        // Speciation
        // threads.emplace_back(std::async(
        //     std::launch::async, &Population::speciate_network, this, &net));
        speciate_network(&net);

        net.protect = false;
    }

    // for (auto& thrd : threads)
    // {
    //     thrd.get();
    // }

    // remove empty species
    std::list<std::list<std::list<Network*>>::iterator> extincts;
    for (std::list<std::list<Network*>>::iterator spec = species.begin();
         spec != species.end(); spec++)
        if (spec->empty())
            extincts.emplace_back(spec);
    for (std::list<std::list<Network*>>::iterator extinct : extincts)
        species.erase(extinct);
}

void Population::compete(float (*comp_func)(Network&))
{
    std::vector<std::pair<std::list<Network>::iterator, std::future<float>>>
        future_scores;
    std::vector<std::pair<std::list<Network>::iterator, float>> scores;

    for (std::list<Network>::iterator net = nets.begin(); net != nets.end();
         net++)
    {
        future_scores.emplace_back(
            net, std::async(std::launch::async, comp_func, std::ref(*net)));
    }

    for (auto& score : future_scores)
    {
        scores.emplace_back(score.first,
                            score.second.get() / score.first->species->size());
    }

    // sort ascending
    std::sort(scores.begin(), scores.end(), [](auto& left, auto& right)
              { return left.second < right.second; });

    // choose random with better scores having higher probability
    // std::partial_sum(scores.begin(), scores.end(), cumsum.begin(),
    //                  [](float part, auto& score)
    //                  { return part + score.second; });

    int count = 0;
    std::vector<std::pair<std::list<Network>::iterator, float>>::iterator
        score = scores.begin();
    while (score != scores.end())
    {
        if (count >= scores.size() / 2)
        {
            break;
        }

        // cull the weak
        score->first->species->erase(std::find(score->first->species->begin(),
                                               score->first->species->end(),
                                               &(*score->first)));
        nets.erase(score->first);

        count++;
        score++;
    }

    // remove empty species
    std::list<std::list<std::list<Network*>>::iterator> extincts;
    for (std::list<std::list<Network*>>::iterator spec = species.begin();
         spec != species.end(); spec++)
        if (spec->empty())
            extincts.emplace_back(spec);
    for (std::list<std::list<Network*>>::iterator extinct : extincts)
        species.erase(extinct);

    scores.erase(scores.begin(), score);

    count = 0;
    score = scores.begin();
    while (score != scores.end())
    {
        // duplicate the strong
        nets.emplace_back(*score->first);

        if (count < nets.size() / 10)
            score->first->protect = true;

        count++;
        score++;
    }

    scores.back().first->save(folder + "/" + std::to_string(generation) +
                              ".net");
    generation++;
}

float Population::compatibility(std::list<Gene>& genome1,
                                std::list<Gene>& genome2)
{
    int N = std::max(genome1.size(), genome2.size());
    int D = 0;
    int nW = 0;
    float W = 0;

    std::list<Gene>::iterator genome1it = genome1.begin();
    std::list<Gene>::iterator genome2it = genome2.begin();

    while ((genome1it != genome1.end()) && (genome2it != genome2.end()))
    {
        if (genome1it->innovation < genome2it->innovation)
        {
            D++;
            genome1it++;
        }
        else if (genome1it->innovation > genome2it->innovation)
        {
            D++;
            genome2it++;
        }
        else
        {
            W += std::abs(*genome1it->weight - *genome2it->weight);
            nW++;
            genome1it++;
            genome2it++;
        }
    }

    int E = std::distance(genome1it, genome1.end()) +
            std::distance(genome2it, genome2.end());

    if (nW > 0)
        W /= nW;

    return (N == 0) ? 0.0 : (c1 * E / N) + (c2 * D / N) + (c3 * W);
}

float Population::compatibility(Network& net1, Network& net2)
{
    return compatibility(net1.genome, net2.genome);
}

}  // namespace Genetic
