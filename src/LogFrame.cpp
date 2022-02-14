//
//  LogFrame.cpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 13.02.2022.
//

#include "LogFrame.hpp"
#include <regex>
#include <iostream>

using namespace std;

LogFrame::LogFrame(int ind_v, float Offset_v, string BW_v, string MCS_v, int Size_v, string Frame_v, string info_v, bool FCS_v) {
    ind = ind_v;
    Offset = Offset_v;
    BW = BW_v;
    MCS = MCS_v;
    Size = Size_v;
    Frame = Frame_v;
    info = info_v;
    FCS = FCS_v;
}

bool LogFrame::isCorrect() {
    return FCS;
}

LogFrame parse(const vector<string> &lines) {
    // init
    float Offset_v = 0.0;
    string BW_v = "", MCS_v = "", Frame_v = "", info_v = "";
    int ind_v = 1, Size_v = 0;
    bool FCS_v = false;
    
    // parse
    // regular expressions for parsing DataFrame
    const regex regex_line1("^(\\d+)\\s+Offset=(\\d+\\.\\d+),BW=(\\w+),MCS=(.+),Size=(\\d+)$");
    const regex regex_line2("^Frame=(\\d+)$");
    const regex regex_line3("^FCS=Fail$");
    
    smatch line1_groups;
    if (regex_match(lines[0], line1_groups, regex_line1)) {
        ind_v = stoi(line1_groups[1]);
        Offset_v = stof(line1_groups[2]);
        BW_v = line1_groups[3];
        MCS_v = line1_groups[4];
        Size_v = stoi(line1_groups[5]);
    }
    
    smatch line2_groups;
    if (regex_match(lines[1], line2_groups, regex_line2)) {
        Frame_v = line2_groups[1];
    }
    
    smatch line3_groups;
    if (!regex_match(lines[2], line3_groups, regex_line3)) {
        FCS_v = true;
    }
    
    // return
    return LogFrame(ind_v, Offset_v, BW_v, MCS_v, Size_v, Frame_v, info_v, FCS_v);
}
