#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <chrono>
#include <thread>

// include required modules 
#include "include/ants.cpp"
#include "include/anthill.cpp"
#include "include/food.cpp"
#include "include/pheromone.cpp"

using namespace std;


// Code to always update terminal
void gotoxy(int x,int y)    
{
    printf("%c[%d;%df",0x1B,y,x);
}
void clrscr(void)
{
    system("clear");
}



// Basic structure of the space
struct space_unit
{
    space_unit(){

    }

    int ant_number=0;
    bool has_pheromone = false;
    bool has_food = false;
    bool has_anthill = false;
    // Pointer to current pheromone
    pheromone* pheromone_here;
    food* food_here;
    anthill* anthill_here;
    
    // Visualization of the unit
    string visualization = " ";

    void set_ant(){
        ant_number++;
    }
    void remove_ant(){
        ant_number--;
    }
    void set_pheromone(){
        has_pheromone = true;
    }
    void remove_pheromone(){
        has_pheromone = false;
    }
    void set_food(){
        has_food = true;
    }
    void remove_food(){
        has_food = false;
    }
    void set_anthill(){
        has_anthill = true;
    }
    void remove_anthill(){
        has_anthill = false;
    }
    void generate_visualization(){
        if(has_anthill){
            visualization = "\033[1;41m \033[0m";
        }
        else if(has_food){
            visualization = "\033[1;43m \033[0m";
        }
        else if ((ant_number==0)&(has_pheromone))
        {
            visualization = "\033[1;35m•\033[0m";
        }
        else if ((ant_number>0))
        {
            visualization = "\033[1;31m*\033[0m";
        }
        else{
            visualization = " ";
        }
    }

    void show(){
        generate_visualization();
        cout << visualization;
    }
};



struct space
{
    // Space dimension
    int width;
    int heigth;
    // Matrix with space_unit
    vector<vector<space_unit>> map;
    // Vector to store the ants
    vector<anthill> anthills;
    // Vector to store the pheromone
    vector<pheromone> pheromones;
    // Vector to store food coordinates
    vector<food> foods;

    space(int h_dimension, int w_dimension)
    {
        width = w_dimension;
        heigth = h_dimension;

        for (int i = 0; i < h_dimension; i++)
        {
            map.push_back(vector<space_unit>());
            for (int j = 0; j < w_dimension; j++)
            {
                map[i].push_back(* new space_unit());
            }
        }
    };

    void add_anthill(int number_anthills){
        for (int i = 0; i < number_anthills; i++)
        {
            anthills.push_back(*new anthill(i));

            int h_position = anthills[i].h_position;
            int w_position = anthills[i].w_position;
            // Adding anthill pointer
            map[h_position][w_position].anthill_here = &anthills[i];
            this->set_anthill_map(h_position,w_position);
        }
    }

    // Add pheromone entity to vector and add to map
    void add_pheromone(int h_position, int w_position ){
        pheromones.push_back(*new pheromone(h_position, w_position));
        // Adding pheromone pointer
        map[h_position][w_position].pheromone_here = &pheromones[pheromones.size()-1];
        set_pheromone_map(h_position,w_position);
    }
    // Add food entity to vector and add to map
    void add_food(int quantity){
        foods.push_back(*new food(quantity));
        int size_vector = foods.size();
        int w_position = foods[size_vector-1].w_position;
        int h_position = foods[size_vector-1].h_position;
        // Adding food pointer
        map[h_position][w_position].food_here = &foods[size_vector-1];
        
        set_food_map(h_position,w_position);

    }
    // Set an ant at (i,j) position in the space
    void set_ant_map(int i, int j){
        map[i][j].set_ant();
    }
    
    // Remove an ant at (i,j) position
    void remove_ant_map(int i, int j){
        map[i][j].remove_ant();
    }
    // Set pheromone to terminal visualizationat position (i,j)
    void set_pheromone_map(int i, int j){
        map[i][j].set_pheromone();
    }
    // Remove a pheromone at (i,j) position
    void remove_pheromone_map(int i, int j){
        map[i][j].remove_pheromone();
    }
    // Set food to terminal visualization at position (i,j)
    void set_food_map(int i, int j){
        map[i][j].set_food();
    }
    void remove_food_map(int i, int j){
        map[i][j].remove_food();
    }
    // Set anthill to terminal visualization at position (i,j)
    void set_anthill_map(int i, int j){
        map[i][j].set_anthill();
    }
    // Remove an anthill at (i,j) position
    void remove_anthill_map(int i, int j){
        map[i][j].remove_anthill();
    }
    // Update the terminal
    void show_map(){
        // Lines to clear the teminal
        int x=0, y=10;
        clrscr();
        gotoxy(x,y);

        // Upper wall
        for (int i = 0; i < width+2;i++)
        {
            cout << "#";
        }
        cout << "\n";

        // Left and right wall
        for (int i = 0; i < heigth; i++)
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
        for (int i = 0; i < width+2;i++)
        {
            cout << "#";
        }
        cout << "\n";

    }
};

int main(int argc, char const *argv[])
{
    space map(15, 40); 
    int w_postion = 0;
    int h_postion = 0;

    map.add_anthill(1);
    map.add_pheromone(10,20);
    map.add_food(1000);
    map.show_map();

    // map.set_food_map(10, 35);
    // map.set_anthill_map(1,1);

    // for (int i = 0; i < 15; i++)
    // {
    //     map.set_ant_map(h_postion,w_postion);
    //     map.show_map();
        
    //     // Sleep in miliseconds
    //     std::this_thread::sleep_for(std::chrono::milliseconds(500));
    //     map.set_pheromone_map(i,i);
    //     map.remove_ant_map(h_postion,w_postion);

    //     // Generating random number
    //     int RandW = rand() % 2;
    //     int RandH = rand() % 2;
        
    //     w_postion += RandW;
    //     h_postion += RandH;

    // }
    return 0;
}

