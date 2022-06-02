//
//  LogFrame.cpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 13.02.2022.
//

#include "LogFrame.hpp"

using namespace std;

LogFrame::LogFrame() {}

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

u_int64_t LogFrame::getInd() {
    return ind;
}

string LogFrame::getBW() {
    return BW;
}

string LogFrame::getMCS() {
    return MCS;
}

string LogFrame::getFrame() {
    return Frame;
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

void LogFrame::setOffset(const float offset) {
    this->Offset = offset;
}

void LogFrame::setBW(const string bw) {
    this->BW = bw;
}

void LogFrame::setMCS(const string mcs) {
    this->MCS = mcs;
}

void LogFrame::setSize(const int size) {
    this->Size = size;
}

void LogFrame::setFrame(const string frame) {
    this->Frame = frame;
}

void LogFrame::setData(const string info) {
    this->info = info;
}

void LogFrame::setType(const optional<string> type) {
    this->Type = type;
}

void LogFrame::setSSID(const optional<string> ssid) {
    this->SSID = ssid;
}

void LogFrame::setTA(const optional<u_int64_t> ta) {
    this->TA = ta;
}

void LogFrame::setRA(const optional<u_int64_t> ra) {
    this->RA = ra;
}

void LogFrame::setFCS(const bool fcs) {
    this->FCS = fcs;
}

void LogFrame::setMoreFrags(const optional<bool> moreFrags) {
    this->moreFragments = moreFrags;
}

void LogFrame::setSeqNum(const optional<u_int64_t> seqNum) {
    this->seqNum = seqNum;
}

void LogFrame::setFragNum(const optional<u_int64_t> fragNum) {
    this->fragNum = fragNum;
}

LogFrame parse(const vector<string> &lines, const bool hasHeader, const bool hasBody) {
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
    const regex regex_ind("^(\\d+)\\s+");
    const regex regex_line1("Offset=(\\d+\\.\\d+),BW=(\\w+),MCS=(.+),Size=(\\d+)");
    const regex regex_line2("Frame=([0-9a-fA-F]+)|Bits=([0-9a-fA-F]+)");
    const regex regex_FCS("FCS=Fail");
    const regex regex_Type("Type=([^,]+),");
    const regex regex_MoreFrags("More Fragments=(\\d+)");
    const regex regex_SSID("[^B]SSID=('.*')");
    const regex regex_TA("TA.*?=" + MAC_template);
    const regex regex_RA("RA.*?=" + MAC_template);
    const regex regex_Seqnum("Seqnum=(\\d+)");
    const regex regex_Fragnum("Fragnum=(\\d+)");
    
    smatch ind;
    if (regex_search(lines[0], ind, regex_ind))
        ind_v = stoull(ind[1]);
    
    if (hasHeader) {
        smatch line1_groups;
        if (regex_search(lines[0], line1_groups, regex_line1)) {
            Offset_v = stof(line1_groups[1]);
            BW_v = line1_groups[2];
            MCS_v = line1_groups[3];
            Size_v = stoi(line1_groups[4]);
        }
        
        smatch line2_groups;
        if (regex_search(lines[hasBody ? 1 : 0], line2_groups, regex_line2)) {
            Frame_v = line2_groups[hasBody ? 1 : 2];
        }
    }
    
    if (hasBody) {
        smatch line3_groups;
        if (!regex_search(lines[hasHeader ? 2 : 0], line3_groups, regex_FCS)) {
            FCS_v = true;
        }
        info_v = lines[hasHeader ? 2 : 0];
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
    }
    
    // return
    return LogFrame(ind_v, Offset_v, BW_v, MCS_v, Size_v, Frame_v, info_v, FCS_v, Type_v, SSID_v, TA_v, RA_v, moreFrags, seqNum, fragNum);
}
