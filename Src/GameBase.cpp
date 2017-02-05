#include "pch.h"
#include "GameBase.h"

using namespace DirectX;

GameBase::GameBase()
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

	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
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

	// TODO: Add your rendering code here.
	target.BeginDraw();
	Draw(target);
	target.EndDraw();

	m_deviceResources->PIXEndEvent();

	// Show the new frame.
	m_deviceResources->Present();
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
}
#pragma endregion

void GameBase::CreateDeviceResources()
{
}

void GameBase::CreateWindowSizeResources()
{
}

void GameBase::OnDeviceLost()
{
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

// Properties
void GameBase::GetDefaultSize(int& width, int& height) const
{
	// TODO: Change to desired default window size (note minimum size is 320x200).
	width = 800;
	height = 600;
}
#pragma endregion