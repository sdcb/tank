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
		Eager,
		Born,
		Player1,
		Player2,
	};

	class MapHelper
	{
	public:
		static EnvironmentBody ReadFromString(std::string str);
		static void DeleteSpecialEnvs(EnvironmentBody& body);
		static void SetPos4ToEnv(EnvironmentBody& body, int x, int y, bool isSmall, EnvType type);

		static EnvironmentBody CreateBasic();
		static EnvironmentBody CreateTest();
	};
}