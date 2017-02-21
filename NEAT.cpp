#include "NEAT.h"

NEAT::NEAT(int nbInput, int nbOutput, int popSize)
{
    _nbInput = nbInput;
    _nbOutput= nbOutput;
    setInitialPopulationSize(popSize);
}

NEAT::~NEAT()
{
}

void NEAT::populate() {
    Genome aModel(_nbInput, _nbOutput);
    for (int i=0; i<_initialPoolSize; i++) {
        Genome aGenome = aModel;
        aGenome.randomizeWeight();
        _population.push_back(aGenome);
    }
    
    for (const Genome& g: _population) {
        g.display();
    }
}