#include "montecarlo.h"

#include <cmath>
#include <random>

// Monte Carlo simulation function
double monteCarloSimulation(int totalPoints) {
    // Initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    // Count of points inside the quarter circle
    int insideCircle = 0;

    // Generate random points and check if they are inside the quarter circle
    for (int i = 0; i < totalPoints; ++i) {
        double x = dis(gen);
        double y = dis(gen);

        // Check if the point is inside the quarter circle
        if (std::sqrt(x * x + y * y) <= 1.0) {
            insideCircle++;
        }
    }

    // Estimate the value of pi
    return 4.0 * insideCircle / totalPoints;
}
