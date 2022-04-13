#include <vector>
#include <iostream>
#include <random>
#include <cstdlib>
#include <string>
#include <iostream>
using namespace std;

void gotoxy(int x, int y) { printf("%c[%d;%df", 0x1B, y, x); }
void clrscr(void) { system("clear"); }
int rand_between(int start, int final)
{
    
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(start, final); // distribution in range [1, 6]

    return( int(dist6(rng)));
};

// Basic structure of the space
struct space_unit
{

    int ant_number = 0;
    bool has_pheromone = false;
    bool has_food = false;
    bool has_anthill = false;
    // ARRUMAR AQUI DEPOIS

    // Visualization of the unit
    string visualization = " ";

    void set_ant() { ant_number++; }
    void remove_ant() { ant_number--; }
    void set_pheromone() { has_pheromone = true; }
    void remove_pheromone() { has_pheromone = false; }
    void set_food() { has_food = true; }
    void remove_food() { has_food = false; }
    void set_anthill() { has_anthill = true; }
    void remove_anthill() { has_anthill = false; }

    void generate_visualization()
    {

        if (has_anthill)
        {
            visualization = "\033[1;41m \033[0m";
            // visualization = "A";
        }
        else if (has_food)
        {
            visualization = "\033[1;43m \033[0m";
            // visualization = "O";
        }
        else if ((ant_number == 0) & (has_pheromone))
        {
            visualization = "\033[1;35m•\033[0m";
            // visualization = ".";
        }
        else if ((ant_number > 0))
        {
            visualization = "\033[1;31m*\033[0m";
            // visualization = "*";
        }
        else
        {
            visualization = " ";
        }
    }

    void show()
    {
        this->generate_visualization();
        cout << visualization;
    }
};

struct space
{
    // Space dimension
    int width;
    int height;

    vector<vector<space_unit>> map; // Matrix with space_unit

    // ******************* CONSTRUCTOR **************************
    space(int h_dimension, int w_dimension)
    {
        width = w_dimension;
        height = h_dimension;

        for (int i = 0; i < h_dimension; i++)
        {
            map.push_back(vector<space_unit>());
            for (int j = 0; j < w_dimension; j++)
            {
                map[i].push_back(*new space_unit());
            }
        }
    };

    // ************************************************ INICIALIZATION FUNCTIONS ************************************************
    // void add_anthill(vector<vector<int>> anthill_info, vector<vector<int>>ants_info){

    //     for (int i = 0; i < anthill_info.size(); i++)
    //     {
    //         anthills.push_back(*new anthill(anthill_info[i][0],anthill_info[i][1],i));

    //         int h_position = anthills[i].h_position;
    //         int w_position = anthills[i].w_position;

    //         // Adding anthill pointer
    //         map[h_position][w_position].anthill_here = &anthills[i];
    //         this->set_anthill_map(h_position,w_position);

    //         anthills[i].spawn_ants(ants_info[i][0],ants_info[i][1]); // create ants
    //     }

    // }

    // void add_food(vector<vector<int>> food_info){

    //     for (int i = 0; i < food_info.size(); i++){
    //     foods.push_back(*new food(food_info[i][0],food_info[i][1],food_info[i][2]));

    //     int w_position = foods[i].w_position;
    //     int h_position = foods[i].h_position;

    //     // Adding food pointer
    //     map[h_position][w_position].food_here = &foods[i];
    //     set_food_map(h_position,w_position);

    //     }
    // }

    // //***************************************** SIMULATION FUNCTIONS ************************************************
    // void add_pheromone(int h_position, int w_position, int life){
    //     pheromones.push_back(*new pheromone(h_position, w_position,life));
    //     map[h_position][w_position].pheromone_here = &pheromones[pheromones.size()-1];  // Adding pheromone pointer
    //     set_pheromone_map(h_position,w_position);
    // }

    //***************************************** CLASS FUNCTIONS ************************************************
    void set_ant_map(int i, int j) { map[i][j].set_ant(); }                   // Set an ant at (i,j) position in the space
    void remove_ant_map(int i, int j) { map[i][j].remove_ant(); }             // Remove an ant at (i,j) position
    void set_pheromone_map(int i, int j) { map[i][j].set_pheromone(); }       // Set pheromone to terminal visualizationat position (i,j)
    void remove_pheromone_map(int i, int j) { map[i][j].remove_pheromone(); } // Remove a pheromone at (i,j) position
    void set_food_map(int i, int j) { map[i][j].set_food(); }                 // Set food to terminal visualization at position (i,j)
    void remove_food_map(int i, int j) { map[i][j].remove_food(); }
    void set_anthill_map(int i, int j) { map[i][j].set_anthill(); }       // Set anthill to terminal visualization at position (i,j)
    void remove_anthill_map(int i, int j) { map[i][j].remove_anthill(); } // Remove an anthill at (i,j) position

    // //***************************************** Update the terminal //*****************************************
    void show_map()
    {

        // Lines to clear the teminal
        int x = 0, y = 10;
        clrscr();
        gotoxy(x, y);

        // Upper wall
        for (int i = 0; i < width + 2; i++)
        {
            cout << "#";
        }

        cout << "\n";

        // Left and right wall
        for (int i = 0; i < height; i++)
        {

            cout << "#";

            for (int j = 0; j < width; j++)
            {
                map[i][j].show();
            }

            cout << "#";
            cout << "\n";
        }

        // Down wall
        for (int i = 0; i < width + 2; i++)
        {
            cout << "#";
        }
        cout << "\n";
    }
};

struct ant
{

    int h_position;
    int w_position;
    vector<int> home_position;
    int field_of_vision;
    int status; // 0: walking around  1:going home  2:following the pheromone trail 3: getting food
    space *current_map;
    int w_direction;
    int h_direction;
    vector<int> food_position;
    // Indicate fi already saw food
    bool saw_food = false;
    bool going_home = false;
    bool dropping = false;


    ant(vector<int> home, int field, space *map)
    {
        h_position = home[0];
        w_position = home[1];
        home_position = home;
        field_of_vision = field;
        status = 0;
        
        (*map).set_ant_map(h_position, w_position);
        current_map = map;

        // Define the move direction
        w_direction = rand_between(1,3)-2;
        h_direction = rand_between(1,3)-2;
    }

    bool see_around()
    {
        // Square area vision
        int w_start = w_position-field_of_vision;
        int h_start = h_position-field_of_vision;
        // Rever caso as formigas não vejam as extremidades
        if(w_start<=0){w_start = 0;};
        if(h_start<=0){h_start = 0;};
        int w_final = w_position+field_of_vision;
        int h_final = h_position+field_of_vision;
        // Rever caso as formigas não vejam as extremidades
        if(w_final>=(*current_map).width){w_final = (*current_map).width-1;}
        if(h_final>=(*current_map).height){h_final = (*current_map).height-1;}
        // Food sense
        // cout << "upper corner:" << w_start << "," << h_start << endl;
        // cout << "upper corner:" << w_final << "," << h_final << endl;
        for (int i = h_start; i <= h_final; i++)
        {
            for (int j = w_start; j <= w_final; j++)
            {
                // cout << "("<<j<<","<<i<<")";
                if((*current_map).map[i][j].has_food){
                    food_position.push_back(i);
                    food_position.push_back(j);
                    return (true);
                }
            }
            // cout << endl;
        }
        return(false);
        // (*current_map)
    }

    void walk_randomly(){
        // Tem 1/5 de chance da formiga rever a direção que está seguindo
        if(rand_between(1,5)<2){
            w_direction = rand_between(1,3)-2;
        }
        if(rand_between(1,5)<2){
            h_direction = rand_between(1,3)-2;
        }
        // Removing the old position
        (*current_map).remove_ant_map(h_position, w_position);

        // if touch the wall change the direction
        if((w_position+w_direction)>=(*current_map).width || (w_position+w_direction)<0){
            w_direction = -w_direction;    
        }
        
        if((h_position+h_direction)>=(*current_map).height || (h_position+h_direction)<0){
            h_direction = -h_direction;    
        }
        
        w_position += w_direction;
        h_position += h_direction;
    }

    int move()
    {
        // if(!saw_food){
        //     saw_food = see_around();
        //     // Ainda falta definir a movimentação até a comida
        // }
        // cout << (*current_map).map[h_position][w_position].has_food;

        walk_randomly();

        if((*current_map).map[h_position][w_position].has_food ){
            cout << "FOOOOD";
            dropping = true;
        }
        
        (*current_map).set_ant_map(h_position, w_position);

         if (dropping){
            (*current_map).set_pheromone_map(h_position,w_position);
        }

        return 0;
    }

    void get_food(){
        dropping = true;
    }

    void go_home()
    {
        // incompleto
        w_direction = w_position-home_position[1];
        
        h_direction = h_position-home_position[0];

        (*current_map).remove_ant_map(h_position,w_position);
        if(w_direction > 0){
            w_position -= w_direction;
        }
        else{
            w_position += w_direction;
        }
        if(h_direction > 0){
            h_position -= h_direction;
        }
        if(h_direction < 0){
            h_position += h_direction;
        }
        

        (*current_map).set_pheromone_map(h_position,w_position);

        if (w_position==home_position[1] && h_position==home_position[0]){
            going_home = false;
        }
    }

    void drop_pheromone()
    {
    }
};

struct anthill
{
    int h_position;
    int w_position;
    vector<ant> ants_list;
    int id;
    space *current_map;
    // constructor
    anthill(int i, int j, int int_id,int number_of_ants, space *map)
    {
        h_position = i;
        w_position = j;
        id = int_id;
        current_map = map;
        (*map).set_anthill_map(i,j);
        spawn_ants(number_of_ants,1);
    }
    
    void spawn_ants(int number_of_ants, int ant_field_vision)
    {
        for(int i = 0; i < number_of_ants; i++){
          ant new_ant({h_position,w_position}, 1, current_map);
          ants_list.push_back(new_ant);
        }
    }
    void ant_moves(){
        // Aqui entra movimento das sauvas
        for (int i = 0; i < ants_list.size(); i++)
        {
            ants_list[i].move();
        }
        
    }

    // void next_ant(){
    // vai dizer qual a proxima formiga a ser processada
    //   }
};

struct food
{
    int h_position;
    int w_position;
    int max_quantity;
    int current_quantity;
    space *current_map;
    // constructor
    food(int i, int j, int int_quantity, space *map)
    {
        h_position = i;
        w_position = j;
        current_quantity = int_quantity;
        max_quantity = int_quantity;
        current_map = map;
        (*current_map).set_food_map(i,j);
    }
    void update(){
        // Por enquanto, para cada formiga na posição da comida, há um decréscimo da comida
        for (int i = 0; i < (*current_map).map[h_position][w_position].ant_number; i++)
        {
            decay_quantity();
            // Se a comida acabar
            if(current_quantity<=0){
                
                // A comida é removida da localização atual
                (*current_map).remove_food_map(h_position,w_position);
                // Um nova posição aleatoria é escolhida
                h_position = rand_between(1,(*current_map).height-1);
                w_position = rand_between(1,(*current_map).width-1);
                (*current_map).set_food_map(h_position,w_position);
                // A quantidade é atualizada
                reset_quantity();
                break;
            }
        }
        
    }

    
    void decay_quantity()
    {
        current_quantity = current_quantity - 1;
    }

    void reset_quantity()
    {
        current_quantity = max_quantity;
    }
};
struct pheromone
{
    int h_position;
    int w_position;
    int life_time;

    // constructor
    pheromone(int height, int weight, int life)
    {
        h_position = height;
        w_position = weight;
        life_time = life;
    }

    void decay_life()
    {
        life_time = life_time - 1;
    }

    void increase_life(int time)
    {
        life_time = life_time + time;
    }
};

