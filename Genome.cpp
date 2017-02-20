#include "Genome.h"
#include "InnovationBank.h"


Genome::Genome(int numberOfInputNode, int numberOfOuputNode) {
    // add input nodes
    for(int i=0; i<numberOfInputNode; i++) {
        NodeGene aNodeGene("input");
        _nodes.insert(_nodes.begin(), std::pair<int, NodeGene>(aNodeGene._id, aNodeGene));
        _inputLayer.push_back(aNodeGene._id);
    }
    // add output nodes
    for(int i=0; i<numberOfOuputNode; i++) {
        NodeGene aNodeGene("ouput");
        _nodes.insert(_nodes.begin(), std::pair<int, NodeGene>(aNodeGene._id, aNodeGene));
        _outputLayer.push_back(aNodeGene._id);
    }
};
    
void Genome::buildMinimalStructure() {
    // we want a minimal structure : add connections randomly
    // every node should be linked
    
    // start with input layer
    for (int const& aId: _inputLayer) {
        // new random connection
        _connections.push_back(ConnectionGene(aId, _outputLayer.at(rand()%_outputLayer.size()) ));            
    }
    // verify all output node is linked
    for (int const& aId: _outputLayer) {
        // new random connection
        if (!hasConnection(aId)) {
            _connections.push_back(ConnectionGene(_inputLayer.at(rand()%_inputLayer.size()), aId ));
        }           
    }       
};

// fills inputs, set nodes to unused
void Genome::prepareNetwork(const vector<int>& input) {
    // clean network
    for (auto& aPairIntNode : _nodes) { 
        aPairIntNode.second.reset();
    }
    // fill inputs layer
    for (int i=0; i<_inputLayer.size(); i++) {
        _nodes[_inputLayer.at(i)]._value = input.at(i);
        _nodes[_inputLayer.at(i)].setUsed();
    }
};

// runs network!
vector<int> Genome::applyInput(const vector<int>& input) {
    bool verbose = true;
    
    if (input.size() != _inputLayer.size()) {
        cerr << "invalid input" << endl;
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
    vector<int> aOutput;
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
    
    InnovationBank* aInnovationBank = InnovationBank::instance();
    // if yes, retieve the innovation usin the pair input/output
    
    
    
    if (aInnovationBank->isInnovationNew(inputId, outputId)) {
        NodeGene aNodeGene("hidden"); // hidden by default
        _nodes.insert(std::pair<int, NodeGene>(aNodeGene._id, aNodeGene));
        _hiddenLayer.push_back(aNodeGene._id);
        
        // add new connections
        ConnectionGene aConnection1(_connections.at(index)._inputNodeId, aNodeGene._id);
        aConnection1._weight = 1.0;
        _connections.push_back(aConnection1);
        
        ConnectionGene aConnection2(aNodeGene._id, _connections.at(index)._outputNodeId);
        aConnection2._weight = _connections.at(index)._weight;
        _connections.push_back(aConnection2);
        
        aInnovationBank->registerInnovation(
                            inputId, 
                            outputId, 
                            aNodeGene._id, 
                            aConnection1._innovationNumber, 
                            aConnection2._innovationNumber);
        
    }
    // if not 
    else {
        
    }

    
};
// enable or disable connection
void Genome::enableDisableMutate() {
    _connections.at(rand()%_connections.size()).switchEnableDisable();
};
    
// SETTER
void Genome::setFitness(float iFitness) {
  _fitness = iFitness;
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

bool Genome::hasConnection(int aNodeId) {
    auto it = find_if(
            _connections.begin(), 
            _connections.end(), 
            [aNodeId] (const ConnectionGene& aConnnectionGene) { 
                    return aConnnectionGene._inputNodeId == aNodeId 
                        || aConnnectionGene._outputNodeId == aNodeId;});
    return (it!=_connections.end());
};


