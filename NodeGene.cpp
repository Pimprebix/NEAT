#include "NodeGene.h"
#include "IDGenerator.h"

NodeGene::NodeGene(string iType) : Gene()
{
    _id = IDGenerator::instance()->getId();
    _type = iType;
}

