#include <MainApplication.hpp>
#include <stdio.h>
#include <switch.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>
#include "ModUtils.h"

namespace fs = std::filesystem;

// Implement all the layout/application functions here

#define MODS_PATH "sdmc:/switch/Iridium/Mods"

CustomLayout::CustomLayout() : Layout::Layout() {
    try {
        pu::ui::Color white = { 255, 255, 255, 255 };
    pu::ui::Color halftransparent_black = { 255, 255, 255, 128 };
    pu::ui::Color lightgrey = { 200, 200, 200, 255 };
    pu::ui::Color darkgrey = { 100, 100, 100, 255 };
    int switch_screen_width = 1280;


    // Create the TextBlock instance with the text we want
    //this->helloText = pu::ui::elm::TextBlock::New(200, 100, "Press X to answer my question");

    //this->Add(this->helloText);

    //read files in sdmc:/switch/Iridium/Mods via fs and add them to a vector
    vector<string> modFNameList = {};
    //vector<string> listFile = {};
    //ofstream listFile("sdmc:/switch/Iridium/Mods/list.txt");
    
    for (const auto & entry : fs::directory_iterator(MODS_PATH)) {
        //listFile << entry.path().filename().string() << "<- entry\n";
        if (fs::is_directory(entry.path())) {
            //oop through the files in the directory and check if it has a a folder, if it does, exit the loop
            bool hasFolder = false;
            for (const auto & entry2 : fs::directory_iterator(entry.path())) {
                //listFile << entry.path().filename().string() << "<- entry2\n";


                if (fs::is_directory(entry2.path())) {
                    modFNameList.push_back(entry.path().filename().string());
                    hasFolder = true;
                    //run WriteListFile
                    //listFile = WriteListFile(entry.path().filename().string());
                    break;
                }
            }
            if (!hasFolder) {
                modFNameList.push_back(entry.path().filename().string() + " (invalid) (try restart Iridium)");
            }
            
        }
    }
    //listFile.close();



    //alert the user of their mod list
    if (modFNameList.size() > 0) {
        //convert mods to a string, then alert the user of their mod list
        std::string mods = "";
        //conbvert modFNameList.size() to an int
        int max = modFNameList.size();
        for (int i = 0; i < max; i++) {
            mods += modFNameList[i] + "\n";
            this->Add(pu::ui::elm::TextBlock::New(200, 150 + (i * 50), modFNameList[i]));
            
            string buttonStatus = "";
            if (CheckEnabled(modFNameList[i])) 
                buttonStatus = "On";
            
            else 
                buttonStatus = "Off";
            

            pu::ui::elm::Button::Ref button = pu::ui::elm::Button::New(switch_screen_width - 200,
                140 + (i * 50), 50, 50, buttonStatus, darkgrey, lightgrey);

            //set an event handler for the button (when it is pressed)
            //given this code: button->SetOnClick([this, i]() {
            //how would I capture the button variable?
            vector<string> listFile = WriteListFile(modFNameList[i]);
            button->SetOnClick([this, i, button, modFNameList, listFile]() {
                //toggle the button
                if (button->GetContent() == "On") {
                    try {
                        DisableMod(modFNameList[i]);
                        button->SetContent("Off");
                    } catch (const std::exception& e) {
                        //set button coords to 50,50
                        button->SetX(550);
                        button->SetY(150);
                    
                        //set the button's content to the e.what
                        //but only limit the e.what to 50 characters/line
                        string content = e.what();
                        string out = ErrorMessageTextPrettify(content);
                        button->SetContent(out);
                    }
                }
                else {
                    try {
                        EnableMod(modFNameList[i]);
                        button->SetContent("On");
                    } catch (const std::exception& e) {
                        //set button coords to 50,50
                        button->SetX(550);
                        button->SetY(150);
                    
                        //set the button's content to the e.what
                        //but only limit the e.what to 50 characters/line
                        string content = e.what();
                        string out = ErrorMessageTextPrettify(content);
                        button->SetContent(out);
                    }
                    
                }
            });

            
            this->Add(button);
            //Toggle::New(200 + (modFNameList[i].length() * 15), 150 + (i * 50), "On", false, lightgrey);
            
        }
        

    }
    else {
        //alert the user that they have no mods
        this->Add(pu::ui::elm::TextBlock::New(200, 170, "You have no mods!"));
        this->Add(pu::ui::elm::TextBlock::New(200, 300, "Place mods in sdmc:/switch/Iridium/Mods"));
    }
    } catch (const std::exception& e) {
        //write error to sdmc:/switch/Iridium/error.txt
        std::ofstream errorFile;
        errorFile.open("sdmc:/switch/Iridium/error.txt");
        errorFile << e.what();
        errorFile.close();

    }




    
    
    // Add the instance to the layout. IMPORTANT! this MUST be done for them to be used, having them as members is not enough (just a simple way to keep them)
}

void MainApplication::OnLoad() {
    // Create the layout (calling the smart constructor above)
    this->layout = CustomLayout::New();

    // Load the layout. In applications layouts are loaded, not added into a container (you don't select an added layout, just load it from this function)
    // Simply explained: loading layout = the application will render that layout in the very next frame
    this->LoadLayout(this->layout);

    fs::create_directories(MODS_PATH);

    //read files in sdmc:/switch/Iridium/Mods
    




    // Set a function when input is caught. This input handling will be the first one to be handled (before Layout or any Elements)
    // Using a lambda function here to simplify things
    // You can use member functions via std::bind() C++ wrapper
    /*
    //Basic example stuff
    this->SetOnInput([&](const u64 keys_down, const u64 keys_up, const u64 keys_held, const pu::ui::TouchPoint touch_pos) {
         // If X is pressed, start with our dialog questions!
        if(keys_down & HidNpadButton_X) {
            int opt = this->CreateShowDialog("Question", "Do you like apples?", { "Yes!", "No...", "Cancel" }, true); // (using latest option as cancel option)
            // -1 and -2 are similar, but if the user cancels manually -1 is set, other types or cancel should be -2.
            if((opt == -1) || (opt == -2))  {
                this->CreateShowDialog("Cancel", "Last question was canceled.", { "Ok" }, true); // If we will ignore the option, it doesn't matter if this is true or false
            }
            else {
                // Otherwise, opt will be the index of the options we passed to the dialog
                switch(opt) {
                    // "Yes!" was selected
                    case 0:  {
                        this->CreateShowDialog("Answer", "Really? I like apples too!", { "Ok" }, true); // Same here ^
                        break;
                    }
                    // "No..." was selected
                    case 1: {
                        this->CreateShowDialog("Answer", "Oh, bad news then...", { "Ok" }, true); // And here ^
                        break;
                    }
                }
            }
        } 
        else if (keys_down & HidNpadButton_Plus) {
            this->Close();
        }
        else if  (keys_down) {
            // If any other key is pressed, show a dialog with the key pressed
            this->CreateShowDialog("Key pressed", "You pressed " + std::to_string(keys_down), { "Ok" }, true);
        }
    });*/
}