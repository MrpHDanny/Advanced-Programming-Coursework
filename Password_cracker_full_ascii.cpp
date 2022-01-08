#include "Password_cracker_full_ascii.h"

/* Generate extended ascii letter password that will encrypt to input */
string Password_cracker_full_ascii::crack_password(const string& password)
{
	int attempts = 0;
	int max_attempts = 100;
	string real_password = "";
	string remainder = password;
	int max_ascii_len = min(3, int(remainder.length()));
	int current_val = -1;

	/* Process first value */
	/* Starting with digit 1 through 3 */
	for (int i = 1; i <= max_ascii_len; i++)
	{
		current_val = stoi(remainder.substr(0, i));

		// AVOID PROCESSING VALUE IF THE NEXT NEXT VALUE IS A 0
		if (i + 2 <= max_ascii_len)
		{
			if (stoi(remainder.substr(i + 1, 1)) == 0)	// if next digit is 0, continue
			{
				i++;
				continue;
			}
		}
		// AVOID PROCESSING VALUE IF THE NEXT VALUE IS A 0
		if (i + 1 <= max_ascii_len)
		{
			if (stoi(remainder.substr(i, 1)) == 0)	// if next digit is 0, continue
			{
				i++;
				continue;
			}
		}

		unsigned char match = find_matching_ascii(current_val, 0);

		// If matched
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
		if (attempts == max_attempts)
			return "failed";
		int new_val = -1;
		max_ascii_len = min(3, int(remainder.length()));

		/* Starting with digit 1 through 3 */
		for (int i = 1; i <= max_ascii_len; i++)
		{
			new_val = stoi(remainder.substr(0, i));

			// AVOID PROCESSING VALUE IF THE NEXT NEXT VALUE IS A 0
			if (i + 2 <= max_ascii_len)
			{
				if (stoi(remainder.substr(i + 1, 1)) == 0)	// if next digit is 0, continue
				{
					i++;
					continue;
				}
			}
			// AVOID PROCESSING VALUE IF THE NEXT VALUE IS A 0
			if (i + 1 <= max_ascii_len)
			{
				if (stoi(remainder.substr(i, 1)) == 0)	// if next digit is 0, continue
				{
					i++;
					continue;
				}
			}

			unsigned char match = find_matching_ascii(new_val, current_val);
			// If matched
			if (match != 0)
			{
				remainder = remainder.substr(i, remainder.length());
				real_password += match;
				current_val = new_val;
				attempts = 0;
				break;
			}

			// If we failed with val xx0, just try 1 digit as last effort
			if (i == max_ascii_len && match == 0)
			{
				new_val = stoi(remainder.substr(0, 1));
				unsigned char match = find_matching_ascii(new_val, current_val);
				if (match != 0)
				{
					remainder = remainder.substr(0, 1);
					real_password += match;
					current_val = new_val;
					attempts = 0;
					break;
				}
			}
		}
	}

	return real_password;
}

/* Find the first extended ascii value which will give 'steps' when fed into collatz function with 'offset' */
unsigned char Password_cracker_full_ascii::find_matching_ascii(int steps, int offset)
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
				if (stoi(val) - offset > 0)	// ascii value can't be 0 or less
					return stoi(val) - offset;
			}

		}
	}

	return 0;
}
