#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include <mutex>

#define BLOCK_SIZE MAX_PRIME_TARGET / NUM_THREADS


using namespace std;


/**
 * Max limit for our example
 */
const long int MAX_PRIME_TARGET = 100000;


/**
 * Number of threads to be used
 */
const int NUM_THREADS = 16;


/**
 * True if the run must balance the input
 */
const bool IS_BALANCED = true;


/**
 * Number of runs to be executed for one measurement
 */
const int NUM_RUNS = 10;


/**
 * Counter used in the balanced approach
 */
int kCounter = 0;


/**
 * Prime counter used in the balanced approach
 */
int kPrimeCounter = 0;


/**
 * Mutex to protect access to the counter
 */
mutex kCounterMutex;


/**
 * Mutex to protect access to the file descriptor
 */
mutex kFileMutex;


/**
 * Output file for balanced approach
 */
ofstream kOutputFile; // NOLINT(cert-err58-cpp)


/**
 * Checks if a given number is prime.
 *
 * @param n the number to be checked
 * @return True if is prime
 */
bool isPrime(long int n) {

    // Enforce 0 and 1 are not prime numbers
    if (n == 0 || n == 1) {
        return false;
    }

    // Check if the number is prime
    for (int inx = 2; inx < n; inx++) {

        // If the number is divisible by inx then it is not prime
        if ((n % inx) == 0) {
            return false;
        }
    }

    // The number is prime
    return true;
}


/**
 * Thread to find primes in a given block
 *
 * @param blockStart start index
 * @param blockEnd end index
 */
void findPrimesThreadUnbalanced(long int blockStart, long int blockEnd) {

    // Get the thread ID
    ostringstream threadIdStream;
    threadIdStream << this_thread::get_id();
    string threadId = threadIdStream.str();

    printf("[%s] START: %ld | END: %ld\n", threadId.c_str(), blockStart, blockEnd);
    cout << flush;

    // Open a file to write this thread result
    ofstream outputFile;
    string filename = "primes_" + threadId + ".txt";
    outputFile.open(filename.c_str());

    // Check how many prime numbers exist in the block
    int numPrimes = 0;
    for (long int i=blockStart; i < blockEnd; i++) {
        if (isPrime(i)) {
            numPrimes++;
            outputFile << i << "\n";
        }
    }
    printf("[%s] START: %ld | END: %ld | NUM_PRIMES: %d\n", threadId.c_str(), blockStart, blockEnd, numPrimes);
    cout << flush;
    outputFile.close();
}


/**
 * Get the next number from a shared counter.
 *
 * @return The next number to compute
 */
long getNextNumber() {
    const lock_guard<std::mutex> lock(kCounterMutex);
    return kCounter++;
}


/**
 * Thread to find primes querying the next number to check.
 */
void findPrimesThreadBalanced() {
    // Get the thread ID
    ostringstream threadIdStream;
    threadIdStream << this_thread::get_id();
    string threadId = threadIdStream.str();

    printf("[%s] Start\n", threadId.c_str());
    cout << flush;

    // Execute until the target is reached
    long number;
    while ((number = getNextNumber()) < MAX_PRIME_TARGET) {

        // Check if the number is prime
        if (isPrime(number)) {
            const lock_guard<std::mutex> lock(kFileMutex);

            // Save to the file
            kOutputFile << number << "\n";

            // Increment the prime counter
            kPrimeCounter++;
        }
    }

    printf("[%s] Finish\n", threadId.c_str());
    cout << flush;
}


/**
 *  Assess how many prime numbers exist between 0 and 10ˆ5.
 *
 *  The input data will be divided in equal slices if the execution is unbalanced. If the balanced approach is
 *  requested, the thread will ask for a new number to check in each cycle.
 *
 * @param balanced True if must execute the balanced approach.
 */
void runMultiThread(bool balanced=false) {

    // Reset global counters
    kCounter = 0;
    kPrimeCounter = 0;

    // If the approach is balanced, open the output file
    if (balanced) {
        kOutputFile.open("primes.txt");
    }

    // Create the threads
    vector<std::thread*> threadList;
    for (int threadInx=0; threadInx < NUM_THREADS; threadInx++) {
        thread * thread;
        if (balanced) {
            // Add the balanced function
            thread = new std::thread(findPrimesThreadBalanced);
        } else {
            // Add the unbalanced function
            long int blockStart = threadInx * BLOCK_SIZE;
            long int blockEnd = blockStart + BLOCK_SIZE;
            thread = new std::thread(findPrimesThreadUnbalanced, blockStart, blockEnd);
        }

        // Add to the thread list
        threadList.push_back(thread);
    }

    // Wait for all threads to finish
    for (std::thread * thread : threadList) {
        // Wait for the thread to finish
        thread->join();

        // Remove the thread
        delete thread;
    }

    // If the approach is balanced, close the output file
    if (balanced) {
        kOutputFile.close();
    }
}


/**
 * Entry point
 */
int main() {

    // Execute several times to ger an average time
    long long elapsed = 0;
    for (int i=0; i<NUM_RUNS; i++) {

        // Measure elapsed time
        auto startTime = chrono::steady_clock::now();

        // Create the threads
        vector<std::thread*> threadList;
        threadList.reserve(NUM_THREADS);
        runMultiThread(IS_BALANCED);

        // Compute elapsed time
        auto endTime = chrono::steady_clock::now();
        elapsed += chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
    }
    double averageTime = double(elapsed) / double(NUM_RUNS);

    // Report
    printf("\n");
    printf("Number of threads: %d\n", NUM_THREADS);
    printf("Total time: %f milliseconds\n", averageTime);
    printf("Total numbers: %ld\n", MAX_PRIME_TARGET);
    printf("Total primes: %d\n", kPrimeCounter);

    return EXIT_SUCCESS;
}
