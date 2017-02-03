#pragma once

#include "PreDefined.h"

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

EnvironmentBody ReadEnvFromString4(std::string str);
EnvironmentBody ReadEnvFromString(std::string str);

EnvironmentBody CreateBasicEnv();