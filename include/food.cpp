
#include <vector>
using namespace std;

struct food
{
    int h_position;
    int w_position;
    int max_quantity;
    int current_quantity;

    //constructor
    food(int height,int weight,int int_quantity){
        h_position = height;
        w_position = weight;
        current_quantity = int_quantity;
        max_quantity = int_quantity;
    }

    void decay_quantity(){
        current_quantity = current_quantity - 1;
    }

    void reset_quantity(){
        current_quantity = max_quantity;        
    }


};
