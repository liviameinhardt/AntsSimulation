
#include "include/structures.cpp"
#include <chrono>
#include <thread>


int main(int argc, char const *argv[])
{

    // ****************** PROGRAM ******************
    // int num_threads = 1;
    int map_h = 10;
    int map_w = 30;
    vector<anthill> anthills; // Vector to store the anthills
    // vector<pheromone> pheromones; // Vector to store the pheromone
    vector<food> foods; // Vector to store food coordinates
    space map(map_h, map_w);
    
    
    anthill sauvas(2,2,0,2,5, &map);
    
    // map.show_map();
    food bolo(7,20, 2, &map);
    // food chocolate(2,2, 1, &map);
    // map.show_map();

    // ant first_ant({9,19},1, &map);
    // first_ant.see_around();
    while (true)
    {
        sauvas.ant_moves();
        map.show_map();
        bolo.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    
    return 0;
}


// #################### PARTE LIVIA ###################################
// #include "include/structures.cpp"
// #include <chrono>
// #include <thread>

 
// int main(int argc, char const *argv[])
// {
//     // ****************** INICIALIZATION ******************

//     // program entry
//     int num_threads = 1; //Número de threads a ser utilizada
//     int map_h = 15; //Dimensão do mapa altura
//     int map_w = 15;//Dimensão do mapa largura
//     int simulation_time = 120; //Tempo da simulação (em segundos)
//     int pheromone_timelife = 30; //Tempo de vida de um ferominio (em segundos)
//     int ant_field_of_vision = 1; //Campo de visão da formiga
//     int max_ants_food = 5; //Número máximo de fomigas que podem coletar comida concorrentemente
 
//     vector<int> ants_info ={5,ant_field_of_vision}; //only one anthill for now
//     vector<int> anthill_info = {0, 0}; //Posição do formigueiro (coluna_indice, linha_indice) e quantidade de formigas 
//     vector<vector<int>> food_info = {{12, 45,100,30}}; // Posição da fonte de comida; posição; quantidade inicial; taxa de reposição 
    
//     // create structures
//     space map(map_h, map_w);
//     anthill sauvas(2,2,0,2,pheromone_timelife, &map);
//     food bolo(4,3, 1, &map);

//     // ****************** RUN MULTITHREAD PROGRAM ******************
//     while (true)
//     {
//         sauvas.ant_moves();
//         map.show_map();
//         bolo.update();
//         std::this_thread::sleep_for(std::chrono::milliseconds(100));
//     }

//     return 0;
// }