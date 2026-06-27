#include "Enemy.hpp"
#include "Player.hpp"

#include <random>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../external/shader_s.h"

Enemy::Enemy(Type enemy_type, Grid* grid_in, Player* player_in, glm::vec2 start_pos) : 
type(enemy_type), 
color(1.0f, 0.0f, 0.0f),
grid(grid_in),
player(player_in),
spawn_point(start_pos)
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
    glm::vec2 delta = dest - curr;

    if (glm::length(delta) > 0.0f)
        direction = glm::normalize(delta);
    else
        direction = glm::vec2(0.0f, 0.0f);
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
    // Each ghost type follows a different Pac-Man-style targeting rule.
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

    return target;
}

void Enemy::update(float timer, int level)
{
    int currentSecond = static_cast<int>(timer);

    // Scatter/chase schedules change at fixed timestamps for each level range.
    if (currentSecond != last_timer)
    {
        last_timer = currentSecond;
        if(level == 1)
        {
            if(currentSecond == 7 || currentSecond == 27 || currentSecond == 34 || currentSecond == 54 || currentSecond == 59 || currentSecond == 79 || currentSecond == 84)
            {state_change = true;}


            if(currentSecond == 27 || currentSecond == 54 || currentSecond == 79)
            {
                state = State::Scatter;
            } 
            else if(currentSecond == 7 || currentSecond == 34 || currentSecond == 59 || currentSecond == 84)
            {
                state = State::Chase;
            }
        }
        else if(level >= 2 && level <= 4)
        {
            if(currentSecond == 7 || currentSecond == 27 || currentSecond == 34 || currentSecond == 54 || currentSecond == 59)
            {state_change = true;}


            if(currentSecond == 27 || currentSecond == 54)
            {
                state = State::Scatter;
            } 
            else if(currentSecond == 7 || currentSecond == 34 || currentSecond == 59)
            {
                state = State::Chase;
            }
        }
        else if(level >= 5)
        {
            if(currentSecond == 5 || currentSecond == 25 || currentSecond == 30 || currentSecond == 50 || currentSecond == 55)
            {state_change = true;}


            if(currentSecond == 25 || currentSecond == 50)
            {
                state = State::Scatter;
            } 
            else if(currentSecond == 5 || currentSecond == 30 || currentSecond == 55)
            {
                state = State::Chase;
            }
        }
    }

    if (!is_at_center(position))
    {
        move();
        return;
    }
    position = glm::round(position);

    // Direction changes are made only at tile centers to keep grid movement stable.
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
            target = spawn_point;
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

    move();
}

void Enemy::move()
{
    position = position + direction*SPEED;
}

void Enemy::render(Shader& shader, unsigned int cubeVAO)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position.x, 0.1f, position.y));
    model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));

    int modelLoc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    int objectColorLoc = glGetUniformLocation(shader.ID, "objectColor");
    glUniform3f(objectColorLoc, color.r, color.g, color.b);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

bool Enemy::is_at_center(glm::vec2 pos)
{
    return glm::length(pos - glm::round(pos)) < 0.001f;
}
