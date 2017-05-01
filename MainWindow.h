/**
 * Minesweeper main window
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>

#include "GameBoard.h"

class MainWindow : public Fl_Window {
	friend class GameBoard;

	public:
    	MainWindow(int w, int h, const char* t);
    	~MainWindow();

		void startGame();

	protected:
		void updateGameStatus();
		void gameOver();
		void gameWon();

	private:
		friend void timer_cb(void *);
		friend void toggle_debug_cb(Fl_Widget *, void *);

	private:
		void _initMenuBar();
		void _initStatusBar();

		void _newGameCb();
		void _resetGame();

		void _reshape(int w, int h);

		void setupGameSized(int w, int h);

	private:
		Fl_Menu_Bar *_menuBar;

		Fl_Box *_statusBox;
		Fl_Box *_statusImage;
		Fl_Box *_statusMines;
		Fl_Box *_statusTimer;

		Fl_PNG_Image *_imgOk;
		Fl_PNG_Image *_imgWon;
		Fl_PNG_Image *_imgDead;

		GameBoard *board;
};

#endif
