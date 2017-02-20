#include "pch.h"
#include "MapSprite.h"
#include <StepTimer.h>
#include <MathUtil.h>

using namespace Tank;
using namespace KennyKerr;

Tank::TankLife::TankLife(DX::DeviceResources * dxRes, KennyKerr::Point2F topLeft) :
	SpriteBase{ dxRes },
	m_topLeft{ topLeft }, 
	m_secondTimer{},
	m_direction{ Direction::Up }
{
	m_secondTimer.SetFixedTimeStep(true);
	m_secondTimer.SetTargetElapsedSeconds(1.0);
}

void Tank::TankLife::Update(DX::StepTimer const * timer)
{
	const double BornTime = 1.0f;
	m_timer = timer;
	m_secondTimer.Tick([&]() {
		UpdatePerSecond();
	});

	auto time = timer->GetTotalSeconds();
	if (m_life == PlayerLife::Borning && m_bornTime == 0.0)
	{
		m_bornTime = time;
	}

	auto elapsedSinceBorn = time - m_bornTime;
	if (m_life == PlayerLife::Borning && elapsedSinceBorn > BornTime)
	{
		m_life = PlayerLife::Live;
	}
	else if (m_life == PlayerLife::Live)
	{
		UpdateLive();
	}
	else if (m_life == PlayerLife::Dieing)
	{
		const int TotalDieingFrame = 4;
		auto elapsedSinceDieing = time - m_dieingTime;
		m_dieingFrame = int(elapsedSinceDieing * 1000) / 100;
		if (m_dieingFrame >= TotalDieingFrame)
		{
			m_life = PlayerLife::Nothing;
		}
	}
}

void Tank::TankLife::Draw(DrawCall drawCall)
{
	if (m_life == PlayerLife::Borning)
	{
		SpriteUnit frames[] =
		{
			SpriteUnit::Star_0,
			SpriteUnit::Star_1,
			SpriteUnit::Star_2,
			SpriteUnit::Star_3,
		};
		drawCall(frames[m_timer->GetSpriteId(100, _countof(frames))], m_topLeft);
	}
	else if (m_life == PlayerLife::Live)
	{
		DrawLive(drawCall);
	}
	else if (m_life == PlayerLife::Dieing)
	{
		SpriteUnit frames[] =
		{
			SpriteUnit::Exp_1,
			SpriteUnit::Exp_2,
			SpriteUnit::Exp_3,
			SpriteUnit::Exp_0,
		};
		auto frame = frames[m_dieingFrame];

		Point2F topLeft = m_topLeft;
		if (frame == SpriteUnit::Exp_0)
		{
			topLeft.X -= 4.0f;
			topLeft.Y -= 4.0f;
		}
		drawCall(frames[m_dieingFrame], topLeft);
	}
}


Tank::Player::Player(DX::DeviceResources * dxRes, Point2F topLeft, PlayerId playerId) :
	TankLife{ dxRes, topLeft },
	m_playerId{ playerId }
{
}

void Tank::Player::DrawLive(const DrawCall & drawCall)
{
	drawCall(m_liveMovingFrames[m_timer->GetSpriteId(100, 2)], m_topLeft);
}

void Tank::Player::UpdateLive()
{
	m_topLeft = Math::ByDirection(m_topLeft, m_direction, GetSpeed());
	m_liveMovingFrames = GetSprites(m_playerId, m_playerLevel, m_direction);
}

TankLife::MovingFrames Tank::Player::GetSprites(PlayerId id, PlayerLevel level, Direction direction)
{
	int offset = 0;
	offset += (int)id * 32;
	offset += (int)level * 8;
	offset += (int)direction * 2;
	return 
	{
		(SpriteUnit)offset,
		(SpriteUnit)offset + 1,
	};
}

float Tank::Player::GetSpeed()
{
	return 0.01f;
}

void Tank::Player::UpdatePerSecond()
{
	auto r = rand() % 5;
	if (r < 4)
	{
		m_direction = (Direction)r;
	}
}

Tank::Enemy::Enemy(DX::DeviceResources * dxRes, KennyKerr::Point2F topLeft, EnemyId playerId): 
	TankLife(dxRes, topLeft)
{
}

void Tank::Enemy::UpdatePerSecond()
{
	auto r = rand() % 5;
	if (r < 4)
	{
		m_direction = (Direction)r;
	}
}

void Tank::Enemy::UpdateLive()
{
	m_topLeft = Math::ByDirection(m_topLeft, m_direction, GetSpeed());
	m_liveMovingFrames = GetSprites(m_enemyId, m_enemyLevel, m_direction);
}

void Tank::Enemy::DrawLive(const DrawCall & drawCall)
{
}

float Tank::Enemy::GetSpeed()
{
	return 0.0f;
}
