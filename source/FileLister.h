#include <stdio.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>

namespace fs = std::filesystem;
using namespace std;

#define FL_MODS_PATH "sdmc:/switch/Iridium/Mods/"

vector<string> GenerateListFile(string modname) {
    string path = FL_MODS_PATH + modname;
    int MODS_PATH_LENGTH = path.length() + 1;

    //make an array of all paths to files in the mod folder
    vector<string> modFiles = {};

    //walk through the mod folder and add all files to the array
    for (const auto & entry : fs::recursive_directory_iterator(path)) {
        if (fs::is_regular_file(entry.path())) {
            //remove MODS_PATH from the path
            string modFile = entry.path().string().substr(MODS_PATH_LENGTH, entry.path().string().length() - MODS_PATH_LENGTH + 1);
            //add the file to the array if it endswith xnb
            if (modFile.substr(modFile.length() - 4, 4) == ".xnb") {
                modFiles.push_back(modFile);
            }
        }
    }

    return modFiles;
}

vector<string> WriteListFile(string modname) {
    string path = FL_MODS_PATH + modname;
    vector<string> modFiles = GenerateListFile(modname);
    
    ofstream file(path + "/filelist.txt");
    for (int i = 0; i < modFiles.size(); i++) {
        file << modFiles[i] << "\n";
    }
    //file << "TEST";
    file.close();
    return modFiles;
}

vector<string> ReadListFile(string modname) {
    string path = FL_MODS_PATH + modname;

    //read the filelist.txt file
    ifstream file;
    file.open(path + "/filelist.txt");
    vector<string> mods = {};

    string line;

    while (getline(file, line)) {
        mods.push_back(line);
    }

    

    file.close();
    return mods;
}