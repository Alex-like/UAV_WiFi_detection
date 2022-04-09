//
//  main.cpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 13.02.2022.
//

#include <iostream>
#include <fstream>
#include <functional>
#include "LogFrame.hpp"
#include "Statistics.hpp"
#include "Utils.hpp"
#include "Graph.hpp"
#include "GroupedGraph.hpp"
#include "Classifier.hpp"

using namespace std;

void getStatistics(vector<LogFrame> &frames) {
    Statistics stat(frames);
    cout << stat.toString() << '\n';
}

void countFrameTypes(vector<LogFrame> &frames) {
    map<string, u_int64_t> types;
    for (LogFrame frame : frames) {
        if (frame.getType().has_value()) {
            if (types.find(frame.getType().value()) == types.end()) {
                types[frame.getType().value()] = 0;
            }
            types[frame.getType().value()]++;
        }
    }
    for (auto &type : types) {
        cout << type.first << " : " << type.second << '\n';
    }
}

void makeSimpleGraph(vector<LogFrame> &frames) {
    Graph graph = Graph();
    for (LogFrame frame : frames) {
        graph.addEdge(frame.getTA(), frame.getRA());
    }
    cout << graph.toString() << "\n";
}

void getNetworks(vector<LogFrame> &frames) {
    GroupedGraph graph = GroupedGraph();
    for (LogFrame frame : frames) {
        if (frame.getType() == "Management/Beacon") {
            if (frame.getSSID().has_value() && frame.getTA().has_value()) {
                graph.addGroup(frame.getSSID().value(), frame.getTA().value());
            }
        }
        if (frame.getTA().has_value() && frame.getRA().has_value()) {
            graph.addEdge(frame.getTA().value(), frame.getRA().value());
        }
    }
    cout << graph.toString() << '\n';
}

void getAddressesFromFrames(vector<LogFrame> &frames) {
    set<string> macs;
    for (LogFrame frame : frames) {
        getAddressesFromStr(frame.getData(), macs);
    }
    for (string mac : macs) {
        cout << mac << '\n';
    }
    cout << '\n';
}

void getSSIDFromFrames(vector<LogFrame> &frames) {
    set<string> ssids;
    for (LogFrame frame : frames) {
        optional<string> ssid = frame.getSSID();
        if (ssid.has_value()) {
            ssids.insert(ssid.value());
        }
    }
    for (string ssid : ssids) {
        cout << ssid << '\n';
    }
    cout << '\n';
}

void classifyAllMacFromFrames(vector<LogFrame> &frames) {
    GroupedGraph graph = GroupedGraph();
    for (LogFrame frame : frames) {
        if (frame.getType() == "Management/Beacon") {
            if (frame.getSSID().has_value() && frame.getTA().has_value()) {
                graph.addGroup(frame.getSSID().value(), frame.getTA().value());
            }
        }
        if (frame.getTA().has_value() && frame.getRA().has_value()) {
            graph.addEdge(frame.getTA().value(), frame.getRA().value());
        }
    }
    cout << graph.toString() << '\n';
    
    map<u_int64_t, string> macAndSSID;
    for (LogFrame frame : frames) {
        if (frame.getTA().has_value() && !checkExist(frame.getTA().value(), macAndSSID)) {
            string ssid = "";
            if (frame.getSSID().has_value()) {
                ssid = frame.getSSID().value();
            }
            macAndSSID[frame.getTA().value()] = ssid;
        }
    }
    
    set<string> macs;
    for (LogFrame frame : frames) {
        getAddressesFromStr(frame.getData(), macs);
    }
    
    Classifier classifier = Classifier(graph);
    vector<Object> objs;
    for (string mac : macs) {
        u_int64_t mac_v = macToHex(mac);
        optional<Object> obj = classifier.classify(mac_v, macAndSSID[mac_v]);
        if (obj.has_value()) {
            objs.push_back(obj.value());
        }
    }
    for (Object obj : objs) {
        cout << obj.toString() << '\n';
    }
}

void printFramesTypes(vector<LogFrame> &frames) {
    map<u_int64_t, map<u_int64_t, vector<string>>> types;
    for (LogFrame frame : frames) {
        optional<u_int64_t> tmp_num = frame.getTA();
        if (!tmp_num.has_value())
            continue;
        u_int64_t TA = tmp_num.value();
        tmp_num = frame.getRA();
        if (!tmp_num.has_value())
            continue;
        u_int64_t RA = tmp_num.value();
        optional<string> tmp_str = frame.getType();
        if (!tmp_str.has_value())
            continue;
        string type = tmp_str.value();
        types[TA][RA].emplace_back(type);
    }
    for (auto &p1 : types)
        for (auto &p2 : p1.second) {
            cout << p1.first << " -> " << p2.first << " : ";
            for (string type : p2.second)
                cout << type << ' ';
            cout << '\n';
        }
    
}

void workWithDataFrames(vector<LogFrame> &frames) {
    vector<LogFrame> dataFrames = filter(frames, [](LogFrame f) { return f.getType().has_value() && f.getType().value().compare(0, 4, "Data") == 0; });
    map<u_int64_t, map<u_int64_t, u_int64_t>> transmitionsAmount;
    for (LogFrame frame : dataFrames) {
        optional<u_int64_t> tmp_num = frame.getTA();
        if (!tmp_num.has_value())
            continue;
        u_int64_t TA = tmp_num.value();
        tmp_num = frame.getRA();
        if (!tmp_num.has_value())
            continue;
        u_int64_t RA = tmp_num.value();
        transmitionsAmount[TA][RA]++;
    }
    for (auto &p1 : transmitionsAmount)
        for (auto &p2 : p1.second)
            cout << hexToMAC(decToHex(p1.first)) << " -> " << hexToMAC(decToHex(p2.first)) << " : " << p2.second << '\n';
}

void workWithDefiniteFile(function<void(vector<LogFrame> &)> action) {
    string path;
    cout << "Введите путь до файла:\n";
    cin >> path;
    cout << '\n';
    vector<LogFrame> frames;
    readFromFile(path, frames);
    action(frames);
}

void workWithAllFiles(function<void(vector<LogFrame> &)> action) {
    string paths[] = {
        
    };
    vector<LogFrame> frames;
    for (string path : paths) {
        cout << path << '\n';
        readFromFile(path, frames);
        action(frames);
        frames.clear();
    }
}

int main(int argc, const char * argv[]) {
//    workWithDefiniteFile(getStatistics);
//    workWithAllFiles(classifyAllMacFromFrames);
    workWithAllFiles(workWithDataFrames);
    return 0;
}
