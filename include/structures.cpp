#include "map.cpp"


// generate random number
int rand_between(int start, int final){
    
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(start, final); // distribution in range [1, 6]

    return( int(dist6(rng)));
};

// Mutex and Counter to run multi thread program 
mutex AntsCounterMutex; 
int AntsCounter = 0; 

// ************************************** ANTS, ANTHILL AND FOOD **************************************

struct ant
{

     // moving variables
    int h_position;
    int w_position;
    int w_direction;
    int h_direction;

    int field_of_vision;
    int phe_qnt;
    
    vector<int> home_position;
    vector<int> food_position;
    vector<int> pheromone_position;

    // status variables
    bool saw_food = false;
    bool going_home = false;
    bool dropping = false;
    bool following_pheoromone = false;

    space *current_map; // pointer to map


    ant(vector<int> home, int field, int pheromone_quantity ,space *map)
    {
        // inicial positions
        h_position = home[0];
        w_position = home[1];
        // Define the inicial move direction
        w_direction = rand_between(1,3)-2;
        h_direction = rand_between(1,3)-2;

        home_position = home;
        field_of_vision = field;
        phe_qnt = pheromone_quantity;
        
        current_map = map;
        (*current_map).set_ant_map(h_position, w_position);
       
    }

    void see_around(){
        
        // Square area vision
        int w_start = w_position-field_of_vision;
        int h_start = h_position-field_of_vision;

        // Check if ant is on the board egde
        if(w_start<=0){w_start = 0;};
        if(h_start<=0){h_start = 0;};
        int w_final = w_position+field_of_vision;
        int h_final = h_position+field_of_vision;
        if(w_final>=(*current_map).width){w_final = (*current_map).width-1;}
        if(h_final>=(*current_map).height){h_final = (*current_map).height-1;}


        following_pheoromone=false;
        saw_food = false;


        for (int i = h_start; i <= h_final; i++)
        {
            for (int j = w_start; j <= w_final; j++)
            {
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
            }
        }
        
        if(food_position.size()>0){
        if(!(*current_map).map[food_position[0]][food_position[1]].has_food ){
            saw_food = false;
            food_position.clear();
        }}
    }

    void walk_randomly(){

        // Change direction with probability 1/5
        if(rand_between(1,5)<2){ w_direction = rand_between(1,3)-2;}
        if(rand_between(1,5)<2){h_direction = rand_between(1,3)-2;}

        // Removing the old position
        (*current_map).remove_ant_map(h_position, w_position);

        // if touch the wall change the direction
        if((w_position+w_direction)>=(*current_map).width || (w_position+w_direction)<0){
            w_direction = -w_direction;    
        }
        if((h_position+h_direction)>=(*current_map).height || (h_position+h_direction)<0){
            h_direction = -h_direction;    
        }
        // New position
        w_position += w_direction;
        h_position += h_direction;
        (*current_map).set_ant_map(h_position, w_position);
        
    }


    void go_food(){

            // Removing the old position
            (*current_map).remove_ant_map(h_position,w_position);

            // follow the food
            h_direction = food_position[0]-h_position;
            w_direction = food_position[1]-w_position;

            if (h_direction>0){h_position += 1;}
            else if(h_direction<0){h_position -=1; }
            else{ h_position=h_position;}
            if(w_direction >0){w_position +=1 ;}
            else if(w_position<0){ w_position -=1;}
            else{w_position = w_position;}

            if(w_position==food_position[1] && h_position==food_position[0]){
                saw_food = false;
                dropping = true;
                food_position.clear();
            }

            // New position
            (*current_map).set_ant_map(h_position, w_position);
    }


    void go_home(){

        // follow home position
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

        // New position        
        (*current_map).set_ant_map(h_position, w_position);
    }

    void go_pheromone(){

        // Follow pheromone
        int w_phero_direction = pheromone_position[1]-w_position;
        int h_phero_direction = pheromone_position[0]-h_position;

        // Removing the old position
        (*current_map).remove_ant_map(h_position, w_position);

        if( (*current_map).map[h_position][w_position].pheromone_life>0){
                w_position += (*current_map).map[h_position][w_position].w_pheromone_food_direction;
                h_position += (*current_map).map[h_position][w_position].h_pheromone_food_direction;
                if((*current_map).map[h_position][w_position].pheromone_life==0){
                    following_pheoromone = false;
                }
        }
        else{
            if(w_phero_direction<0){ w_position -= 1;}
            else if(w_phero_direction>0){ w_position += 1;}
            if(h_phero_direction<0){h_position -= 1;}
            else if(h_phero_direction>0){h_position += 1;}
        } 
        
        // New position        
        (*current_map).set_ant_map(h_position, w_position);
    }

    void move(){

        see_around();

        if(saw_food && !dropping){ go_food(); }
        else if(dropping){ go_home();}
        else if(following_pheoromone){ go_pheromone();}
        else{walk_randomly();}
        if (dropping){drop_pheromone();} 
        
    }

    void drop_pheromone(){
        (*current_map).set_pheromone_map(h_position,w_position, phe_qnt);
    }


};



struct anthill
{
    int h_position;
    int w_position;
    vector<ant> ants_list;
    space *current_map;


    anthill(int i, int j,int number_of_ants, int field_vision, int pheromone_quantity,space *map){
        h_position = i;
        w_position = j;
        current_map = map;
        (*map).set_anthill_map(i,j);
        spawn_ants(number_of_ants, field_vision,pheromone_quantity);
    }
    
    void spawn_ants(int number_of_ants, int ant_field_vision, int pheromone_quantity){

        for(int i = 0; i < number_of_ants; i++){

          ant new_ant({h_position,w_position}, ant_field_vision,pheromone_quantity, current_map);
          ants_list.push_back(new_ant);

        }
    }

    int getNextAnt(){
        const lock_guard<std::mutex> lock(AntsCounterMutex);
        return AntsCounter++;
    }

    void ant_moves(){

        for (int i =0; i < ants_list.size(); i++){
            ants_list[AntsCounter].move();
            getNextAnt();
        }
    }

};

// VALE A PENA MANTER ASSIM? MNAO É MELHOR COLOCAR DENTRO DO MAPA?
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
        for (int i = 0; i < (*current_map).map[h_position][w_position].ant_number; i++)
        {
            decay_quantity();
            if(current_quantity<=0){
                reset_quantity();
                break;
            }
        }
        
    }

    void decay_quantity(){current_quantity = current_quantity - 1;}
    void reset_quantity(){current_quantity = max_quantity;}


};
