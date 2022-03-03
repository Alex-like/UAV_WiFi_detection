//
//  Group.hpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 02.03.2022.
//

#ifndef Group_hpp
#define Group_hpp

#include <map>
#include <string>
#include <sstream>
#include "Utils.hpp"

using namespace std;

class Group {
private:
    string SSID; // Name
    u_int64_t host; // BSSID
    map<u_int64_t, pair<u_int64_t, u_int64_t>> clients; // MAC -> {received, transmitted}
public:
    Group(string name = "", u_int64_t MAC = 0, map<u_int64_t, pair<u_int64_t, u_int64_t>> addresses = {});
    string toString();
    void addClient(const u_int64_t client, const bool received); // received = true - получено от ТД | received = false - отправлено к ТД
};

#endif /* Group_hpp */
