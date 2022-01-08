#pragma once
#include "Password_cracker.h"

class Password_cracker_full_ascii : public Password_cracker
{
public:
	string crack_password(const string& password) override;
private:
	unsigned char find_matching_ascii(int steps, int offset) override;
};

