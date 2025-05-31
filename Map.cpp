#include "Map.h"
#include "Game.h"

string Map::getMapImages(int stage)
{
	return mapImages[stage];
}


Map::Map()
{
    // 스테이지 별 맵 이미지 저장
    for (int i = MapType::STAGE_1; i < MapType::END_OF_MAP_INDEX; ++i)
        mapImages[i] = MAP_BASE_PATH + "STAGE_" + to_string(i + 1) + ".png";
}

Map& Map::getInstance()
{
    static Map instance; // 인스턴스는 최초 호출 시에만 생성됨
    return instance;
}

void Map::CreateFromImage(const Image& image, vector<Object*>& objects)
{
    if (image.getSize().x == 0 && image.getSize().y == 0) return; // image가 초기화되지 않은 상태

    _grid.clear();
    _grid = vector<vector<Texture*>>(image.getSize().x, vector<Texture*>(image.getSize().y, nullptr));

    objects.clear();
    bodies.clear();
    textures.clear();

    Vector2f playerPosition;

    // 픽셀별 color에 따라 texture 지정
    for (int x = 0; x < _grid.size(); ++x)
    {
        for (int y = 0; y < _grid[x].size(); ++y) 
        {
            Color color = image.getPixel(x, y);
            if (color == Color::White) // 그냥 배경인 경우
                continue;

            Object* object = nullptr;

            // player 위치 지정
            if (color == Color::Red)
            {
                Game::getInstance()._playerPosition = Vector2f(_cellSize * x + _cellSize / 2.f, _cellSize * y + _cellSize / 2.f);
                continue;
            }

            // block 지정
            if (color == Color::Black)
                _grid[x][y] = &Resources::_textures["block.png"];

            // coin 지정
            else if (color == Color::Yellow)
                object = new Coin();

            // enemy 지정
            else if (color == Color::Blue)
            {
                // enemy를 pool에서 관리
                Enemy* enemy = Game::getInstance().GetEnemyPool()->GetAvailableEnemy(Vector2f(_cellSize * x + _cellSize / 2.f, _cellSize * y + _cellSize / 2.f));
                if (!enemy)
                {
                    cerr << "Failed to spawn enemy at " << _cellSize * x + _cellSize / 2.f << ", " << _cellSize * y + _cellSize / 2.f << endl;
                }
            }
            else if (color == Color::Green) // 세이브포인트 지정
                _grid[x][y] = &Resources::_textures["save.png"];
            else if (color == Color(0, 255, 255)) // 가시 방해물 지정
                object = new Spike();
            else if (color == Color(255, 0, 255)) // 부딪혀서 깨는 블록 지정
                object = new BoxFragile();
            else if (color == Color(200, 200, 200)) // 출구 지정
				_grid[x][y] = &Resources::_textures["exit.png"];
            else if (color == Color(215, 123, 186)) // N개 코인을 먹으면 열리는 블록 지정 (lock은 사라질 수 있는 블록인데 object처럼 관리해야하지 않을까?)
            { 
                _grid[x][y] = &Resources::_textures["lock.png"];
                textures["lock"].push_back({x, y});
            }
            else if (color == Color(118, 66, 138)) // 먹으면 점프력이 일정시간 높아지는 버섯 지정
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

            // 아래 코드가 비슷한데 모듈화 시킬 수 있지 않을까?
            if (_grid[x][y] == &Resources::_textures["block.png"])
            {
                b2BodyDef bodyDef; // 타일의 물리적 몸체 정의
                bodyDef.position.Set(_cellSize * x + _cellSize / 2.f, _cellSize * y + _cellSize / 2.f);
                b2Body* body = Physics::world->CreateBody(&bodyDef); // body = 새로 생성된 몸체에 대해 포인터로 참조가능

                // Ghost Collision 회피를 위한 chainShape 사용 (타일 경계 부근에서 캐릭터가 끼이는 현상 방지)
                // 타일모양대로 사각형 충돌체 그리기 (0 ~ 4 순서대로 이어지니 이점 유의)
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

                // unique_ptr로 래핑 후 Physics로 소유권 이동 (Physics에서 모두 관리하도록 하여 스테이지 종료 후 안전하게 메모리 해제되도록 함)
                unique_ptr<FixtureData> fixtureDataPtr(fixtureData);
                Physics::AddFixtureData(move(fixtureDataPtr));

                continue;
            }


            // 세이브포인트인 경우
            if (_grid[x][y] == &Resources::_textures["save.png"])
            {
                b2BodyDef bodyDef; // 타일의 물리적 몸체 정의
                bodyDef.position.Set(_cellSize * x + _cellSize / 2.f, _cellSize * y + _cellSize / 2.f);
                b2Body* body = Physics::world->CreateBody(&bodyDef); // body = 새로 생성된 몸체에 대해 포인터로 참조가능

                b2FixtureDef sensorFixtureDef;
                b2PolygonShape sensorPolygonShape;
                FixtureData* fixtureData = new FixtureData();

                fixtureData->type = FixtureDataType::SaveTile;
                fixtureData->mapX = x;
                fixtureData->mapY = y;

                sensorFixtureDef.userData.pointer = (uintptr_t)fixtureData; // 차후 Contact 상황 등에서 fixture를 통해 fixtureData 접근 가능
                sensorPolygonShape.SetAsBox(_cellSize / 2.f, _cellSize / 2.f);
                sensorFixtureDef.isSensor = true; // sensor : 충돌을 감지하지만 물리적인 상호작용은 하지 않는 특수한 fixture
                sensorFixtureDef.shape = &sensorPolygonShape;

                body->CreateFixture(&sensorFixtureDef);

                // unique_ptr로 래핑 후 Physics로 소유권 이동 (Physics에서 모두 관리하도록 하여 스테이지 종료 후 안전하게 메모리 해제되도록 함)
                unique_ptr<FixtureData> fixtureDataPtr(fixtureData);
                Physics::AddFixtureData(move(fixtureDataPtr));

                continue;
            }

            // 출구 블록인 경우
            if (_grid[x][y] == &Resources::_textures["exit.png"])
            {
				b2BodyDef bodyDef; // 타일의 물리적 몸체 정의
				bodyDef.position.Set(_cellSize * x + _cellSize / 2.f, _cellSize * y + _cellSize / 2.f);
				b2Body* body = Physics::world->CreateBody(&bodyDef); // body = 새로 생성된 몸체에 대해 포인터로 참조가능

				b2FixtureDef sensorFixtureDef;
				b2PolygonShape sensorPolygonShape;
                FixtureData* fixtureData = new FixtureData();

                fixtureData->type = FixtureDataType::ExitTile;
                fixtureData->mapX = x;
                fixtureData->mapY = y;

                sensorFixtureDef.userData.pointer = (uintptr_t)fixtureData; // 차후 Contact 상황 등에서 fixture를 통해 fixtureData 접근 가능
                sensorPolygonShape.SetAsBox(_cellSize / 2.f, _cellSize / 2.f);
                sensorFixtureDef.isSensor = true; // sensor : 충돌을 감지하지만 물리적인 상호작용은 하지 않는 특수한 fixture
                sensorFixtureDef.shape = &sensorPolygonShape;

                body->CreateFixture(&sensorFixtureDef);

                // unique_ptr로 래핑 후 Physics로 소유권 이동 (Physics에서 모두 관리하도록 하여 스테이지 종료 후 안전하게 메모리 해제되도록 함)
                unique_ptr<FixtureData> fixtureDataPtr(fixtureData);
                Physics::AddFixtureData(move(fixtureDataPtr));

                continue;
            }
            
            // 잠김 블록인 경우
            if (_grid[x][y] == &Resources::_textures["lock.png"])
            {
                b2BodyDef bodyDef; // 타일의 물리적 몸체 정의
                bodyDef.position.Set(_cellSize * x + _cellSize / 2.f, _cellSize * y + _cellSize / 2.f);
                b2Body* body = Physics::world->CreateBody(&bodyDef); // body = 새로 생성된 몸체에 대해 포인터로 참조가능

                // Ghost Collision 회피를 위한 chainShape 사용 (타일 경계 부근에서 캐릭터가 끼이는 현상 방지)
                // 타일모양대로 사각형 충돌체 그리기 (0 ~ 4 순서대로 이어지니 이점 유의)
                vector<b2Vec2> vs(4);
                vs[0].Set(-_cellSize / 2.f, -_cellSize / 2.f);
                vs[1].Set(+_cellSize / 2.f, -_cellSize / 2.f);
                vs[2].Set(+_cellSize / 2.f, +_cellSize / 2.f);
                vs[3].Set(-_cellSize / 2.f, +_cellSize / 2.f);

                b2ChainShape chain;
                chain.CreateLoop(&vs[0], 4);

                FixtureData* fixtureData = new FixtureData(); // FixtureData를 객체 안에 두고 unique_ptr로 객체 소멸 관리해야할듯 함
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

                // unique_ptr로 래핑 후 Physics로 소유권 이동 (Physics에서 모두 관리하도록 하여 스테이지 종료 후 안전하게 메모리 해제되도록 함)
                unique_ptr<FixtureData> fixtureDataPtr(fixtureData);
                Physics::AddFixtureData(move(fixtureDataPtr));

                continue;
            }
        }
    }

    int saveX = Game::getInstance()._savePositionX;
    int saveY = Game::getInstance()._savePositionY;

    // 세이브 정보가 없는 경우 return
    if (saveX == PLAYER_NO_SAVE_POSITION && saveY == PLAYER_NO_SAVE_POSITION) return;

    Game::getInstance()._playerPosition = Vector2f(_cellSize * saveX + _cellSize / 2.f, _cellSize * saveY + _cellSize / 2.f);
    _grid[saveX][saveY] = &Resources::_textures["save_used.png"];
}

void Map::Draw(Renderer& renderer)
{
    // 픽셀별 texture 지정된 것을 그림
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
