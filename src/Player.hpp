#pragma once
#include <glm/glm.hpp>

class Grid;
class Shader;

class Player
{
public:
    Player();
    Player(float x, float y);

    // basic moves
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    bool tryMove(float deltaX, float deltaY, Grid& grid);
    bool collectPellet(int x, int y, Grid& grid);

    // getters and setters
    glm::vec2 getPosition() const { return position; }
    void setPosition(float x, float y);
    void setPosition(const glm::vec2& pos);

    void render(Shader& shader, unsigned int cubeVAO);


    
private:
    glm::vec2 position;  // Pozycja na gridzie
    glm::vec3 color;     // Kolor gracza

    int score;
};