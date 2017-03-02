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
    Genome(int id = -1);
    Genome(int numberOfInputNode, int numberOfOuputNode, int id = -1);
    
    // UTILS
    int _id;
    void getNewId();
    void sortConnectionByInnovationNumber();
    void display() const;
    void randomizeWeight();
    void setFitness(float iFitness);
    void setAdjustedFitness(float adjustmentFactor);
    map<int, NodeGene> getNodeMap() const;
    void addConnection(ConnectionGene iCon);
    void addNode(NodeGene iNode);
    bool isNodeRegistered(int aNodeId) const;
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
    static Genome crossOver(const Genome& fitest, const Genome& weakest, bool equal = false);    
    
    float _fitness;
    float _adjustedFitness;
private:
    vector<int> _inputLayer; 
    vector<int> _outputLayer; 
    vector<int> _hiddenLayer; 
    map<int, NodeGene> _nodes; 
    vector<ConnectionGene> _connections; 
    
    
    void prepareNetwork(const vector<int>& input);
    // return a map  first=id of upper node,  second=pointer to connection
    map<int, ConnectionGene*> getUpperNode(int aNodeId);
    // check if a node has connection
    bool hasConnectionWith(int aNodeId) const;
    bool hasConnectionId(int aConId) const;
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
    return _nodes.find(iNodeId) != _nodes.end();
};

inline void Genome::addNode(NodeGene iNode) {
    _nodes.insert(std::pair<int, NodeGene>(iNode._id, iNode));
    if (iNode.isInput())  {
        _inputLayer.push_back(iNode._id);
    } 
    else if (iNode.isOuput())  {
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

inline void Genome::getTopologicalComparision(const Genome&  iGenome1, 
                                        const Genome& iGenome2, 
                                        vector<int>& commonGenes,
                                        vector<int>& disjointInGenome1,
                                        vector<int>& disjointInGenome2,
                                        vector<int>& excessInGenome1,
                                        vector<int>& excessInGenome2) {
    // compute max
    int maxInnovNumberG1 = 0;
    int maxInnovNumberG2 = 0;
    for (const ConnectionGene& aCon: iGenome1._connections) {
        // register new max if necessary
        if (maxInnovNumberG1<aCon._innovationNumber) {
            maxInnovNumberG1 = aCon._innovationNumber;
        }
    }
    for (const ConnectionGene& aCon: iGenome2._connections) {
        // register new max if necessary
        if (maxInnovNumberG2<aCon._innovationNumber) {
            maxInnovNumberG2 = aCon._innovationNumber;
        }
    }
    int minMaxInnovNumber = min(maxInnovNumberG1, maxInnovNumberG2);
    
    
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

#endif // GENOME_H
