#include "GameBoard.h"

#include "MainWindow.h"

#include <iostream>
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
				Fl_Color c = FL_DARK1;

				// should the box be red?
				if(t[RED_BIT]) {
					c = FL_RED;
				}

				fl_draw_box(FL_BORDER_BOX, x, y, CELL_SIZE, CELL_SIZE, c);
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

/**
 * Handle user interaction with the board (we only care about click events)
 */
int GameBoard::handle(int event) {
	// mouse down; make sure it's in the bounds
	if(event == FL_PUSH) {
		if(Fl::event_x() < this->x() || Fl::event_x() > (this->x() + this->w())) {
			return 0;
		} else if(Fl::event_y() < this->y() || Fl::event_y() > (this->y() + this->h())) {
			return 0;
		}

		// click down inside bounds; we're interested.
		return 1;
	}
	// the mouse was up inside the control
	else if(event == FL_RELEASE) {
		// figure out which grid cell it was in
		int x = (Fl::event_x() - this->x()) / CELL_SIZE;
		int y = (Fl::event_y() - this->y()) / CELL_SIZE;

		// left button?
		if(Fl::event_button() == FL_LEFT_MOUSE) {
			this->uncoverCell(x, y);
		}
		// middle button?
		else if(Fl::event_button() == FL_MIDDLE_MOUSE) {

		}
		// right button?
		else if(Fl::event_button() == FL_RIGHT_MOUSE) {

		}

		return 1;
	}
	// unhandled event: bubble up to parent
	else {
		return Fl_Box::handle(event);
	}
}

/**
 * Uncovers the cell.
 */
void GameBoard::uncoverCell(int x, int y) {
	TileType &t = this->storage[x][y];

	// set uncovered flag
	t[UNCOVERED_BIT] = true;

	// is this a mine?
	if(t[MINE_BIT] == true) {
		// lmao game over
		this->_parent->gameOver();

		t[RED_BIT] = true;
	}

	// force redraw
	this->redraw();
}

/**
 * Calculates the number of mines that are on the board and have NOT been
 * uncovered by the user.
 */
int GameBoard::getMinesRemaining() const {
	int remaining = 0;

	for(int i = 0; i < this->gridW; i++) {
		for(int j = 0; j < this->gridH; j++) {
			TileType t = this->storage[i][j];

			if(t[MINE_BIT] == true && t[UNCOVERED_BIT] == false) {
				remaining++;
			}
		}
	}

	return remaining;
}
