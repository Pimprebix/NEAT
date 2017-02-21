#include <iostream>
#include <stdio.h>
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
    Genome aGenome(2, 1);
    Genome aGenome2 = aGenome;
    aGenome2.randomizeWeight();
    aGenome.nodeMutate();
    aGenome.nodeMutate();
    aGenome.nodeMutate();
    aGenome.nodeMutate();
    aGenome.nodeMutate();
    aGenome.nodeMutate();
    aGenome.nodeMutate();
    aGenome.nodeMutate();
    aGenome2.nodeMutate();
    aGenome2.nodeMutate();
    aGenome2.nodeMutate();
    aGenome2.nodeMutate();
    aGenome2.nodeMutate();
    aGenome2.nodeMutate();
    aGenome2.nodeMutate();
    
    vector<int> input = {1,8};
    vector<float> output = aGenome.applyInput(input);
    
    for (int i  = 0; i<output.size();i++) {
        cerr << "output "<< i+1<< " = " << output.at(i) << endl;
    }    
    
    cerr << "-------------------------------" <<endl;
    
    vector<float> output2 = aGenome2.applyInput(input); 
    for (int i  = 0; i<output2.size();i++) {
        cerr << "output "<< i+1<< " = " << output2.at(i) << endl;
    }  

	return 0;
};
