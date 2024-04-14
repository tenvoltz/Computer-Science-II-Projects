# Baseball Player Statistics Analyzer

This C++ program reads data from a file containing baseball player records and calculates various statistics to determine league leaders in different categories.

## Author Information

- **Name:** Tan Vo

## Features

- Parses baseball player records from a file.
- Calculates batting average, on-base percentage, hits, walks, strikeouts, and hit-by-pitch statistics for each player.
- Identifies league leaders in each statistical category.
- Displays player statistics and league leaders.

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

It identifies league leaders in each statistical category and displays their names along with their corresponding statistics as described above.
