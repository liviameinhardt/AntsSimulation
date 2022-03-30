#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>

#include <chrono>
#include <thread>

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


struct ant
{
    ant(){

    }

    void move(){
        
    }
};

struct pheromone
{
    /* data */
};


// Basic structure of the space
struct space_unit
{
    space_unit(){

    }

    bool has_ant = false;
    bool has_pheromone = false;
    // Pointer to the current ant
    ant* ant_here;
    // Pointer to current pheromone
    pheromone* pheromone_here;
    // Visualization of the unit
    string visualization = " ";

    // 
    void set_ant(){
        has_ant = true;
        visualization = "*";
    }

    void remove_ant(){
        has_ant = false;
        visualization = " ";
    }

    void show(){
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


    space(int w_dimension, int h_dimension)
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

    space map(40, 15);
    int w_postion = 0;
    int h_postion = 0;
    for (int i = 0; i < 30; i++)
    {
        map.set_ant(h_postion,w_postion);
        map.show_map();
        
        // Sleep in miliseconds
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        map.remove_ant(h_postion,w_postion);

        // Generating random number
        int RandW = rand() % 2;
        int RandH = rand() % 2;
        
        w_postion += RandW;
        h_postion += RandH;

        

    }


    return 0;
}

