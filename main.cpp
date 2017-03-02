#include <iostream>
#include "NEAT.h"
#include "Game.h"

using namespace std;


int main(int argc, char **argv)
{
    NEAT aNeat(2,1);
    // we can set the mutation step
    // write here all additional parameters we need (popSize, verbose, options for speciation etc)
//    aNeat.setVerboseMode(false);
//    aNeat.setTerminationAfternbOfTurns(1000);
    aNeat.setInitialPopulationSize(10);
    aNeat.setFitnessFunction(&Game::evalSolution);
    aNeat.run();
//    aNeat.getBest();
    
	return 0;
};
