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
#include <queue> // canMoveKnight(), getRoadLength()
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
        for (int edgeNum = 1; edgeNum <= total_edges(); edgeNum++) {
            Edge* toDelete = edges_map[edgeNum];
            delete toDelete;
            toDelete = nullptr;
        }
        for (int nodeNum = 1; nodeNum <= total_nodes(); nodeNum++) {
            Node* toDelete = nodes_map[nodeNum];
            delete toDelete;
            toDelete = nullptr;
        }
        for (int tileNum = 1; tileNum <= total_tiles(); tileNum++) {
            Tile* toDelete = tiles_map[tileNum];
            delete toDelete;
            toDelete = nullptr;

        }
        nodes_map.clear();
        edges_map.clear();
        tiles_map.clear();
        num_tiles.clear();
    }
    
    void Board::display() const {
        cout << "\nNODES MAP:\n";
        for (int nodeNum = 1; nodeNum <= total_nodes(); nodeNum++){
            cout << *nodes_map.at(nodeNum) << endl;
        }
        cout << "\n\nTILES MAP:\n";
        for(int tileNum = 1; tileNum <= total_tiles(); tileNum++){
            cout << tileNum << " - " << tiles_map.at(tileNum)->type << ":{" << *tiles_map.at(tileNum) << "}\n";
        }
    }
    
    void Board::dispSetCit() const {
        for (auto iter = nodes_map.begin(); iter != nodes_map.end(); iter++) {
            Node* node = iter->second;
            if (node->settlement != nullptr ){
                cout << "Settlement is located at position " << iter->first << " and is owned by " << node->owner->getName() << endl;
            }
            else if (node->city != nullptr) {
                cout << "City is located at position " << iter->first << " and is owned by " << node->owner->getName();
                if (node->city->metropolis != nullptr) {
                    Metropolis* met = node->city->metropolis;
                    cout << " and has the ";
                    if (met->color == "green") {
                        cout << " science ";
                    } else if (met->color == "blue"){
                        cout << " politics ";
                    } else {
                        cout << " trade ";
                    }
                    cout << "metropolis.";
                }
                cout << endl;
            }
        }
    }
    
    void Board::distributeResources(int roll) const {
        set<Tile*> tiles = num_tiles.at(roll); // this will only have stuff in it if a settlement/city was built on it
        for (Tile* tile : tiles) {
            if (!tile->blocked) {
                for (Node* node : tile->adj_nodes) {
                    if (node->owner != nullptr && node->knight == nullptr) { // if it's owned, but there's no knight (i.e. there's a settlement/city)
                        node->owner->collectResource(tile->resource); // collect at least one
                        if (node->city != nullptr) {
                            if (tile->resource == "wheat" || tile->resource == "brick") {
                                node->owner->collectResource(tile->resource);
                            } else {
                                node->owner->collectResource(tile->commodity);
                            }
                        }
                    }
                }
            }
        }
    }
    
    vector<int> Board::getAdjEdges(int loc){
        vector<int> edge_ids;
        Node* node = nodes_map.at(loc);
        for (Edge* edge : node->adj_edges) edge_ids.push_back(edge->int_id);
        return edge_ids;
    }
    
    int Board::getKnightLevel(int loc) const {
        if (loc < 1 || loc > total_nodes()) return -1;
        Node* node = nodes_map.at(loc);
        if (node->knight == nullptr) return -1;
        return node->knight->strength;
    }
    
    int Board::getRobberLoc() const { return robberLoc; }
    
    int Board::getRoadLength(Edge* edge) {
        /*
          _/
         / \_/
         \_/
         
         Want to find the max euler path for the system that this road is a part of
         1. find all odd degree nodes that are reachable by our roads
         2. for each one, get the euler path by doing a DFS that traverses all connected, unvisited edges
         3. take the max
         */
        // BFS for all odd degree nodes to search for largest Euler path
        set<Node*> odd_degs;
        set<Node*> visited;
        queue<Node*> next_q;
        Player* owner = edge->owner;
        for (Node* node : edge->adj_nodes) {
            if (node->owner == owner || node->owner == nullptr) next_q.push(node); // initialize the qeueu to have all nodes that border this edge and are owned by either the same person that built the edge or no one.
        }
        while (!(next_q.size() == 0)) {
            Node* curr = next_q.front();
            next_q.pop();
            visited.insert(curr);
            for (Edge* incEdge : curr->adj_edges) { // go to other nodes through the edges that stem from current node
                if (incEdge->owner == owner) { // exclusively if we have OUR own path to some other node
                    for (Node* aNode : incEdge->adj_nodes) {
                        if (visited.find(aNode) == visited.end() && // if we have not yet seen this node
                            (aNode->owner == nullptr || aNode->owner == owner)) // and it's also not owned by someone else
                        {
                            next_q.push(aNode);
                        }
                    }
                }
            }
            int deg = 0;
            for (Edge* anEdge : curr->adj_edges) if (anEdge->owner == owner) deg++; // check the number of our edges the node connects to
            if (deg % 2 == 1) odd_degs.insert(curr);
        }
        int max_path = -1;
        for (Node* node : odd_degs) { // i feel like there's *a lot* of redundancy checking every odd degree node
            set<Edge*> visited;
            int len = pathLength(node, owner, visited);
            if (len > max_path) max_path = len;
        }
        if (odd_degs.size() == 0) max_path = 6; // the only time this is possible is if the road completed a single hex cycle
        return max_path;
    }
    
    int Board::pathLength(Node* node, Player* owner, set<Edge*>& visited) {
        set<Edge*> edges = node->adj_edges;
        int max_len = 0;
        for (Edge* edge : edges) {
            if (visited.find(edge) == visited.end() && edge->owner == owner) { // if the edge hasn't been seen AND it's valid for our chain
                visited.insert(edge);
                Node* next;
                Node* first = *(edge->adj_nodes.begin());
                Node* second = *(edge->adj_nodes.begin()++);
                if (first != node) next = first;
                else next = second; // get the other node that edge links to that isn't this node
                int len = pathLength(next, owner, visited) + 1; // the length of this chain is the length of the adj node + 1
                if (len > max_len) max_len = len;
            }
        }
        return max_len;
    }
    
    Metropolis* Board::removeMetropolis(int loc) {
        Metropolis* met = nodes_map[loc]->city->metropolis;
        nodes_map[loc]->city->metropolis = nullptr;
        return met;
    }
    
    bool Board::isValidSetLoc(int loc, Player* player, bool firstTurn) const {
        if (loc < 1 || loc > total_nodes()) return false;
        Node* node = nodes_map.at(loc);
        if (node->owner == nullptr) {
            for (Node* nodptr : node->adj_nodes) {
                if (nodptr->city != nullptr || nodptr->settlement != nullptr) return false; // of the adjacent locations, one of them is already taken with a city/settlement
            }
            if(!firstTurn) {
                for (Edge* edgptr : node->adj_edges) {
                    if (edgptr->owner == player) return true;
                }
            }
            return true; // if it is the first turn, it is valid automatically
        }
        return false;
    }
    
    bool Board::isValidCityLoc(int loc, Player* player) const {
        if (loc < 1 || loc > total_nodes()) return false;
        Node* node = nodes_map.at(loc);
        return node->owner == player && node->settlement != nullptr; // the node *is* owned by this player, and there *is* a settlement here
    }
    
    bool Board::isValidRoadLoc(int loc, Player* player) const {
        if (loc < 1 || loc > total_edges()) return false;
        Edge* edge = edges_map.at(loc);
        if (edge->owner == nullptr) { // nothing is here
            for (Node* nodptr : edge->adj_nodes) {
                if (nodptr->owner == player) return true; // a connecting node is owned by this player
                if (nodptr->owner == nullptr) { // if the node is vacant, we have to check to see if it has an outgoing edge that's owned by this person
                    for (Edge* neighborEdge : nodptr->adj_edges) {
                        if (neighborEdge->owner == player) return true;
                    }
                }
            }
        }
        return false;
    }
    
    bool Board::isValidFirstRoadLoc(int roadLoc, int settLoc) const { // this road has to immediatley border the settlement/city that it was built after
        if (roadLoc < 1 || roadLoc > total_edges()) return false;
        Edge* edge = edges_map.at(roadLoc);
        for (Node* nodptr : edge->adj_nodes) if (nodptr->int_id == settLoc) return true;
        return false;
    }
    
    bool Board::isValidKnightLoc(int loc, Player* player, bool upgrade) const { // the logic is difference if this is for an upgrade
        if (loc < 1 || loc > total_nodes()) return false;
        Node* node = nodes_map.at(loc);
        if (!upgrade){
            if (node->owner == nullptr) {
                for (Edge* edgptr : node->adj_edges) {
                    if (edgptr->owner == player) return true;
                }
            }
        } else {
            return node->owner == player && node->knight != nullptr && node->knight->strength < 3;
        }
        return false;
    }
    
    bool Board::isValidWallLoc(int loc, Player* player) const {
        if (loc < 1 || loc > total_nodes()) return false;
        Node* node = nodes_map.at(loc);
        return node->city != nullptr && node->city->owner == player && node->city->wall == false;
    }
    
    bool Board::isValidRobberLoc(int loc) const {
        return (loc >=1 && loc <= total_tiles()) && loc != robberLoc; // it's within tile range, and the robber isn't there
    }
    
    bool Board::isOpenCityLoc(int loc, Player* player) const {
        if (loc < 1 || loc > total_nodes()) return false;
        Node* node = nodes_map.at(loc);
        return node->city != nullptr && node->city->owner == player && node->city->metropolis == nullptr;
    }
    
    bool Board::canActivateKnight(int loc, Player* player) const {
        if (loc < 1 || loc > total_nodes()) return false;
        Node* node = nodes_map.at(loc);
        return node->knight != nullptr && node->knight->owner == player && !node->knight->activated; // there's an inactive knight that's owned by the player on this location
    }
    
    bool Board::canDeactivateKnight(int loc, Player* player) const {
        if (loc < 1 || loc > total_nodes()) return false;
        Node* node = nodes_map.at(loc);
        return node->knight != nullptr && node->knight->owner == player && node->knight->activated && !node->knight->activeThisRound; // there's an active knight that's owned by this player that was not activated this round
    }
    
    bool Board::canMoveKnight(Player* player, int source, int destination) {
        if (source < 1 || source > total_nodes()) return false;
        if (destination < 1 || destination > total_nodes()) return false;
        Node* sourceNode = nodes_map.at(source);
        Node* destNode = nodes_map.at(destination);
        if (destNode->owner != nullptr) return false; // there's already something here
        /* BFS for the destination */
        set<Node*> visited;
        queue<Node*> nodeQ({sourceNode});
        while (nodeQ.size() != 0) {
            Node* curr = nodeQ.front();
            nodeQ.pop();
            visited.emplace(curr);
            if (curr == destNode) return true; // we have a valid path
            for (Edge* edge : curr->adj_edges) {
                if (edge->owner == player) {
                    for (Node* node : edge->adj_nodes) {
                        if (visited.find(node) == visited.end()) nodeQ.push(node);
                    }
                }
            }
        }
        return false;
    }
    
    bool Board::nodeIsNextToTile(int nodeLoc, int tileLoc) const {
        if (nodeLoc < 1 || nodeLoc > total_nodes() || tileLoc < 1 || tileLoc > total_tiles()) return false; // just a precaution, shouldn't ever be necessary
        Node* node = nodes_map.at(nodeLoc);
        for (Tile* tileptr : node->adj_tiles) {
            if (tileptr->int_id == tileLoc) return true;
        }
        return false;
    }
    
    void Board::placeSettlement(int loc, Settlement* settlement) {
        Node* node = nodes_map[loc];
        node->settlement = settlement;
        node->owner = settlement->owner;
        for (Tile* tileptr : node->adj_tiles) {
            num_tiles[tileptr->num_tile].emplace(tileptr); // include this tile in the set of tiles that its number maps to
        }
    }
    
    int Board::placeRoad(int loc, Player* player) {
        Edge* edge = edges_map[loc];
        edge->owner = player;
        return getRoadLength(edge);
    }
    
    void Board::placeCity(int loc, City* city, bool setUp) {
        Node* node = nodes_map[loc];
        Settlement* oldSet = node->settlement;
        node->settlement = nullptr;
        delete oldSet;
        node->city = city;
        node->owner = city->owner; // in case of first turn where owner isn't assigned. otherwise, this is redundant
        if (setUp) { // give player resources of neighboring hex tiles
            for (Tile* tileptr : node->adj_tiles) {
                num_tiles[tileptr->num_tile].emplace(tileptr); // this is only necessary during setup, otherwise, you would  already have a settlement there, therefore it would be in the map.
            }
            Player* player = city->owner;
            for (Tile* tilptr : node->adj_tiles)player->collectResource(tilptr->resource);
        }
    }
    
    void Board::removeCity(int loc) {
        Node* node = nodes_map[loc];
        City* city = node->city;
        node->city = nullptr;
        delete city;
    }
    
    void Board::placeWall(int loc) {
        Node* node = nodes_map[loc];
        node->city->wall = true;
    }
    
    void Board::placeKnight(int loc, Knight* knight) {
        Node* node = nodes_map[loc];
        if (node->knight != nullptr) delete node->knight;
        node->knight = knight;
        node->owner = knight->owner;
    }
    
    void Board::placeMetropolis(int loc, Metropolis* met) {
        Node* node = nodes_map[loc];
        node->city->metropolis = met;
    }
    
    void Board::activateKnight(int loc) {
        Node * node = nodes_map[loc];
        node->knight->activated = true;
        node->knight->activeThisRound = true;
    }
    
    void Board::deactivateKnight(int loc) {
        Node* node = nodes_map[loc];
        node->knight->activated = false;
    }
    
    void Board::moveKnight(int source, int destination) {
        Node* sourceNode = nodes_map.at(source);
        Node* destNode = nodes_map.at(destination);
        Knight* knight = sourceNode->knight;
        sourceNode->knight = nullptr;
        sourceNode->owner = nullptr;
        destNode->knight = knight;
        destNode->owner = knight->owner;
    }
    
    void Board::removeSummonSickness(int loc) {
        Node* node = nodes_map.at(loc);
        Knight* knight = node->knight;
        knight->activeThisRound = false;
    }
    
    set<Player*> Board::placeRobber(int newLoc) {
        Tile* prevTile = tiles_map[robberLoc];
        prevTile->blocked = false;
        Tile* newTile = tiles_map[newLoc];
        newTile->blocked = true;
        robberLoc  = newLoc;
        set<Player*> toRob;
        for (Node* node : newTile->adj_nodes) {
            if (node->owner != nullptr && node->knight == nullptr) toRob.emplace(node->owner);
        }
        return toRob;
    }
    
    void Board::link_nodes(int& edge_id, Node& node1, Node& node2){
        Edge* edge(new Edge(edge_id));
        edges_map[edge_id] = edge;
//        cout << "Now adding edge " << edge_id << " to nodes " << node1.int_id << " and " << node2.int_id << endl;
//        cout << "Edge address: " << edge << endl;
        node1.adj_nodes.insert(&node2);
        node1.adj_edges.insert(edge);
        edge->adj_nodes.insert(&node1);
        node2.adj_nodes.insert(&node1);
        node2.adj_edges.insert(edge);
        edge->adj_nodes.insert(&node2);
        edge_id++;
    }
    
    // could I just do this recursively?
    void Board::generate_nodes(){ // keep track of all nodes according to their integer IDs
        int curr_layer = 0;
        int first_in_layer = 1;
        int nodes_after_layer = 6;
        int two_deg_in_prev = 1; // keeps track of the current 2nd degree node from the previous layer that we will need to link to its corresponding third degree node in curr_layer
        set<int> degree_two_nodes = second_degree_nodes(0);
        Node* last_third_degree = nullptr;
        int previous_node = 1;
        int edge_id = 1;
        for (int idNum = 1; idNum <= total_nodes(); idNum++){
            Node* thisNode = new Node(idNum);
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
                link_nodes(edge_id, *thisNode, *nodes_map[previous_node]);
                previous_node++;
            }
            if (idNum == nodes_after_layer) link_nodes(edge_id, *thisNode, *nodes_map[first_in_layer]); // if at end of layer, conenct to first in layer
            if (degree_two_nodes.find(idNum) == degree_two_nodes.end() &&
                idNum != first_in_layer) { // connect a third degree node that isn't the first node in the layer (it's already conencting to its proper node in previous layer) to its proper 2nd degree node in the previous layer
                if (last_third_degree != nullptr) { // if it is nullptr, we didn't jump to get here
                    int jump = idNum - last_third_degree->int_id;
                    two_deg_in_prev += 4 - jump; // the sum of the jumps of the third degree nodes in curr_layer and the 2nd degree nodes in the prev layer always == 4...why?
                }
                link_nodes(edge_id, *thisNode, *nodes_map[two_deg_in_prev]);
                last_third_degree = thisNode;
            }
        }
    }
    void Board::link_node_to_tile(Tile* thisTile, int node_id) {
        Node* toAdd = nodes_map[node_id];
        thisTile->adj_nodes.emplace(toAdd);
        toAdd->adj_tiles.emplace(thisTile);
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
    void Board::generate_tiles() { // TODO: make give an option for random, default it to fixed.
        vector<int> num_tiles{2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12};
        vector<TileType*> types{new Hill(), new Hill(), new Hill(), new Mountain(), new Mountain(), new Mountain(), new Forest(), new Forest(), new Forest(), new Forest(), new Field(),new Field(), new Field(), new Field(), new Pasture(), new Pasture(), new Pasture(), new Pasture(), new Desert()};
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle(num_tiles.begin(), num_tiles.end(), default_random_engine(seed));
        shuffle(types.begin(), types.end(), default_random_engine(seed));
        // TODO: utilize the methods in ConcentricGraph to reduce the number of variables
        // TODO: is this over engineering? see if I can reduce the code...
        // TODO: see if I can make separate functions addNodesToTile/addTilestoTile that could compartmentalize this function and make it cleaner
        /* Variables for Tile-Node links: */
        int currLayer = 0;
        int lastNodeInCurr = 6; // TODO: should be able to use a ConcentricGraph method instead of variable. reference to last node in curent layer
        int firstNodeInCurr = 1; // TODO: should be able to use a ConcentricGraph method instead of variable. reference to first node in the current layer
        int totalNodesInPrev = 0; // TODO: should be able to use a ConcentricGraph method instead of variable. total nodes in the previous layer
        int firstTileInCurr = 1; // TODO: should be able to use a ConcentricGraph method instead of variable. reference to first tile in current layer
        int tilesAfterCurr = 1; // TODO: should be able to use a ConcentricGraph method instead of variable.
        int nodeInPrev = 0; // keeps track of the node in n-1 that we want to add to nodes in n
        int nodeInCurr = 6; // node from this layer we want to add to the tile
        int setCounter = 0; // keeps track of your "location" in the set which perscribes how many nodes you should add to the tile from each layer
        
        /* Variables for Tile-Tile links: */
        int prevTile = 0;
        int inPrevLayer = 0; // tile we are trying to add from the previous layer
        int connectionTicker = 0; // keeps track of where you are relative to a corner tile
        
        for (int tile_id = 1; tile_id < total_tiles() + 1; tile_id++){
            TileType* used = types.back();
            Tile* thisTile = new Tile(tile_id , used);
            delete used; // we don't used the pointer, we just wanted to copy its initial info
            types.pop_back();
            if (thisTile->type != "desert") give_num_tile(thisTile, num_tiles); // give it a number if it isn't desert hex
            else {
                thisTile->blocked = true; // just for consistency, doesn't make a difference
                robberLoc = tile_id;
            }
            if (tile_id > tilesAfterCurr) { // reset stuff
                currLayer++;
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
                for (int i = 1; i < 7; i++) link_node_to_tile(thisTile, i);
            } else{
                /* Tile - Node component */
                for (int i = 0; i < 2; i++) { // every tile has >= 3 nodes from n-1 in their adj_nodes. for two of these, we need to decrement nodeInCurr
                    link_node_to_tile(thisTile, nodeInCurr);
                    nodeInCurr--;
                }
                link_node_to_tile(thisTile, nodeInCurr); // this is the 3rd one, we just don't decrement nodeInCurr because we need to keep it the same for the next tile...
                link_node_to_tile(thisTile, nodeInPrev);
                nodeInPrev--;
                if (nodeInPrev < firstNodeInCurr - nodes_in_layer(currLayer-1)) nodeInPrev = totalNodesInPrev; // if the node in currLayer-1 goes passed the first node in currLayer-1
                link_node_to_tile(thisTile, nodeInPrev);
                if (tile_id == firstTileInCurr) link_node_to_tile(thisTile, firstNodeInCurr);
                else if (setCounter % currLayer == 0) { // ...unless we are on a corner tile, which requires an additional node
                    nodeInCurr--;
                    link_node_to_tile(thisTile, nodeInCurr);
                } else {
                    nodeInPrev--;
                    if (nodeInPrev < firstNodeInCurr - nodes_in_layer(currLayer-1)) nodeInPrev = totalNodesInPrev;
                    link_node_to_tile(thisTile, nodeInPrev);
                }
                setCounter++;
                /* Tile - Tile component */
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
    
    Board::Node::Node(int int_id) : int_id(int_id), owner(nullptr), knight(nullptr), settlement(nullptr), city(nullptr) {}
    Board::Node::~Node() { // owner is deleted in the game's deconstructor
        // Consider if deleting these pointers could cause malloc
        delete knight;
        knight = nullptr;
        delete settlement;
        settlement = nullptr;
        delete city;
        city = nullptr;
        
    }
    
    Board::Edge::Edge(int int_id) : int_id(int_id), owner(nullptr) {}
    Board::Edge::~Edge() {
        adj_nodes.clear(); // will already be deleted,
    }
    
    Board::Tile::Tile(int int_id, TileType* type) : int_id(int_id),  blocked(false), type(type->name), resource(type->resource), commodity(type->commodity) {}
    Board::Tile::~Tile() {
        adj_tiles.clear();
        adj_nodes.clear();
    }
    
    ostream& operator<<(ostream& os, const Board::Node& rhs){
        os << rhs.int_id << ":[";
        int iter = 0;
        for (Board::Node* nodeptr : rhs.adj_nodes) {
            cout << nodeptr->int_id;
            if (iter != rhs.adj_nodes.size()-1) cout << ", ";
            iter++;
        }
        cout << "], [";
        iter = 0;
        for (Board::Edge* edgptr : rhs.adj_edges) {
            cout << edgptr->int_id;
            if (iter != rhs.adj_edges.size()-1) cout << ", ";
            iter++;
        }
        cout << ']';
        return os;
    }
    
    ostream& operator<<(ostream& os, const Board::Tile& rhs) {
        const set<Board::Node*>& nodes_set = rhs.adj_nodes;
        int iter = 0;
        os << "Nodes:[";
        for (Board::Node* nodeptr : nodes_set) {
            os << nodeptr->int_id;
            if (iter != nodes_set.size()-1) os << ", ";
            iter++;
        }
        os << "], Tiles:[";
        const set<Board::Tile*>& tiles_set = rhs.adj_tiles;
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
