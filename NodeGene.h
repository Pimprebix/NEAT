#ifndef NODEGENE_H
#define NODEGENE_H
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
using namespace std;
class NodeGene {
public:
    NodeGene(string iType = "hidden", int aId = -1); 
    void reset();
    bool isHidden() const;
    bool isInput() const;
    bool isOutput() const;
    bool isBias() const;
    void setUsed();
    void setUnused();
    bool isUsed();
    int _id; // id
    float _value; 
protected:
    bool _isUsed;
    string _type;
};

inline bool NodeGene::isHidden() const {
    return _type=="hidden"; 
};
inline bool NodeGene::isInput() const {
    return _type=="input"; 
};
inline bool NodeGene::isOutput() const {
    return _type=="output"; 
};
inline bool NodeGene::isBias() const {
    return _type=="bias"; 
};
inline void NodeGene::reset() {
    if (!isBias()) {
        _value = 0.0;
    }
    else {
        _value = 1.0;
    }
    setUnused();  
};
inline void NodeGene::setUsed() {
    _isUsed = true;
};
inline void NodeGene::setUnused() {
    _isUsed = false;
};
inline bool NodeGene::isUsed() {
    return _isUsed; 
};

#endif // NODEGENE_H
