#include "GameBoard.h"

#include "MainWindow.h"

/**
 * Initializes the game board.
 */
GameBoard::GameBoard(int x, int y, int w, int h, MainWindow *parent) : Fl_Box(FL_DOWN_BOX, x, y, (w * CELL_SIZE), (h * CELL_SIZE), nullptr), _parent(parent) {

}

GameBoard::~GameBoard() {

}
