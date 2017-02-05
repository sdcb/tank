#pragma once

#include "PreDefined.h"

namespace Tank
{
	enum class EnvType : unsigned char
	{
		Empty,
		Grass,
		Ice,
		Iron,
		Wall,
		Sea,
	};

	enum class SpriteType : unsigned char
	{
		Eager,
		Born,
		Player1,
		Player2,
	};

	class MapHelper
	{
	public:
		static MapBody Deserialize(std::string str);
		static std::string Serialize(MapBody body);
		static void DeleteSpecialEnvs(MapBody& body);
		static void SetPos4ToEnv(MapBody& body, int x, int y, bool isSmall, EnvType type);

		static MapBody CreateBasic();
		static MapBody CreateTest();
	};
}