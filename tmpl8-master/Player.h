#pragma once
using namespace Tmpl8;

class Player
{
public:
	Player();
	Player(int x, int y,int width,int height);
	Player(int2 position, int2 dimensions);
	~Player();

	float playerSpeed,playerJumpHeight;
	float2 p_PlayerPos, p_PlayerVel, p_PlayerDim;
private:
	
};

