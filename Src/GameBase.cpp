#include "pch.h"
#include "GameBase.h"
#include "SpriteUnit.h"
#include "MathUtil.h"
#include <SpriteButton.h>

using namespace DirectX;
using D2D1::ColorF;
using D2D1::Matrix3x2F;
using namespace Tank;
using namespace std;
using namespace KennyKerr;
using namespace KennyKerr::Direct2D;

GameBase::GameBase(): 
	m_tankSpriteMap(CreateTankSpriteMap())
{
	m_deviceResources = std::make_unique<DX::DeviceResources>();
	m_deviceResources->RegisterDeviceNotify(this);
}

GameBase::~GameBase()
{
}

void GameBase::Initialize(HWND window, int width, int height)
{
	m_deviceResources->SetWindow(window, width, height);

	m_deviceResources->CreateDeviceResources();
	CreateDeviceResources();

	m_deviceResources->CreateWindowSizeDependentResources();
	CreateWindowSizeResources();

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:

	/*m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);*/
}

#pragma region Frame Update
void GameBase::Tick()
{
	m_timer.Tick([&]()
	{
		Update(m_timer);
	});

	Render();
}

// Updates the world.
void GameBase::Update(DX::StepTimer const& timer)
{
	m_deviceResources->Update();

	for (auto & button : m_buttons)
	{
		button->Update(&m_timer);
	}
}
#pragma endregion

#pragma region Frame Render

// Draws the scene.
void GameBase::Render()
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
	auto & state = m_deviceResources->GetKeyboardState();

	// TODO: Add your rendering code here.
	target.BeginDraw();
	Draw(target);
	for (auto & button : m_buttons)
	{
		button->Draw([&](SpriteUnit id, Point2F topLeft) {
			DrawUnit(id, topLeft);
		});
	}
	
	if (state.LeftControl && state.LeftAlt && state.F || _DEBUG)
	{
		auto fps = fmt::format(L"FPS: {0}", m_timer.GetFramesPerSecond());
		target.SetTransform(Matrix3x2F::Identity());
		target.DrawText(fps.c_str(), fps.size(), m_deviceResources->GetTextFormat(), KennyKerr::RectF{ 0, 0, 500.0f, 500.0f },
			m_deviceResources->GetOrCreateColor(ColorF::Yellow));
	}
	
	target.EndDraw();

	m_deviceResources->PIXEndEvent();

	// Show the new frame.
	m_deviceResources->Present();
}

void GameBase::DrawUnit(SpriteUnit id, KennyKerr::Point2F topLeft)
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

// Helper method to clear the back buffers.
void GameBase::Clear()
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
void GameBase::Draw(KennyKerr::Direct2D::DeviceContext target)
{
	target.SetTransform(m_deviceResources->GetWorldTranslation());
	
}
#pragma endregion

void GameBase::CreateDeviceResources()
{
	auto wic = m_deviceResources->GetWicFactory();
	auto target = m_deviceResources->GetD2DDeviceContext();

	auto stream = wic.CreateStream();
	HR(stream.InitializeFromFilename(L"All.png", GENERIC_READ));
	auto decoder = wic.CreateFormatConverter();
	decoder.Initialize(wic.CreateDecoderFromStream(stream).GetFrame());
	m_bmp = target.CreateBitmapFromWicBitmap1(decoder);
}

void GameBase::CreateWindowSizeResources()
{
	
}

void GameBase::OnDeviceLost()
{
	m_bmp.Reset();
}

void GameBase::OnDeviceRestored()
{
	CreateDeviceResources();

	CreateWindowSizeResources();
}

#pragma region Message Handlers
// Message handlers
void GameBase::OnActivated()
{
	// TODO: Game is becoming active window.
}

void GameBase::OnDeactivated()
{
	// TODO: Game is becoming background window.
}

void GameBase::OnSuspending()
{
	// TODO: Game is being power-suspended (or minimized).
}

void GameBase::OnResuming()
{
	m_timer.ResetElapsedTime();

	// TODO: Game is being power-resumed (or returning from minimize).
}

void GameBase::OnWindowSizeChanged(int width, int height)
{
	if (!m_deviceResources->WindowSizeChanged(width, height))
		return;

	CreateWindowSizeResources();

	// TODO: Game window is being resized.
}

void GameBase::OnClick(int x, int y)
{
	auto pos = Math::GetMousePos(x, y, m_deviceResources->GetWorldTranslation());
	for (auto & button : m_buttons)
	{
		button->OnClick(pos);
	}
}

void GameBase::OnMouseMove(int x, int y)
{
	auto pos = Math::GetMousePos(x, y, m_deviceResources->GetWorldTranslation());
	for (auto & button : m_buttons)
	{
		button->OnMouseMove(pos);
	}
}

void GameBase::OnKeyUp(DirectX::Keyboard::Keys key)
{
	for (auto & button : m_buttons)
	{
		button->OnKeyUp(key);
	}
}
#pragma endregion

// Properties
void GameBase::GetDefaultSize(int& width, int& height) const
{
	// TODO: Change to desired default window size (note minimum size is 320x200).
	width = 832;
	height = 624;
}

Tank::SpriteButton * GameBase::NewSpriteButton(KennyKerr::Point2F topLeft, std::vector<Tank::SpriteUnit> unites)
{
	m_buttons.emplace_back(std::make_unique<Tank::SpriteButton>(topLeft, unites, m_deviceResources.get()));
	return (Tank::SpriteButton*)m_buttons[m_buttons.size() - 1].get();
}