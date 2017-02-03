#pragma once

#include "PreDefined.h"

enum class EnvType
{
	Empty,
	Grass,
	Ice,
	Iron,
	Wall,
	Sea,
	Eager,
};

EnvironmentBody ReadEnvFromString(std::string str);

EnvironmentBody CreateBasicEnv();