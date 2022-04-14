#include <vector>
#include <iostream>
#include <random>
#include <cstdlib>
#include <string>
#include <mutex>
#include <thread>
using namespace std;

// ************************************** MAP STRUCTURES **************************************


// visualization auxiliar functions
void gotoxy(int x, int y) { printf("%c[%d;%df", 0x1B, y, x); }
void clrscr(void) { system("clear"); }


/* 
Tail Structure 
 */
struct space_unit
{

    int ant_number = 0;
    int pheromone_life = 0;

    bool has_food = false;
    int food_quantity = 0;

    bool has_anthill = false;

    int w_pheromone_food_direction;
    int h_pheromone_food_direction;

    // Visualization of the unit
    string visualization = " ";

    void set_ant() { ant_number++; }
    void remove_ant() { ant_number--; }

    void set_anthill() { has_anthill = true; }
    void remove_anthill() { has_anthill = false; }

    void set_pheromone(int quantity) { pheromone_life=quantity ; }
    void decay_pheromone() { 
        if (pheromone_life > 0){   pheromone_life--;}
        if(pheromone_life == 0 ){ h_pheromone_food_direction= 0 ; w_pheromone_food_direction= 0;}
    }

    void set_food() { has_food = true; }
    void remove_food() { has_food = false; }


    void generate_visualization(){
        if (has_anthill){visualization = "\033[1;41m \033[0m";}
        else if (has_food){ visualization = "\033[1;43m \033[0m";}
        else if (ant_number > 0){ visualization = "\033[1;31m*\033[0m";}  
        else if (pheromone_life > 0){visualization = "\033[1;35m•\033[0m";}
        else{visualization = " ";}
    }

    void show(){
        this->generate_visualization();
        cout << visualization;
    }

};




/* 
Space Structure
 */
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
    void show_map(){

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