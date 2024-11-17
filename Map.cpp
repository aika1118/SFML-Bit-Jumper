#include "Map.h"

Map::Map()
{
}

Map& Map::getInstance()
{
    static Map instance; // �ν��Ͻ��� ���� ȣ�� �ÿ��� ������
    return instance;
}

Vector2f Map::CreateFromImage(const Image& image, vector<Object*>& objects)
{
    _grid.clear();
    _grid = vector<vector<Texture*>>(image.getSize().x, vector<Texture*>(image.getSize().y, nullptr));

    objects.clear();

    Vector2f playerPosition;

    for (int x = 0; x < _grid.size(); ++x)
    {
        for (int y = 0; y < _grid[x].size(); ++y)
        {
            Color color = image.getPixel(x, y);
            if (color == Color::White) // �׳� ����� ���
                continue;

            Object* object = nullptr;

            if (color == Color::Red)
                playerPosition = Vector2f(_cellSize * x + _cellSize / 2.f, _cellSize * y + _cellSize / 2.f);

            if (color == Color::Black)
                _grid[x][y] = &Resources::_textures["block.png"];

            b2BodyDef bodyDef; // Ÿ���� ������ ��ü ����
            bodyDef.position.Set(_cellSize * x + _cellSize / 2.f, _cellSize * y + _cellSize / 2.f);
            b2Body* body = Physics::world->CreateBody(&bodyDef); // body = ���� ������ ��ü�� ���� �����ͷ� ��������

            // Ghost Collision ȸ�Ǹ� ���� chainShape ��� (Ÿ�� ��� �αٿ��� ĳ���Ͱ� ���̴� ���� ����)
            // Ÿ�ϸ���� �簢�� �浹ü �׸��� (0 ~ 4 ������� �̾����� ���� ����)
            vector<b2Vec2> vs(4);
            vs[0].Set(-_cellSize / 2.f, -_cellSize / 2.f);
            vs[1].Set(+_cellSize / 2.f, -_cellSize / 2.f);
            vs[2].Set(+_cellSize / 2.f, +_cellSize / 2.f);
            vs[3].Set(-_cellSize / 2.f, +_cellSize / 2.f);

            b2ChainShape chain;
            chain.CreateLoop(&vs[0], 4);

            FixtureData* fixtureData = new FixtureData();
            fixtureData->type = FixtureDataType::MapTile;
            fixtureData->mapX = x;
            fixtureData->mapY = y;

            b2FixtureDef fixtureDef;
            fixtureDef.userData.pointer = (uintptr_t)fixtureData;
            fixtureDef.density = 0.f;
            fixtureDef.shape = &chain;
            body->CreateFixture(&fixtureDef);
        }
    }

    return playerPosition;
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
