// Your implementation for the record class goes here.
#include <sstream>

/* << operator overload
 * Format of output is opening {, following lines of entries of type: <attribute> = <value> and final closing }
*/
template <class value>
ostream& operator<<(ostream& out, const Record<value>& r)
{
  out << "{" << endl;

  //Iterate over fields
  for (auto it = r.fields.begin(); it != r.fields.end(); ++it) {
    out << "  " << it->first << " = " << it->second << endl;  // (2 spaces) <attribute> = <value>
  }

  out << "}";
  
  return out;
}

/* >> overload
 * 
*/
template <class value>
istream& operator>>(istream& in, Record<value>& r)
{
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

    //Make new pair
    pair<string, value> p = pair<string, value>(attribute, val);

    //Add pair to end of fields
    r.fields.push_back(p);

  }

  return in;
}


template <class value>
bool Record<value>::matchesQuery(const string& attr, DBQueryOperator op, const value& want) const {
 
}


//Helper template functions

//Read in value using defined >> on value
template <class value>
void readValue(istream& is, value& val) {
  is >> val;
}

//Read in string so that entire string is stored and not just upto first whitespace
template <>
void readValue<string>(istream& is, string& val) {
  string input;

  while (getline(is, val)) {
    val += input;
  }

  //Trim final string
  TrimString(val);
}
