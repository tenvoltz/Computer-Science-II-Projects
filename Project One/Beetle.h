#ifndef BEETLE_H
#define BEETLE_H

#include "Creature.h"
#include <iostream>
#include <string>

class Beetle : public Creature
{
   public:
      //Constructors
      Beetle():Creature() { m_eatTimer = 0; }
      Beetle(int lifetime, int eatTimer):Creature(lifetime){ m_eatTimer = eatTimer; }

      //Accessors and Mutators
      int GetEatTimer() const { return m_eatTimer; }
      void SetEatTimer(int val) { m_eatTimer = val; }
      void AddEatTimer(int val) { m_eatTimer += val; }

      //Overloading Output
      friend std::ostream& operator<<(std::ostream& os, const Beetle& beetle){
         os << beetle.toString();
         return os;
      }
      std::string toString() const{
         return "Beetle:{EatTimer: " + std::to_string(m_eatTimer) + ", " + Creature::toString() + "}";
      }

      //Overriding Function
      Direction Move(int possibleClosest[4]) const override;
      Direction Breed(bool emptyNeighbor[4]) const override;

      //Functions
      int GetClosestAnt(int possibleClosest[4], int closestAnt[4]) const;
      void GetMostNeighborAnt(int closestAnt[4], int mostNeighbors[4]) const;
      void GetFarthestWall(int possibleClosest[4], int farthestWall[4]) const;
      bool Starve() const {return m_eatTimer != 0 && m_eatTimer % 5 == 0;}
   private:
      int m_eatTimer;
};

#endif // BEETLE_H
