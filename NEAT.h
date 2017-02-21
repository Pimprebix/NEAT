#ifndef NEAT_H
#define NEAT_H
#include "Genome.h"
#include <vector>

class NEAT
{
public:
    NEAT(int nbInput, int nbOutput, int popSize=100);
    ~NEAT();
    void setInitialPopulationSize(int iSize);
private:
    void populate();
    
    std::vector<Genome> _population;
    int _initialPoolSize;
    int _nbInput;
    int _nbOutput;
    
};

inline void NEAT::setInitialPopulationSize(int iSize) {
    _initialPoolSize = iSize;
    populate();
}

#endif // NEAT_H
