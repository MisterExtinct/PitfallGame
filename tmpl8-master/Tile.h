#pragma once
using namespace Tmpl8;

class Tile
{
public:
	Tile();
	~Tile();
	enum Corner
	{
		TL, TR, BL, BR
	};
	enum colliderType
	{
		Air, Solid, Trigger
	};
	void SetID(int id);
	int GetID();
	//doing bool IsCollider(); { return p_collision; } seems to be breaking things?
	void SetCollision(colliderType colType);
	float2 GetCornerData(Corner corner);
	float2 GetCenterPosition();
	void SetPosition(float2 vecpos);
	void SetPosition(int x, int y);
	void SetWidthHeight(int width, int height);
	float2 GetDimensions() { return float2(p_width, p_height); };
	void AddTickTime(float deltaTime);
	void DrawTile(Surface* screen, float2 screenOffset, Sprite& sprite);
	void SetupRope(float2 anchor, float length, float gravity, float dampingFactor);
	bool isDeathHole, isPlatform, isRope;
	bool IsUpdatable() { return p_tickTime > 0; };
	Tile GetRopeEndTile();
	float GetRopeVelocity() { return rope_Velocity; };
	const colliderType GetCollisionType() { return p_collision; };
private:
	float p_tickTime;
	float2 p_pos;
	int p_tileID;
	colliderType p_collision;
	int p_width, p_height;

	//rope variables
	float rope_length, rope_gravity, rope_dampingFactor, rope_Angle, rope_Velocity;
	float2 rope_anchor, rope_end;
};