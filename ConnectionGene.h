#ifndef CONNECTIONGENE_H
#define CONNECTIONGENE_H
#include "Gene.h"

class ConnectionGene : public Gene {
public:
    ConnectionGene(int aStart, int aEnd, int aInnovationNumber=-1);

    void setRandomWeight();
    void disable();
    void switchEnableDisable();
    
    int _inputNodeId;
    int _outputNodeId;
    bool _enabled;
    float _weight;
    int _innovationNumber;
};


inline void ConnectionGene::setRandomWeight() {
    _weight = float(rand()%5) - 2.0;
};
inline void ConnectionGene::disable() {
    _enabled = false;  
};
inline void ConnectionGene::switchEnableDisable() {
    _enabled = !_enabled;  
};


#endif // CONNECTIONGENE_H
