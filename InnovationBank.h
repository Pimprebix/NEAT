#ifndef INNOVATIONBANK_H
#define INNOVATIONBANK_H
#include <iostream>
#include <stdio.h>
#include <map>
#include <utility>
#include <tuple>

class InnovationBank {
public:
    static InnovationBank* instance ();
    int getInnovationNumber ();
    std::tuple<int, int, int> getInnovationNumbersAndNodeId(int inputId, int outputId);
    bool isInnovationNew(int inputId, int outputId);
    void registerInnovation(int inputId, int outputId, int nodeId, int inputConnectionId, int outputConnectionId);
private:
    InnovationBank () : _id(0) {};
    static InnovationBank* _address;
    int _id;
    std::map<std::pair<int, int>, std::tuple<int, int, int> > _innovationMap;
};


inline void InnovationBank::registerInnovation(int inputId, int outputId, int newNodeId, int inputConnectionId, int outputConnectionId) {
    std::pair<int, int> aIdPair = std::make_pair(inputId, outputId);
    std::tuple<int, int, int> aTuple = std::make_tuple(newNodeId, inputConnectionId, outputConnectionId);
    _innovationMap.insert(std::make_pair(aIdPair, aTuple));
};

inline std::tuple<int, int, int> InnovationBank::getInnovationNumbersAndNodeId(int inputId, int outputId) {
    std::pair<int, int> aIdPair = std::make_pair(inputId, outputId);
    if (!isInnovationNew(inputId,outputId)) {
        return _innovationMap[aIdPair];
    }
    else {
        return std::make_tuple(-1,-1,-1);
    }
};

inline int InnovationBank::getInnovationNumber() {
    return _id++; 
};



#endif // INNOVATIONBANK_H
