#include "pch.h"
#include "MainScene.h"
#include <MathUtil.h>
#include <SpriteUnit.h>
#include "MapSprite.h"
#include <random>

using namespace std;
using namespace Tank;
using namespace KennyKerr;
using D2D1::ColorF;

Game::Game(): 
	m_mapId(1), 
	m_mapStore()
{
	m_map = m_mapStore.LoadMap(m_mapId);
	mt19937 rd{ random_device()() };
	uniform_real<float> u{0.0f, 208.0f};
	for (auto i = 0; i < 100; ++i)
	{
		m_sprites.push_back(unique_ptr<Enemy>(new Enemy(
			m_deviceResources.get(), 
			Point2F{ u(rd), u(rd) }, 
			EnemyId::_1 + rand() % 4, 
			EnemyLevel::_1 + rand() % 4)));
	}
	m_sprites.push_back(unique_ptr<Player>(new Player( 
		m_deviceResources.get(), Point2F{ u(rd), u(rd) }, PlayerId::_1 )));
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
	target.FillRectangle(RectF{ 0.0f, 0.0f, GridSize, GridSize }, m_deviceResources->GetOrCreateColor(ColorF::Black));
}