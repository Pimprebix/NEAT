#ifndef GENOME_H
#define GENOME_H
#include "ConnectionGene.h"
#include "IDGenerator.h"
#include "NodeGene.h"
#include <vector>
#include <string>
#include <map>
#include <algorithm>

class Genome {  // a genome is a network
public:
    Genome(int id = -1);
    Genome(int numberOfInputNode, int numberOfOuputNode, int id = -1);
    
    // UTILS
    void getNewId();
    void randomizeWeight();
    void display() const;
    void printCPP() const;
    
    // sorting
    void sortConnectionByInnovationNumber();
    void sortAllNodes();
    
    // setters
    void setFitness(float iFitness);
    void setAdjustedFitness(float adjustmentFactor);
    
    map<int, vector<ConnectionGene> > getNodeIncomingConnectionMap() const;
    map<int, NodeGene> getNodeMap() const;
    
    // add elements to Genome
    void addConnection(ConnectionGene iCon);
    void addNode(NodeGene iNode, int insertAfterNode=-1);
    
    ConnectionGene getConnectionFromInnovationNumber(int nb) const;
    static float getDistance(const Genome&  iGenome1, const Genome& iGenome2);    
    static void getTopologicalComparision(const Genome&  iGenome1, 
                                            const Genome& iGenome2, 
                                            vector<int>& commonGenes,
                                            vector<int>& disjointInGenome1,
                                            vector<int>& disjointInGenome2,
                                            vector<int>& excessInGenome1,
                                            vector<int>& excessInGenome2);
    
    // runs network!
    vector<float> applyInput(const vector<float>& input);

    
    // MUTATIONS
    // pointMutate modifies a weight radomly or according to a step
    void pointMutate();
    // nodeMutate silence a connection and adds a connection between the 2 nodes adding 1 new intermediary node 
    void nodeMutate();
    // enable or disable connection
    void enableDisableMutate();
    // ConnectionCreate creates a connection between 2 nodes
    void createConnection();
    // crossover
    static Genome crossOver(const Genome& fitest, const Genome& weakest, bool equal = false);    
    
    // attributes
    int _id;
    float _fitness;
    float _adjustedFitness;
    
private:
    int _maxInnovationNumber;
    
    // collections of Ids:
    vector<int> _inputLayer; 
    vector<int> _outputLayer; 
    vector<int> _hiddenLayer; 
    vector<int> _allNodes;
    
    map<int, NodeGene> _nodes; 
    vector<ConnectionGene> _connections; 
    
    // check if a node has connection or exists
    bool hasConnectionWithNodeId(int aNodeId) const;
    bool hasConnectionId(int aConId) const;
    bool isNodeRegistered(int aNodeId) const;
};

inline void Genome::addConnection(ConnectionGene iCon) {
    _connections.push_back(iCon);
};

inline ConnectionGene Genome::getConnectionFromInnovationNumber(int aConId) const {    
    auto it = find_if(
            _connections.begin(), 
            _connections.end(), 
            [aConId] (const ConnectionGene& aConnnectionGene) { 
                    return aConnnectionGene._innovationNumber == aConId;});
    ConnectionGene aCon = (*it);
    return aCon;
};

inline map<int, NodeGene> Genome::getNodeMap() const {
  return _nodes;  
};

inline bool Genome::isNodeRegistered(int iNodeId) const {
//    auto it = std::find(_nodes.begin(), _nodes.end(), iNodeId)
    return std::find(_allNodes.begin(), _allNodes.end(), iNodeId) != _allNodes.end();
};

inline void Genome::addNode(NodeGene iNode, int insertAfterNode) {
    _nodes.insert(std::pair<int, NodeGene>(iNode._id, iNode));
    
    auto it = find (_allNodes.begin(), _allNodes.end(), insertAfterNode);
    if (it!=_allNodes.end()) {it++;}
    
    _allNodes.insert(it, iNode._id); // insert at the end by default
    
    if (iNode.isInput() || iNode.isBias())  {
        _inputLayer.push_back(iNode._id);
    } 
    else if (iNode.isOutput())  {
        _outputLayer.push_back(iNode._id);
    } 
    else {
        _hiddenLayer.push_back(iNode._id);
    }
};

inline void Genome::setFitness(float iFitness) {
  _fitness = iFitness;
};

inline void Genome::setAdjustedFitness(float iAdjustedfitness) {
  _adjustedFitness =  iAdjustedfitness;
};

inline float Genome::getDistance(const Genome&  iGenome1, const Genome& iGenome2) {
    float aReturnedDistance;
    // dist = (excess * c1 + disjoint * c2 ) / N + c3 * AverageWeightDifference
    // c1, c2, c3 arbitary, N=1 or normalization (= total number of genes for the biggest)
    vector<int> commonGenes;
    vector<int> disjointInGenome1;
    vector<int> disjointInGenome2;
    vector<int> excessInGenome1;
    vector<int> excessInGenome2;
    getTopologicalComparision(iGenome1, 
                            iGenome2, 
                            commonGenes,
                            disjointInGenome1,
                            disjointInGenome2,
                            excessInGenome1,
                            excessInGenome2);
    
    float c1 = 2.0;
    float c2 = 2.0;
    float c3 = 1.0;
    float N = float(max(iGenome1._connections.size(), iGenome2._connections.size()));
    float W = 0.0;
    for (int innovNber : commonGenes) {
        W += iGenome1.getConnectionFromInnovationNumber(innovNber)._weight;
        W -= iGenome2.getConnectionFromInnovationNumber(innovNber)._weight;
    }
    W = abs( W / float(commonGenes.size()) );
    
    aReturnedDistance = (c1 * float(excessInGenome1.size() + excessInGenome2.size())) / N
                      + (c2 * float(disjointInGenome1.size() + disjointInGenome2.size())) / N  
                      +  c3 * W;
    
    if (false) {
        cerr << "average weight difference = " << W << endl;
        cerr << "nb of genes in excess = " << float(excessInGenome1.size() + excessInGenome2.size()) << endl;
        cerr << "nb of genes disjoint  = " << float(disjointInGenome1.size() + disjointInGenome2.size()) << endl;
        cerr << "distance = " << aReturnedDistance << endl;
    }
    return aReturnedDistance;
};

inline void Genome::sortConnectionByInnovationNumber() {
    std::sort(_connections.begin(),
        _connections.end());
};

inline void Genome::getTopologicalComparision(const Genome&  iGenome1, 
                                        const Genome& iGenome2, 
                                        vector<int>& commonGenes,
                                        vector<int>& disjointInGenome1,
                                        vector<int>& disjointInGenome2,
                                        vector<int>& excessInGenome1,
                                        vector<int>& excessInGenome2) {
    int minMaxInnovNumber = min(iGenome1._maxInnovationNumber, iGenome2._maxInnovationNumber);
    
    for (const ConnectionGene& aCon: iGenome1._connections) {  
        // check if it's common
        if (iGenome2.hasConnectionId(aCon._innovationNumber)) {
            commonGenes.push_back(aCon._innovationNumber);
        }
        else if (aCon._innovationNumber < minMaxInnovNumber) {
            disjointInGenome1.push_back(aCon._innovationNumber);
        }
        else {
            excessInGenome1.push_back(aCon._innovationNumber);
        }
    }
    
    for (const ConnectionGene& aCon: iGenome2._connections) {  
        if (std::find(commonGenes.begin(), commonGenes.end(), aCon._innovationNumber) == commonGenes.end()) {
            if (aCon._innovationNumber < minMaxInnovNumber) {
                disjointInGenome2.push_back(aCon._innovationNumber);
            }
            else {
                excessInGenome2.push_back(aCon._innovationNumber);
            }
        }
    }
};

inline void Genome::getNewId() {
    _id = IDGenerator::instance()->getId();
};
#endif // GENOME_H
