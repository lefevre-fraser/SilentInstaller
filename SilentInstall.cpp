#include <windows.h>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include "FunctionPrototypes.h"
#include "Deffinitions.h"

using namespace std;

int main(int argc, char ** argv)
{
    // Initialize Log File for this executable.
    int ERROR_CODE;
    ERROR_CODE =  Log("Beginning Process");
    switch (ERROR_CODE) {
        case CREATE_DIR_ERROR:
            cout << "Problem Encountered While Trying to create Directory for Logs";
            break;
        case ALL_IS_WELL:
            break;
    }

    // create a map and populate it with the command line arguments
    map<string, char *> CommandArgs;
    ERROR_CODE = CollectArgs(CommandArgs, argc, argv);
    switch (ERROR_CODE) {
        case FILE_NOT_ACCESSABLE:
            Log("Unable to access the specified file in the specified path");
            break;
        case FILE_NOT_SPECIFIED:
            Log("No file was specified, and the default was not found");
            break;
        case HELP_REQUEST:
            DisplayHelp();
            break;
        case ALL_IS_WELL:
            ExecuteCMD(CommandArgs["file"], CommandArgs["path"], CommandArgs["action"]);
            break;
    }

    Log("Ending the Process\n \n \n \n ");

    return 0;
}