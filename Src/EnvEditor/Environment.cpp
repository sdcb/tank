#include "pch.h"
#include "Environment.h"

using namespace std;

EnvType CharToEnvType(int v);

EnvironmentBody ReadEnvFromString(std::string str)
{
	if (str.size() != GridCount * GridCount)
		throw exception{ "str is not 13x13." };

	EnvironmentBody result;
	for (size_t i = 0; i < str.size(); ++i)
	{
		auto x = i % GridCount;
		auto y = i / GridCount;

		result[y][x] = CharToEnvType(str[i]);
	}
	return result;
}

EnvironmentBody CreateBasicEnv()
{
	return ReadEnvFromString(
		"             "
		"             "
		"             "
		"             "
		"             "
		"             "
		"             "
		"             "
		"             "
		"             "
		"             "
		"     WWW     "
		"     WEW     ");
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
		throw exception{"char text not supported."};
	}
}
