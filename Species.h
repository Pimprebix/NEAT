#ifndef SPECIES_H
#define SPECIES_H
#include "Genome.h"
#include "IDGenerator.h"
//#include "
#include <vector>

class Species
{
public:
    static float THRESHOLD;
    
    Species(int aId = -1);
    bool testGenomeForSpecies(const Genome& iGenome) const;
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
    return Genome::getDistance(iGenome, aRefenceGenome) < Settings::THRESHOLD;
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

inline Species::Species(int aId) {
    if (aId == -1) {
        _id = IDGenerator::instance()->getId();
    }
    else {
        _id = aId;
    }
};


#endif // SPECIES_H
