/**
 * Stores the high scores persistently on disk.
 */
#ifndef SCORE_PERSISTENCE_H
#define SCORE_PERSISTENCE_H

#include <string>
#include <vector>

struct ScoreEntry {
    ScoreEntry(int time, std::string name) : time(time), name(name) {}

    int time;
    std::string name;
};

class ScorePersistence {
    public:
        ScorePersistence();
        ~ScorePersistence();

        const ScoreEntry &entryForDifficulty(int difficulty) const {
            return this->data[difficulty];
        }

        void setScoreForDifficulty(int difficulty, std::string name, int time);

    public:
        std::vector<ScoreEntry> data;

    private:
        void _readScores();
        void _writeScores();

    private:
};

#endif
