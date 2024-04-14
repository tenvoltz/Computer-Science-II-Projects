#include "Ant.h"
#include <string>
#include <climits>

Direction Ant::Move(int possibleClosest[4]) const{
   int closestBeetle[4] = {-1, -1, -1, -1};
   int closestAmount = GetClosestBeetle(possibleClosest, closestBeetle);
   //If there is no beetle, stay still
   if(closestAmount == 0) return Direction::Stay;
   //Move away from the closest Beetle
   if(closestAmount == 1){
      for(int dir = 0; dir < 4; dir++){
         if(closestBeetle[dir] > 0) return Reverse(Direction(dir));
      }
   }
   //Since there is multiple beetle, move in direction of no beetle
   for(int dir = 0; dir < 4; dir++){
      //Negative means no beetle
      if(possibleClosest[dir] < 0) return Direction(dir);
   }
   //Since there is no direction with no beetle, move to the farthest beetle
   int farthestBeetle[4] = {-1, -1, -1, -1};
   GetFarthestBeetle(possibleClosest, farthestBeetle);
   //Get the first farthest beetle
   for(int dir = 0; dir < 4; dir++){
      if(farthestBeetle[dir] > 0) return Direction(dir);
   }

   return Direction::Stay;
}
int Ant::GetClosestBeetle(int possibleClosest[4], int closestBeetle[4]) const{
   int closestDistance = INT_MAX;
   int closestAmount = 0;
   for(int i = 0; i < 4; i++){
      //Ignore Wall
      if(possibleClosest[i] < 0) continue;
      //Get distance digit
      int distance = possibleClosest[i];
      //Store the closest
      if(distance < closestDistance){
         closestDistance = distance;
         ResetIntArray(closestBeetle, 4, -1);
         closestBeetle[i] = possibleClosest[i];
         closestAmount = 1;
      }
      else if(distance == closestDistance){
         closestBeetle[i] = possibleClosest[i];
         closestAmount++;
      }
   }
   return closestAmount;
}
void Ant::GetFarthestBeetle(int possibleClosest[4], int farthestBeetle[4]) const{
   int farthestDistance = -1;
   for(int i = 0; i < 4; i++){
      //Ignore Wall
      if(possibleClosest[i] < 0) continue;
      //Get distance digit
      int distance = possibleClosest[i];
      //Store the farthest distance
      if(distance > farthestDistance){
         farthestDistance = distance;
         ResetIntArray(farthestBeetle, 4, -1);
         farthestBeetle[i] = distance;
      }
   }
}

Direction Ant::Breed(bool emptyNeighbor[4]) const{
   //No breeding if the not living in interval of 3 turns
   if(GetLifetime() == 0 || GetLifetime() % 3 != 0)
      return Direction::Stay;
   //Get the first empty position
   for(int dir = 0; dir < 4; dir++){
      if(emptyNeighbor[dir]) return Direction(dir);
   }
   return Direction::Stay;
}
