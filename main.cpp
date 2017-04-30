/**
 * Entrypoint for FLTK minesweeper program.
 */
#include "MainWindow.h"

int main(int argc, char *argv[]) {
	// set up FLTK
	MainWindow w(320, 500, "Minesweeper (CSCE121-502)");

	// show window
	w.show(argc, argv);

	// start main loop for FLTK
	return Fl::run();
}
