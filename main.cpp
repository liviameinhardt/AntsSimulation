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
    // Pointer to the current ant
    ant* ant_here;
    // Pointer to current pheromone
    pheromone* pheromone_here;
    // Visualization of the unit
    string visualization = " ";

    // 
    void set_ant(){
        ant_number++;
        visualization = "*";
    }

    void remove_ant(){
        ant_number--;
        visualization = " ";
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


    void generate_visualization(){
        if ((ant_number==0)&(has_pheromone))
        {
            visualization = "\033[1;35m•\033[0m";
        }
        else if ((ant_number>0))
        {
            visualization = "\033[1;36m*\033[0m";
        }

        if(has_food){
            visualization = "\033[1;43m \033[0m";
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
    vector<ant> ants;
    
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

    // Set an ant at (i,j) position in the space
    void set_ant(int i, int j){
        map[i][j].set_ant();
    }
    
    // Remove an ant at (i,j) position
    void remove_ant(int i, int j){
        map[i][j].remove_ant();
    }

    void set_pheromone(int i, int j){
        map[i][j].set_pheromone();
    }
    void remove_pheromone(int i, int j){
        map[i][j].remove_pheromone();
    }

    void set_food(int i, int j){
        map[i][j].set_food();
    }
    void remove_food(int i, int j){
        map[i][j].remove_food();
    }
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
    map.set_food(10, 35);

    for (int i = 0; i < 15; i++)
    {
        map.set_ant(h_postion,w_postion);
        map.show_map();
        
        // Sleep in miliseconds
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        map.set_pheromone(i,i);
        map.remove_ant(h_postion,w_postion);

        // Generating random number
        int RandW = rand() % 2;
        int RandH = rand() % 2;
        
        w_postion += RandW;
        h_postion += RandH;

    }
    return 0;
}

