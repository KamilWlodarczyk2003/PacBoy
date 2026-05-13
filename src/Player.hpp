#pragma once
#include <glm/glm.hpp>

class Grid;
class Shader;

// player's walking direction
enum class Direction
{
    Right,
    Left,
    Back,
    Forward
};

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

    bool canMoveTo(glm::vec2 targetPos, Grid& grid);
    bool update(Grid& grid);
    bool collectPellet(int x, int y, Grid& grid);

    // getters and setters
    glm::vec2 getPosition() const { return visual_position; }
    glm::vec2 getCurrentDirection() const {return curr_direction; }
    glm::vec2 getCameraDirection() const {return camera_direction;}
    void setPosition(float x, float y);
    void setPosition(const glm::vec2& pos);
    void setDirection(Direction direct);

    void render(Shader& shader, unsigned int cubeVAO);


    
private:
    glm::vec2 position;         // Position on the grid
    glm::vec3 color;            // Player's color
    glm::vec2 visual_position;   // drawn position on the grid
    glm::vec2 curr_direction;
    glm::vec2 target_direction;
    glm::vec2 camera_direction;

    int score;
    const float MOVEMENT_THRESHOLD_VIS = 0.1;
    const float MOVEMENT_THRESHOLD_POS = 0.05;
    const float SPEED = 0.05;
};