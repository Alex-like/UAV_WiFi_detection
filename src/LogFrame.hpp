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
    // Correctness of Frame
    bool FCS;
public:
    LogFrame(int ind_v, float Offset_v, string BW_v, string MCS_v, int Size_v, string Frame_v, string info_v, bool FCS_v);
    
    bool isCorrect();
    
    string toString();
};

LogFrame parse(const vector<string> &lines);

#endif /* LogFrame_hpp */