#include "pch.h"
#include "SpriteButton.h"
#include "SpriteUnit.h"
#include <DeviceResources.h>
#include "StepTimer.h"
#include "MathUtil.h"
#include <sstream>

using namespace std;
using namespace Tank;
using namespace KennyKerr;
using D2D1::ColorF;

std::vector<std::string> string_split(std::string, char delim);

Tank::SpriteButton::SpriteButton(KennyKerr::Point2F topLeft, std::vector<SpriteUnit> sprites) :
	m_topLeft(topLeft),
	m_sprites(sprites),
	m_spriteId{ 0 },
	m_isX4{ false },
	m_visible{ true }, 
	m_showOutline{ false }
{
}

Tank::SpriteButton::~SpriteButton()
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

SpriteUnit Tank::SpriteButton::GetCurrentSprite() const
{
	return m_sprites[m_spriteId];
}

float Tank::SpriteButton::GetCurrentSpriteSize() const
{
	return GetSpriteUnitSize(GetCurrentSprite()).Width *
		(m_isX4 ? 2 : 1);
}

void Tank::SpriteButton::Update(DX::StepTimer const * timer, DX::DeviceResources * dxRes)
{
	if (m_hover)
	{
		m_spriteId = timer->GetSpriteId(500, m_sprites.size());
	}
	
	m_dxRes = dxRes;
}

void Tank::SpriteButton::Draw(DrawCall drawCall)
{
	if (!m_visible) return;

	auto topLeft = GetTopLeft();
	drawCall(GetCurrentSprite(), topLeft);
	if (m_isX4)
	{
		auto size = GetSpriteUnitSize(GetCurrentSprite());
		drawCall(GetCurrentSprite(), Point2F{topLeft.X, topLeft.Y + size.Height});
		drawCall(GetCurrentSprite(), Point2F{topLeft.X + size.Width, topLeft.Y});
		drawCall(GetCurrentSprite(), Point2F{topLeft.X + size.Width, topLeft.Y + size.Height});
	}

	if (m_hover || m_showOutline)
	{
		DrawOutline();
	}
}

ShortCut* Tank::SpriteButton::GetShortCut() const
{
	return m_shortCut.get();
}

void Tank::SpriteButton::SetShortCut(ShortCut shortCut)
{
	m_shortCut.reset(new ShortCut(shortCut));
}

// visible

void Tank::SpriteButton::SetVisible(bool visible) { m_visible = visible; }

bool Tank::SpriteButton::Visible() const { return m_visible; }

// isX4

void Tank::SpriteButton::SetIsX4(bool isX4) { m_isX4 = isX4; }

bool Tank::SpriteButton::IsX4() const { return m_isX4; }

// outline
void Tank::SpriteButton::ShowOutline(bool showOutline) { m_showOutline = showOutline; }

void Tank::SpriteButton::SetClickHandler(std::function<void()> clickHandler)
{
	m_clickHandler = clickHandler;
}

void Tank::SpriteButton::OnClick(KennyKerr::Point2F cursorPos)
{
	if (m_clickHandler)
	{
		if (Math::IsPointInSquare(cursorPos, m_topLeft, GetCurrentSpriteSize()))
		{
			m_clickHandler();
		}
	}
}

void Tank::SpriteButton::OnMouseMove(KennyKerr::Point2F cursorPos)
{
	m_hover = Math::IsPointInSquare(cursorPos, m_topLeft, GetCurrentSpriteSize());
}

void Tank::SpriteButton::OnKeyUp(DirectX::Keyboard::Keys key)
{
	if (m_clickHandler && m_shortCut)
	{
		auto& state = m_dxRes->GetKeyboardState();

		auto ctrl = state.LeftControl || state.RightControl;
		auto alt = state.LeftAlt || state.RightAlt;
		auto shift = state.LeftShift || state.RightShift;

		if (m_shortCut->Match(ctrl, alt, shift, key))
		{
			m_clickHandler();
		}
	}
}

void Tank::SpriteButton::DrawOutline()
{
	auto size = GetCurrentSpriteSize();
	auto topLeft = GetTopLeft();
	auto rect = RectF
	{
		topLeft.X,
		topLeft.Y,
		topLeft.X + size,
		topLeft.Y + size,
	};
	auto target = m_dxRes->GetD2DDeviceContext();
	target.DrawRectangle(rect, m_dxRes->GetOrCreateColor(ColorF::Red), Math::Clamp(size / 16.0f, 0.0f, 1.0f));
}

Tank::ShortCut::ShortCut() :
	Ctrl(false),
	Alt(false),
	Shift(false)
{
}

ShortCut Tank::ShortCut::Parse(std::string str)
{
	ShortCut result;

	auto tokens = string_split(str, '+');
	for (auto& token : tokens)
	{
		if (token == "Ctrl")
		{
			result.Ctrl = true;
		}
		else if (token == "Alt")
		{
			result.Alt = true;
		}
		else if (token == "Shift")
		{
			result.Shift = true;
		}
		else if (token.size() == 1)
		{
			result.Key = (DirectX::Keyboard::Keys)token[0];
		}
		else
		{
			throw std::exception{ "unknown shortcut" };
		}
	}
	return result;
}

ShortCut Tank::ShortCut::Key_(DirectX::Keyboard::Keys key)
{
	ShortCut shortCut;
	shortCut.Key = key;
	return shortCut;
}

ShortCut Tank::ShortCut::Shift_(DirectX::Keyboard::Keys key)
{
	ShortCut v;
	v.Shift = true;
	v.Key = key;
	return v;
}

inline bool Tank::ShortCut::Match(bool ctrl, bool alt, bool shift, DirectX::Keyboard::Keys key) const
{
	return
		ctrl == Ctrl &&
		alt == Alt &&
		shift == Shift &&
		key == Key;
}

std::vector<std::string> string_split(std::string str, char delim)
{
	std::stringstream ss{ str };
	std::string item;
	std::vector<std::string> result;
	while (std::getline(ss, item, delim))
	{
		result.emplace_back(item);
	}
	return result;
}
