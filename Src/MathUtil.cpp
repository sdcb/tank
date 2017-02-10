#include "pch.h"
#include "MathUtil.h"

using namespace Tank;
using namespace DirectX;
using namespace KennyKerr;
using namespace DirectX::SimpleMath;
using D2D1::Matrix3x2F;

bool Math::IsPointInSquare(Point2F mousePos, Point2F spriteTopLeft, float spriteSize)
{
	auto rect = MakeRectangleSquareByWH(spriteTopLeft, spriteSize);
	return rect.Contains(Vector2(mousePos.X, mousePos.Y));
}

Point2F Math::GetMousePos(int x, int y, Matrix3x2F world)
{
	auto worldCopy = Matrix3x2F(world);
	worldCopy.Invert();
	return worldCopy.TransformPoint({ (float)x, (float)y });
}

Point2U Math::GetMouseGridPos(Point2F mousePos)
{
	Point2U gridPos
	{
		(unsigned int)Math::Clamp(int(mousePos.X / GridUnitHalfSize), 0, GridCountDouble - 1),
		(unsigned int)Math::Clamp(int(mousePos.Y / GridUnitHalfSize), 0, GridCountDouble - 1)
	};
	return gridPos;
}

RectF Math::MakeRectSquareByWH(Point2F topLeft, float width)
{
	return RectF
	{
		topLeft.X,
		topLeft.Y,
		topLeft.X + width,
		topLeft.Y + width
	};
}

Rectangle Math::MakeRectangleSquareByWH(Point2F topLeft, float width)
{
	return Rectangle
	{
		long(topLeft.X),
		long(topLeft.Y),
		long(width),
		long(width)
	};
}

int Tank::Math::Clamp(int v, int min, int max)
{
	if (v < min) return min;
	if (v > max) return max;
	return v;
}

float Tank::Math::Clamp(float v, float min, float max)
{
	if (v < min) return min;
	if (v > max) return max;
	return v;
}

D2D1::Matrix3x2F Tank::Math::CreateWorldTransform(KennyKerr::SizeF size)
{
	auto scale = size.Height / GridSize;
	auto offsetX = (size.Width - size.Height) / 2;
	return 
		Matrix3x2F::Scale(scale, scale) *
		Matrix3x2F::Translation(offsetX, 0);
}

KennyKerr::Point2F Tank::Math::ByDirection(KennyKerr::Point2F p, Direction direction, float speed)
{
	switch (direction)
	{
	case Direction::Left:
		p.X -= speed;
		break;
	case Direction::Up:
		p.Y -= speed;
		break;
	case Direction::Right:
		p.X += speed;
		break;
	case Direction::Down:
		p.Y += speed;
		break;
	default:
		throw std::exception{ "unknown direction." };
	}
	return p;
}
