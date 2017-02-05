//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "PreDefined.h"
#define PROJECT_NAME L"Map Editor"

enum class EnvType : unsigned char;

class BodyEnvType;


// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game : public DX::IDeviceNotify
{
public:

    Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    virtual void OnDeviceLost() override;
    virtual void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

private:

    void Update(DX::StepTimer const& timer);
    void Render();
	void Draw(KennyKerr::Direct2D::DeviceContext target);

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

	void DrawTankSprite(TankSpriteType id, KennyKerr::Point2F center);
	void DrawEnv(EnvType env, KennyKerr::Point2F topLeft);
	void DrawEnv4(EnvType env, KennyKerr::Point2F topLeft);
	void DrawEnvSelection();
	KennyKerr::Point2F GetMousePos();
	KennyKerr::Point2U GetMouseGridPos();
	void DrawMousePosText();
	void DrawBodyEnv();
	void DrawSpecialEnvironments();

    // Device resources.
    std::unique_ptr<DX::DeviceResources>    m_deviceResources;
	KennyKerr::Direct2D::Bitmap1            m_bmp;
	KennyKerr::Direct2D::SolidColorBrush    m_black, m_red;

	// text format
	KennyKerr::DirectWrite::TextFormat      m_textFormat;

	// selected environment type
	EnvType                                 m_selectedEnv;
	bool                                    m_small = false;

	// scene array
	EnvironmentBody                         m_envs;
	std::array<EnvType, 6>                  m_envSequence;

    // Rendering loop timer.
    DX::StepTimer                           m_timer;

	// sprite map
	TankSpriteMapArray                      m_tankSpriteMap;

	// scale
	D2D1::Matrix3x2F                        m_world;
};