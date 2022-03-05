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

/**
 * Working with network.
 *
 * Contains name `SSID`, MAC-address of hotspot `host` and information about each client `clients`, which connected with.
 */
class Group {
private:
    /// Name equals SSID
    string SSID;
    /// BSSID MAC-address
    u_int64_t host;
    /// Get number of recieved and transmitted frames by hexadecimal value of MAC-address.
    map<u_int64_t, pair<u_int64_t, u_int64_t>> clients;
public:
    /**
     * Basic default constructor.
     *
     * @param name SSID value from "Management/Beacon" frame.
     * @param MAC BSSID MAC-address from that frame.
     * @param addresses map with MAC-addresses and number of recieved and transmitted frames. If it's empty init it by empty map.
     */
    Group(string name = "", u_int64_t MAC = 0, map<u_int64_t, pair<u_int64_t, u_int64_t>> addresses = {});
    /**
     * Convert Network to string value.
     *
     * @return string representation of network.
     */
    string toString();
    /**
     * Append 1 to number of recieved / transmitted frames.
     *
     * @param client client's MAC-address.
     * @param received flag which show if frame was received or transmitted. If frame was received `received` equals True else False.
     */
    void addClient(const u_int64_t client, const bool received); // received = true - получено от ТД | received = false - отправлено к ТД
};

#endif /* Group_hpp */
