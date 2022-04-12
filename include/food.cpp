
#include <vector>
using namespace std;

struct food
{
    vector<int> position;
    int quantity;
    int id;

    void decay_quantity(){
        quantity = quantity - 1;
    }

    void spawn(){

        // after spawn time has passed / quantity = 0 run this function

    
        int RandH = rand() % 2;
        int RandW = rand() % 2;

        position.push_back(RandH);
        position.push_back(RandW);

        // still need to check if new position is available
        

    }


};
