#include "Enemy.hpp"
#include "Player.hpp"

Enemy::Enemy(Type enemy_type) : type(enemy_type)
{

}

glm::vec2 Enemy::find_target()
{
    glm::vec2 player_position = glm::round(player->getPosition());
    if(type == Type::Red)
    {
        target = player_position;
        return target;
    }
    else if(type == Type::Pink)
    {
        target = player_position + 3.0f * player->getCurrentDirection();
        return target;
    }
    else if(type == Type::Orange)
    {
        if(glm::distance(player_position, position) <= 8)
        {
            target = scatter_target;
        }
        else target = player_position;
    }
}