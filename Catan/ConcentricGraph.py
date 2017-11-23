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
        self.height = 3 + 4 * self.layers # SEE: self.nodes_on_level

    def exterior_nodes(self):
        """
        An exterior node is any node that adjacent to two exterior edges
        :return: number of exterior nodes
        """
        if self.layers == 0:
            return 6
        return 12*self.layers + 6

    def total_nodes(self): #TODO: I don't like that this is O(n), try to find O(1) equation
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
        An exterior edge connects to at least one node such that deg(node) = 2
        :return: number of exterior edges
        """
        if self.layers == 0:
            return 6
        return 12*self.layers + 6

    def total_edges(self): #TODO: I don't like that this is O(n), try to find O(1) equation
        return self._total_edges_helper(self.layers)

    def _total_edges_helper(self, n):
        if n == 0:
            return 6
        return self._total_edges_helper(n-1) + 18*n + 6

    def exterior_tiles(self):
        """
        Every "tile" is an independent C6 graph. An "exterior tile" is an independent C6 graph
        where at least two of its edges are exterior.
        :return: number of exterior tiles
        """
        if self.layers == 0:
            return 1
        return 6*self.layers

    def total_tiles(self): #TODO: I don't like that this is O(n), try to find O(1) equation
        return self._total_tiles_helper(self.layers)

    def _total_tiles_helper(self, n):
        if n == 0:
            return 1
        return self._total_tiles_helper(n-1) + 6 * n

    def nodes_on_level(self, level):
        """
        A "level" is the horizontal row on which nodes lie. For convention, I have chosen
        to count levels in the following way:
level = 0----->  __
level = 1---> __/  \__
.            /  \__/  \
.            \__/  \__/
.            /  \__/  \
.            \__/  \__/
self.height---> \__/
        :return: total number of nodes that lie on the level
        """
        if level < 0 or level > self.height-1: # out of bounds
            print(level)
            raise ValueError("Level not in range.")
        if level < self.layers:
            return 2 * (level + 1)
        if self.layers <= level and level <= (3 + 4 * self.layers) / 2:
            return int(2 * (self.layers + 1))
        else: # the graph is symmetrical. if the level is past the half way point,
              # just give its corresponding value from the first half
            return int(self.nodes_on_level(fabs(4 * self.layers + 2 - level)))

    def level_of_node(self, node):
        layer = 0
        previous_nodes = 0
        while previous_nodes < node:
            previous_nodes += self.nodes_on_level(layer)
            layer += 1
        return layer - 1

    def max_nodes_on_level(self):
        """
        :return: maximum number of nodes that will be on any of the levels
        """
        return 2*self.layers+2

    def levels_with_max(self):
        """
        :return: number of levels that have the maximum number of nodes
        """
        return self.layers*2 + 3

    def nodes_before(self, level): #TODO: I don't like that this is O(n), try to find O(1) equation
        """
        :return: number of nodes in the graph BEFORE the level
        """
        if level == 0:
            return 0
        return self.nodes_on_level(level-1) + self.nodes_before(level-1)
