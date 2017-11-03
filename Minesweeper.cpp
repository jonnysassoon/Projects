/*
author: Jonathan Sassoon
program: minesweeper
This program runs minesweeper with a 10x10 board and 10 bombs
*/
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
using namespace std;

struct Tile{
	Tile(bool ib = false, int ab = 0){
		adj_bombs = ab;
		is_bomb = ib;
	}
	int adj_bombs;
	bool is_bomb;
	bool is_visible = false;
};

class Minesweeper{
public:
	Minesweeper(){ // create new board with random bomb locations
		Tile aTile = Tile();
		vector<Tile> cushion_top(COLUMNS+2, aTile);
		board.push_back(cushion_top);
		for (int row = 0; row < ROWS; row++){
			Tile aTile = Tile(false, 0);
			vector<Tile> row_data(COLUMNS+2, aTile);
			board.push_back(row_data);
		}
		aTile = Tile();
		vector<Tile> cushion_bottom(COLUMNS+2, aTile);
		board.push_back(cushion_bottom);
		add_bombs(board);
	}
	void display(bool display_bombs = false) const { // displays the current board state
		for (int row_ind = 1; row_ind < ROWS+1; row_ind++){ // need offset because of cushion
			cout << row_ind << '.';
			if (!(row_ind == 10)){
				cout << ' ';
			}
			for (int col_ind = 1; col_ind < COLUMNS+1; col_ind++){
				if (board[row_ind][col_ind].is_bomb){
					if (display_bombs){
						cout << 'X';
					}
					else{
						cout << '-';
					}
				}
				else if (!board[row_ind][col_ind].is_visible){
					cout << '-';
				}
				else {
					if (board[row_ind][col_ind].adj_bombs == 0){
						cout << ' ';
					}
					else{
						cout << board[row_ind][col_ind].adj_bombs;
					}
				}
			}
		cout << endl;
		}
	}
	bool done() const {
		if (unchecked == BOMBS){ // if only tiles left unchecked are the bombs, we win
			return true;
		}
		return false;
	}
	bool validRow(int row_num) const { // returns true if row is valid, false otherwise
		if (row_num > 0 && row_num <= ROWS){
			return true;
		}
		return false;
	}
	bool validCol(int col_num) const { // returns true if column is, false otherwise
		if (col_num > 0 && col_num <= COLUMNS){
			return true;
		}
		return false;
	}
	bool isVisible(int row_num, int col_num) const { // returns true if cell is visible
		if (board[row_num][col_num].is_visible){
			return true;
		}
		return false;
	}
	bool play(int row_ind, int col_ind){ // plays the cell according to rules
		if (row_ind < 1 || row_ind > 10 || col_ind < 1 || col_ind > 10){
			return false;
		}
		if (board[row_ind][col_ind].is_bomb){
			return false; // explosion
		}
		else if (board[row_ind][col_ind].adj_bombs > 0){
			board[row_ind][col_ind].is_visible = true;
			unchecked -= 1;
			return true;
		}
		else{ // if it has no bombs in neighbors, recursively play all of its neighbors
			board[row_ind][col_ind].is_visible = true;
			if (!board[row_ind+1][col_ind].is_visible){
				play(row_ind+1, col_ind);	
			}
			if (!board[row_ind+1][col_ind-1].is_visible){
				play(row_ind+1, col_ind-1);	
			}
			if (!board[row_ind][col_ind-1].is_visible){
				play(row_ind, col_ind-1);	
			}
			if (!board[row_ind-1][col_ind-1].is_visible){
				play(row_ind-1, col_ind-1);	
			}
			if (!board[row_ind-1][col_ind].is_visible){
				play(row_ind-1, col_ind);	
			}
			if (!board[row_ind-1][col_ind+1].is_visible){
				play(row_ind-1, col_ind+1);	
			}
			if (!board[row_ind][col_ind+1].is_visible){
				play(row_ind, col_ind+1);	
			}
			if (!board[row_ind+1][col_ind+1].is_visible){
				play(row_ind+1, col_ind+1);	
			}
			
			unchecked -= 1;
			return true;
		}
	}

private:
	const int BOMBS = 10; 
	const int ROWS = 10;
	const int COLUMNS = 10;
	int unchecked = ROWS*COLUMNS;
	vector<vector<Tile> > board;
	void add_bombs(vector<vector<Tile> >& board){
		srand(time(NULL)); // create the seed
		for (int bomb_num = 0; bomb_num < BOMBS; bomb_num++){ // randomly insert bombs into board
			int row_ind = 1+rand() % ROWS;
			int col_ind = 1+rand() % COLUMNS;
			while (board[row_ind][col_ind].is_bomb){ // if we already placed a bomb there, choose again
				row_ind = 1+rand() % ROWS;
				col_ind = 1+rand() % COLUMNS;
			}
			board[row_ind][col_ind].is_bomb = true;
			increment_neighbor_count(row_ind, col_ind);
		}
	}
	void increment_neighbor_count(int row_ind, int col_ind){
		board[row_ind+1][col_ind].adj_bombs += 1;
		board[row_ind+1][col_ind-1].adj_bombs += 1;
		board[row_ind][col_ind-1].adj_bombs += 1;
		board[row_ind-1][col_ind-1].adj_bombs += 1;
		board[row_ind-1][col_ind].adj_bombs += 1;
		board[row_ind-1][col_ind+1].adj_bombs += 1;
		board[row_ind][col_ind+1].adj_bombs += 1;
		board[row_ind+1][col_ind+1].adj_bombs += 1;
	}
};

int main() {
	Minesweeper sweeper;
    // Continue until the only invisible cells are bombs
    while (!sweeper.done()) {
        sweeper.display(false); // display the board without bombs

        int row_sel = -1, col_sel = -1;
        while (row_sel == -1) {
            // Get a valid move
            int r, c;
            cout << "row? ";
             cin >> r;
            if (!sweeper.validRow(r)) {
                cout << "Row out of bounds\n";
                continue;
            }
            cout << "col? ";
            cin >> c;
            if (!sweeper.validCol(c)) {
                cout << "Column out of bounds\n";
                continue;
            }
            if (sweeper.isVisible(r,c)) {
                cout << "Square already visible\n";
                continue;
            }
            row_sel = r;
            col_sel = c;
        }
        // Set selected square to be visible. May effect other cells.
        if (!sweeper.play(row_sel,col_sel)) {
            cout << "BOOM!!!\n";
            sweeper.display(true);
            // We're done! Should consider ending more "cleanly",
            // eg. Ask user to play another game.
            exit(0);
        }
    }
    // Ah! All invisible cells are bombs, so you won!
    cout << "You won!!!!\n";
    sweeper.display(true); // Final board with bombs shown
}
