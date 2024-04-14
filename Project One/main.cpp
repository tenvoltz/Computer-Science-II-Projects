#include "Creature.h"
#include "Ant.h"
#include "Beetle.h"
#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>

//CONSTANTS
const int BOARD_SIZE = 10;
//PROTOTYPE
   //Processing Input & Output
void ProcessData(Creature* boardRow[BOARD_SIZE], const std::string& dataLine);
void ClearBoard(Creature* board[BOARD_SIZE][BOARD_SIZE]);
void DisplayBoard(Creature* board[BOARD_SIZE][BOARD_SIZE], char antChar, char beetleChar);
   //Ant Movement
void AntMove(Creature* board[BOARD_SIZE][BOARD_SIZE]);
void ScanBeetle(Creature* board[BOARD_SIZE][BOARD_SIZE], Position pos, int closestData[4]);
int ClosestBeetle2D(Creature* board[BOARD_SIZE][BOARD_SIZE], Position startPos, Position endPos, Position comparePos);
   //Beetle Movement
void BeetleMove(Creature* board[BOARD_SIZE][BOARD_SIZE]);
void ScanAnt(Creature* board[BOARD_SIZE][BOARD_SIZE], Position pos, int closestData[4]);
int ClosestAnt2D(Creature* board[BOARD_SIZE][BOARD_SIZE], Position startPos, Position endPos, Position comparePos);
int GetNeighbor(Creature* board[BOARD_SIZE][BOARD_SIZE], Position antPos);
   //Beetle Starve
void BeetleStarve(Creature* board[BOARD_SIZE][BOARD_SIZE]);
   //Creature Breeding
void AntBreed(Creature* board[BOARD_SIZE][BOARD_SIZE]);
void BeetleBreed(Creature* board[BOARD_SIZE][BOARD_SIZE]);
void GetEmptyNeighbor(Creature* board[BOARD_SIZE][BOARD_SIZE], Position pos, bool emptyNeighbor[4]);
   //Helper Function
void SetStatus(Creature* board[BOARD_SIZE][BOARD_SIZE]);
bool OutOfBound(Position pos);



int main(){
   // Ask user for the data's filename
   std::string filename;
   //std::cout << "Input filename:: ";
   std::cin >> filename;

   // Opening and validate the input file
   std::ifstream data(filename);
   if (!data.is_open()) {
      std::cout << "Could not open file " << filename << std::endl;
      return 1;
   }

   //Initialize board to nullptr
   Creature* board[BOARD_SIZE][BOARD_SIZE] = {nullptr};

   for (int row = 0; row < BOARD_SIZE && data.good(); row++) {
      // Get the line
      std::string worldLine;
      std::getline(data, worldLine);

      // Process the data into creatures and storing them
      if (!data.fail()) { ProcessData(board[row], worldLine); }
   }
   data.close();

   //Prompt for creature's output
   char antChar;
   //std::cout << "Ant Character:: ";
   std::cin >> antChar;
   char beetleChar;
   //std::cout << "Beetle Character: ";
   std::cin >> beetleChar;

   //Ask user for amount of turn
   int turnAmount;
   //std::cout << "Turn Amount:: ";
   std::cin >> turnAmount;

   //Turn-based loop
   for(int turn = 0; turn < turnAmount; turn++){
      std::cout << "TURN " << turn + 1 << std::endl;  //Print the turn
      SetStatus(board);                               //Set the move+lifetime+starve status
      BeetleMove(board);                              //Beetle movement
      AntMove(board);                                 //Ant movement
      BeetleStarve(board);                            //Beetle check starving
      AntBreed(board);                                //Ant breed
      BeetleBreed(board);                             //Beetle breed
      DisplayBoard(board, antChar, beetleChar);       //Print the board at the end of the turn
   }

   //Free up any dynamic allocation
   ClearBoard(board);
   return 0;
}

void AntMove(Creature* board[BOARD_SIZE][BOARD_SIZE]){
   for(int col = 0; col < BOARD_SIZE; col++){
      for(int row = 0; row < BOARD_SIZE; row++){
         //Skip if the current space is not beetle
         Ant* ant = dynamic_cast<Ant*>(board[row][col]);
         if(ant == nullptr) continue;

         //Check Move Status, do not move twice in a turn
         if(ant->AlreadyMoved()) continue;
         else ant->SetMoved(true);

         //Get closest beetle's data (Distance)
         Position pos(col, row);
         int closestBeetleData[4];
         ScanBeetle(board, pos, closestBeetleData);

         //Get direction to move
         Direction direction = ant->Move(closestBeetleData);

         //If stay, continue to next ant
         if(direction == Direction::Stay) continue;
         pos.MoveTo(direction);

         //Stay if out of bound
         if(OutOfBound(pos)) continue;

         //Stay if not going to an empty space
         if(board[pos.GetRow()][pos.GetCol()] != nullptr) continue;

         //Swap position to the newPosition
         board[pos.GetRow()][pos.GetCol()] = ant;
         board[row][col] = nullptr;
      }
   }
}
void ScanBeetle(Creature* board[BOARD_SIZE][BOARD_SIZE], Position pos, int closestData[4]){
   //Scan North
   Position northWall(pos.GetCol(), 0);
   closestData[0] = ClosestBeetle2D(board, northWall, pos, pos);
   //Scan East
   Position eastWall(BOARD_SIZE - 1, pos.GetRow());
   closestData[1] = ClosestBeetle2D(board, pos, eastWall, pos);
   //Scan South
   Position southWall(pos.GetCol(), BOARD_SIZE - 1);
   closestData[2] = ClosestBeetle2D(board, pos, southWall, pos);
   //Scan West
   Position westWall(0, pos.GetRow());
   closestData[3] = ClosestBeetle2D(board, westWall, pos, pos);
}
int ClosestBeetle2D(Creature* board[BOARD_SIZE][BOARD_SIZE], Position startPos, Position endPos, Position comparePos){
   int closestDistance = BOARD_SIZE;
   bool beetleFound = false;
   for(int row = startPos.GetRow(); row <= endPos.GetRow(); row++){
      for(int col = startPos.GetCol(); col <= endPos.GetCol(); col++){
         //Ignore self
         if(row == comparePos.GetRow() && col == comparePos.GetCol()) continue;
         //Check if the tile is an ant
         Beetle* beetle = dynamic_cast<Beetle*>(board[row][col]);
         if(beetle == nullptr) continue;
         //Calculate distance to ant
         Position beetlePos(col, row);
         int distance = beetlePos.DistanceTo(comparePos);
         //Store the closest
         if(distance < closestDistance){
            closestDistance = distance;
            beetleFound = true;
         }
      }
   }
   if(beetleFound) return closestDistance;
   else return -1; //Return -1 if no beetle found

}

void BeetleMove(Creature* board[BOARD_SIZE][BOARD_SIZE]){
   for(int col = 0; col < BOARD_SIZE; col++){
      for(int row = 0; row < BOARD_SIZE; row++){
         //Skip if the current space is not beetle
         Beetle* beetle = dynamic_cast<Beetle*>(board[row][col]);
         if(beetle == nullptr) continue;

         //Check Move Status, do not move twice in a turn
         if(beetle->AlreadyMoved()) continue;
         else beetle->SetMoved(true);

         //Get closest ant's data (Distance & Neighbors)
         Position pos(col, row);
         int closestAntData[4];
         ScanAnt(board, pos, closestAntData);

         //Get direction to move
         Direction direction = beetle->Move(closestAntData);

         //If stay, continue to next beetle
         if(direction == Direction::Stay) continue;
         pos.MoveTo(direction);

         //Stay if out of bound
         if(OutOfBound(pos)) continue;

         //Stay if hitting another beetle
         Beetle* otherBeetle = dynamic_cast<Beetle*>(board[pos.GetRow()][pos.GetCol()]);
         if(otherBeetle != nullptr) continue;

         //Eat if hitting another ant
         Ant* otherAnt = dynamic_cast<Ant*>(board[pos.GetRow()][pos.GetCol()]);
         if(otherAnt != nullptr){
            delete otherAnt;
            beetle->SetEatTimer(0);
            board[pos.GetRow()][pos.GetCol()] = beetle;
            board[row][col] = nullptr;
            continue;
         }

         //Swap position to the newPosition
         board[pos.GetRow()][pos.GetCol()] = beetle;
         board[row][col] = nullptr;
      }
   }
}
void ScanAnt(Creature* board[BOARD_SIZE][BOARD_SIZE], Position pos, int closestData[4]){
   //Scan North
   Position northWall(pos.GetCol(), 0);
   closestData[0] = ClosestAnt2D(board, northWall, pos, pos);
   //Scan East
   Position eastWall(BOARD_SIZE - 1, pos.GetRow());
   closestData[1] = ClosestAnt2D(board, pos, eastWall, pos);
   //Scan South
   Position southWall(pos.GetCol(), BOARD_SIZE - 1);
   closestData[2] = ClosestAnt2D(board, pos, southWall, pos);
   //Scan West
   Position westWall(0, pos.GetRow());
   closestData[3] = ClosestAnt2D(board, westWall, pos, pos);
}
int ClosestAnt2D(Creature* board[BOARD_SIZE][BOARD_SIZE], Position startPos, Position endPos, Position comparePos){
   int closestDistance = BOARD_SIZE;
   Position closestPosition;
   bool antFound = false;
   for(int row = startPos.GetRow(); row <= endPos.GetRow(); row++){
      for(int col = startPos.GetCol(); col <= endPos.GetCol(); col++){
         //Ignore self
         if(row == comparePos.GetRow() && col == comparePos.GetCol()) continue;
         //Check if the tile is an ant
         Ant* ant = dynamic_cast<Ant*>(board[row][col]);
         if(ant == nullptr) continue;
         //Calculate distance to ant
         Position antPos(col, row);
         int distance = antPos.DistanceTo(comparePos);
         //Store the closest
         if(distance < closestDistance){
            closestDistance = distance;
            closestPosition = antPos;
            antFound = true;
         }
      }
   }
   if(antFound){
      int neighborAmount = GetNeighbor(board, closestPosition);
      //Encode distance XX and neighbor Y into XXY
      return closestDistance * 10 + neighborAmount;
   }
   else{
      //Negative distance to wall include wall if no ant found
      //Encode distance XX and neighbor Y into XXY with no neighbor (Y=0)
      return -(startPos.DistanceTo(endPos) + 1) * 10;
   }
}
int GetNeighbor(Creature* board[BOARD_SIZE][BOARD_SIZE], Position antPos){
   int neighborAmount = 0;
   for(int deltaRow = -1; deltaRow <= 1; deltaRow++){
      for(int deltaCol = -1; deltaCol <= 1; deltaCol++){
         //Do not count self
         if(deltaCol == 0 && deltaRow == 0) continue;

         Position pos(antPos.GetCol() + deltaCol, antPos.GetRow() + deltaRow);
         //Do not count out of bound
         if(OutOfBound(pos)) continue;
         //Count nearby ant in 3x3 square
         Ant* ant = dynamic_cast<Ant*>(board[pos.GetRow()][pos.GetCol()]);
         if(ant != nullptr) neighborAmount++;
      }
   }
   return neighborAmount;
}

void BeetleStarve(Creature* board[BOARD_SIZE][BOARD_SIZE]){
   for(int col = 0; col < BOARD_SIZE; col++){
      for(int row = 0; row < BOARD_SIZE; row++){
         //Skip if the current space is not beetle
         Beetle* beetle = dynamic_cast<Beetle*>(board[row][col]);
         if(beetle == nullptr) continue;

         //Get starve condition
         bool starve = beetle->Starve();

         //Delete beetle that had starved
         if(starve){
            delete board[row][col];
            board[row][col] = nullptr;
         }
      }
   }
}

void AntBreed(Creature* board[BOARD_SIZE][BOARD_SIZE]){
   for(int col = 0; col < BOARD_SIZE; col++){
      for(int row = 0; row < BOARD_SIZE; row++){
         //Skip if the current space is not beetle
         Ant* ant = dynamic_cast<Ant*>(board[row][col]);
         if(ant == nullptr) continue;

         //Get Empty Tile in cardinal direction
         bool emptyNeighbor[4];
         Position pos(col, row);
         GetEmptyNeighbor(board, pos, emptyNeighbor);
         Direction dir = ant->Breed(emptyNeighbor);

         //Ignore no breeding
         if(dir == Direction::Stay) continue;

         //Breed in the direction provided
         pos.MoveTo(dir);
         board[pos.GetRow()][pos.GetCol()] = new Ant();
      }
   }
}
void BeetleBreed(Creature* board[BOARD_SIZE][BOARD_SIZE]){
   for(int col = 0; col < BOARD_SIZE; col++){
      for(int row = 0; row < BOARD_SIZE; row++){
         //Skip if the current space is not beetle
         Beetle* beetle = dynamic_cast<Beetle*>(board[row][col]);
         if(beetle == nullptr) continue;

         //Get Empty Tile in cardinal direction
         bool emptyNeighbor[4];
         Position pos(col, row);
         GetEmptyNeighbor(board, pos, emptyNeighbor);
         Direction dir = beetle->Breed(emptyNeighbor);

         //Ignore no breeding
         if(dir == Direction::Stay) continue;

         //Breed in the direction provided
         pos.MoveTo(dir);
         board[pos.GetRow()][pos.GetCol()] = new Beetle();
      }
   }
}
void GetEmptyNeighbor(Creature* board[BOARD_SIZE][BOARD_SIZE], Position pos, bool emptyNeighbor[4]){
   //Get the empty tile in 4 cardinal direction
   Position North(pos.GetCol(), pos.GetRow()-1);
   emptyNeighbor[0] = !OutOfBound(North) && board[North.GetRow()][North.GetCol()] == nullptr;
   Position East(pos.GetCol()+1, pos.GetRow());
   emptyNeighbor[1] = !OutOfBound(East) && board[East.GetRow()][East.GetCol()] == nullptr;
   Position South(pos.GetCol(), pos.GetRow()+1);
   emptyNeighbor[2] = !OutOfBound(South) && board[South.GetRow()][South.GetCol()] == nullptr;
   Position West(pos.GetCol()-1, pos.GetRow());
   emptyNeighbor[3] = !OutOfBound(West) && board[West.GetRow()][West.GetCol()] == nullptr;
}

void SetStatus(Creature* board[BOARD_SIZE][BOARD_SIZE]){
   for(int col = 0; col < BOARD_SIZE; col++){
      for(int row = 0; row < BOARD_SIZE; row++){
         //Skip if the current space is empty
         if(board[row][col] == nullptr) continue;
         //Reset the movement check
         board[row][col]->SetMoved(false);
         //Age the creature by 1 turn
         board[row][col]->AddLifetime(1);
         //If beetle, increase time from last meal by 1
         Beetle* beetle = dynamic_cast<Beetle*>(board[row][col]);
         if(beetle != nullptr) beetle->AddEatTimer(1);
      }
   }
}
bool OutOfBound(Position pos){
   //Check for out of bound for position row and col
   bool outOfBound = false;
   if(pos.GetX() < 0 || pos.GetX() >= BOARD_SIZE) outOfBound = true;
   if(pos.GetY() < 0 || pos.GetY() >= BOARD_SIZE) outOfBound = true;
   return outOfBound;
}

void ProcessData(Creature* boardRow[BOARD_SIZE], const std::string& dataLine){
   for(int i = 0; i < BOARD_SIZE; i++){
      //Default character is a space if the line is less than 10 char
      char creatureChar = ' ';
      if(i < (int)dataLine.length())
         creatureChar = dataLine.at(i);

      //Converting character into Creature
      switch(creatureChar){
         case 'a': boardRow[i] = new Ant();     break;
         case 'B': boardRow[i] = new Beetle();  break;
         default:  boardRow[i] = nullptr;       break;
      }
   }
}
void DisplayBoard(Creature* board[BOARD_SIZE][BOARD_SIZE], char antChar, char beetleChar){
   for(int row = 0; row < BOARD_SIZE; row++){
      for(int col = 0; col < BOARD_SIZE; col++){
         //If empty, print a space
         if(board[row][col] == nullptr) std::cout << " ";

         //Dynamic cast to an ant pointer
         Ant* ant = dynamic_cast<Ant*>(board[row][col]);
         if(ant != nullptr) std::cout << antChar;

         //Dynamic cast to a beetle pointer
         Beetle* beetle = dynamic_cast<Beetle*>(board[row][col]);
         if(beetle != nullptr) std::cout << beetleChar;
      }
      std::cout << std::endl;
   }
   std::cout << std::endl;
}
void ClearBoard(Creature* board[BOARD_SIZE][BOARD_SIZE]){
   //Go through each element and free the pointer
   for(int row = 0; row < BOARD_SIZE; row++){
      for(int col = 0; col < BOARD_SIZE; col++){
         delete board[row][col];
      }
   }
}
