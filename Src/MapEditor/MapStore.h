#pragma once

#include "PreDefined.h"
#include <sqlite_modern_cpp.h>

namespace Tank
{
	class MapStore
	{
	public:
		MapStore();
		virtual ~MapStore();
		Tank::MapBody LoadMap(int mapId);
		void SaveMap(int mapId, Tank::MapBody);

	private:
		sqlite::database m_db;
	};
}