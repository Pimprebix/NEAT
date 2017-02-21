#ifndef NODEGENE_H
#define NODEGENE_H
#include "Gene.h"

class NodeGene : public Gene {
public:
    NodeGene(string iType = "hidden", int aId = -1); 
    void reset();
    bool isHidden();
    int _id; // id
private:
    string _type;
};

inline bool NodeGene::isHidden() {
    return _type=="hidden"; 
};
inline void NodeGene::reset() {
    _value = 0.0;
    setUnused();  
};

#endif // NODEGENE_H
