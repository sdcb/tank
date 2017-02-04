#include "pch.h"
#include "Environment.h"

using namespace std;

EnvType CharToEnvType(int v);

EnvironmentBody ReadEnvFromString4(std::string str)
{
	if (str.size() != GridCount * GridCount)
		throw exception{ "str is not 13x13." };

	EnvironmentBody result;
	for (size_t i = 0; i < str.size(); ++i)
	{
		auto x = i % GridCount;
		auto y = i / GridCount;

		auto v = CharToEnvType(str[i]);
		result[y * 2][x * 2] = v;
		result[y * 2][x * 2 + 1] = v;
		result[y * 2 + 1][x * 2] = v;
		result[y * 2 + 1][x * 2 + 1] = v;
	}
	return result;
}

EnvironmentBody ReadEnvFromString(std::string str)
{
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

EnvironmentBody CreateBasicEnv()
{
	return ReadEnvFromString(
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
