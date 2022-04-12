#include <vector>
#include <iostream>

using namespace std;

struct ant{

    int h_position;
    int w_position;
    vector<int> home_position;
    int field_of_vision;
    int status;  // 0: walking around  1:going home  2:following the pheromone trail 3: getting food

    ant(int height,int weight,vector<int> home, int field){
        h_position = height;
        w_position = weight;
        home_position = home;
        field_of_vision = field;
        status = 0;
    }


    void see_around(){
        
    }

    void move(){
        
    }

    void get_food(){
        
    }

    void go_home(){
        
    }

    void drop_pheromone(){
        
    }

};
