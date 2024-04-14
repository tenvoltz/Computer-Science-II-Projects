# Baseball Scoreboard System

This is a C++ program for managing a baseball scoreboard system usign HashMap. It includes functionality for parsing baseball game records, calculating player statistics, and determining league leaders based on various performance metrics.

## Files

### `main.cpp`

The `main.cpp` file is the entry point of the program. Key functionalities include building the scoreboard, determining league leaders, and displaying player statistics.

### `HashMap.h`

This file contains the implementation of a custom hash map data structure used in the program. It includes templates for the hash map, buckets, and entries. The hash map is implemented using open addressing and double hashing for collision resolution.

### `Player.h` and `Player.cpp`

These files define the `Player` class, which represents a baseball player. The class includes methods for parsing game records, calculating statistics such as batting average and on-base percentage, and comparing players based on different performance metrics.

## Statistical Categories

1. Batting Average (BA): Hits / At-bats
2. On-base Percentage (OB%): (Hits + Walks + Hit-by-pitch) / Plate appearances
3. Hits (H)
4. Walks (BB)
5. Strikeouts (K)
6. Hit-by-pitch (HBP)

## Input

The following seciton describes the format for inputting data related to a baseball game. Each line represents an event in the game, consisting of the team abbreviation (A for AWAY, H for HOME), player name, and the outcome of the play.

Input Format

The input section in the README.md file describes the format for inputting data related to a baseball game. Each line represents an event in the game, consisting of the team abbreviation (A for AWAY, H for HOME), player name, and the outcome of the play.

The format for input is as follows:

```
    <TEAM ABBREVIATION> <PLAYER NAME> <PLAY OUTCOME>
```

Where:

```
    <TEAM ABBREVIATION>: A single letter representing the team the player belongs to (A for AWAY, H for HOME).
    <PLAYER NAME>: The name of the player involved in the play.
    <PLAY OUTCOME>: Describes the outcome of the play, which is mapped using the keyfile.txt
```

## Output

The following section provides detailed statistics from a baseball game between two teams, AWAY and HOME, along with league leaders.

**Team AWAY**

This section lists the performance statistics of players from the AWAY team, including their at-bats, runs, hits, RBIs, walks, strikeouts, batting average (AVG), and on-base percentage (OBP).

**Team HOME**

Similarly, this section presents the performance statistics of players from the HOME team, covering their at-bats, runs, hits, RBIs, walks, strikeouts, batting average (AVG), and on-base percentage (OBP).

**League Leaders**

Under this subsection, the first, second, and third leading players in specific statistical categories are highlighted.
