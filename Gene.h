#ifndef GENE_H
#define GENE_H

#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

class Gene {
public:
    Gene();
    void setUsed();
    void setUnused();
    bool isUsed();
    float _value; // weight for connection / output for nodes 
protected:
    bool _isUsed;
};

inline void Gene::setUsed() {
    _isUsed = true;
};
inline void Gene::setUnused() {
    _isUsed = false;
};
inline bool Gene::isUsed() {
    return _isUsed; 
};


#endif // GENE_H
