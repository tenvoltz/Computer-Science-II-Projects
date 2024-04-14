#include "Term.h"
#include <math.h>
Fraction::Fraction(int num, int den){
   numerator = num;
   denominator = den;
}
void Fraction::Simplify(){
   //Reduce the fraction by the GCD
   int gcd = Fraction::EuclideanGCD(numerator, denominator);
   numerator /= gcd;
   denominator /= gcd;
   //Move the negative sign: -a/-b = a/b and a/-b = -a/b
   if(denominator < 0){
      numerator = -numerator;
      denominator = -denominator;
   }
}
int Fraction::EuclideanGCD(int divisor, int remainder){
   //Euclidean Algorithm - continuously divide the divisor by the remainder
   if(remainder == 0) return divisor; //The remainder is 0, GCD found
   return EuclideanGCD(remainder, divisor % remainder);
}
Fraction Fraction::operator+(const Fraction& rhs) const{
   Fraction temp; // a/b + c/d = (ad + bc)/bd
   temp.numerator = this->numerator * rhs.getDenominator() +
                    this->denominator * rhs.getNumerator();
   temp.denominator = this->denominator * rhs.getDenominator();
   temp.Simplify();
   return temp;
}
Fraction Fraction::operator-(const Fraction& rhs) const{
   Fraction temp; // a/b - c/d = (ad - bc)/bd
   temp.numerator = this->numerator * rhs.getDenominator() -
                     this->denominator * rhs.getNumerator();
   temp.denominator = this->denominator * rhs.getDenominator();
   temp.Simplify();
   return temp;
}
Fraction Fraction::operator*(const Fraction& rhs) const{
   Fraction temp; // a/b * c/d = ac/bd
   temp.numerator = this->numerator * rhs.getNumerator();
   temp.denominator = this->denominator * rhs.getDenominator();
   temp.Simplify();
   return temp;
}
Fraction Fraction::operator/(const Fraction& rhs) const{
   Fraction temp; // a/b / c/d = ad/bc
   temp.numerator = this->numerator * rhs.getDenominator();
   temp.denominator = this->denominator * rhs.getNumerator();
   temp.Simplify();
   return temp;
}
bool Fraction::operator<(const Fraction& rhs) const{
   // a/b < c/d -> ad/bd < bc/bd
   return this->numerator * rhs.getDenominator() <
          this->denominator * rhs.getNumerator();
}
bool Fraction::operator>(const Fraction& rhs) const{
   // a/b < c/d -> ad/bd > bc/bd
   return this->numerator * rhs.getDenominator() >
          this->denominator * rhs.getNumerator();
}
bool Fraction::operator==(const Fraction& rhs) const{
   // a/b == c/d -> ad/bd == bc/bd
   return this->numerator * rhs.getDenominator() ==
          this->denominator * rhs.getNumerator();
}
std::ostream& operator<<(std::ostream& stream, const Fraction& frac){
   //Display numerator/denominator, () if necessary
   if(frac.getDenominator() != 1){
      stream << "(" << frac.getNumerator() << "/" << frac.getDenominator() << ")";
   }
   else{
      stream << frac.getNumerator();
   }
   return stream;
}


Term::Term(int c, std::string b, int exp){
   coefficient = Fraction(c);
   base = b;
   exponent = Fraction(exp);
}
Term::Term(Fraction c, std::string b, Fraction exp){
   coefficient = c;
   base = b;
   exponent = exp;
}
Term::Term(int c, std::string _funct, std::string b, int exp){
   coefficient = Fraction(c);
   funct = _funct;
   base = b;
   exponent = Fraction(exp);
}
Term::Term(Fraction c, std::string _funct, std::string b, Fraction exp){
   coefficient = c;
   funct = _funct;
   base = b;
   exponent = exp;
}
Term Term::AntiDerivative(std::string differential){
   // A term has an anti-derivative if the differential match or it is a constant
   if(differential == base || base == ""){
      Term term;
      if(funct == "sin") {
         // |(asin(bx)) = -(a/b)*cos(bx)
         term.funct = "cos";
         term.exponent = exponent;
         term.coefficient = coefficient * Fraction(-1) / term.exponent;
         term.base = differential;
      }
      else if(funct == "cos"){
         // |(acos(bx)) = (a/b)*sin(bx)
         term.funct = "sin";
         term.exponent = exponent;
         term.coefficient = coefficient / term.exponent;
         term.base = differential;
      }
      // |(ax^b) = a/(b+1)x^(b+1)
      else{
         term.exponent = exponent + Fraction(1);
         term.coefficient = coefficient / term.exponent;
         term.base = differential;
      }
      return term;
   }
   else return Term(0, "ERROR", 1);
}

bool Term::IsCombinable(const Term& term){
   // A term is combinable if it has the same base and exponent and function type
   return base == term.base && exponent == term.exponent && funct == term.funct;
}
Term Term::Combine(const Term& term){
   // Combine the coefficient of like term
   return Term(coefficient + term.coefficient, funct, base, exponent);
}
double Term::Evaluate(double value){
   // Evaluate at a value of the expression a*sin(b[value])
   if(funct == "sin") return (coefficient.Evaluate() * sin(value * exponent.Evaluate()));
   // Evaluate at a value of the expression a*cos(b[value])
   else if(funct == "cos") return (coefficient.Evaluate() * cos(value * exponent.Evaluate()));
   // Evaluate at a value of the expression a(value)^b
   else return (coefficient.Evaluate() * pow(value, exponent.Evaluate()));

}

bool Term::operator<(const Term& rhs) const{
   // Compare by exponent if there is no function
   if(funct == "" && rhs.funct == ""){
      return exponent < rhs.exponent;
   }
   // If similar function and base-coefficient
   if(funct == rhs.funct && exponent == rhs.exponent){
      return false;
   }
   // A function is always less than a power term
   if(funct == "") return false;
   else if(rhs.funct == "") return true;
   else return true;
}
bool Term::operator>(const Term& rhs) const{
   if(funct == "" && rhs.funct == ""){
      // Compare by exponent if there is no function
      return exponent > rhs.exponent;
   }
   // If similar function and base-coefficient
   if(funct == rhs.funct && exponent == rhs.exponent){
      return true;
   }
   // A function is always less than a power term
   if(funct == "") return true;
   else if(rhs.funct == "") return false;
   else return false;
}
bool Term::operator==(const Term& rhs) const{
   // A term is the same if their function and exponent value are the same
   return funct == rhs.funct && exponent == rhs.exponent;
}

std::ostream& operator<<(std::ostream& out, const Term& term){
   if(term.coefficient.Evaluate() == 0.0){
      // If exponent is 0, print zero
      return out << "0";
   }
   if(term.base == ""){
      // If the term is constant, print the coefficient only
      return out << term.coefficient;
   }
   // Don't print 1 if the coefficient is 1
   if(term.coefficient.Evaluate() != 1.0){
      if(term.coefficient.Evaluate() == -1.0){
         // Print only the sign if coefficient is -1
         out << "-";
      }
      else{
         out << term.coefficient;
      }
   }
   // Print the function out
   if(term.funct != ""){
      out << term.funct << " ";
      // Don't print the 1 if the base-coefficient is 1
      if(term.exponent.Evaluate() != 1.0){
         if(term.exponent.Evaluate() == -1.0){
            // Print only the sign if base-coefficient is -1
            out << "-";
         }
         else{
            out << term.exponent;
         }
      }
      out << term.base;
   }
   else{
      // Don't print x^b if the exponent is 0
      if(term.exponent.Evaluate() != 0){
         out << term.base;
         if(term.exponent.Evaluate() != 1){
            // Print the exponent if it exists
            out << "^" << term.exponent;
         }
      }
   }
   return out;
}
