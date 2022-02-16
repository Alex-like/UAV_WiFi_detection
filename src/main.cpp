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
#include "PCAPFrame.hpp"

using namespace std;

void readFromLog() {
    string path;
    cout << "Введите путь до файла:\n";
    cin >> path;
    vector<LogFrame> frames;
    readFromFile(path, frames);
    
//    for (LogFrame frame : frames)
//        cout << frame.toString();
    
    Statistics stat(frames);
    cout << stat.toString() << endl;
}

void readFromPcap() {
    string path = "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/3dr_solo/dsss/frames.pcap";
//    char errbuff[PCAP_ERRBUF_SIZE];
//    pcap_t * pcap = pcap_open_offline(path.c_str(), errbuff);
//    struct pcap_pkthdr *header;
//    const u_char *data;
//
//    u_int packetCount = 0;
//    while( int returnValue = pcap_next_ex(pcap , &header, &data) >= 0) {
//        cout << "test";
//        printf("Packet number %i\n", ++packetCount);
//        printf("Packet size: %d bytes\n", header->len);
//        if (header->len != header->caplen)
//            printf("Warning! Packet size different from capture size: %u bytes\n", header->len);
//        printf("Epoch time: %ld:%d seconds\n\n\n", header->ts.tv_sec, header->ts.tv_usec);
//    }
//    pcap_close(pcap);
}

int main(int argc, const char * argv[]) {
    readFromPcap();
    return 0;
}
