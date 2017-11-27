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
from sets import Set

class ConcentricGraph:
    def __init__(self, layers = 0):
        self.layers = layers
        self.height = 3 + 4 * self.layers # SEE: self.nodes_on_level(level)
        self.adjacency_list = self.create_board()

    def display(self):
        print(self.adjacency_list)

    def exterior_nodes(self, n):
        """
        An exterior node is any node that adjacent to two exterior edges
        :return: number of exterior nodes
        """
        if n == 0:
            return 6
        return 12*n + 6

    def total_nodes(self): #TODO: I don't like that this is O(n), try to find O(1) equation. Is this possible?
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

    def degree_two_gen(self, n):
        if n == 0:
            for i in range(1,7):
                yield i
        elif n > 0:
            start_node = self._total_nodes_helper(n-1) + 1
            set = [2 for i in range(n)] + [1]
            for i in range(len(set)): # first do an abridged set leaving out first two step
                if i == 0:
                    start_node += 1
                    yield start_node
                else:
                    start_node += set[i]
                    yield start_node
            for i in range(5): # need to go over the set 5 times for each side of the hexagonal graph
                for jump in set:
                    start_node += jump
                    yield start_node

    def create_board(self):
        board = dict()
        curr_layer = 0
        first_in_layer = 1
        nodes_after_layer = 6
        two_deg_in_prev = 1
        degree_two_nodes = Set()
        last_third_degree = None
        for i in range(1, self.total_nodes() + 1):
            if i == nodes_after_layer + 1: #we're in a new layer, need to reset some stuff
                curr_layer += 1
                if i != 7:
                    two_deg_in_prev = first_in_layer + 1 # assign this before moving on
                first_in_layer = nodes_after_layer + 1
                nodes_after_layer += self.exterior_nodes(curr_layer)
                degree_two_nodes = Set()
                last_third_degree = None
            if len(degree_two_nodes) == 0:
                for num in self.degree_two_gen(curr_layer):
                    degree_two_nodes.add(num)
            board[i] = []
            if i > 1:
                board[i].append(i-1)
                board[i-1].append(i)
            if i == nodes_after_layer:
                board[i].append(first_in_layer)
                board[first_in_layer].append(i)
            if i not in degree_two_nodes and i != first_in_layer: # if i is supposed to be a third degree node
                if last_third_degree != None:
                    jump = i - last_third_degree
                    two_deg_in_prev += 4-jump
                board[i].append(two_deg_in_prev)
                board[two_deg_in_prev].append(i)
                last_third_degree = i
        return board

graph = ConcentricGraph(2)
graph.display()