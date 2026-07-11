#pragma once

class GameState
{
public:
    void addScore(int points);
    void loseLife();
    void nextLevel();

    int getScore() const { return score; }
    int getLives() const { return lives; }
    int getLevel() const { return level; }
    bool isGameOver() const { return gameOver; }

private:
    int score{0};
    int lives{3};
    int level{1};
    bool gameOver{false};
};