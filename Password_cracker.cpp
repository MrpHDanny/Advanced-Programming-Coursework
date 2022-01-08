#include "Password_cracker.h"
#include <iostream>

// Only implemented to demonstrate polymorphism
string Password_cracker::crack_password(const string& password)
{
	cout << "Base class 'crack_password' method called" << endl;
	return "base type method called";
}

unsigned char Password_cracker::find_matching_ascii(int steps, int offset)
{
	return '0';
}
