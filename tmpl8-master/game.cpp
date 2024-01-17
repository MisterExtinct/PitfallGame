// Template, IGAD version 3
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2023

#include "precomp.h"
#include "game.h"

#include <charconv>
#include <format>

using namespace Tmpl8;


/*=========== T O D O ===========

	CLEAN UP CODE  =====  PLEASE DO IT

	Might be able to make the loadedLevel/collisionSheet smaller by assigning it a smaller storage value, i dont expect to be using 32 bits lmao
*/

//STRING ALLOWED BUT AVOID IT

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------


//checktiletouch using the corner of the tiles
bool CheckTileTouch(Tile A, Tile B)
{
	if (A.GetCornerData(Tile::TL).x < B.GetCornerData(Tile::BR).x &&
		A.GetCornerData(Tile::BR).x > B.GetCornerData(Tile::TL).x &&
		A.GetCornerData(Tile::TL).y < B.GetCornerData(Tile::BR).y &&
		A.GetCornerData(Tile::BR).y > B.GetCornerData(Tile::TL).y)
	{
		return true;
	}
	return false;
}
Tile ConvertPlayerToTileData(Player player)
{
	Tile temp;
	temp.SetPosition(player.p_PlayerPos);
	temp.SetWidthHeight(player.p_PlayerDim.x, player.p_PlayerDim.y);
	return temp;
}

bool GetKetStateDown(int key)
{
	static bool previousState = false;
	bool currentState = (GetAsyncKeyState(key) & 0x8000) != 0;

	if (currentState && !previousState)
	{
		previousState = currentState;
		return true;
	}

	previousState = currentState;
	return false;
}

void Game::Init()
{
	// anything that happens only once at application start goes here
	player.p_PlayerPos = float2(0, 0);
	player.p_PlayerVel = float2(0, 0);
	player.p_PlayerDim = float2(32, 32);
	player.playerSpeed = 20;
	player.playerJumpHeight = 50;
	deltaTimeSec = 0;

	ropeState = false;
	isGettingOffRope = false;
	rowSize = sizeof loadedTiles / sizeof loadedTiles[0];
	collumnSize = sizeof loadedTiles[0] / sizeof(Tile);


	printf("Loading level data\n");
	LoadLevel("assets/PitfallMap.csv");
	printf("Level data loaded!\n");
	ladderState = false;
	ropeState = false;
}

/*
What does this doooo??? (unprofessional i know)
Loads a CSV path link into array
it grabs every cycle the CHAR and sees if its a important character
Important     : 0 1 2 3 4 5 6 7 8 9 -
Not important : , \ \n
it collects them over time into a value, which (thanks to GeeksforGeeks) becomes a value by modifying into a parseable ascii
if it detected a - beforehand it will flip it
this then gets added to its slot in the array
*/
void Game::LoadLevel(char* fileName)
{
	//int findX = 0, findY = 0;
	int lX = 0;
	int lY = 0;
	//string holder = raw;
	string raw = TextFileRead(fileName);
	int i = 0;
	bool isNeg = false;

	//Tile reminders
	/*
	 21: rope
	 3:  Toggling platform
	 1:  Deathpit
	 5:  ladder
	 */

	

	
	for (char c : raw) { //Stores the file data into raw instead of evaluating it in the for loop, prevents unecessary reads to the file.

		if (c == ',' || c == '\\' || c == '\n') // RESET
		{
			//loadedLevel[lY][lX] = i;
			loadedTiles[lY][lX].SetID(i);
			loadedTiles[lY][lX].SetPosition(lX * 32, lY * 32);
			loadedTiles[lY][lX].SetWidthHeight(32, 32);

			if (i == 0 || i == 6) { loadedTiles[lY][lX].SetCollision(Tile::Solid); }
			else { loadedTiles[lY][lX].SetCollision(Tile::Air); }
			if (i == 1) { loadedTiles[lY][lX].isDeathHole = true; }
			if (i == 3) { loadedTiles[lY][lX].isPlatform = true; }
			if (i == 21)
			{
				loadedTiles[lY][lX].isRope = true;
				loadedTiles[lY][lX].SetupRope(loadedTiles[lY][lX].GetCenterPosition(), 100, 9.81, 1);
			}
			lX++;
			i = 0;
			isNeg = false;
			if (lX > collumnSize - 1) { lX = 0; lY++; }
		}
		else {
			if (c >= '0' && c <= '9') { //https://www.geeksforgeeks.org/convert-string-to-int-in-cpp/
				i = i * 10 + (c - '0');
			}
			if (c == '-') {  //remember if its a negative value
				isNeg = true;
			}
		}
		if (isNeg) {//flip it when negative
			i *= -1;
		}
	}

	
}
//DRAWOFFSET = WORLD + CAMERAOFFSET

//buffer could be done using some fancy stuff? for now just keep it to sprites
void Game::DrawLevel() {
	//surprisingly fast!!!
	const float holdTime = deltaTimeSec;

	for (int iY = 0; iY < rowSize; iY++) for (int iX = 0; iX < collumnSize; iX++)
	{
		loadedTiles[iY][iX].AddTickTime(holdTime);
		if (32 * iX + drawOffset.x > SCRWIDTH || 32 * iX + drawOffset.y + 64 < 0) {
			continue;//do not draw tiles offscreen
		}

		loadedTiles[iY][iX].DrawTile(screen, drawOffset, tileSheet);

		if (debug && loadedTiles[iY][iX].GetCollisionType() == Tile::Solid) {
			screen->Box(32 * iX + drawOffset.x, 32 * iY + drawOffset.y, 32 * iX + drawOffset.x + 32, 32 * iY + drawOffset.y + 32, 0x0000ff);
		}
	}

	for (int iY = 0; iY < rowSize; iY++) for (int iX = 0; iX < collumnSize; iX++) {
		if (32 * iX + drawOffset.x > SCRWIDTH || 32 * iX + drawOffset.y + 64 < 0) {
			continue;//do not draw tiles offscreen
		}
		if (loadedTiles[iY][iX].isRope)
		{
			loadedTiles[iY][iX].DrawTile(screen, drawOffset, tileSheet);
			if (debug)
			{
				screen->Box(loadedTiles[iY][iX].GetRopeEndTile().GetCornerData(Tile::TL).x + drawOffset.x, loadedTiles[iY][iX].GetRopeEndTile().GetCornerData(Tile::TL).y + drawOffset.y, loadedTiles[iY][iX].GetRopeEndTile().GetCornerData(Tile::BR).x + drawOffset.x, loadedTiles[iY][iX].GetRopeEndTile().GetCornerData(Tile::BR).y + drawOffset.y, 0x000000);
			}
		}
	}

	/*
	allocated space for probable draw rework to accomodate for drawing pixels instead of tiles, not required, but possible later on
	Not enough time anymore
	*/

	if (debug) { screen->Box(SCRWIDTH / 4, 0, SCRWIDTH / 4 * 3, SCRHEIGHT, 0xffffff); }


	//this could be done neater, but it works for now
	if (player.p_PlayerPos.x + player.p_PlayerDim.x > ((SCRWIDTH / 4) * 3) + cameraX) {
		const int intersection = 32 + player.p_PlayerPos.x - (((SCRWIDTH / 4) * 3) + cameraX);
		drawOffset.x -= intersection;
		cameraX += intersection;
	}
	if (player.p_PlayerPos.x < (SCRWIDTH / 4) + cameraX) {
		const int intersection = ((SCRWIDTH / 4)) + cameraX - player.p_PlayerPos.x;
		drawOffset.x += intersection;
		cameraX -= intersection;
	}
}
//drawing the player: coordinate + drawing offset
//additional offsets might be required to draw it properly
void Game::DrawPlayer() {
	screen->Box(player.p_PlayerPos.x + (drawOffset.x), player.p_PlayerPos.y + (drawOffset.y), player.p_PlayerPos.x + 32 + (drawOffset.x), player.p_PlayerPos.y + 32 + (drawOffset.y), 0xffffff);
	if (debug)
	{
		const MyString mString = MyString("PlayerPos: ") + MyString((int)player.p_PlayerPos.x) + " " + MyString((int)player.p_PlayerPos.y);
		screen->Print(mString.GetString(), player.p_PlayerPos.x + (drawOffset.x), player.p_PlayerPos.y + (drawOffset.y), 0xffffff);

		//debug text to show ground state
		const MyString mString2 = MyString("Grounded: ") + (isGrounded ? "true" : "false");
		screen->Print(mString2.GetString(), player.p_PlayerPos.x + (drawOffset.x), player.p_PlayerPos.y + (drawOffset.y) + 5, isGrounded ? 0x00ff00 : 0xff0000);

		//debug text to show ladder state
		const MyString mString3 = MyString("Ladder: ") + (isOnLadder ? "true" : "false");
		screen->Print(mString3.GetString(), player.p_PlayerPos.x + (drawOffset.x), player.p_PlayerPos.y + (drawOffset.y) + 10, isOnLadder ? 0x00ff00 : 0xff0000);

		//debug line of player velocity
		screen->Line(player.p_PlayerPos.x + (drawOffset.x) + 16, player.p_PlayerPos.y + (drawOffset.y) + 16, player.p_PlayerPos.x + (drawOffset.x) + 16 + player.p_PlayerVel.x, player.p_PlayerPos.y + (drawOffset.y) + 16 + player.p_PlayerVel.y, 0xffffff);
	}
}
//simple AABB 


/*
	A short (not) explanation of my janky collision system

		[ ]	[ ]	[L]
		[ ]	[P]	[L]
		[X]	[X]	[X]

	X = floor
	P = Player
	L = Ladder

	Check for collision in a 3x3 square around the player instead of every single tile using AABB
	To do this we need to find the position of the player on the map!
	Getting the middle point of the player is done through:

	POS + (width/2) same with the other axis

	Then we find the Tile
	X/Y = (playerpos.X/Y + (width/2)) / tilesize;

	Optimization moment!

	by using the direction of the player we can limit the search area to :

		<- LEFT				RIGHT ->
		[ ]	[ ]				[ ] [L]
		[ ]	[P]				[P] [L]
			[X] [X]		[X] [X]

	This might bring some movement exploits by jumping backwards for example, but this should not be possible.
	jumping at certain moments *might* cause the player to clip, but this should also not be possible.


	Player :

		X	X
		  C
		X	X
*/
void Game::Physics() {
	touchingBelow = false;
	touchingTop = false;
	touchingSide = false;
	isOnLadder = false;

	//check if the player is on a ladder and do appropriate stuff
	int2 playerPosOnGrid;
	playerPosOnGrid.x = (player.p_PlayerPos.x + 16) / 32;
	playerPosOnGrid.y = (player.p_PlayerPos.y + 16) / 32;
	isOnLadder = loadedTiles[playerPosOnGrid.y][playerPosOnGrid.x].GetID() == 5 ? true : false;
	//gravity func

	if (!ladderState && !ropeState) { //only happens if they aren't locked in a certain state
		player.p_PlayerVel.y += 0.3f;
	}
	else if (ropeState) //if the player is on a ladder, lock the y velocity to 0
	{
		player.p_PlayerVel.y = 0;
	}
	else if (ladderState) //if the player is on a ladder, lock the y velocity to 0. And fix the X alignment to the ladder
	{
		player.p_PlayerPos.x = loadedTiles[playerPosOnGrid.y][playerPosOnGrid.x].GetCornerData(Tile::TL).x;
		player.p_PlayerVel.y = 0;
	}

	for (int iY = 0; iY < rowSize; iY++) for (int iX = 0; iX < collumnSize; iX++)
	{
		if(iX < 0 || iX > collumnSize || iY < 0 || iY > rowSize) { continue; } //prevent out of bounds errors)
		if (loadedTiles[iY][iX].isRope && !isGettingOffRope)
		{
			//draw debug bog at the end of the rope using the end of the rope tile
			Tile endTile = loadedTiles[iY][iX].GetRopeEndTile();
			screen->Box(endTile.GetCornerData(Tile::TL).x + drawOffset.x, endTile.GetCornerData(Tile::TL).y + drawOffset.y, endTile.GetCornerData(Tile::BR).x + drawOffset.x, endTile.GetCornerData(Tile::BR).y + drawOffset.y, 0x000000);
			if (CheckTileTouch(ConvertPlayerToTileData(player), loadedTiles[iY][iX].GetRopeEndTile()) && loadedTiles[iY][iX].IsUpdatable())
			{
				//lock player to end of rope
				ropeState = true;
				ropeEndTile = loadedTiles[iY][iX].GetRopeEndTile();
				//break out of for loop

			}
		}
	};

	if (ropeState)
	{
		//attach player to rope
		player.p_PlayerPos = ropeEndTile.GetCenterPosition() - (ConvertPlayerToTileData(player).GetDimensions() / 2);
	}

	//printf("touching : ");

	//main collision loop
	for (int i = 0; i < 5; i++) {
		if (ropeState) { break; } //if the player is attached to a rope, skip the collision loop

		int2 findTile = int2(0, 0);

		switch (i)
		{
		case 0: // BELOW THE PLAYER
			findTile.x = ((player.p_PlayerPos.x + 16) / 32);
			findTile.y = ((player.p_PlayerPos.y + 16) / 32) + 1;
			touchingBelow = (CheckTileTouch(ConvertPlayerToTileData(player), loadedTiles[findTile.y][findTile.x]) && loadedTiles[findTile.y][findTile.x].GetCollisionType() == Tile::Solid);
			break;
		case 1: // BELOW, OPPOSITE DIRECTION
			findTile.x = ((player.p_PlayerPos.x + 16) / 32) + (direction ? -1 : 1);
			findTile.y = ((player.p_PlayerPos.y + 16) / 32) + 1;
			touchingBelow = (CheckTileTouch(ConvertPlayerToTileData(player), loadedTiles[findTile.y][findTile.x]) && loadedTiles[findTile.y][findTile.x].GetCollisionType() == Tile::Solid);
			//touchingSide = (CheckTileTouch(player.p_PlayerPos, loadedTiles[findTile.y][findTile.x].GetCornerData(Tile::TL)) && loadedTiles[findTile.y][findTile.x].IsCollider()); // for some reason this is rather buggy?
			break;
		case 2: // DIRECTION OF PLAYER
			findTile.x = ((player.p_PlayerPos.x + 16) / 32) + (direction ? 1 : -1);
			findTile.y = ((player.p_PlayerPos.y + 16) / 32);
			touchingSide = (CheckTileTouch(ConvertPlayerToTileData(player), loadedTiles[findTile.y][findTile.x]) && loadedTiles[findTile.y][findTile.x].GetCollisionType() == Tile::Solid);
			break;
		case 3: // ABOVE PLAYER
			findTile.x = ((player.p_PlayerPos.x + 16) / 32);
			findTile.y = ((player.p_PlayerPos.y + 16) / 32) - 1;
			touchingTop = (CheckTileTouch(ConvertPlayerToTileData(player), loadedTiles[findTile.y][findTile.x]) && loadedTiles[findTile.y][findTile.x].GetCollisionType() == Tile::Solid);
			break;
		case 4: // ABOVE, DIRECTION OF PLAYER
			findTile.x = ((player.p_PlayerPos.x + 16) / 32) + (!direction ? 1 : -1);
			findTile.y = ((player.p_PlayerPos.y + 16) / 32) - 1;
			touchingTop = (CheckTileTouch(ConvertPlayerToTileData(player), loadedTiles[findTile.y][findTile.x]) && loadedTiles[findTile.y][findTile.x].GetCollisionType() == Tile::Solid);
			//touchingSide = (CheckTileTouch(player.p_PlayerPos, loadedTiles[findTile.y][findTile.x].GetCornerData(Tile::TL)) && loadedTiles[findTile.y][findTile.x].IsCollider());
			break;
		default:
			break;
		}

		//Detect if the player is intersecting with a wall/obstacle and move the player back to prevent clipping (does the proper distance)
		if (touchingSide) {
			if (direction) {
				const float distanceIntersectingX = player.p_PlayerDim.x + player.p_PlayerPos.x - loadedTiles[findTile.y][findTile.x].GetCornerData(Tile::TL).x;
				player.p_PlayerPos.x -= distanceIntersectingX;
				player.p_PlayerVel.x *= -1;
			}
			else
			{
				const float distanceIntersectingX = loadedTiles[findTile.y][findTile.x].GetCornerData(Tile::TR).x - player.p_PlayerPos.x;
				player.p_PlayerPos.x += distanceIntersectingX;
				player.p_PlayerVel.x *= -1;
			}
			touchingSide = false;
			direction = !direction;
			//printf("Side, ");
		}
		if (touchingBelow) {
			const int distanceIntersectingY = player.p_PlayerDim.x + player.p_PlayerPos.y - loadedTiles[findTile.y][findTile.x].GetCornerData(Tile::TL).y;
			player.p_PlayerPos.y -= distanceIntersectingY;
			player.p_PlayerVel.y = 0;
			isGrounded = true;
			touchingBelow = false;
			isGettingOffRope = false;
			//printf("Below, ");
		}
		if (touchingTop) {
			const int distanceIntersectingY = player.p_PlayerPos.y - loadedTiles[findTile.y][findTile.x].GetCornerData(Tile::BL).y;
			player.p_PlayerPos.y -= distanceIntersectingY;
			player.p_PlayerVel.y *= -1;
			touchingTop = false;
			//printf("Top, ");
		}


		//debug
		screen->Box(loadedTiles[findTile.y][findTile.x].GetCornerData(Tile::TL).x + drawOffset.x, loadedTiles[findTile.y][findTile.x].GetCornerData(Tile::TL).y + drawOffset.y, loadedTiles[findTile.y][findTile.x].GetCornerData(Tile::BR).x + drawOffset.x, loadedTiles[findTile.y][findTile.x].GetCornerData(Tile::BR).y + drawOffset.y, 0xff0000);
		screen->Box(loadedTiles[playerPosOnGrid.y][playerPosOnGrid.x].GetCornerData(Tile::TL).x + drawOffset.x, loadedTiles[playerPosOnGrid.y][playerPosOnGrid.x].GetCornerData(Tile::TL).y + drawOffset.y, loadedTiles[playerPosOnGrid.y][playerPosOnGrid.x].GetCornerData(Tile::BR).x + drawOffset.x, loadedTiles[playerPosOnGrid.y][playerPosOnGrid.x].GetCornerData(Tile::BR).y + drawOffset.y, 0x00ff00);
		if (loadedTiles[playerPosOnGrid.y][playerPosOnGrid.x].isDeathHole) {
			if (CheckTileTouch(ConvertPlayerToTileData(player), loadedTiles[playerPosOnGrid.y][playerPosOnGrid.x])) {
				printf("deathpit");
				player.p_PlayerPos = float2(0, 0);
				player.p_PlayerVel = float2(0, 0);
			}
		}
	}

	if (isGrounded)
	{
		player.p_PlayerVel.x = lerp(player.p_PlayerVel.x, 0, 0.2);
	}
	//printf("\n");
	player.p_PlayerPos += player.p_PlayerVel * deltaTimeSec;
}

void Game::DrawScore() {


}

// -----------------------------------------------------------
// Main application tick function - Executed once per frame 
// -----------------------------------------------------------
void Game::Tick(float val)
{
	// clear the screen to black
	screen->Clear(0);
	deltaTimeSec = val / 100.0;
	if (debug)
	{
		const MyString fpsString = MyString("FPS: ") + MyString(1 / deltaTimeSec);
		screen->Print(fpsString.GetString(), 0, 0, 0xffffff);
	}

	keyboard.updateKeyStates();

	if (!ropeState) {
		if (isOnLadder) {
			if (keyboard.isPressed('W')) {
				ladderState = true;
				isGrounded = false;
				player.p_PlayerPos.y -= player.playerSpeed / 20;
			}
			if (keyboard.isPressed('S')) {
				player.p_PlayerPos.y += player.playerSpeed / 20;
				if (isGrounded) {
					ladderState = false;
				}
			}
		}
		else if (!isOnLadder && ladderState) // litterally just abusing a bug, use this kind of limbo state to allow the player to be launched to the side or go back down.
		{
			if (keyboard.isPressedThisFrame('D')) {
				player.p_PlayerVel.x = player.playerSpeed;
				player.p_PlayerVel.y = -player.playerJumpHeight;
				ladderState = false;
				direction = true;
			}
			if (keyboard.isPressedThisFrame('A')) {
				player.p_PlayerVel.x = -player.playerSpeed;
				player.p_PlayerVel.y = -player.playerJumpHeight;
				ladderState = false;
				direction = false;
			}
			if (keyboard.isPressed('S')) { //In case you want to go back down again, just a simple fix to a bug
				player.p_PlayerPos.y += player.playerSpeed / 20;
			}
		}
		if (!ladderState && isGrounded) { // normal control
			if (keyboard.isPressed('D')) {
				player.p_PlayerVel.x = player.playerSpeed;
				direction = true;

			}
			if (keyboard.isPressed('A')) {
				player.p_PlayerVel.x = -player.playerSpeed;
				direction = false;
			}
			if (keyboard.isPressed('W')) {
				player.p_PlayerVel.y = -player.playerJumpHeight;
				isGrounded = false;
			}
		}
	}
	else //getting off the rope
	{
		if (keyboard.isPressedThisFrame('D'))
		{
			player.p_PlayerVel.x = player.playerSpeed * 2;
			direction = true;
			ropeState = false;
			isGettingOffRope = true;
		}
		if (keyboard.isPressedThisFrame('A'))
		{
			player.p_PlayerVel.x = -player.playerSpeed * 2;
			direction = false;
			ropeState = false;
			isGettingOffRope = true;
		}
	}

	if (keyboard.isPressedThisFrame('Q')) {
		debug = !debug;
	}
	if (keyboard.isPressedThisFrame('R'))
	{
		player.p_PlayerPos.x = 0;
		player.p_PlayerPos.y = 0;
	}

	// the essentials
	Physics();
	DrawLevel();
	DrawPlayer();
	DrawScore();
	//DrawRope();
	//~6ms for a frame as of (27/09/23)
}