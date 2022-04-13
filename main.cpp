 #include "include/map.cpp"


int main(int argc, char const *argv[])
{

    // entrada do programa
    // int num_threads = 1;
    int map_h = 15;
    int map_w = 50;
    // int simulation_time = 0;

    int pheromone_timelife = 30;
    int ant_field_of_vision = 1;
    vector<vector<int>> anthill_info = {{0, 0}};
    vector<vector<int>> food_info = {{12, 45,100,30}}; // position, quantity, taxa_rep
    // int max_ants_food = 5;


    // inicializar o programa
    space map(map_h, map_w); 
    map.add_anthill(anthill_info);
    map.add_food(food_info);
    map.show_map();


    int w_postion = 0;
    int h_postion = 0;

    // map.set_food_map(10, 35);
    // map.set_anthill_map(1,1);

    for (int i = 0; i < 10; i++)
    {
        map.set_ant_map(h_postion,w_postion);
        map.set_ant_map(h_postion-1,w_postion-1);
        map.show_map();
        
        // Sleep in miliseconds
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        map.set_pheromone_map(h_postion,w_postion);
        map.remove_ant_map(h_postion,w_postion);

        // Generating random number
        int RandW = rand() % 2;
        int RandH = rand() % 2;
        
        w_postion += RandW;
        h_postion += RandH;

    }
    // return 0;
}

