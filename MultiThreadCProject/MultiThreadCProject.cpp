#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>
#include "montecarlo.h"

constexpr int NUM_THREADS = 1000;
constexpr int ARRAY_SIZE = 1000;

struct ThreadData {
    const std::vector<double>* inputArray;
    std::vector<double>* resultArray;
    int startIndex;
    int endIndex;
};

// Global vector to store results
std::vector<double> globalResultVector(ARRAY_SIZE);

// Mutex for protecting access to globalResultVector
std::mutex resultMutex;

void computeInverseSqrt(const ThreadData& data) {
    std::cout << "==== thread begin" << std::endl;
    for (int i = data.startIndex; i < data.endIndex; i++) {
        //double result = 1.0 / std::sqrt((*data.inputArray)[i]);
        std::cout << "==== chrunk begin" << std::endl;
        
        double result = monteCarloSimulation(100000);

        std::cout << "==== chrunk end" << std::endl;

        // Acquire the mutex before updating globalResultVector
        std::lock_guard<std::mutex> lock(resultMutex);
        (*data.resultArray)[i] = result;
        // Release the mutex after updating globalResultVector
    }
}

int main() {
    std::vector<double> inputArray(ARRAY_SIZE);

    // Fill inputArray with random numbers
    for (int i = 0; i < ARRAY_SIZE; i++) {
        inputArray[i] = static_cast<double>(rand()) / RAND_MAX;
    }

    std::vector<std::thread> threads(NUM_THREADS);
    std::vector<ThreadData> threadData(NUM_THREADS);

    // Divide the work among threads
    int chunkSize = ARRAY_SIZE / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; i++) {
        threadData[i] = { 
            &inputArray, 
            &globalResultVector, 
            i * chunkSize, 
            (i == NUM_THREADS - 1) ? ARRAY_SIZE : (i + 1) * chunkSize 
        };

        threads[i] = std::thread(computeInverseSqrt, std::ref(threadData[i]));
    }

    // Wait for threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i].join();
    }

    // Use the globalResultVector as needed
    std::cout << "Size of globalResultVector: " << globalResultVector.size() << std::endl;


    return 0;
}
