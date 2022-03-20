//
//  Utils.cpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 14.02.2022.
//

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
}

string decToHex(const u_int64_t dec) {
    stringstream ss;
    ss << std::hex << dec;
    return ss.str();
}

string hexToMAC(string hex) {
    uint8_t len = hex.length();
    if (len < 12) {
        for (int i = 0; i < 12 - len; i++) {
            hex = '0' + hex;
        }
    }
    string res = hex.substr(0, 2);
    for (int i = 1; i < 6; i++) {
        res += ':' + hex.substr(2 * i, 2);
    }
    return res;
}

void getAddressesFromFile(string path) {
    ifstream in(path);
    set<string> macs = {};
    if (in.is_open()) {
        smatch match;
        string line;
        while (getline(in, line, '\n')) {
            // skip empty string
            if (regex_match(line, match, regex("^\\s*$"))) {
                continue;
            }
            
            getAddressesFromStr(line, macs);
        }
    }
    in.close();
    for (string mac : macs) {
        cout << mac << '\n';
    }
}

// TODO merge `getAddressesFromFile` and `getAddressesFromStr`

void getAddressesFromStr(string str, set<string> &macs) {
    const regex regex_MAC("[0-9A-Za-z]+(:[0-9A-Za-z]+){5}");
    const vector<smatch> mac{
        sregex_iterator{cbegin(str), cend(str), regex_MAC},
        sregex_iterator{}
    };
    for (smatch address : mac) {
        macs.insert(address.str(0));
    }
}

u_int64_t macToHex(string mac) {
    string hex = "";
    const regex regex_ditits("([0-9A-Za-z]+):([0-9A-Za-z]+):([0-9A-Za-z]+):([0-9A-Za-z]+):([0-9A-Za-z]+):([0-9A-Za-z]+)");
    smatch digits;
    if (regex_search(mac, digits, regex_ditits)) {
        for (string num : digits) {
            if (num.size() < 3) {
                hex += num;
            }
        }
    }
    return stoull(hex, 0, 16);
}
