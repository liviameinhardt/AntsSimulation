#include <vector>
#include <iostream>
#include <random>
#include <cstdlib>
#include <string>
#include <mutex>
#include <thread>
using namespace std;


mutex AntsCounterMutex;
int AntsCounter = 0;


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
    int pheromone_life = 0;
    bool has_food = false;
    bool has_anthill = false;
    int w_pheromone_food_direction;
    int h_pheromone_food_direction;
    // ARRUMAR AQUI DEPOIS

    // Visualization of the unit
    string visualization = " ";

    void set_ant() { ant_number++; }
    void remove_ant() { ant_number--; }
    void set_pheromone(int quantity) { pheromone_life=quantity ; }
    void decay_pheromone() { 
        if (pheromone_life > 0){   
            pheromone_life--;
        }
        if(pheromone_life == 0 ){
            h_pheromone_food_direction= 0 ;
            w_pheromone_food_direction= 0;
        }
        }
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
        else if (ant_number > 0)
        {
            visualization = "\033[1;31m*\033[0m";
        }  
        else if (pheromone_life > 0)
        {
            visualization = "\033[1;35m•\033[0m";
            // visualization = ".";
        }
         // visualization = "*";
        
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

    void set_ant_map(int i, int j) { map[i][j].set_ant(); }                   // Set an ant at (i,j) position in the space
    void remove_ant_map(int i, int j) { map[i][j].remove_ant(); }             // Remove an ant at (i,j) position
    void set_pheromone_map(int i, int j, int quantity) { map[i][j].set_pheromone(quantity); }       // Set pheromone to terminal visualizationat position (i,j)
    void decay_pheromone_map(int i, int j) { map[i][j].decay_pheromone(); } // Remove a pheromone at (i,j) position
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
                decay_pheromone_map(i,j);
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
    vector<int> pheromone_position;
    // Indicate fi already saw food
    bool saw_food = false;
    bool going_home = false;
    bool dropping = false;
    bool following_pheoromone = false;
    int phe_qnt;


    ant(vector<int> home, int field, int pheromone_quantity ,space *map)
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

        phe_qnt = pheromone_quantity;
    }

    void see_around()
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
        following_pheoromone=false;
        saw_food = false;
        for (int i = h_start; i <= h_final; i++)
        {
            for (int j = w_start; j <= w_final; j++)
            {
                // cout << "("<<j<<","<<i<<")";
                if((*current_map).map[i][j].has_food){
                    food_position.push_back(i);
                    food_position.push_back(j);
                    saw_food = true;
                }

                if((*current_map).map[i][j].pheromone_life>0){
                    pheromone_position.push_back(i);
                    pheromone_position.push_back(j);
                    following_pheoromone = true;
                }
                // else{
                    // Se nao houver nenhum feromonio ao redor para o go_pheoromone
                    // following_pheoromone = (following_pheoromone||false);
                // }

            }
            // cout << endl;
        }
        // Se nao tiver mais comida naquele lugar
        if(food_position.size()>0){
        if(!(*current_map).map[food_position[0]][food_position[1]].has_food ){
            saw_food = false;
            food_position.clear();
        }}
        // return(false);
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
        (*current_map).set_ant_map(h_position, w_position);
    }

    void go_food(){
        (*current_map).remove_ant_map(h_position,w_position);
            h_direction = food_position[0]-h_position;
            w_direction = food_position[1]-w_position;
            if (h_direction>0)
            {
                h_position += 1;
            }
            else if(h_direction<0){
                h_position -=1;
            }
            else{
                h_position=h_position;
            }
            if(w_direction >0){
                w_position +=1 ;
            }
            else if(w_position<0){
                w_position -=1;
            }
            else{
                w_position = w_position;
            }
            if(w_position==food_position[1] && h_position==food_position[0]){
                saw_food = false;
                dropping = true;
                food_position.clear();
            }
            (*current_map).set_ant_map(h_position, w_position);
    }

    void go_home()
    {
        int w_home_direction = home_position[1]-w_position; 
        int h_home_direction = home_position[0]-h_position;
        // Removing ant from current position
        (*current_map).remove_ant_map(h_position,w_position);
        // Updating position
        if(w_home_direction<0){
            w_position-=1;
            (*current_map).map[h_position][w_position].w_pheromone_food_direction = 1;
        }
        else if(w_home_direction > 0){
            w_position+=1;
            (*current_map).map[h_position][w_position].h_pheromone_food_direction = -1;
        }
        
        if(h_home_direction < 0){
            h_position-=1;
            (*current_map).map[h_position][w_position].h_pheromone_food_direction = 1;
        }
        else if(h_home_direction > 0){
            h_position+=1;
            (*current_map).map[h_position][w_position].h_pheromone_food_direction = -1;
        }
        // Armazenando a direçao ate a comida
        if(w_home_direction<0){(*current_map).map[h_position][w_position].w_pheromone_food_direction = 1;}
        else if(w_home_direction > 0){(*current_map).map[h_position][w_position].h_pheromone_food_direction = -1;}
        else{(*current_map).map[h_position][w_position].h_pheromone_food_direction = 0;}

        if(h_home_direction < 0){(*current_map).map[h_position][w_position].h_pheromone_food_direction = 1;}
        else if(h_home_direction > 0){(*current_map).map[h_position][w_position].h_pheromone_food_direction = -1;}
        else{(*current_map).map[h_position][w_position].h_pheromone_food_direction = 0;}
        if(h_home_direction==0 && w_home_direction==0){
            dropping = false;
            saw_food = false;
        }
        (*current_map).set_ant_map(h_position, w_position);
    }

    void go_pheromone(){
        // cout << "Go to pheromone";
        int w_phero_direction = pheromone_position[1]-w_position;
        int h_phero_direction = pheromone_position[0]-h_position;
        (*current_map).remove_ant_map(h_position, w_position);

        if( (*current_map).map[h_position][w_position].pheromone_life>0){
                w_position += (*current_map).map[h_position][w_position].w_pheromone_food_direction;
                h_position += (*current_map).map[h_position][w_position].h_pheromone_food_direction;
                if((*current_map).map[h_position][w_position].pheromone_life==0){
                    following_pheoromone = false;
                }
        }
        else{
            if(w_phero_direction<0){
                w_position -= 1;
            }
            else if(w_phero_direction>0){
                w_position += 1;
            }
            if(h_phero_direction<0){
                h_position -= 1;
            }
            else if(h_phero_direction>0){
                h_position += 1;
            }
        } 
        // else {
        //     following_pheoromone = false;
        // }
        //     cout << "cheguei no feromonio" << endl;
        
        // }
        (*current_map).set_ant_map(h_position, w_position);
    }

    void move()
    {
        see_around();
        // cout << following_pheoromone << endl;
        // (*current_map).remove_ant_map(h_position, w_position);
        if(saw_food && !dropping){
            go_food();
        }
        else if(dropping){
            go_home();
        }
        else if(following_pheoromone){
            go_pheromone();
        }
        else{
            walk_randomly();
        }
         if (dropping){
            drop_pheromone();
        } 
        
    }

    void drop_pheromone()
    {
        (*current_map).set_pheromone_map(h_position,w_position, phe_qnt);
    }
};

void test(){
    cout << "here" << endl;
}


struct anthill
{
    int h_position;
    int w_position;
    vector<ant> ants_list;
    space *current_map;
    anthill(int i, int j,int number_of_ants, int field_vision, int pheromone_quantity,space *map)
    {
        h_position = i;
        w_position = j;
        current_map = map;
        (*map).set_anthill_map(i,j);
        spawn_ants(number_of_ants, field_vision,pheromone_quantity);
    }
    
    void spawn_ants(int number_of_ants, int ant_field_vision, int pheromone_quantity)
    {
        for(int i = 0; i < number_of_ants; i++){
          ant new_ant({h_position,w_position}, ant_field_vision,pheromone_quantity, current_map);
          ants_list.push_back(new_ant);
        }
    }

    long getNextAnt(){
        // const lock_guard<std::mutex> lock(AntsCounterMutex);
        //  cout << "HERE" << endl;
        return AntsCounter++;
    }

    void ant_moves(){
        
        int number;
        do{
            number = getNextAnt();
            cout << number << endl;
            ants_list[number].move();
        }
        while (number <= ants_list.size());

    }

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
