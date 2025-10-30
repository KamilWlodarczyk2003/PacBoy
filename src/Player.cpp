#include "Player.hpp"
#include "Grid.hpp"
#include "../external/shader_s.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Player::Player() : position(0.0f, 0.0f), color(1.0f, 1.0f, 0.0f) // yellow
{
}

Player::Player(float x, float y) : position(x, y), color(1.0f, 1.0f, 0.0f) // yellow
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

bool Player::tryMove(float deltaX, float deltaY, const Grid& grid)
{
    std::cout << "Trying to move from (" << position.x << ", " << position.y << ") by (" << deltaX << ", " << deltaY << ")" << std::endl;
    
    // new position
    float newX = position.x + deltaX;
    float newY = position.y + deltaY;
    
    // grid border
    if (newX < 0 || newX >= grid.getWidth() || newY < 0 || newY >= grid.getHeight())
    {
        return false;
    }
    
    // wall colission
    Tile tileAtNewPos = grid.getTile((int)newX, (int)newY);
    if (tileAtNewPos == Tile::Wall)
    {
        return false;
    }
    
    // movement is possible
    position.x = newX;
    position.y = newY;
    std::cout << "Move successful! New position: (" << position.x << ", " << position.y << ")" << std::endl;
    return true;
}

void Player::setPosition(float x, float y)
{
    position.x = x;
    position.y = y;
}

void Player::setPosition(const glm::vec2& pos)
{
    position = pos;
}

void Player::render(Shader& shader, unsigned int cubeVAO)
{
    // Utwórz macierz transformacji dla gracza
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position.x, 0.1f, position.y)); // Lekko wyżej niż kafelki
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
