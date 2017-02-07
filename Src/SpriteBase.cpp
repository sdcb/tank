#include "pch.h"
#include "SpriteBase.h"
#include "TankSprite.h"
#include <DeviceResources.h>

using namespace Tank;
using namespace KennyKerr;
using D2D1::ColorF;

SpriteBase::SpriteBase(DrawCall drawCall) :
	m_drawCall(drawCall)
{
}

void Tank::SpriteBase::SetTopLeft(KennyKerr::Point2F topLeft)
{
	m_topLeft = topLeft;
}

KennyKerr::Point2F Tank::SpriteBase::GetTopLeft() const
{
	return m_topLeft;
}

void Tank::SpriteBase::DrawOutline(const SpriteBase & sprite, DX::DeviceResources * dxRes)
{
	auto unit = sprite.GetCurrentSprite();
	auto topLeft = sprite.GetTopLeft();
	auto size = GetSpriteUnitSize(unit);
	auto rect = RectF
	{
		topLeft.X, 
		topLeft.Y, 
		topLeft.X + size.Width, 
		topLeft.Y + size.Height, 
	};
	auto target = dxRes->GetD2DDeviceContext();
	target.DrawRectangle(rect, dxRes->GetOrCreateColor(ColorF::Red));
}
