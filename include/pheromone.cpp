#include <vector>
#include <iostream>

using namespace std;
struct pheromone
{
    vector<int> position;
    int life_time;

    void decay_life(){
        life_time = life_time - 1;
    }

    void increase_life(int time){
        life_time = life_time + time;
    }

};


// Testando 
int main()
{
    struct pheromone test;
    test.life_time = 10;
    test.decay_life();

    cout << test.life_time << endl;

    test.increase_life(2);

    cout << test.life_time;


    return 0;
}

