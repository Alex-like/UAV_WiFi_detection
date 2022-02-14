//
//  main.cpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 13.02.2022.
//

#include <iostream>
#include <fstream>
#include <regex>
#include "LogFrame.hpp"
#include "Statistics.hpp"
#include "Utils.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    string path;
    cin >> path;
    vector<LogFrame> frames;
    readFromFile(path, frames);
    
//    for (LogFrame frame : frames)
//        cout << frame.toString();
    
    Statistics stat(frames);
    cout << stat.toString() << endl;
    return 0;
}
