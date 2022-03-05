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

/**
 * Network structure.
 *
 * Decompose network to edges by transmitter and receiver addresses. Contains number of connection times between each other.
 */
class Graph {
private:
    /// Map to get number of connections by transmitter's and receiver's addresses.
    map<u_int64_t, map<u_int64_t, u_int64_t>> edges;
public:
    /**
     * Basic default constructor.
     *
     * @param edges_v network's connections with their number. If it's empty init by empty map.
     */
    Graph(map<u_int64_t, map<u_int64_t, u_int64_t>> edges_v = {});
    /**
     * Alternative constructor.
     *
     * @param edges_v reference to vector of edges.
     */
    Graph(vector<Edge> &edges_v);
    /**
     * Convert graph to string value.
     *
     * @return string representation of graph.
     */
    string toString();
    /**
     * Append connection.
     *
     * @param e edge equal one time of connection between transmitter and receiver.
     */
    void addEdge(Edge e);
    /**
     * Append connection.
     * @param from transmitter address in number value of MAC-address.
     * @param to receiver address in number value of MAC-address.
     */
    void addEdge(optional<u_int64_t> from, optional<u_int64_t> to);
};

#endif /* Graph_hpp */
