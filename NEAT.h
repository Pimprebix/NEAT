#ifndef NEAT_H
#define NEAT_H
#include "Genome.h"
#include "Species.h"
#include <vector>

class NEAT
{
public:
    NEAT(int nbInput, int nbOutput, int popSize=100);
    ~NEAT();
    void setInitialPopulationSize(int iSize);
    void setFitnessFunction(float (*foo) (const Genome&, const vector<int>&) ) {
        _fitnessFunction = foo;
        };
    void run();
private:
    void assignToSpecies(Genome iGenome);
    void initialize();
    void evalFitness();
    void killWeakests();
    void reproduceAndMutate();
    Species& getSpeciesById(int id);
    
    std::vector<Species> _species;
    float (*_fitnessFunction) (const Genome&, const vector<int>&);
    int _initialPoolSize;
    int _nbInput;
    int _nbOutput;
    float _killRate;
    int _adoptionPercentage;
    
};

inline void NEAT::setInitialPopulationSize(int iSize) {
    _initialPoolSize = iSize;
    initialize();
};

inline void NEAT::evalFitness() {
    for (Species& aSpecies : _species) {
        aSpecies.evalFitness(_fitnessFunction);
    }
};

inline void NEAT::killWeakests() {
// 1 - order all species by adjusted fitness
// create vector< tuple<genomeId , speciesId, fitness, adjustedfiteness> > 
    std::vector< std::tuple<int, int, float, float> > globalPopulation;
    for (const Species& aSpecies: _species) {
        for (const Genome& aGenome: aSpecies._members) {
            globalPopulation.push_back(std::make_tuple(aGenome._id, aSpecies._id, aGenome._fitness, aGenome._adjustedFitness));            
        }
    }
    
    // sort ascending order (0 first)
    std::sort(globalPopulation.begin(), globalPopulation.end(),
            [] (const std::tuple<int, int, float, float>& t1, const std::tuple<int, int, float, float>& t2) { return get<3>(t1)<get<3>(t2);});
    
    
//    for (const std::tuple<int, int, float, float>& t : globalPopulation) {
//        cerr << get<0>(t) << " " << get<1>(t) << " " << get<3>(t) << endl;
//    }
    
// 2 - kill the least performing of all based on the killRate (so far)
    int numberOfVictims = abs(globalPopulation.size() * _killRate);
    for (int i =0 ; i <= numberOfVictims; i++) {
        cerr << "Remove genome " << get<0>(globalPopulation.at(i)) << " from species " << get<1>(globalPopulation.at(i)) << " / adjusted fitness = " << get<3>(globalPopulation.at(i)) << endl;
        getSpeciesById(get<1>(globalPopulation.at(i))).removeGenomeById(get<0>(globalPopulation.at(i)));
    }
};

inline void NEAT::reproduceAndMutate()  {
    for (Species& aSpecies: _species) {
        // select parents
        // aSpecies.orderByAdjustedFitness(); ?
        
        //  mutations:
        
    }    
};

inline void NEAT::run() {
  for (int generation = 0 ; generation < 10; generation++ ) {  //termination ?
      evalFitness();  // done
      killWeakests(); // done
      reproduceAndMutate();
  }  
};

inline Species& NEAT::getSpeciesById(int aId) {
    auto it = find_if(
            _species.begin(), 
            _species.end(), 
            [aId] (Species& aSpecies) { 
                    return aSpecies._id == aId;});
    if (it != _species.end()) {
        return *it;
    }
    else {
        cerr << "!!!!!!!!! Species " << aId << " not found! !!!!!!!!!!!!!!!!!!" << endl; 
    }
};
#endif // NEAT_H
