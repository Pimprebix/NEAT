#include <iostream>
#include <stdio.h>
#include <IDGenerator.h>
#include "Genome.h"
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using namespace std;




class Population {
public:
    void breed() {
        if (_genomes.empty()) {
            
        }
    }; // throught mutation xOver
    void select() {};
    void getBest() {};
    
    // PARAMETERS
    int _populationSizeMax;
    int _numberOfInput;
    int _numberOfOuput;
    
private:
    vector<Genome> _genomes;
};


int main(int argc, char **argv)
{
//    Simulator aSimulator;
    Genome aGenome(6, 10);
    aGenome.buildMinimalStructure();
    aGenome.nodeMutate();
    aGenome.nodeMutate();
    aGenome.nodeMutate();
    aGenome.nodeMutate();
    
    vector<int> input = {1,8,1,8,4,1};
    vector<int> output = aGenome.applyInput(input);
    
    
    for (int i  = 0; i<output.size();i++) {
        cerr << "output "<< i+1<< " = " << output.at(i) << endl;
    }    
    


    
	return 0;
};
