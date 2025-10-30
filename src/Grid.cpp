#include "Grid.hpp"
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../external/GLAD/include/glad/glad.h"
#include "../external/shader_s.h"

bool Grid::loadFromFile(const std::string& path)
{
    std::ifstream in(path);
    if(!in.is_open())
    {
        std::cerr << "Cannot open level file: " << path << "\n";
        return false;
    }

    std::vector<std::string> lines;
    std::string line;

    while(std::getline(in, line))
    {
        if(!line.empty() && line.back() == '\r') line.pop_back();
        lines.push_back(line);
    }
    if (lines.empty()) return false;

    height = lines.size();
    width = lines[0].size();

    tiles.resize(width*height);

    for(int y{}; y < height; y++)
    {
        if((int)lines[y].size() != width)
        {
            std::cerr << "Level is not rectangular at line " << y << "/n";
            return false;
        }

        for (int x=0; x<width; ++x) {
            char c = lines[y][x];
            Tile t = Tile::Empty;
            switch(c) {
                case '#': t=Tile::Wall; break;
                case '.': t=Tile::Pellet; break;
                case '*': t=Tile::Energizer; break;
                case 'P': t=Tile::PacmanStart; pacmanStartPos={x,y}; break;
                case 'G': t=Tile::GhostStart; ghostStartPos.push_back({x,y}); break;
                case ' ': t=Tile::Empty; break;
                default: std::cerr<<"Unknown char "<<c<<"\n"; break;
            }
            tiles[y*width+x] = t;
        }

    }
    return true;
}

int Grid::getWidth() const
{
    return width;
}

int Grid::getHeight() const
{
    return height;
}

Tile Grid::getTile(int x, int y) const
{
    if(x < 0 || x >= width || y < 0 || y >= height)
    {
        std::cerr << "Invalid tile coordinates: (" << x << ", " << y << ")\n";
        return Tile::Empty;
    }
    return tiles[y*width+x];
}

glm::vec2 Grid::getPacmanStartPosition() const
{
    return pacmanStartPos;
}

void Grid::render(Shader& shader, unsigned int cubeVAO)
{
    for(int y{}; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            Tile tile = getTile(x,y);
            if (tile == Tile::Empty) continue;

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(x, 0.0f, y));

            // Set the model matrix
            int modelLoc = glGetUniformLocation(shader.ID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            // Set color based on tile type
            int objectColorLoc = glGetUniformLocation(shader.ID, "objectColor");
            switch (tile) {
                case Tile::Wall:
                    glUniform3f(objectColorLoc, 0.0f, 0.0f, 1.0f); // blue
                    break;
                case Tile::Pellet:
                    glUniform3f(objectColorLoc, 1.0f, 1.0f, 0.0f); // yellow
                    break;
                case Tile::Energizer:
                    glUniform3f(objectColorLoc, 1.0f, 0.0f, 1.0f); // purple
                    break;
                case Tile::PacmanStart:
                    glUniform3f(objectColorLoc, 1.0f, 1.0f, 1.0f); // white
                    break;
                case Tile::GhostStart:
                    glUniform3f(objectColorLoc, 1.0f, 0.0f, 0.0f); // red
                    break;
                default:
                    glUniform3f(objectColorLoc, 0.5f, 0.5f, 0.5f); // gray
                    break;
            }
            
            glBindVertexArray(cubeVAO);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
    }
}