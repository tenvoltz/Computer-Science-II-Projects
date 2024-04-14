#include "Beetle.h"
#include <stdlib.h>
#include <climits>

Direction Beetle::Move(int possibleClosest[4]) const{
   int closestAnt[4] = {-1, -1, -1, -1};
   int closestAmount = GetClosestAnt(possibleClosest, closestAnt);
   //Move to the closest Ant
   if(closestAmount == 1){
      for(int dir = 0; dir < 4; dir++){
         if(closestAnt[dir] > 0) return Direction(dir);
      }
   }
   //If more than one closest ant, move to the greater neighbors count
   if(closestAmount > 1){
      int mostNeighbors[4] = {-1, -1, -1, -1};
      GetMostNeighborAnt(closestAnt, mostNeighbors);
      //Get the first highest neighbor
      for(int dir = 0; dir < 4; dir++){
         if(mostNeighbors[dir] > 0) return Direction(dir);
      }
   }
   //If no ant, move to the farthest wall
   if(closestAmount == 0){
      int farthestWall[4] = {-1, -1, -1, -1};
      GetFarthestWall(possibleClosest, farthestWall);
      //Get the first farthest wall
      for(int dir = 0; dir < 4; dir++){
         if(farthestWall[dir] > 0) return Direction(dir);
      }
   }
   return Direction::Stay;
}
int Beetle::GetClosestAnt(int possibleClosest[4], int closestAnt[4]) const{
   int closestDistance = INT_MAX;
   int closestAmount = 0;
   for(int i = 0; i < 4; i++){
      //Ignore Wall
      if(possibleClosest[i] < 0) continue;
      //Get distance digit
      int distance = possibleClosest[i] / 10;
      //Store the closest
      if(distance < closestDistance){
         closestDistance = distance;
         ResetIntArray(closestAnt, 4, -1);
         closestAnt[i] = possibleClosest[i];
         closestAmount = 1;
      }
      else if(distance == closestDistance){
         closestAnt[i] = possibleClosest[i];
         closestAmount++;
      }
   }
   return closestAmount;
}
void Beetle::GetMostNeighborAnt(int closestAnt[4], int mostNeighbors[4]) const{
   int mostNeighbor = -1;
   for(int i = 0; i < 4; i++){
      //Ignore Non-closest
      if(closestAnt[i] < 0) continue;
      //Get neighbor digit
      int neighbor = closestAnt[i] % 10;
      //Store the most neighbor
      if(neighbor > mostNeighbor){
         mostNeighbor = neighbor;
         ResetIntArray(mostNeighbors, 4, -1);
         mostNeighbors[i] = neighbor;
      }
   }
}
void Beetle::GetFarthestWall(int possibleClosest[4], int farthestWall[4]) const{
   int farthestDistance = -1;
   for(int i = 0; i < 4; i++){
      //Ignore Non-Wall
      if(possibleClosest[i] > 0) continue;
      //Get wall distance digit
      int distance = abs(possibleClosest[i] / 10);
      //Store the farthest distance
      if(distance > farthestDistance){
         farthestDistance = distance;
         ResetIntArray(farthestWall, 4, -1);
         farthestWall[i] = distance;
      }
   }
}

Direction Beetle::Breed(bool emptyNeighbor[4]) const{
   //No breeding if the not living in interval of 8 turns
   if(GetLifetime() == 0 || GetLifetime() % 8 != 0)
      return Direction::Stay;
   //Get the first empty position
   for(int dir = 0; dir < 4; dir++){
      if(emptyNeighbor[dir]) return Direction(dir);
   }
   return Direction::Stay;
}

