//
//  main.cpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 13.02.2022.
//

#include <iostream>
#include <fstream>
#include <regex>
#include <pcap.h>
#include "LogFrame.hpp"
#include "Statistics.hpp"
#include "Utils.hpp"

using namespace std;

void readFromLog() {
    string path;
    cout << "Введите путь до файла:\n";
    cin >> path;
    vector<LogFrame> frames;
    readFromFile(path, frames);
    
    for (LogFrame frame : frames) {
        cout << frame.toString() << '\n';
        cout << frame.getTAAndRA() << "\n\n";
    }
    
    Statistics stat(frames);
    cout << stat.toString() << endl;
}

int main(int argc, const char * argv[]) {
    readFromLog();
    return 0;
}
