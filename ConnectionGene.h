#ifndef CONNECTIONGENE_H
#define CONNECTIONGENE_H
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using namespace std;
class ConnectionGene {
public:
    ConnectionGene(int aStart, int aEnd, int aInnovationNumber=-1);
    bool operator< (const ConnectionGene& other) const;
    void setRandomWeight();
    void disable();
    void switchEnableDisable();
    void pointMutate();
    
    int _inputNodeId;
    int _outputNodeId;
    bool _enabled;
    float _weight;
    int _innovationNumber;
};

inline void ConnectionGene::pointMutate() {
    int r = rand()%4;
    if (r == 0) {
        _weight += 0.1;
    }
    else if (r == 1) {
        _weight -= 0.1;
    }
    else {
//        _weight += (float(rand()%400)/100. - 2.0);
        setRandomWeight();
    }
};
inline void ConnectionGene::setRandomWeight() {
    _weight = (float(rand()%400)/100. - 2.0);
};
inline void ConnectionGene::disable() {
    _enabled = false;  
};
inline void ConnectionGene::switchEnableDisable() {
    _enabled = !_enabled;  
};
inline bool ConnectionGene::operator< (const ConnectionGene& other) const {
    return _innovationNumber<other._innovationNumber;
};

#endif // CONNECTIONGENE_H
