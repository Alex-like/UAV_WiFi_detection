//
//  Edge.cpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 20.02.2022.
//

#include "Edge.hpp"

Edge::Edge(u_int64_t from_v, u_int64_t to_v, u_int64_t cnt) {
    from = from_v;
    to = to_v;
    weight = cnt;
}

string Edge::toString() {
    stringstream ss;
    ss << hexToMAC(decToHex(from)) << " -> " << hexToMAC(decToHex(to)) << " : " << weight;
    return ss.str();
}

u_int64_t Edge::getFrom() {
    return from;
}

u_int64_t Edge::getTo() {
    return to;
}

u_int64_t Edge::getWeight() {
    return weight;
}
