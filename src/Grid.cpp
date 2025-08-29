#include "Grid.hpp"
#include <fstream>
#include <iostream>

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