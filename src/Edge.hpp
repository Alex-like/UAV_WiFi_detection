//
//  Edge.hpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 20.02.2022.
//

#ifndef Edge_hpp
#define Edge_hpp

#include <string>
#include <sstream>
#include "Utils.hpp"

using namespace std;

class Edge {
private:
    u_int64_t from; // TA - transmitter address
    u_int64_t to; // RA - receiver address
    u_int64_t weight; // Number of requests
public:
    Edge(u_int64_t from_v, u_int64_t to_v, u_int64_t cnt);
    string toString();
    u_int64_t getFrom();
    u_int64_t getTo();
    u_int64_t getWeight();
};

#endif /* Edge_hpp */
