//
// Game.cpp
//

#include "pch.h"
#include "MainScene.h"
#include "TankSprite.h"
#include "MapHelper.h"

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

bool IsMouseInSprite(Point2F mousePos, Point2F spriteTopLeft, float spriteSize = GridUnitSize);
Point2F GetMousePos(const Mouse::State& mouseState, Matrix3x2F world);
Point2U GetMouseGridPos(Point2F mousePos);
RectF MakeRectSquareByWH(Point2F topLeft, float width);
Rectangle MakeRectangleSquareByWH(Point2F topLeft, float width);

Game::Game() :
	m_tankSpriteMap(CreateTankSpriteMap()),
	m_selectedEnv(EnvType::Empty),
	m_envSequence(
{
	EnvType::Empty, EnvType::Grass, EnvType::Ice, EnvType::Iron, EnvType::Sea, EnvType::Wall
})
{
	m_map = m_mapStore.LoadMap(1);
	m_textFormat = m_deviceResources->GetDWriteFactory().CreateTextFormat(L"Consolas", 12.0f);
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
	float elapsedTime = float(timer.GetElapsedSeconds());
	m_keyboardState = m_deviceResources->GetKeyboardState();
	m_mouseState = m_deviceResources->GetMouseState();

	// TODO: Add your game logic here.
	for (size_t i = 0; i < m_envSequence.size(); ++i)
	{
		if (m_keyboardState.IsKeyDown(Keyboard::Keys(Keyboard::D1 + i)))
		{
			m_selectedEnv = m_envSequence[i];
		}
	}
	if (m_keyboardState.IsKeyDown(Keyboard::F1))
	{
		m_small = false;
	}
	if (m_keyboardState.IsKeyDown(Keyboard::F2))
	{
		m_small = true;
	}
}

void Game::Draw(KennyKerr::Direct2D::DeviceContext target)
{
	target.SetTransform(m_world);
	target.FillRectangle(RectF{ 0, 0, (float)GridSize, (float)GridSize }, m_black);
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

	m_black = target.CreateSolidColorBrush(ColorF(ColorF::Black));
	m_red = target.CreateSolidColorBrush(ColorF(ColorF::Red));
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
}

void Game::DrawUnit(TankSpriteUnit id, KennyKerr::Point2F topLeft)
{
#ifdef _DEBUG
	if ((size_t)id > m_tankSpriteMap.size())
	{
		// asset not found.
		DebugBreak();
	}
#endif
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
	default:
		throw exception{ "Unkonwn sprite type." };
	}
}

void Game::DrawLeft()
{
	Point2F topLeft{ -GridUnitSize - 2.0f, 2.0f };

	auto pos = GetMousePos(m_mouseState, m_world);

	auto HandleEnvs = [&](bool isSmall) {
		for (size_t i = 0; i < m_envSequence.size(); ++i)
		{
			auto env = m_envSequence[i];
			auto size = isSmall ? GridUnitHalfSize : GridUnitSize;

			auto rect = MakeRectangleSquareByWH(topLeft, size);
			if (rect.Contains(Vector2(pos.X, pos.Y)))
			{
				if (m_mouseState.leftButton)
				{
					m_selectedEnv = env;
					m_small = isSmall;
				}
			}
 			isSmall ? DrawEnv(env, topLeft) : DrawEnv4(env, topLeft);
			if ((isSmall == m_small) && m_selectedEnv == env || rect.Contains(Vector2(pos.X, pos.Y)))
			{
				// draw selected
				auto target = m_deviceResources->GetD2DDeviceContext();
				auto rect = MakeRectSquareByWH(topLeft, size);
				target.DrawRectangle(rect, m_red);
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
	auto mousePos = GetMousePos(m_mouseState, m_world);
	auto gridPos = GetMouseGridPos(mousePos);
	
	auto fullRect = Rectangle{ 0, 0, (int)GridSize, (int)GridSize };
	auto cursorInBody = fullRect.Contains(Vector2(mousePos.X, mousePos.Y));

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
		auto currentRect = MakeRectSquareByWH(topLeft, times * GridUnitHalfSize);
		target.DrawRectangle(currentRect, m_red);
	}
}

void Game::DrawRight()
{
	auto target = m_deviceResources->GetD2DDeviceContext();

	if (m_pendingSave)
	{		
		auto mousePos = GetMousePos(m_mouseState, m_world);
		auto topLeft = Point2F{ GridSize + 2.0f, 2.0f };

		DrawSprite(SpriteType::Player1, topLeft);
		if (IsMouseInSprite(mousePos, topLeft))
		{
			RectF rect{ topLeft.X, topLeft.Y, topLeft.X + GridUnitSize, topLeft.Y + GridUnitSize };
			target.DrawRectangle(rect, m_red);
			
			auto mouseState = m_deviceResources->GetMouseState();
			if (mouseState.leftButton)
			{
				m_mapStore.SaveMap(1, m_map);
				m_pendingSave = false;
			}
		}
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

bool IsMouseInSprite(Point2F mousePos, Point2F spriteTopLeft, float spriteSize)
{
	auto rect = MakeRectangleSquareByWH(spriteTopLeft, spriteSize);
	return rect.Contains(Vector2(mousePos.X, mousePos.Y));
}

Point2F GetMousePos(const Mouse::State& mouseState, Matrix3x2F world)
{
	auto worldCopy = Matrix3x2F(world);
	ASSERT(worldCopy.Invert());
	return worldCopy.TransformPoint({ (float)mouseState.x, (float)mouseState.y });
}

Point2U GetMouseGridPos(Point2F mousePos)
{
	Point2U gridPos
	{
		(unsigned int)clamp(mousePos.X / GridUnitHalfSize, 0, GridCountDouble - 1),
		(unsigned int)clamp(mousePos.Y / GridUnitHalfSize, 0, GridCountDouble - 1)
	};
	return gridPos;
}

RectF MakeRectSquareByWH(Point2F topLeft, float width)
{
	return RectF
	{
		topLeft.X, 
		topLeft.Y, 
		topLeft.X + width, 
		topLeft.Y + width
	};
}

Rectangle MakeRectangleSquareByWH(Point2F topLeft, float width)
{
	return Rectangle
	{
		long(topLeft.X),
		long(topLeft.Y),
		long(width),
		long(width)
	};
}
