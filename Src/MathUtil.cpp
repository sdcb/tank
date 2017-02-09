#include "pch.h"
#include "MathUtil.h"

using namespace Tank;
using namespace DirectX;
using namespace KennyKerr;
using namespace DirectX::SimpleMath;
using D2D1::Matrix3x2F;

bool MathUtil::IsPointInSquare(Point2F mousePos, Point2F spriteTopLeft, float spriteSize)
{
	auto rect = MakeRectangleSquareByWH(spriteTopLeft, spriteSize);
	return rect.Contains(Vector2(mousePos.X, mousePos.Y));
}

Point2F MathUtil::GetMousePos(int x, int y, Matrix3x2F world)
{
	auto worldCopy = Matrix3x2F(world);
	worldCopy.Invert();
	return worldCopy.TransformPoint({ (float)x, (float)y });
}

Point2U MathUtil::GetMouseGridPos(Point2F mousePos)
{
	Point2U gridPos
	{
		(unsigned int)clamp(mousePos.X / GridUnitHalfSize, 0, GridCountDouble - 1),
		(unsigned int)clamp(mousePos.Y / GridUnitHalfSize, 0, GridCountDouble - 1)
	};
	return gridPos;
}

RectF MathUtil::MakeRectSquareByWH(Point2F topLeft, float width)
{
	return RectF
	{
		topLeft.X,
		topLeft.Y,
		topLeft.X + width,
		topLeft.Y + width
	};
}

Rectangle MathUtil::MakeRectangleSquareByWH(Point2F topLeft, float width)
{
	return Rectangle
	{
		long(topLeft.X),
		long(topLeft.Y),
		long(width),
		long(width)
	};
}
