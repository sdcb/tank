#pragma once

#include <GameBase.h>
#include "MapEditor/MapStore.h"

#define PROJECT_NAME L"Test World"

class Game: public GameBase
{
public:
	Game();

private:
	Tank::MapBody  m_map;
	int            m_mapId;
	Tank::MapStore m_mapStore;
};