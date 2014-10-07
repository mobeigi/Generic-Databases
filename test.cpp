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


  db.select(Add, "favorite sport", Equal, "question answering");
  db.select(Remove, "name", Equal, "Jingling Xue");
  db.select(Refine, "favorite sport", Equal, "question answering");

  db.select(Refine, "e-mail", Equal, "gabd@cse.unsw.edu.au");

  db.write(std::cout, SelectedRecords);

  //stuff
  cin.ignore();
  cin.get();

  return 0;
}