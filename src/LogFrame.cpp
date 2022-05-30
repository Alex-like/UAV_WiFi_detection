//
//  LogFrame.cpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 13.02.2022.
//

#include "LogFrame.hpp"

using namespace std;

LogFrame::LogFrame(u_int64_t ind_v, float Offset_v, string BW_v, string MCS_v, int Size_v, string Frame_v, string info_v, bool FCS_v, optional<string> Type_v, optional<string> SSID_v, optional<u_int64_t> TA_v, optional<u_int64_t> RA_v, optional<bool> moreFragments_v, optional<u_int64_t> seqNum_v, optional<u_int64_t> fragNum_v) {
    ind = ind_v;
    Offset = Offset_v;
    BW = BW_v;
    MCS = MCS_v;
    Size = Size_v;
    Frame = Frame_v;
    info = info_v;
    FCS = FCS_v;
    Type = Type_v;
    SSID = SSID_v;
    TA = TA_v;
    RA = RA_v;
    moreFragments = moreFragments_v;
    seqNum = seqNum_v;
    fragNum = fragNum_v;
}

bool LogFrame::isCorrect() {
    return FCS;
}

string LogFrame::toString() {
    stringstream s;
    s << "Index=" << ind << " FCS=" << (isCorrect() ? "Success" : "Fail") << endl;
    return s.str();
}

string LogFrame::getTAAndRA() {
    string TA_str = TA.has_value() ? to_string(TA.value()) : " –";
    string RA_str = RA.has_value() ? to_string(RA.value()) : " –";
    return "TA=" + TA_str + " RA=" + RA_str;
}

optional<string> LogFrame::getType() {
    return Type;
}

optional<string> LogFrame::getSSID() {
    return SSID;
}

optional<u_int64_t> LogFrame::getTA() {
    return TA;
}

optional<u_int64_t> LogFrame::getRA() {
    return RA;
}

string LogFrame::getData() {
    return info;
}

int LogFrame::getSize() {
    return Size;
}

bool LogFrame::getMoreFrags() {
    return moreFragments.has_value() ? moreFragments.value() : false;
}

u_int64_t LogFrame::getSeqNum() {
    return seqNum.has_value() ? seqNum.value() : 0;
}

u_int64_t LogFrame::getFragNum() {
    return fragNum.has_value() ? fragNum.value() : 0;
}

float LogFrame::getOffset() {
    return Offset;
}

LogFrame parse(const vector<string> &lines) {
    // init
    float Offset_v = 0.0;
    string BW_v = "", MCS_v = "", Frame_v = "", info_v = "";
    u_int64_t ind_v = 1;
    int Size_v = 0;
    optional<u_int64_t> TA_v, RA_v, seqNum, fragNum;
    optional<string> Type_v, SSID_v;
    bool FCS_v = false, moreFrags= false;
    
    // parse
    const string MAC_template = "([0-9a-fA-F]+):([0-9a-fA-F]+):([0-9a-fA-F]+):([0-9a-fA-F]+):([0-9a-fA-F]+):([0-9a-fA-F]+)";
    // regular expressions for parsing DataFrame
    const regex regex_line1("(\\d+)\\s+Offset=(\\d+\\.\\d+),BW=(\\w+),MCS=(.+),Size=(\\d+)");
    const regex regex_line2("Frame=([0-9a-fA-F]+)");
    const regex regex_FCS("FCS=Fail");
    const regex regex_Type("Type=([^,]+),");
    const regex regex_MoreFrags("More Fragments=(\\d+)");
    const regex regex_SSID("[^B]SSID=('.*')");
    const regex regex_TA("TA.*?=" + MAC_template);
    const regex regex_RA("RA.*?=" + MAC_template);
    const regex regex_Seqnum("Seqnum=(\\d+)");
    const regex regex_Fragnum("Fragnum=(\\d+)");
    
    smatch line1_groups;
    if (regex_search(lines[0], line1_groups, regex_line1)) {
        ind_v = stoull(line1_groups[1]);
        Offset_v = stof(line1_groups[2]);
        BW_v = line1_groups[3];
        MCS_v = line1_groups[4];
        Size_v = stoi(line1_groups[5]);
    }
    
    smatch line2_groups;
    if (regex_search(lines[1], line2_groups, regex_line2)) {
        Frame_v = line2_groups[1];
    }
    
    smatch line3_groups;
    if (!regex_search(lines[2], line3_groups, regex_FCS)) {
        FCS_v = true;
    }
    info_v = lines[2];
    if (FCS_v && regex_search(info_v, line3_groups, regex_Type)) {
        Type_v = line3_groups[1];
    }
    if (FCS_v && regex_search(info_v, line3_groups, regex_SSID)) {
        SSID_v = line3_groups[1];
    }
    if (FCS_v && regex_search(info_v, line3_groups, regex_TA)) {
        string hex = line3_groups[1].str() + line3_groups[2].str() + line3_groups[3].str() + line3_groups[4].str() + line3_groups[5].str() + line3_groups[6].str();
        TA_v = stoull(hex, 0, 16);
    }
    if (FCS_v && regex_search(info_v, line3_groups, regex_RA)) {
        string hex = line3_groups[1].str() + line3_groups[2].str() + line3_groups[3].str() + line3_groups[4].str() + line3_groups[5].str() + line3_groups[6].str();
        RA_v = stoull(hex, 0, 16);
    }
    if (FCS_v && regex_search(info_v, line3_groups, regex_MoreFrags)) {
        string flag = line3_groups[1].str();
        int tmp = stoi(flag);
        moreFrags = tmp > 0 ? true : false;
    }
    if (FCS_v && regex_search(info_v, line3_groups, regex_Seqnum)) {
        string tmp = line3_groups[1].str();
        seqNum = stoull(tmp);
    }
    if (FCS_v && regex_search(info_v, line3_groups, regex_Fragnum)) {
        string tmp = line3_groups[1].str();
        fragNum = stoull(tmp);
    }
    
    // return
    return LogFrame(ind_v, Offset_v, BW_v, MCS_v, Size_v, Frame_v, info_v, FCS_v, Type_v, SSID_v, TA_v, RA_v, moreFrags, seqNum, fragNum);
}
