#pragma once
#include <glm/glm.hpp>

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
};