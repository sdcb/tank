#include "pch.h"
#include "MapHelper.h"
#include <regex>

using namespace std;
using namespace Tank;

EnvType CharToEnvType(int v);

EnvironmentBody Tank::MapHelper::ReadFromString(std::string str)
{
	str = regex_replace(str, regex{ R"(\r\n)" }, "");
	if (str.size() != GridCountDouble * GridCountDouble)
		throw exception{ "str is not 26x26." };

	EnvironmentBody result;
	for (size_t i = 0; i < str.size(); ++i)
	{
		auto x = i % result.size();
		auto y = i / result.size();

		auto v = CharToEnvType(str[i]);
		result[y][x] = v;
	}
	return result;
}

void Tank::MapHelper::DeleteSpecialEnvs(EnvironmentBody & body)
{
	// eager
	SetPos4ToEnv(body, GridCountDouble / 2 - 1, GridCountDouble - 2, false, EnvType::Empty);
	// player born 1-2
	SetPos4ToEnv(body, GridCountDouble / 2 - 5, GridCountDouble - 2, false, EnvType::Empty);
	SetPos4ToEnv(body, GridCountDouble / 2 + 3, GridCountDouble - 2, false, EnvType::Empty);
}

EnvironmentBody Tank::MapHelper::CreateBasic()
{
	return ReadFromString(
		"                          "
		"                          "
		"                          "
		"                          "
		"                          "
		"                          "
		"                          "
		"                          "
		"                          "
		"                          "
		"                          "
		"                          "
		"                          "
		"                          "
		"                          "
		"                          "
		"                          "
		"                          "
		"                          "
		"                          "
		"                          "
		"                          "
		"                          "
		"           WWWW           "
		"           W  W           "
		"           W  W           ");
}

EnvironmentBody Tank::MapHelper::CreateTest()
{
	return ReadFromString(
		"  WWWWWWWWWW  WWWWWWWWWW  "
		"  WWWWWWWWWW  WWWWWWWWWW  "
		"WWWWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWW  WIIIIIIIIIII"
		"WWWWWWWWWWWW  WFFFFFFFFFFF");
}

EnvType CharToEnvType(int v)
{
	switch (v)
	{
	case ' ':
		return EnvType::Empty;
	case 'G':
		return EnvType::Grass;
	case 'W':
		return EnvType::Wall;
	case 'F':
		return EnvType::Iron;
	case 'S':
		return EnvType::Sea;
	case 'E':
		return EnvType::Eager;
	case 'I':
		return EnvType::Ice;
	default:
		throw exception{ "char text not supported." };
	}
}

void Tank::MapHelper::SetPos4ToEnv(EnvironmentBody& body, int x, int y, bool isSmall, EnvType type)
{
	body[y][x] = type;
	if (!isSmall)
	{
		auto ox = clamp(x + 1, 0, GridCountDouble - 1);
		auto oy = clamp(y + 1, 0, GridCountDouble - 1);

		body[y][ox] = type;
		body[oy][x] = type;
		body[oy][ox] = type;
	}
};