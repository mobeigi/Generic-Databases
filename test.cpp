#include <iostream>
#include "record.h"
#include <fstream>

using namespace std;

int main() {

  Record<string> rec;

  //Open test file
  ifstream ifs("O:/Dropbox/Github/Generic-Databases/record.txt");
  string s;

  if (ifs.is_open())
  {
    ifs >> rec;
    ifs.close();
  }
  else cout << "Unable to open file";
  
  //Explore
  cout << rec;

  //stuff
  cin.ignore();
  cin.get();

  return 0;
}