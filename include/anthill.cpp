#include "ants.cpp"
#include <iostream>

using namespace std;

struct anthill
{
  vector<int> position;
  vector<ant> ants_list;
  int id;

  void spawn_ants(int number_of_ants, int ant_field_vision){

    for(int i = 0; i <= number_of_ants; i++){

      ant new_ant;

      // qual será a posicao inicial da formiga?
      new_ant.position.push_back(0); 
      new_ant.position.push_back(0); 

      new_ant.home_position = position; 
      new_ant.field_of_vision = ant_field_vision; 
      new_ant.status = 0; // walking around

      ants_list.push_back(new_ant);

    }

      
  }

  void next_ant(){

    
  }


};

int main(){

  anthill formigueiro1;
  formigueiro1.position.push_back(0);
  formigueiro1.position.push_back(1);

  formigueiro1.spawn_ants(5,2); 

   for (ant x : formigueiro1.ants_list){
        cout << x.status << " ";
   }
       

  
  return 1;
}