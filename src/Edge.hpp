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

/**
 * Connection structure.
 *
 * Decompose connection to transmitter address, receiver address and number of times they connected with each other.
 */
class Edge {
private:
    /// TA - transmitter address
    u_int64_t from;
    /// RA - receiver address
    u_int64_t to;
    /// Number of requests
    u_int64_t weight;
public:
    /**
     * Basic constructor.
     *
     * @param from_v transmitted address.
     * @param to_v receiver address.
     * @param cnt how many times they connected with each other.
     */
    Edge(u_int64_t from_v, u_int64_t to_v, u_int64_t cnt);
    /**
     * String value of edge.
     *
     * @return string representation of Connection.
     */
    string toString();
    /**
     * Getter for transmitter addres.
     *
     * @return transmitter address.
     */
    u_int64_t getFrom();
    /**
     * Getter for receiver addres.
     *
     * @return receiver address.
     */
    u_int64_t getTo();
    /**
     * Getter for number of connections.
     *
     * @return times of connections.
     */
    u_int64_t getWeight();
};

#endif /* Edge_hpp */
