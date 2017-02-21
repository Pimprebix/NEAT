#ifndef GENOME_H
#define GENOME_H
#include "ConnectionGene.h"
#include "NodeGene.h"
#include <vector>
#include <string>
#include <map>
#include <algorithm>

class Genome {  // a genome is a network
public:
    Genome() {};
    Genome(int numberOfInputNode, int numberOfOuputNode);
    
    // UTILS
    void buildMinimalStructure();
    void sortConnectionByInnovationNumber();
    void display() const;
    void randomizeWeight();
    void setFitness(float iFitness);
    
    // runs network!
    vector<float> applyInput(const vector<int>& input);

    
    // MUTATIONS
    void mutateOrNot(int forPointMutate, int forNodeMutate, int forEnableDisableMutate);
    // pointMutate modifies a weight radomly or according to a step
    void pointMutate();
    // nodeMutate silence a connection and adds a connection between the 2 nodes adding 1 new intermediary node 
    void nodeMutate();
    // enable or disable connection
    void enableDisableMutate();
    // coopulate
    void crossOver(const Genome& other) const;    
    
private:
    float _fitness;
    vector<int> _inputLayer; 
    vector<int> _outputLayer; 
    vector<int> _hiddenLayer; 
    map<int, NodeGene> _nodes; 
    vector<ConnectionGene> _connections; 
    
    
    void prepareNetwork(const vector<int>& input);
    // return a map  first=id of upper node,  second=pointer to connection
    map<int, ConnectionGene*> getUpperNode(int aNodeId);
    // check if a node has connection
    bool hasConnection(int aNodeId);
};

#endif // GENOME_H
