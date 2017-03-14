#include "Settings.h"

// speciation 
float Settings::EXCESS_DIST_FACTOR  = 2.0 ;
float Settings::DISJOINT_DIST_FACTOR  = 2.0 ;
float Settings::WEIGHT_DIST_FACTOR  = 1.0 ;
float Settings::THRESHOLD = 4.0;

// genetics - reproduction
float Settings::KILLING_RATE = 0.2;
float Settings::ADOPTION_RATE = 0.3;

// genetics - mutations
//   Preliminary experiments indicate that high weight mutation rates (i.e. 50% or more) 
//   are useful for control tasks, but lower rates (i.e. under 1%) are more appropriate for 
//   high input games like Othello. It may be that the number of inputs is the critical factor, 
//   and that low-input tasks respond better to high mutation weights. 
//   Although I do not have concrete statistics from which to draw strong conclusions, 
//   a good rule of thumb is to change the weight mutation rate 
//   if the systems seems to be performing below expectations. 
float Settings::MUTATION_RATE_NODE = 0.1;
float Settings::MUTATION_RATE_WEIGHT = 0.1;
float Settings::MUTATION_RATE_DISABLE_LINK = 0.1;
float Settings::MUTATION_RATE_CREATE_LINK = 0.1;


// population
int Settings::POPULATION_SIZE = 50;
int Settings::MAX_NUMBER_GENERATION = 100;