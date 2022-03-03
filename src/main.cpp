//
//  main.cpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 13.02.2022.
//

#include <iostream>
#include <fstream>
#include "LogFrame.hpp"
#include "Statistics.hpp"
#include "Utils.hpp"
#include "Graph.hpp"
#include "GroupedGraph.hpp"

using namespace std;

void readFromLog() {
    string path;
    cout << "Введите путь до файла:\n";
    cin >> path;
    cout << '\n';
    vector<LogFrame> frames;
    readFromFile(path, frames);
    
//    for (LogFrame frame : frames) {
//        cout << frame.toString() << '\n';
//        cout << frame.getTAAndRA() << "\n\n";
//    }
    
//    Statistics stat(frames);
//    cout << stat.toString() << "\n";
    
//    Graph graph = Graph();
//    for (LogFrame frame : frames) {
//        graph.addEdge(frame.getTA(), frame.getRA());
//    }
//    cout << graph.toString() << "\n";
    
    GroupedGraph graph = GroupedGraph();
    for (LogFrame frame : frames) {
        if (frame.getType() == "Management/Beacon") {
            if (frame.getSSID().has_value() && frame.getTA().has_value()) {
                graph.addGroup(frame.getSSID().value(), frame.getTA().value());
            }
        } else {
            if (frame.getTA().has_value() && frame.getRA().has_value()) {
                graph.addEdge(frame.getTA().value(), frame.getRA().value());
            }
        }
    }
    cout << graph.toString() << '\n';
}

int main(int argc, const char * argv[]) {
    readFromLog();
    return 0;
}
