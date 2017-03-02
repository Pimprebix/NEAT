#include <vector>
#include <cmath>

class Game {
public:
    template <class T>
    static float evalSolution(const T& aSolution, const std::vector<int>& input = {} ) {
        // to get the result of the current predictive/decision model, call applyInput
        T aSolutionCopy = aSolution;
        
        // inputs must be randomized in most cases
        // for a race, we csn create a huge circuit and see how much of it can complete within X turns,
        std::vector<float> aPrediciton = aSolutionCopy.applyInput(input);
        
        // here the goal is to get a network to compute sin(a)*sin(b)
        float expected = sin(input.at(0)) * sin(input.at(1));
        
        return 1 - std::abs(aPrediciton.front() - expected);
    };
};