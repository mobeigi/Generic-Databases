#include <iostream>
#include "record.h"
#include "database.h"
#include <fstream>

using namespace std;

int main() {


  //Open test file
  ifstream ifs2("O:/Dropbox/Github/Generic-Databases/record.txt");

  Database<string> db;
  db.read(ifs2);

  cout << db.numRecords() << endl << endl;

  db.write(std::cout, SelectedRecords);

  //stuff
  cin.ignore();
  cin.get();

  return 0;
}