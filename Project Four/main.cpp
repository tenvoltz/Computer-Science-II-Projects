#include "Player.h"
#include "HashMap.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

HashMap<string, string> buildResultKeysFrom(string filename);
HashMap<string, Player> buildScoreBoardWith(HashMap<string, string>& resultKeys);
void FindLeader(vector<Player> &first, vector<Player> &second, vector<Player> &third, const set<Player> &players);
void DisplayLeader(const set<Player> &players, COMPARISON_MODE mode);

int main()
{
   // Mapping the category with the plate appearance's result
   HashMap<string, string> resultKeys = buildResultKeysFrom("keyfile.txt");
   // Construct players info from the record
   HashMap<string, Player> scoreBoard = buildScoreBoardWith(resultKeys);
   // Get the list of players
   set<Player> playerList = scoreBoard.ValueSet();
   // Print out the player, separated by team respectively
   stringstream awayTeamList;
   stringstream homeTeamList;
   awayTeamList << "AWAY" << endl;
   homeTeamList << "HOME" << endl;
   for (Player player : playerList){
      if(player.GetTeam() == "A") awayTeamList << player << endl;
      else if(player.GetTeam() == "H") homeTeamList << player << endl;
   }
   cout << awayTeamList.str() << endl;
   cout << homeTeamList.str() << endl;

   // Print out LEAGUE LEADER
   std::cout << "LEAGUE LEADERS" << std::endl;
   std::cout << "BATTING AVERAGE" << std::endl;
   DisplayLeader(playerList, COMPARISON_MODE::BattingAverage);
   std::cout << std::endl;

   std::cout << "ON-BASE PERCENTAGE" << std::endl;
   DisplayLeader(playerList, COMPARISON_MODE::OnBasePercentage);
   std::cout << std::endl;

   std::cout << "HITS" << std::endl;
   DisplayLeader(playerList, COMPARISON_MODE::Hits);
   std::cout << std::endl;

   std::cout << "WALKS" << std::endl;
   DisplayLeader(playerList, COMPARISON_MODE::Walks);
   std::cout << std::endl;

   std::cout << "STRIKEOUTS" << std::endl;
   DisplayLeader(playerList, COMPARISON_MODE::Strikeout);
   std::cout << std::endl;

   std::cout << "HIT BY PITCH" << std::endl;
   DisplayLeader(playerList, COMPARISON_MODE::HitByPitch);
   std::cout << std::endl;
   return 0;
}

HashMap<string, string> buildResultKeysFrom(string filename){
   // Opening and validate the input file
   ifstream data(filename);
   if (!data.is_open()) throw invalid_argument("Could not open file " + filename);

   HashMap<string, string> resultKeys;
   string category, key;
   while (!data.eof() && data.good()) {
      // Get the data line
      getline(data, key);
      // Process the data into categories
      if (!data.fail() && key != "") {
         // Category is in the form of ## <category> ##
         if(key.find("##") != string::npos){
            // Cut out the category in between ## and ##
            auto startIndex = key.find("##") + 2;
            auto endIndex = key.rfind("##");
            category = key.substr(startIndex, endIndex - startIndex);
            // Remove any space
            category.erase(remove(category.begin(), category.end(), ' '), category.end());
         }
         // Key is in the form of <key>
         else resultKeys.Insert(key, category);
      }
   }
   return resultKeys;
}


HashMap<string, Player> buildScoreBoardWith(HashMap<string, string>& resultKeys){
   // Ask user for the data's filename
   string filename;
   cout << "Input filename:: " << endl;
   cin >> filename;
   // filename = "sample_playbyplay.txt";

   // Opening and validate the input file
   ifstream data(filename);
   if (!data.is_open()) throw invalid_argument("Could not open file " + filename);

   HashMap<string, Player> scoreBoard;
   string team, name, result;
   while (!data.eof() && data.good()) {
      // Get the data line in form of <team> <name> <result>
      data >> team >> name >> result;
      // Process the data into player
      if (!data.fail()) {
         // If the player already exist, update their record
         if(scoreBoard.Contain(team + " " + name))
            scoreBoard.At(team + " " +name).ParseRecord(resultKeys.At(result));
         // If there is no player, add the player
         else scoreBoard.Insert(team + " " +name, Player(name, team, resultKeys.At(result)));
      }
   }
   return scoreBoard;
}

void FindLeader(vector<Player> &first, vector<Player> &second, vector<Player> &third, const set<Player> &players){
   for(Player currentPlayer : players){
      // If there's no first, add the player
      if(first.empty()){first.push_back(currentPlayer);}
      // If having similar ranking value, add the player
      else if(currentPlayer == first.at(0)){
         first.push_back(currentPlayer);
      }
      // If having higher extrema, cascade the ranking downward
      else if(currentPlayer > first.at(0)){
         third.clear();
         third = second;
         second = first;
         first.clear();
         first.push_back(currentPlayer);
      }
      // If there's no second, add the player
      else if(second.empty()){second.push_back(currentPlayer);}
      // If having similar ranking value, add the player
      else if(currentPlayer == second.at(0)){
         second.push_back(currentPlayer);
      }
      // If having higher extrema, cascade the ranking downward
      else if(currentPlayer > second.at(0)){
         third.clear();
         third = second;
         second.clear();
         second.push_back(currentPlayer);
      }
      // If there's no third, add the player
      else if(third.empty()){third.push_back(currentPlayer);}
      // If having similar ranking value, add the player
      else if(currentPlayer == third.at(0)){
         third.push_back(currentPlayer);
      }
      // If having higher extrema, cascade the ranking downward
      else if(currentPlayer > third.at(0)){
         third.clear();
         third.push_back(currentPlayer);
      }
   }
}
void DisplayLeader(const set<Player> &players, COMPARISON_MODE mode){
   // Change the comparison mode
   Player::mode = mode;
   // Find the leader in first, second, and third ranking
   vector<Player> first, second, third;
   FindLeader(first, second, third, players);

   // Display the first ranking
   if(first.empty()) return;
   cout << first.at(0).GetModeData() << "\t";
   cout << first.at(0).GetName();
   for(int i = 1; i < (int)first.size(); i++){
      cout << ", " << first.at(i).GetName();
   }
   cout << endl;

   // Display the second ranking if 3 player has not been displayed
   if(second.empty() || first.size() > 2) return;
   cout << second.at(0).GetModeData() << "\t";
   cout << second.at(0).GetName();
   for(int i = 1; i < (int)second.size(); i++){
      cout << ", " << second.at(i).GetName();
   }
   cout << endl;

   // Display the second ranking if 3 player has not been displayed
   if(third.empty() || second.size() > 1 || first.size() > 1) return;
   cout << third.at(0).GetModeData() << "\t";
   cout << third.at(0).GetName();
   for(int i = 1; i < (int)third.size(); i++){
      cout << ", " << third.at(i).GetName();
   }
   cout << endl;
}
