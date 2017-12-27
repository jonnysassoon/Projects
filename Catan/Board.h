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
// vertex of hex-1 with two being adjacent to it to the right. The node ids continue incrementing as you
// go clockwise around that layer. When we get to 6 (middle left vertex of hex-1), node 7 is immeditaley
// to its left (top left vertex of hex-2). The pattern continues for all nodes in the board.
// 


#ifndef Board_h
#define Board_h
#include "ConcentricGraph.h"
#include <map>
#include <set>
#include <vector>
#include <string>
#include <iostream>

namespace Catan{
        
    struct Edge;

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
        struct Node {
            Node(int int_id);
//            ~Node(); if I have a destructor for the board that deletes all the Tiles and Nodes, is this necessary?
            int int_id;
//            Knight* knight;
//            Player* owner;
            bool has_settlement;
            bool has_city;
            std::set<Edge*> adj_edges;
            std::set<Node*> adj_nodes;
        };
        
        friend std::ostream& operator<<(std::ostream& os, const Board::Node& rhs);
        
        struct Edge {
            Edge(int int_id);
            int int_id;
            std::set<Node*> adj_nodes;
        };
        
        struct Tile { // TODO: turn into class, this isn't so simple anymore. Should have data hiding
            Tile(int int_id, TileType* type);
//            ~Tile(); if I have a destructor for the board that deletes all the Tiles and Nodes, is this necessary?
            void give_resources() const;
            int int_id; // unique identifier on the board
            int num_tile; // dice number
            bool blocked; // for the robber
            std::set<Node*> adj_nodes; // set of the nodes adjacent to the tile.
            std::set<Tile*> adj_tiles; // set of the tiles adjacent to this tile.
            TileType* type; // mountain, field, pasture, etc...
        };
        friend std::ostream& operator<<(std::ostream& os, const Board::Tile& rhs);
    public:
        Board(int layer = 2);
        ~Board();
        void display() const;
    private:
        void create_board();
        void generate_tiles();
        void link_tiles(Tile& tile1, Tile& tile2);
        void add_node_to_tile(Tile* thisTile, int node_id);
        void give_num_tile(Tile* the_tile, std::vector<int>& tiles_vec);
        void log_tile(Tile* the_tile);
        void generate_nodes();
        void link_nodes(Node& node1, Node& node2); // adds node1 to node2's adj_lst and vice versa
        std::map<int, Node*> nodes_map;
        std::map<int, Tile*> tiles_map; // TODO: is there a way to efficiently access the tile(s) when the dice are rolled?
    };
}

#endif /* Board_h */
