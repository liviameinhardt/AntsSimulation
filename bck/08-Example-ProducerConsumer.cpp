#include <chrono>
#include <cmath>
#include <iostream>
#include <queue>
#include <random>
#include <thread>
#include <vector>


using namespace std;


/**
 * Semaphore class
 */
class Semaphore {

    public:
        /**
         * Constructor
         *
         * @param counter The starting value
         */
        explicit Semaphore(const string & name, int counter = 0) {
            this->_name = name;
            this->_counter = counter;
        }
        virtual ~Semaphore() = default;

        /**
         * Attempt to enter the critical region. Wait if it is closed.
         */
        void down() {
            // Wrap the mutex and kLockMutex
            unique_lock<mutex> lock(_mutex);

            // Wait for the kLockMutex to be released (coping with spurious wake-ups)
            while (!_counter) {
                printf("Semaphore blocked: %s\n", this->_name.c_str());
                cout << std::flush;
                _condition.wait(lock);
            }

            // Update the counter
            _counter--;

            // The mutex is automatically unlocked
        }

        /**
         * Attempt to enter the critical region. Skip if it is closed.
         *
         * @return True if entered in the region
         */
        bool tryDown() {
            // Wrap the mutex and kLockMutex
            std::lock_guard<mutex> lock(_mutex);

            // Check if the thread can enter the critical region
            bool can_enter = _counter > 0;
            if (can_enter) {

                // Update the counter
                _counter--;
            }

            return can_enter;

            // The mutex is automatically unlocked
        }

        /**
         * Notify it is exiting the critical region.
         */
        void up() {
            // Wrap the mutex and kLockMutex
            std::lock_guard<mutex> lock(_mutex);

            // Update the counter
            _counter++;

            // Notify the condition was changed
            _condition.notify_one();

            // The mutex is automatically unlocked
        }

    private:
        mutex _mutex;
        condition_variable _condition;
        int _counter = 0;
        string _name;
};


/**
 * Buffer size
 */
int MAX_ITEMS = 10;

/**
 * The buffer
 */
queue<long> kBuffer;

/**
 * Mutex for critical sections
 */
mutex kLockMutex;

/**
 * Semaphore to count empty slots in the buffer
 */
Semaphore kEmptySlotsSemaphore("EMPTY_SLOTS", MAX_ITEMS); // NOLINT(cert-err58-cpp)

/**
 * Semaphore to count kFullSlotsSemaphore slots in the buffer
 */
Semaphore kFullSlotsSemaphore("FULL_SLOTS"); // NOLINT(cert-err58-cpp)

/**
 * The list of threads
 */
vector<thread*> threadList;


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
 * Generates a random number inside the given limits
 */
long generateRandomNumber(long min_value, long max_value) {
    random_device random_device;
    mt19937 random_generator(random_device());
    uniform_int_distribution<std::mt19937::result_type> distribution(min_value, max_value);
    return distribution(random_generator);
}


/**
 * Generates an item.
 *
 * Actually, it only consumes time and returns a number. The time elapsed is variable
 * in order to attempt to simulate a real-world scenario.
 *
 * @return The item
 */
long produceItem() {
    // Generate a random number
    long min_value = static_cast<long>(pow(2, 10));
    long max_value = static_cast<long>(pow(2, 24));
    long number = generateRandomNumber(min_value, max_value);

    // Force the thread to waste time computing
    isPrime(number);

    printf("Generating item %ld\n", number);
    cout << std::flush;

    return number;
}

/**
 * Consumes an item
 *
 * Actually, it only consumes time based on the input number. Again, attempting to
 * simulate a real-world scenario.
 *
 * @param item The item to be processed
 */
void consumeItem(long item) {
    // Force the thread to waste time computing
    isPrime(item);

    printf("Consuming item %ld\n", item);
    cout << std::flush;
}


/**
 * Execute a producer generating a given number of items
 *
 * @param num_times
 */
void executeProducer(int num_items) {

    while (num_items--) {

        // Generate an item
        long item = produceItem();

        // Enter the critical region
        kEmptySlotsSemaphore.down();
        kLockMutex.lock();

        // Actually insert the item
        kBuffer.push(item);
        printf("Buffer size: %lu\n", kBuffer.size());
        cout << std::flush;

        // Leave the critical region
        kLockMutex.unlock();
        kFullSlotsSemaphore.up();
    }
}

/**
 * Executes a consumer processing a given number of items
 * @param count
 */
void executeConsumer(int num_items) {

    while (num_items--) {

        // Enter the critical region
        kFullSlotsSemaphore.down();
        kLockMutex.lock();

        // Remove the item from the buffer
        long item = kBuffer.front();
        kBuffer.pop();
        printf("Buffer size: %lu\n", kBuffer.size());
        cout << std::flush;

        // Leave the critical region
        kLockMutex.unlock();
        kEmptySlotsSemaphore.up();

        // Consume the item
        consumeItem(item);

        // Do something with the item
    }
}

/**
 * Main.
 */
int main() {

    // Single thread
    {
//        executeProducer(10);
//        executeConsumer(10);
    }

    // Multi thread
    {
        // Create producers and consumers
        threadList.push_back(new thread(executeProducer, 30));
        this_thread::sleep_for(chrono::milliseconds(50));

        threadList.push_back(new thread(executeConsumer, 45));
        this_thread::sleep_for(chrono::milliseconds(10));

        threadList.push_back(new thread(executeConsumer, 15));
        this_thread::sleep_for(chrono::milliseconds(10));

        threadList.push_back(new thread(executeProducer, 15));
        this_thread::sleep_for(chrono::milliseconds(10));

        threadList.push_back(new thread(executeConsumer, 30));
        this_thread::sleep_for(chrono::milliseconds(10));

        threadList.push_back(new thread(executeProducer, 45));
        this_thread::sleep_for(chrono::milliseconds(10));

        // Wait for all threads to finish
        for (thread * thread : threadList) {
            thread->join();
            delete thread;
        }
    }

    return EXIT_SUCCESS;
}
