#include "ants.cpp"
#include <iostream>

using namespace std;

struct anthill
{
  int h_position;
  int w_position;
  vector<ant> ants_list;
  int id;

  // constructor
  anthill(int height,int weight, int int_id){
    h_position = height;
    w_position = weight;
    id = int_id;
  }

  void spawn_ants(int number_of_ants, int ant_field_vision){

    for(int i = 0; i <= number_of_ants; i++){
      ant new_ant(0,0,{h_position,w_position}, 5);
      ants_list.push_back(new_ant);
    }
      
  }

  void next_ant(){

    
  }


};
