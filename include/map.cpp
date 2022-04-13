
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <chrono>
#include <thread>

// include required modules 
#include "anthill.cpp"
#include "food.cpp"
#include "pheromone.cpp"


// Code to always update terminal
void gotoxy(int x,int y){printf("%c[%d;%df",0x1B,y,x);}
void clrscr(void){ system("clear"); }


// Basic structure of the space
struct space_unit
{
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

    void set_ant(){ ant_number++; }
    void remove_ant(){ ant_number--; }
    void set_pheromone(){ has_pheromone = true; }
    void remove_pheromone(){has_pheromone = false; }
    void set_food(){ has_food = true; }
    void remove_food(){ has_food = false;}
    void set_anthill(){ has_anthill = true;}
    void remove_anthill(){has_anthill = false;}


    void generate_visualization(){

        if(has_anthill){
            visualization = "\033[1;41m \033[0m";
            // visualization = "A";
        }
        else if(has_food){
            visualization = "\033[1;43m \033[0m";
            // visualization = "O";
        }
        else if ((ant_number==0)&(has_pheromone))
        {
            visualization = "\033[1;35m•\033[0m";
            // visualization = ".";
        }
        else if ((ant_number>0))
        {
            visualization = "\033[1;31m*\033[0m";
            // visualization = "*";
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

    vector<vector<space_unit>> map; // Matrix with space_unit
    vector<anthill> anthills; // Vector to store the anthills
    vector<pheromone> pheromones; // Vector to store the pheromone
    vector<food> foods; // Vector to store food coordinates

    // ******************* CONSTRUCTOR **************************
    space(int h_dimension, int w_dimension){
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

    // ************************************************ INICIALIZATION FUNCTIONS ************************************************
    void add_anthill(vector<vector<int>> anthill_info,vector<vector<int>>ants_info){

        for (int i = 0; i < anthill_info.size(); i++)
        {
            anthills.push_back(*new anthill(anthill_info[i][0],anthill_info[i][1],i)); 

            int h_position = anthills[i].h_position;
            int w_position = anthills[i].w_position;

            // Adding anthill pointer
            map[h_position][w_position].anthill_here = &anthills[i];
            this->set_anthill_map(h_position,w_position);

            anthills[i].spawn_ants(ants_info[i][0],ants_info[i][1]); // create ants
        }

    }

    void add_food(vector<vector<int>> food_info){

        for (int i = 0; i < food_info.size(); i++){
        foods.push_back(*new food(food_info[i][0],food_info[i][1],food_info[i][2]));

        int w_position = foods[i].w_position;
        int h_position = foods[i].h_position;

        // Adding food pointer
        map[h_position][w_position].food_here = &foods[i];
        set_food_map(h_position,w_position);
        
        }
    }

    //***************************************** SIMULATION FUNCTIONS ************************************************
    void add_pheromone(int h_position, int w_position, int life){
        pheromones.push_back(*new pheromone(h_position, w_position,life));
        map[h_position][w_position].pheromone_here = &pheromones[pheromones.size()-1];  // Adding pheromone pointer
        set_pheromone_map(h_position,w_position);
    }


    //***************************************** CLASS FUNCTIONS ************************************************
    void set_ant_map(int i, int j){ map[i][j].set_ant(); } // Set an ant at (i,j) position in the space
    void remove_ant_map(int i, int j){ map[i][j].remove_ant();} // Remove an ant at (i,j) position
    void set_pheromone_map(int i, int j){ map[i][j].set_pheromone();}  // Set pheromone to terminal visualizationat position (i,j)
    void remove_pheromone_map(int i, int j){ map[i][j].remove_pheromone();} // Remove a pheromone at (i,j) position
    void set_food_map(int i, int j){ map[i][j].set_food();} // Set food to terminal visualization at position (i,j)
    void remove_food_map(int i, int j){ map[i][j].remove_food();}
    void set_anthill_map(int i, int j){ map[i][j].set_anthill();} // Set anthill to terminal visualization at position (i,j)
    void remove_anthill_map(int i, int j){map[i][j].remove_anthill();} // Remove an anthill at (i,j) position



    // //***************************************** Update the terminal //*****************************************
    void show_map(){

        // Lines to clear the teminal
        int x=0, y=10;
        clrscr();
        gotoxy(x,y);

        // Upper wall
        for (int i = 0; i < width+2;i++){ cout << "#"; }

        cout << "\n";

        // Left and right wall
        for (int i = 0; i < heigth; i++){

            cout << "#";

            for (int j = 0; j < width; j++){
                map[i][j].show();
            }

            cout << "#";
            cout << "\n";
        }
        
        // Down wall
        for (int i = 0; i < width+2;i++){ cout << "#";}
        cout << "\n";

    }
};