// Your implementation for the database class goes here.

/*
* Writes records to stream in insertion order.
* Required Record class to have << implemented.
* Complexity: O(n) regardless of scope, comparison still made on all records with scope SelectedRecords
*/

template <class value>
void Database<value>::write(ostream& out, DBScope scope) const {

  //Iterate over records, printing them out in definition order
  //Print either selected records or all records based on scope
  for (auto it = this->records.begin(); it != this->records.end(); ++it) {
    if (scope == AllRecords || (scope == SelectedRecords && it->isSelected())) {
      out << *it << endl;
    }
  }
}

/*
* Read values from input stream and detect and store all valid records.
* Complexity: O(n)
*/
template <class value>
void Database<value>::read(istream& in) {

  Record<value> r;

  //Read records from stream until stream is exhausted
  //Our >> operator on Records ensures each read will read 1 record unless of course eof is reached
  while (in >> r) {
      records.push_back(r);
  }

}

/*
*
*/
template <class value>
void Database<value>::deleteRecords(DBScope scope) {

}

/*
* Select all records.
* Complexity: O(n) - Iterate through all records, and setSelected on every record
*/
template <class value>
void Database<value>::selectAll() {
  for (auto it = this->records.begin(); it != this->records.end(); ++it) {
    it->setSelected(true);
  }

  //Set numSelected_ to correct value
  numSelected_ = this->records.size();
}

/*
* Deselect all records.
* Complexity: O(n) - Iterate through all records, and setSelected on every record
*/
template <class value>
void Database<value>::deselectAll() {
  for (auto it = this->records.begin(); it != this->records.end(); ++it) {
    it->setSelected(false);
  }

  //Set numSelected_ to correct value
  numSelected_ = 0;
}

/*
*
*/
template <class value>
void Database<value>::select(DBSelectOperation selOp, const string& attr, DBQueryOperator op, const value& val) {

}