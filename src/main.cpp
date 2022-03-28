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

void readFromLog(string path) {
    
    vector<LogFrame> frames;
    readFromFile(path, frames);
    
//    for (LogFrame frame : frames) {
//        cout << frame.toString() << '\n';
//        cout << frame.getTAAndRA() << "\n\n";
//    }
    
//    Statistics stat(frames);
//    cout << stat.toString() << '\n';
    
//    map<string, u_int64_t> types;
//    for (LogFrame frame : frames) {
//        if (frame.getType().has_value()) {
//            if (types.find(frame.getType().value()) == types.end()) {
//                types[frame.getType().value()] = 0;
//            }
//            types[frame.getType().value()]++;
//        }
//    }
//    for (auto &type : types) {
//        cout << type.first << " : " << type.second << '\n';
//    }
    
//    Graph graph = Graph();
//    for (LogFrame frame : frames) {
//        graph.addEdge(frame.getTA(), frame.getRA());
//    }
//    cout << graph.toString() << "\n";
    
//    GroupedGraph graph = GroupedGraph();
//    for (LogFrame frame : frames) {
//        if (frame.getType() == "Management/Beacon") {
//            if (frame.getSSID().has_value() && frame.getTA().has_value()) {
//                graph.addGroup(frame.getSSID().value(), frame.getTA().value());
//            }
//        }
//        if (frame.getTA().has_value() && frame.getRA().has_value()) {
//            graph.addEdge(frame.getTA().value(), frame.getRA().value());
//        }
//    }
//    cout << graph.toString() << '\n';
//
//    map<u_int64_t, optional<string>> macAndSsid;
//    for (LogFrame frame : frames) {
//        if (frame.getTA().has_value()) {
//            macAndSsid[frame.getTA().value()] = frame.getSSID();
//        }
//    }
//
//    set<string> macs;
//    for (LogFrame frame : frames) {
//        getAddressesFromStr(frame.getData(), macs);
//    }
//
//    Classifier classifier = Classifier(graph);
//    vector<Object> objs;
//    for (string mac : macs) {
//        u_int64_t mac_v = macToHex(mac);
//        optional<Object> obj = classifier.classify(mac_v, macAndSsid[mac_v]);
//        if (obj.has_value()) {
//            objs.push_back(obj.value());
//        }
//    }
//    for (Object obj : objs) {
//        cout << obj.toString() << '\n';
//    }
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

void workWithAllFiles(function<void(vector<LogFrame> &)> action) {
    string paths[] = {
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/3dr_solo/dsss/frames.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/3dr_solo/wifi-ofdm-20/frames.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/fimi_x8_me_2020/1wifi_fc_5825000000_fs_12000000.pcm.result/frames.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/fimi_x8_me_2020/2wifi_fc_5825000000_fs_12000000.pcm.result/frames.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/fimi_x8_me_2020/wifi_fc_5825000000_fs_12000000.pcm.result/frames.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/hubsan_zino_2/Vega_2021-03-30_15-13-49-781_1_5785000000_10000000_11764706.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/nelk_b6/NELK_B6_Downlink_5220.213483MHz_46625.000000KHz.pcm.log/frames.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/parrot_bebop2/on_the_ground_gps-dsss/frames.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/parrot_bebop2/on_the_ground_gps-ofdm-20/frames.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/skydio2/3_Подключение_App,_калибровка,_попытка_полета-Unsafe_space/frames.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/skydio2/4_Взлет_и_посадка_в_офисе/frames.log"
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
//    string path;
//    cout << "Введите путь до файла:\n";
//    cin >> path;
//    cout << '\n';
//    readFromLog(path);
    workWithAllFiles(classifyAllMacFromFrames);
    
    return 0;
}
