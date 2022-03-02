//
//  Group.hpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 02.03.2022.
//

#ifndef Group_hpp
#define Group_hpp

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include "Utils.hpp"

using namespace std;

class Group {
private:
    string SSID; // Name
    u_int64_t host; // BSSID
    map<u_int64_t, pair<u_int64_t, u_int64_t>> clients;
public:
    string toString();
    void addClient(const u_int64_t client, const bool received); // recived = true - получено | recieved = false - отправлено
};


#endif /* Group_hpp */
