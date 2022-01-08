// Demonstrate compile time optimizations
// Some compilers can do compilation speed optimizations using pragma and guard conditions
#pragma once
#ifndef password_cracker_h
#define password_cracker_h

#include <string>
#include <fstream>	// Forward declaration for compile time optimization
#include <algorithm>
using namespace std;

class Password_cracker
{
public:
	virtual string crack_password(const string& password);
private:
	virtual unsigned char find_matching_ascii(int steps, int offset);
};

#endif	