

struct food
{
    int max_quantity;
    int current_quantity = 0;
    int replacement_rate;

    // constructor
    food(int int_quantity, int rep_rate){
        current_quantity = int_quantity;
        max_quantity = int_quantity;
        replacement_rate = rep_rate;
    }


    void decay_quantity(){current_quantity = current_quantity - 1;}
    void reset_quantity(){current_quantity = max_quantity;}

};