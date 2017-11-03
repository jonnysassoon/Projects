/*
Author: Jonathan Sassoon
Name of Program: Conway's Game of Life
This program accepts an input file of a given format and runs Conway's Game of Life
on that file
*/
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

vector<vector<char>> getBoard(){
	ifstream ifs("life.txt");
	vector<vector<char>> board; 
	char aChar;
	vector<char> data;
	for (int i = 0; i < 22; i++){ // create ceiling
		data.push_back('-');
	}
	board.push_back(data);
	data.clear(); // recreate empty vector
	data.push_back('-'); // add a cushion to reduce if statements
	while (ifs >> aChar){
		data.push_back(aChar);
		if (data.size() == 21){ // 20 logical characters plus the added cushion
			data.push_back('-'); // add a cushion to reduce if statements
			board.push_back(data);
			data.clear();
			data.push_back('-'); // add a cushion to reduce if statements
		}
	}
	for (int i = 0; i < 22; i++){ // create ceiling
		data.push_back('-');
	}
	board.push_back(data);
	ifs.close();
	return board;
}

void revealBoard(const vector<vector<char>>& board){
	for (int i = 1; i < board.size()-1; i++){
		for (int j = 1; j < board[0].size()-1; j++){
			if (board[i][j] == '*'){
				cout << board[i][j];
			}
			else{
				cout << ' ';
			}
			if (j % 20 == 0){
				cout << '\n';
			}
		}
	}
}

void gameOfLife(){
	vector<vector<char>> board = getBoard();
	cout << "Initial world\n";
	revealBoard(board);
	cout << "===========================\n";
	for (int gen = 1; gen < 11; gen++){ // for each generation
		cout << "Generation: " << gen << endl;
		vector<vector<char>> copy = board;
		for (int i = 1; i < board.size()-1; i++){
			for (int j = 1; j < board[i].size()-1; j++){
				int living_neighbors = 0;
				int dead_neighbors = 0;
				if (board[i-1][j] == '-'){
					dead_neighbors += 1;
				} else{
					living_neighbors += 1;
				}
				if (board[i-1][j-1] == '-'){
					dead_neighbors += 1;
				} else{
					living_neighbors += 1;
				}
				if (board[i][j-1] == '-'){
					dead_neighbors += 1;
				} else{
					living_neighbors += 1;
				}
				if (board[i+1][j-1] == '-'){
					dead_neighbors += 1;
				} else{
					living_neighbors += 1;
				}
				if (board[i+1][j] == '-'){
					dead_neighbors += 1;
				} else{
					living_neighbors += 1;
				}
				if (board[i+1][j+1] == '-'){
					dead_neighbors += 1;
				} else{
					living_neighbors += 1;
				}
				if (board[i][j+1] == '-'){
					dead_neighbors += 1;
				} else{
					living_neighbors += 1;
				}
				if (board[i-1][j+1] == '-'){
					dead_neighbors += 1;
				} else{
					living_neighbors += 1;
				}
				if (board[i][j] == '-' && living_neighbors == 3){ //Each empty/dead cell that has exactly three living neighbors--no more, no fewer-- comes to life in the next generation.
					copy[i][j] = '*';
				}
				else if (board[i][j] == '*' && (living_neighbors > 3 || living_neighbors < 2)){ // A living cell with fewer than two living neighbors dies of loneliness and a living cell with more than three living neighbors will die from overcrowding
					copy[i][j] = '-';
				}
			}
		}
		board = copy;
		revealBoard(board);
		cout << "===========================\n";
	}
}

int main(){
	gameOfLife();
	return 0;
}