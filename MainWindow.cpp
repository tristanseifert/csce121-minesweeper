#include "MainWindow.h"

#include <iostream>
#include <sstream>
#include <iomanip>

#include <FL/Fl_Menu_Item.H>
#include <FL/fl_ask.H>

using namespace std;

/**
 * Quit callback. Simply exit the program.
 */
void quit_cb(Fl_Widget *, void *ctx) {
	exit(0);
}

/**
 * Debug mode toggle callback
 */
void toggle_debug_cb(Fl_Widget *, void *ctx) {
	MainWindow *m = static_cast<MainWindow *>(ctx);

	m->board->toggleDebug();
}

/**
 * New game callback: this will open the "New Game" dialog.
 */
void menu_new_game_cb(Fl_Widget *, void *ctx) {
	MainWindow *m = static_cast<MainWindow *>(ctx);

	m->_newGameCb();
}

/**
 * Timer callback, called every second.
 */
void timer_cb(void *ctx) {
	(static_cast<MainWindow *>(ctx))->updateGameStatus();

	// repeat timer
	Fl::repeat_timeout(1.0, timer_cb, ctx);
}


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
	// menu items
	static const Fl_Menu_Item _menuItems[] = {
		{ "&Game", 0, 0, 0, FL_SUBMENU },
		{ "&New Game...", FL_COMMAND + 'n', (Fl_Callback *) menu_new_game_cb, this },
		{ "High Scores...", 0, (Fl_Callback *)nullptr, this, FL_MENU_DIVIDER },
		{ "Toggle Debug Mode", 0, (Fl_Callback *)toggle_debug_cb, this, FL_MENU_DIVIDER },
		{ "E&xit", FL_COMMAND + 'q', (Fl_Callback *) quit_cb, this },
		{ 0 },

		{ "&Help", 0, 0, 0, FL_SUBMENU },
		{ "About...", 0, (Fl_Callback *) nullptr, this },

		{ 0 },
		{ 0 }
	};

	// copy the menu items and create the menu bar
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
	this->_statusMines = new Fl_Box(FL_BORDER_BOX, 20, (30 + 18), 48, 32, "Mines");

	// game status
	this->_statusImage = new Fl_Box(FL_BORDER_BOX, 100, (30 + 18), 32, 32, nullptr);

	// timer (right)
	this->_statusTimer = new Fl_Box(FL_BORDER_BOX, 200, (30 + 18), 48, 32, "Timer");

	// add to window
	this->add(this->_statusBox);

	this->add(this->_statusMines);
	this->add(this->_statusImage);
	this->add(this->_statusTimer);

	// load images
	this->_imgOk = new Fl_PNG_Image("images/faces_05.png");
	this->_imgWon = new Fl_PNG_Image("images/faces_02.png");
	this->_imgDead = new Fl_PNG_Image("images/faces_03.png");
}

MainWindow::~MainWindow() {

}

/**
 * Called after the window has been shown.
 */
void MainWindow::startGame() {
	this->_newGameCb(true);
}

/**
 * Queries the user to determine what size the board should be.
 */
void MainWindow::_newGameCb(bool quitIfCanceled) {
	this->newGameDialog = new NewGameDialog();

	// show and wait for it to be closed
	this->newGameDialog->show();
	while(this->newGameDialog->shown()) {
		Fl::wait();
	}

	if(this->newGameDialog->getModalState() == 0) {
		cout << "Starting new game..." << endl;
	} else if(quitIfCanceled == true) {
		exit(0);
	}

	// set up the game
	int w = this->newGameDialog->getBoardSizeW();
	int h = this->newGameDialog->getBoardSizeH();
	int mines = this->newGameDialog->getNumMines();

	this->setupGameSized(w, h);
	this->board->generateMines(mines);

	// get whether we can store a high score
	this->canSaveHighScore = (this->newGameDialog->getDifficulty() != NewGameDialog::Custom);
	this->highScoreLevel = this->newGameDialog->getDifficulty();
}

/**
 * Resets the game state. This is called when the game ends, either because the
 * user has decided to stop the game, won the game, or the game ended because
 * the user exploded.
 */
void MainWindow::_resetGame() {
	// clear timer
	Fl::remove_timeout(timer_cb, this);
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
	this->_menuBar->size(this->w(), 30);

	// create board
	int boardY = (30 + 10) + (48 + 10);

	this->board = new GameBoard(10, boardY, w, h, this);

	this->add(this->board);

	// update game status
	this->_statusImage->image(this->_imgOk);
	this->updateGameStatus();

	// start timer
	Fl::repeat_timeout(1.0, timer_cb, this);
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
	this->_statusImage->position(((newW / 2) - 16), 48);
	this->_statusTimer->position((newW - (10 * 2) - 48), (30 + 18));

}

/**
 * Called at regular intervals to update the mines and time counters.
 */
void MainWindow::updateGameStatus() {
	// update mines counter
	stringstream ss;
	ss << setfill('0') << setw(3) << this->board->getMinesRemaining();

	this->_statusMines->copy_label(ss.str().c_str());

	// update timer
	ss.str(std::string());
	ss << setfill('0') << setw(3) << this->board->getTime();

	this->_statusTimer->copy_label(ss.str().c_str());

	// redraw image
	this->redraw();
}

/**
 * Called when the game has ended, because the user tried uncovering what was
 * a mine and died.
 */
void MainWindow::gameOver() {
	cout << "Game over lmao" << endl;

	this->_statusImage->image(this->_imgDead);

	this->_resetGame();
	this->updateGameStatus();

	Fl::wait();
	fl_beep();

	// display message
	fl_message_title("Game Over!");
	fl_message("Sorry, you lost. Try harder next time.");
}

/**
 * Called when all mines have been marked, i.e. the game is won.
 */
void MainWindow::gameWon() {
	this->_statusImage->image(this->_imgWon);

	this->_resetGame();
	this->updateGameStatus();

	// if this game wasn't a custom game,
}
