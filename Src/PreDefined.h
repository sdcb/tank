#pragma once

const int GridUnitSize = 16;
const int GridUnitHalfSize = GridUnitSize / 2;
const int GridCount = 13;
const int GridSize = GridUnitSize * GridCount;
enum class EnvType;

typedef std::array<std::array<EnvType, GridCount>, GridCount> EnvironmentBody;