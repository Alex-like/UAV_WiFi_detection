//
//  Graph.hpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 19.02.2022.
//

#ifndef Graph_hpp
#define Graph_hpp

#include <map>
#include <vector>
#include <string>
#include <optional>
#include "Edge.hpp"

using namespace std;

class Graph {
private:
    map<u_int64_t, map<u_int64_t, u_int64_t>> edges;
public:
    Graph(map<u_int64_t, map<u_int64_t, u_int64_t>> edges_v = {});
    Graph(vector<Edge> &edges_v);
    string toString();
    void addEdge(Edge e);
    void addEdge(optional<u_int64_t> from, optional<u_int64_t> to);
};

#endif /* Graph_hpp */
