#include <iostream>
#include "NEAT.h"
#include "Game.h"

#include "cmath"
#include "Genome.h"
using namespace std;


int main(int argc, char **argv)
{
    NEAT aNeat(1,1);
    aNeat.setInitialPopulationSize(100);
    aNeat.setNbOfGenerations(400);
    aNeat.setFitnessFunction(&Game::evalSolution);

    aNeat.run();   
    Genome g = aNeat.getBest();
    g.display();


    float angle;
    float aPrediction;
    float anExpectedResult;
    for (int i = 0; i < 360; i++) {
        angle = float(i - 180) * 3.14159265359/180.; 
        aPrediction = g.applyInput({angle}).at(0);
        anExpectedResult = Game::f(angle);
        cerr << aPrediction << endl;
    }



    
	return 0;
};
