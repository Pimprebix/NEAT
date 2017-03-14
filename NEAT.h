#ifndef NEAT_H
#define NEAT_H
#include "Genome.h"
#include "Species.h"
#include "InnovationBank.h"
#include <vector>

class NEAT
{
public:
    NEAT(int nbInput, int nbOutput);
    void setFitnessFunction(float (*foo) (const Genome&) ) {
        _fitnessFunction = foo;
        };
    void run();
    Genome getBest();
private:
    void assignToSpecies(Genome iGenome);
    void initialize();
    void evalFitness();
    void killWeakests();
    void reproduce();
    void mutate();
    Species& getSpeciesById(int id);
    
    std::vector<Species> _species;
    float (*_fitnessFunction) (const Genome&);
    int _nbInput;
    int _nbOutput;
    std::vector< std::tuple<int, int, float, float> > _globalPopulation;
};

inline void NEAT::evalFitness() {
    for (Species& aSpecies : _species) {
        for (Genome& aGenome : aSpecies._members) { 
            float aFitness = _fitnessFunction(aGenome);
            aGenome.setFitness(aFitness);
            
            if (aFitness < 0.) {
                throw "Error : negative fitness! fitness should be always positive!" ;
            }
            
            // adjusted fitness
            float counter = 0.0;
            for (const Genome& otherGenome : aSpecies._members) { //  it will be at least equal to 1 since the distance from itself is 0
                if (Genome::getDistance(aGenome, otherGenome) < Settings::THRESHOLD) {  // could use some cache here
                    counter +=1.0;
                }
            }
            aGenome.setAdjustedFitness(aFitness / counter);
        }
    }
};

inline void NEAT::killWeakests() {
// 1 - order all species by adjusted fitness    
    _globalPopulation.clear();
    for (const Species& aSpecies: _species) {
        for (const Genome& aGenome: aSpecies._members) {
            _globalPopulation.push_back(std::make_tuple(aGenome._id, aSpecies._id, aGenome._fitness, aGenome._adjustedFitness));            
        }
    }
    
    // sort ascending order (0 first)
    std::sort(_globalPopulation.begin(), _globalPopulation.end(),
            [] (const std::tuple<int, int, float, float>& t1, const std::tuple<int, int, float, float>& t2) { return get<3>(t1)<get<3>(t2);});
    
    
// 2 - kill the least performing of all based on the killRate (so far)
    int numberOfVictims = abs(_globalPopulation.size() * Settings::KILLING_RATE);
    for (int i =0 ; i <= numberOfVictims; i++) {
//        cerr << "Remove genome " << get<0>(_globalPopulation.at(i)) << " from species " << get<1>(_globalPopulation.at(i)) << " / adjusted fitness = " << get<3>(_globalPopulation.at(i)) << endl;
        getSpeciesById(get<1>(_globalPopulation.at(i))).removeGenomeById(get<0>(_globalPopulation.at(i)));
    }
    
    // remove empty species
   _species.erase(
        std::remove_if(
            _species.begin(), 
            _species.end(), 
            [] (Species& aSpecies) { 
                    return aSpecies._members.empty();}),
        _species.end());

    std::reverse(_globalPopulation.begin(), _globalPopulation.end());
    // now best elements are first
    for (int i =0 ; i <= numberOfVictims; i++) {
        _globalPopulation.pop_back();
    }
};

inline void NEAT::reproduce()  {
//    _globalPopulation already exists and is sorted (best first)    
    int numberOfChildren = Settings::POPULATION_SIZE - _globalPopulation.size();
    for (int i = 0 ; i < numberOfChildren ; i++) {
        Species& aSpecies = getSpeciesById(get<1>(_globalPopulation.at(i)));
        
        const Genome& aParent1 = aSpecies.getGenomeById(get<0>(_globalPopulation.at(i)));
        const Genome& aParent2 = aSpecies._members.at(rand()%aSpecies._members.size());
        
        Genome aNewGenome = Genome::crossOver(aParent1, aParent2);
        
        if(false) {
            cerr << "parents: "<<endl;
            aParent1.display();
            aParent2.display();
            cerr << "child: "<<endl;
            aNewGenome.display();
        }

        // adoption mechanism
        if (float(rand()%100)/100. < Settings::ADOPTION_RATE) {
            assignToSpecies(aNewGenome);
        }
        else {
            aSpecies.addGenome(aNewGenome);
        }
    }

};

inline void NEAT::mutate()  {
    for (Species& s: _species) {
        for (Genome& g: s._members) {
            if ((rand() / (float) (RAND_MAX)) < Settings::MUTATION_RATE_NODE )  {
                g.nodeMutate();
            }
            if ((rand() / (float) (RAND_MAX)) < Settings::MUTATION_RATE_WEIGHT )  {
                g.pointMutate();
            }
            if ((rand() / (float) (RAND_MAX)) < Settings::MUTATION_RATE_DISABLE_LINK )  {
                g.enableDisableMutate();
            }
            if ((rand() / (float) (RAND_MAX)) < Settings::MUTATION_RATE_CREATE_LINK )  {
                g.createConnection();
            }
        }
    }
};

inline void NEAT::run() {
  for (int generation = 0 ; generation < Settings::MAX_NUMBER_GENERATION; generation++ ) {  //termination ?
    // clear innovationBank
    InnovationBank::instance()->resetBank();
    
    cerr << endl << " ********************** " << endl; 
    cerr << " Generation : " << generation << endl; 
    cerr << " Number of Species : " << _species.size() << endl; 
    for (const Species& s : _species) {
        cerr << "   - Species " << s._id << " : " << s._members.size() << endl; 
    }
    
    cerr << " 1 - Eval fitness" << endl;
    evalFitness();  // done
    cerr << " 2 - Kill weakests" << endl;
    killWeakests(); 
    cerr << " 3 - Reproduce" << endl;
    reproduce();
    cerr << " 4 - Mutate " << endl;
    mutate();
  }  
};

inline Genome NEAT::getBest() {
    evalFitness();
    Genome theBest;
    theBest._fitness = 0;
    for (const Species& aSpecies: _species) {
        for (const Genome& aGenome: aSpecies._members) {
//            cerr << "current fitness : "<< aGenome._fitness << endl;
            if (aGenome._fitness > theBest._fitness) {
//                cerr << "found better : "<< aGenome._fitness;
                theBest = aGenome;
            }
        }
    }
    theBest.display();
    cerr << "best fitness found = " << theBest._fitness << endl;
    return theBest;
}

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
