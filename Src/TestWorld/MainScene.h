#pragma once

#include <GameBase.h>
#include "MapEditor/MapStore.h"

#define PROJECT_NAME L"Test World"

class Game final: public GameBase
{
public:
	Game();

	void Draw(KennyKerr::Direct2D::DeviceContext target) override;
	void CreateWindowSizeResources() override;

private:
	Tank::MapBody  m_map;
	int            m_mapId;
	Tank::MapStore m_mapStore;

	// world translation
	D2D1::Matrix3x2F m_world;
};