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
#include "AnaMainHeader.h"
#include "CoreTools.h"
#include "GetProcessList.h"
#include "GetModulesList.h"
#include "GetThreadsList.h"
#include "ProcessTools.h"

/*	Core of the program.	*/
VOID LoadAllInformation(BOOL Parts){
	if(Parts == ALL){
		//	Creating a list of processes and their information.
		GetProcessList();
		//	Retrieves information about modules of the process.
		GetModuleList(Selected.Process.Current,PeInfo[Selected.Process.Current].PID);
		//	Retrieving Information about threads of the process.
		GetThreadsList(Selected.Process.Current,PeInfo[Selected.Process.Current].PID);
	}else{
		//	Creating a list of processes and their information.
		if(Selected.CurrentTab == GENERAL)	GetProcessList();
		//	Retrieves information about modules of the process.
		if(Selected.CurrentTab == MODULES)	GetModuleList(Selected.Process.Current,PeInfo[Selected.Process.Current].PID);
		//	Retrieving Information about threads of the process.
		if(Selected.CurrentTab == THREADS)	GetThreadsList(Selected.Process.Current,PeInfo[Selected.Process.Current].PID);
	}
	//	Retrieving System Up time.
	SystemUpTime();
	// Retrieving System Information.
	GetSystemInfo(&SysInfo);
	//	Retrieving System Perfermance Information.
	GetPerformanceInfo(&PerInfo,sizeof(PerInfo));
	//	Geting and refreshing CPU usage in a seperated thread.
	TotalCPU_Usage();
	//	Retrieving information about memory.
	GlobalMemoryStatus(&SysMemory);
}