#include "UserInterface.h"


int UserInterface::select_menu_option()
{
	printf("Please select one of the options: \n"
		"(1) Create username and password \n"
		"(2) Check username and password \n"
		"(3) Generate password strength analysis file \n"
		"(4) Analyse password strength analysis file \n"
		"(5) Exit \n");
	
	printf("> ");
	
	int selection;
	cin >> selection;

	if (selection >= 1 && selection <= 5)
	{
		if (selection == 5)
			printf("Exiting...");
		else
			printf("Your selection: %d \n", selection);

		return selection;
	}

	std::cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	printf(" ! Please select valid option !\n");
	select_menu_option(); // recursion
}

string UserInterface::ask_for_username()
{
	printf("Insert your username: \n");
	printf("> ");

	string username = "";
	cin >> username;

	return username;
}

string UserInterface::ask_for_password()
{
	printf("Insert your password: \n");
	printf("> ");

	string password;
	cin >> password;

	for (unsigned char c : password)
	{
		if ((int)c < 32 || (int)c > 127)
		{
			printf(" ! Invalid password inserted !\n");
			ask_for_password();
		}
	}

	return password;
}