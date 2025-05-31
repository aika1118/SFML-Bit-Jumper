#include "Map.h"
#include "Game.h"

string Map::getMapImages(int stage)
{
	return mapImages[stage];
}


Map::Map()
{
    // �������� �� �� �̹��� ����
    for (int i = MapType::STAGE_1; i < MapType::END_OF_MAP_INDEX; ++i)
        mapImages[i] = MAP_BASE_PATH + "STAGE_" + to_string(i + 1) + ".png";
}

Map& Map::getInstance()
{
    static Map instance; // �ν��Ͻ��� ���� ȣ�� �ÿ��� ������
    return instance;
}

void Map::CreateFromImage(const Image& image, vector<Object*>& objects)
{
    if (image.getSize().x == 0 && image.getSize().y == 0) return; // image�� �ʱ�ȭ���� ���� ����

    _grid.clear();
    _grid = vector<vector<Texture*>>(image.getSize().x, vector<Texture*>(image.getSize().y, nullptr));

    objects.clear();
    bodies.clear();
    textures.clear();

    Vector2f playerPosition;

    // �ȼ��� color�� ���� texture ����
    for (int x = 0; x < _grid.size(); ++x)
    {
        for (int y = 0; y < _grid[x].size(); ++y) 
        {
            Color color = image.getPixel(x, y);
            if (color == Color::White) // �׳� ����� ���
                continue;

            Object* object = nullptr;

            // player ��ġ ����
            if (color == Color::Red)
            {
                Game::getInstance()._playerPosition = Vector2f(_cellSize * x + _cellSize / 2.f, _cellSize * y + _cellSize / 2.f);
                continue;
            }

            // block ����
            if (color == Color::Black)
                _grid[x][y] = &Resources::_textures["block.png"];

            // coin ����
            else if (color == Color::Yellow)
                object = new Coin();

            // enemy ����
            else if (color == Color::Blue)
            {
                // enemy�� pool���� ����
                Enemy* enemy = Game::getInstance().GetEnemyPool()->GetAvailableEnemy(Vector2f(_cellSize * x + _cellSize / 2.f, _cellSize * y + _cellSize / 2.f));
                if (!enemy)
                {
                    cerr << "Failed to spawn enemy at " << _cellSize * x + _cellSize / 2.f << ", " << _cellSize * y + _cellSize / 2.f << endl;
                }
            }
            else if (color == Color::Green) // ���̺�����Ʈ ����
                _grid[x][y] = &Resources::_textures["save.png"];
            else if (color == Color(0, 255, 255)) // ���� ���ع� ����
                object = new Spike();
            else if (color == Color(255, 0, 255)) // �ε����� ���� ��� ����
                object = new BoxFragile();
            else if (color == Color(200, 200, 200)) // �ⱸ ����
				_grid[x][y] = &Resources::_textures["exit.png"];
            else if (color == Color(215, 123, 186)) // N�� ������ ������ ������ ��� ���� (lock�� ����� �� �ִ� ����ε� objectó�� �����ؾ����� ������?)
            { 
                _grid[x][y] = &Resources::_textures["lock.png"];
                textures["lock"].push_back({x, y});
            }
            else if (color == Color(118, 66, 138)) // ������ �������� �����ð� �������� ���� ����
            {
				object = new Mushroom();
            }

            else
                continue;

            if (object) 
            {
                object->_position = Vector2f(_cellSize * x + _cellSize / 2.f, _cellSize* y + _cellSize / 2.f);
                objects.push_back(object);
                continue;
            }

            // �Ʒ� �ڵ尡 ����ѵ� ���ȭ ��ų �� ���� ������?
            if (_grid[x][y] == &Resources::_textures["block.png"])
            {
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
                fixtureData->canJump = true;

                b2FixtureDef fixtureDef;
                fixtureDef.userData.pointer = (uintptr_t)fixtureData;
                fixtureDef.density = 0.f;
                fixtureDef.shape = &chain;
                body->CreateFixture(&fixtureDef);

                // unique_ptr�� ���� �� Physics�� ������ �̵� (Physics���� ��� �����ϵ��� �Ͽ� �������� ���� �� �����ϰ� �޸� �����ǵ��� ��)
                unique_ptr<FixtureData> fixtureDataPtr(fixtureData);
                Physics::AddFixtureData(move(fixtureDataPtr));

                continue;
            }


            // ���̺�����Ʈ�� ���
            if (_grid[x][y] == &Resources::_textures["save.png"])
            {
                b2BodyDef bodyDef; // Ÿ���� ������ ��ü ����
                bodyDef.position.Set(_cellSize * x + _cellSize / 2.f, _cellSize * y + _cellSize / 2.f);
                b2Body* body = Physics::world->CreateBody(&bodyDef); // body = ���� ������ ��ü�� ���� �����ͷ� ��������

                b2FixtureDef sensorFixtureDef;
                b2PolygonShape sensorPolygonShape;
                FixtureData* fixtureData = new FixtureData();

                fixtureData->type = FixtureDataType::SaveTile;
                fixtureData->mapX = x;
                fixtureData->mapY = y;

                sensorFixtureDef.userData.pointer = (uintptr_t)fixtureData; // ���� Contact ��Ȳ ��� fixture�� ���� fixtureData ���� ����
                sensorPolygonShape.SetAsBox(_cellSize / 2.f, _cellSize / 2.f);
                sensorFixtureDef.isSensor = true; // sensor : �浹�� ���������� �������� ��ȣ�ۿ��� ���� �ʴ� Ư���� fixture
                sensorFixtureDef.shape = &sensorPolygonShape;

                body->CreateFixture(&sensorFixtureDef);

                // unique_ptr�� ���� �� Physics�� ������ �̵� (Physics���� ��� �����ϵ��� �Ͽ� �������� ���� �� �����ϰ� �޸� �����ǵ��� ��)
                unique_ptr<FixtureData> fixtureDataPtr(fixtureData);
                Physics::AddFixtureData(move(fixtureDataPtr));

                continue;
            }

            // �ⱸ ����� ���
            if (_grid[x][y] == &Resources::_textures["exit.png"])
            {
				b2BodyDef bodyDef; // Ÿ���� ������ ��ü ����
				bodyDef.position.Set(_cellSize * x + _cellSize / 2.f, _cellSize * y + _cellSize / 2.f);
				b2Body* body = Physics::world->CreateBody(&bodyDef); // body = ���� ������ ��ü�� ���� �����ͷ� ��������

				b2FixtureDef sensorFixtureDef;
				b2PolygonShape sensorPolygonShape;
                FixtureData* fixtureData = new FixtureData();

                fixtureData->type = FixtureDataType::ExitTile;
                fixtureData->mapX = x;
                fixtureData->mapY = y;

                sensorFixtureDef.userData.pointer = (uintptr_t)fixtureData; // ���� Contact ��Ȳ ��� fixture�� ���� fixtureData ���� ����
                sensorPolygonShape.SetAsBox(_cellSize / 2.f, _cellSize / 2.f);
                sensorFixtureDef.isSensor = true; // sensor : �浹�� ���������� �������� ��ȣ�ۿ��� ���� �ʴ� Ư���� fixture
                sensorFixtureDef.shape = &sensorPolygonShape;

                body->CreateFixture(&sensorFixtureDef);

                // unique_ptr�� ���� �� Physics�� ������ �̵� (Physics���� ��� �����ϵ��� �Ͽ� �������� ���� �� �����ϰ� �޸� �����ǵ��� ��)
                unique_ptr<FixtureData> fixtureDataPtr(fixtureData);
                Physics::AddFixtureData(move(fixtureDataPtr));

                continue;
            }
            
            // ��� ����� ���
            if (_grid[x][y] == &Resources::_textures["lock.png"])
            {
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

                FixtureData* fixtureData = new FixtureData(); // FixtureData�� ��ü �ȿ� �ΰ� unique_ptr�� ��ü �Ҹ� �����ؾ��ҵ� ��
                fixtureData->type = FixtureDataType::LockTile;
                fixtureData->mapX = x;
                fixtureData->mapY = y;
                fixtureData->canJump = true;

                b2FixtureDef fixtureDef;
                fixtureDef.userData.pointer = (uintptr_t)fixtureData;
                fixtureDef.density = 0.f;
                fixtureDef.shape = &chain;
                body->CreateFixture(&fixtureDef);

                bodies["lock"].push_back(body);

                // unique_ptr�� ���� �� Physics�� ������ �̵� (Physics���� ��� �����ϵ��� �Ͽ� �������� ���� �� �����ϰ� �޸� �����ǵ��� ��)
                unique_ptr<FixtureData> fixtureDataPtr(fixtureData);
                Physics::AddFixtureData(move(fixtureDataPtr));

                continue;
            }
        }
    }

    int saveX = Game::getInstance()._savePositionX;
    int saveY = Game::getInstance()._savePositionY;

    // ���̺� ������ ���� ��� return
    if (saveX == PLAYER_NO_SAVE_POSITION && saveY == PLAYER_NO_SAVE_POSITION) return;

    Game::getInstance()._playerPosition = Vector2f(_cellSize * saveX + _cellSize / 2.f, _cellSize * saveY + _cellSize / 2.f);
    _grid[saveX][saveY] = &Resources::_textures["save_used.png"];
}

void Map::Draw(Renderer& renderer)
{
    // �ȼ��� texture ������ ���� �׸�
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
