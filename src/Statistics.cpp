//
//  Statistics.cpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 14.02.2022.
//

#include "Statistics.hpp"

using namespace std;

Statistics::Statistics(u_int64_t whole_v, u_int64_t correct_v, u_int64_t incorrect_v, u_int64_t noAddress_v) {
    whole = whole_v;
    correct = correct_v;
    incorrect = incorrect_v;
    noAddress = noAddress_v;
}

Statistics::Statistics(const vector<LogFrame> &frames) {
    whole = frames.size();
    correct = 0;
    incorrect = 0;
    noAddress = 0;
    for (LogFrame frame : frames) {
        if (frame.isCorrect()) {
            correct++;
        } else {
            incorrect++;
        }
        if (frame.isCorrect() && (!frame.getTA().has_value() || !frame.getRA().has_value())) {
            noAddress++;
        }
    }
}

string Statistics::toString() {
    stringstream s;
    s << "Whole frames' number: " << whole << "\nCorrect frames: " << correct << "\nIncorrect frames: " << incorrect << "\nFrames without some address information: " << noAddress << "\nPercentage correct frames of the total: " << ((long double)correct / (long double)whole * 100.0) << '%' << endl;
    return s.str();
}
