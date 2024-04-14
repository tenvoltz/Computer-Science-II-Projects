#ifndef TERM_H
#define TERM_H
#include <iostream>
#include <string>
#include <math.h>

// Supposed to be in a separate file
class Fraction
{
   public:
      //Constructors
      Fraction(){numerator = 0; denominator = 1;};
      Fraction(int num){numerator = num; denominator = 1;};
      Fraction(int num, int den);
      //Accessors and Mutators
      int getNumerator() const { return numerator; }
      void setNumerator(int val) { numerator = val; this->Simplify();}
      int getDenominator() const { return denominator; }
      void setDenominator(int val) { denominator = val; this->Simplify();}
      //Function
      void Simplify();
      static int EuclideanGCD(int divisor, int remainder);
      double Evaluate() const{return (double)numerator/denominator;}
      //Overloaded Math
      Fraction operator+(const Fraction& rhs) const;
      Fraction operator-(const Fraction& rhs) const;
      Fraction operator*(const Fraction& rhs) const;
      Fraction operator/(const Fraction& rhs) const;
      //Overloaded Relational
      bool operator<(const Fraction& rhs) const;
      bool operator>(const Fraction& rhs) const;
      bool operator==(const Fraction& rhs) const;
      //Overloaded Stream
      friend std::ostream& operator<<(std::ostream& stream, const Fraction& frac);
   private:
      int numerator;
      int denominator;
};



class Term{
   public:
      // Constructors
      Term(){}
      Term(int c, std::string b, int exp);
      Term(Fraction c, std::string b, Fraction exp);
      Term(int c, std::string _funct, std::string b, int exp);
      Term(Fraction c, std::string _funct, std::string b, Fraction exp);
      // Mutators and Accessors
      Fraction GetCoefficient() const { return coefficient; }
      void SetCoefficient(Fraction val) { coefficient = val;}
      void SetCoefficient(int val) { coefficient = Fraction(val);}
      std::string GetBase() const { return base; }
      void SetBase(std::string val) { base = val;}
      std::string GetFunction() const { return funct; }
      void SetFunction(std::string val) { funct = val;}
      Fraction GetExponent() const { return exponent; }
      void SetExponent(Fraction val) { exponent = val;}
      void SetExponent(int val) { exponent = Fraction(val);}
      // Functions
      bool IsNull(){return base == "NULL";}
      bool IsCombinable(const Term& term);
      Term Combine(const Term& term);
      double Evaluate(double value);
      Term AntiDerivative(std::string differential);
      // Overloading Relational
      bool operator<(const Term& rhs) const;
      bool operator>(const Term& rhs) const;
      bool operator==(const Term& rhs) const;
      // Overloading Outputs
      friend std::ostream& operator<<(std::ostream& out, const Term& term);
   private:
      Fraction coefficient;
      std::string funct;
      std::string base = "NULL";
      // If there is a function, the exponent is used as base-coefficient
      Fraction exponent;
};
#endif // TERM_H
