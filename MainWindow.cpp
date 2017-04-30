#include "MainWindow.h"

/**
 * Initialize main window.
 */
MainWindow::MainWindow(int w, int h, const char* t) : Fl_Window(w, h) {
	this->label(t);

	// create all the widgets lmao
	this->_initMenuBar();
}

/**
 * Initializes the menu bar
 */
void MainWindow::_initMenuBar() {

}

MainWindow::~MainWindow() {

}
