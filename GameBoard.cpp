#include "GameBoard.h"

#include "MainWindow.h"

#include <stdexcept>

using namespace std;

/**
 * Initializes the game board.
 */
GameBoard::GameBoard(int x, int y, int w, int h, MainWindow *parent) : Fl_Box(FL_DOWN_BOX, x, y, (w * CELL_SIZE), (h * CELL_SIZE), nullptr), _parent(parent) {
	this->gridW = w;
	this->gridH = h;

	// create storage
	this->storage.clear();

	for(int x = 0; x < w; x++) {
		this->storage.push_back(vector<TileType>());

		for(int y = 0; y < h; y++) {
			// cleared
			this->storage[x].push_back(0);
		}
	}
}

GameBoard::~GameBoard() {

}

/**
 * Randomly distributes mines in the board.
 */
void GameBoard::generateMines(int mines) {
	if(mines > (this->gridW * this->gridH)) {
		throw runtime_error("cannot fill more mines than there are cells");
	}

	int minesPlaced = 0;

	while(minesPlaced != mines) {
		// generate random X/Y coordinate
		int x = arc4random_uniform(this->gridW);
		int y = arc4random_uniform(this->gridH);

		// is there already a mine here?
		if(this->storage[x][y][MINE_BIT] != true) {
			this->storage[x][y][MINE_BIT] = true;
			minesPlaced++;
		}
	}
}

/**
 * Updates the grid contents.
 */
void GameBoard::updateGrid() {

}
