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
    addNode(NodeGene("bias")); // will be stored in inputLayer
    
    for (int const& aId1: _inputLayer) {
        for (int const& aId2: _outputLayer) {
            addConnection(ConnectionGene(aId1, aId2));       
        }     
    }    
    // set id
    if (id == -1) {
        getNewId();
    }
    else {
        _id = id;
    }
    _maxInnovationNumber = 0;
};


// MUTATIONS ////////////////////////////////////////
// pointMutate modifies a weight radomly or according to a step
void Genome::pointMutate() {
    _connections.at(rand()%_connections.size()).pointMutate();
};
// nodeMutate silence a connection and adds a connection between the 2 nodes adding 1 new intermediary node 
void Genome::nodeMutate() {
    vector<int> aListOfEligibleNodes;
    int aBiasId = _biasLayer.at(0);
    for (const ConnectionGene& aConnection : _connections) {
        if (aConnection.isEnabled() && (aConnection._inputNodeId != aBiasId) ) { // !_nodes[aConnection._inputNodeId].isBias())
            aListOfEligibleNodes.push_back(aConnection._innovationNumber);
        }
    }
    if (aListOfEligibleNodes.empty()) {
        return; // all links are mutated. it should never happen.
    }
        
    ConnectionGene& aConnection = getConnectionFromInnovationNumber(
            aListOfEligibleNodes.at(rand()%aListOfEligibleNodes.size())
            );
    
    // disable existing connection
    aConnection.disable();

    // have we already faced the same structural innovation ?
    InnovationBank* aInnovationBank = InnovationBank::instance();
    // if yes, retieve the innovation usin the pair input/output
    
    int inputNodeId = aConnection._inputNodeId;
    int outputNodeId = aConnection._outputNodeId;
    std::tuple<int, int, int> aIdTuple 
        = aInnovationBank->getInnovationNumbersAndNodeId(
            inputNodeId, 
            outputNodeId);
    // format : newNodeId, inputConnectionId, outputConnectionId, biasId)
    
    
    // add new gene
    NodeGene aNodeGene("hidden", std::get<0>(aIdTuple)); // hidden by default
    addNode(aNodeGene, inputNodeId); // we need to insert it right after the inputNodeId
    
    // add new connections
    ConnectionGene aConnection1(inputNodeId, aNodeGene._id, std::get<1>(aIdTuple));
    aConnection1._weight = 1.0;
    _connections.push_back(aConnection1);

    ConnectionGene aConnection2(aNodeGene._id, outputNodeId, std::get<2>(aIdTuple));
    aConnection2._weight = aConnection._weight;
    _connections.push_back(aConnection2);
    
//  each hidden Node can be connected to a bias but the algo will figure it out    
//    ConnectionGene aConnection3(_inputLayer.back(), aNodeGene._id, std::get<3>(aIdTuple));
//     aConnection3._weight = 0.0;
//    _connections.push_back(aConnection3);
    
    
    // maximum innovation number to register (will be used for Xover)
    _maxInnovationNumber = max({_maxInnovationNumber, 
                    aConnection1._innovationNumber,
                    aConnection2._innovationNumber});
    
    // register if necessary the innovation
    if (aInnovationBank->isInnovationNew(inputNodeId, outputNodeId)) {
//        cerr << "Innovation is new!" << endl;
        aInnovationBank->registerInnovation(
                            inputNodeId,   // original connection
                            outputNodeId,  // original connection
                            aNodeGene._id, 
                            aConnection1._innovationNumber, 
                            aConnection2._innovationNumber);
    }
};
// enable or disable connection
void Genome::enableDisableMutate() {
    _connections.at(rand()%_connections.size()).switchEnableDisable();
};
// crossOver 
Genome Genome::crossOver(const Genome& fitest, const Genome& weakest, bool equal) {
    // the fitest will  be the model
    Genome aReturnedGenome;
     // build connections
     for (ConnectionGene aCon : fitest._connections) {
         // common connections are inherited randomley from the parents 
         if (weakest.hasConnectionId(aCon._innovationNumber)) {  
             if (rand()%2 == 0) {  // get from weakest
                 aCon = weakest.getCopyConnectionFromInnovationNumber(aCon._innovationNumber);
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
     
     // reconstruct nodes
     // step 1 : merge the 2 maps from fitest ans weakest
     map<int, NodeGene> aMapForFitest = fitest.getNodeMap();
     map<int, NodeGene> aMapForWeakest = weakest.getNodeMap();
     aMapForFitest.insert(aMapForWeakest.begin(), aMapForWeakest.end());
     
     // step 2 : add bias nodes
     for (const int aInputNodeId : fitest._biasLayer) {
         aReturnedGenome.addNode(aMapForFitest[aInputNodeId]);
     }
     
     // step 3 : for each node found in connections, we add it
     // we still  need to have them ordered in _allNodes     
    for (const ConnectionGene& aCon : aReturnedGenome._connections) {
        if (!aReturnedGenome.isNodeRegistered(aCon._inputNodeId)) {
            aReturnedGenome.addNode(aMapForFitest[aCon._inputNodeId]);
        }
        if (!aReturnedGenome.isNodeRegistered(aCon._outputNodeId)) {
            aReturnedGenome.addNode(aMapForFitest[aCon._outputNodeId]);
        }
     }
     aReturnedGenome.sortAllNodes();
     
     
     // apply 25% chances to reactivate disabled genes
     for (ConnectionGene& aCon : aReturnedGenome._connections) {
         if (!aCon._enabled && rand()%4==0) {
             aCon.switchEnableDisable();
         }
     }
     
//    cerr << "-------------" << endl;
//    fitest.display();
//    weakest.display();
//    aReturnedGenome.display();
//    cerr << "-------------" << endl;
     return aReturnedGenome;
};
// createConnection
void Genome::createConnection() {
    std::vector<int> inputCandidateIds = _hiddenLayer;
    for (int aNodeId : _inputLayer) {
        inputCandidateIds.push_back(aNodeId);
    }
    for (int aNodeId : _biasLayer) {
        inputCandidateIds.push_back(aNodeId);
    }
    int i = inputCandidateIds.at(rand()%inputCandidateIds.size());
    
    // output candidates are 
    // 1- not upstream, 
    // 2- not already linked
    // 3- not itself
    std::vector<int> anExclusionList;
    anExclusionList.push_back(i);
    // nodes upstream
    map<int, vector<ConnectionGene> > aMapOfIncommingConnections = getNodeIncomingConnectionMap();
    bool allNetworkScanned = false;
    while (!allNetworkScanned) {
        allNetworkScanned = true;
        for (int aNodeIdExcluded: anExclusionList) {
            for (const ConnectionGene& aCon: aMapOfIncommingConnections[aNodeIdExcluded]) {
                if (std::find(anExclusionList.begin(),
                                anExclusionList.end(),
                                aCon._inputNodeId) == anExclusionList.end() ) {
                    // if the connection is linked to an updstream node
                    anExclusionList.push_back(aCon._inputNodeId);
                    allNetworkScanned = false;
                }
            }
                
        }
    } 
    // nodes already linked
    for (const ConnectionGene& aCon: _connections) {
        if (aCon._inputNodeId == i) {
            anExclusionList.push_back(aCon._outputNodeId);
        }
    }
    // build list based on hidden and output layers, fitlering out the nodes on the exclusion list
    std::vector<int> outputCandidateIds;
    for (int aNodeId : _hiddenLayer) {
        if (std::find(anExclusionList.begin(),
                    anExclusionList.end(),
                    aNodeId) == anExclusionList.end()) {
            outputCandidateIds.push_back(aNodeId);
        }
    }
    for (int aNodeId : _outputLayer) {
        if (std::find(anExclusionList.begin(),
                    anExclusionList.end(),
                    aNodeId) == anExclusionList.end()) {
            outputCandidateIds.push_back(aNodeId);
        }
    }
    
    if (!outputCandidateIds.empty()) { // TODO: register innovation
        int o = outputCandidateIds.at(rand()%outputCandidateIds.size());
//        cerr << "we create a new connection from "<< i << " to " << o << endl;
        ConnectionGene aNewConnection(i, o);
        _connections.push_back(aNewConnection);
    }
    else {
//        cerr << "we cannot create a new connection" << endl;
    }
}

// MISC //////////////////////////////////////
// sorting
void Genome::sortAllNodes() {
    bool sorted = false;
    while (!sorted) {
        sorted = true;
        for (const ConnectionGene& aCon : _connections) {
            auto i = std::find(_allNodes.begin(), _allNodes.end(), aCon._inputNodeId);
            auto o = std::find(_allNodes.begin(), _allNodes.end(), aCon._outputNodeId);
            // we need to verify that o is after i in _allNodes
            if (std::distance(i,o)<0) {
                // if not switch positions
                std::iter_swap(i, o);
                sorted = false;
            }
        }
    }
}

// testing
bool Genome::hasConnectionWithNodeId(int aNodeId) const {
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



// NETWORK RUN ///////////////////////////////
map<int, vector<ConnectionGene> > Genome::getNodeIncomingConnectionMap() const {
    // returns a map from node id to all incoming connections (for which the node id is registered as the output)
    std::map<int, vector<ConnectionGene> > aMap;
    for (int nodeId : _allNodes) {
        vector<ConnectionGene> incomingConnections;
        for (ConnectionGene aConnection: _connections) {
            if (aConnection._outputNodeId == nodeId) {
                incomingConnections.push_back(aConnection);
            }
        }
        aMap.insert(std::make_pair(nodeId, incomingConnections));
    }
        
    return aMap;
};
// runs network!
vector<float> Genome::execute(const vector<float>& input) {
    
    if (input.size() != _inputLayer.size()) { 
        cerr << "invalid input : not enough input nodes or too much: entries number = "
                << input.size() << " / input nodes number = " << _inputLayer.size() << endl;
        throw "Error : invalid input ";
    }
    else {
        // clean network
        for (int nodeId : _allNodes) {
            _nodes[nodeId].reset();
        }
        // fill inputs layer with values
        for (unsigned int i=0; i<_inputLayer.size(); i++) { 
            _nodes[_inputLayer.at(i)]._value = input.at(i);
        }
    }

    map<int, vector<ConnectionGene> > aNodeInputMap = getNodeIncomingConnectionMap();
    for (int nodeId : _allNodes) {
        // they are ordered so we can compute one node after an other
        if (!_nodes[nodeId].isInput() && !_nodes[nodeId].isBias()) {
            float aNodeInput = 0.0;
            // we multiply each input by the weight of the connection
            for (const ConnectionGene& aConnection : aNodeInputMap[nodeId]) {
                if (aConnection._enabled) {
                    aNodeInput += aConnection._weight * _nodes[aConnection._inputNodeId]._value;
                }
            }
            _nodes[nodeId]._value = aNodeInput / (1.0 + abs(aNodeInput));
        }
    }
    
    // return output
    vector<float> aOutput;
    for (int const& aOutputNodeId: _outputLayer) {
        aOutput.push_back(_nodes[aOutputNodeId]._value);
    }
    return aOutput;
};


// UTILS /////////////////////////////////////
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

void Genome::printCPP() const {
    cerr<< "----------------------------------------------------------"<<endl;
    map<int, vector<ConnectionGene> > aNodeInputMap = getNodeIncomingConnectionMap();
    
    map<int, NodeGene> nodesCopy = _nodes; 
    for (int nodeId : _allNodes) {
        // they are ordered so we can compute one node after an other
        if (nodesCopy[nodeId].isInput()) {
            cerr << "float n" << nodeId << " = input ;" << endl;
        }
        if (nodesCopy[nodeId].isHidden()) {
            cerr << "float n" << nodeId << " = 0.0 ;" << endl;        
        }
        if (nodesCopy[nodeId].isOutput()) {
            cerr << "float n" << nodeId << " = 0.0 ; // <= your output" << endl;   
        }
    }
     
    for (int nodeId : _allNodes) {
        if (nodesCopy[nodeId].isOutput() || nodesCopy[nodeId].isHidden()) {
            // we multiply each input by the weight of the connection
            for (const ConnectionGene& aConnection : aNodeInputMap[nodeId]) {
                if (aConnection._enabled) {
                    cerr << "n" << nodeId << " += float(" 
                            << aConnection._weight 
                            << ") * n" << aConnection._inputNodeId
                            << ";" << endl;
                }
            }
            
//            if (nodesCopy[nodeId].isHidden()) {
                cerr << "n" << nodeId 
                        << " = n" << nodeId 
                        << " / (1.0 + abs(n" << nodeId  <<"));" << endl;
//            }
        }
    }
    cerr<< "----------------------------------------------------------"<<endl;
};