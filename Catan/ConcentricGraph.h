//
//Author: Jonny Sassoon
//Program: Concentric Graph
//
//  I have developed my own class for my analysis of a Concentric Graph: a graph structured such that
//  we start with the C3, C4, or C6 graph, and we extend off every exposed edge a new cycle
//  graph of the same order as the one we started with. This process is done repeatedly such that
//  doing this n times creates an n-layered Concentric Graph centered at a Cm where m is the
//  order of the cycle graph.
//
//  Graphs like this are often used in architectural designs, especially Concentric Graphs centered
//  at C6 - the familiar honeycomb pattern. Perhaps even more familiar, this graph implemented
//  where n = 2 is the mathematical structure for the board of Settlers of Catan.
//
//  This program contains several equations that define certain key properties of a Concentric Graph
//  centered at C6.
//
//

#ifndef ConcentricGraph_h
#define ConcentricGraph_h

#include <vector>
#include <set>

namespace Catan{
    
    class ConcentricGraph {
    public:
        ConcentricGraph(int layers);
        int nodes_in_layer(int layer) const;
        int total_nodes() const; // total number of nodes in entire graph
        int total_nodes(int layer) const; // gets total number of nodes in the entire graph up to layer n
        int edges_in_layer(int layer) const;
        int total_edges() const; // gets edges
        int total_edges(int layer) const;
        int tiles_in_layer(int layer) const;
        int total_tiles() const; // gets tiles
        int total_tiles(int layer) const;
        int nodes_on_level(int level) const; // never used in Catan implementation
        int max_on_level() const;
        int levels_with_max() const;
        std::set<int> second_degree_nodes(int layer) const;
    private:
        
        int layers;
        int height;
    };
}

#endif /* ConcentricGraph_h */
