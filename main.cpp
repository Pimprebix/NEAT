#include <iostream>
#include "NEAT.h"
#include "Game.h"

#include "cmath"
#include "Genome.h"
using namespace std;


int main(int argc, char **argv)
{
    NEAT aNeat(2,1);
    // we can set the mutation step
    // write here all additional parameters we need (popSize, verbose, options for speciation etc)
//    aNeat.setVerboseMode(false);
//    aNeat.setTerminationAfternbOfTurns(1000);
    aNeat.setInitialPopulationSize(100);
    aNeat.setFitnessFunction(&Game::evalSolution);
    aNeat.run();
    
    
    Genome g = aNeat.getBest();
    
    int a = 39;
    int b = 30;
    cerr << "RESULTS FOR a = " << a << " / b = " << b 
    << ". Error: " << g.applyInput({a,b}).at(0) - (sin(a) * sin(b)) <<endl;
//    aNeat.getBest();
    
	return 0;
};
