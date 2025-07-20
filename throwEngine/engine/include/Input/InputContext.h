#pragma once

namespace Input
{
	
	struct InputContext
	{
	public:
		float m_delta;
		float m_rotationSpeed;
		float m_currentTime;
		float m_lastTime;
		float m_angle;
		float m_interval;
		float m_radius;

		bool m_isJumping;
		float m_jumpDuration;
		float m_jumpHeight;

		float m_firstVelocityY;
		float m_gravity;
		float m_startY;
	};

}

