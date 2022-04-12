#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <chrono>
#include <thread>


struct pheromone
{
    int h_position;
    int w_position;
    pheromone(int h_position, int w_position){
        h_position = h_position;
        w_position = w_position;
    }
};
