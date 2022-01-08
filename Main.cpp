	#include <iostream>
	#include <chrono>
	#include <queue>
	#include <any>
	#include <algorithm>
	#include <string>
	#include <thread>
	#include <fstream>	// Forward declaration for compile time optimization
	#include "UserInterface.h"
	#include <filesystem>
	#include <random>
	#include <string_view>
	#include "Password_cracker.h"
	#include "Password_cracker_lowercase.h"
	#include "SmartPtr.h"
	#include "Password_cracker_full_ascii.h"
	#include <functional>

using namespace std;

// Encryption

int collatz(any num); // Demonstrate any keyword
string encrypt(string& password); // Demonstrate compile time optimization by passing by reference

// User login functions

void create_new_credentials();
void authenticate_user();

// Crack passwords

void crack_passwords();

// Helper functions

void generate_passwords();
inline void say_goodbye() { cout << "Goodbye!" << endl; } // inline function for compile time optimization

// Sentence cracking functions
void crack_all_possiblilities(string password);
void print_possibilities(vector<string> input);
void find_printable_ascii_matches(vector<unsigned char>& matches, int steps, int offset);

// Classes
UserInterface UI;
SmartPtr<Password_cracker> lowercase_cracker(new Password_cracker_lowercase);	// Demonstrate smart pointer
SmartPtr<Password_cracker> full_ascii_cracker(new Password_cracker_full_ascii);

// Note: C++20 standard was used in Visual studio project settings for this coursework

int main()
{
	
	int user_choice = 0;

	while (user_choice != 5)
	{
		user_choice = UI.select_menu_option();
		switch (user_choice)
		{
		case 1:
			create_new_credentials();
			break;
		case 2:
			authenticate_user();
			break;
		case 3:
			cout << "Generating passwords..." << endl;
			//generate_passwords();
			
			// Demonstrate threaded programming 
			// The user does not have to wait for the password generation
			// to complete before moving on.
			// (A bad place and way of demonstrating threaded programming)
			[]()
			{
				thread td(generate_passwords);
				td.detach();
			}();

			cout << "Done!" << endl;
			break;
		case 4:
			crack_passwords();
			break;
		default:
			break;
		}
	}


	// Demonstrate Pointer to a function, pointer to pointer, auto keyword
	void (*bye)();
	bye = &say_goodbye;

	auto ptr_to_ptr = &bye;

	(*ptr_to_ptr)();	// Say goodbye once program finishes

	// Demonstrate polymorphism
	// lowercase_cracker and full_ascii_cracker are Pointers of Base type but are instantiated to derived classes
	// thus, only the derived class methods are called in the program 
	// Code below shows base class method being called by instantiated base type 
	
	/*
	string pass = "daniel";
	string encrypted = encrypt(pass);

	Password_cracker* pw = new Password_cracker();

	pw->crack_password(encrypted); // Will print to console that base class method called
	cout << lowercase_cracker->crack_password(encrypted) << endl;	// Will call derived class method
	*/


	// Attempt to crack the given sentence (give it 10-15 seconds to finish executing and print results)
	// This will spit out 77 lines of characters (total length of the encrypted sentence).
	// Each line shows every possible match for each character in the sentence.
	// If this was implemented correctly, the sentence should be hidden in those lines with the first character of the 
	// sentence being on line 0 (A), second character on line 1 (space), etc...
	// To print out all possible combinations of characters for each line would simply take too long.

	//string encrypted_sentence = "27322810313331033910211452912207344136146925461033281533271031012815108114101";
	//crack_all_possiblilities(encrypted_sentence);

	return 0;
}

/* Attempt to crack passwords and output results into file */
void crack_passwords()
{
	string input = "passwordstest.txt";
	string output = "results.txt";

	if (!filesystem::exists(output) || !filesystem::exists(input))
	{
		cout << "ERROR: FILE(-S)  \"" << output << "\", \"" << input << "\" DO(-ES) NOT EXIST" << endl;
		return;
	}

	ifstream input_file(input);
	ofstream output_file(output, ios::app);	// Output in append mode to prevent overriding previous results (they take a while to complete)

	if (input_file.is_open() && output_file.is_open())
	{
		int count = 0;
		double total_cracked = 0;
		// Standard ascii passwords
		output_file << "__Standard ascii passwords__" << endl;

		for (int len = 1; len <= 100; len++)
		{
			// Start timer
			auto start = chrono::steady_clock::now();
			for (int i = 0; i < 100; i++)
			{
				// Grab password from file
				string encrypted_password;
				getline(input_file, encrypted_password);

				// Crack password
				string generated_password = lowercase_cracker->crack_password(encrypted_password);
				string encrypted_generated_password = encrypt(generated_password);
			
				// Compare
				if (encrypted_generated_password == encrypted_password)
					count++;
			}

			// Stop timer
			auto end = chrono::steady_clock::now();
			auto elapsed_time_ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();
			output_file << "Len: " << len << "  | Cracked " << count << "/100" << " passwords  " << " avg time: " << elapsed_time_ms/100 << "ms" << "   total time: " << elapsed_time_ms << endl;
			cout << "lower_password len : " << len << " cracked: " << count << "/100" << endl;
			total_cracked += count;
			count = 0;
		}

		output_file << "Average password cracking success rate: " << total_cracked / 100 << " %\n\n\n";
		total_cracked = 0;
		// Extended ascii passwords
		output_file << "__Extended ascii passwords__" << endl;

		for (int len = 1; len <= 100; len++)
		{

			// Start timer
			auto start = chrono::steady_clock::now();
			for (int i = 0; i < 100; i++)
			{
				// Grab password from file
				string encrypted_password;
				getline(input_file, encrypted_password);

					// Crack password
					string generated_password = full_ascii_cracker->crack_password(encrypted_password);
					string encrypted_generated_password = encrypt(generated_password);

					// Compare
					if (encrypted_generated_password == encrypted_password)
						count++;
			}

			// Stop timer
			auto end = chrono::steady_clock::now();

			auto elapsed_time_ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();
			output_file << "Len: " << len << "  | Cracked " << count << "/100" << " passwords  " << " avg time: " << elapsed_time_ms / 100 << "ms" << "   total time: " << elapsed_time_ms << endl;
			cout << "full_ascii_password len : " << len << " cracked: " << count << "/100" << endl;

			total_cracked += count;
			count = 0;
		}
		output_file << "Average password cracking success rate: " << total_cracked / 100 << " %\n\n\n";
	}
	else
	{
		cout << "ERROR: COULD NOT OPEN FILE(-S)  \"" << output << "\" , \"" << input << "\"" << endl;
		return;
	}
}

/* Generates 20000 random passwords into a file */
void generate_passwords()
{
	string filename = "passwordstest.txt";
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()

	std::uniform_int_distribution<> genASCII_standard(97, 122);   // Generates random standard lowercase ascii
	std::uniform_int_distribution<> genASCII_extended(1, 255);  // Generates randon extended ascii 
	std::uniform_int_distribution<> pickASCII_standard(0, 9);  // Picks random number from 0 - 9
	std::uniform_int_distribution<> pickASCII_extended(0, 99);  // Picks random number 0 - 99  

	char standardASCII[10];
	unsigned char extendedASCII[100];

	// Pick 10 random standard ascii values
	for (int i = 0; i < 10; i++)
	{
		standardASCII[i] = genASCII_standard(gen);
	}
	
	// Pick 100 random non-duplicate extended ascii values
	for (int i = 0; i < 100; i++)
	{
		int newChar = genASCII_extended(gen);

		while (find(begin(extendedASCII), end(extendedASCII), newChar) != end(extendedASCII))
		{
			newChar = genASCII_extended(gen);
		}

		extendedASCII[i] = newChar;
	}

	if (!filesystem::exists(filename))
	{
		cout << "ERROR: FILE  \"" << filename << "\" DOES NOT EXIST";
		return;
	}

	ofstream file(filename);

	// Standard ascii passwords
	for (int len = 1; len <= 100; len++)
	{
		for (int i = 0; i < 100; i++)
		{
			string password = "";
			string encrypted_password = "";

			// Generate password
			for (int j = 0; j < len; j++)
			{
				int index = pickASCII_standard(gen);
				password += standardASCII[index];
			}

			encrypted_password = encrypt(password);

			if (file.is_open())
			{
				file << encrypted_password << '\n';
			}
			else 
			{
				cout << "ERROR: COULD NOT OPEN FILE  \"" << filename << "\"";
				return;
			}

		}
	}

	// Extended ASCII passwords
	for (int len = 1; len <= 100; len++)
	{

		for (int i = 0; i < 100; i++)
		{
			string password = "";
			string encrypted_password = "";

			// Generate password
			for (int j = 0; j < len; j++)
			{
				int index = pickASCII_extended(gen);
				password += extendedASCII[index];
			}

			encrypted_password = encrypt(password);

			if (file.is_open())
			{
				file << encrypted_password << '\n';
			}
			else 
			{
				cout << "ERROR: COULD NOT OPEN FILE  \"" << filename << "\"";
				return;
			}

		}
	}
}

/* Prompts the user to input username and password */
/* Checks if current username and password have a match in stored credentials list */
void authenticate_user()
{
	int attempts = 2;

	string filename = "password.txt";
	string username = UI.ask_for_username();
	string password = UI.ask_for_password();
	string encrypted_pw = encrypt(password);
	vector<string> correct_passwords;

	if (!filesystem::exists(filename))
	{
		cout << "ERROR: FILE  \"" << filename << "\" DOES NOT EXIST";
		return;
	}

	ifstream file("password.txt");
	string line;

	if (file.is_open())
	{
		bool found_username = false;
		bool matched_password = false;
		string correct_password;

		while (getline(file,line))
		{
			string stored_username = line.substr(0, line.find(' '));
			string stored_password = line.substr(line.find(' ') + 1, line.length());
			
			// Check if found username matches
			if (username == stored_username)
			{
				found_username = true;
				correct_passwords.push_back(stored_password);
			}
		}

		// Check if failed to find username
		if (!found_username)
		{
			cout << "ERROR: USERNAME \"" << username << "\" NOT FOUND" << endl;
			return;
		}

		// Check if any of the found passwords match input
		for (string correct : correct_passwords)
		{
			if (encrypted_pw == correct)
			{
				cout << "Success! Login details verified." << endl;
				return;
			}
		}
		
		while (attempts != 0)
		{
			cout << "ERROR: WRONG PASSWORD " << endl;
			cout << "You have " << attempts << " attempts left" << endl;
			password = UI.ask_for_password();
			encrypted_pw = encrypt(password);

			for (string correct : correct_passwords)
			{
				if (encrypted_pw == correct)
				{
					cout << "Success! Login details verified." << endl;
					return;
				}
			}
			attempts--;
		}

		cout << "ERROR: RAN OUT OF ATTEMPTS " << endl;
		return;
	}
	else
	{
		cout << "ERROR: COULD NOT OPEN FILE  \"" << filename << "\"";
		return;
	}
}

/* Prompts the user to create new username and password */
/* Stores credentials in file */
void create_new_credentials()
{
	string filename = "password.txt";
	string username = UI.ask_for_username();
	string password = UI.ask_for_password();

	if (!filesystem::exists(filename))
	{
		//cout << "ERROR: FILE  \"" << filename << "\" DOES NOT EXIST" << endl;
		
		// Demonstrate lambda function and auto keyword
		[filename]() {
			cout << "ERROR: FILE  \"" << filename << "\" DOES NOT EXIST" << endl;
		}();

		return;
	}

	ofstream file("password.txt", ios::app);
	if (file.is_open())
	{

		file << username << " " << encrypt(password) << '\n';
		cout << "Entry added to \"" << filename << "\"" << endl;
	
	}
	else
	{
		cout << "ERROR: COULD NOT OPEN FILE  \"" << filename << "\"" << endl;
		return;
	}
}

/* Encrypts password using collatz conjecture */
string encrypt(string& password)
{
	string output = "";
	int offset = 0;

	for (unsigned char c : password)
	{
		int current_char = c + offset;
		offset = collatz(current_char);
		output += to_string(offset);
	}

	return output;
}

/* Returns collatz steps for given number */
int collatz(any number)
{
	int num;
	// Catch bad cast
	try
	{
		num = any_cast<int>(number);
	}
	catch (const std::bad_any_cast& e)
	{
		std::cout << e.what() << '\n';
		return -1;
	}

	int count = 0;
	if (num < 0) num *= -1; // Fix for negative integers

	while (num != 1)
	{
		num = num % 2 == 0 ? num / 2 : 3 * num + 1;
		count++;
	}

	return count;
}

/* !EXPERIMENTAL! */
/* Generate every possible character match for every single letter position in sentence */
void crack_all_possiblilities(string password)
{
	const int max_y = 77;	// Most characters possible
	const int max_x = 100;  // Most matches for single character possible
	unsigned char possibilities[max_y][max_x];
	vector<unsigned char> matches;
	int index = 0;
	int x_count = 0;
	int y_count = 0;

	// Process first three characters
	for (int i = 1; i <= 3; i++)
	{
		int current_val = stoi(password.substr(index, i));
		//cout << "current value : " << current_val << endl;
		find_printable_ascii_matches(matches, current_val, 0);
	}
	// Insert them into possibilities array
	for (unsigned char c : matches)
	{
		cout << (int)c << '(' << c << ") ";
		possibilities[y_count][x_count] = c;
		x_count++;
	}
	y_count++; // Move to next character
	index++;   // Move to next index
	x_count = 0; // Reset x_count 
	matches.clear();
	cout << endl;

	// While end of string not reached
	while (index != password.length())
	{
		int remaining_string = password.length() - index;
		int character_pool = min(5, remaining_string);	// Number of characters to test [1,5]

		//int singles = min(3, character_pool); // Number of single characters to test [1,3]
		//int doubles = character_pool > 3 ? 3 : character_pool - 1;	 // Number of double digit characters to test [0,3]
		//int triples = character_pool >= 3 ? character_pool - 2 : 0; // Number of triple digit characters to test [0,3]

		int singles = min(remaining_string, 1 + (index * 2));
		int doubles = min(remaining_string, 2 + (index * 2));
		int triples = min(remaining_string, 3 + (index * 2));

		// How many indexes we have before current index to check offset
		int previous_indexes = 0;
		if (index == 1) previous_indexes = 1;
		if (index == 2) previous_indexes = 2;
		if (index >= 3) previous_indexes = 3;

		// Find matches for singles
		/*
		for (int i = 0; i < singles; i++)
		{
			int current_single = stoi(password.substr(index + i, 1));
			//cout << "current single: " << current_single << endl;

			// For each previous index try and find matches
			for (int j = 1; j <= previous_indexes; j++)
			{
				int current_offset = stoi(password.substr((index+i) - j, j));
				find_printable_ascii_matches(matches, current_single, current_offset);
				//cout << "Matches for " << current_single << " with offset: " << current_offset << " : " << endl;
				//for (unsigned char c : matches)
				//{
				//	cout << (int)c << "(" << c << ")" << ' ';
				//}
				//cout << endl;
			}

			previous_indexes++; // Next single will have 1 more previous index to check
			previous_indexes = min(3, previous_indexes); // Can't be more than 3
		}*/

		// Reset previous_indexes
		if (index == 1) previous_indexes = 1;
		if (index == 2) previous_indexes = 2;
		if (index >= 3) previous_indexes = 3;

		// Find matches for doubles
		for (int i = 0; i < doubles; i++)
		{
			int current_double = stoi(password.substr((index + i), 2));
			//cout << "_______________________________ " << endl;
			//cout << "Current y: " << y_count << endl;
			//cout << "current double: " << current_double << endl;

			// For each previous index try and find matches
			for (int j = 1; j <= previous_indexes; j++)
			{
				int current_offset = stoi(password.substr((index + i) - j, j));
				find_printable_ascii_matches(matches, current_double, current_offset);
				//cout << "Matches for " << current_double << " with offset: " << current_offset << " : " << endl;
				//for (unsigned char c : matches)
				//{
				//	cout << (int)c << "(" << c << ")" << ' ';
				//}
				//cout << endl;
			}
			previous_indexes++; // Next double will have 1 more previous index to check
			previous_indexes = min(3, previous_indexes); // Can't be more than 3
		}

		// Reset previous_indexes
		if (index == 1) previous_indexes = 1;
		if (index == 2) previous_indexes = 2;
		if (index >= 3) previous_indexes = 3;

		// Find matches for triples
		for (int i = 0; i < triples; i++)
		{
			int current_triple = stoi(password.substr((index + i), 3));
			//cout << "current triple: " << current_triple << endl;

			// For each previous index try and find matches
			for (int j = 1; j <= previous_indexes; j++)
			{
				int current_offset = stoi(password.substr((index + i) - j, j));
				find_printable_ascii_matches(matches, current_triple, current_offset);
				//cout << "Matches for " << current_triple << " with offset: " << current_offset << " : " << endl;
				//for (unsigned char c : matches)
				//{
				//	cout << (int)c << "(" << c << ")" << ' ';
				//}
				//cout << endl;

			}
			previous_indexes++; // Next double will have 1 more previous index to check
			previous_indexes = min(3, previous_indexes); // Can't be more than 3
		}

		// All done with this index
		// Insert all found matches for this index
		for (unsigned char c : matches)
		{
			possibilities[y_count][x_count] = c;
			x_count++;
		}
		// Clear matches
		matches.clear();
		y_count++; // Move to next character
		index++;   // Move to next index
		x_count = 0; // Reset x_count 
	}


	possibilities[0][0] = 'A';

	// Print all possibilities
	for (int i = 0; i < y_count; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			if ((int)possibilities[i][j] != 204)
				cout << possibilities[i][j] << ' ';
		}
		cout << endl;
	}


	vector<string> input;
	int col = 0;
	int rows = 0;

	// Get rows
	while (possibilities[rows][0] != 204)
		rows++;

	string row_str = "";

	for (int i = 0; i < rows; i++)
	{
		while (possibilities[i][col] != 204)
		{
			row_str += possibilities[i][col];
			col++;
		}

		input.push_back(row_str);
		row_str = "";
		col = 0;
	}


	//print_possibilities(input);
}

/* !EXPERIMENTAL! */
/* Print every possible match for every single letter position in sentence */
void print_possibilities(vector<string> input) {

	ofstream file("possibilities.txt");
	if (file.is_open())
	{
		// gather all combinations
		vector<string> output;
		// first row
		//cout << "len : 1" << endl;

		const string row0 = input.front();
		for (char c : row0) {
			output.emplace_back(1, c);
			file << c << '\n';
		}
		// other rows
		for (size_t i = 1, n = input.size(); i < n; ++i) {
			//cout << "len : " << i + 1 << endl;
			const string rowI = input[i];
			const vector<string> outputPrev = move(output);
			output.clear();
			for (const string textPrev : outputPrev) {
				for (char c : rowI) {
					output.emplace_back(textPrev + c);
					//file << output.back() << '\n';
					file << textPrev + c << '\n';
				}
			}
		}
	}
}

/* !EXPERIMENTAL! */
/* Find every possible character match for given steps and offset */
void find_printable_ascii_matches(vector<unsigned char>& matches, int steps, int offset)
{
	ifstream file("extended_ascii.txt");

	if (file.is_open())
	{
		string line;
		while (getline(file, line))
		{
			string val = line.substr(0, line.find(" : "));
			string collatz_steps = line.substr(line.find(":") + 2, line.length());

			if (steps == stoi(collatz_steps))	// Found match
				if (find(matches.begin(), matches.end(), stoi(val) - offset) == matches.end()) // Don't insert duplicates
				{
					if ((stoi(val) - offset != 120))			// Exclude 'x'
						if ((stoi(val) - offset != 122))			// Exclude 'z'
							if ((stoi(val) - offset != 113))			// Exclude 'q'

								if ( // Only allow letters and space character
									((stoi(val) - offset) == 32) ||									// Space character
									//((stoi(val) - offset) >= 65 && (stoi(val) - offset) <= 90) ||   // A - Z
									((stoi(val) - offset) >= 97 && (stoi(val) - offset) <= 122))	// a - z
									matches.push_back(stoi(val) - offset);
				}
		}
	}

}