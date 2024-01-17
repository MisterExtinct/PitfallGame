#include "precomp.h"

using namespace Tmpl8;

MyString::MyString(const char* text)
{
	length = 0;
	while(text[length] != '\0')
	{
		length++;
	}

	data = new char[length + 1];
	for (size_t i = 0; i < length; i++)
	{
		data[i] = text[i];
	}
	data[length] = '\0';
}
MyString::MyString(int val)
{
	length = 0;
	int temp = val;
	while (temp != 0)
	{
		temp /= 10;
		length++;
	}

	data = new char[length + 1];
	for (size_t i = 0; i < length; i++)
	{
		data[i] = val % 10 + '0';
		val /= 10;
	}
	data[length] = '\0';
}

MyString::MyString(const MyString& other)
{
	length = other.length;
	data = new char[length + 1];
	for (size_t i = 0; i < length; i++)
	{
		data[i] = other.data[i];
	}
	data[length] = '\0';
}
MyString::~MyString()
{
	delete[] data;
}

const char* MyString::GetString() const
{
	return data;
}
int MyString::GetLength() const
{
	return length;
}

MyString MyString::ToString(int nParam)
{
	return MyString(nParam);
}
MyString MyString::operator+(const MyString& rhs) // made with assistance of ChatGPT "How do i write a custom C++ func for adding two strings to each other without STL"
{
	int length1 = 0;
	int length2 = 0;

	while(data[length1] != '\0')
	{
		length1++;
	}
	while(rhs.data[length2] != '\0')
	{
		length2++;
	}

	const int resultingLength = length1 + length2;

	char* resultingString = new char[resultingLength + 1];

	for(int i = 0; i < length1; i++)
	{
		resultingString[i] = data[i];
	}
	for(int i = 0; i < length2; i++)
	{
		resultingString[i + length1] = rhs.data[i];
	}

	resultingString[resultingLength] = '\0';
	return MyString(resultingString);
}



