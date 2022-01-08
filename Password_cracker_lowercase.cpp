#include "Password_cracker_lowercase.h"


/* Generate lowercase letter password that will encrypt to input */
string Password_cracker_lowercase::crack_password(const string& password)
{
	int attempts = 0;
	int max_attemps = 100;
	string real_password = "";
	string remainder = password;
	int max_ascii_len = min(3, int(remainder.length()));
	int current_val = -1;
	/* Process first value */
	/* Starting with digits 3 down to 1 */
	for (int i = max_ascii_len; i >= 1; i--)
	{
		current_val = stoi(remainder.substr(0, i));

		unsigned char match = find_matching_ascii(current_val, 0);

		if (match != 0)
		{
			remainder = remainder.substr(i, remainder.length());
			real_password += match;
			break;
		}

	}

	/* Process further values */
	while (!remainder.empty())
	{
		attempts++;
		if (attempts == max_attemps)
			return "failed";

		int new_val = -1;
		max_ascii_len = min(3, int(remainder.length()));

		/* Starting with digits 3 down to 1*/
		for (int i = max_ascii_len; i >= 1; i--)
		{
			new_val = stoi(remainder.substr(0, i));

			unsigned char match = find_matching_ascii(new_val, current_val);

			if (match != 0)
			{
				remainder = remainder.substr(i, remainder.length());
				real_password += match;
				current_val = new_val;
				break;
			}

		}
	}

	return real_password;
}

/* Find the first lowercase ascii value which will give 'steps' when fed into collatz function with 'offset' */
unsigned char Password_cracker_lowercase::find_matching_ascii(int steps, int offset)
{
	ifstream file("extended_ascii.txt");

	if (file.is_open())
	{
		string line;
		// For every line
		while (getline(file, line))
		{
			// Get val : collatz_steps
			string val = line.substr(0, line.find(" : "));
			string collatz_steps = line.substr(line.find(":") + 2, line.length());

			// If steps match
			if (steps == stoi(collatz_steps))
			{
				if (stoi(val) >= 97 + offset && stoi(val) <= 122 + offset)	// Only allow lowercase letter ascii codes
					return stoi(val) - offset;
			}

		}
	}

	return 0;
}
