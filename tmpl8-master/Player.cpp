#include "precomp.h"
using namespace Tmpl8;
Player::Player() {
	p_PlayerPos.x = 0;
	p_PlayerPos.y = 0;
	p_PlayerVel.x = 0;
	p_PlayerVel.y = 0;
	p_PlayerDim.x = 0;
	p_PlayerDim.y = 0;
}
Player::Player(int x, int y, int width, int height)
{
	p_PlayerPos.x = x;
	p_PlayerPos.y = y;
	p_PlayerVel.x = 0;
	p_PlayerVel.y = 0;
	p_PlayerDim.x = width;
	p_PlayerDim.y = height;
}
Player::Player(int2 position, int2 dimensions)
{
	p_PlayerPos = position;
	p_PlayerVel.x = 0;
	p_PlayerVel.y = 0;
	p_PlayerDim = dimensions;
}

Player::~Player()
{

}



