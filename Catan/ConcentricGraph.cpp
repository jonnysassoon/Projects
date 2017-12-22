// Author: Jonny Sassoon
// Program: Concentric Graph
//
// Implementation of Concentric Graph Data

#include "ConcentricGraph.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <unordered_set>
#include <unordered_map>

using namespace std;

namespace Catan{
    ConcentricGraph::ConcentricGraph(int layers) : layers(layers), height(3+4*layers), nodes(nodes_helper(layers)), edges(edges_helper(layers)), tiles(tiles_helper(layers)) { }
    int ConcentricGraph::nodes_in_layer(int layer) const { return 12 * layer + 6; }
    int ConcentricGraph::total_nodes() const { return nodes; }
    int ConcentricGraph::edges_in_layer(int layer) const { return 12 * layer + 6; }
    int ConcentricGraph::total_edges() const { return edges; }
    int ConcentricGraph::tiles_in_layer(int layer) const {
        if (layer == 0) return 1;
        return 6*layer;
    }
    int ConcentricGraph::total_tiles() const { return tiles; }
    int ConcentricGraph::nodes_on_level(int level) const { // not sure i need this function
        if (level < 0 || level > height-1){ // out of bounds
            cerr << "Level not in range.\n";
        }
        if (level < layers) return 2 * (level + 1);
        if (layers <= level && level <= (3 + 4 * layers) / 2) return int(2 * (layers + 1));
        return nodes_on_level(abs(4 * layers + 2 - level));
    }
    int ConcentricGraph::max_on_level() const { return 2*layers + 2; }
    int ConcentricGraph::levels_with_max() const { return 2 *layers + 3; }
    unordered_set<int> ConcentricGraph::second_degree_nodes(int layer) const {
        unordered_set<int> set;
        if (layer == 0) for (int val = 1; val < 7; val++) set.insert(val);
        else{
            int start_node = nodes_helper(layer-1) + 1; // get the node after last node in previous ring, i.e. first node in this ring
            vector<int> group(layer, 2);
            group.push_back(1);
            for (int i = 0; i < group.size(); i++){
                if (i == 0) start_node += 1; // instead of jumping to, just jump one to start
                else start_node += group[i];
                set.insert(start_node);
            }
            for (int i = 0; i < 5; i++){ // do the group five more times
                for (int step: group) {
                    start_node += step;
                    set.insert(start_node);
                }
            }
        }
        return set;
    }
    int ConcentricGraph::nodes_helper(int n) const {
        if (n == 0) return 6;
        return nodes_helper(n-1) + 12*n+6;
    }
    int ConcentricGraph::edges_helper(int n) const {
        if (n == 0) return 6;
        return edges_helper(n-1) + 18*n + 6;
    }
    int ConcentricGraph::tiles_helper(int n) const {
        if (n == 0) return 1;
        return tiles_helper(n-1) + 6*n;
    }
}
