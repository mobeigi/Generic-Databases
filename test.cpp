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

  //db.selectAll();
  (*db.records.begin()).setSelected(true);
  db.deleteRecords(SelectedRecords);

  db.write(std::cout, AllRecords);

  //stuff
  cin.ignore();
  cin.get();

  return 0;
}