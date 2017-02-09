//
// Game.cpp
//

#include "pch.h"
#include "MainScene.h"
#include "TankSprite.h"
#include "MapHelper.h"
#include "SpriteType.h"
#include "TankDialog.h"
#include <MathUtil.h>

extern void ExitGame();

using namespace std;
using namespace Tank;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace KennyKerr;
using namespace KennyKerr::Direct2D;

using D2D1::ColorF;
using D2D1::Matrix3x2F;
using Microsoft::WRL::ComPtr;

Game::Game() :
	m_mapId(1),
	m_tankSpriteMap(CreateTankSpriteMap()),
	m_selectedEnv(EnvType::Empty),
	m_black([&]() {return m_deviceResources->GetOrCreateColor(ColorF::Black); }),
	m_red([&]() {return m_deviceResources->GetOrCreateColor(ColorF::Red); }),
	m_textFormat([&]() {return m_deviceResources->GetTextFormat(); }),
	m_keyboardState([&]() { return m_deviceResources->GetKeyboardState(); }),
	m_mouseState([&]() { return m_deviceResources->GetMouseState(); }),
	m_mousePos([&]() { return m_deviceResources->GetMousePos(); })
{
	m_map = m_mapStore.LoadMap(m_mapId);

	SetupLeftButtons();
	SetupRightButtons();	
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
	m_deviceResources->Update();

	// TODO: Add your game logic here.
	for (auto & button : m_buttons)
	{
		button->Update(&m_timer, m_deviceResources.get());
	}
}

void Game::Draw(KennyKerr::Direct2D::DeviceContext target)
{
	target.SetTransform(m_world);
	target.FillRectangle(RectF{ 0, 0, (float)GridSize, (float)GridSize }, m_black());
	for (auto & button : m_buttons)
	{
		button->Draw([&](SpriteUnit id, Point2F topLeft) {
			DrawUnit(id, topLeft);
		});
	}
	DrawBody();
	DrawRight();
}

// These are the resources that depend on the device.
void Game::CreateDeviceResources()
{
	auto device = m_deviceResources->GetD3DDevice();
	auto target = m_deviceResources->GetD2DDeviceContext();
	auto wic = m_deviceResources->GetWicFactory();

	// TODO: Initialize device dependent objects here (independent of window size).
	auto stream = wic.CreateStream();
	HR(stream.InitializeFromFilename(L"All.png", GENERIC_READ));
	auto decoder = wic.CreateFormatConverter();
	decoder.Initialize(wic.CreateDecoderFromStream(stream).GetFrame());
	m_bmp = target.CreateBitmapFromWicBitmap1(decoder);
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeResources()
{
	// TODO: Initialize windows-size dependent objects here.
	auto target = m_deviceResources->GetD2DDeviceContext();
	auto size = target.GetSize();
	auto scale = size.Height / GridSize;
	auto offsetX = (size.Width - size.Height) / 2;
	m_world =
		Matrix3x2F::Scale(scale, scale) *
		Matrix3x2F::Translation(offsetX, 0);
	m_deviceResources->SetWorldTranslation(m_world);
}

void Game::DrawUnit(SpriteUnit id, KennyKerr::Point2F topLeft)
{
	if (id == SpriteUnit::Env_Empty) return;
	auto imagePos = m_tankSpriteMap[(size_t)id];
	RectF source
	{
		imagePos.Left,
		imagePos.Top,
		imagePos.Right,
		imagePos.Bottom
	};
	RectF destination
	{
		topLeft.X,
		topLeft.Y,
		topLeft.X + source.Width(),
		topLeft.Y + source.Height()
	};
	m_deviceResources->GetD2DDeviceContext().DrawBitmap(
		m_bmp, destination, 1.0f, BitmapInterpolationMode::NearestNeighbor, source);
}

void Game::DrawEnv(EnvType env, KennyKerr::Point2F topLeft)
{
	auto units = MapHelper::GetEnvTypeUnits(env);
	auto id = m_timer.GetSpriteId(200, units.size());
	DrawUnit(units[id], topLeft);
}

void Game::DrawSprite(Tank::SpriteType sprite, KennyKerr::Point2F topLeft)
{
	switch (sprite)
	{
	case SpriteType::Eager:
		DrawUnit(SpriteUnit::Eager, topLeft);
		break;
	case SpriteType::Born:
		DrawUnit(SpriteUnit::Star_0 + m_timer.GetSpriteId(100, 4), topLeft);
		break;
	case SpriteType::Player1:
		DrawUnit(SpriteUnit::P1_1_U0 + m_timer.GetSpriteId(200, 2), topLeft);
		break;
	case SpriteType::Player2:
		DrawUnit(SpriteUnit::P2_1_U0 + m_timer.GetSpriteId(200, 2), topLeft);
		break;
	case SpriteType::Left:
		DrawUnit(SpriteUnit::P1_2_L0 + m_timer.GetSpriteId(200, 2), topLeft);
		break;
	case SpriteType::Right:
		DrawUnit(SpriteUnit::P1_2_R0 + m_timer.GetSpriteId(200, 2), topLeft);
		break;
	default:
		throw exception{ "Unkonwn sprite type." };
	}
}

void Game::GoOffsetMap(int offset)
{
	if (m_pendingSaveBtn->Visible() && !TankDialog::Confirm(L"尚未保存，确定要放弃已编辑的地图吗？"))
	{
		return;
	}
	m_mapId = clamp(m_mapId + offset, 1, INFINITE);
	m_map = m_mapStore.LoadMap(m_mapId);
	m_pendingSaveBtn->SetVisible(false);
}

void Game::SaveMap()
{
	m_mapStore.SaveMap(m_mapId, m_map);
	m_pendingSaveBtn->SetVisible(false);
}

void Game::DrawBody()
{
	auto mouseState = m_deviceResources->GetMouseState();
	auto gridPos = MathUtil::GetMouseGridPos(m_mousePos());
	auto cursorInBody = MathUtil::IsPointInSquare(m_mousePos(), Point2F(), GridSize);
	if (mouseState.leftButton && cursorInBody)
	{
		// cursor click
		MapHelper::SetPos4ToEnv(m_map, gridPos.X, gridPos.Y, m_small, m_selectedEnv);
		m_pendingSaveBtn->SetVisible(true);
	}

	auto target = m_deviceResources->GetD2DDeviceContext();
	auto localEnv = m_map;
	MapHelper::DeleteSpecialEnvs(m_map);

	// cursor environment
	MapHelper::SetPos4ToEnv(localEnv, gridPos.X, gridPos.Y, m_small, m_selectedEnv);

	DrawSpecialEnvironments();

	for (size_t row = 0; row < localEnv.size(); ++row)
	{
		for (size_t col = 0; col < localEnv[row].size(); ++col)
		{
			Point2F topLeft
			{
				GridUnitHalfSize * col,
				GridUnitHalfSize * row
			};
			DrawEnv(localEnv[row][col], topLeft);
		}
	}

	if (cursorInBody)
	{
		// cursor red rectangle
		int times = m_small ? 1 : 2;
		Point2F topLeft
		{
			GridUnitHalfSize * gridPos.X,
			GridUnitHalfSize * gridPos.Y
		};
		auto currentRect = MathUtil::MakeRectSquareByWH(topLeft, times * GridUnitHalfSize);
		target.DrawRectangle(currentRect, m_red());
	}
}

void Game::DrawRight()
{
	auto target = m_deviceResources->GetD2DDeviceContext();
	auto topLeft = Point2F{ GridSize + 2.0f, 2.0f };
	auto levelStr = fmt::format(L"L{0}", m_mapId);
	target.DrawText(levelStr.c_str(), levelStr.size(), m_textFormat(), MathUtil::MakeRectSquareByWH(topLeft, 800), m_black());
}

void Game::DrawSpecialEnvironments()
{
	// born
	DrawSprite(SpriteType::Born, Point2F
	{
		0, 0
	});
	DrawSprite(SpriteType::Born, Point2F
	{
		GridUnitHalfSize * (GridCountDouble / 2 - 1), 0
	});
	DrawSprite(SpriteType::Born, Point2F
	{
		GridUnitHalfSize * (GridCountDouble - 2), 0
	});
	// player born 1-2
	DrawSprite(SpriteType::Player1, Point2F
	{
		GridUnitHalfSize * (GridCountDouble / 2 - 5), GridUnitHalfSize * (GridCountDouble - 2)
	});
	DrawSprite(SpriteType::Player2, Point2F
	{
		GridUnitHalfSize * (GridCountDouble / 2 + 3), GridUnitHalfSize * (GridCountDouble - 2)
	});
	// center eager
	DrawSprite(SpriteType::Eager, Point2F
	{
		GridUnitHalfSize * (GridCountDouble / 2 - 1),
		GridUnitHalfSize * (GridCountDouble - 2),
	});
}

void Game::SetupRightButtons()
{
	auto topLeft = Point2F{ GridSize + 2.0f, 2.0f };
	auto MoveOffset = [&topLeft]() { topLeft.Y += GridUnitSize + 2.0f; };

	MoveOffset();
	m_leftBtn = NewSpriteButton(topLeft, { SpriteUnit::P1_2_L0, SpriteUnit::P1_2_L1 });
	m_leftBtn->SetClickHandler(bind(&Game::GoOffsetMap, this, -1));
	m_leftBtn->SetShortCut(ShortCut::Key_(Keyboard::Left));

	MoveOffset();
	m_rightBtn = NewSpriteButton(topLeft, { SpriteUnit::P1_2_R0, SpriteUnit::P1_2_R1 });
	m_rightBtn->SetClickHandler(bind(&Game::GoOffsetMap, this, 1));
	m_rightBtn->SetShortCut(ShortCut::Key_(Keyboard::Right));

	MoveOffset();
	m_pendingSaveBtn = NewSpriteButton(topLeft, { SpriteUnit::P2_4_U0, SpriteUnit::P2_4_U1 });
	m_pendingSaveBtn->SetClickHandler(bind(&Game::SaveMap, this));
	m_pendingSaveBtn->SetShortCut(ShortCut::Parse("Ctrl+S"));
	m_pendingSaveBtn->SetVisible(false);
}

void Game::SetupLeftButtons()
{
	Point2F topLeft{ -GridUnitSize - 2.0f, 2.0f };
	std::vector<EnvType> envSeq = { EnvType::Empty, EnvType::Grass, EnvType::Ice, EnvType::Iron, EnvType::Sea, EnvType::Wall };

	auto HandleEnvs = [&](bool isSmall) {
		for (size_t i = 0; i < envSeq.size(); ++i)
		{
			auto env = envSeq[i];
			auto size = isSmall ? GridUnitHalfSize : GridUnitSize;

			auto theBtn = NewSpriteButton(topLeft, MapHelper::GetEnvTypeUnits(env));
			m_envButtons.push_back(theBtn);
			
			theBtn->SetIsX4(!isSmall);
			theBtn->SetShortCut(isSmall ?
				ShortCut::Shift_((Keyboard::Keys)('1' + i)) : 
				ShortCut::Key_((Keyboard::Keys)('1' + i)));
			theBtn->SetClickHandler([isSmall, this, env, theBtn]() {
				m_small = isSmall;
				m_selectedEnv = env;
				for (auto btn : m_envButtons)
					btn->ShowOutline(false);
				theBtn->ShowOutline(true);
			});

			topLeft.Y += size + 2.0f;
		}
	};
	HandleEnvs(false);
	HandleEnvs(true);
	m_envButtons[0]->ShowOutline(true);
}

SpriteButton * Game::NewSpriteButton(KennyKerr::Point2F topLeft, std::vector<SpriteUnit> unites)
{
	m_buttons.emplace_back(make_unique<SpriteButton>(topLeft, unites));
	return m_buttons[m_buttons.size() - 1].get();
}

void Game::OnClick(int x, int y)
{
	auto pos = MathUtil::GetMousePos(x, y, m_world);
	for (auto & button : m_buttons)
	{
		button->OnClick(pos);
	}
}

void Game::OnMouseMove(int x, int y)
{
	auto pos = MathUtil::GetMousePos(x, y, m_world);
	for (auto & button : m_buttons)
	{
		button->OnMouseMove(pos);
	}
}

void Game::OnKeyUp(DirectX::Keyboard::Keys key)
{
	for (auto & button : m_buttons)
	{
		button->OnKeyUp(key);
	}
}

void Game::OnDeviceLost()
{
	// TODO: Add Direct3D resource cleanup here.
	m_bmp.Reset();
}