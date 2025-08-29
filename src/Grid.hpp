#pragma once
#include <vector>
#include <string>
#include<glm/glm.hpp>

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
private:
    std::vector<Tile> tiles;
    int width;
    int height;
    glm::vec2 pacmanStartPos;
    std::vector<glm::vec2> ghostStartPos;
};