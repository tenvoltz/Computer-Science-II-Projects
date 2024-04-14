#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include <string>
enum COMPARISON_MODE{Name, Team, Hits, Walks, Strikeout, HitByPitch, BattingAverage, OnBasePercentage};

class Player
{
   public:
      // Constructors
      Player(){};
      Player(std::string _name, std::string _team, std::string _result);
      ~Player(){};//{std::cout << "Deleted " << name << std::endl;}
      // Mutator and Accessors
      std::string GetName() { return name; }
      void SetName(std::string val) { name = val; }
      std::string GetTeam() { return team; }
      void SetTeam(std::string val) { team = val; }
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
      std::string GetModeData() const;
      // Function
      void ParseRecord(std::string record);
      int GetAtBat() const;
      int GetPlateAppereance() const;
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
      std::string team = "";
      int hit = 0;
      int out = 0;
      int strikeOut = 0;
      int walk = 0;
      int hitByPitch = 0;
      int sacrifice = 0;
      int error = 0;
};

#endif // PLAYER_H
