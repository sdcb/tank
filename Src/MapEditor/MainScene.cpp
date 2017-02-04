//
// Game.cpp
//

#include "pch.h"
#include "MainScene.h"
#include "TankSprite.h"
#include "Environment.h"

extern void ExitGame();

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace KennyKerr;
using namespace KennyKerr::Direct2D;

using D2D1::ColorF;
using D2D1::Matrix3x2F;
using Microsoft::WRL::ComPtr;

Game::Game() :
	m_tankSpriteMap(CreateTankSpriteMap()),
	m_selectedEnv(EnvType::Empty)
{
	m_deviceResources = std::make_unique<DX::DeviceResources>();
	m_deviceResources->RegisterDeviceNotify(this);
	m_envs = CreateBasicEnv();
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
	m_deviceResources->SetWindow(window, width, height);

	m_deviceResources->CreateDeviceResources();
	CreateDeviceDependentResources();

	m_deviceResources->CreateWindowSizeDependentResources();
	CreateWindowSizeDependentResources();

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:

	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);

	m_textFormat = m_deviceResources->GetDWriteFactory().CreateTextFormat(L"Consolas", 12.0f);
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
	m_timer.Tick([&]()
	{
		Update(m_timer);
	});

	Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
	float elapsedTime = float(timer.GetElapsedSeconds());

	// TODO: Add your game logic here.
	elapsedTime;
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}

	Clear();

	m_deviceResources->PIXBeginEvent(L"Render");
	auto context = m_deviceResources->GetD3DDeviceContext();
	auto target = m_deviceResources->GetD2DDeviceContext();

	// TODO: Add your rendering code here.
	target.BeginDraw();
	Draw(target);
	target.EndDraw();

	m_deviceResources->PIXEndEvent();

	// Show the new frame.
	m_deviceResources->Present();
}

void Game::Draw(KennyKerr::Direct2D::DeviceContext target)
{
	target.SetTransform(m_world);
	target.FillRectangle(RectF{ 0, 0, (float)GridSize, (float)GridSize }, m_black);
	DrawEnvSelection();
	DrawBodyEnv();
	target.SetTransform(Matrix3x2F::Identity());
	if (m_deviceResources->GetKeyboardState().Space)
	{
		DrawMousePosText();
	}
}

// Helper method to clear the back buffers.
void Game::Clear()
{
	m_deviceResources->PIXBeginEvent(L"Clear");

	// Clear the views.
	auto context = m_deviceResources->GetD3DDeviceContext();
	auto renderTarget = m_deviceResources->GetRenderTargetView();
	auto depthStencil = m_deviceResources->GetDepthStencilView();

	context->ClearRenderTargetView(renderTarget, Colors::Gray);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);

	// Set the viewport.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
	// TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
	// TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
	// TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
	m_timer.ResetElapsedTime();

	// TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int width, int height)
{
	if (!m_deviceResources->WindowSizeChanged(width, height))
		return;

	CreateWindowSizeDependentResources();

	// TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
	// TODO: Change to desired default window size (note minimum size is 320x200).
	width = 800;
	height = 600;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
	auto device = m_deviceResources->GetD3DDevice();
	auto target = m_deviceResources->GetD2DDeviceContext();
	auto wic = m_deviceResources->GetWicFactory();

	// TODO: Initialize device dependent objects here (independent of window size).
	auto stream = wic.CreateStream();
	HR(stream.InitializeFromFilename(L"All.png"));
	auto decoder = wic.CreateFormatConverter();
	decoder.Initialize(wic.CreateDecoderFromStream(stream).GetFrame());
	m_bmp = target.CreateBitmapFromWicBitmap1(decoder);

	m_black = target.CreateSolidColorBrush(ColorF(ColorF::Black));
	m_red = target.CreateSolidColorBrush(ColorF(ColorF::Red));
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
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

void Game::DrawTankSprite(std::string id, KennyKerr::Point2F topLeft)
{
#ifdef _DEBUG
	if (m_tankSpriteMap.find(id) == m_tankSpriteMap.end())
	{
		// asset not found.
		DebugBreak();
	}
#endif
	auto imagePos = m_tankSpriteMap[id];
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
		DrawTankSprite("Env-Grass", topLeft);
		return;
	case EnvType::Ice:
		DrawTankSprite("Env-Ice", topLeft);
		break;
	case EnvType::Iron:
		DrawTankSprite("Env-Iron", topLeft);
		break;
	case EnvType::Sea:
		DrawTankSprite(fmt::format("Env-Sea{0}", m_timer.GetFrameCount() / 10 % 2), topLeft);
		break;
	case EnvType::Wall:
		DrawTankSprite("Env-Wall", topLeft);
		break;
	default:
		throw exception("Env is not supported.");
	}
}

void Game::DrawEnv4(EnvType env, KennyKerr::Point2F topLeft)
{
	if (env == EnvType::Eager)
	{
		DrawTankSprite("Eager", topLeft);
		return;
	}
	else if (env == EnvType::Born)
	{
		DrawTankSprite(fmt::format("Star-{0}", m_timer.GetFrameCount() / 10 % 4), topLeft);
	}
	else if (env == EnvType::Player1)
	{
		DrawTankSprite(fmt::format("P1-1-U{0}", m_timer.GetFrameCount() / 10 % 2), topLeft);
	}
	else if (env == EnvType::Player2)
	{
		DrawTankSprite(fmt::format("P2-1-U{0}", m_timer.GetFrameCount() / 10 % 2), topLeft);
	}
	else
	{
		DrawEnv(env, topLeft);
		DrawEnv(env, Point2F{ topLeft.X + GridUnitHalfSize, topLeft.Y });
		DrawEnv(env, Point2F{ topLeft.X, topLeft.Y + GridUnitHalfSize });
		DrawEnv(env, Point2F{ topLeft.X + GridUnitHalfSize, topLeft.Y + GridUnitHalfSize });
	}
}

void Game::DrawEnvSelection()
{
	vector<EnvType> envs =
	{
		EnvType::Empty,
		EnvType::Grass,
		EnvType::Ice,
		EnvType::Iron,
		EnvType::Sea,
		EnvType::Wall,
	};

	Point2F topLeft{ -GridUnitSize - 2.0f, 2.0f };

	auto pos = GetMousePos();

	for (size_t i = 0; i < envs.size(); ++i)
	{
		auto env = envs[i];
		DirectX::SimpleMath::Rectangle rect
		{
			long(topLeft.X),
			long(topLeft.Y),
			long(GridUnitSize),
			long(GridUnitSize)
		};
		if (rect.Contains(Vector2(pos.X, pos.Y)))
		{
			auto mouseState = m_deviceResources->GetMouseState();
			if (mouseState.leftButton)
			{
				m_selectedEnv = env;
				m_small = false;
			}
		}
		DrawEnv4(env, topLeft);
		if (!m_small && m_selectedEnv == env || rect.Contains(Vector2(pos.X, pos.Y)))
		{
			// draw selected
			auto target = m_deviceResources->GetD2DDeviceContext();
			auto rect = RectF
			{
				topLeft.X, topLeft.Y,
				topLeft.X + GridUnitSize,
				topLeft.Y + GridUnitSize
			};
			target.DrawRectangle(rect, m_red);
		}

		topLeft.Y += GridUnitSize + 2.0f;
	}

	for (size_t i = 0; i < envs.size(); ++i)
	{
		auto env = envs[i];
		DirectX::SimpleMath::Rectangle rect
		{
			long(topLeft.X),
			long(topLeft.Y),
			long(GridUnitHalfSize),
			long(GridUnitHalfSize)
		};
		if (rect.Contains(Vector2(pos.X, pos.Y)))
		{
			auto mouseState = m_deviceResources->GetMouseState();
			if (mouseState.leftButton)
			{
				m_selectedEnv = env;
				m_small = true;
			}
		}
		DrawEnv(env, topLeft);
		if (m_small && m_selectedEnv == env || rect.Contains(Vector2(pos.X, pos.Y)))
		{
			auto target = m_deviceResources->GetD2DDeviceContext();
			auto rect = RectF
			{
				topLeft.X, topLeft.Y,
				topLeft.X + GridUnitHalfSize,
				topLeft.Y + GridUnitHalfSize
			};
			target.DrawRectangle(rect, m_red);
		}

		topLeft.Y += GridUnitHalfSize + 2.0f;
	}
}

KennyKerr::Point2F Game::GetMousePos()
{
	auto worldCopy = Matrix3x2F(m_world);
	ASSERT(worldCopy.Invert());
	auto mouseState = m_deviceResources->GetMouseState();
	return worldCopy.TransformPoint({ (float)mouseState.x, (float)mouseState.y });
}

KennyKerr::Point2U Game::GetMouseGridPos()
{
	auto mousePos = GetMousePos();
	Point2U gridPos
	{
		(unsigned int)clamp(mousePos.X / GridUnitHalfSize, 0, GridCountDouble - 1),
		(unsigned int)clamp(mousePos.Y / GridUnitHalfSize, 0, GridCountDouble - 1)
	};
	return gridPos;
}

void Game::DrawMousePosText()
{
	auto target = m_deviceResources->GetD2DDeviceContext();

	auto size = target.GetSize();
	auto rect = RectF{ 0, 0, size.Width, size.Height };

	auto pos = GetMousePos();
	auto gridPos = GetMouseGridPos();
	auto str = fmt::format(L"pos: ({0}, {1})\r\n", pos.X, pos.Y);
	str += fmt::format(L"grid: ({0}, {1})", gridPos.X, gridPos.Y);

	target.DrawText(str.c_str(), str.size(), m_textFormat, rect, m_red);
}

void Game::DrawBodyEnv()
{
	auto mouseState = m_deviceResources->GetMouseState();
	auto gridPos = GetMouseGridPos();
	auto mousePos = GetMousePos();
	auto fullRect = Rectangle{ 0, 0, (int)GridSize, (int)GridSize };
	auto cursorInBody = fullRect.Contains(Vector2(mousePos.X, mousePos.Y));

	if (mouseState.leftButton && cursorInBody)
	{
		// cursor click
		SetPos4ToEnv(m_envs, gridPos.X, gridPos.Y, m_small, m_selectedEnv);
	}

	auto target = m_deviceResources->GetD2DDeviceContext();
	auto localEnv = m_envs;
	DeleteSpecialEnvironments(m_envs);

	// cursor environment
	SetPos4ToEnv(localEnv, gridPos.X, gridPos.Y, m_small, m_selectedEnv);

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
		auto currentRect = RectF
		{
			(float)GridUnitHalfSize * gridPos.X,
			(float)GridUnitHalfSize * gridPos.Y,
			(float)GridUnitHalfSize * (gridPos.X + times),
			(float)GridUnitHalfSize * (gridPos.Y + times)
		};
		target.DrawRectangle(currentRect, m_red);
	}
}

void Game::DrawSpecialEnvironments()
{
	// born
	DrawEnv4(EnvType::Born, Point2F
	{
		0, 0
	});
	DrawEnv4(EnvType::Born, Point2F
	{
		GridUnitHalfSize * (GridCountDouble / 2 - 1), 0
	});
	DrawEnv4(EnvType::Born, Point2F
	{
		GridUnitHalfSize * (GridCountDouble - 2), 0
	});
	// player born 1-2
	DrawEnv4(EnvType::Player1, Point2F
	{
		GridUnitHalfSize * (GridCountDouble / 2 - 5), GridUnitHalfSize * (GridCountDouble - 2)
	});
	DrawEnv4(EnvType::Player2, Point2F
	{
		GridUnitHalfSize * (GridCountDouble / 2 + 3), GridUnitHalfSize * (GridCountDouble - 2)
	});
	// center eager
	DrawEnv4(EnvType::Eager, Point2F
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

void Game::OnDeviceRestored()
{
	CreateDeviceDependentResources();

	CreateWindowSizeDependentResources();
}
#pragma endregion