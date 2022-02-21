//
//  LogFrame.hpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 13.02.2022.
//

#ifndef LogFrame_hpp
#define LogFrame_hpp

#include <vector>
#include <string>
#include <regex>
#include <iostream>
#include <sstream>
#include <optional>

using namespace std;

class LogFrame {
private:
    // 1 Line
    int ind;
    float Offset;
    string BW;
    string MCS;
    int Size;
    // 2 Line
    string Frame;
    // 3 Line
    string info;
    optional<string> Type;
    // MAC addresses
    optional<u_int64_t> TA; // TA - transmitter address
    optional<u_int64_t> RA; // RA - receiver address
    // Correctness of Frame
    bool FCS;
public:
    LogFrame(int ind_v, float Offset_v, string BW_v, string MCS_v, int Size_v, string Frame_v, string info_v, bool FCS_v, optional<string> Type_v, optional<u_int64_t> TA_v, optional<u_int64_t> RA_v);
    
    bool isCorrect();
    
    string toString();
    
    string getTAAndRA();
    
    optional<string> getType();
    
    optional<u_int64_t> getTA();
    
    optional<u_int64_t> getRA();
};

LogFrame parse(const vector<string> &lines);

#endif /* LogFrame_hpp */
