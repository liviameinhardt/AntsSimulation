#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <chrono>
#include <thread>
struct anthill
{
    int w_position;
    int h_position;
    anthill(int id){
        // Aqui deve ter uma geração da posição aleatória
        w_position = 1;
        h_position = 1;
    }   
};
