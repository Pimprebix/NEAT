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
    bool isHidden();
    bool isInput();
    bool isOuput();
    void setUsed();
    void setUnused();
    bool isUsed();
    int _id; // id
    float _value; 
protected:
    bool _isUsed;
    string _type;
};

inline bool NodeGene::isHidden() {
    return _type=="hidden"; 
};
inline bool NodeGene::isInput() {
    return _type=="input"; 
};
inline bool NodeGene::isOuput() {
    return _type=="output"; 
};
inline void NodeGene::reset() {
    _value = 0.0;
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
