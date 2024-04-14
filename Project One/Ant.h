#ifndef ANT_H
#define ANT_H

#include "Creature.h"
#include <string>
#include <iostream>

class Ant : public Creature
{
   public:
      //Constructors
      Ant():Creature() {}
      Ant(int lifetime):Creature(lifetime){}

      //Overloading Output
      friend std::ostream& operator<<(std::ostream& os, const Ant& ant){
         os << ant.toString();
         return os;
      }
      std::string toString() const{
         return "Ant:{" + Creature::toString() + "}";
      }

      //Overriding Function
      Direction Move(int possibleClosest[4]) const override;
      Direction Breed(bool emptyNeighbor[4]) const override;

      //Functions
      int GetClosestBeetle(int possibleClosest[4], int closestBeetle[4]) const;
      void GetFarthestBeetle(int possibleClosest[4], int farthestBeetle[4]) const;
};

#endif // ANT_H
