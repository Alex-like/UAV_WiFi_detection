//
//  Graph.cpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 19.02.2022.
//

#include "Graph.hpp"

using namespace std;

Graph::Graph(map<u_int64_t, map<u_int64_t, u_int64_t>> edges_v) {
    edges = edges_v;
}

Graph::Graph(vector<Edge> &edges_v) {
    Graph();
    for (Edge e : edges_v) {
        addEdge(e);
    }
}

string Graph::toString() {
    stringstream ss;
    for (auto& fromToPairs : edges) {
        for (auto& toWeightPair : fromToPairs.second) {
            Edge e = Edge(fromToPairs.first, toWeightPair.first, toWeightPair.second);
            ss << e.toString() << '\n';
        }
    }
    return ss.str();
}

void Graph::addEdge(Edge e) {
    u_int64_t from = e.getFrom(), to = e.getTo(), cnt = e.getWeight();
    if (edges.find(from) == edges.end()) {
        edges[from] = {};
    }
    if (edges[from].find(to) == edges[from].end()) {
        edges[from][to] = 0;
    }
    edges[from][to] += cnt;
}


void Graph::addEdge(optional<u_int64_t> from, optional<u_int64_t> to) {
    if (!from.has_value() || !to.has_value()) {
        return;
    }
    if (edges.find(from.value()) == edges.end()) {
        edges[from.value()] = {};
    }
    if (edges[from.value()].find(to.value()) == edges[from.value()].end()) {
        edges[from.value()][to.value()] = 0;
    }
    edges[from.value()][to.value()] += 1;
}
