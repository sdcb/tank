#pragma once

namespace Tank
{
	const float GridUnitSize = 16;
	const float GridUnitHalfSize = GridUnitSize / 2;
	const int GridCount = 13;
	const int GridCountDouble = GridCount * 2;
	const float GridSize = GridUnitSize * GridCount;
	enum class EnvType : unsigned char;
	enum class SpriteUnit : unsigned char;
	const int TankSpriteCount = 226;

	typedef std::array<std::array<EnvType, GridCountDouble>, GridCountDouble> MapBody;
	typedef std::array<KennyKerr::RectF, TankSpriteCount> TankSpriteMapArray;

	template <typename T, typename TMin, typename TMax>
	T clamp(T v, TMin min, TMax max)
	{
		if ((TMin)v < min) return (T)min;
		if ((TMax)v > max) return (T)max;
		return v;
	}
}