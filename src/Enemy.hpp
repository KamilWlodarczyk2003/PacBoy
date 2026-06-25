#pragma once
#include <glm/glm.hpp>
#include "Grid.hpp"

class Player;
class Shader;

enum class State
{
    Chase,
    Scatter,
    Dead,
    Scared
};

enum class Type
{
    Red,
    Pink,
    Blue,
    Orange
};

class Enemy
{
public:

    Enemy(Type enemy_type);

    glm::vec2 find_target();

    glm::vec2 get_position() const {return position;};

    void set_red_ghost(Enemy* red_ghost_v);
    void set_grid(Grid* grid_v);
    void assign_scatter();
    void update(float timer, int level);
    void calc_direction(glm::vec2 curr, glm::vec2 dest);
    void move();
    void render(Shader& shader, unsigned int cubeVAO);
    

private:
    Type type;
    State state = State::Scatter;

    glm::vec2 target;
    glm::vec2 scatter_target;
    glm::vec2 position;
    glm::vec2 direction;
    glm::vec2 spawn_point;
    glm::vec2 spawn_entrance;
    glm::vec3 color;

    bool is_at_center(glm::vec2 pos);

    bool state_change = false;
    int last_timer {-1};

    Enemy* red_ghost;
    Grid* grid;
    Player* player;

    const float SPEED = 0.05;
};