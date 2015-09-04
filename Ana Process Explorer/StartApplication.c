/*	Project Name:	Ana Process Explorer
*	Written By	:	Ahmad			Siavashi 	->	Email: a.siavosh@yahoo.com,
*					Ali				Kianinejad	->	Email: af.kianinejad@gmail.com,
*				 	Farid			Amiri,
*				 	Mohammad Javad	Moein.
*	Course Title:	Principles of Programming.
*	Instructor	: 	Dr. Ali Hamze.
*	T.A			: 	Mr. Hojat Doulabi.
*	Shiraz University, Shiraz, Iran.
*	Spring 1391, 2012.
*/
#include "AnaMainHeader.h"
#include "DynamicParts.h"
#include "Core.h"
#include "StaticParts.h"
#include "Tools.h"

//	Creating an object for multithreading purposes.
HANDLE hScreenMutex;

/* Sets Console Attributes at the beginning og the program.	*/
VOID InitializeConsole(VOID){
	//	A struture that holds console cursor features.
	CONSOLE_CURSOR_INFO ConsoleCursorInfo;
	//	Structures which are necessary for changing console's background color.
	COORD Pos = {0,0};
	//	For multithreading purposes.
	hScreenMutex = CreateMutex(NULL,FALSE,NULL);
	//	Opening a handle to the console.
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//	Hiding console's cursor by changing its attributes.
	ConsoleCursorInfo.bVisible = FALSE;
	ConsoleCursorInfo.dwSize = 100;
	SetConsoleCursorInfo(hConsole,&ConsoleCursorInfo);
	//	Setting Console's title.
	SetConsoleTitle(CONSOLE_TITLE);
	//	Retrieving Largest possible console screen.
	WindowSize = GetLargestConsoleWindowSize(hConsole);
	//	Changing Console screen buffer size.
	SetConsoleScreenBufferSize(hConsole,WindowSize);
	//	Maximizing console window.
	ShowWindow(GetConsoleWindow(),SW_MAXIMIZE);
	//	Changing console's background color.
	system(CONSOLE_INITIALIZING_COLOR);
	//	Evaluate max possible lines to be written in console screen.
}

/*	Runs the program.	*/
BOOL StartApplication(VOID){
	//	Initializing Console features.
	InitializeConsole();
	//	Loads All Information.
	LoadAllInformation(ALL);
	//	Drawing static parts of the console program.
	DrawStaticBordersAndStyles();
	//	Fills Status bar and System Up time information.
	FillSedentaryInfo();
	//	Opens the current tab.
	DrawCurrentTab(TRUE);
	// Opens a thread to run GetKey function which watches keyboard signals.
	_beginthread(GetKey);
	//	Watching screen size changes to redraw the console.
	while(TRUE){
		//	Refreshes everything.
		Refresh();
		//	Waits for the next round.
		Sleep(1000);
	}
	return TRUE;
}