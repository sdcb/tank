#pragma once

#include <dx/dx.h>
#include <DirectXTK/SimpleMath.h>
#include <DirectXTK/Mouse.h>
#include "PreDefined.h"
#include <d2d1.h>

namespace Tank
{
	enum class Direction
	{
		Left,
		Up,
		Right,
		Down,
	};

	class Math final
	{
	public:
		static bool IsPointInSquare(KennyKerr::Point2F mousePos, KennyKerr::Point2F spriteTopLeft, float spriteSize = GridUnitSize);
		static KennyKerr::Point2F GetMousePos(int x, int y, D2D1::Matrix3x2F world);
		static KennyKerr::Point2U Math::GetMouseGridPos(KennyKerr::Point2F mousePos);
		static KennyKerr::RectF MakeRectSquareByWH(KennyKerr::Point2F topLeft, float width);
		static DirectX::SimpleMath::Rectangle MakeRectangleSquareByWH(KennyKerr::Point2F topLeft, float width);
		static int Clamp(int v, int min, int max);
		static float Clamp(float v, float min, float max);
		static D2D1::Matrix3x2F CreateWorldTransform(KennyKerr::SizeF size);
		static KennyKerr::Point2F ByDirection(KennyKerr::Point2F p, Direction direction, float speed);
	};
}