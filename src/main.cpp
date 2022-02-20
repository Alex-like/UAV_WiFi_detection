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
#include "Graph.hpp"

using namespace std;

void readFromLog() {
    string path;
    cout << "Введите путь до файла:\n";
    cin >> path;
    vector<LogFrame> frames;
    readFromFile(path, frames);
    
//    for (LogFrame frame : frames) {
//        cout << frame.toString() << '\n';
//        cout << frame.getTAAndRA() << "\n\n";
//    }
    
//    Statistics stat(frames);
//    cout << stat.toString() << endl;
    Graph graph = Graph();
    for (LogFrame frame : frames) {
        graph.addEdge(frame.getTA(), frame.getRA());
    }
    cout << graph.toString() << endl;
}

int main(int argc, const char * argv[]) {
    readFromLog();
//    cout << hexToMAC(decToHex(1252452)) << endl;
    return 0;
}
