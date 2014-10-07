#ifndef RECORD_H
#define RECORD_H

#include <iostream>
#include <string>
#include <vector>

#include <unordered_map>
#include <list>

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
  Record<value>() : selected(false), fields(unordered_map<string, vector<value>>()), insertionOrder(list<pair<string, size_t>>()) {};

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
  bool selected;  //used to select/unselect record

  //Record data will be stored in an unordered_map which maps strings to a vector of values
  //This has many advantages over a vector of pairs at the cost of a little extra memory (as insertion order has to be kept in a unordered_set)
  unordered_map<string, vector<value>> fields;
  list<pair<string, size_t>> insertionOrder;
  //vector<pair<string, value>> fields;


  //Private helper functions
  template <class value> void readValue(istream& is, value& val);
  template <> void readValue(istream& is, string& val);

};

#include "record.cpp"

#endif
