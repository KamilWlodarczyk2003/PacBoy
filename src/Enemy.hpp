#pragma once
#include <glm/glm.hpp>
#include "Grid.hpp"

class Player;

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
    void update();
    void calc_direction(glm::vec2 curr, glm::vec2 dest);

private:
    Type type;
    State state;

    glm::vec2 target;
    glm::vec2 scatter_target;
    glm::vec2 position;
    glm::vec2 direction;

    bool state_change = false;

    Enemy* red_ghost;
    Grid* grid;
    Player* player;
};