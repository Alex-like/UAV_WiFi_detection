//
//  GroupedGraph.cpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 03.03.2022.
//

#include "GroupedGraph.hpp"

using namespace std;

GroupedGraph::GroupedGraph(map<u_int64_t, Group> groups_v) : Graph() {
    groups = groups_v;
}

void GroupedGraph::addGroup(string name, u_int64_t address) {
    if (groups.find(address) == groups.end()) {
        groups[address] = Group(name, address);
    }
}

void GroupedGraph::addEdge(u_int64_t from, u_int64_t to) {
    if (groups.find(from) != groups.end() && to != BROADCAST) {
        groups[from].addClient(to, true);
        return;
    }
    if (groups.find(to) != groups.end() && from != BROADCAST) {
        groups[to].addClient(from, false);
        return;
    }
    Graph::addEdge(from, to);
}

string GroupedGraph::toString() {
    stringstream ss;
    for (auto& group : groups) {
        ss << group.second.toString() << '\n';
    }
    ss << Graph::toString();
    return ss.str();
}
