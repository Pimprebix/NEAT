#include "Species.h"
#include "IDGenerator.h"

float Species::THRESHOLD = 4.0;

Species::Species(int aId) {
    if (aId == -1) {
        _id = IDGenerator::instance()->getId();
    }
    else {
        _id = aId;
    }
};
