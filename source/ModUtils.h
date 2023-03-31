#include <stdio.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include "FileLister.h"
#include "EnabledFile.h"

namespace fs = std::filesystem;
using namespace std;

#define FL_MODS_PATH "sdmc:/switch/Iridium/Mods/"
#define SDV_MODS_PATH "sdmc:/atmosphere/contents/0100E65002BB8000/romfs/Content/"

vector<string> splitstr(string text, char delim) {
    string line;
    vector<std::string> vec;
    std::stringstream ss(text);
    while(std::getline(ss, line, delim)) {
        vec.push_back(line);
    }
    return vec;
}

void CopyFile(const char* srcPath, const char* dstPath) {
    FILE* srcFile = fopen(srcPath, "rb");
    if (!srcFile) {
        // Handle error opening source file
        return;
    }

    FILE* dstFile = fopen(dstPath, "wb");
    if (!dstFile) {
        // Handle error opening destination file
        fclose(srcFile);
        return;
    }

    char buffer[4096];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), srcFile)) > 0) {
        fwrite(buffer, 1, bytesRead, dstFile);
    }

    fclose(srcFile);
    fclose(dstFile);
}
void EnableMod(string modName) {
    string modPath = FL_MODS_PATH + modName;
    EnableModFile(modName);

    vector<string> modFiles = GenerateListFile(modName);

    //copy files to SDV
    for (int i = 0; i < modFiles.size(); i++) {
        string modFile = modFiles[i];
        string modFilePath = modPath + "/" + modFile;
        string sdvFilePath = SDV_MODS_PATH + modFile;
        //create all necessary directories
        fs::create_directories(fs::path(sdvFilePath).parent_path());
        if (fs::exists(sdvFilePath)) 
            fs::remove(sdvFilePath);
        
        CopyFile(modFilePath.c_str(), sdvFilePath.c_str());

    }

    return;
}

void DisableMod(string modName) {
    string modPath = FL_MODS_PATH + modName;
    DisableModFile(modName);

    vector<string> modFiles = GenerateListFile(modName);

    for (int i = 0; i < modFiles.size(); i++) {
        string modFile = modFiles[i];
        string sdvFilePath = SDV_MODS_PATH + modFile;

        if (fs::exists(sdvFilePath))
            fs::remove(sdvFilePath);
    }

    return;
}

string ErrorMessageTextPrettify(string content) {
    int max = content.length();
    string out = "";
    int line = 0;
    for (int i = 0; i < max; i++) {
        out += content[i];
        if (i % 50 == 0 && i > 10) {
            out += "\n";
            line++;
        }
    }

    return out;
}