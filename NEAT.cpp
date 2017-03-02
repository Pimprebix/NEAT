#include "NEAT.h"


NEAT::NEAT(int nbInput, int nbOutput, int popSize) {
    _nbInput = nbInput;
    _nbOutput= nbOutput;
    _killRate = 0.20;
    _adoptionPercentage = 10;
    setInitialPopulationSize(popSize);
}

NEAT::~NEAT() {
}

//void NEAT::setFitnessFunction(float (*fitnessFonction) (const Genome&, const vector<int>&) ) {};

void NEAT::initialize() {
    _species.clear();
    // create wanted numbers of genomes with minimal topology within one species
    Species aSpecies;
    Genome aModel(_nbInput, _nbOutput);
    for (int i=0; i<_initialPoolSize; i++) {
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