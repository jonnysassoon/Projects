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

#include <unordered_set>

namespace Catan{
    class ConcentricGraph {
    public:
        ConcentricGraph(int layers);
        int exterior_nodes() const;
        int total_nodes() const;
        int exterior_edges() const;
        int total_edges() const;
        int exterior_tiles() const;
        int total_tiles() const;
        int nodes_on_level(int level) const;
        int max_on_level() const;
        int levels_with_max() const;
        std::unordered_set<int> second_degree_nodes(int layer) const;
    private:
        int total_nodes_helper(int n) const;
        int total_edges_helper(int n) const;
        int total_tiles_helper(int n) const;
        int layers;
        int height = 3 + 4 * layers;
    };
}

#endif /* ConcentricGraph_h */
