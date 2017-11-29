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
    class Node;
    class Tile;
    
    
    Board::Board(int layer) : ConcentricGraph(layer){
        create_board();
    }
    
    void Board::add_adjacencies(int node1_id, int node2_id){
        Node* node1 = adj_list[node1_id].begin()->first;
        Node* node2 = adj_list[node2_id].begin()->first;
        adj_list[node1->int_id][node1].push_back(node2);
        adj_list[node2->int_id][node2].push_back(node1);
    }
    
    void Board::create_adj_lst(){ // keep track of all nodes according to their integer IDs
        int curr_layer = 0;
        int first_in_layer = 1;
        int nodes_after_layer = 6;
        int two_deg_in_prev = 1;
        unordered_set<int> degree_two_nodes = second_degree_nodes(0);
        Node* last_third_degree = nullptr;
        int previous_node = 1;
        for (int idNum = 1; idNum < total_nodes() +1; idNum++){
            Node* thisNode = new Node(idNum);
            vector<Node*> neighbors;
            unordered_map<Node*, vector<Node*>> node_dat;
            node_dat.emplace(thisNode, neighbors);
            adj_list.emplace(idNum, node_dat);
            if (idNum == nodes_after_layer+1) {
                curr_layer += 1;
                if (idNum != 7) two_deg_in_prev = first_in_layer + 1;
                first_in_layer = nodes_after_layer + 1;
                nodes_after_layer += 12 * curr_layer + 6;
                degree_two_nodes = second_degree_nodes(curr_layer);
                last_third_degree = nullptr;
            }
            if (idNum > 1){
                add_adjacencies(idNum, previous_node);
                previous_node++;
            }
            if (idNum == nodes_after_layer) add_adjacencies(idNum, first_in_layer);
            if (degree_two_nodes.find(idNum) == degree_two_nodes.end()
                && idNum != first_in_layer) {
                if (last_third_degree != nullptr) {
                    int jump = idNum - last_third_degree->int_id;
                    two_deg_in_prev += 4 - jump;
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
            unordered_map<Node*, std::vector<Node*>> neighbor_list = rhs.adj_list[nodeNum];
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
