//
//  Utils.cpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 14.02.2022.
//

#include "Utils.hpp"

void readFromFile(const string path, vector<LogFrame> &to, const bool hasHeader, const bool hasBody) {
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
                to.push_back(parse(lines, hasHeader, hasBody));
                count = 0;
            }
        }
    }
    in.close();
}

void printToFile(const string path, map<u_int64_t, vector<float>> &from) {
    ofstream out(path, ios::app);
    if (out.is_open()) {
        for (auto &p : from) {
            out << hexToMAC(decToHex(p.first)) << " : ";
            out << "1 : ";
            out << toString(p.second) << '\n';
        }
    }
    out.close();
}

tuple<vector<u_int64_t>,vector<vector<float>>, vector<u_int32_t>> readDataForKNNFromFile(const string path) {
    vector<u_int32_t> classes;
    vector<u_int64_t> macs;
    vector<vector<float>> data;
    ifstream in(path);
    if (in.is_open()) {
        string line;
        while (getline(in, line, '\n')) {
            // skip empty string
            smatch match;
            if (regex_match(line, match, regex("^\\s*$"))) {
                continue;
            }
            u_int64_t mac;
            vector<float> features;
            u_int32_t cl;
            u_int32_t pos = 0;
            // parse
            const regex regexMAC("([0-9a-fA-F]+):([0-9a-fA-F]+):([0-9a-fA-F]+):([0-9a-fA-F]+):([0-9a-fA-F]+):([0-9a-fA-F]+)");
            const regex regexClass(": ([0-9]+) :");
            const regex regexFeature("([0-9e.-]+)");
            if (regex_search(line, match, regexMAC)) {
                string hex = match[1].str() + match[2].str() + match[3].str() + match[4].str() + match[5].str() + match[6].str();
                pos += hex.length() + 5;
                mac = stoull(hex, 0, 16);
            }
            if (regex_search(line, match, regexClass)) {
                string dec = match[1].str();
                pos += dec.length() + 4;
                cl = stoi(dec, 0);
            }
            string subString = line.substr(pos);
            const vector<string> matches{
                sregex_token_iterator{subString.begin(), subString.end(), regexFeature, 1}, // Mark `0` here i.e. whole regex match
                sregex_token_iterator{}
            };
            for (const string& num : matches)
                features.emplace_back(stof(num));
            macs.emplace_back(mac);
            data.emplace_back(features);
            classes.emplace_back(cl);
        }
    }
    return make_tuple(macs, data, classes);
}

string decToHex(const u_int64_t dec) {
    stringstream ss;
    ss << std::hex << dec;
    return ss.str();
}

string hexToMAC(string hex) {
    u_int8_t len = hex.length();
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

string strToLower(string s) {
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return tolower(c); });
    return s;
}

bool checkExist(u_int64_t key, map<u_int64_t, string> &dict) {
    return dict.find(key) != dict.end();
}

vector<LogFrame> filter(const vector<LogFrame>& vec, function<bool(LogFrame)> predicate) {
    vector<LogFrame> result;
    copy_if(begin(vec), end(vec), back_inserter(result), predicate);
    return result;
}

string toString(const vector<u_int64_t> &vec) {
    stringstream ss;
    for (auto &el : vec)
        ss << el << ' ';
    return ss.str();
}

string toString(const vector<float> &vec) {
    stringstream ss;
    for (auto &el : vec)
        ss << el << ' ';
    return ss.str();
}

float fpow(float base, int exp) {
    if (exp == 0) return 1.0;
    if (exp == 1) return base;
    float tmp = fpow(base, exp / 2);
    tmp *= tmp;
    return exp % 2 != 0 ? tmp * base : tmp;
}
