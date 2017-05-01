#include "NewGameDialog.h"

/**
 * Cancel callback (simply close modal and set state)
 */
void cancel_cb(Fl_Widget *, void *ctx) {
	NewGameDialog *m = static_cast<NewGameDialog *>(ctx);

	m->returnVal = -1;

    m->hide();
}

/**
 * New game callback
 */
void new_game_cb(Fl_Widget *, void *ctx) {
	NewGameDialog *m = static_cast<NewGameDialog *>(ctx);
    m->returnVal = 0;

    m->hide();
}

/**
 * Creates the dialog and lays out all the widgets.
 */
NewGameDialog::NewGameDialog() : Fl_Window(300, 240, "New Game") {
    // set modal state
    this->set_modal();

    // clear variables used in display
    this->boardW = this->boardH = this->mines = 0;

    // create buttons
    this->_newGameBtn = new Fl_Button(10, (240 - 30 - 10), 85, 30);
    this->_newGameBtn->label("&New Game");
    this->_newGameBtn->callback(new_game_cb, this);

    this->_cancelBtn = new Fl_Button((300 - 85 - 10), (240 - 30 - 10), 85, 30);
    this->_cancelBtn->label("&Cancel");
    this->_cancelBtn->callback(cancel_cb, this);

    this->add(this->_newGameBtn);
    this->add(this->_cancelBtn);

    // create radio group
    this->_radioGroup = new Fl_Group(10, 10, (300 - (10 * 2)), 150);

    // create radio buttons
    this->_radBeginner = new Fl_Radio_Round_Button(10, 10, (300 - (10 * 2)), 25, "Beginner");
    this->_radBeginner->setonly();
    this->_radioGroup->add(this->_radBeginner);

    this->_radIntermediate = new Fl_Radio_Round_Button(10, 35, (300 - (10 * 2)), 25, "Intermediate");
    this->_radioGroup->add(this->_radIntermediate);

    this->_radExpert = new Fl_Radio_Round_Button(10, 60, (300 - (10 * 2)), 25, "Expert");
    this->_radioGroup->add(this->_radExpert);

    this->_radCustom = new Fl_Radio_Round_Button(10, 85, (300 - (10 * 2)), 25, "Custom");
    this->_radioGroup->add(this->_radCustom);

    // create spinners
    this->_custW = new Fl_Spinner(74, 110, 74, 25, "Width");
    this->_custW->minimum(1);
    this->_custW->maximum(99);
    this->add(this->_custW);

    this->_custH = new Fl_Spinner(74, 135, 74, 25, "Height");
    this->_custH->minimum(1);
    this->_custH->maximum(99);
    this->add(this->_custH);

    this->_custMines = new Fl_Spinner(74, 160, 74, 25, "Mines");
    this->_custMines->minimum(0);
    this->_custMines->maximum(256);
    this->add(this->_custMines);

    // add radio group
    this->_radioGroup->end();
    this->add(this->_radioGroup);
}

/**
 * Destroys any allocated resources.
 */
NewGameDialog::~NewGameDialog() {

}
