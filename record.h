#ifndef RECORD_H
#define RECORD_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "utility.h"

/* Database enums
* --------------
* Ideally, these enums would be declared inside the scope of
* the Database class, but it turns out gcc doesn't not handle
* nested enums inside a template class very well, so you will
* want to define that at the top-level scope as a workaround.
*/

enum DBSelectOperation { All, Clear, Add, Remove, Refine };
enum DBScope { AllRecords, SelectedRecords };
enum DBQueryOperator { Equal, NotEqual, LessThan, GreaterThan };


// Need to add declarations for operator<< and operator >> here
template <class value> class Record;
template <class value> ostream& operator<<(ostream& out, const Record<value>& r);
template <class value> istream& operator>>(istream& in, Record<value>& r);

template <class value>
class Record {

public:
  Record<value>() : selected(false), fields(vector<pair<string, value>>()) {};

  //Member functions
  inline bool isSelected() const { return selected; };
  inline void setSelected(bool val) { selected = val; };

  //Main query matching command
  bool matchesQuery(const string& attr, DBQueryOperator op, const value& want) const;

  //Operator overloads
  friend ostream& operator<<<value>(ostream& out, const Record<value>& r);
  friend istream& operator>><value>(istream& in, Record<value>& r);

  //Default Destructor
  ~Record() {};

//private:
  bool selected;
  vector<pair<string, value>> fields;
};

#include "record.cpp"

#endif
