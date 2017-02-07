#include "pch.h"
#include "SpriteButton.h"
#include "TankSprite.h"
#include <DeviceResources.h>
#include "StepTimer.h"
#include "MathUtil.h"

using namespace Tank;
using namespace KennyKerr;
using D2D1::ColorF;

Tank::SpriteButton::SpriteButton(KennyKerr::Point2F topLeft, std::vector<TankSpriteUnit> sprites): 
	m_topLeft(topLeft), 
	m_sprites(sprites)
{
}

void Tank::SpriteButton::SetTopLeft(KennyKerr::Point2F topLeft)
{
	m_topLeft = topLeft;
}

KennyKerr::Point2F Tank::SpriteButton::GetTopLeft() const
{
	return m_topLeft;
}

TankSpriteUnit Tank::SpriteButton::GetCurrentSprite() const
{
	return m_sprites[m_spriteId];
}

void Tank::SpriteButton::Update(DX::StepTimer const * timer)
{
	m_spriteId = m_spriteId + timer->GetFrameCount() / 10 % m_sprites.size();
}

void Tank::SpriteButton::Draw(DrawCall drawCall, DX::DeviceResources * dxRes)
{
	drawCall(GetCurrentSprite(), GetTopLeft());

	if (m_hover)
	{
		DrawOutline(this, dxRes);
	}
}

void Tank::SpriteButton::SetClickHandler(std::function<void()> clickHandler)
{
	m_clickHandler = clickHandler;
}

void Tank::SpriteButton::OnClick(KennyKerr::Point2F cursorPos)
{
}

void Tank::SpriteButton::OnMouseMove(KennyKerr::Point2F cursorPos)
{
	if (MathUtil::IsPointInSprite(cursorPos, m_topLeft, GetSpriteUnitSize(GetCurrentSprite()).Width))
	{
		m_hover = true;
	}
}

void Tank::SpriteButton::OnKeyUp(DirectX::Keyboard::Keys key)
{
}

void Tank::SpriteButton::DrawOutline(SpriteButton const * sprite, DX::DeviceResources * dxRes)
{
	auto unit = sprite->GetCurrentSprite();
	auto topLeft = sprite->GetTopLeft();
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
