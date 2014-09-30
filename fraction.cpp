#include <cstdlib>
#include "fraction.h"

Fraction::Fraction()
{
   numerator = 0;
   denominator = 1;
}

Fraction::Fraction(int num)
{
   numerator = num;
   denominator = 1;
}

Fraction::Fraction(int n, int d)
{
   if (d < 0) {
      numerator = -n;
      denominator = -d;
   } else {
      numerator = n;
      denominator = d;
   }
   Reduce();
}

ostream& operator<<(ostream& os, const Fraction& f)
{
   if (f.numerator == 0) {
      os << "0";
      return os;
   }
   
   if (f.numerator < 0)
      os << "-";

   int i = abs(f.numerator / f.denominator);
   if (i > 0)
      os << i;
   
   if (f.denominator > 1) {
      int r = abs(f.numerator % f.denominator);
      if (r > 0) {
         if (i > 0)
            os << "+";
         os << r << "/" << f.denominator;
      }
   }
   return os;
}

istream& operator>>(istream& is, Fraction& f) {
  char c;
   
  // skip delimiters
  while (true) {
    c = is.peek();
    if (c != ' ' && c != '\n' && c != '\t')
      break;
    if (!is.get(c))
      return is;
  }
  
  bool negative = false;
  if (is.peek() == '-') {
    negative = true;
    is.get(c);
  }
  
  int num, whole, numerator, denominator;
  num = whole = numerator = 0;
  denominator = 1;

  is >> num;
  
  if (!is.eof() && is.peek() == '+') {
    whole = num;
    is.get(c);
    is >> numerator;
  } else {
    whole = 0;
    numerator = num;
  }
  
  if (!is.eof() && is.peek() == '/') {
    is.get(c);
    is >> denominator;
  } else
    denominator = 1;
  
  numerator += denominator * whole;
  
  if (negative)
    numerator = -numerator;
  
  f = Fraction(numerator, denominator);
  
  return is;
}

bool
Fraction::operator<(const Fraction& f) const
{
   return numerator * f.denominator < f.numerator * denominator;
}

bool
operator<(const Fraction& f, int i)
{
   return f < Fraction(i);
}

bool
operator<(int i, const Fraction& f)
{
   return Fraction(i) < f;
}

bool
Fraction::operator<=(const Fraction& f) const
{
   return numerator * f.denominator <= f.numerator * denominator;
}

bool
operator<=(const Fraction& f, int i)
{
   return f <= Fraction(i);
}

bool
operator<=(int i, const Fraction& f)
{
   return Fraction(i) <= f;
}

bool
Fraction::operator>(const Fraction& f) const
{
   return numerator * f.denominator > f.numerator * denominator;
}

bool
operator>(const Fraction& f, int i)
{
   return f > Fraction(i);
}

bool
operator>(int i, const Fraction& f)
{
   return Fraction(i) > f;
}

bool
Fraction::operator>=(const Fraction& f) const
{
   return numerator * f.denominator >= f.numerator * denominator;
}

bool
operator>=(const Fraction& f, int i)
{
   return f >= Fraction(i);
}

bool
operator>=(int i, const Fraction& f)
{
   return Fraction(i) >= f;
}

bool
Fraction::operator==(const Fraction& f) const
{
   return numerator * f.denominator == f.numerator * denominator;
}

bool
operator==(const Fraction& f, int i)
{
   return f == Fraction(i);
}

bool
operator==(int i, const Fraction& f)
{
   return Fraction(i) == f;
}

bool
Fraction::operator!=(const Fraction& f) const
{
   return numerator * f.denominator != f.numerator * denominator;
}

bool
operator!=(const Fraction& f, int i)
{
   return f != Fraction(i);
}

bool
operator!=(int i, const Fraction& f)
{
   return Fraction(i) != f;
}

Fraction
Fraction::operator+(const Fraction& f) const
{
   Fraction ft = *this;
   ft += f;
   return ft;
}

Fraction
operator+(const Fraction& f, int i)
{
   return f + Fraction(i);
}

Fraction
operator+(int i, const Fraction& f)
{
   return Fraction(i) + f;
}

Fraction
Fraction::operator-(const Fraction& f) const
{
   Fraction ft = *this;
   ft -= f;
   return ft;
}

Fraction
operator-(const Fraction& f, int i)
{
   return f - Fraction(i);
}

Fraction
operator-(int i, const Fraction& f)
{
   return Fraction(i) - f;
}

Fraction
Fraction::operator*(const Fraction& f) const
{
   Fraction ft = *this;
   ft *= f;
   return ft;
}

Fraction
operator*(const Fraction& f, int i)
{
   return f * Fraction(i);
}

Fraction
operator*(int i, const Fraction& f)
{
   return Fraction(i) * f;
}

Fraction
Fraction::operator/(const Fraction& f) const
{
   Fraction ft = *this;
   ft /= f;
   return ft;
}

Fraction
operator/(const Fraction& f, int i)
{
   return f / Fraction(i);
}

Fraction
operator/(int i, const Fraction& f)
{
   return Fraction(i) / f;
}

const Fraction&
Fraction::operator+=(const Fraction& f)
{
   numerator = numerator * f.denominator + denominator * f.numerator;
   denominator = denominator * f.denominator;
   Reduce();
   return *this;
}

const Fraction&
Fraction::operator-=(const Fraction& f)
{
   numerator = numerator * f.denominator - denominator * f.numerator;
   denominator = denominator * f.denominator;
   Reduce();
   return *this;
}

const Fraction&
Fraction::operator*=(const Fraction& f)
{
   numerator = numerator * f.numerator;
   denominator = denominator * f.denominator;
   Reduce();
   return *this;
}

const Fraction&
Fraction::operator/=(const Fraction& f)
{
   numerator = numerator * f.denominator;
   denominator = denominator * f.numerator;
   if (denominator < 0) {
      denominator = -denominator;
      numerator = -numerator;
   }
   Reduce();
   return *this;
}

const Fraction&
Fraction::operator--()
{
   numerator -= denominator;
   return *this;
}

Fraction
Fraction::operator--(int)
{
   Fraction f = *this;
   numerator -= denominator;
   return f;
}

const Fraction&
Fraction::operator++()
{
   numerator += denominator;
   return *this;
}

Fraction
Fraction::operator++(int)
{
   Fraction f = *this;
   numerator += denominator;
   return f;
}

int
Fraction::GCD(int x, int y)
{
   while (x != y) {
      if (x > y)
         x -= y;
      else
         y -= x;
   }
   return x;
}

void
Fraction::Reduce()
{
   if (denominator <= 1)
      return;

   if (numerator == 0) {
      denominator = 1;
      return;
   }
   
   int gcd = GCD(abs(numerator), denominator);

   if (gcd > 1) {
      numerator /= gcd;
      denominator /= gcd;
   }
}
