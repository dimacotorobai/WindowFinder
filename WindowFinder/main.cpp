#include "stdafx.h"
#include "typedef.h"
#include "structs.h"
#include "functions.h"
#include "Console.h"


int main(int argc, char* argv[])
{	
	//Create console object
	Console console(TEXT("Window Finder"));
	console.SetupConsole();

	//Main Loop
	while (!console.ShouldQuit()) {
		console.ShowUserInterface();
		console.GetUserInput();
		console.PollEvents();
	}

	//Exit process
	return 0;
}