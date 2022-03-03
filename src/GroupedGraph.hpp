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

class GroupedGraph: public Graph {
private:
    map<u_int64_t, Group> groups; // access point -> Group
public:
    GroupedGraph(map<u_int64_t, Group> groups_v = {});
    void addGroup(string name, u_int64_t address);
    void addEdge(u_int64_t from, u_int64_t to);
    string toString();
};

#endif /* GroupedGraph_hpp */
