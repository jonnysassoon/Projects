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
        for (int nodeNum = 1; nodeNum <= nodes_map.size(); nodeNum++){
            Node* toDelete = nodes_map[nodeNum];
            toDelete->adj_nodes.clear(); // not memory leak because we're deleting everything
            delete toDelete;
        }
    }
    
    void Board::display() const {
        cout << "\nNODES MAP:\n";
        for (int nodeNum = 1; nodeNum <= total_nodes(); nodeNum++){
            cout << *nodes_map.at(nodeNum) << endl;
        }
        cout << "\n\nTILES MAP:\n";
        for(int tileNum = 1; tileNum <= total_tiles(); tileNum++){
            cout << tileNum << ":{" << *tiles_map.at(tileNum) << "}\n";
        }
    }
    void Board::link_nodes(Node& node1, Node& node2){
        node1.adj_nodes.emplace(&node2);
        node2.adj_nodes.emplace(&node1);
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
        for (int idNum = 1; idNum <= total_nodes(); idNum++){
            Node* thisNode = new Node(idNum);
            unordered_set<Node*> neighbors;
            nodes_map.emplace(idNum, thisNode);
            if (idNum == nodes_after_layer+1) { // we're in a new layer of the board, need to reset values
                curr_layer += 1;
                if (idNum != 7) two_deg_in_prev = first_in_layer + 1; // the first time we enter a new layer, we don't want to adjust two_deg_in_prev since its initialized for this case
                first_in_layer = nodes_after_layer + 1;
                nodes_after_layer += 12 * curr_layer + 6; // implicitly does recursive work for ConcetricGraph.total_nodes()
                degree_two_nodes = second_degree_nodes(curr_layer);
                last_third_degree = nullptr;
            }
            if (idNum > 1){ // conect current to one behind it
                link_nodes(*thisNode, *nodes_map[previous_node]);
                previous_node++;
            }
            if (idNum == nodes_after_layer) link_nodes(*thisNode, *nodes_map[first_in_layer]); // if at end of layer, conenct to first in layer
            if (degree_two_nodes.find(idNum) == degree_two_nodes.end() &&
                idNum != first_in_layer) { // connect a third degree node that isn't the first node in the layer (it's already conencting to its proper node in previous layer) to its proper 2nd degree node in the previous layer
                if (last_third_degree != nullptr) { // if it is nullptr, we didn't jump to get here
                    int jump = idNum - last_third_degree->int_id;
                    two_deg_in_prev += 4 - jump; // the sum of the jumps of the third degree nodes in curr_layer and the 2nd degree nodes in the prev layer always == 4...why?
                }
                link_nodes(*thisNode, *nodes_map[two_deg_in_prev]);
                last_third_degree = thisNode;
            }
        }
    }
    void Board::add_node_to_tile(Tile* thisTile, int node_id) {
        Node* toAdd = nodes_map[node_id];
        thisTile->adj_nodes.emplace(toAdd);
    }
    void Board::give_num_tile(Tile* the_tile, vector<int>& numbers_vec) { // TODO: handle 8s and 6s
        int num_tile = numbers_vec.back();
        the_tile->num_tile = num_tile;
        numbers_vec.pop_back();
    }
    
    void Board::link_tiles(Tile& tile1, Tile& tile2) {
        tile1.adj_tiles.emplace(&tile2);
        tile2.adj_tiles.emplace(&tile1);
    }
    void Board::generate_tiles() {
        vector<int> num_tiles{2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12};
        vector<TileType*> types{new Hill(), new Hill(), new Hill(), new Mountain(), new Mountain(), new Mountain(), new Forest(), new Forest(), new Forest(), new Forest(), new Field(),new Field(), new Field(), new Field(), new Pasture(), new Pasture(), new Pasture(), new Pasture(), new Desert()};
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle(num_tiles.begin(), num_tiles.end(), default_random_engine(seed));
        shuffle(types.begin(), types.end(), default_random_engine(seed));
        // TODO: utilize the methods in ConcentricGraph to reduce the number of tracker variables
        // TODO: reduce the code...
        /* Tracker variables for Tile-Node links: */
        int currLayer = 0;
        int firstNodeInPrev = 0; // TODO: should be able to use a ConcentricGraph method instead of variable. reference to the first node in the previous layer
        int lastNodeInCurr = 6; // reference to last node in curent layer
        int firstNodeInCurr = 1; // reference to first node in the current layer
        int totalNodesInPrev = 0; // value of total nodes in the previous layer
        int firstTileInCurr = 1; // first tile in current layer
        int tilesAfterCurr = 1; // changed from 7 to 1. equivalent to totTilesInCurr -> understand its initialization logic
        int nodeInPrev = 0; // keeps track of the node in n-1 that we want to add to nodes in n
        int nodeInCurr = 6; // node from this layer we want to add to the tile
        int setCounter = 0; // keeps track of your "location" in the set which perscribes how many nodes you should add to the tile from each layer
        
        /* Tracker variables for Tile-Tile links: */
        int prevTile = 0;
        int inPrevLayer = 0; // tile we are trying to add from the previous layer
        int connectionTicker = 0; // keeps track of where you are relative to a corner tile
        
        for (int tile_id = 1; tile_id < total_tiles() + 1; tile_id++){
            Tile* thisTile = new Tile(tile_id , types.back());
            types.pop_back();
            if (thisTile->type->name != "desert") give_num_tile(thisTile, num_tiles); // give it a number if it isn't desert hex
            if (tile_id > tilesAfterCurr) { // reset stuff
                cout << "before resetting, the nodeInCurr was reduced to: " << nodeInCurr << endl;
                currLayer++;
                firstNodeInPrev = totalNodesInPrev+1; // at this point, we haven't reset total in prev, so 1 above this is the first in the layer after the layer totalNodesInPrev is referring to, which is the previous layer from the one we are in now.
                nodeInPrev = lastNodeInCurr;
                lastNodeInCurr += nodes_in_layer(currLayer);
                nodeInCurr = lastNodeInCurr;
                firstNodeInCurr = nodeInPrev+1;
                totalNodesInPrev = nodeInPrev;
                firstTileInCurr = tile_id;
                tilesAfterCurr += 6*currLayer;
                setCounter = 0;
                inPrevLayer = currLayer; // the way we created the board, the id number of the first tile in the previous layer == the value for currLayer
                connectionTicker = 0;
            }
            if (tile_id == 1) {
                for (int i = 1; i < 7; i++) add_node_to_tile(thisTile, i);
            } else{
                for (int i = 0; i < 2; i++) { // every tile has >= 3 nodes from n-1 in their adj_nodes. for two of these, we need to decrement nodeInCurr
                    add_node_to_tile(thisTile, nodeInCurr);
                    nodeInCurr--;
                }
                add_node_to_tile(thisTile, nodeInCurr); // this is the 3rd one, we just don't decrement nodeInCurr because we need to keep it the same for the next tile...
                add_node_to_tile(thisTile, nodeInPrev);
                nodeInPrev--;
                if (nodeInPrev < firstNodeInPrev) nodeInPrev = totalNodesInPrev;
                add_node_to_tile(thisTile, nodeInPrev);
                if (tile_id == firstTileInCurr) add_node_to_tile(thisTile, firstNodeInCurr);
                else if (setCounter % currLayer == 0) { // ...unless we are on a corner tile, which requires an additional node
                    nodeInCurr--;
                    add_node_to_tile(thisTile, nodeInCurr);
                } else {
                    nodeInPrev--;
                    if (nodeInPrev < firstNodeInPrev) nodeInPrev = totalNodesInPrev;
                    add_node_to_tile(thisTile, nodeInPrev);
                }
                setCounter++;
                if (tile_id != firstTileInCurr) link_tiles(*thisTile, *tiles_map[prevTile]); // you don't do this if it's the first in curr because it isn't adjacent to the previous tile (8 does not border 7, etc.), it's equivalent "previous" tile is the inPrevLayer tile (8 links to 2)
                link_tiles(*thisTile, *tiles_map[inPrevLayer]);
                if (connectionTicker % 2 != 0){
                    inPrevLayer++;
                    if (inPrevLayer == firstTileInCurr) inPrevLayer = currLayer;
                    link_tiles(*thisTile, *tiles_map[inPrevLayer]);
                    connectionTicker++;
                    if (connectionTicker == 2 * currLayer - 1) connectionTicker = 0; // reset this
                }
                if (tile_id == tilesAfterCurr) {
                    link_tiles(*thisTile, *tiles_map[firstTileInCurr]);
                }
                connectionTicker++;
                if (connectionTicker == 2 * currLayer - 1) connectionTicker = 0; // reset this
            }
            prevTile = tile_id;
            tiles_map[tile_id] = thisTile; // log the tile we just worked on
        }
    }
    
    void Board::create_board() {
        generate_nodes();
        generate_tiles();
    }
    
    Board::Edge::Edge(int int_id) : int_id(int_id) {}
    
    Board::Node::Node(int int_id) : int_id(int_id), /* knight(nullptr), owner(nullptr), */ has_settlement(false), has_city(false) {}
    
    Board::Tile::Tile(int int_id, TileType* type) : int_id(int_id),  blocked(false), type(type) {}
    
    ostream& operator<<(ostream& os, const Board::Node& rhs){
        os << rhs.int_id << ":[";
        int iter = 0;
        for (Board::Node* nodeptr : rhs.adj_nodes) {
            cout << nodeptr->int_id;
            if (iter != rhs.adj_nodes.size()-1) cout << ", ";
            iter++;
        }
        cout << "]";
        return os;
    }
    
    ostream& operator<<(ostream& os, const Board::Tile& rhs) {
        const unordered_set<Board::Node*>& nodes_set = rhs.adj_nodes;
        int iter = 0;
        os << "Nodes:[";
        for (Board::Node* nodeptr : nodes_set) {
            os << nodeptr->int_id;
            if (iter != nodes_set.size()-1) os << ", ";
            iter++;
        }
        os << "], Tiles:[";
        const unordered_set<Board::Tile*>& tiles_set = rhs.adj_tiles;
        iter = 0;
        for (Board::Tile* tileptr: tiles_set){
            os << tileptr->int_id;
            if (iter != tiles_set.size()-1) os << ", ";
            iter++;
        }
        os << ']';
        return os;
    }
}
