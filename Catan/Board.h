//
// Author: Jonny Sassoon
// Program: Catan Board
//
// Header file for Catan Board using a Concetric Graph
//
//
// APPEARANCE OF CATAN BOARD WITH TILE-NUMBERING CONVENTION:
//      _
//    _/g\_
//  _/h\_/f\_
// /i\_/6\_/e\
// \_/7\_/5\_/
// /j\_/1\_/d\
// \_/2\_/4\_/
// /8\_/3\_/c\
// \_/9\_/b\_/
//   \_/a\_/
//     \_/
// 
// Numbers represent the tile IDs where a - j represent numbers 10 - 19. Node 1 is at the top left
// vertex of hex-1 with Node 2 being adjacent to it to the right. The node ids continue incrementing as you
// go clockwise around that layer. When we get to 6 (middle left vertex of hex-1), node 7 is immeditaley
// to its left (top left vertex of hex-2). The pattern continues for all nodes in the board. The Edges are
// made as nodes are connected. As such, the numbering system for the edges follows the order of nodes being
// added to another nodes adjacency list (see implementation for generate_nodes())


#ifndef Board_h
#define Board_h
#include "ConcentricGraph.h"
#include "Pieces.h"
#include "Player.h"
#include <map>
#include <set>
#include <vector>
#include <string>
#include <iostream>

namespace Catan{
    
    struct TileType{
        TileType(std::string name, std::string resource);
        std::string name;
        std::string resource;
    };
    
    struct Hill : public TileType {
        Hill();
    };
    
    struct Field : public TileType {
        Field();
    };
    
    struct Desert : public TileType {
        Desert();
    };
    
    struct CommTile : public TileType {
        CommTile(std::string name, std::string resource, std::string commodity);
        std::string commodity;
    };
    
    struct Mountain : public CommTile {
        Mountain();
    };
    
    struct Forest : public CommTile {
        Forest();
    };
    
    struct Pasture : public CommTile {
        Pasture();
    };
    
    class Board : public ConcentricGraph {
        struct Edge;
        struct Tile;
        
        struct Node {
            Node(int int_id);
            int int_id;
            Player* owner;
            Knight* knight;
            Settlement* settlement;
            City* city;
            std::set<Edge*> adj_edges;
            std::set<Node*> adj_nodes;
            std::set<Tile*> adj_tiles;
        };
        
        struct Edge {
            Edge(int int_id);
            int int_id;
            Player* owner;
            std::set<Node*> adj_nodes;
        };
        
        struct Tile {
            Tile(int int_id, TileType* type);
            int int_id; // unique identifier on the board
            int num_tile; // dice number.
            bool blocked; // for the robber
            std::set<Node*> adj_nodes; // set of the nodes adjacent to the tile.
            std::set<Tile*> adj_tiles; // set of the tiles adjacent to this tile.
            TileType* type; // mountain, field, pasture, etc...
        };
        
        friend std::ostream& operator<<(std::ostream& os, const Board::Node& rhs);
        friend std::ostream& operator<<(std::ostream& os, const Board::Tile& rhs);
    public:
        Board(int layer = 2);
        ~Board();
        void display() const;
        bool isValidSetLoc(int loc, Player* player); // node loc
        bool isValidCityLoc(int loc, Player* player); // node loc
        bool isValidRoadLoc(int loc, Player* player); // edge loc
        bool isValidFirstRoadLoc(int roadLoc, int settLoc); // the settlement is already validated, no need for player pointer
        bool isValidKnightLoc(int loc, Player* player); // node loc
        bool isValidWallLoc(int loc, Player* player); // node loc
        bool isValidRobberLoc(int loc); // tile loc
        void placeSettlement(int loc, Settlement* settlement);
        void placeRoad(int loc, Road* road);
        void placeCity(int loc, City* city, bool setUp = false);
    private:
        void create_board();
        void generate_tiles();
        void link_tiles(Tile& tile1, Tile& tile2);
        void link_node_to_tile(Tile* thisTile, int node_id);
        void give_num_tile(Tile* the_tile, std::vector<int>& tiles_vec);
        void log_tile(Tile* the_tile);
        void generate_nodes();
        void link_nodes(int& edge_id, Node& node1, Node& node2); // adds node1 to node2's adj_lst and vice versa
        std::map<int, Node*> nodes_map;
        std::map<int, Edge*> edges_map;
        std::map<int, Tile*> tiles_map; // TODO: is there a way to efficiently access the tile(s) when the dice are rolled?
        std::map<int, Player*> city_locs; // location of all the cities with their owners
        int robberLoc;
    };
}

#endif /* Board_h */
