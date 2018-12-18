#include <bits/stdc++.h>
#include <windows.h>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <map>
#include <iostream>
#include <fstream>
#include "FunctionPrototypes.h"
#include "Deffinitions.h"

using namespace std;

/*
    Append the Log file and close it afterward to ensure
        that the log takes
*/
int Log(string logs) {
    // Check if C:\\Temp exists
    if (CreateDirectory(TEMP_FOLDER.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {
        // Check if C:\\Temp\\Silent Exists
        if (CreateDirectory(OUTPUT_FOLDER.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {
            // Output to file C:\\Temp\\Silent\\InstallLogs.log
            ofstream logfile;
            string file = OUTPUT_FOLDER;
            file += "\\";
            file += OUTPUT_FILE.c_str();

            // Open the file for appending
            logfile.open(file, ios_base::app);
            logfile << logs << endl;
            logfile.close();
        } else {
            return CREATE_DIR_ERROR;
        }
    } else {
        return CREATE_DIR_ERROR;
    }
    
    return ALL_IS_WELL;
}

/*
    Use the path and the filename in order to determine the file's existance
*/
bool FileExists(const char * filename, const char * path) {
    Log("Checking existance of file: " + (string) filename);
    Log("In the directory: " + (string) path);

    // Make the full file path into a string
    string fullfile;
    fullfile = path + (string) "\\" + (string) filename;
    fstream file(fullfile);

    // Check whether the file can be accessed
    string goodfile = file.good() ? "true" : "false";
    Log("File good: " + goodfile);
    return file.good();
}

/*
    Execute the Execute.bat File with the appropriate command line arguments
*/
void ExecuteCMD(const char * filename, const char * path, const char * action) {
    // Prepare the arguments (combine path, and filename into one variable)
    Log("Preparing Command Line Arguments For Execute.bat");
    string args = "\"";
    args += path;
    args += "\" \"";
    args += filename;
    args += "\" ";
    args += action;

    //Preparing for execution
    Log("Preparing the Shell Command");
    SHELLEXECUTEINFO ShExecInfo = {0};
    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask  = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.hwnd   = NULL;
    ShExecInfo.lpVerb = NULL;
    ShExecInfo.lpFile = "Execute.bat";
    ShExecInfo.lpParameters = args.c_str();
    ShExecInfo.lpDirectory  = NULL;
    ShExecInfo.nShow        = SW_HIDE;
    ShExecInfo.hInstApp     = NULL;

    // Execute the command
    Log("Executing Execute.bat with arguments: " + (string) args);
    ShellExecuteEx(&ShExecInfo);

    // Wait for Termination of program
    Log("Waiting For Program to Terminate! :)");
    WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
    CloseHandle(ShExecInfo.hProcess);

    // ShellExecute(NULL, "open", "Execute.bat", args.c_str(), NULL, SW_HIDE);
}

/*
    Display Help file
*/
void DisplayHelp() {
    // Open the log file
    Log("Opening HelpFile.txt");
    ifstream fin("HelpFile.txt");

    // Display help file line by line
    Log("Outputting HelpFile.txt to Command Line line by line");
    string help;
    while(getline(fin, help)) {
        cout << help << endl;
    }
}

/*
    Collect Command Line arguments and store them in a map
*/
int CollectArgs(map<string, char *> &CommandArgs, int &argc, char ** argv) {
    // Loop through the arguments looking for key words
    Log("Collecting Arguments from command line and storing them");
    for (int i = 1; i < argc; i += 2) {

        // Take the key-word and convert it to a lowercase string
        string arg = argv[i];
        transform(arg.begin(), arg.end(), arg.begin(), ::tolower);

        // Hanlde the file flag
        if (arg == "-file" || arg == "-f") {
            Log("File: " + (string) argv[i + 1]);
            CommandArgs["file"] = argv[i + 1];
        } 
        
        // Handle the path flag
        else if (arg == "-path" || arg == "-p") {
            Log("Path: " + (string) argv[i + 1]);
            CommandArgs["path"] = argv[i + 1];
        } 
        
        // Handle the help flag
        else if (arg == "--help" || arg == "-help" || 
                    arg == "-h" || arg == "/?" ||  arg == "/h") {
            Log("Help flag found");
            return HELP_REQUEST;
        } 

        // Handle the uninstall flag
        else if (arg == "-uninstall" || arg == "-u") {
            Log("Uninstall Flag Detected");
            CommandArgs["action"] = (char *) "/x";
        } 

        // Handle the install flag
        else if (arg == "-install" || arg == "-i") {
            Log("Install Flag Detected");
            CommandArgs["action"] = (char *) "/i";
        } 
    }

    // Default the action to install if not flag was set
    Log("Default the action to install if none was specified");
    if (!CommandArgs.count("action")) {
        CommandArgs["action"] = (char *) "/i";
    }
    
    // Make sure there is a path, or default it
    Log("Ensuring there is a path to the file specified");
    if (!CommandArgs.count("path") && !CommandArgs.count("help")) {
        CommandArgs["path"] = (char *) ".";
    }

    // Make sure the path has good format
    Log("Ensuring correctness of path format");
    if (CommandArgs["path"][((string) CommandArgs["path"]).length() - 1] == '\\') {
        CommandArgs["path"][((string) CommandArgs["path"]).length() - 1] = '\0';
    }

    // Make sure a file is specified, or default it to (SilentInstall.bat or SilentInstall.ps1)
    Log("Ensuring there is a program to execute");
    if (CommandArgs.count("file")) {
        // If file is not found return error code: FILE_NOT_ACCESSABLE
        if (!FileExists(CommandArgs["file"], CommandArgs["path"])) {
            return FILE_NOT_ACCESSABLE;
        }
    }

    // If there was no file specified, then search for a default file
    Log("If file was not specified search for a default file");
    if (!CommandArgs.count("file") && !CommandArgs.count("help")) {
        // Select a default filename based on the action
        Log("Searching for a default file based on action");
        if (((string) CommandArgs["action"]) == "/i") {
            // Search for a ps1 file
            if (FileExists("SilentInstall.ps1", CommandArgs["path"])) {
                CommandArgs["file"] = (char *) "SilentInstall.ps1";
            } 

            // Search for a bat file
            else if (FileExists("SilentInstall.bat", CommandArgs["path"])) {
                CommandArgs["file"] = (char *) "SilentInstall.bat";
            } 
            
            // If no file is found return error code: FILE_NOT_SPECIFIED
            else {
                return FILE_NOT_SPECIFIED;
            }
        } else {
            // Search for a ps1 file
            if (FileExists("SilentUninstall.ps1", CommandArgs["path"])) {
                CommandArgs["file"] = (char *) "SilentUninstall.ps1";
            } 

            // Search for a bat file
            else if (FileExists("SilentUninstall.bat", CommandArgs["path"])) {
                CommandArgs["file"] = (char *) "SilentUninstall.bat";
            } 
            
            // If no file is found return error code: FILE_NOT_SPECIFIED
            else {
                return FILE_NOT_SPECIFIED;
            }
        }
    }

    return ALL_IS_WELL;
}