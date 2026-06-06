#pragma once
#include <glm/glm.hpp>

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

private:
    Type type;
    glm::vec2 target;
    Player* player;
    glm::vec2 scatter_target;
    glm::vec2 position;
};