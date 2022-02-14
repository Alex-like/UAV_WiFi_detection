//
//  Utils.cpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 14.02.2022.
//

#include <fstream>
#include <regex>
#include "Utils.hpp"

void readFromFile(const string path, vector<LogFrame> &to) {
    vector <string> lines(3);
    int count = 0;
    ifstream in(path);
    if (in.is_open()) {
        string line;
        while (getline(in, line, '\n')) {
            // skip empty string
            smatch match;
            if (regex_match(line, match, regex("^\\s*$"))) {
                continue;
            }
            
            // remeber string
            lines[count++] = line;
            if (count == 3) {
                to.push_back(parse(lines));
                count = 0;
            }
        }
    }
    in.close();
    return;
}
