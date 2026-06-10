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

private:
    Type type;
    glm::vec2 target;
    Player* player;
    glm::vec2 scatter_target;
    glm::vec2 position;
    glm::vec2 direction;
    Enemy* red_ghost;
    Grid* grid;
};