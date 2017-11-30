//
//  Board.cpp
//  Catan
//
//  Created by Jonathan Sassoon on 11/27/17.
//  Copyright © 2017 Jonathan Sassoon. All rights reserved.
//

#include "Board.h"
using namespace std;

namespace Catan{
    struct Node;
    struct Tile;
    
    
    Board::Board(int layer) : ConcentricGraph(layer){
        create_board();
    }
    
    Board::~Board() {
        for (size_t nodeNum = 1; nodeNum <= adj_list.size(); nodeNum++){
            unordered_map<Node*, std::vector<Node*>>& neighbor_list = adj_list[nodeNum];
            neighbor_list.begin()->second.clear(); // this won't be a memory leak because we will have deleted all of the nodes as keys
            delete neighbor_list.begin()->first;
        }
    }
    
    void Board::add_adjacencies(int node1_id, int node2_id){
        Node* node1 = adj_list[node1_id].begin()->first;
        Node* node2 = adj_list[node2_id].begin()->first;
        adj_list[node1->int_id][node1].push_back(node2);
        adj_list[node2->int_id][node2].push_back(node1);
    }
    
    // To think about: could I just do this recursively?
    void Board::create_adj_lst(){ // keep track of all nodes according to their integer IDs
        int curr_layer = 0;
        int first_in_layer = 1;
        int nodes_after_layer = 6;
        int two_deg_in_prev = 1; // keeps track of the current 2nd degree node from the previous layer that we will need to link to its corresponding third degree node in curr_layer
        unordered_set<int> degree_two_nodes = second_degree_nodes(0);
        Node* last_third_degree = nullptr;
        int previous_node = 1;
        for (int idNum = 1; idNum < total_nodes() +1; idNum++){
            Node* thisNode = new Node(idNum);
            vector<Node*> neighbors;
            unordered_map<Node*, vector<Node*>> node_dat;
            node_dat.emplace(thisNode, neighbors);
            adj_list.emplace(idNum, node_dat);
            if (idNum == nodes_after_layer+1) { // we're in a new layer of the board, need to reset values
                curr_layer += 1;
                if (idNum != 7) two_deg_in_prev = first_in_layer + 1; // the first time we enter a new layer, we don't want to adjust two_deg_in_prev since its initialized for this case
                first_in_layer = nodes_after_layer + 1;
                nodes_after_layer += 12 * curr_layer + 6; // implicitly does recursive work for ConcetricGraph.total_nodes()
                degree_two_nodes = second_degree_nodes(curr_layer);
                last_third_degree = nullptr;
            }
            if (idNum > 1){ // conect current to one behind it
                add_adjacencies(idNum, previous_node);
                previous_node++;
            }
            if (idNum == nodes_after_layer) add_adjacencies(idNum, first_in_layer); // if at end of layer, conenct to first in layer
            if (degree_two_nodes.find(idNum) == degree_two_nodes.end() &&
                idNum != first_in_layer) { // connect a third degree node that isn't the first node in the layer (it's already conencting to its proper node in previous layer) to its proper 2nd degree node in the previous layer
                if (last_third_degree != nullptr) { // if it is nullptr, we didn't jump to get here
                    int jump = idNum - last_third_degree->int_id;
                    two_deg_in_prev += 4 - jump; // the sum of the jumps of the third degree nodes in curr_layer and the 2nd degree nodes in the prev layer always == 4...why?
                }
                add_adjacencies(idNum, two_deg_in_prev);
                last_third_degree = thisNode;
            }
        }
    }
    
    void Board::create_board() {
        create_adj_lst();
    }

    
    Edge::Edge(int int_id) : int_id(int_id) {}
    
    Node::Node(int int_id) : int_id(int_id) {}
    
    ostream& operator<<(ostream& os, const Node& rhs) {
        cout << rhs.int_id;
        return os;
    }
    
    Tile::Tile(int int_id) : int_id(int_id) {}
    
    ostream& operator<<(ostream& os, Board& rhs) {
        for (size_t nodeNum = 1; nodeNum <= rhs.adj_list.size(); nodeNum++){
            const unordered_map<Node*, std::vector<Node*>>& neighbor_list = rhs.adj_list[nodeNum];
            cout << neighbor_list.begin()->first->int_id << ": [";
            for (size_t i = 0; i < neighbor_list.begin()->second.size(); i++) {
                cout << neighbor_list.begin()->second[i]->int_id;
                if (i != neighbor_list.begin()->second.size()-1) cout << ", ";
            }
            cout << "]";
            if (nodeNum != rhs.adj_list.size()) cout << "\n";
        }
        return os;
    }

}
