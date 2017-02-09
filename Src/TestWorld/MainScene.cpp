#include "pch.h"
#include "MainScene.h"
#include <MathUtil.h>
#include <SpriteUnit.h>

using namespace Tank;
using namespace KennyKerr;

Game::Game(): 
	m_mapId(1), 
	m_mapStore()
{
	m_map = m_mapStore.LoadMap(m_mapId);
	auto btn = NewSpriteButton(Point2F{ 0.0f, 0.0f }, { SpriteUnit::Exp_1, SpriteUnit::Exp_2, SpriteUnit::Exp_3, SpriteUnit::Exp_0 });
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