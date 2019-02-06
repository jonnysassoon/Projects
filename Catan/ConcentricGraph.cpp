// Author: Jonny Sassoon
// Program: Concentric Graph
//
// Implementation of Concentric Graph Data

#include "ConcentricGraph.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <set>

using namespace std;


/*
 TODO: Put the create_board method in here. When you start the game, the Concentric graph creates the underlying map that relates the numbers, and the Board will iterate through the map and create nodes/tiles/edges accordingly.
 */

namespace Catan{
    ConcentricGraph::ConcentricGraph(int layers) : layers(layers), height(3+4*layers) {}
    int ConcentricGraph::nodes_in_layer(int layer) const { return 12 * layer + 6; }
    int ConcentricGraph::total_nodes() const { return 6*pow(layers, 2) + 12 *layers +6; } // 6n^2 + 12n +6
    int ConcentricGraph::total_nodes(int layer) const { return 6*pow(layer, 2) + 12 * layer + 6; }
    int ConcentricGraph::edges_in_layer(int layer) const { return 18 * layer + 6; }
    int ConcentricGraph::total_edges() const {return 9*pow(layers, 2) + 15*layers + 6; } // 9n^2 +15n +6
    int ConcentricGraph::total_edges(int layer) const { return 9*pow(layer, 2) + 15*layer + 6; }
    int ConcentricGraph::tiles_in_layer(int layer) const {
        if (layer == 0) return 1;
        return 6*layer;
    }
    int ConcentricGraph::total_tiles() const { return 3*pow(layers, 2) + 3*layers + 1; } // 3n^2 + 3n + 1
    int ConcentricGraph::total_tiles(int layer) const { return 3*pow(layer, 2) + 3*layer + 1; }
    int ConcentricGraph::nodes_on_level(int level) const { // not sure i need this function
        if (level < 0 || level > height-1){ // out of bounds
            cerr << "Level not in range.\n";
        }
        else if (level < layers) return 2 * (level + 1);
        else if (layers <= level && level <= (3 + 4 * layers) / 2) return int(2 * (layers + 1));
        return nodes_on_level(abs(4 * layers + 2 - level));
    }
    int ConcentricGraph::max_on_level() const { return 2*layers + 2; }
    int ConcentricGraph::levels_with_max() const { return 2 *layers + 3; }
    set<int> ConcentricGraph::second_degree_nodes(int layer) const {
        set<int> set;
        if (layer == 0) for (int val = 1; val < 7; val++) set.insert(val);
        else{
            int start_node = total_nodes(layer-1) + 1; // get the node after last node in previous ring, i.e. first node in this ring
            vector<int> group(layer, 2);
            group.push_back(1);
            for (int i = 0; i < group.size(); i++){
                if (i == 0) start_node += 1; // instead of jumping two, just jump one to start
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
}
