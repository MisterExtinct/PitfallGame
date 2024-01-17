// Template, IGAD version 3
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2023

#pragma once

namespace Tmpl8
{

	class Game : public TheApp
	{
	public:
		// game flow methods
		void Init();
		void Tick(float deltaTime);
		void LoadLevel(char* levelFileName);
		void DrawLevel();
		void DrawPlayer();
		void DrawScore(); 
		void Physics();
		void DrawRope();
		void Shutdown() { /* implement if you want to do something on exit */ }
		// input handling
		void MouseUp(int) { /* implement if you want to detect mouse button presse s */ }
		void MouseDown(int) { /* implement if you want to detect mouse button presses */ }
		void MouseMove(int x, int y) { mousePos.x = x, mousePos.y = y; }
		void MouseWheel(float) { /* implement if you want to handle the mouse wheel */ }
		void KeyUp(int) { /* implement if you want to handle keys */ }
		void KeyDown(int) { /* implement if you want to handle keys */ }
		// data members
		int2 mousePos;

		float cameraX, cameraY, worldX, worldY;
		float2 drawOffset;
		//float playerX = 0, playerY = 16;
		//int2 playerPos;

		Player player;
		KeySystem keyboard;
		//int loadedLevel[16][80];
		Tile loadedTiles[16][80];
		Tile ropeEndTile;
		int collisionSheet[16][80];
		int collumnSize, rowSize; //colums and rows
		bool debug = false;
		bool direction = true; //true = going right | false = going left
		inline static Sprite tileSheet = Sprite(new Surface("assets/Pitfall_tilesheet.png"), 21);
		bool touchingBelow, touchingTop, touchingSide, isGrounded, isOnLadder, isGettingOffRope;
		bool ladderState, ropeState;
		float deltaTimeSec;
	};

} // namespace Tmpl8