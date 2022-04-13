// #include "include/map.cpp"
// #include "include/ants.cpp"
// #include "include/anthill.cpp"
// #include "include/food.cpp"
// #include "include/pheromone.cpp"
#include "include/structures.cpp"
#include <chrono>
#include <thread>
struct pointer_test
{
    pointer_test(space *map){
        // (*map).set_food_map(1,2);
        (*map).show_map();
    }
};


int main(int argc, char const *argv[])
{

    // ****************** PROGRAM ******************
    // int num_threads = 1;
    int map_h = 15;
    int map_w = 50;
    vector<anthill> anthills; // Vector to store the anthills
    vector<pheromone> pheromones; // Vector to store the pheromone
    vector<food> foods; // Vector to store food coordinates
    space map(map_h, map_w);
    
    
    anthill sauvas(2,2,0,10, &map);
    // map.show_map();
    food bolo(10,20, 1, &map);

    map.show_map();

    // ant first_ant({0,0},1, &map);
    while (true)
    {
        // first_ant.move();
        sauvas.ant_moves();
        map.show_map();
        bolo.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    

  


    // int pheromone_timelife = 30;
    // int ant_field_of_vision = 1;
    // vector<vector<int>> ants_info ={{5,ant_field_of_vision}};
    // vector<vector<int>> anthill_info = {{0, 0}};
    // vector<vector<int>> food_info = {{12, 45,100,30}}; // position, quantity, taxa_rep
    // int max_ants_food = 5;


    return 0;
}

