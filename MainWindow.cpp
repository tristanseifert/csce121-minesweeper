#include "MainWindow.h"

#include <iostream>

#include <FL/Fl_Menu_Item.H>

using namespace std;

/**
 * Quit callback. Simply exit the program.
 */
static void quit_cb(Fl_Widget*, void*) {
	exit(0);
}


// define menu items
static const Fl_Menu_Item _menuItems[] = {
	{ "&Game",              0, 0, 0, FL_SUBMENU },
	{ "&New Game...",		FL_COMMAND + 'n', (Fl_Callback *)nullptr },
	{ "&Reset Game", 		0, (Fl_Callback *)nullptr, 0, FL_MENU_DIVIDER },
	{ "High Scores...", 	0, (Fl_Callback *)nullptr, 0, FL_MENU_DIVIDER },
	{ "Toggle Debug Mode", 		0, (Fl_Callback *)nullptr, 0, FL_MENU_DIVIDER },
	{ "E&xit", FL_COMMAND + 'q', (Fl_Callback *) quit_cb, 0 },
	{ 0 },

	{ "&Help", 0, 0, 0, FL_SUBMENU },
	{ "About...", 			0, (Fl_Callback *)nullptr },

	{ 0 },
	{ 0 }
};

/**
 * Initialize main window.
 */
MainWindow::MainWindow(int w, int h, const char* t) : Fl_Window(w, h) {
	this->label(t);

	// create all the widgets lmao
	this->_initMenuBar();
	this->_initStatusBar();
}

/**
 * Initializes the menu bar
 */
void MainWindow::_initMenuBar() {
	this->_menuBar = new Fl_Menu_Bar(0, 0, this->w(), 30);
	this->_menuBar->copy(_menuItems);

	// add to window
	this->add(this->_menuBar);
}

/**
 * Initializes the status bar along the top of the window.
 */
void MainWindow::_initStatusBar() {
	this->_statusBox = new Fl_Box(FL_EMBOSSED_BOX, 10, (30 + 10), 32, 32, nullptr);

	// mines remaining (left)
	this->_statusMines = new Fl_Box(FL_BORDER_BOX, 20, (30 + 18), 64, 32, "099");

	// timer (right)
	this->_statusTimer = new Fl_Box(FL_BORDER_BOX, 200, (30 + 18), 64, 32, "000");

	// add to window
	this->add(this->_statusBox);

	this->add(this->_statusMines);
	this->add(this->_statusTimer);
}

MainWindow::~MainWindow() {

}

/**
 * Called after the window has been shown.
 */
void MainWindow::startGame() {
	this->_newGameCb();
}

/**
 * Queries the user to determine what size the board should be.
 */
void MainWindow::_newGameCb() {
	this->setupGameSized(16, 16);
}

/**
 * Set up a game with a grid of the given size. This will reshape the window and
 * all controls in it.
 */
void MainWindow::setupGameSized(int w, int h) {
	// clear any old shit
	delete this->board;

	// reshape window
	this->_reshape(w, h);

	// create board
	int boardY = (30 + 10) + (48 + 10);

	this->board = new GameBoard(10, boardY, w, h, this);
	this->board->generateMines(48);

	this->add(this->board);
}

/**
 * Reshapes the window and existing controls, given a board size.
 */
void MainWindow::_reshape(int w, int h) {
	// calculate the total size of the window
	int newW = (w * CELL_SIZE) + (10 * 2);
	int newH = (30 + 10) + (48 + 10) + ((h * CELL_SIZE) + 10);

	this->size(newW, newH);

	// reshape status bar
	this->_statusBox->size((newW - (10 * 2)), 48);
	this->_statusTimer->position((newW - (10 * 2) - 64), (30 + 18));

}

/**
 * Called at regular intervals to update the mines and time counters.
 */
void MainWindow::updateGameStatus() {
	// this->board->redraw();
	this->redraw();
}

/**
 * Called when the game has ended, because the user tried uncovering what was
 * a mine and died.
 */
void MainWindow::gameOver() {
	cout << "Game over lmao" << endl;

	this->updateGameStatus();
}

/**
 * Called when all mines have been marked, i.e. the game is won.
 */
void MainWindow::gameWon() {
	this->updateGameStatus();
}
