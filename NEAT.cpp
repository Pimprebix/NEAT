#include <ctime>
#include "NEAT.h"

NEAT::NEAT(int nbInput, int nbOutput) {
    srand(time(0)); // seed rand()
    _nbInput = nbInput; // bias is considered as an input and is manage at genome level
    _nbOutput= nbOutput;
    initialize();
}

void NEAT::initialize() {
    _species.clear();
    _globalPopulation.clear();
    
    // create wanted numbers of genomes with minimal topology within one species
    Species aSpecies;
    Genome aModel(_nbInput, _nbOutput);
    for (int i=0; i<Settings::POPULATION_SIZE; i++) {
        Genome aGenome = aModel;
        aGenome.getNewId();
        aGenome.randomizeWeight();
        aSpecies.addGenome(aGenome);
    }   
    _species.push_back(aSpecies);
}

void NEAT::assignToSpecies(Genome iGenome) {
    for (Species& aSpecies : _species) {
        if (aSpecies.testGenomeForSpecies(iGenome)) {
            aSpecies.addGenome(iGenome);
            return;
        }
    }
    Species aNewSpecies;
    aNewSpecies.addGenome(iGenome);
    _species.push_back(aNewSpecies);
};