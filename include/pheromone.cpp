#include <vector>
#include <iostream>

using namespace std;
struct pheromone
{
    int h_position;
    int w_position;
    int life_time;

    // constructor
    pheromone(int height, int weight, int life){
        h_position = height;
        w_position = weight;
        life_time = life;
    }

    void decay_life(){
        life_time = life_time - 1;
    }

    void increase_life(int time){
        life_time = life_time + time;
    }

};


// // Testando 
// int main()
// {
//     pheromone test(10,10,4);
//     test.decay_life();

//     cout << test.life_time << endl;

//     test.increase_life(2);

//     cout << test.life_time;


//     return 0;
// }

