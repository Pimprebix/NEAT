#ifndef SPECIES_H
#define SPECIES_H
#include "Genome.h"
//#include "
#include <vector>

class Species
{
public:
    static float THRESHOLD;
    
    Species(int aId = -1);
    bool testGenomeForSpecies(const Genome& iGenome) const;
    void evalFitness(float (*foo) (const Genome&, const vector<int>&) );
//    void orderByAdjustedFitness();
    void removeGenomeById(int id);
    void addGenome(Genome iGenome);
    Genome& getGenomeById(int aId) ;
    std::vector<Genome> _members;
    int _id;
};

inline void Species::addGenome(Genome iGenome) {
    _members.push_back(iGenome);
};

inline bool Species::testGenomeForSpecies(const Genome& iGenome) const {
    const Genome& aRefenceGenome = _members.at(rand()%_members.size()); 
    return Genome::getDistance(iGenome, aRefenceGenome) < THRESHOLD;
};

inline void Species::evalFitness(float (*foo) (const Genome&, const vector<int>&)  ) {
    for (Genome& aGenome : _members) { 
        cerr << "eval Fitness of genome " << aGenome._id << endl;
        float aFitness = foo(aGenome, {1,1});
        aGenome.setFitness(aFitness);
//        cerr << aFitness << endl;
        
        // adjusted fitness
        float counter = 0.0;
        for (const Genome& otherGenome : _members) {
            if (Genome::getDistance(aGenome, otherGenome)<THRESHOLD) {  // could use some cache here
                counter +=1.0;
            }
        }
        aGenome.setAdjustedFitness(aFitness / counter);
    }
};

inline Genome& Species::getGenomeById(int aId) {
    auto it = find_if(
            _members.begin(), 
            _members.end(), 
            [aId] (Genome& aGenome) { 
                    return aGenome._id == aId;});
    if (it != _members.end()) {
        return *it;
    }
    else {
        cerr << "!!!!!!!!! Genome " << aId << " not found! !!!!!!!!!!!!!!!!!!" << endl; 
    }
};

inline void Species::removeGenomeById(int aId) {
   _members.erase(
        std::remove_if(
            _members.begin(), 
            _members.end(), 
            [aId] (Genome& aGenome) { 
                    return aGenome._id == aId;}),
        _members.end());
    
};
#endif // SPECIES_H
