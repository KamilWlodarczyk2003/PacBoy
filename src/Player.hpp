#pragma once
#include <glm/glm.hpp>
#include "Rect.hpp"
#include "GameState.hpp"

class Grid;
class Shader;

// Movement commands are relative to the camera direction.
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
    Player(float x, float y, GameState *state);

    // Grid-space movement helpers.
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    bool canMoveTo(glm::vec2 targetPos, Grid& grid);
    bool update(Grid& grid);
    bool collectPellet(int x, int y, Grid& grid);

    // Player state accessors.
    glm::vec2 getPosition() const { return visual_position; }
    glm::vec2 getCurrentDirection() const {return curr_direction; }
    glm::vec2 getCameraDirection() const {return camera_direction;}
    Rect getPlayerRect() const { return Rect{visual_position.x, visual_position.y, WIDTH}; }
    bool getCollided() const { return collided; }
    void setPosition(float x, float y);
    void setPosition(const glm::vec2& pos);
    void setDirection(Direction direct);

    void render(Shader& shader, unsigned int cubeVAO);
    void setCollided(bool collidedValue);


    
private:
    GameState *gameState;
    glm::vec2 position;          // Logical grid position.
    glm::vec3 color;             // Render color.
    glm::vec2 visual_position;   // Interpolated draw position.
    glm::vec2 curr_direction;
    glm::vec2 target_direction;
    glm::vec2 camera_direction;

    bool collided{false};
    const float MOVEMENT_THRESHOLD_VIS = 0.1;
    const float MOVEMENT_THRESHOLD_POS = 0.05;
    const float SPEED = 0.05;
    const float WIDTH = 1.0f;
};
