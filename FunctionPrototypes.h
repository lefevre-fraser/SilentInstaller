#include <map>
#include <string>
#include <fstream>

#ifndef FUN_PROTO
#define FUN_PROTO

// function prototypes

// logs data to file from this executable
int Log(std::string log);

// checks the existance of a file
bool FileExists(const char * filename);

// checks the existance of a folder
bool FolderExists(const char * filename);

// executes a command in cmd
void ExecuteCMD(const char * filename, const char * path, const char * action);

// disaplays the help file
void DisplayHelp();

// collects arguments from the command line into a map
int CollectArgs(std::map<std::string, char *> &CommandArgs, int &argc, char ** argv);

#endif // FUN_PROTO