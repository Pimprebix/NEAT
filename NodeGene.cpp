#include "NodeGene.h"
#include "IDGenerator.h"

NodeGene::NodeGene(string iType, int aId) : Gene()
{
    if (aId == -1) {
        _id = IDGenerator::instance()->getId();
    }
    else {
        _id = aId;
    }
    _type = iType;
}

