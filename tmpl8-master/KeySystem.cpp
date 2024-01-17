#include "precomp.h"
using namespace Tmpl8;

KeySystem::KeySystem()
{

}
KeySystem::~KeySystem()
{
	
}
bool KeySystem::isPressed(int key)
{
	return keyStates[key].isPressed;
}
bool KeySystem::isReleased(int key)
{
	return keyStates[key].isReleased;
}
bool KeySystem::isPressedThisFrame(int key)
{
		return keyStates[key].isPressedThisFrame;
}
void KeySystem::updateKeyStates()
{
	for (int i = 0; i < 256; i++)
	{
		const Keystate lastState = keyStates[i];
		keyStates[i].isPressed = GetAsyncKeyState(i) & 0x8000; //is the key pressed

		keyStates[i].isPressedThisFrame = keyStates[i].isPressed && !lastState.isPressed; //is the key pressed this frame

		keyStates[i].isReleased = !keyStates[i].isPressed && lastState.isPressed; //is the key released this frame
	}
}
