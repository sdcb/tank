#include "pch.h"
#include "MapSprite.h"
#include <StepTimer.h>

using namespace KennyKerr;

Tank::Player::Player(DX::DeviceResources * dxRes, Point2F topLeft, PlayerId playerId): 
	SpriteBase{ dxRes }, 
	m_topLeft(topLeft), 
	m_playerId{ playerId }, 
	m_life(PlayerLife::Borning), 
	m_bornTime{ 0.0 }
{
}

void Tank::Player::Update(DX::StepTimer const * timer)
{
	const double BornTime = 1.0f;
	m_timer = timer;

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
	else if (m_life == PlayerLife::Live && elapsedSinceBorn > BornTime * 2)
	{
		m_life = PlayerLife::Dieing;
		m_dieingTime = time;
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

void Tank::Player::Draw(DrawCall drawCall)
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

void Tank::Player::DrawLive(const DrawCall & drawCall)
{
	SpriteUnit frames[] =
	{
		SpriteUnit::P1_1_L0,
		SpriteUnit::P1_1_L1,
		SpriteUnit::P1_1_U0,
		SpriteUnit::P1_1_U1,
		SpriteUnit::P1_1_R0,
		SpriteUnit::P1_1_R1,
		SpriteUnit::P1_1_D0,
		SpriteUnit::P1_1_D1,
	};
	drawCall(frames[m_timer->GetSpriteId(100, _countof(frames))], m_topLeft);
}
