#include "ConnectionGene.h"
#include "InnovationBank.h"

ConnectionGene::ConnectionGene(int aStart, int aEnd)  : _inputNodeId(aStart), _outputNodeId(aEnd) {
    _enabled = true;
    _weight = float(rand()%5) - 2.0;
    _innovationNumber = InnovationBank::instance()->getInnovationNumber();
};

