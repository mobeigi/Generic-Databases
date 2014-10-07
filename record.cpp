// Your implementation for the record class goes here.
#include <sstream>

/*
 * << operator overload
 * Format of output is opening {, following lines of entries of type: <attribute> = <value> and final closing }
 *
 * Complexity: O(n) to iterate over each field in insertion order
*/
template <class value>
ostream& operator<<(ostream& out, const Record<value>& r)
{
  out << "{" << endl;

  //Iterate over fields using insertionOrder to determine order
  //We will use the fact that insertionorder provides us with the index of the element in the vector
  for (auto iot = r.insertionOrder.begin(); iot != r.insertionOrder.end(); ++iot) {
    out << "  " << iot->first << " = " << r.fields.at(iot->first).at(iot->second) << endl;  // (2 spaces) <attribute> = <value>
  }

  out << "}";
  
  return out;
}

/* >> overload
 * 
 * Allows reading into records from streams.
*/
template <class value>
istream& operator>>(istream& in, Record<value>& r)
{
  //Clear contents of record before reading in new data (ie we overwrite any pre-existing data)
  r.fields.clear();

  string input;
  
  //Get first line manually
  getline(in, input);
  TrimString(input);

  //Check if valid block
  if (input.compare("{") != 0) {
    //Do some error reporting
    return in;
  }

  //If valid block, go through remaining lines reading in entries
  while (getline(in, input)) {
    //Trim the input
    TrimString(input);

    //Break if we reach end of record block
    if (input.compare("}") == 0) {
      break;
    }

    //Get two tokens from string
    size_t equalPos = input.find("=");
    string attribute = input.substr(0, equalPos);
    string valString = input.substr(equalPos + 1, input.length());
    
    //Trim above strings
    TrimString(attribute);
    TrimString(valString);

    //Create istringstream out of valString
    //We require value to have >> defined
    istringstream valStream(valString.c_str());
    value val;

    //Use helper function to read in values with some specialization
    readValue<value>(valStream, val);

    //Note insertion order of this field for ordered printing later on
    if (r.fields[attribute].empty())
      r.insertionOrder.push_back(pair<string, size_t>(attribute, 0));
    else 
      r.insertionOrder.push_back(pair<string, size_t>(attribute, r.fields[attribute].size()));

    //Add our value to our vector
    r.fields[attribute].push_back(val);

  }

  return in;
}


/*
 * Query Matching function for records
 * 
 * Complexity: O(n) if searching all fields, ie attr = "*", O(k) otherwise where k is the number of fields with attribute 'attr'
 * Return: true if there exists value that is 'equivalent' to want under under operation 'op'
*/
template <class value>
bool Record<value>::matchesQuery(const string& attr, DBQueryOperator op, const value& want) const {
  bool fullSearch = false;
  string attribute = attr;

  //Check to see if we need to search entire list
  if (attribute == "*")
    fullSearch = true;
  //otherwise ensure this attribute exists, if not result is always false
  else if (!this->fields.count(attribute)) {
    return false;
  }

  //Iterate over fields using insertionOrder to determine order
  for (auto iot = this->insertionOrder.begin(); iot != this->insertionOrder.end(); ++iot) {

    //If fullsearch, set attribute to ordered list attribute value
    if (fullSearch)
      attribute = iot->first;

    //For each attribute, check all the values in the vector that belong to it (there may be more than 1)
    for (auto vit = this->fields.at(attribute).begin(); vit != this->fields.at(attribute).end(); ++vit) {
      
      //Perform comparison based on provided operator
      switch (op) {
        case Equal:
          if (*vit == want)
            return true;
          break;
        case NotEqual:
          if (*vit != want)
            return true;
          break;
        case LessThan:
          if (*vit < want)
            return true;
          break;
        case GreaterThan:
          if (*vit > want)
            return true;
          break;
      }
    }

    //If not a fullsearch, leave after first loop
    if (!fullSearch)
      break;

  }
  
  //If search finished without returning true, condition was not met
  return false;
}


//Private Helper functions

//Read in value using defined >> on value
template <class value>
void readValue(istream& is, value& val) {
  is >> val;
}

//Read in string so that entire string is stored and not just upto first whitespace
template <>
void readValue(istream& is, string& val) {
  string input;

  while (getline(is, val)) {
    val += input;
  }

  //Trim final string
  TrimString(val);
}
