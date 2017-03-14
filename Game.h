#include <vector>
#include <cmath>

class Game {
public:
    template <class T>
    static float evalSolution(const T& aSolution ) {
        // to get the result of the current predictive/decision model, call applyInput
        T aSolutionCopy = aSolution;
        
        ////////////////////////////////////////////
        // here goal is to approximate sin(x) * 0.8 
        
        float anAverageDeviation = 0.0;
        float angle;
        float aPrediction;
        float anExpectedResult;
        // inputs must be randomized in most cases
        
        ///////////////////////////////////////////////////////////////////////
        // input data needs to be normalized aka be in [-4, 4] with a mean of 0 \
        // for current activation function ( x/(1+|x|) ) 
        // 
        // here a is the angle in radian between [-pi, pi]
        
        for (int i = 0; i < 36; i++) {
            angle = float(i*10 - 180) * 3.14159265359/180.; 
            
            aPrediction = aSolutionCopy.execute({angle}).at(0);
            anExpectedResult = f(angle);
            
            // equals 1.9 at maximun => divide by 3.61 to have it normalise error
            anAverageDeviation += (aPrediction - anExpectedResult) * (aPrediction - anExpectedResult) / 3.61;
//            anAverageDeviation += abs(aPrediction - anExpectedResult)/1.8;
        }
        anAverageDeviation *= 1.0/36.0;

        
        return 1.0 - (anAverageDeviation);
    };
    
    static float f(float x) {
        return sin(x) * 0.9;
    }
};