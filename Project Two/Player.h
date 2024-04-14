#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include <string>
enum COMPARISON_MODE{Name, Hits, Walks, Strikeout, HitByPitch, BattingAverage, OnBasePercentage};

class Player
{
   public:
      // Constructors
      Player(){};
      Player(std::string _name, std::string record){name = _name; ParseRecord(record);};
      ~Player(){};//{std::cout << "Deleted " << name << std::endl;}
      // Mutator and Accessors
      std::string GetName() { return name; }
      void SetName(std::string val) { name = val; }
      int GetHit() { return hit; }
      void SetHit(int val) { hit = val; }
      int GetOut() { return out; }
      void SetOut(int val) { out = val; }
      int GetStrikeOut() { return strikeOut; }
      void SetStrikeOut(int val) { strikeOut = val; }
      int GetWalk() { return walk; }
      void SetWalk(int val) { walk = val; }
      int GetHitByPitch() { return hitByPitch; }
      void SetHitByPitch(int val) { hitByPitch = val; }
      int GetSacrifice() { return sacrifice; }
      void SetSacrifice(int val) { sacrifice = val; }
      int GetAtBat() { return atBat; }
      void SetAtBat(int val) { atBat = val; }
      int GetPlateAppearance() { return plateAppearance; }
      void SetPlateAppearance(int val) { plateAppearance = val; }
      std::string GetModeData() const;
      // Function
      void ParseRecord(std::string record);
      double GetBattingAverage() const;
      double GetOnBasePercent() const;
      // Overloading Comparison
      bool operator<(const Player& other) const;
      bool operator>(const Player& other) const;
      bool operator==(const Player& other) const;
      static COMPARISON_MODE mode;
      // Overloading Output
      friend std::ostream& operator<<(std::ostream& stream, const Player& player);
      static double rounding(double number, int decimalPlace);
   private:
      std::string name = "";
      int hit = 0;
      int out = 0;
      int strikeOut = 0;
      int walk = 0;
      int hitByPitch = 0;
      int sacrifice = 0;
      int atBat = 0;
      int plateAppearance = 0;
};

#endif // PLAYER_H
