#include "Enemy.hpp"
#include "Player.hpp"

#include <random>
#include <algorithm>

Enemy::Enemy(Type enemy_type) : type(enemy_type)
{
    assign_scatter();
}

void Enemy::set_red_ghost(Enemy* red_ghost_v)
{
    red_ghost = red_ghost_v;
}

void Enemy::set_grid(Grid* grid_v)
{
    grid = grid_v;
}

void Enemy::calc_direction(glm::vec2 curr, glm::vec2 dest)
{
    direction = dest - curr;
}

void Enemy::assign_scatter()
{
    switch (type)
    {
    case Type::Red:
        
        scatter_target = glm::vec2(grid->getWidth(), 0.0f);
        break;
    case Type::Pink:
        
        scatter_target = glm::vec2(0.0f, 0.0f);
        break;
    case Type::Orange:
        
        scatter_target = glm::vec2(0.0f, grid->getHeight());
        break;
    case Type::Blue:
        
        scatter_target = glm::vec2(grid->getWidth(), grid->getHeight());
        break;
    
    default:
        break;
    }
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
    else if(type == Type::Blue)
    {
        glm::vec2 two_spaces = player_position + 2.0f * player->getCurrentDirection();
        target = 2.0f * two_spaces - red_ghost->get_position();
    }
}

void Enemy::update()
{
    if(state == State::Scared)
    {
        if(state_change)
        {
            direction *= -1.0f;
            state_change = false;
        } else
        {
            std::vector<glm::vec2> possible_positions = grid->possible_moves(position);

            static std::random_device rd;
            static std::mt19937 gen(rd());
            glm::vec2 random_position;

            do
            {
                std::uniform_int_distribution<std::size_t> dist(
                0,
                possible_positions.size() - 1
                );
                random_position = possible_positions[dist(gen)];
            }
            while (random_position == position - direction);

            calc_direction(position, random_position);
        }
    }
    else if(state == State::Chase)
    {
        if(state_change)
        {
            direction *= -1.0f;
            state_change = false;
        } else
        {
            find_target();
            std::vector<glm::vec2> possible_positions = grid->possible_moves(position);

            int best_val = INT_MAX;
            glm::vec2 next_move;

            for(glm::vec2 pos : possible_positions)
            {
                if(pos != position * 1.0f)
                {
                    float dist = glm::distance(pos, target);

                    if(dist < best_val)
                    {
                        best_val = dist;
                        next_move = pos;
                    }
                }
            }

            calc_direction(position, next_move);
        }
        
    }
    else if(state == State::Scatter)
    {
        if(state_change)
        {
            direction *= -1.0f;
            state_change = false;
        }else
        {
            target = scatter_target;
            std::vector<glm::vec2> possible_positions = grid->possible_moves(position);

            int best_val = INT_MAX;
            glm::vec2 next_move;

            for(glm::vec2 pos : possible_positions)
            {
                if(pos != position * 1.0f)
                {
                    float dist = glm::distance(pos, target);

                    if(dist < best_val)
                    {
                        best_val = dist;
                        next_move = pos;
                    }
                }
            }

            calc_direction(position, next_move);
        }
    }
    else if(state == State::Dead)
    {
        if(position == spawn_entrance) 
        {
            target == spawn_point;
            calc_direction(position, target);
        }
        else
        {
            target = spawn_entrance;
            std::vector<glm::vec2> possible_positions = grid->possible_moves(position);

            int best_val = INT_MAX;
            glm::vec2 next_move;

            for(glm::vec2 pos : possible_positions)
            {

                float dist = glm::distance(pos, target);

                if(dist < best_val)
                {
                    best_val = dist;
                    next_move = pos;
                }
            }
            calc_direction(position, next_move);
        }
        
    }
}

