/* interactive.cpp
 * --------------
 * A primitive database shell that can serve as an interactive testbed.
 * Rather than give you a file of C++ code that exercises the database
 * functionality, I thought I'd write something a little more general.
 * This interactive client allows the user to perform operations like 
 * read, write, select, and delete on a database interactively and 
 * immediately see the results. This means you can try out lots of
 * queries on your database in a fairly easy fashion.  The "language"
 * for the shell is a pretty simplistic variant-- think of it as
 * mini-micro-SQL.  It has some built-in help, so you should be
 * able to learn from trying things and asking for help. You shouldn't
 * change this code at all, and you don't even really need to understand
 * most of it, you only need to be concerned that you implement the
 * database routines correctly so that this client can use them.
 * 
 * This code requires that the database value type that has a reasonable
 * overloaded version of >>, since the user will need to implement values
 * for criteria at cin and this code relies on >> working for the value.
 * Also, all database value types must have versions of the binary
 * operators =, !=, <, and >, as well as the output << operator.
 */ 
 
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <cstdlib>	// for exit()
#include <cstring>	// for strncmp()
#include <string>
using namespace std;

#include "fraction.h"
#include "utility.h"
#include "record.h"
#include "database.h"

/* 
 * const maxline
 * -------------
 * Used in various places to as buffer limit. If you're going to have some
 * arbitrary limit, you should at the least way-overestimate, especially
 * since all these buffers come from stack where space is cheap.
 */

enum CommandT {Help, Read, Print, Select, Delete, Write, Quit, NumOptions};
static CommandT GetCommandFromUser();
template <typename value> void GetCriteriaValue(value& val);
void GetCriteriaValue(string& val);
template <typename value> bool DispatchCommand(CommandT cmd, Database<value>& db);
template <typename value> bool ReadCommand(Database<value>& db);
template <typename value> bool WriteCommand(Database<value>& db);
template <typename value> bool PrintCommand(Database<value>& db);
template <typename value> bool SelectCommand(Database<value>& db);
template <typename value> bool DeleteCommand(Database<value>& db);
template <typename value> bool SelectWithCriteria(DBSelectOperation type, Database<value>& db);
static bool HelpCommand();
static bool QuitCommand();
static void PrintHelpFile(const string& filename);
static void PrintMenuOptions();
static void InitCommandLine();
static string GetNextToken(bool singleWord = true);

/* 
 * MainLoop
 * --------
 * The top-level loop that just asks user to input a new command
 * and then dispatches that command appropriately.  The loop appears
 * to be infinite, but the "quit" command just directly uses exit()
 * to terminate when the user is done.
 */
 
template <typename value> void MainLoop(Database<value>& db)
{ 
  InitCommandLine();
  while(true) {
    if (DispatchCommand(GetCommandFromUser(), db))
      cout << "\n" << db.numRecords() << " records (" << db.numSelected() << " selected)\n";
    else 
      cout << "\n";
  }
}

/* 
 * main
 * ----
 * The interactive code is templatized to 
 * work with any Database value type, this main 
 * just lets you pick from one of 3 types to test on.
 */

int main()
{
  PrintHelpFile("help_interactive");
  cout << "What type values would you like to test in the database?\n";
  cout << "(1 = integer, 2 = string, 3 = Fraction): ";
  int choice;
  cin >> choice;
  switch (choice) {
  case 1: { 
    Database<int> db;
    MainLoop(db);
    break; 
  }
  case 2: { 
    Database<string> db;
    MainLoop(db);
    break; 
  }
  case 3: { 
    Database<Fraction> db;
    MainLoop(db);
    break; 
  }
  default:
    cout << "Invalid choice, exiting.\n";
  }
  return 0;
}

/* DispatchCommand
 * ---------------
 * The most straightforward of command dispatch routines.
 */
 
template <typename value> bool DispatchCommand(CommandT command, Database<value>& db)
{
  switch(command) {
  case Read:   return ReadCommand(db); 
  case Write:  return WriteCommand(db);
  case Print:  return PrintCommand(db);
  case Select: return SelectCommand(db); 
  case Delete: return DeleteCommand(db);
  case Help:   return HelpCommand(); 
  case Quit:   return QuitCommand();
  default: assert(0); return false;
  }
}

/* 
 * Command table
 * -------------
 * Lists the command enumerations, the command string expected from
 * user and the short description printed for the Help command.  Ideally
 * you would add a function pointer for the dispatch routine to handle
 * each command, but given the fns take a templatized argument that's just a
 * bit too much for the compiler to cope with.  Doing this as a table is 
 * a nice way to track all the commands together and make it fairly
 * straightforward to add new commands.
 */

static struct { 
  CommandT choice; 
  const char *name; 
  const char *helpString; 
} menu[] = {
  { Help, "help", 
      "Print this table of the command descriptions."},
    { Read, "read", 
   	"Read database in from file (replaces current db). Requires filename arg."},
      { Print, "print", 
	  "Print selected records. Can add arg \"all\" to print all records."},
	{ Select, "select", 
	    "Defines and changes selection. Try select with no args for more help."},
	  { Delete, "delete", 
	      "Delete selected records. Can add arg \"all\" to delete all records."},
	    { Write, "write", 
		"Write current database to a file. Requires filename arg."},
	      { Quit, "quit", 
		  "Quit the program."},
		{}
};

/* HelpCommand
 * -----------
 * When help is chosen.  Prints the contents of the cmd table,
 * which has a short description of each of the commands.
 */

static bool HelpCommand()
{
  cout << "\nCmd \tWhat it does\n";
  cout << "------------------------------------------\n";
  for (int i = 0; i < NumOptions; i++)
    cout << menu[i].name << '\t' << menu[i].helpString << '\n';
  cout << "NOTE: You can abbreviate if the string is unique.\n";
  return true;
}

/* QuitCommand
 * -----------
 * When quit is chosen.  Just exits the program.
 */

static bool QuitCommand()
{
  cout << "Thanks for visiting!\n";
  exit(0);
  return true;  // doesn't get here, but compiler doesn't know that
}

/* 
 * PrintCommand
 * ------------
 * When print is chosen.  If the option argument 
 * "all" is specified, prints all the records in the 
 * database, otherwise just prints the records in the 
 * current selection.
 */

template <typename value> bool PrintCommand(Database<value>& db)
{
  string arg = GetNextToken();
  bool doAll = arg != "" && strncmp(arg.c_str(), "all", arg.length()) == 0;
  db.write(cout, doAll? AllRecords: SelectedRecords);
  return true;
}

/* DeleteCommand
 * -------------
 * When delete is chosen. If the option argument "all" is specified,
 * deletes all the records in the database, otherwise just deletes
 * the records in the current selection.
 */

template <typename value> bool DeleteCommand(Database<value>& db)
{
  string arg = GetNextToken();
  bool doAll = arg != "" && strncmp(arg.c_str(), "all", arg.length()) == 0;
  db.deleteRecords(doAll? AllRecords: SelectedRecords);
  return true;
}

/* ReadCommand
 * -----------
 * When read is chosen.  The next argument must specify the filename
 * to read from. The database contents are wiped out and replaced by
 * the new records read from the named file.  The database is unchanged
 * if no filename argument was given or the named file could not be
 * opened.
 */

template <typename value> bool ReadCommand(Database<value>& db)
{
  string filename = GetNextToken();
  if (filename == "") {
    cout << "ERROR: Read requires an argument of file to read from.\n";
    return false;
  }
  
  ifstream in(filename.c_str());
  if (!in) {
    cout << "ERROR: Cannot open file named \"" << filename << "\".\n";
    return false;
  }
  
  db.read(in);
  cout << "Read " << db.numRecords() << " records from \""<< filename <<"\".\n";
  return true;
}

/* 
 * WriteCommand
 * ------------
 * When write is chosen.  The next argument must specify the filename
 * to write. If there are any selected records, the selected records
 * are written to the named file. Otherwise, the entire database 
 * contents are written to the named file.  
 * The database itself is unchanged. If no filename argument was given 
 * or the named file could not be opened, the write is aborted.
 */

template <typename value> bool WriteCommand(Database<value>& db)
{
  string filename = GetNextToken();
  if (filename == "") {
    cout << "ERROR: Write requires an argument of filename to write to.\n";
    return false;
  }
  
  ofstream out(filename.c_str());
  if (!out) {
    cout << "ERROR: Cannot open file named \"" << filename << "\".\n";
    return false;
  }
  
  bool doAll = !db.numSelected();
  db.write(out, doAll? AllRecords : SelectedRecords);
  cout << "Wrote " << (doAll ? db.numRecords() : db.numSelected()) << " records to \""<< filename <<"\".\n";
  return true;
}

static struct { 
  DBSelectOperation type; 
  const char *name; 
  int minChars;
} selectCmds[] = {
  { All,    "all",    2},// min chars required to distinguish from others
    { Clear,  "clear",  1},
      { Add,    "add",    2},
	{ Remove, "remove", 3},
	  { Refine, "refine", 3},
	    {}
};

/* SelectCommand
 * -------------
 * When select is chosen.  Select requires arguments, if known are
 * given, the help file is printed.  The first argument identifies
 * the operation (all, clear, add, remove, refine) and if one of
 * the last three we hand over to the criteria processing function.
 * If the args are ill-formed, we report an error and leave selection
 * unchanged.
 */

template <typename value> bool SelectCommand(Database<value>& db)
{
  string arg = GetNextToken();
  DBSelectOperation type = (DBSelectOperation)-1;
  
  if (arg == "") { PrintHelpFile("help_select"); return false;};
  
  for (int i = 0; selectCmds[i].name != NULL ; i++) {
    if ((int(arg.length()) >= selectCmds[i].minChars) && 
	(strncmp(arg.c_str(), selectCmds[i].name, int(arg.length())) == 0)) {
      type = selectCmds[i].type;
      break;
    }
  }
  
  switch (type) {
  case All:	
    db.selectAll(); return true;
  case Clear: 	
    db.deselectAll(); return true;
  case Add: case Remove: case Refine:
    return SelectWithCriteria(type, db);
  default:
    cout << "ERROR: Invalid arguments to select command.\n";
  }
  
  return false;
}

static struct {
  DBQueryOperator op; 
  const char *name;
} queryOps[] = {
  { Equal, "=" },
    { NotEqual,  "!="},
      { LessThan, "<"},
	{ GreaterThan, ">"},
	  {}
};

static DBQueryOperator isQuery(const string &arg)
{
  for (int i = 0; queryOps[i].name != NULL; i++) {
    if (arg == queryOps[i].name)
      return queryOps[i].op;
  }
  
  return (DBQueryOperator)-1;
}

template <typename value> bool SelectWithCriteria(DBSelectOperation type, Database<value>& db)
{
  string arg = GetNextToken();
  DBQueryOperator op = (DBQueryOperator)-1;
  
  if (arg == "") { PrintHelpFile("help_criteria"); return false;};
  
  string fieldname = arg;
  while (true) {
    string arg = GetNextToken();
    if (arg == "") { // got to end of line without query op
      cout << "ERROR: Invalid criteria given to select command.\n"; 
      return false;
    };
    
    op = isQuery(arg);	 // check if query operator
    if (op != -1) break;
    fieldname += " " + arg;  // otherwise concatenate onto fieldname
  }

  TrimString(fieldname);
  value val;
  GetCriteriaValue(val);
  db.select(type, fieldname, op, val);
  return true;
}

/* PrintHelpFile
 * -------------
 * Just opens a text file and echos its contents to the terminal.  It's 
 * a bit easier to maintain and edit instructions for the user if they're not 
 * embedded in your code as a bunch of cout/printf statements.  In this
 * case, I supplied the help for the more complicated select and criteria
 * commands in separate files.
 */

static void PrintHelpFile(const string& filename)
{
  ifstream in(filename.c_str());
  
  assert(in);
  while (in && in.peek() != EOF) {
    string line;
    getline(in, line);
    cout << '\n' << line;		// echo line to terminal
  }
}

/* PrintMenuOptions
 * ----------------
 * Used to print the list of available menu choices. Prints using
 * names from command table, so any changes there are automatically
 * reflected here.
 */

static void PrintMenuOptions()
{
  cout << "Commands are [";
  for (int i = 0; i < NumOptions; i++) {
    cout << menu[i].name;
    if (i != NumOptions-1) cout << ", ";
  }
  
  cout << "].\n";
}

/* istrstream static variable
 * --------------------------
 * When the user enters the commandline, we construct a istrstream to
 * scan it.  Then we can repeatedly call GetNextToken or GetCriteriaValue
 * to retrieve the next thing off the command line.
 */

static istringstream *istr = NULL;	// keep stream around between calls

/* 
 * InitCommandLine
 * ---------------
 * Called once for each command entered.  We use a static buffer to
 * record the entire line, then start up a new strstream to scan
 * through it.  Pulling it in one line at a time avoids weird
 * interactions between one command and the next, especially when
 * something went wrong parsing the earlier line.
 */

static void InitCommandLine()
{
  static string commandline;  // static since str uses it
  getline(cin, commandline);  // get line from user
  delete istr;
  istr = new istringstream(commandline.c_str()); // start new stream up
}

/* GetCriteriaValue
 * ----------------
 * Used to pull off the value for the selection criteria.  Since the value
 * can be of any type, we rely on the overloaded >> operator to pull the
 * right thing from the command line into value.
 */

template <typename value>
void GetCriteriaValue(value& val)
{
  *istr >> val;   // pull off next "whatever" from command line
}

/* GetCriteriaValue
 * ----------------
 * A specialization of GetCriteriaValue for strings that doesn't stop
 * at spaces, but instead concatenates all remaining words into one
 * string for the value.
 */

void GetCriteriaValue(string& val)
{
  val = GetNextToken(false);
}

/* GetNextToken
 * ------------
 * Reads the next string token from the command line strstream.
 * If the singleWord flag is true, we use the ordinary string>>
 * operator to pull off characters to the next space, otherwise,
 * we grab everything to the end of the line using the string
 * read method.
 */

static string GetNextToken(bool singleWord)
{
  string result;
  
  if (singleWord)
    *istr >> result;   // get next white-space-delimited from command line
  else {
    getline(*istr, result, '\n');
    TrimString(result);
  }
  return result;
}

/* GetCommandFromUser
 * ------------------
 * Prompts user to enter a command and uses the first token from
 * line entered to  determine which command was requested.  We compare 
 * the string to the command table entries to find a matching one, 
 * else they have to enter again.
 */

static CommandT GetCommandFromUser()
{    
  while (true) {
    PrintMenuOptions();
    cout << "Enter command: ";
    InitCommandLine();	// read in next line and start scanning it
    string command = GetNextToken();
    
    for (int i = 0; i < NumOptions; i++) {	// look up in cmd table
      if (strncmp(command.c_str(), menu[i].name, int(command.length())) == 0)
	return menu[i].choice;	// found a match
    }
    cout <<"ERROR: \"" << command <<"\" is not a valid option.\n\n";
  }
}
