#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <math.h>
using namespace std;

//CONSTANT
const int MAX_ARRAY_SIZE = 30;
//PROTOTYPE
void parseRecord(const string& record, int results[]);
void calculateStats(const int results[], double playerStats[][MAX_ARRAY_SIZE], int playerIndex);
void displayPlayerData(const string playerName[], const double playerStats[][MAX_ARRAY_SIZE], int playerIndex);
double rounding(double number, int decimalPlace);
void findLeader(const double categoryStats[], bool highest, int leaderIndex[], int playerAmount, int& leaderAmount);
void displayLeaderCategory(const char category[], const string playerNames[], const double categoryStats[], const int leaderIndex[], int leaderAmount, int decimalPlaces);
void displayLeader(const string playerNames[], const double playerStats[][MAX_ARRAY_SIZE], int playerAmount);

int main() {
  string playerNames[MAX_ARRAY_SIZE] = {};
  double playerStats[6][MAX_ARRAY_SIZE] = {};
  int playerAmount = 0;

  // Ask user for the data's filename
  string filename;
  //cout << "Input filename::" << endl;
  cin >> filename;

  // Opening and validate the file
  ifstream data(filename);
  if (!data.is_open()) {
    cout << "Could not open file " << filename << endl;
    return 1;
  }

  while (!data.eof() && data.good()) {
    // Read the data in the format: <name> <record>
    string playerName;
    string rawRecord;
    data >> playerName >> rawRecord;
    // Process the data into categories and display it
    if (!data.fail()) {
      playerNames[playerAmount] = playerName;
      int resultRecord[8] = {};
      parseRecord(rawRecord, resultRecord);
      calculateStats(resultRecord, playerStats, playerAmount);
      displayPlayerData(playerNames, playerStats, playerAmount);
      playerAmount++;
    }
  }
  data.close();
  displayLeader(playerNames, playerStats, playerAmount);
}

void parseRecord(const string& record, int results[]){
  //Looping through the record letter-by-letter, increasing category respectively
  for(int i = 0; i < (int)record.length(); i++){
    switch(record[i]){
      case 'H': results[0]++; results[6]++; break;  //0 - Hits
      case 'O': results[1]++; results[6]++; break;  //1 - Out
      case 'K': results[2]++; results[6]++; break;  //2 - Strikeout
      case 'W': results[3]++;               break;  //3 - Walk
      case 'P': results[4]++;               break;  //4 - Hit-by-pitch
      case 'S': results[5]++;               break;  //5 - Sacrifice
      default:  results[7]--;               break;  //6 - At-bat
    }                                               //7 - Plate Apperance
    results[7]++;
  }
}

void calculateStats(const int results[], double playerStats[][MAX_ARRAY_SIZE], int playerIndex){
  //Batting Average = Hits / At-bats; If division by 0, set to 0
  if(results[6] != 0)
    playerStats[0][playerIndex] = (double)(results[0]) / results[6];
  else playerStats[0][playerIndex] = 0.0;
  //On-base percentage = (Hits + Walks + HBP) / Plate appearances; If division by 0, set to 0
  if(results[7] != 0)
    playerStats[1][playerIndex] = (double)(results[0] + results[3] + results[4]) / results[7];
  else playerStats[1][playerIndex] = 0.0;
  playerStats[2][playerIndex] = results[0]; //Hits
  playerStats[3][playerIndex] = results[3]; //Walk
  playerStats[4][playerIndex] = results[2]; //Strikeout
  playerStats[5][playerIndex] = results[4]; //Hit-by-pitch
}

void displayPlayerData(const string playerName[], const double playerStats[][MAX_ARRAY_SIZE], int playerIndex){
  cout << playerName[playerIndex] << endl;
  //Batting Average and On-base Percentage round to 3 decimal places
  cout << fixed << setprecision(3);
  cout << "BA: "  << rounding(playerStats[0][playerIndex], 3) << endl; //Batting Average
  cout << "OB%: " << rounding(playerStats[1][playerIndex], 3) << endl; //On-base Percentage
  //The rest is display as integer
  cout << fixed << setprecision(0);
  cout << "H: "  << rounding(playerStats[2][playerIndex], 0) << endl; //Hits
  cout << "BB: "  << rounding(playerStats[3][playerIndex], 0) << endl; //Walk
  cout << "K: "   << rounding(playerStats[4][playerIndex], 0) << endl; //Strikeout
  cout << "HBP: " << rounding(playerStats[5][playerIndex], 0) << endl; //Hit-by-pitch
  cout << endl;
}

double rounding(double number, int decimalPlace){
  //Round a value up to X decimal places
  return round(number * pow(10.0, decimalPlace)) / pow(10.0, decimalPlace);
}

void findLeader(const double categoryStats[], bool highest, int leaderIndex[], int playerAmount, int& leaderAmount){
  //Set the extrema value and list of leader to the first player
  double currentExtrema = categoryStats[0];
  leaderAmount = 0;
  leaderIndex[leaderAmount] = 0;
  leaderAmount++;
  for(int i = 1; i < playerAmount; i++){
    //If a new maxima found, reset the list to the new maxima
    if(highest && categoryStats[i] > currentExtrema){
      currentExtrema = categoryStats[i];
      leaderAmount = 0;
      leaderIndex[leaderAmount] = i;
      leaderAmount++;
    }
    //If a new minima found, reset the list to the new minima
    else if(!highest && categoryStats[i] < currentExtrema){
      currentExtrema = categoryStats[i];
      leaderAmount = 0;
      leaderIndex[leaderAmount] = i;
      leaderAmount++;
    }
    //Append leader with the same extrema
    else if(categoryStats[i] == currentExtrema){
      leaderIndex[leaderAmount] = i;
      leaderAmount++;
    }
  }
}
void displayLeaderCategory(const char category[], const string playerNames[], const double categoryStats[], const int leaderIndex[], int leaderAmount, int decimalPlaces){
  cout << category;
  //Convert list of leader's index to <name list> <value>; account for off-by-one and rounding
  for(int i = 0; i < leaderAmount - 1; i++){
    cout << playerNames[leaderIndex[i]] << ", ";
  }
  cout << playerNames[leaderIndex[leaderAmount - 1]] << " ";
  cout << fixed << setprecision(decimalPlaces);
  cout << rounding(categoryStats[leaderIndex[0]], decimalPlaces) << endl;
}


void displayLeader(const string playerNames[], const double playerStats[][MAX_ARRAY_SIZE], int playerAmount){
  cout << "LEAGUE LEADERS" << endl;
  int leaderAmount = 0;
  int leaderIndex[MAX_ARRAY_SIZE] = {};
  //Find and display leaders for each categories
  findLeader(playerStats[0], true, leaderIndex, playerAmount, leaderAmount);
  displayLeaderCategory("BA: ",  playerNames, playerStats[0], leaderIndex, leaderAmount, 3);
  findLeader(playerStats[1], true, leaderIndex, playerAmount, leaderAmount);
  displayLeaderCategory("OB%: ", playerNames, playerStats[1], leaderIndex, leaderAmount, 3);
  findLeader(playerStats[2], true, leaderIndex, playerAmount, leaderAmount);
  displayLeaderCategory("H: ",   playerNames, playerStats[2], leaderIndex, leaderAmount, 0);
  findLeader(playerStats[3], true, leaderIndex, playerAmount, leaderAmount);
  displayLeaderCategory("BB: ",  playerNames, playerStats[3], leaderIndex, leaderAmount, 0);
  findLeader(playerStats[4], false, leaderIndex, playerAmount, leaderAmount);
  displayLeaderCategory("K: ",   playerNames, playerStats[4], leaderIndex, leaderAmount, 0);
  findLeader(playerStats[5], true, leaderIndex, playerAmount, leaderAmount);
  displayLeaderCategory("HBP: ", playerNames, playerStats[5], leaderIndex, leaderAmount, 0);
}
