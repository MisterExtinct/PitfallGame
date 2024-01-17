#pragma once
namespace  Tmpl8
{


	class Rope
	{
	public:
		Rope(float2 anchor, float length, float damping, float angle);
		~Rope();
		void UpdateRope(float deltaTime,Surface* screen,float2 screenOffset);

	private:
		float rope_length, rope_gravity, rope_dampingFactor, rope_Angle, rope_Velocity;
		float2 rope_anchor, rope_end;
	};
};