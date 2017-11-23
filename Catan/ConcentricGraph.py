"""
Author: Jonny Sassoon
Program: Concentric Graph

I have developed my own class for my analysis of a Concentric Graph: a graph structured such that
we start with the C3, C4, or C6 graph, and we extend off every exposed edge a new cycle
graph of the same order as the one we started with. This process is done repeatedly such that
doing this n times creates an n-layered Concentric Graph centered at a Cm where m is the
order of the cycle graph.

Graphs like this are often used in architectural designs, especially Concentric Graphs centered
at C6 - the familiar honeycomb pattern. Perhaps even more familiar, this graph implemented
where n = 2 is the mathematical structure for the board of Settlers of Catan.

This program contains several equations that define certain key properties of a Concentric Graph
centered at C6.
"""
from math import fabs

class ConcentricGraph:
    def __init__(self, layers = 0):
        self.layers = layers
        self.height = 3 + 4 * self.layers

    def exterior_nodes(self):
        """
        An exterior node is any node that links to two exterior edges
        :return: number of exterior nodes
        """
        if self.layers == 0:
            return 6
        return 12*self.layers + 6

    def total_nodes(self):
        """
        :return: number of total nodes
        """
        return self._total_nodes_helper(self.layers)

    def _total_nodes_helper(self, n):
        if n == 0:
            return 6
        return self._total_nodes_helper(n-1) + 12*n + 6

    def exterior_edges(self):
        """
        An exterior edge connects to at least one node such that deg(node) = 1
        :return: number of exterior edges
        """
        if self.layers == 0:
            return 6
        return 12*self.layers + 6

    def total_edges(self):
        return self._total_edges_helper(self.layers)

    def _total_edges_helper(self, n):
        if n == 0:
            return 6
        return self._total_edges_helper(n-1) + 18*n + 6

    def exterior_tiles(self):
        if self.layers == 0:
            return 1
        return 6*self.layers

    def total_tiles(self):
        return self._total_tiles_helper(self.layers)

    def _total_tiles_helper(self, n):
        if n == 0:
            return 1
        return self._total_tiles_helper(n-1) + 6 * n

    def nodes_on_level(self, level):
        if level < 0 or level > self.height-1:
            print(level)
            raise ValueError("Level not in range.")
        if level < self.layers:
            return 2 * (level + 1)
        if self.layers <= level and level <= (3 + 4 * self.layers) / 2:
            return int(2 * (self.layers + 1))
        else:
            return int(self.nodes_on_level(fabs(4 * self.layers + 2 - level)))

    def level_of_node(self, node):
        layer = 0
        previous_nodes = 0
        while previous_nodes < node:
            previous_nodes += self.nodes_on_level(layer)
            layer += 1
        return layer - 1

    def max_row_pop(self):
        """
        :return: maximum number of nodes in a row of the graph
        """
        return 2*self.layers+2

    def rows_with_max(self):
        """
        :return: number of rows that have the maximum number of nodes
        """
        return self.layers*2 + 3

    def nodes_before(self, level):
        if level == 0:
            return 0
        return self.nodes_on_level(level-1) + self.nodes_before(level-1)
