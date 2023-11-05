#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define STEPS 1000
//Created with the help of ChatGPT (GPT-4)
//(C)Tsubasa Kato 114/2023 - Inspire Search Corporation
//Website: https://www.inspiresearch.io/en
// Assuming these are global variables that would be dynamically set in a real application.
double difficulty = 0.5;       // Range from 0 (easy) to 1 (hard)
double foreseen_future = 0.7;  // Range from 0 (bad) to 1 (good)
double disturbance = 0.3;      // Range from 0 (calm) to 1 (chaotic)

// Function to generate a random double between -1.0 and 1.0.
double random_double() {
    double value = 2.0 * ((double)rand() / RAND_MAX) - 1.0;
    printf("Random double generated: %f\n", value);
    return value;
}

// Decision making influenced by a weight.
int weighted_decision(double weight) {
    // Generate a probability threshold based on the weight.
    double threshold = (1.0 - weight) * (double)rand() / RAND_MAX;
    int decision = threshold < 0.5 ? 1 : 0;
    printf("Weighted decision made with weight %f: %d\n", weight, decision);
    return decision;
}

// Simulate Brownian motion considering different influencing factors.
void simulate_brownian_motion(int steps, double difficulty, double foreseen_future, double disturbance) {
    double x = 0.0, y = 0.0; // Initial position of the particle.
    printf("Initial position of the particle: (%f, %f)\n", x, y);

    srand((unsigned int)time(NULL)); // Seed the random number generator.

    for (int i = 0; i < steps; i++) {
        printf("\nStarting step %d\n", i + 1);

        // Calculate the combined effect of influencing factors on the decision probability.
        double move_probability = foreseen_future * (1 - difficulty) * (1 - disturbance);
        printf("Calculated move probability: %f\n", move_probability);

        // Make a decision to move in the x-direction.
        double dx = 0.0;
        if (weighted_decision(move_probability)) {
            dx = random_double();
        }
        printf("Decided movement in x-direction: %f\n", dx);

        // Make a decision to move in the y-direction.
        double dy = 0.0;
        if (weighted_decision(move_probability)) {
            dy = random_double();
        }
        printf("Decided movement in y-direction: %f\n", dy);

        // Update the particle's position.
        x += dx;
        y += dy;
        printf("Updated position of the particle: (%f, %f)\n", x, y);
    }
}

// Main function where the program execution begins.
int main() {
    printf("Simulating Brownian Motion with influenced decisions for %d steps.\n", STEPS);
    simulate_brownian_motion(STEPS, difficulty, foreseen_future, disturbance);
    return 0;
}

