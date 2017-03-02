#include "Genome.h"
#include "InnovationBank.h"
#include "IDGenerator.h"


Genome::Genome(int id) {
    if (id == -1) {
        getNewId();
    }
    else {
        _id = id;
    }
}

Genome::Genome(int numberOfInputNode, int numberOfOuputNode, int id) {
    // add input nodes
    for(int i=0; i<numberOfInputNode; i++) {
        addNode(NodeGene("input"));
    }
    // add output nodes
    for(int i=0; i<numberOfOuputNode; i++) {
        addNode(NodeGene("output"));
    }
    // buildMinimalStructure();
    for (int const& aId1: _inputLayer) {
        for (int const& aId2: _outputLayer) {
            _connections.push_back(ConnectionGene(aId1, aId2));       
        }     
    }    
    // set id
    if (id == -1) {
        getNewId();
    }
    else {
        _id = id;
    }
};

void Genome::getNewId() {
    _id = IDGenerator::instance()->getId();
};

// fills inputs, set nodes to unused
void Genome::prepareNetwork(const vector<int>& input) {
    // clean network
    for (auto& aPairIntNode : _nodes) { 
        aPairIntNode.second.reset();
    }
    // fill inputs layer
    for (unsigned int i=0; i<_inputLayer.size(); i++) {
        _nodes[_inputLayer.at(i)]._value = input.at(i);
        _nodes[_inputLayer.at(i)].setUsed();
    }
};

// runs network!
vector<float> Genome::applyInput(const vector<int>& input) {
    bool verbose = false;
    
    if (verbose) {
        display();
    }
    
    if (input.size() != _inputLayer.size()) {
        cerr << "invalid input : not enough input nodes or too much: entries number = "
                << input.size() << " / input nodes number = " << _inputLayer.size() << endl;
    }
    else {
        prepareNetwork(input);
    }
    
    // run graph:
    
    // first build a list of all nodes left to be computed:
    vector<int> listOfNodesToCompute = _hiddenLayer;
    for (int aNodeId: _outputLayer) {
        listOfNodesToCompute.push_back(aNodeId);
    }
    
    // we compute outputs 
    while (!listOfNodesToCompute.empty()) {
        vector<int> NodesThatCannotBeComputedYet;
        
        for (int aNodeId: listOfNodesToCompute) {
            NodeGene& aNode = _nodes[aNodeId];
            
            // get the list of nodes that we need to compute our subject's value
            bool areAllPreviousNodesComputed = true;
            map<int, ConnectionGene*> upperNodeIds = getUpperNode(aNode._id);
            
            // check if all upper nodes are computed:
            for (auto const& aPairIdConnector: upperNodeIds) {
                if (!_nodes[aPairIdConnector.first].isUsed()) {
                    areAllPreviousNodesComputed = false;
                }
            }
            
            // if so, compute the value:
            if (areAllPreviousNodesComputed) {
                if (verbose) {cerr << endl <<"computing value of node "<<aNode._id << endl;}
                for (auto const& aPairIdConnector: upperNodeIds) {
                    if (verbose) {
                        cerr << " - node " << _nodes[aPairIdConnector.first]._id 
                            << " is connected with value " 
                            << _nodes[aPairIdConnector.first]._value 
                            << " / weigth "
                            << aPairIdConnector.second->_weight 
                            << " (enabled = "
                            << aPairIdConnector.second->_enabled
                            << " )"
                            << endl;
                    }
                    if (aPairIdConnector.second->_enabled) {                            
                        aNode._value +=
                                _nodes[aPairIdConnector.first]._value 
                                * aPairIdConnector.second->_weight; 
                    }
                            
                }
                // if the node is hidden, apply sigmoid
                if (aNode.isHidden()) {
                    aNode._value = aNode._value / (1.0 + abs(aNode._value));
                }
                
                if (verbose) {cerr << "Value = "<< aNode._value << endl;}
                aNode.setUsed();
            }
            else {
                NodesThatCannotBeComputedYet.push_back(aNode._id);
            }
        }
        listOfNodesToCompute = NodesThatCannotBeComputedYet;
    }

    
    // return output
    vector<float> aOutput;
    for (int const& aOutputNodeId: _outputLayer) {
        aOutput.push_back(_nodes[aOutputNodeId]._value);
    }
    return aOutput;
};


// MUTATIONS
void Genome::mutateOrNot(int forPointMutate, int forNodeMutate, int forEnableDisableMutate) {
    if (rand()%forPointMutate == 0) {
        pointMutate();
    }
    if (rand()%forNodeMutate == 0) {
        nodeMutate();
    }
    if (rand()%forEnableDisableMutate == 0) {
        enableDisableMutate();
    }
};
// pointMutate modifies a weight radomly or according to a step
void Genome::pointMutate() {
    _connections.at(rand()%_connections.size()).setRandomWeight();
};
// nodeMutate silence a connection and adds a connection between the 2 nodes adding 1 new intermediary node 
void Genome::nodeMutate() {
    // get connection enabled
    int index = rand()%_connections.size();
    while (!_connections.at(index)._enabled) {
        index = rand()%_connections.size();
    }        
    
    // disable existing connection
    _connections.at(index).disable();
    
    // have we already faced the same structural innovation ?
    int inputId = _connections.at(index)._inputNodeId;
    int outputId = _connections.at(index)._outputNodeId;
//    cerr << "mutating "<<inputId<< " <=== "<<outputId<< endl;
    
    InnovationBank* aInnovationBank = InnovationBank::instance();
    // if yes, retieve the innovation usin the pair input/output
    
    std::tuple<int, int, int> aIdTuple = aInnovationBank->getInnovationNumbersAndNodeId(inputId, outputId);
    // format : newNodeId, inputConnectionId, outputConnectionId)
    
    // add new gene
    NodeGene aNodeGene("hidden", std::get<0>(aIdTuple)); // hidden by default
    _nodes.insert(std::pair<int, NodeGene>(aNodeGene._id, aNodeGene));
    _hiddenLayer.push_back(aNodeGene._id);
    
    // add new connections
    ConnectionGene aConnection1(_connections.at(index)._inputNodeId, aNodeGene._id, std::get<1>(aIdTuple));
    aConnection1._weight = 1.0;
    _connections.push_back(aConnection1);
    
    ConnectionGene aConnection2(aNodeGene._id, _connections.at(index)._outputNodeId, std::get<2>(aIdTuple));
    aConnection2._weight = _connections.at(index)._weight;
    _connections.push_back(aConnection2);
    
    
    // register if necessary the innovation
    if (aInnovationBank->isInnovationNew(inputId, outputId)) {
//        cerr << "Innovation is new!" << endl;
        aInnovationBank->registerInnovation(
                            inputId, 
                            outputId, 
                            aNodeGene._id, 
                            aConnection1._innovationNumber, 
                            aConnection2._innovationNumber);
    }
//    else {
//        cerr << "Innovation is NOT new!" << endl;
//    }
};
// enable or disable connection
void Genome::enableDisableMutate() {
    _connections.at(rand()%_connections.size()).switchEnableDisable();
};
// crossOver 
Genome Genome::crossOver(const Genome& fitest, const Genome& weakest, bool equal) {
//    cerr << " ---------- XOVER ---------" << endl;

parents: 

/*
 Genome 129 ---------------------------- 
List of input nodes  : 106 107 
List of output nodes : 108 
List of hidden nodes : 123 132 
List of active connections : 
107 ===(0 / N-3)===> 108
123 ===(2 / N-5)===> 108
106 ===(1 / N-10)===> 132
132 ===(1 / N-11)===> 123
List of inactive connections : 
106 ===(2 / N-2)===> 108
106 ===(1 / N-4)===> 123


 Genome 126 ---------------------------- 
List of input nodes  : 106 107 
List of output nodes : 108 
List of hidden nodes : 128 
List of active connections : 
106 ===(1 / N-2)===> 108
107 ===(1 / N-8)===> 128
128 ===(1 / N-9)===> 108
List of inactive connections : 
107 ===(1 / N-3)===> 108

child: 

 Genome 136 ---------------------------- 
List of input nodes  : 106 107 
List of output nodes : 108 
List of hidden nodes : 123 137 
List of active connections : 
106 ===(1 / N-2)===> 108
107 ===(0 / N-3)===> 108
123 ===(2 / N-5)===> 108
128 ===(1 / N-9)===> 108
List of inactive connections : 
106 ===(1 / N-4)===> 123
*/



    // the fitest will  be the model
    Genome aReturnedGenome;
     // build connections
     for (ConnectionGene aCon : fitest._connections) {
         // common connections are inherited randomley from the parents 
         if (weakest.hasConnectionId(aCon._innovationNumber)) {  
             if (rand()%2 == 0) {  // get from weakest
                 aCon = weakest.getConnectionFromInnovationNumber(aCon._innovationNumber);
             }
            aReturnedGenome.addConnection(aCon);
         }
         // nodes in excess or  disjoint:
         // - equal fitness = take randomly 50% chance
         // - or : take from fitest
         else  if (!equal || (rand()%2 == 0)) {
            aReturnedGenome.addConnection(aCon);
         }
     }
     if (equal) {
        for (ConnectionGene aCon : weakest._connections) {
            if (!fitest.hasConnectionId(aCon._innovationNumber) && (rand()%2 == 0)) { 
                 aReturnedGenome.addConnection(aCon);
            }
        }
     }
     
//     if (fitest._id == 129) {
//        aReturnedGenome.display();
//     }
     
     // reconstruct nodes
     // step 1 : merge the 2 maps from fitest ans weakest
     map<int, NodeGene> aMapForFitest = fitest.getNodeMap();
     map<int, NodeGene> aMapForWeakest = fitest.getNodeMap();
     aMapForFitest.insert(aMapForWeakest.begin(), aMapForWeakest.end());
     
     // setp 2 : for each node found in connections, we add it
    for (const ConnectionGene& aCon : aReturnedGenome._connections) {
        if (!aReturnedGenome.isNodeRegistered(aCon._inputNodeId)) {
            aReturnedGenome.addNode(aMapForFitest[aCon._inputNodeId]);
        }
        if (!aReturnedGenome.isNodeRegistered(aCon._outputNodeId)) {
            aReturnedGenome.addNode(aMapForFitest[aCon._outputNodeId]);
        }
     }
     
     // apply 25% chances to reactivate disabled genes
     for (ConnectionGene& aCon : aReturnedGenome._connections) {
         if (!aCon._enabled && rand()%4==0) {
             aCon.switchEnableDisable();
         }
     }
     
     return aReturnedGenome;
};

// return a map  first=id of upper node,  second=pointer to connection
map<int, ConnectionGene*> Genome::getUpperNode(int aNodeId) {
    map<int, ConnectionGene*> mapOfNodesIdAndConnection;
    for (ConnectionGene& aConnectionGene: _connections) {
        if (aConnectionGene._outputNodeId == aNodeId) {
            mapOfNodesIdAndConnection.insert(
                mapOfNodesIdAndConnection.begin(),
                std::pair<int, ConnectionGene*>(aConnectionGene._inputNodeId, &aConnectionGene)
                );
        }
    }
    return mapOfNodesIdAndConnection;
};

bool Genome::hasConnectionWith(int aNodeId) const {
    auto it = find_if(
            _connections.begin(), 
            _connections.end(), 
            [aNodeId] (const ConnectionGene& aConnnectionGene) { 
                    return aConnnectionGene._inputNodeId == aNodeId 
                        || aConnnectionGene._outputNodeId == aNodeId;});
    return (it!=_connections.end());
};

bool Genome::hasConnectionId(int aConId) const {
    auto it = find_if(
            _connections.begin(), 
            _connections.end(), 
            [aConId] (const ConnectionGene& aConnnectionGene) { 
                    return aConnnectionGene._innovationNumber == aConId;});
    return (it!=_connections.end());
};

void Genome::randomizeWeight() {
    for (ConnectionGene& aCon: _connections) {
        aCon.setRandomWeight();
    }
};

void Genome::sortConnectionByInnovationNumber() {
    std::sort(_connections.begin(),
        _connections.end());
};

void Genome::display() const {
//    sortConnectionByInnovationNumber();
    cerr << endl << " Genome " << _id << " ---------------------------- " << endl 
        << "List of input nodes  : ";
    for (int n : _inputLayer) {
        cerr << n << " " ;
    }
    cerr << endl << "List of output nodes : ";
    for (int n : _outputLayer) {
        cerr << n << " " ;
    }
    cerr << endl << "List of hidden nodes : ";
    for (int n : _hiddenLayer) {
        cerr << n << " " ;
    }
    cerr << endl<< "List of active connections : "<<endl;
    
    for (const ConnectionGene& c: _connections) {
        if (c._enabled) {
            cerr << c._inputNodeId 
            << " ===(" << c._weight << " / N-" <<c._innovationNumber <<")===> " << c._outputNodeId << endl;
        }
    }
    cerr << "List of inactive connections : "<<endl;
        for (const ConnectionGene& c: _connections) {
        if (!c._enabled) {
            cerr << c._inputNodeId 
            << " ===(" << c._weight << " / N-" <<c._innovationNumber <<")===> " << c._outputNodeId << endl;
        }
    }
    cerr << endl;
};
