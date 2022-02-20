//
//  Utils.hpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 14.02.2022.
//

#ifndef Utils_hpp
#define Utils_hpp

#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <sstream>
#include "LogFrame.hpp"

void readFromFile(const string path, vector<LogFrame> &to);

string decToHex(const u_int64_t dec);

string hexToMAC(const string hex);

#endif /* Utils_hpp */
