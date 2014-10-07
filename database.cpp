// Your implementation for the database class goes here.

/*
*
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
*
*/
template <class value>
void Database<value>::read(istream& in) {

  Record<value> r;

  //Read records from stream until stream is exhausted
  //Our >> operator on Records ensures each read will read 1 record until eof is reached
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
*
*/
template <class value>
void Database<value>::selectAll() {

}

/*
*
*/
template <class value>
void Database<value>::deselectAll() {

}

/*
*
*/
template <class value>
void Database<value>::select(DBSelectOperation selOp, const string& attr, DBQueryOperator op, const value& val) {

}