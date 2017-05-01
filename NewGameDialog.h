/**
 * This modal dialog allows the user to select what level (i.e. board size and
 * number of mines) they want to use for a new game.
 */
#ifndef NEW_GAME_DIALOG
#define NEW_GAME_DIALOG

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Radio_Round_Button.H>
#include <FL/Fl_Spinner.H>

class NewGameDialog : public Fl_Window {
    public:
        NewGameDialog();
        ~NewGameDialog();

        int getModalState() const {
            return this->returnVal;
        }

        int getBoardSizeW() const {
            return this->boardW;
        }
        int getBoardSizeH() const {
            return this->boardH;
        }
        int getNumMines() const {
            return this->mines;
        }

    public:
        enum Difficulty {
            Beginner    = 0,
            Intermediate= 1,
            Expert      = 2,
            Custom      = 3
        };

    private:
        friend void cancel_cb(Fl_Widget *, void *);
        friend void new_game_cb(Fl_Widget *, void *);
        friend void difficulty_radio_cb(Fl_Widget *, void *);

    private:
        Fl_Button *_newGameBtn;
        Fl_Button *_cancelBtn;

        Fl_Group *_radioGroup;

        Fl_Radio_Round_Button *_radBeginner;
        Fl_Radio_Round_Button *_radIntermediate;
        Fl_Radio_Round_Button *_radExpert;
        Fl_Radio_Round_Button *_radCustom;

        Fl_Spinner *_custW;
        Fl_Spinner *_custH;
        Fl_Spinner *_custMines;

    private:
        int returnVal = 0;

        Difficulty diff = Beginner;

        int boardW, boardH, mines;
};

#endif
