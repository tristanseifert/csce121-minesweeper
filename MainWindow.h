/**
 * Minesweeper main window
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Box.H>

#include "GameBoard.h"

class MainWindow : public Fl_Window {
	public:
    	MainWindow(int w, int h, const char* t);
    	~MainWindow();

		void startGame();

	private:
		void _initMenuBar();
		void _initStatusBar();

		void _newGameCb();

		void _reshape(int w, int h);

		void setupGameSized(int w, int h);

		Fl_Menu_Bar *_menuBar;

		Fl_Box *_statusBox;
		Fl_Box *_statusMines;
		Fl_Box *_statusTimer;

		GameBoard *board;
};

#endif
