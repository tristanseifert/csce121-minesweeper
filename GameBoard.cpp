#include "GameBoard.h"

#include "MainWindow.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <FL/fl_draw.H>

using namespace std;

// colors for mine counters, 0 through 8
static const Fl_Color mineCountColors[] = {
	FL_BLACK,					// 0
	fl_rgb_color(1, 0, 254),	// 1
	fl_rgb_color(1, 127, 1),	// 2
	fl_rgb_color(254, 0, 0),	// 3
	fl_rgb_color(1, 0, 128),	// 4
	fl_rgb_color(129, 1, 2),	// 5
	fl_rgb_color(0, 128, 129),	// 6
	fl_rgb_color(0, 0, 0),		// 7
	fl_rgb_color(128, 128, 128),// 8
};

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
			this->storage[x].push_back(TileType());
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
	this->_imgFlag = new Fl_PNG_Image("images/flag.png");
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
		if(this->storage[x][y].isMine != true) {
			this->storage[x][y].isMine = true;
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
			if(t.uncovered == false) {
				fl_draw_box(FL_UP_BOX, x, y, CELL_SIZE, CELL_SIZE, FL_GRAY);
			}
			// otherwise, draw a plain border
			else {
				Fl_Color c = FL_DARK1;

				// should the box be red?
				if(t.bgRed) {
					c = FL_RED;
				}

				fl_draw_box(FL_BORDER_BOX, x, y, CELL_SIZE, CELL_SIZE, c);
			}

			// draw on top if uncovered OR debug mode is enabled
			if(t.uncovered == true || this->debugMode) {
				// is it a mine?
				if(t.isMine == true) {
					this->_imgMine->draw(x, y);
				}

				// render the number
				if(t.surroundingMines > 0) {
					stringstream s;
					s << t.surroundingMines;

					fl_color(mineCountColors[t.surroundingMines]);
					fl_font(FL_COURIER | FL_BOLD, 16);
					fl_draw(s.str().c_str(), x + 2, y - fl_descent() + fl_height());
				}
			}

			// is it flagged?
			if(t.flagged == true) {
				this->_imgFlag->draw(x, y);
			}
			// is it a question mark?
			if(t.question == true) {
				fl_color(FL_BLACK);
				fl_font(FL_COURIER | FL_BOLD, 16);
				fl_draw("?", x + 2, y - fl_descent() + fl_height());
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
			this->flagQuestion(x, y);
		}

		this->_parent->updateGameStatus();

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
void GameBoard::uncoverCell(int x, int y, bool isRecursive) {
	TileType &t = this->storage[x][y];

	// check if this tile is flagged
	if(t.flagged == true) {
		return;
	}

	// check if this tile has been uncovered before
	if(std::find(this->uncoveredCells.begin(), this->uncoveredCells.end(), Point(x, y)) != this->uncoveredCells.end()) {
		return;
	}

	this->uncoveredCells.push_back(Point(x, y));


	// set uncovered flag
	t.uncovered = true;

	// is this a mine?
	if(t.isMine == true && !isRecursive) {
		// lmao game over
		this->_parent->gameOver();

		t.bgRed = true;
	} else {
		// calculate how many mines there are around this point
		t.surroundingMines = this->minesAroundCell(x, y);

		// if clue value is zero, uncover all adjacent tiles
		if(t.surroundingMines == 0) {
			if((x - 1) >= 0 && (y - 1) >= 0) {
				this->uncoverCell((x - 1), (y - 1), true);
			}
			// top middle
			if(/*(x - 0) >= 0 &&*/ (y - 1) >= 0) {
				this->uncoverCell((x - 0), (y - 1), true);
			}
			// top right
			if((x + 1) < this->gridW && (y - 1) >= 0) {
				this->uncoverCell((x + 1), (y - 1), true);
			}

			// middle left
			if((x - 1) >= 0/* && (y - 1) >= 0*/) {
				this->uncoverCell((x - 1), (y - 0), true);
			}
			// middle right
			if((x + 1) < this->gridW/* && (y - 1) >= 0*/) {
				this->uncoverCell((x + 1), (y - 0), true);
			}

			// bottom left
			if((x - 1) >= 0 && (y + 1) < this->gridH) {
				this->uncoverCell((x - 1), (y + 1), true);
			}
			// bottom middle
			if(/*(x - 0) >= 0 &&*/ (y + 1) < this->gridH) {
				this->uncoverCell((x - 0), (y + 1), true);
			}
			// bottom right
			if((x + 1) < this->gridW && (y + 1) < this->gridH) {
				this->uncoverCell((x + 1), (y + 1), true);
			}
		}
	}


	// force redraw of display
	this->redraw();
}

/**
 * Flags a cell, or turns it into a question mark if it's already flagged.
 */
void GameBoard::flagQuestion(int x, int y) {
	TileType &t = this->storage[x][y];

	// return if it's already uncovered
	if(t.uncovered == true) {
		return;
	}

	// if it's a question mark, remove it
	if(t.question == true) {
		t.question = false;
	}
	// if it's not flagged, flag it
	else if(t.flagged == false) {
		t.flagged = true;
	}
	// if it's flagged, turn it into a question mark
	else {
		t.flagged = false;
		t.question = true;
	}

	// force redraw of display
	this->redraw();
}

/**
 * Determines how many mines surround the cell at the given position.
 */
int GameBoard::minesAroundCell(int x, int y) {
	int mines = 0;

	// top left
	if((x - 1) >= 0 && (y - 1) >= 0) {
		mines += (this->storage[(x - 1)][(y - 1)].isMine == true) ? 1 : 0;
	}
	// top middle
	if(/*(x - 0) >= 0 &&*/ (y - 1) >= 0) {
		mines += (this->storage[(x - 0)][(y - 1)].isMine == true) ? 1 : 0;
	}
	// top right
	if((x + 1) < this->gridW && (y - 1) >= 0) {
		mines += (this->storage[(x + 1)][(y - 1)].isMine == true) ? 1 : 0;
	}

	// middle left
	if((x - 1) >= 0/* && (y - 1) >= 0*/) {
		mines += (this->storage[(x - 1)][(y - 0)].isMine == true) ? 1 : 0;
	}
	// middle right
	if((x + 1) < this->gridW/* && (y - 1) >= 0*/) {
		mines += (this->storage[(x + 1)][(y - 0)].isMine == true) ? 1 : 0;
	}

	// bottom left
	if((x - 1) >= 0 && (y + 1) < this->gridH) {
		mines += (this->storage[(x - 1)][(y + 1)].isMine == true) ? 1 : 0;
	}
	// bottom middle
	if(/*(x - 0) >= 0 &&*/ (y + 1) < this->gridH) {
		mines += (this->storage[(x - 0)][(y + 1)].isMine == true) ? 1 : 0;
	}
	// bottom right
	if((x + 1) < this->gridW && (y + 1) < this->gridH) {
		mines += (this->storage[(x + 1)][(y + 1)].isMine == true) ? 1 : 0;
	}

	return mines;
}

/**
 * Calculates the number of mines that are on the board and have NOT been
 * uncovered by the user.
 */
int GameBoard::getMinesRemaining() const {
	int remaining = 0;

	// calculate the number of mines
	for(int i = 0; i < this->gridW; i++) {
		for(int j = 0; j < this->gridH; j++) {
			TileType t = this->storage[i][j];

			if(t.isMine == true) {
				remaining++;
			}
		}
	}

	// subtract the number of uncovered/flagged mines
	for(int i = 0; i < this->gridW; i++) {
		for(int j = 0; j < this->gridH; j++) {
			TileType t = this->storage[i][j];

			if(t.flagged == true || (t.isMine == true && t.uncovered == true)) {
				remaining--;
			}
		}
	}

	return remaining;
}
