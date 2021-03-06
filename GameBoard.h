/**
 * Renders the contents of the Minesweeper game board, takes input from the user
 * (in the form of clicks on cells,) and actually drives the game.
 */
#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Box.H>

#include <vector>
#include <bitset>
#include <ctime>

class MainWindow;

// cell size, square pixels
static const int CELL_SIZE = 16;

// is this cell a mine?
static const int MINE_BIT = 0x00;
// was this cell flagged?
static const int FLAGGED_BIT = 0x01;
// the background of the cell should be red
static const int RED_BIT = 0x0D;
// has this cell been uncovered?
static const int UNCOVERED_BIT = 0x0F;

struct TileType {
	bool uncovered = false;

	bool isMine = false;
	bool flagged = false;
	bool question = false;

	bool bgRed = false;

	// number of surrounding mines (show if non-negative)
	int surroundingMines = -1;
};

struct Point {
	Point(int x, int y) : x(x), y(y) {}

	int x, y;
};

inline bool operator==(const Point& lhs, const Point& rhs){ return (lhs.x == rhs.x) && (lhs.y == rhs.y); }
inline bool operator!=(const Point& lhs, const Point& rhs){ return !(lhs == rhs); }

class GameBoard : public Fl_Box {
	friend class MainWindow;

	public:
		/**
		 * Creates a new game board. w/h is in cells
		 */
		GameBoard(int x, int y, int w, int h, MainWindow *parent);
		~GameBoard();

		void generateMines(int mines);

		void toggleDebug();

		virtual void draw();
		virtual int handle(int);

		int getMinesRemaining() const;
		int getTime() const {
			return difftime(time(nullptr), this->start);
		}

	private:
		void updateGrid();

		void _loadImages();

		int minesAroundCell(int x, int y);

		void uncoverCell(int x, int y, bool isRecursive = false);
		void flagQuestion(int x, int y);

		void _checkWinStatus();

	private:
		Fl_PNG_Image *_imgMine;
		Fl_PNG_Image *_imgMineCrossed;
		Fl_PNG_Image *_imgFlag;

		MainWindow *_parent;

		// size of the grid
		int gridW, gridH;

		// storage
		std::vector<std::vector<TileType>> storage;

		// when set, all mines are displayed
		bool debugMode = false;
		bool revealMines = false;

		// all cells uncovered
		std::vector<Point> uncoveredCells;

		// time at which the game was started (i.e. the board is constructed)
		time_t start;
};

#endif
