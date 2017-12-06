//
// Author: Jonny Sassoon
// Program: Catan Board
//
// Header file for Catan Board using a Concetric Graph

#ifndef Board_h
#define Board_h
#include "ConcentricGraph.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

namespace Catan{
    // TODO: WRITE ALGORITHM FOR TILE-TILE ADJACENCIES
    
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
//            ~Node();
            int int_id;
            std::vector<Edge*> adj_edges;
        };
        
        friend std::ostream& operator<<(std::ostream& os, const Board::Node& rhs) { // just defined this here. might change
            os << rhs.int_id;
            return os;
        }
        
        struct Edge {
            Edge(int int_id);
            int int_id;
            std::vector<Node*> adj_nodes;
        };
        
        struct Tile { // TODO: turn into class, this isn't so simple anymore. Should have data hiding
            Tile(int int_id, TileType* type);
//            ~Tile();
            void give_resources() const;
            int int_id; // unique identifier on the board
            int num_tile; // dice number
            bool blocked; // for the robber
            std::vector<Node*> adj_nodes; // list of the nodes adjacent to the tile
            std::vector<Tile*> adj_tiles; // list of the tiles adjacent to this tile
            TileType* type; // mountain, field, pasture, etc...
        };
    public:
        Board(int layer = 2);
        ~Board();
        void display() const;
    private:
        void create_board();
        void generate_tiles();
        void add_node_to_tile(Tile* thisTile, int node_id);
        void give_num_tile(Tile* the_tile, std::vector<int>& tiles_vec);
        void log_tile(Tile* the_tile);
        void generate_nodes();
        void add_adjacencies(int node1_id, int node2_id); // adds node1 to node2's adj_lst and vice versa
        std::unordered_map<int, std::unordered_map<Node*, std::vector<Node*>>> adj_list;
        std::unordered_map<int, std::unordered_map<int, Tile*>> tiles_map; // maps int (number tile) to a map of the tiles that with that number tile according to the tiles id number.
    };
}

#endif /* Board_h */
