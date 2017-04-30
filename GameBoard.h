/**
 * Renders the contents of the Minesweeper game board, takes input from the user
 * (in the form of clicks on cells,) and actually drives the game.
 */
#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <FL/Fl_Box.H>

#include <vector>
#include <bitset>

class MainWindow;

// cell size, square pixels
static const int CELL_SIZE = 16;

// is this cell a mine?
static const int MINE_BIT = 0x0;
// was this cell flagged?
static const int FLAGGED_BIT = 0x1;
// has this cell been uncovered?
static const int UNCOVERED_BIT = 0xF;

typedef std::bitset<16> TileType;

class GameBoard : public Fl_Box {
	public:
		/**
		 * Creates a new game board. w/h is in cells
		 */
		GameBoard(int x, int y, int w, int h, MainWindow *parent);
		~GameBoard();

		void generateMines(int mines);

	protected:
		void updateGrid();

	private:
		MainWindow *_parent;

		// size of the grid
		int gridW, gridH;

		// storage
		std::vector<std::vector<TileType>> storage;

		// when set, all mines are displayed
		bool debugMode = true;
};

#endif
