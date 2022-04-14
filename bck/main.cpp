
#include "include/structures.cpp"
#include <chrono>



int main(int argc, char const *argv[])
{

    // ****************** INICIALIZATION ******************

    // program entry
    int NUM_THREADS = 1; //Número de threads a ser utilizada
    int map_h = 15; //Dimensão do mapa altura
    int map_w = 15;//Dimensão do mapa largura
    int simulation_time = 1200; //Tempo da simulação
    int pheromone_timelife = 30; //Tempo de vida de um ferominio (em segundos)
    int ant_field_of_vision = 1; //Campo de visão da formiga
    int max_ants_food = 5; //Número máximo de fomigas que podem coletar comida concorrentemente
 
    vector<int> ants_info ={5,ant_field_of_vision}; //only one anthill for now
    vector<int> anthill_info = {0, 0}; //Posição do formigueiro (coluna_indice, linha_indice) e quantidade de formigas 
    vector<vector<int>> food_info = {{12, 45,100,30}}; // Posição da fonte de comida; posição; quantidade inicial; taxa de reposição 
    
    // create structures
    space map(map_h, map_w);
    anthill sauvas(2,2,0,10,pheromone_timelife, &map);
    food bolo(10,3, 1, &map);

    // ****************** MULTI THREAD ******************

    long long elapsed = 0;
    auto startTime = chrono::steady_clock::now();

    do
    {
        sauvas.RunMultithread();
        bolo.update();
        map.show_map(); 

        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        auto endTime = chrono::steady_clock::now();
        elapsed += chrono::duration_cast<chrono::seconds>(endTime - startTime).count();

    }while (elapsed <= simulation_time);
    
    return 0;
}

