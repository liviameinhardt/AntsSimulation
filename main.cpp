
#include "include/structures.cpp"
#include <chrono>
#include <thread>


int main(int argc, char const *argv[])
{

    // ****************** PROGRAM ******************
    // int num_threads = 1;
    int map_h = 20;
    int map_w = 40;
    vector<anthill> anthills; // Vector to store the anthills
    // vector<pheromone> pheromones; // Vector to store the pheromone
    vector<food> foods; // Vector to store food coordinates
    space map(map_h, map_w);
    
    
    anthill sauvas(2,2,0,2,10, &map);
    
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
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    
    return 0;
}


