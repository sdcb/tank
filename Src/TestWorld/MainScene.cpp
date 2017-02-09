#include "pch.h"
#include "MainScene.h"

Game::Game(): 
	m_mapId(1), 
	m_mapStore()
{
	m_map = m_mapStore.LoadMap(m_mapId);
}