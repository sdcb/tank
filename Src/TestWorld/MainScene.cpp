#include "pch.h"
#include "MainScene.h"
#include <MathUtil.h>
#include <SpriteUnit.h>
#include "MapSprite.h"

using namespace std;
using namespace Tank;
using namespace KennyKerr;

Game::Game(): 
	m_mapId(1), 
	m_mapStore()
{
	m_map = m_mapStore.LoadMap(m_mapId);
	m_sprites.push_back(unique_ptr<Player>(new Player( m_deviceResources.get(), Point2F{8.0f, 8.0f}, PlayerId::_1 )));
}

void Game::CreateWindowSizeResources()
{
	auto target = m_deviceResources->GetD2DDeviceContext();
	m_world = Math::CreateWorldTransform(target.GetSize());
	m_deviceResources->SetWorldTranslation(m_world);
}

void Game::Draw(KennyKerr::Direct2D::DeviceContext target)
{
	target.SetTransform(m_world);
}