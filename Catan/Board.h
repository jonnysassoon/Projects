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
#include <iostream>

namespace Catan{
    struct Node;
    struct Tile;
    
    class Board : public ConcentricGraph {
        friend std::ostream& operator<<(std::ostream& os, Board& rhs);
    public:
        Board(int layer = 2);
//        ~Board();
    private:
        void create_board();
        void create_adj_lst();
        void add_adjacencies(int node1_id, int node2_id); // adds node1 to node2's adj_lst and vice versa
        std::unordered_map<int, std::unordered_map<Node*, std::vector<Node*>>> adj_list;
        std::vector<Tile*> tiles_lst;
    };
    
    struct Edge {
        Edge(int int_id);
        int int_id;
        std::vector<Node*> adj_nodes;
    };
    
    std::ostream& operator<<(std::ostream& os, const Edge& rhs);
    
    struct Node {
        Node(int int_id);
        ~Node();
        int int_id;
        std::vector<Edge*> adj_edges;
    };
    
    std::ostream& operator<<(std::ostream& os, const Node& rhs);
    
    struct Tile {
        Tile(int int_id);
        ~Tile();
        int int_id;
        std::vector<Node*> adj_nodes;
    };
    
    std::ostream& operator<<(std::ostream& os, const Tile& rhs);
    
}

#endif /* Board_h */
