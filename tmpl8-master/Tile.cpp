#include "precomp.h"
//#include "Tile.h"
using namespace Tmpl8;

Tile::Tile() {
	p_pos.x = 0.0f;
	p_pos.y = 0.0f;
	p_width = 0;
	p_height = 0;
	p_tileID = -1;
	p_collision = colliderType::Air;
	isDeathHole = false;
	isRope = false;
	rope_anchor = float2(0, 0);
	rope_length = 0;
	rope_gravity = 0;
	rope_dampingFactor = 0;
	rope_Angle = 0;
	rope_Velocity = 0;
	rope_end = float2(0, 0);
	p_tickTime = 0;
}
Tile::~Tile() {

}

void Tile::DrawTile(Surface* screen, float2 screenOffset, Sprite& sprite)
{
	if (!isRope) { sprite.SetFrame(p_tileID); } //Final check to make sure it works properly

	try
	{
		sprite.Draw(screen, p_pos.x + screenOffset.x, p_pos.y + screenOffset.y);
	}
	catch (...) //in case something happens to the sprite sheet
	{
		screen->Bar(p_pos.x + screenOffset.x, p_pos.y + screenOffset.y, p_pos.x + p_width + screenOffset.x, p_pos.y + p_height + screenOffset.y, 0x00FF00);
	}

	if (isRope)
	{
		screen->Line(rope_anchor.x + screenOffset.x, rope_anchor.y + screenOffset.y, rope_end.x + screenOffset.x, rope_end.y + screenOffset.y, 0x000000);
	}
}


void Tile::SetID(int id)
{
	p_tileID = id;
}

int Tile::GetID() {
	return p_tileID;
}

float2 Tile::GetCornerData(Corner corner) {
	float2 temp;
	switch (corner)
	{
	case Tile::TL:
		temp = p_pos;
		break;
	case Tile::TR:
		temp.x = p_pos.x + p_width;
		temp.y = p_pos.y;
		break;
	case Tile::BL:
		temp.x = p_pos.x;
		temp.y = p_pos.y + p_height;
		break;
	case Tile::BR:
		temp.x = p_pos.x + p_width;
		temp.y = p_pos.y + p_height;
		break;
	default:
		throw EXCEPTION_DATATYPE_MISALIGNMENT;
		break;
	}
	float2 rtval = int2(temp.x, temp.y);
	return rtval;
}
float2 Tile::GetCenterPosition() {
	float2 temp;
	temp.x = p_pos.x + (p_width / 2);
	temp.y = p_pos.y + (p_height / 2);
	return temp;
}
void Tile::SetPosition(float2 vecpos) {
	p_pos.x = vecpos.x;
	p_pos.y = vecpos.y;
}
void Tile::SetPosition(int x, int y) {
	p_pos.x = x;
	p_pos.y = y;
}

void Tile::SetCollision(colliderType colType) {
	p_collision = colType;
}
void Tile::SetWidthHeight(int width, int height) {
	p_width = width;
	p_height = height;
}

void Tile::AddTickTime(float deltaTime) {
	p_tickTime += deltaTime;
	if (isDeathHole) {
		if (p_tickTime < 20) {
			SetCollision(colliderType::Solid);
			p_tileID = 1;
		}
		if (p_tickTime > 20) {
			SetCollision(colliderType::Air);
			p_tileID = 7;
		}
		if (p_tickTime > 40) { p_tickTime -= 40; }
	}
	if (isPlatform)
	{
		if (p_tickTime < 20) {
			SetCollision(colliderType::Solid);
			p_tileID = 3;
		}
		if (p_tickTime > 20) {
			SetCollision(colliderType::Air);
			p_tileID = 7;
		}
		if (p_tickTime > 40) { p_tickTime -= 40; }
	}
	if (isRope)
	{
		rope_end.x = rope_anchor.x + rope_length * sin(rope_Angle);
		rope_end.y = rope_anchor.y + rope_length * cos(rope_Angle);
		const float angularAcceleration = -rope_gravity / rope_length * sin(rope_Angle);
		rope_Velocity += angularAcceleration * deltaTime;
		rope_Velocity *= rope_dampingFactor;
		rope_Angle += rope_Velocity * deltaTime;
	}
}
void Tile::SetupRope(float2 anchor, float length, float gravity, float dampingFactor)
{
	rope_anchor = anchor;
	rope_length = length;
	rope_gravity = gravity;
	rope_dampingFactor = dampingFactor;
	rope_Angle = 0.5f;
	rope_Velocity = 0;
	rope_end = float2(0, 0);
}
Tile Tile::GetRopeEndTile()
{
	Tile temp;
	temp.SetWidthHeight(10, 10);
	temp.SetPosition(rope_end - (temp.GetDimensions()/2));
	temp.SetCollision(colliderType::Trigger);
	temp.SetID(7);
	return temp;
}
