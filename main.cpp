
#include "include/structures.cpp"
#include <chrono>
#include <thread>


int main(int argc, char const *argv[])
{

    // ****************** PROGRAM ******************

    int NUM_THREADS = 1; //Número de threads a ser utilizada
    int map_h = 15; //Dimensão do mapa altura
    int map_w = 30;//Dimensão do mapa largura
    int simulation_time = 1200; //Tempo da simulação
    int pheromone_timelife = 30; //Tempo de vida de um ferominio (em segundos)
    int ant_field_of_vision = 1; //Campo de visão da formiga
    int max_ants_food = 5; //Número máximo de fomigas que podem coletar comida concorrentemente
    int number_of_ants = 30;
    int food_quantity = 100; 

    vector<int> ants_info ={5,ant_field_of_vision}; //only one anthill for now
    vector<int> anthill_position = {0, 0}; //Posição do formigueiro (coluna_indice, linha_indice) e quantidade de formigas 
    vector<int> food_position = {12, 20};
    
    space map(map_h, map_w);
    anthill sauvas(anthill_position[0],anthill_position[1],number_of_ants,ant_field_of_vision,pheromone_timelife, &map);
    food bolo(food_position[0],food_position[1],food_quantity, &map);

    // ****************** MULTI THREAD ******************

    long long elapsed = 0;
    auto startTime = chrono::steady_clock::now();

    do{

        AntsCounter = 0; // reset counter

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

        bolo.update();
        map.show_map();
       
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    
        auto endTime = chrono::steady_clock::now();
        elapsed += chrono::duration_cast<chrono::seconds>(endTime - startTime).count();

    }while (elapsed <= simulation_time);

    return 0;
}
