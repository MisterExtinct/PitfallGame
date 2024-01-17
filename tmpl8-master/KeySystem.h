#pragma once
class KeySystem
{
public:
	KeySystem();
	~KeySystem();
	struct Keystate
	{
		bool isPressed;
		bool isReleased;
		bool isPressedThisFrame;
		int framePressed;
	};
	void updateKeyStates();
	bool isPressed(int key);
	bool isReleased(int key);
	bool isPressedThisFrame(int key);

private:
	Keystate keyStates[256] = { 0 };
};