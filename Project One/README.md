# Creature Simulation

This project is a simple simulation of creatures (ants and beetles) moving on a 10x10 board. Each turn, ants and beetles make decisions based on their surroundings and perform actions such as moving, breeding, and eating.

## Files

### `Ant.h` and `Ant.cpp`

These files define the `Ant` class, which represents an ant creature in the simulation. It includes methods for moving and breeding, as well as helper functions for finding the closest beetle and farthest beetle.

### `Beetle.h` and `Beetle.cpp`

These files define the `Beetle` class, representing a beetle creature in the simulation. Similar to `Ant`, it includes methods for moving, breeding, and starving, as well as helper functions for finding the closest ant and farthest wall.

### `Creature.h`

This file defines the `Creature` base class, from which `Ant` and `Beetle` inherit. It includes common attributes and methods shared by both types of creatures, such as lifetime management and pure virtual functions for movement and breeding.

### `main.cpp`

The main file orchestrates the simulation. It reads input from a file to initialize the board, runs the simulation for a specified number of turns, and displays the board after each turn. It also contains functions for creature movement, breeding, and starvation.

### `Position` (inside `Creature.h`)

A helper class representing a position on the board. It includes methods for calculating distance and moving to adjacent positions.

### `Direction` (inside `Creature.h`)

An enum representing cardinal directions and a `Stay` option. It includes a helper to get the reverse direction.

## Input File Format

The input file should contain 10 lines, each representing a row on the board. Each character in a line represents a cell on the board, with the following a mapping of your choice. An example mappign could be:

    'a': Ant
    'B': Beetle
    Any other character: Empty space

For example:

```
    B aa
   B   aa
  B     aa
 B       a
B       aa
       aa
a     aa
aa   aa  B
 aa aa  B
  aaa  B
```

## Output

After each turn, the program will display the current state of the board, showing ants and beetles as specified characters and empty spaces as spaces. The simulation will run for the specified number of turns.

## Rules

### Ant Movement Rules

The movement of the ant is governed by the following rules:

1. **No Nearby Beetles**: If there are no beetles nearby, the ant stays still.

2. **Single Nearby Beetle**: If there is only one beetle nearby, the ant moves away from it. It selects the direction opposite to the direction of the nearest beetle.

3. **Multiple Nearby Beetles**: When there are multiple beetles nearby, the ant prioritizes moving to a space where there is no beetle. It chooses the first available direction without a beetle.

4. **No Safe Space**: If there is no direction without a beetle nearby, the ant moves towards the farthest beetle. It selects the direction towards the farthest beetle.

### Ant Breeding Rules

The breeding behavior of the ant is governed by the following rules:

1. **Breed in Intervals**: Ants breed only if they have been alive for an interval of 8 turns.

2. **Occupying Empty Neighbor**: When it's time to breed, ants will search for the first available empty position in their neighboring cells.

3. **Stay Still if No Empty Neighbor**: If there are no empty neighboring spaces available, the ants will stay still and not breed.

### Beetle Movement Rules

The movement of the beetle is determined by the following rules:

1. **Move Towards Closest Ant**: If there is only one closest ant, the beetle will move towards it. It prioritizes proximity to ants.

2. **Move Towards Ant with Most Neighbors**: If there are multiple closest ants, the beetle will move towards the ant with the greatest number of neighboring ants.

3. **Move Towards Farthest Wall**: If there are no ants nearby, the beetle will move towards the farthest wall.

### Beetle Breeding Rules

The breeding behavior of beetles follows these rules:

1. **Breed in Intervals**: Beetles breed only if they have been alive for an interval of 8 turns.

2. **Occupying Empty Neighbor**: When it's time to breed, beetles will search for the first available empty position in their neighboring cells.

3. **Stay Still if No Empty Neighbor**: If there are no empty neighboring spaces available, the bettles will stay still and not breed.

### Beetle Starvation Condition

Beetles face starvation if they do not consume food within a certain time frame. The starvation condition follows these rules:

1. **Starvation Interval**: Beetles starve if they have not consumed food for a consecutive period of 5 turns.
