/**
 * Renders the contents of the Minesweeper game board, takes input from the user
 * (in the form of clicks on cells,) and actually drives the game.
 */
#ifndef GAMEBOARD_H
#define GAMEBOARD_H

class MainWindow;

#include <FL/Fl_Box.H>

// cell size, square pixels
static const int CELL_SIZE = 16;

class GameBoard : public Fl_Box {
	public:
		/**
		 * Creates a new game board. w/h is in cells
		 */
		GameBoard(int x, int y, int w, int h, MainWindow *parent);
		~GameBoard();

	private:

	private:
		MainWindow *_parent;
};

#endif
