#include "LinkedList.h"
#include "LinkedList.cpp"
#include "Node.h"
#include "Player.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

void FindLeader(vector<Node<Player>*> &first, vector<Node<Player>*> &second, vector<Node<Player>*> &third, LinkedList<Player>& players);
void DisplayLeader(LinkedList<Player>& linkedList, COMPARISON_MODE mode);

int main()
{
   // Ask user for the data's filename
   std::string filename;
   std::cout << "Input filename:: ";
   std::cin >> filename;

   // Opening and validate the input file
   std::ifstream data(filename);
   if (!data.is_open()) {
      std::cout << "Could not open file " << filename << std::endl;
      return 1;
   }

   LinkedList<Player> linkedList;

   while (!data.eof() && data.good()) {
      // Read the data in the format: <name> <record>
      string playerName;
      string rawRecord;
      data >> playerName >> rawRecord;
      // Process the data into categories
      if (!data.fail()) {
         Player player(playerName, rawRecord);
         Node<Player>* node = linkedList.Search(player);
         if(node == nullptr){
            linkedList.Append(new Node<Player>(player));
         }
         else{
            node->GetData().ParseRecord(rawRecord);
         }
      }
   }
   data.close();

   // Sort and display the list of Player alphabetically by name
   Player::mode = COMPARISON_MODE::Name;
   linkedList.InsertionSort();
   std::cout << linkedList << std::endl;

   // Print out LEAGUE LEADER
   std::cout << "LEAGUE LEADERS" << std::endl;
   std::cout << "BATTING AVERAGE" << std::endl;
   DisplayLeader(linkedList, COMPARISON_MODE::BattingAverage);
   std::cout << std::endl;

   std::cout << "ON-BASE PERCENTAGE" << std::endl;
   DisplayLeader(linkedList, COMPARISON_MODE::OnBasePercentage);
   std::cout << std::endl;

   std::cout << "HITS" << std::endl;
   DisplayLeader(linkedList, COMPARISON_MODE::Hits);
   std::cout << std::endl;

   std::cout << "WALKS" << std::endl;
   DisplayLeader(linkedList, COMPARISON_MODE::Walks);
   std::cout << std::endl;

   std::cout << "STRIKEOUTS" << std::endl;
   DisplayLeader(linkedList, COMPARISON_MODE::Strikeout);
   std::cout << std::endl;

   std::cout << "HIT BY PITCH" << std::endl;
   DisplayLeader(linkedList, COMPARISON_MODE::HitByPitch);
   std::cout << std::endl;
}

void FindLeader(vector<Node<Player>*> &first, vector<Node<Player>*> &second, vector<Node<Player>*> &third, LinkedList<Player> &players){
   // Traverse through the linked list
   Node<Player>* currentPlayer = players.GetHeadPtr();
   while(currentPlayer != nullptr){
      // If there's no first, add the player
      if(first.empty()){first.push_back(currentPlayer);}
      // If having similar ranking value, add the player
      else if(*currentPlayer == *first.at(0)){
         first.push_back(currentPlayer);
      }
      // If having higher extrema, cascade the ranking downward
      else if(*currentPlayer > *first.at(0)){
         third.clear();
         third = second;
         second = first;
         first.clear();
         first.push_back(currentPlayer);
      }
      // If there's no second, add the player
      else if(second.empty()){second.push_back(currentPlayer);}
      // If having similar ranking value, add the player
      else if(*currentPlayer == *second.at(0)){
         second.push_back(currentPlayer);
      }
      // If having higher extrema, cascade the ranking downward
      else if(*currentPlayer > *second.at(0)){
         third.clear();
         third = second;
         second.clear();
         second.push_back(currentPlayer);
      }
      // If there's no third, add the player
      else if(third.empty()){third.push_back(currentPlayer);}
      // If having similar ranking value, add the player
      else if(*currentPlayer == *third.at(0)){
         third.push_back(currentPlayer);
      }
      // If having higher extrema, cascade the ranking downward
      else if(*currentPlayer > *third.at(0)){
         third.clear();
         third.push_back(currentPlayer);
      }
      currentPlayer = currentPlayer->GetNextPtr();
   }
}
void DisplayLeader(LinkedList<Player>& linkedList, COMPARISON_MODE mode){
   // Change the comparison mode
   Player::mode = mode;
   // Find the leader in first, second, and third ranking
   vector<Node<Player>*> first;
   vector<Node<Player>*> second;
   vector<Node<Player>*> third;
   FindLeader(first, second, third, linkedList);

   // Display the first ranking
   if(first.empty()) return;
   std::cout << first.at(0)->GetData().GetModeData() << "\t";
   std::cout << first.at(0)->GetData().GetName();
   for(int i = 1; i < (int)first.size(); i++){
      std::cout << ", " << first.at(i)->GetData().GetName();
   }
   std::cout << std:: endl;

   // Display the second ranking if 3 player has not been displayed
   if(second.empty() || first.size() > 2) return;
   std::cout << second.at(0)->GetData().GetModeData() << "\t";
   std::cout << second.at(0)->GetData().GetName();
   for(int i = 1; i < (int)second.size(); i++){
      std::cout << ", " << second.at(i)->GetData().GetName();
   }
   std::cout << std:: endl;

   // Display the second ranking if 3 player has not been displayed
   if(third.empty() || second.size() > 1 || first.size() > 1) return;
   std::cout << third.at(0)->GetData().GetModeData() << "\t";
   std::cout << third.at(0)->GetData().GetName();
   for(int i = 1; i < (int)third.size(); i++){
      std::cout << ", " << third.at(i)->GetData().GetName();
   }
   std::cout << std:: endl;
}
