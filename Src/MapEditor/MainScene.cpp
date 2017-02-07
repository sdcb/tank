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
	m_mousePos([&]() { return m_deviceResources->GetMousePos(); }), 
	m_envSequence(
{
	EnvType::Empty, EnvType::Grass, EnvType::Ice, EnvType::Iron, EnvType::Sea, EnvType::Wall
})
{
	m_map = m_mapStore.LoadMap(1);
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
	m_deviceResources->Update();

	// TODO: Add your game logic here.
	for (size_t i = 0; i < m_envSequence.size(); ++i)
	{
		if (m_keyboardState().IsKeyDown(Keyboard::Keys(Keyboard::D1 + i)))
		{
			m_selectedEnv = m_envSequence[i];
		}
	}
	if (m_keyboardState().F1)
	{
		m_small = false;
	}
	if (m_keyboardState().F2)
	{
		m_small = true;
	}

	if (m_keyboardState().Right && m_timer.GetFrameCount() % 7 == 0)
	{
		GoOffsetMap(1);
	}
	else if (m_keyboardState().Left && m_timer.GetFrameCount() % 7 == 0)
	{
		GoOffsetMap(-1);
	}
	if (m_keyboardState().Enter)
	{
		SaveMap();
	}
}

void Game::Draw(KennyKerr::Direct2D::DeviceContext target)
{
	target.SetTransform(m_world);
	target.FillRectangle(RectF{ 0, 0, (float)GridSize, (float)GridSize }, m_black());
	DrawLeft();
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

void Game::DrawUnit(TankSpriteUnit id, KennyKerr::Point2F topLeft)
{
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
	switch (env)
	{
	case EnvType::Empty:
		// do nothing;
		return;
	case EnvType::Grass:
		DrawUnit(TankSpriteUnit::Env_Grass, topLeft);
		return;
	case EnvType::Ice:
		DrawUnit(TankSpriteUnit::Env_Ice, topLeft);
		break;
	case EnvType::Iron:
		DrawUnit(TankSpriteUnit::Env_Iron, topLeft);
		break;
	case EnvType::Sea:
		DrawUnit(TankSpriteUnit::Env_Sea0 + m_timer.GetFrameCount() / 10 % 2, topLeft);
		break;
	case EnvType::Wall:
		DrawUnit(TankSpriteUnit::Env_Wall, topLeft);
		break;
	default:
		throw exception("Env is not supported.");
	}
}

void Game::DrawEnv4(EnvType env, KennyKerr::Point2F topLeft)
{
	DrawEnv(env, topLeft);
	DrawEnv(env, Point2F{ topLeft.X + GridUnitHalfSize, topLeft.Y });
	DrawEnv(env, Point2F{ topLeft.X, topLeft.Y + GridUnitHalfSize });
	DrawEnv(env, Point2F{ topLeft.X + GridUnitHalfSize, topLeft.Y + GridUnitHalfSize });
}

void Game::DrawSprite(Tank::SpriteType sprite, KennyKerr::Point2F topLeft)
{
	switch (sprite)
	{
	case SpriteType::Eager:
		DrawUnit(TankSpriteUnit::Eager, topLeft);
		break;
	case SpriteType::Born:
		DrawUnit(TankSpriteUnit::Star_0 + m_timer.GetFrameCount() / 10 % 4, topLeft);
		break;
	case SpriteType::Player1:
		DrawUnit(TankSpriteUnit::P1_1_U0 + m_timer.GetFrameCount() / 10 % 2, topLeft);
		break;
	case SpriteType::Player2:
		DrawUnit(TankSpriteUnit::P2_1_U0 + m_timer.GetFrameCount() / 10 % 2, topLeft);
		break;
	case SpriteType::Left:
		DrawUnit(TankSpriteUnit::P1_2_L0 + m_timer.GetFrameCount() / 10 % 2, topLeft);
		break;
	case SpriteType::Right:
		DrawUnit(TankSpriteUnit::P1_2_R0 + m_timer.GetFrameCount() / 10 % 2, topLeft);
		break;
	default:
		throw exception{ "Unkonwn sprite type." };
	}
}

void Game::AddClickHandler(KennyKerr::Point2F topLeft, float size, std::function<void()> clickHandler)
{
	auto rectangle = MathUtil::MakeRectangleSquareByWH(topLeft, size);
	auto rect = MathUtil::MakeRectSquareByWH(topLeft, size);
	auto target = m_deviceResources->GetD2DDeviceContext();
	if (rectangle.Contains(Vector2{ m_mousePos().X, m_mousePos().Y }))
	{
		target.DrawRectangle(rect, m_red());
		if (m_mouseState().leftButton && m_timer.GetFrameCount() % 5 == 0)
		{
			clickHandler();
		}
	}
}

void Game::GoOffsetMap(int offset)
{
	if (m_pendingSave && !TankDialog::Confirm(L"尚未保存，确定要放弃已编辑的地图吗？"))
	{
		return;
	}
	m_mapId = clamp(m_mapId + offset, 1, INFINITE);
	m_map = m_mapStore.LoadMap(m_mapId);
	m_pendingSave = false;
}

void Game::SaveMap()
{
	m_mapStore.SaveMap(m_mapId, m_map);
	m_pendingSave = false;
}

void Game::DrawLeft()
{
	Point2F topLeft{ -GridUnitSize - 2.0f, 2.0f };

	auto HandleEnvs = [&](bool isSmall) {
		for (size_t i = 0; i < m_envSequence.size(); ++i)
		{
			auto env = m_envSequence[i];
			auto size = isSmall ? GridUnitHalfSize : GridUnitSize;

			auto rect = MathUtil::MakeRectangleSquareByWH(topLeft, size);
			if (rect.Contains(Vector2(m_mousePos().X, m_mousePos().Y)))
			{
				if (m_mouseState().leftButton)
				{
					m_selectedEnv = env;
					m_small = isSmall;
				}
			}
			isSmall ? DrawEnv(env, topLeft) : DrawEnv4(env, topLeft);
			if ((isSmall == m_small) && m_selectedEnv == env || rect.Contains(Vector2(m_mousePos().X, m_mousePos().Y)))
			{
				// draw selected
				auto target = m_deviceResources->GetD2DDeviceContext();
				auto rect = MathUtil::MakeRectSquareByWH(topLeft, size);
				target.DrawRectangle(rect, m_red());
			}

			topLeft.Y += size + 2.0f;
		}
	};
	HandleEnvs(false);
	HandleEnvs(true);
}

void Game::DrawBody()
{
	auto mouseState = m_deviceResources->GetMouseState();
	auto gridPos = MathUtil::GetMouseGridPos(m_mousePos());

	auto fullRect = Rectangle{ 0, 0, (int)GridSize, (int)GridSize };
	auto cursorInBody = fullRect.Contains(Vector2(m_mousePos().X, m_mousePos().Y));

	if (mouseState.leftButton && cursorInBody)
	{
		// cursor click
		MapHelper::SetPos4ToEnv(m_map, gridPos.X, gridPos.Y, m_small, m_selectedEnv);
		m_pendingSave = true;
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
	auto MoveOffset = [&topLeft]() { topLeft.Y += GridUnitSize + 2.0f; };

	auto levelStr = fmt::format(L"L{0}", m_mapId);
	target.DrawText(levelStr.c_str(), levelStr.size(), m_textFormat(), MathUtil::MakeRectSquareByWH(topLeft, 800), m_black());
	MoveOffset();

	DrawSprite(SpriteType::Left, topLeft);
	AddClickHandler(topLeft, GridUnitSize, bind(&Game::GoOffsetMap, this, -1));
	MoveOffset();

	DrawSprite(SpriteType::Right, topLeft);
	AddClickHandler(topLeft, GridUnitSize, bind(&Game::GoOffsetMap, this, 1));
	MoveOffset();

	if (m_pendingSave)
	{
		DrawSprite(SpriteType::Player2, topLeft);
		AddClickHandler(topLeft, GridUnitSize, bind(&Game::SaveMap, this));
		MoveOffset();
	}
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

void Game::OnDeviceLost()
{
	// TODO: Add Direct3D resource cleanup here.
	m_bmp.Reset();
}