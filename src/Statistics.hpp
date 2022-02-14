//
//  Statistics.hpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 14.02.2022.
//

#ifndef Statistics_hpp
#define Statistics_hpp

#include <vector>
#include <string>
#include "LogFrame.hpp"

using namespace std;

class Statistics {
private:
    int correct;
    int incorrect;
    unsigned long long whole;
public:
    Statistics(int whole_v, int correct_v, int incorrect_v);
    
    Statistics(const vector<LogFrame> &frames);
    
    string toString();
};

#endif /* Statistics_hpp */
