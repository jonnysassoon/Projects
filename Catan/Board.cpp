//
//  Board.cpp
//  Catan
//
//  Created by Jonathan Sassoon on 11/27/17.
//  Copyright © 2017 Jonathan Sassoon. All rights reserved.
//

#include "Board.h"
#include <algorithm> // shuffle
#include <random> // random engine
#include <chrono> // seed val
using namespace std;

namespace Catan{
    
    TileType::TileType(string name, string resource) : name(name), resource(resource) {}
    
    Hill::Hill() : TileType("hill", "brick") {}
    
    Field::Field() : TileType("field", "wheat") {}
    
    Desert::Desert() : TileType("desert", "") {}
    
    CommTile::CommTile(string name, string resource, string commodity) : TileType(name, resource), commodity(commodity) {}
    
    Mountain::Mountain() : CommTile("mountain", "ore", "coin") {}
    
    Forest::Forest() : CommTile("forest", "wood", "paper") {}
    
    Pasture::Pasture() : CommTile("pasture", "sheep", "silk") {}
    
    Board::Board(int layer) : ConcentricGraph(layer){
        create_board();
    }
    
    Board::~Board() {
        for (int nodeNum = 1; nodeNum <= adj_list.size(); nodeNum++){
            unordered_map<Node*, std::vector<Node*>>& neighbor_list = adj_list[nodeNum];
            neighbor_list.begin()->second.clear(); // this won't be a memory leak because we will have deleted all of the nodes as keys
            delete neighbor_list.begin()->first;
        }
    }
    
    void Board::display() const {
        cout << "\nNODES MAP:\n";
        for (int nodeNum = 1; nodeNum <= total_nodes(); nodeNum++){
            const unordered_map<Board::Node*, std::vector<Board::Node*>>& neighbor_list = adj_list.at(nodeNum);
            cout << neighbor_list.begin()->first->int_id << ": ["; // output operator defined later. problem?
            for (size_t i = 0; i < neighbor_list.begin()->second.size(); i++) {
                cout << neighbor_list.begin()->second[i]->int_id;
                if (i != neighbor_list.begin()->second.size()-1) cout << ", ";
            }
            cout << "]";
            if (nodeNum != adj_list.size()) cout << "\n";
        }
        // TODO: I don't like the fact that the tiles print out of id order. consider changing how they're stored
        cout << "\n\nTILES MAP:\n";
        for (int num_tile = 2; num_tile <= 12; num_tile++) {
            if (num_tile != 7){
                const unordered_map<int, Tile*>& id_map = tiles_map.at(num_tile);
                for (pair<const int, Tile*> info: id_map) {
                    cout << info.first << ":["; // prints tile id number
                    const Tile& thisTile = *info.second;
                    for (size_t i = 0; i < thisTile.adj_nodes.size(); i++){ // prints all adjacent nodes to the tile
                        cout << thisTile.adj_nodes[i]->int_id;
                        if (i != thisTile.adj_nodes.size()-1) cout << ", ";
                    }
                    cout << "]\n";
                }
            }
        }
    }
    
    void Board::add_adjacencies(int node1_id, int node2_id){
        Node* node1 = adj_list[node1_id].begin()->first;
        Node* node2 = adj_list[node2_id].begin()->first;
        adj_list[node1->int_id][node1].push_back(node2);
        adj_list[node2->int_id][node2].push_back(node1);
    }
    
    // To think about: could I just do this recursively?
    void Board::generate_nodes(){ // keep track of all nodes according to their integer IDs
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
    void Board::add_node_to_tile(Tile* thisTile, int node_id) {
        Node* toAdd = adj_list[node_id].begin()->first;
        thisTile->adj_nodes.push_back(toAdd);
    }
    void Board::give_num_tile(Tile* the_tile, vector<int>& numbers_vec) { // TODO: handle 8s and 6s
        int num_tile = numbers_vec.back();
        the_tile->num_tile = num_tile;
        numbers_vec.pop_back();
    }
    void Board::log_tile(Tile* the_tile) {
        if (tiles_map.find(the_tile->num_tile) == tiles_map.end()) { // haven't logged it yet
            unordered_map<int, Tile*> id_map;
            id_map.emplace(the_tile->int_id, the_tile);
            tiles_map.emplace(the_tile->num_tile, id_map);
        } else {
            unordered_map<int, Tile*>& id_map = tiles_map[the_tile->num_tile];
            id_map.emplace(the_tile->int_id, the_tile);
        }
    }
    void Board::generate_tiles() {
        vector<int> num_tiles{2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12};
        vector<TileType*> types{new Hill(), new Hill(), new Hill(), new Mountain(), new Mountain(), new Mountain(), new Forest(), new Forest(), new Forest(), new Forest(), new Field(),new Field(), new Field(), new Field(), new Pasture(), new Pasture(), new Pasture(), new Pasture(), new Desert()};
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle(num_tiles.begin(), num_tiles.end(), default_random_engine(seed));
        shuffle(types.begin(), types.end(), default_random_engine(seed));
        int curr_layer = 1;
        int first_in_prev = 1;
        int last_in_prev = 6;
        int last_in_curr = 24;
        int fixed_last_in_curr = 24;
        int first_in_curr = 7;
        int total_in_prev = 6;
        int first_tile_in_curr = 2;
        int tiles_in_layer = 7;
        int set_counter = 0; // keeps track of your "location" in the set perscribing how many nodes you should add to the tile from each layer
        for (int tile_id = 1; tile_id < total_tiles() + 1; tile_id++){
            Tile* thisTile = new Tile(tile_id , types.back());
            types.pop_back();
            if (thisTile->type->name != "desert") give_num_tile(thisTile, num_tiles); // give it a number if it isn't desert hex
            if (tile_id == 1) {
                for (int i = 1; i < 7; i++) add_node_to_tile(thisTile, i);
            } else{
                if (tile_id > tiles_in_layer) { // reset stuff
                    first_in_prev += 12 * (curr_layer-1) + 6;
                    curr_layer++;
                    last_in_prev += 12 * (curr_layer-1) + 6;
                    fixed_last_in_curr += 12 * (curr_layer) + 6;
                    last_in_curr = fixed_last_in_curr;
                    first_in_curr = last_in_prev+1;
                    total_in_prev = last_in_prev;
                    first_tile_in_curr = tile_id;
                    tiles_in_layer += 6*curr_layer;
                    set_counter = 0;
                }
                for (int i = 0; i < 2; i++) {
                    add_node_to_tile(thisTile, last_in_curr);
                    last_in_curr--;
                }
                add_node_to_tile(thisTile, last_in_curr);
                add_node_to_tile(thisTile, last_in_prev);
                last_in_prev--;
                if (last_in_prev < first_in_prev) last_in_prev = total_in_prev;
                add_node_to_tile(thisTile, last_in_prev);
                if (tile_id == first_tile_in_curr) add_node_to_tile(thisTile, first_in_curr);
                else if (set_counter % curr_layer == 0) {
                    last_in_curr--;
                    add_node_to_tile(thisTile, last_in_curr);
                } else {
                    last_in_prev--;
                    if (last_in_prev < first_in_prev) last_in_prev = total_in_prev;
                    add_node_to_tile(thisTile, last_in_prev);
                }
                set_counter++;
            }
            log_tile(thisTile);
        }
    }
    
    void Board::create_board() {
        generate_nodes();
        generate_tiles();
    }
    
    Board::Edge::Edge(int int_id) : int_id(int_id) {}
    
    Board::Node::Node(int int_id) : int_id(int_id) {}
    
    Board::Tile::Tile(int int_id, TileType* type) : int_id(int_id),  blocked(false), type(type) {}

}
