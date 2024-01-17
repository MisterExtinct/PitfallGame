#include "precomp.h"

using namespace Tmpl8;
Rope::Rope(float2 anchor, float length, float damping, float angle)
{
	rope_anchor = anchor;
	rope_length = length;
	rope_dampingFactor = damping;
	rope_Angle = angle;
	rope_gravity = 9.81f;
	rope_Velocity = 0.0f;
	rope_end = float2(0, 0);
}

void Rope::UpdateRope(float deltaTime, Surface* screen,float2 screenOffset)
{
	rope_end.x = rope_anchor.x + rope_length * sin(rope_Angle);
	rope_end.y = rope_anchor.y + rope_length * cos(rope_Angle);

	const float angularAcceleration = -rope_gravity / rope_length * sin(rope_Angle);
	rope_Velocity += angularAcceleration * deltaTime;
	rope_Velocity *= rope_dampingFactor;
	rope_Angle += rope_Velocity * deltaTime;

	screen->Line(rope_anchor.x + screenOffset.x, rope_anchor.y + screenOffset.y, rope_end.x + screenOffset.x, rope_end.y + screenOffset.y, 0x00FF00);
}

