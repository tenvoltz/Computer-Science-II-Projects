#include "Player.h"
#include <iostream>
#include <string>
#include <math.h>
#include <iomanip>
#include <sstream>

Player::Player(std::string _name, std::string _team, std::string _result) :
   name(_name), team(_team) {ParseRecord(_result);}

void Player::ParseRecord(std::string record){
   // Increment the category respectively
   if     (record == "HITS")       hit++;
   else if(record == "OUTS")       out++;
   else if(record == "STRIKEOUT")  strikeOut++;
   else if(record == "WALK")       walk++;
   else if(record == "HITBYPITCH") hitByPitch++;
   else if(record == "SACRIFICE")  sacrifice++;
   else if(record == "ERRORS")     error++;
}
int Player::GetAtBat() const {
   return hit + out + strikeOut + error;
}
int Player::GetPlateAppereance() const {
   return hit + out + strikeOut + error + walk + hitByPitch + sacrifice;
}
double Player::GetBattingAverage() const{
   //Batting Average = Hits / At-bats; If division by 0, set to 0
   if(GetAtBat() != 0) return (double)(hit) / GetAtBat();
   else return 0.0;
}
double Player::GetOnBasePercent() const{
   //On-base percentage = (Hits + Walks + HBP) / Plate appearances; If division by 0, set to 0
   if(GetPlateAppereance() != 0) return (double)(hit + walk + hitByPitch) / GetPlateAppereance();
   else return 0.0;
}

// The default comparison is Name
COMPARISON_MODE Player::mode = COMPARISON_MODE::Name;
std::string Player::GetModeData() const{
   // Return the data in string format, rounding double as necessary
   switch(Player::mode){
      case COMPARISON_MODE::Name:             return this->name;
      case COMPARISON_MODE::BattingAverage:{
         std::stringstream stream;
         stream << std::fixed << std::setprecision(3);
         stream << Player::rounding(this->GetBattingAverage(), 3);
         return stream.str();
      }
      case COMPARISON_MODE::OnBasePercentage:{
         std::stringstream stream;
         stream << std::fixed << std::setprecision(3);
         stream << Player::rounding(this->GetOnBasePercent(), 3);
         return stream.str();
      }
      case COMPARISON_MODE::Hits:             return std::to_string(this->hit);
      case COMPARISON_MODE::Walks:            return std::to_string(this->walk);
      case COMPARISON_MODE::Strikeout:        return std::to_string(this->strikeOut);
      case COMPARISON_MODE::HitByPitch:       return std::to_string(this->hitByPitch);
      default: return this->name;
   }
}
bool Player::operator<(const Player& other) const{
   // Compare value by mode; strikeout is reverse
   switch(Player::mode){
      // Compare the team then compare the name
      case COMPARISON_MODE::Name: {
         if(this->team != other.team)
              return this->team < other.team;
         else return this->name < other.name;
         break;
      }
      case COMPARISON_MODE::BattingAverage:   return this->GetBattingAverage() < other.GetBattingAverage(); break;
      case COMPARISON_MODE::OnBasePercentage: return this->GetOnBasePercent() < other.GetOnBasePercent(); break;
      case COMPARISON_MODE::Hits:             return this->hit < other.hit; break;
      case COMPARISON_MODE::Walks:            return this->walk < other.walk; break;
      case COMPARISON_MODE::Strikeout:        return this->strikeOut > other.strikeOut; break;
      case COMPARISON_MODE::HitByPitch:       return this->hitByPitch < other.hitByPitch; break;
      default: return this->name < other.name; break;
   }
}
bool Player::operator>(const Player& other) const{
   // Compare value by mode; strikeout is reverse
   switch(Player::mode){
      // Compare the team then compare the name
      case COMPARISON_MODE::Name: {
         if(this->team != other.team)
              return this->team > other.team;
         else return this->name > other.name;
         break;
      }
      case COMPARISON_MODE::BattingAverage:   return this->GetBattingAverage() > other.GetBattingAverage(); break;
      case COMPARISON_MODE::OnBasePercentage: return this->GetOnBasePercent() > other.GetOnBasePercent(); break;
      case COMPARISON_MODE::Hits:             return this->hit > other.hit; break;
      case COMPARISON_MODE::Walks:            return this->walk > other.walk; break;
      case COMPARISON_MODE::Strikeout:        return this->strikeOut < other.strikeOut; break;
      case COMPARISON_MODE::HitByPitch:       return this->hitByPitch > other.hitByPitch; break;
      default: return this->name < other.name; break;
   }
}
bool Player::operator==(const Player& other) const{
   // Compare value by mode
   switch(Player::mode){
      case COMPARISON_MODE::Name:             return this->team == other.team && this->name == other.name; break;
      case COMPARISON_MODE::BattingAverage:   return this->GetBattingAverage() == other.GetBattingAverage(); break;
      case COMPARISON_MODE::OnBasePercentage: return this->GetOnBasePercent() == other.GetOnBasePercent(); break;
      case COMPARISON_MODE::Hits:             return this->hit == other.hit; break;
      case COMPARISON_MODE::Walks:            return this->walk == other.walk; break;
      case COMPARISON_MODE::Strikeout:        return this->strikeOut == other.strikeOut; break;
      case COMPARISON_MODE::HitByPitch:       return this->hitByPitch == other.hitByPitch; break;
      default: return this->name == other.name; break;
   }
}
double Player::rounding(double number, int decimalPlace){
  //Round a value up to X decimal places
  return round(number * pow(10.0, decimalPlace)) / pow(10.0, decimalPlace);
}
std::ostream& operator<<(std::ostream& stream, const Player& player){
   //Display each player data with a tab between each field
   stream << std::fixed << std::setprecision(0);
   stream << std::setw(15) << std::left << player.name;
   stream << player.GetAtBat() << "\t";
   stream << player.hit << "\t";
   stream << player.walk << "\t";
   stream << player.strikeOut << "\t";
   stream << player.hitByPitch << "\t";
   stream << player.sacrifice << "\t";
   stream << std::fixed << std::setprecision(3);
   stream << Player::rounding(player.GetBattingAverage(), 3) << "\t";
   stream << Player::rounding(player.GetOnBasePercent(), 3);
   return stream;
}
