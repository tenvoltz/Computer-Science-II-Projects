#include "BinarySearchTree.h"
#include "Term.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
void ParseExpression(const std::string& expression, std::string& integral, std::string& integrand, std::string& differential);
void ParseIntegrandPositive(const std::string& integrand, BinarySearchTree<Term>& bst);
void ParseIntegrandNegative(const std::string& integrand, BinarySearchTree<Term>& bst);
void ParseTerm(const std::string& term, BinarySearchTree<Term>& bst);
bool ParseIntegral(std::string integral, int& upper, int& lower);
std::string removeSpace(const std::string& line);

void CombineLikeTerm(std::vector<Term>& terms);
std::string GetAntiDerivative(std::vector<Term>& terms, std::string differential);
double Evaluate(int upper, int lower, std::vector<Term>& terms, std::string differential);

int main(){
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

   BinarySearchTree<Term> expression;

   while (!data.eof() && data.good()) {
      // Read the data, continue if line has no data
      std::string line;
      getline(data, line);
      if(line.length() == 0) continue;
      // Process the data into categories
      if (!data.fail()) {
         // Parsing the data from the line
         std::string integral, integrand, differential;
         ParseExpression(line, integral, integrand, differential);
         // Remove space for easier parsing
         integrand = removeSpace(integrand);
         ParseIntegrandPositive(integrand, expression);
         // Get the list of terms from the sorted binary tree
         std::vector<Term> terms = expression.GetVector();
         // Combine like terms
         CombineLikeTerm(terms);
         // Get anti-derivative expression
         std::string antiderivative = GetAntiDerivative(terms, differential);
         int upper, lower;
         if(ParseIntegral(integral, upper, lower)){
            std::cout << antiderivative;
            // Print the result of definite integral
            std::cout << ", " << integral << " = ";
            // Rounding to 3 decimal place
            std::cout << std::fixed << std::setprecision(3);
            std::cout << Evaluate(upper, lower, terms, differential) << std::endl;
         }
         else std::cout << antiderivative << " + C" << std::endl;
         // Clean the tree for next expression
         expression.Clear();
      }
   }
   data.close();
}

void ParseExpression(const std::string& expression, std::string& integral, std::string& integrand, std::string& differential){
   // Integral always has a space at the end
   auto integralEnd = expression.find(" ");
   // Differential always start with differential "d"
   auto differentialStart = expression.find("d") - 1;
   // Split the string up into 3 components - integral, integrand, differential
   integral = expression.substr(0, integralEnd);
   integrand = expression.substr(integralEnd + 1, differentialStart - integralEnd - 1);
   differential = expression.substr(differentialStart + 2, 1);
}
void ParseIntegrandPositive(const std::string& integrand, BinarySearchTree<Term>& bst){
   auto startTerm = 0;
   auto endTerm = integrand.find("+", 0);
   std::string term = "";
   // Repeatedly searching from the front until no positive symbol left
   while(endTerm != std::string::npos){
      // Break off the term, including the "+" sign
      term = integrand.substr(startTerm, endTerm - startTerm);
      // Pass through to parse the negative, if any
      ParseIntegrandNegative(term, bst);
      // Advance forward
      startTerm = (int)endTerm;
      endTerm = integrand.find("+", endTerm + 1);
   }
   // Fencepost problem - Flush the rest of the string
   term = integrand.substr(startTerm);
   ParseIntegrandNegative(term, bst);
}
void ParseIntegrandNegative(const std::string& integrand, BinarySearchTree<Term>& bst){
   auto startTerm = 0;
   auto endTerm = integrand.find("-", 0);
   std::string term = "";
   // Repeatedly searching from the front until no negative symbol left
   while(endTerm != std::string::npos){
      // Avoid breaking off the negative exponent
      if(endTerm == 0 || integrand.at(endTerm - 1) != '^'){
         // Break off the term, including the "-" sign
         term = integrand.substr(startTerm, endTerm - startTerm);
         ParseTerm(term, bst);
         startTerm = (int)endTerm;
      }
      // Advance forward
      endTerm = integrand.find("-", endTerm + 1);
   }
   // Fencepost problem - Flush the rest of the string
   term = integrand.substr(startTerm);
   ParseTerm(term, bst);
}
void ParseTerm(const std::string& str, BinarySearchTree<Term>& bst){
   // Ignore empty string
   if(str == "") return;
   // Test for coefficient
   char* containChar;
   auto coefficient = std::strtol(str.c_str(), &containChar, 10);
   // The term is a constant
   if(!(*containChar)){
      bst.Insert(Term((int)coefficient, "", 0));
      return;
   }
   // The term has a variable
   Term term;
   // The term has no coefficient
   if(*containChar == *str.begin()){
      // If there is only negative sign, coefficient is -1
      if(*containChar == '-') term.SetCoefficient(-1);
      // If there is no coefficient, the default is 1
      else term.SetCoefficient(1);
   }
   // If coefficient is 0, the term is a 0 constant
   else if (coefficient == 0){
      bst.Insert(Term(0, "", 0));
      return;
   }
   // Else, there is a coefficient and extract it
   else term.SetCoefficient((int)coefficient);
   // If contain function SIN or COS
   if(str.find("sin") != std::string::npos || str.find("cos") != std::string::npos){
      int functionStart;
      // Extract the function
      if((functionStart = (int)str.find("sin")) != (int)std::string::npos) term.SetFunction("sin");
      else if((functionStart = (int)str.find("cos")) != (int)std::string::npos) term.SetFunction("cos");
      // Get the internal term
      std::string internal = str.substr(functionStart + 3);
      // Test for coefficient
      char* containBase;
      auto baseCoefficient = std::strtol(internal.c_str(), &containBase, 10);
      // Extract the base
      std::string base;
      base = *containBase;
      term.SetBase(base);
      // If there is no base-coefficient, set it to 1
      if(*containBase == *internal.begin()) term.SetExponent(1);
      // Else, extract the base-coefficient
      else term.SetExponent((int)baseCoefficient);
   }
   else{
      // If the char is sign, advance once to get to the base
      if(*containChar == '+' || *containChar == '-') ++containChar;
      // Extract the base
      std::string base;
      base = *containChar;
      term.SetBase(base);
      // Test for exponent
      auto powerSign = str.find("^");
      if(powerSign != std::string::npos){ // Exponent exists
         // Extract the exponent
         term.SetExponent(std::stoi(str.substr(powerSign + 1)));
      }
      // Exponent does not exists, the default exponent is 1
      else term.SetExponent(1);
   }
   // Insert the term into the tree
   bst.Insert(term);

}
bool ParseIntegral(std::string integral, int& upper, int& lower){
   // If only the bar "|", the integral is indefinite
   if(integral.length() == 1) return false;
   // Find the bar and parse upper and lower bound
   int barIndex = (int)integral.find("|");
   lower = std::stoi(integral.substr(0, barIndex));
   upper = std::stoi(integral.substr(barIndex+1));
   return true;
}
std::string removeSpace(const std::string& line){
   std::string modified = "";
   // Iterate through the string, only add non-space characters
   for(char c : line){
      if(!isspace(c)) modified += c;
   }
   return modified;
}

void CombineLikeTerm(std::vector<Term>& terms){
   std::vector<Term> combined;
   Term prev;
   // Iterate through all the ordered terms
   for(Term term: terms){
      // Combine like term
      if(term.IsCombinable(prev)){
         prev = prev.Combine(term);
      }
      // Push the previous like term
      else{
         if(!prev.IsNull()) combined.push_back(prev);
         prev = term;
      }
   }
   // Fencepost problem - Flush the last like term
   if(!prev.IsNull()) combined.push_back(prev);
   terms = combined;
}
std::string GetAntiDerivative(std::vector<Term>& terms, std::string differential){
   std::stringstream stream;
   // Going backward from max exponent to min exponent
   for(int i = (int)terms.size()-1; i >= 0; i--){
      stream << terms.at(i).AntiDerivative(differential) << " + ";
   }
   // Fencepost problem - Delete the last plus sign
   std::string result = stream.str();
   result = result.substr(0, result.length() - 3);
   int signIndex;
   // Replace all "+ -" with only "-"
   while((signIndex = (int)result.find("+ -")) != (int)std::string::npos)
      result.replace(signIndex, 3, "- ");
   // Replace all "+ (-" with only "- ("
   while((signIndex = (int)result.find("+ (-")) != (int)std::string::npos)
      result.replace(signIndex, 4, "- (");
   // If anti-derivative is empty, default to zero
   if(result == "") result = "0";
   return result;
}
double Evaluate(int upper, int lower, std::vector<Term>& terms, std::string differential){
   double result = 0.0;
   // For all term, compute the value of anti-derivative: a|b = F(b) - F(a)
   for(Term t : terms){
      Term antiDerivative = t.AntiDerivative(differential);
      result += antiDerivative.Evaluate(upper) - antiDerivative.Evaluate(lower);
   }
   return result;
}
