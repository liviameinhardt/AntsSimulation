# Ants Simulation

The main objective of this project was to create an emergent system using multiprocessing through threads. To achieve this, a simulation of ants (which search for food and bring it back to the anthill) was created. Project developed for the Scalable Computing course at FGV EMAp, 5th semester of Data Science.

## Start the Simulation

To start the simulation, simply execute the make command in the terminal with a C++ compiler installed.

There are various scenario possibilities to simulate, and some examples are in the scenarios.cpp file in the include folder. Just comment/uncomment the chosen scenario and recompile the program using the make command to visualize the simulation with the new parameters.

Here is an example scenario showing which attributes can be defined:

```

    int NUM_THREADS = 6; // Total number of threads    
    int map_h = 20; //Map height
    int map_w = 30;//Map Width
    int simulation_time = 1400; //Simulation total time in seconds
    int pheromone_timelife = 15; //Pheromone time life (in simulation time unit)
    int ant_field_of_vision = 1; //Ant field of vision
    int max_ants_food = 9; //Number max of ants that can collect food 
    int number_of_ants = 6; // Total number of Ants
    int food_quantity = 10;  // Total food quantity
    int rep_rate = 1; // Food replacement Rate

    vector<int> anthill_position = {12, 0}; //Anthill position vector indicates (height, width)
    vector<int> food_position = {10, 15}; //Food position vector indicates (height, width)

```

## Model 

The figure bellow illustrates the implementation created to solve the problem using multiprocessing concepts. The SPACE structure manages the simulation by controlling each TAIL (implemented in the SPACE MAP structure). It stores a matrix of space map objects and controls reading and writing in these spaces. In this version, the reading and writing problem was solved using only mutex.

![model](annotations/model.png)

The space unit mentioned next stores which element is present in that space unit. This storage is different for each type of element. In the case of pheromone, the structure also handles the decrement and restart of its quantities - depending on the simulation conditions - and stores the direction it points to (towards the food). This way, the ant always follows the expected direction when there is a pheromone in its field of view. For food, a pointer is stored so that the ants can directly access and modify the object.

The food structure is quite simple; it only stores its current amount, maximum amount, and replenishment rate, and has methods to reset the quantity and decrement the current amount.

The anthill represents the "home of the ants" and has attributes that indicate its position, the list of ants that are part of it, and a pointer to the map (SPACE). The main method of this structure is to release the ants (spawn ants) during the program initialization.

Finally, the Ant structure has several attributes, the main ones being: position, movement direction, field of vision, and its status attributes (e.g., whether the ant is following pheromone, going home, or seeing food). To make changes to the map, the ant also has a pointer to the map and can store a pointer to the food it is interacting with. Depending on its status, the ant can move randomly, go home, look around, or release pheromone.

### Multithreading and Critical Regions

In this simulation, we can consider all elements as static except for the ant and its actions. In other words, the ant modifies the map by interacting with the food and pheromone. Therefore, threads were implemented to manage the ants' routine (via the antsmove function).

In this scenario, several critical regions are generated. We can define two types in our work. The first refers to reading and writing of the tails, and the second to the action of grabbing (and then decrementing) the food.

Ideally, these problems would be solved according to the statement and discussions in class (handling the tails with the solution seen for simultaneous reading and writing issues, and the second using the barber problem and subsequently the dining philosophers). However, in this version, we managed to resolve the issues only by managing specific mutexes for the functions.


### Group

- Ari Oliveira
- Carlos 
- Lívia Cereja Meinhardt
- Luiz Fernando Luz

