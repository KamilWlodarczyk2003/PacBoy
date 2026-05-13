#include "Player.hpp"
#include "Grid.hpp"
#include "../external/shader_s.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Player::Player() :  position(0.0f, 0.0f), 
                    color(1.0f, 1.0f, 0.0f), 
                    score(0), 
                    visual_position(0.0f, 0.0f), 
                    curr_direction(1.0f, 0.0f),
                    camera_direction(1.0f, 0.0f),
                    target_direction(1.0f, 0.0f)
{
}

Player::Player(float x, float y) :  position(x, y), 
                                    color(1.0f, 1.0f, 0.0f), 
                                    score(0), 
                                    visual_position(x, y), 
                                    curr_direction(1.0f, 0.0f),
                                    camera_direction(1.0f, 0.0f),
                                    target_direction(1.0f, 0.0f)
{
}

void Player::moveUp()
{
    position.y -= 1.0f; // W gridzie y=0 to góra, więc moveUp zmniejsza y
}
void Player::moveDown()
{
    position.y += 1.0f;
}
void Player::moveRight()
{
    position.x += 1.0f;
}
void Player::moveLeft()
{
    position.x -= 1.0f;
}

bool Player::collectPellet(int x, int y, Grid& grid)
{
    Tile tile = grid.getTile(x,y);
    if(tile == Tile::Pellet)
    {
        score += 10;
        grid.collectTile(x,y);
        return true;
    } else if(tile == Tile::Energizer)
    {
        score += 50;
        grid.collectTile(x,y);
        return true;
    }
    return false;
    
}

// setting player's direction considering current direction
void Player::setDirection(Direction direct)
{
    switch(direct)
    {
        case Direction::Forward:
            target_direction = camera_direction;
            break;
        case Direction::Back:
            target_direction = -camera_direction;
            break;
        case Direction::Left:
            target_direction = glm::vec2(camera_direction.y, -camera_direction.x);
            break;
        case Direction::Right:
            target_direction = glm::vec2(-camera_direction.y, camera_direction.x);
            break;
    }
}

bool Player::update(Grid& grid)
{
    float fracX = visual_position.x - glm::floor(visual_position.x);
    float fracY = visual_position.y - glm::floor(visual_position.y);
    
    if(curr_direction != glm::vec2(0.0f, 0.0f))
    {
        camera_direction = curr_direction;
    }

    if((fracX < 0.07f || fracX > 0.93f) && (fracY < 0.07f || fracY > 0.93f))
    {
        collectPellet(round(visual_position.x), round(visual_position.y), grid);
        std::cout <<"CHECKING"<<std::endl;
        Tile new_pos_tile = grid.getTile(
            glm::round(visual_position.x + curr_direction.x), 
            glm::round(visual_position.y + curr_direction.y));

        if(new_pos_tile == Tile::Wall)
        {
            new_pos_tile = grid.getTile(
                glm::round(visual_position.x + target_direction.x), 
                glm::round(visual_position.y + target_direction.y)
            );

            if(new_pos_tile != Tile::Wall)
            {
                curr_direction = target_direction;
            }
            else
            {
                curr_direction = glm::vec2(0.0f, 0.0f); 
            }
        }else
        {
            new_pos_tile = grid.getTile(
                glm::round(visual_position.x + target_direction.x), 
                glm::round(visual_position.y + target_direction.y)
            );
            if(new_pos_tile != Tile::Wall)
            {
                curr_direction = target_direction;
            }
        }
    }

    visual_position += curr_direction * SPEED;

    std::cout << "visual: (" << visual_position.x << ", " << visual_position.y << ") real: (" << position.x << ", " << position.y << ")" << std::endl;
    std::cout << "curr: (" << curr_direction.x << ", " << curr_direction.y << ") target: (" << target_direction.x << ", " << target_direction.y << ")" << std::endl;
    return true;
}

void Player::setPosition(float x, float y)
{
    position.x = x;
    position.y = y;

    visual_position.x = x;
    visual_position.y = y;
}

void Player::setPosition(const glm::vec2& pos)
{
    position = pos;
    visual_position = pos;
}

void Player::render(Shader& shader, unsigned int cubeVAO)
{
    // Utwórz macierz transformacji dla gracza
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(visual_position.x, 0.1f, visual_position.y)); // Lekko wyżej niż kafelki
    model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f)); // Trochę mniejszy niż kafelki
    
    // Wyślij macierz modelu do shadera
    int modelLoc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    
    // Ustaw kolor gracza (żółty)
    int objectColorLoc = glGetUniformLocation(shader.ID, "objectColor");
    glUniform3f(objectColorLoc, color.r, color.g, color.b);
    
    // Narysuj gracza
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
