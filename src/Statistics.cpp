//
//  Statistics.cpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 14.02.2022.
//

#include "Statistics.hpp"

using namespace std;

Statistics::Statistics(u_int64_t whole_v, u_int64_t correct_v, u_int64_t incorrect_v) {
    whole = whole_v;
    correct = correct_v;
    incorrect = incorrect_v;
}

Statistics::Statistics(const vector<LogFrame> &frames) {
    whole = frames.size();
    correct = 0;
    incorrect = 0;
    for (LogFrame frame : frames) {
        if (frame.isCorrect()) {
            correct++;
        } else {
            incorrect++;
        }
    }
}

string Statistics::toString() {
    stringstream s;
    s << "Whole frames' number: " << whole << "\nCorrect frames: " << correct << "\nIncorrect frames: " << incorrect << "\nPercentage correct frames of the total: " << ((long double)correct / (long double)whole * 100.0) << endl;
    return s.str();
}
