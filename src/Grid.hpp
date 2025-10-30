#pragma once
#include <vector>
#include <string>
#include<glm/glm.hpp>

// Forward declaration
class Shader;

enum class Tile {
    Empty,
    Wall,
    Pellet,
    Energizer,
    PacmanStart,
    GhostStart
};

class Grid
{
public:
    bool loadFromFile(const std::string& path);
    Tile getTile(int x, int y) const;
    void render(Shader& shader, unsigned int cubeVAO);
    int getWidth() const;
    int getHeight() const;
    glm::vec2 getPacmanStartPosition() const;
private:
    std::vector<Tile> tiles;
    int width;
    int height;
    glm::vec2 pacmanStartPos;
    std::vector<glm::vec2> ghostStartPos;
};