#pragma once

#include <PreDefined.h>
#include <SpriteBase.h>
#include <vector>

namespace Tank
{
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

	class Player final : public SpriteBase
	{
	public:
		Player(DX::DeviceResources * dxRes, KennyKerr::Point2F topLeft, PlayerId playerId);
		virtual void Update(DX::StepTimer const * timer) override;
		virtual void Draw(DrawCall drawCall) override;

	private:
		void DrawLive(const DrawCall& drawCall);

		KennyKerr::Point2F    m_topLeft;
		DX::StepTimer const * m_timer;
		const PlayerId        m_playerId;

		PlayerLife m_life         = PlayerLife::Borning;
		int        m_dieingFrame  = 0;
		int        m_moveingFrame = 0;
		double     m_dieingTime   = 0;
		double     m_bornTime     = 0;
	};
}
