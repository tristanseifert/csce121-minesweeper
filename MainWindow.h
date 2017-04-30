/**
 * Minesweeper main window
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>

class MainWindow : public Fl_Window {
	public:
    	MainWindow(int w, int h, const char* t);
    	~MainWindow();

	private:
		void _initMenuBar();
};

#endif
