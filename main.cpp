
#include "include/scenarios.cpp"
#include "include/structures.cpp"
#include <chrono>
#include <thread>


int main(int argc, char const *argv[])
{

    // ***************** INICIALIZATE *****************

    space simulation(map_h, map_w);
    anthill sauvas(anthill_position[0],anthill_position[1],number_of_ants,ant_field_of_vision,pheromone_timelife, &simulation);

    food bolo(food_quantity,rep_rate);
    simulation.map[food_position[0]][food_position[1]].set_food(&bolo);


    // simulation time counter
    long long elapsed = 0;
    auto startTime = chrono::steady_clock::now();

    // ****************** MULTI THREAD *****************

 
    do{

        AntsCounter = 0; // reset ants global counter

        // process ants in threads
        vector<std::thread*> threadList;
        threadList.reserve(NUM_THREADS);
        for (int threadInx=0; threadInx < NUM_THREADS; threadInx++) {
                thread * thread;
                thread = new std::thread(&anthill::ant_moves,&sauvas);
                threadList.push_back(thread);
        }

        for (std::thread * thread : threadList) {
            thread->join();
            delete thread;
        }

        // update map
        simulation.show_map();

        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        auto endTime = chrono::steady_clock::now();
        elapsed += chrono::duration_cast<chrono::seconds>(endTime - startTime).count();

    }while (elapsed <= simulation_time);

    return 0;
}
