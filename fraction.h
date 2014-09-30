#ifndef FRACTION_H
#define FRACTION_H

#include <iostream>
using namespace std;

class Fraction {
public:
   Fraction(int numerator, int denominator);
   Fraction(int num);
   Fraction();

   friend ostream& operator<<(ostream& os, const Fraction&);
   friend istream& operator>>(istream& is, Fraction&);

   bool operator<(const Fraction&) const;
   bool operator<=(const Fraction&) const;
   bool operator>(const Fraction&) const;
   bool operator>=(const Fraction&) const;
   bool operator==(const Fraction&) const;
   bool operator!=(const Fraction&) const;

   Fraction operator+(const Fraction&) const;
   Fraction operator-(const Fraction&) const;
   Fraction operator*(const Fraction&) const;
   Fraction operator/(const Fraction&) const;
   
   const Fraction& operator+=(const Fraction&);
   const Fraction& operator-=(const Fraction&);
   const Fraction& operator*=(const Fraction&);
   const Fraction& operator/=(const Fraction&);
   
   const Fraction& operator--();
   const Fraction& operator++();

   Fraction operator--(int);
   Fraction operator++(int);

   int Numerator() const { return numerator; }
   int Denominator() const { return denominator; }
   
private:
   void Reduce();
   int GCD(int x, int y);
   
   int numerator;
   int denominator;
};

bool operator<(const Fraction&, int);
bool operator<(int, const Fraction&);
bool operator<=(const Fraction&, int);
bool operator<=(int, const Fraction&);
bool operator>(const Fraction&, int);
bool operator>(int, const Fraction&);
bool operator>=(const Fraction&, int);
bool operator>=(int, const Fraction&);
bool operator==(const Fraction&, int);
bool operator==(int, const Fraction&);
bool operator!=(const Fraction&, int);
bool operator!=(int, const Fraction&);

Fraction operator+(const Fraction&, int);
Fraction operator+(int, const Fraction&);
Fraction operator-(const Fraction&, int);
Fraction operator-(int, const Fraction&);
Fraction operator*(const Fraction&, int);
Fraction operator*(int, const Fraction&);
Fraction operator/(const Fraction&, int);
Fraction operator/(int, const Fraction&);

#endif
