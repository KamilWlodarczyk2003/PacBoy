#include "GameState.hpp"

void GameState::addScore(int points)
{
    score += points;
}

void GameState::loseLife()
{
    if (lives > 0)
    {
        lives--;
    }

    if (lives == 0)
    {
        gameOver = true;
    }
}

void GameState::nextLevel()
{
    level++;
}