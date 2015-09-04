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
// Core's library.
#include "AnaCoreMainHeader.h"

/*	Lists modules of the running process plus
*	Some information about them. */
INT GetModuleList(INT NumberOfProcess,DWORD dwPID){
	MODULEENTRY32 me32;
	//	Initializing Number Of Modules.
	INT NumberOfModule=0;
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	// Take a snapshot of all modules in the specified process.
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,dwPID);
	// Set the size of the structure before using it.
	me32.dwSize=sizeof(MODULEENTRY32);
	// Retrieve information about the first module and exit if unsuccessful.
	if(!Module32First(hModuleSnap,&me32)){
		//	This means that there is no module in the snapshot.
		PeInfo[NumberOfProcess].ModuleCount = 0;
		// Clean the snapshot object.
		CloseHandle(hModuleSnap);
		return FALSE;
	}
	do{
		//	Retrieves module's name.
		wcscpy(PeInfo[NumberOfProcess].Modules[NumberOfModule].ModuleName,me32.szModule);
		//	Retrieves module's path.
		wcscpy(PeInfo[NumberOfProcess].Modules[NumberOfModule].ModulePath,me32.szExePath);
		//	The Size Of the Module, in bytes.
		PeInfo[NumberOfProcess].Modules[NumberOfModule].ModuleBaseSize = me32.modBaseSize;
		//	Adding NumberOfModules.
		++NumberOfModule;
	}while(Module32Next(hModuleSnap,&me32));
	// Closing (cleaning) the Handle.
	CloseHandle(hModuleSnap);
	//	Sending number of modules of this process.
	PeInfo[NumberOfProcess].ModuleCount = NumberOfModule;
	return TRUE;
}