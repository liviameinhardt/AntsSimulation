struct food
{
    int quantity;
    int w_position;
    int h_position;
    food(int quantity){
        quantity = quantity;
        // Suponha que aqui existe uma função para gerar a posição aleatoriamente
        w_position = 10;
        h_position = 10;
    }
};
