//
//  Group.cpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 02.03.2022.
//

#include "Group.hpp"

using namespace std;

Group::Group(string name, u_int64_t MAC, map<u_int64_t, pair<u_int64_t, u_int64_t>> addresses) {
    SSID = name;
    host = MAC;
    clients = addresses;
}

string Group::toString() {
    stringstream ss;
    ss << SSID << " : " << hexToMAC(decToHex(host)) << '\n';
    for (auto& client : clients) {
        pair<u_int64_t, u_int64_t> framesCount = client.second;
        ss << '\t' << hexToMAC(decToHex(client.first)) << " : received " << framesCount.first << " | transmitted " << framesCount.second << '\n';
    }
    return ss.str();
}

void Group::addClient(const u_int64_t client, const bool received) {
    if (clients.find(client) == clients.end()) {
        clients[client] = pair<u_int64_t, u_int64_t>(0, 0);
    }
    if (received) {
        clients[client].first += 1;
    } else {
        clients[client].second += 1;
    }
}
