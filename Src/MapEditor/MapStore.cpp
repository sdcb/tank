#include "pch.h"
#include "MapStore.h"
#include "MapHelper.h"

using namespace std;
using namespace Tank;

Tank::MapStore::MapStore() :
	m_db{ u"Tank.db" }
{
	m_db << uR"~(
CREATE TABLE IF NOT EXISTS [Map] 
(
    Id      INTEGER NOT NULL PRIMARY KEY, 
    Content TEXT    NOT NULL
))~";
}

Tank::MapStore::~MapStore()
{
}

Tank::MapBody Tank::MapStore::LoadMap(int mapId)
{
	MapBody body = MapHelper::CreateBasic();
	m_db << uR"~(
SELECT [Content] FROM [Map] WHERE [Id] = ?
)~" << mapId >> [&](string str) {
		body = MapHelper::Deserialize(str);
	};

	return body;
}

void Tank::MapStore::SaveMap(int mapId, Tank::MapBody body)
{
	string str = MapHelper::Serialize(body);
	m_db << uR"~(
INSERT OR REPLACE INTO [Map] (Id, Content) VALUES(?, ?)
)~" << mapId << str;
}
