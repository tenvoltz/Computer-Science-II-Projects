#include "Player.h"
#include <iostream>
#include <string>
#include <math.h>
#include <iomanip>
#include <sstream>
void Player::ParseRecord(std::string record){
  //Looping through the record letter-by-letter, increasing category respectively
  for(int i = 0; i < (int)record.length(); i++){
    switch(record[i]){
      case 'H': hit++;        atBat++; break;
      case 'O': out++;        atBat++; break;
      case 'K': strikeOut++;  atBat++; break;
      case 'W': walk++;                break;
      case 'P': hitByPitch++;          break;
      case 'S': sacrifice++;           break;
      default:  plateAppearance--;     break;
    }
    plateAppearance++;
  }
}
double Player::GetBattingAverage() const{
   //Batting Average = Hits / At-bats; If division by 0, set to 0
   if(atBat != 0) return (double)(hit) / atBat;
   else return 0.0;
}
double Player::GetOnBasePercent() const{
   //On-base percentage = (Hits + Walks + HBP) / Plate appearances; If division by 0, set to 0
   if(plateAppearance != 0) return (double)(hit + walk + hitByPitch) / plateAppearance;
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
      case COMPARISON_MODE::Name:             return this->name < other.name; break;
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
      case COMPARISON_MODE::Name:             return this->name > other.name; break;
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
      case COMPARISON_MODE::Name:             return this->name == other.name; break;
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
   stream << player.name << "\t";
   stream << player.atBat << "\t";
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
