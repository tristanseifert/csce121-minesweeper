#include "MainWindow.h"

Fl_Menu_Item menuitems[] = {
	{ "&File",              0, 0, 0, FL_SUBMENU },
	{ "&New File",        0, (Fl_Callback *)nullptr },
	{ "&Open File...",    FL_COMMAND + 'o', (Fl_Callback *)nullptr },
	{ "&Insert File...",  FL_COMMAND + 'i', (Fl_Callback *)nullptr, 0, FL_MENU_DIVIDER },
	{ "&Save File",       FL_COMMAND + 's', (Fl_Callback *)nullptr },
	{ "Save File &As...", FL_COMMAND + FL_SHIFT + 's', (Fl_Callback *)nullptr, 0, FL_MENU_DIVIDER },
	{ "New &View", FL_ALT + 'v', (Fl_Callback *)nullptr, 0 },
	{ "&Close View", FL_COMMAND + 'w', (Fl_Callback *)nullptr, 0, FL_MENU_DIVIDER },
	{ "E&xit", FL_COMMAND + 'q', (Fl_Callback *)nullptr, 0 },
	{ 0 },
	{ "&Edit", 0, 0, 0, FL_SUBMENU },
	{ "&Undo",       FL_COMMAND + 'z', (Fl_Callback *)nullptr, 0, FL_MENU_DIVIDER },
	{ "Cu&t",        FL_COMMAND + 'x', (Fl_Callback *)nullptr },
	{ "&Copy",       FL_COMMAND + 'c', (Fl_Callback *)nullptr },
	{ "&Paste",      FL_COMMAND + 'v', (Fl_Callback *)nullptr },
	{ "&Delete",     0, (Fl_Callback *) nullptr },
	{ 0 },
	{ "&Search", 0, 0, 0, FL_SUBMENU },
	{ "&Find...",       FL_COMMAND + 'f', (Fl_Callback *)nullptr },
	{ "F&ind Again",    FL_COMMAND + 'g', (Fl_Callback *)nullptr },
	{ "&Replace...",    FL_COMMAND + 'r', (Fl_Callback *)nullptr },
	{ "Re&place Again", FL_COMMAND + 't', (Fl_Callback *)nullptr },
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
}

/**
 * Initializes the menu bar
 */
void MainWindow::_initMenuBar() {

}

MainWindow::~MainWindow() {

}
