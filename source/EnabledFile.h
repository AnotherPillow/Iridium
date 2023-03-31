#include <stdio.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>

namespace fs = std::filesystem;
using namespace std;

#define FL_MODS_PATH "sdmc:/switch/Iridium/Mods/"

bool ModExists(string modName) {
    string modPath = FL_MODS_PATH + modName;
    if (!fs::exists(modPath)) 
        return false;
    else
        return true;
}

bool CheckEnabled(string modName) {
    //check mod exists
    string modPath = FL_MODS_PATH + modName;
    if (!ModExists(modName)) 
        return false;

    //check enabled file exists
    string enabledPath = modPath + "/enabled";

    //if (!fs::exists(enabledPath)) 
    //    return false;
    
    //check enabled file is true
    ifstream enabledFile(enabledPath);
    string enabled;
    getline(enabledFile, enabled);
    enabledFile.close();

    if (enabled == "1") 
        return true;
    else 
        return false;    
}

void EnableModFile(string modName) {
    //check mod exists
    string modPath = FL_MODS_PATH + modName;
    if (!ModExists(modName))
        return;

    //check enabled file exists
    string enabledPath = modPath + "/enabled";
    
    //check enabled file is true
    ofstream enabledFile(enabledPath);
    enabledFile << "1";
    enabledFile.close();
}

void DisableModFile(string modName) {
    //check mod exists
    string modPath = FL_MODS_PATH + modName;
    if (!ModExists(modName))
        return;

    //check enabled file exists
    string enabledPath = modPath + "/enabled";

    //if (!fs::exists(enabledPath)) 
    //    return;
    
    //check enabled file is true
    ofstream enabledFile(enabledPath);
    enabledFile << "0";
    enabledFile.close();
}