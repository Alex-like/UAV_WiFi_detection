//
//  GroupedGraph.hpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 03.03.2022.
//

#ifndef GroupedGraph_hpp
#define GroupedGraph_hpp

#include <map>
#include <vector>
#include <string>
#include <optional>
#include <sstream>
#include "Edge.hpp"
#include "Graph.hpp"
#include "Group.hpp"

using namespace std;

/// Hexadecimal value of broadcast MAC-address' mask.
const u_int64_t BROADCAST = 0xffffffffffff;

/**
 * Grouped graph into Network with hotspot and clients.
 *
 * Decompose graph to Networks and other simple edges. Make hotspot for Network by "addGroup". Append a directed edge to graph by "addEdge".
 */
class GroupedGraph: public Graph {
private:
    /// Get Group by number equals hexadecimal value of hotspot.
    map<u_int64_t, Group> groups;
public:
    /**
     * Basic default constructor.
     *
     * @param groups_v map with hotspots and groups. If it's empty init by empty map.
     */
    GroupedGraph(map<u_int64_t, Group> groups_v = {});
    /**
     * Add Network if it is necessary.
     *
     * @param name SSID value from frame.
     * @param address MAC-address, which equals BSSID value from frame.
     */
    void addGroup(string name, u_int64_t address);
    /**
     * Add hotspot-client connection.
     *
     * @param from MAC-address of transmitter.
     * @param to MAC-address of receiver.
     */
    void addEdge(u_int64_t from, u_int64_t to);
    /**
     * Convert grapg to string value.
     *
     * @return string representation of graph.
     */
    string toString();
};

#endif /* GroupedGraph_hpp */
