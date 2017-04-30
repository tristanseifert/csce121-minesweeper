#include "GameBoard.h"

#include "MainWindow.h"

#include <stdexcept>
#include <FL/fl_draw.H>

using namespace std;

/**
 * Initializes the game board.
 */
GameBoard::GameBoard(int x, int y, int w, int h, MainWindow *parent) : Fl_Box(FL_NO_BOX, x, y, (w * CELL_SIZE), (h * CELL_SIZE), nullptr), _parent(parent) {
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

	// load images
	this->_loadImages();
}

GameBoard::~GameBoard() {

}

/**
 * Loads images (flag, question mark, mines, etc.) from disk so they can be
 * easily drawn later.
 */
void GameBoard::_loadImages() {
	this->_imgMine = new Fl_PNG_Image("images/mine.png");
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

/**
 * Draw the grid.
 */
void GameBoard::draw() {
	// draw box border
	Fl_Box::draw();

	// draw each of the cells
	for(int i = 0; i < this->gridW; i++) {
		for(int j = 0; j < this->gridH; j++) {
			TileType t = this->storage[i][j];

			int x = (i * CELL_SIZE) + this->x();
			int y = (j * CELL_SIZE) + this->y();

			// draw box outline if not revealed
			if(t[UNCOVERED_BIT] == false) {
				fl_draw_box(FL_UP_BOX, x, y, CELL_SIZE, CELL_SIZE, FL_GRAY);
			}
			// otherwise, draw a plain border
			else {
				fl_draw_box(FL_BORDER_BOX, x, y, CELL_SIZE, CELL_SIZE, FL_DARK1);
			}

			// draw on top if uncovered OR debug mode is enabled
			if(t[UNCOVERED_BIT] == true || this->debugMode) {
				// is it a mine?
				if(t[MINE_BIT] == true) {
					this->_imgMine->draw(x, y);
				}
			}
		}
	}
}
