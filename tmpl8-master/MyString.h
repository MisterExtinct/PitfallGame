#pragma once
class MyString
{
public:
	MyString();
	MyString(const char* text);
	MyString(const MyString& rhs);
	MyString(int val);
	MyString(float& val, int numBehindDot);
	~MyString();

	MyString& operator=(const MyString& rhs);
	MyString& operator=(const char* pszParam);

	MyString operator+(const MyString& rhs);
	MyString& operator+=(const MyString& rhs);

	bool operator==(const MyString& rhs);
	bool operator!=(const MyString& rhs);

	char& operator[](int nIndex);
	const char& operator[](int nIndex) const;

	int GetLength() const;
	const char* GetString() const;

	MyString ToString(int nParam);
private:
	char* data;
	size_t length;
};