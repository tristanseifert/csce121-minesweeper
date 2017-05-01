#include "ScorePersistence.h"

#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

/**
 * Reads scores from the file.
 */
ScorePersistence::ScorePersistence() {
    this->_readScores();
}

/**
 * Write scores to file
 */
ScorePersistence::~ScorePersistence() {
    this->_writeScores();
}


/**
 * Sets the score for the given difficulty level.
 */
void ScorePersistence::setScoreForDifficulty(int difficulty, std::string name, int time) {
    this->data[difficulty].time = time;
    this->data[difficulty].name = name;

    cout << "Adding high score: " << name << ": " << time << endl;
    this->_writeScores();
}

void ScorePersistence::_readScores() {
    ifstream in("scores.dat");

    if(!in) {
        cout << "Couldn't open high scores: using defaults!" << endl;

        this->data.push_back(ScoreEntry(999, "No Name"));
        this->data.push_back(ScoreEntry(999, "No Name"));
        this->data.push_back(ScoreEntry(999, "No Name"));

        return;
    }

    // read three lines from the file
    string str;
    while(std::getline(in, str)) {
        int score; std::string name;

        istringstream isstr(str);
        isstr >> score >> name;

        this->data.push_back(ScoreEntry(score, name));
    }
}

void ScorePersistence::_writeScores() {
    ofstream out("scores.dat");

    for(int i = 0; i < 3; i++) {
        out << this->data[i].time << "\t";
        out << this->data[i].name << endl;
    }
}
