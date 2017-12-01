#include "ConcentricGraph.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <unordered_set>
#include <unordered_map>
using namespace std;
using namespace Catan;

class TestGraph : public ConcentricGraph {
public:
	TestGraph(int layers) : ConcentricGraph(layers) {}
	void display() const {
		for (int val = 1; val <= tiles_map.size(); val++) {
			cout << val << ": [";
			for (int adj: tiles_map.at(val)) {
				cout << adj << ", ";
			}
			cout << "]\n";
		}
	}
	void add_node_to_tile(int tile_num, int node_num) {
		tiles_map[tile_num].push_back(node_num);
	}
	void generate_tiles() {
		int curr_layer = 1;
		int first_in_prev = 1;
		int last_in_prev = 6;
		int last_in_curr = 24;
		int fixed_last_in_curr = 24;
		int first_in_curr = 7;
		int total_in_prev = 6;
		int first_tile_in_curr = 2;
		int tiles_in_layer = 7;
		int set_counter = 0;
		for (int tile_num = 1; tile_num < total_tiles() + 1; tile_num++){
			vector<int> links;
			tiles_map.emplace(tile_num, links);
			if (tile_num == 1) {
				for (int i = 1; i < 7; i++) add_node_to_tile(tile_num, i);
			} else{
				if (tile_num > tiles_in_layer) { // reset stuff
					first_in_prev += 12 * (curr_layer-1) + 6;
					curr_layer++;
					last_in_prev += 12 * (curr_layer-1) + 6;
					fixed_last_in_curr += 12 * (curr_layer) + 6;
					last_in_curr = fixed_last_in_curr;
					first_in_curr = last_in_prev+1;
					total_in_prev = last_in_prev;
					first_tile_in_curr = tile_num;
					tiles_in_layer += 6*curr_layer;
					set_counter = 0;
				}
				for (int i = 0; i < 2; i++) {
					add_node_to_tile(tile_num, last_in_curr);
					last_in_curr--;
				}
				add_node_to_tile(tile_num, last_in_curr);
				add_node_to_tile(tile_num, last_in_prev);
				last_in_prev--;
				if (last_in_prev < first_in_prev) last_in_prev = total_in_prev;
				add_node_to_tile(tile_num, last_in_prev);
				if (tile_num == first_tile_in_curr) add_node_to_tile(tile_num, first_in_curr);
				else if (set_counter % curr_layer == 0) {
					last_in_curr--;
					add_node_to_tile(tile_num, last_in_curr);
				} else {
					last_in_prev--;
					if (last_in_prev < first_in_prev) last_in_prev = total_in_prev;
					add_node_to_tile(tile_num, last_in_prev);
				}
				set_counter++;
			}
		}
	}
private:
	unordered_map<int, vector<int>> tiles_map;
};


int main() {
	TestGraph graph(5);
	graph.generate_tiles();
	graph.display();
	return 0;
}
