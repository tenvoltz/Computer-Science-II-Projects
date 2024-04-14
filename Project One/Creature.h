#ifndef CREATURE_H
#define CREATURE_H
#include <iostream>
#include <string>

enum Direction{North = 0, East = 1, South = 2, West = 3, Stay};
inline Direction Reverse(Direction dir){
   switch(dir){
      case Direction::North: return Direction::South; break;
      case Direction::East:  return Direction::West;  break;
      case Direction::South: return Direction::North; break;
      case Direction::West:  return Direction::East;  break;
      case Direction::Stay:
      default:               return Direction::Stay;  break;
   }
}
class Position{
   public:
      //Constructor
      Position(){}
      Position(int col, int row){
         this->x = col;
         this->y = row;
      }
      //Accessors and Mutators
      int GetX() const{ return x; }
      void SetX(int val) { x = val; }
      int GetY() const{ return y; }
      void SetY(int val) { y = val; }
      int GetRow() const { return y; }
      int GetCol() const { return x; }
      //Functions
      int DistanceTo(const Position& other){
         return abs(other.GetX() - x) + abs(other.GetY() - y);
      }
      void MoveTo(const Direction& dir){
         switch(dir){
            case Direction::North: y -= 1; break;
            case Direction::East:  x += 1; break;
            case Direction::South: y += 1; break;
            case Direction::West:  x -= 1; break;
            case Direction::Stay:          break;
            default:                       break;
         }
      }
      //Overloading Output
      friend std::ostream& operator<<(std::ostream& os, const Position& pos){
         os << pos.toString();
         return os;
      }
      std::string toString() const{
         return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
      }
   private:
      int x = 0;
      int y = 0;
};

class Creature
{
   public:
      //Constructors
      Creature() {m_lifetime = 0; m_moved = false;}
      Creature(int lifetime){m_lifetime = lifetime; m_moved = false;}

      //Accessors and mutators
      int GetLifetime() const { return m_lifetime; }
      void SetLifetime(int val) { m_lifetime = val; }
      void AddLifetime(int addAmount) {m_lifetime += addAmount;}
      bool AlreadyMoved() const { return m_moved; }
      void SetMoved(bool val) { m_moved = val; }

      //Overloading Output
      friend std::ostream& operator<<(std::ostream& os, const Creature& creature){
         os << creature.toString();
         return os;
      }
      std::string toString() const{
         return "Creature:{Lifetime: " + std::to_string(m_lifetime) + "}";
      }

      //Abstract Function
      virtual Direction Move(int possibleClosest[4]) const = 0;
      virtual Direction Breed(bool emptyNeighbor[4]) const = 0;

      //Virtual Destructor
      virtual ~Creature(){}
   private:
      int m_lifetime;
      bool m_moved;
};

inline void ResetIntArray(int arr[], int arrSize, int value){
   for(int i = 0; i < arrSize; i++){
      arr[i] = value;
   }
}

#endif // CREATURE_H
