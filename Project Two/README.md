# Baseball Player Statistics Tracker

This project implements a linked list data structure to manage and sort baseball player statistics. It includes functionality to read player data from a file, parse and update player statistics, sort players based on various criteria, and identify league leaders in different statistical categories.

## Files Included

### `main.cpp`

Main program file responsible for reading player data from a file, processing it, and displaying league leaders.

### `LinkedList.h` and `Node.h`

These files define the linked list used to store the player records.

### `Player.h` and `player.cpp`

These files define the `Player` class, which represents a player's records.

## File Format

The program expects the input file to contain data in the following format:

`<player_name>` `<record>`

Where:

- `<player_name>` is the name of the baseball player.
- `<record>` is a string representing the player's performance in a game, consisting of characters:
  - `H`: Hit
  - `O`: Out
  - `K`: Strikeout
  - `W`: Walk
  - `P`: Hit-by-pitch
  - `S`: Sacrifice

## Statistical Categories

1. Batting Average (BA): Hits / At-bats
2. On-base Percentage (OB%): (Hits + Walks + Hit-by-pitch) / Plate appearances
3. Hits (H)
4. Walks (BB)
5. Strikeouts (K)
6. Hit-by-pitch (HBP)

## Output

The program displays the following information:

- Player name
- Batting Average (rounded to 3 decimal places)
- On-base Percentage (rounded to 3 decimal places)
- Hits
- Walks
- Strikeouts
- Hit-by-pitch

It also identifies first, second, and third league leaders in each statistical category and displays their names along with their corresponding statistics as described above.
