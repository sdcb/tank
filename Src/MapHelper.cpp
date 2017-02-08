#include "pch.h"
#include "MapHelper.h"
#include <regex>
#include "TankSprite.h"

using namespace std;
using namespace Tank;

EnvType CharToEnvType(char v);
char    EnvTypeToChar(EnvType v);

MapBody Tank::MapHelper::Deserialize(std::string str)
{
	str = regex_replace(str, regex{ R"(\r\n)" }, "");
	if (str.size() != GridCountDouble * GridCountDouble)
		throw exception{ "str is not 26x26." };

	MapBody result;
	for (size_t i = 0; i < str.size(); ++i)
	{
		auto x = i % result.size();
		auto y = i / result.size();

		auto v = CharToEnvType(str[i]);
		result[y][x] = v;
	}
	return result;
}

std::string Tank::MapHelper::Serialize(MapBody body)
{
	std::string result;
	for (size_t y = 0; y < body.size(); ++y)
	{
		for (size_t x = 0; x < body[y].size(); ++x)
		{
			result += EnvTypeToChar(body[y][x]);
		}
		result += "\r\n";
	}
	return result;
}

void Tank::MapHelper::DeleteSpecialEnvs(MapBody & body)
{
	// eager
	SetPos4ToEnv(body, GridCountDouble / 2 - 1, GridCountDouble - 2, false, EnvType::Empty);
	// player born 1-2
	SetPos4ToEnv(body, GridCountDouble / 2 - 5, GridCountDouble - 2, false, EnvType::Empty);
	SetPos4ToEnv(body, GridCountDouble / 2 + 3, GridCountDouble - 2, false, EnvType::Empty);
}

MapBody Tank::MapHelper::CreateBasic()
{
	return Deserialize(
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

const std::vector<Tank::SpriteUnit> Tank::MapHelper::GetEnvTypeUnits(Tank::EnvType env)
{
	switch (env)
	{
	case EnvType::Empty:
		return { SpriteUnit::Env_Empty };
	case EnvType::Grass:
		return { SpriteUnit::Env_Grass };
	case EnvType::Ice:
		return { SpriteUnit::Env_Ice };
	case EnvType::Iron:
		return { SpriteUnit::Env_Iron };
	case EnvType::Sea:
		return { SpriteUnit::Env_Sea0, SpriteUnit::Env_Sea1 };
	case EnvType::Wall:
		return { SpriteUnit::Env_Wall };
	default:
		throw std::exception{ "unknown env" };
	}
}

EnvType CharToEnvType(char v)
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
	case 'I':
		return EnvType::Ice;
	default:
		throw exception{ "char text not supported." };
	}
}

char EnvTypeToChar(EnvType v)
{
	switch (v)
	{
	case EnvType::Empty:
		return ' ';
	case EnvType::Grass:
		return 'G';
	case EnvType::Wall:
		return 'W';
	case EnvType::Iron:
		return 'F';
	case EnvType::Sea:
		return 'S';
	case EnvType::Ice:
		return 'I';
	default:
		throw exception{ "EnvType not supported." };
	}
}

void Tank::MapHelper::SetPos4ToEnv(MapBody& body, int x, int y, bool isSmall, EnvType type)
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