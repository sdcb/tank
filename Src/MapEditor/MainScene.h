//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "PreDefined.h"
#include "GameBase.h"
#include "MapStore.h"
#define PROJECT_NAME L"Map Editor"

namespace Tank
{
	enum class SpriteType : unsigned char;
}

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game : public GameBase
{
public:
    Game();

    // IDeviceNotify
    virtual void OnDeviceLost() override;

private:

    void Update(DX::StepTimer const& timer) override;
	void Draw(KennyKerr::Direct2D::DeviceContext target) override;

    void CreateDeviceResources() override;
    void CreateWindowSizeResources() override;

	void DrawUnit(Tank::TankSpriteUnit id, KennyKerr::Point2F center);
	void DrawEnv(Tank::EnvType env, KennyKerr::Point2F topLeft);
	void DrawEnv4(Tank::EnvType env, KennyKerr::Point2F topLeft);
	void DrawSprite(Tank::SpriteType sprite, KennyKerr::Point2F topLeft);
	void AddClickHandler(KennyKerr::Point2F topLeft, float size, std::function<void()> clickHandler);

	// map functions
	void GoNextMap();
	void GoPrevMap();
	void SaveMap();

	void DrawLeft();
	void DrawRight();
	void DrawBody();
	void DrawSpecialEnvironments();

    // Device resources.
	KennyKerr::Direct2D::Bitmap1            m_bmp;
	KennyKerr::Direct2D::SolidColorBrush    m_black, m_red;

	// text format
	KennyKerr::DirectWrite::TextFormat      m_textFormat;

	// selected environment type
	Tank::EnvType                           m_selectedEnv;
	bool                                    m_small;

	// scene array
	int                                     m_mapId;
	Tank::MapBody                           m_map;
	Tank::MapStore                          m_mapStore;
	bool                                    m_pendingSave;
	std::array<Tank::EnvType, 6>            m_envSequence;

	// input states
	DirectX::Keyboard::State                m_keyboardState;
	DirectX::Mouse::State                   m_mouseState;

	// sprite map
	Tank::TankSpriteMapArray                m_tankSpriteMap;

	// scale
	D2D1::Matrix3x2F                        m_world;
};