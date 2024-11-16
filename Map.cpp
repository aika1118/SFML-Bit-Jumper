#include "Map.h"

Map::Map()
{
}

Map& Map::getInstance()
{
    static Map instance; // 인스턴스는 최초 호출 시에만 생성됨
    return instance;
}

Vector2f Map::CreateFromImage(const Image& image, vector<Object*>& objects)
{
    _grid.clear();
    _grid = vector<vector<Texture*>>(image.getSize().x, vector<Texture*>(image.getSize().y, nullptr));

    objects.clear();

    Vector2f characterPosition;

    for (int x = 0; x < _grid.size(); ++x)
    {
        for (int y = 0; y < _grid[x].size(); ++y)
        {
            Color color = image.getPixel(x, y);
            if (color == Color::White) // 그냥 배경인 경우
                continue;

            Object* object = nullptr;

            if (color == Color::Red)
                characterPosition = Vector2f(_cellSize * x + _cellSize / 2.f, _cellSize * y + _cellSize / 2.f);

            if (color == Color::Black)
                _grid[x][y] = &Resources::_textures["block.png"];
        }
    }

    return characterPosition;
}

void Map::Draw(Renderer& renderer)
{
    int x = 0;
    for (const vector<Texture*>& cells : _grid)
    {
        int y = 0;
        for (const Texture* cell : cells)
        {
            if (cell)
                renderer.Draw(*cell, Vector2f(_cellSize * x + _cellSize / 2.f, _cellSize * y + _cellSize / 2.f), Vector2f(_cellSize, _cellSize));

            ++y;
        }
        ++x;
    }
}
