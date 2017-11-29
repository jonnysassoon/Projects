// Author: Jonny Sassoon
// Program: Concentric Graph
//
// Implementation of Concentric Graph Data

#include "ConcentricGraph.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <unordered_set>
using namespace std;

namespace Catan{
    ConcentricGraph::ConcentricGraph(int layers) : layers(layers) {}
    int ConcentricGraph::exterior_nodes() const { return 12 * layers + 6; }
    int ConcentricGraph::total_nodes() const { return total_nodes_helper(layers); }
    int ConcentricGraph::exterior_edges() const { return 12*layers + 6; }
    int ConcentricGraph::total_edges() const { return total_edges_helper(layers); }
    int ConcentricGraph::exterior_tiles() const {
        if (layers == 0) return 1;
        return 6*layers;
    }
    int ConcentricGraph::total_tiles() const { return total_tiles_helper(layers); }
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
            int start_node = total_nodes_helper(layer-1) + 1; // get the node after last node in previous ring, i.e. first node in this ring
//            cout << "start_node = " << start_node << endl;
            vector<int> group(layer, 2);
            group.push_back(1);
//            cout << "jump set is: ";
//            for (int val : group) cout << val << ' ';
//            cout << endl;
            for (int i = 0; i < group.size(); i++){
                if (i == 0) start_node += 1; // instead of jumping to, just jump one to start
                else start_node += group[i];
//                cout << "adding " << start_node << " to set\n";
                set.insert(start_node);
            }
            for (int i = 0; i < 5; i++){ // do the group five more times
                for (int step: group) {
                    start_node += step;
//                    cout << "adding " << start_node << " to set\n";
                    set.insert(start_node);
                }
            }
        }
        return set;
    }
    int ConcentricGraph::total_nodes_helper(int n) const {
        if (n == 0) return 6;
        return total_nodes_helper(n-1) + 12*n+6;
    }
    int ConcentricGraph::total_edges_helper(int n) const {
        if (n == 0) return 6;
        return total_edges_helper(n-1) + 18*n + 6;
    }
    int ConcentricGraph::total_tiles_helper(int n) const {
        if (n == 0) return 1;
        return total_edges_helper(n-1) + 6*n;
    }
}
