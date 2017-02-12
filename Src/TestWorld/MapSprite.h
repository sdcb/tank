#pragma once

#include <PreDefined.h>
#include <SpriteBase.h>
#include <vector>
#include <StepTimer.h>

namespace Tank
{
	enum class Direction;

	enum class PlayerLife
	{
		Borning, 
		Live, 
		Dieing, 
		Nothing, 
	};

	enum class PlayerId
	{
		_1, 
		_2, 
	};

	enum class PlayerLevel
	{
		_1, 
		_2, 
		_3, 
	};

	class TankLife : public SpriteBase
	{
	public:
		typedef std::array<SpriteUnit, 2> MovingFrames;

		TankLife(DX::DeviceResources * dxRes, KennyKerr::Point2F topLeft);
		void Update(DX::StepTimer const * timer) override;
		void Draw(DrawCall drawCall) override;

		Direction GetDirection() const { return m_direction; }
		void SetDirection(Direction direction) { m_direction = direction; }

		KennyKerr::Point2F GetTopLeft() const { return m_topLeft; }
		void SetTopLeft(KennyKerr::Point2F v) { m_topLeft = v; }

	protected:
		virtual void UpdatePerSecond() = 0;
		virtual void UpdateLive() = 0;
		virtual void DrawLive(const DrawCall & drawCall) = 0;
		virtual float GetSpeed() = 0;

		KennyKerr::Point2F    m_topLeft;
		DX::StepTimer const * m_timer;
		DX::StepTimer m_secondTimer;

		PlayerLife m_life = PlayerLife::Borning;
		Direction  m_direction;
		double     m_dieingTime = 0;
		double     m_bornTime = 0;
		int        m_dieingFrame = 0;
	};

	class Enemy final : public TankLife
	{
		// Í¨¹ý TankLife ¼Ì³Ð
		virtual void UpdatePerSecond() override;
		virtual void UpdateLive() override;
		virtual void DrawLive(const DrawCall & drawCall) override;
		virtual float GetSpeed() override;
	};

	class Player final : public TankLife
	{
	public:
		Player(DX::DeviceResources * dxRes, KennyKerr::Point2F topLeft, PlayerId playerId);

		PlayerId GetPlayerId() const { return m_playerId; }
		void SetPlayerId(PlayerId playerId) { m_playerId = playerId; }

		PlayerLevel GetPlayerLevel() const { return m_playerLevel; }
		void SetPlayerLevel(PlayerLevel level) { m_playerLevel = level; }
		

	private:
		void DrawLive(const DrawCall& drawCall) override;
		virtual void UpdateLive() override;
		virtual void UpdatePerSecond() override;
		virtual float GetSpeed() override;

		PlayerId    m_playerId;
		int         m_movingFrame = 0;
		PlayerLevel m_playerLevel = PlayerLevel::_1;

		MovingFrames m_liveMovingFrames;

		static MovingFrames GetSprites(PlayerId id, PlayerLevel level, Direction direction);
	};
}
