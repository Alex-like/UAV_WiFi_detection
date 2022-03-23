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
#include "Classifier.hpp"

using namespace std;

void readFromLog(string path) {
    
    vector<LogFrame> frames;
    readFromFile(path, frames);
    
    set<string> macs;
    for (LogFrame frame : frames) {
        getAddressesFromStr(frame.getData(), macs);
    }
    Classifier classifier = Classifier();
    vector<Object> objs;
    for (string mac : macs) {
        optional<Object> obj = classifier.classify(macToHex(mac), "");
        if (obj.has_value()) {
            objs.push_back(obj.value());
        }
    }
    for (Object obj : objs) {
        cout << obj.toString() << '\n';
    }
    
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
}

void readAddressesFromAllFiles() {
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
    set<string> macs;
    for (string path : paths) {
//        getAddressesFromFile(path);
        cout << path << '\n';
        readFromFile(path, frames);
        for (LogFrame frame : frames) {
            getAddressesFromStr(frame.getData(), macs);
        }
        for (string mac : macs) {
            cout << mac << '\n';
        }
        cout << '\n';
        frames.clear();
        macs.clear();
    }
}

void readSSIDFromAllFiles() {
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
    set<string> ssids;
    for (string path : paths) {
//        getAddressesFromFile(path);
        cout << path << '\n';
        readFromFile(path, frames);
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
        frames.clear();
        ssids.clear();
    }
}

int main(int argc, const char * argv[]) {
//    string path;
//    cout << "Введите путь до файла:\n";
//    cin >> path;
//    cout << '\n';
//    readFromLog(path);
    
//    readAddressesFromAllFiles();
    
//    readSSIDFromAllFiles();
    
    Classifier classifier = Classifier();
    set<string> companies;
    for (auto &p : classifier.getCompanies()) {
        companies.insert(p.second);
    }
    for (string str : companies) {
        cout << str << '\n';
    }
    return 0;
}
