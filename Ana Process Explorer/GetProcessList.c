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
#include "ProcessTools.h"
#include <strsafe.h>

//	Holds number of counted processes.
INT CoreProcessCount;

/*	Lists running processes plus
*	Some information about them. */
BOOL GetProcessList(VOID){
	//	Initializing the number of processes variable.
	INT NumberOfProcess=0;
	//	Creating a structure to sustain each process information.
	PROCESSENTRY32 pe32;
	//	Contains the memory statistics for a process.
	PROCESS_MEMORY_COUNTERS	pmc32;
	// Creating a snapshot of all running processes in the system.
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	HANDLE hProcess;
	// If taking the snapshot was unsuccessful, returns FALSE.
	if(hProcessSnap==INVALID_HANDLE_VALUE)
		return FALSE;
	// Setting the size of the stuctute before using it.
	pe32.dwSize=sizeof(PROCESSENTRY32);
	//	Checking availability of processes in the snapshot.
	if(!Process32First(hProcessSnap,&pe32))
		// failure of taking snapshot or there is no process in the list.
		return FALSE;
	// Obtaining the information of each process.
	do{
		// Opens a handle to the current process.
		hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pe32.th32ProcessID);
		//	Retrieving memory infromation about the process.
		GetProcessMemoryInfo(hProcess,&pmc32,sizeof(pmc32));
		//	Assigning Obtained information about each process to its specific structure.
			//	Process Name.
			wcscpy(PeInfo[NumberOfProcess].ProcessName,pe32.szExeFile);
			//	Process Identifier.
			PeInfo[NumberOfProcess].PID = pe32.th32ProcessID;
			//	Retrieves the path of the process.
			GetModuleFileNameEx(hProcess,NULL,PeInfo[NumberOfProcess].ProcessPath,sizeof(PeInfo[NumberOfProcess].ProcessPath));
			if(!wcscmp(TEXT(""),PeInfo[NumberOfProcess].ProcessPath))
				wcscpy(PeInfo[NumberOfProcess].ProcessPath,TEXT("N/A"));
			//	ParentID of the process.
			PeInfo[NumberOfProcess].ParentPID = pe32.th32ParentProcessID;
				/*	Memory Information of the selected process.	*/
			if(NumberOfProcess > 1){
				PeInfo[NumberOfProcess].PageFaultCount = pmc32.PageFaultCount;
				PeInfo[NumberOfProcess].PagefileUsage = pmc32.PagefileUsage;
				PeInfo[NumberOfProcess].PeakPagefileUsage = pmc32.PeakPagefileUsage;
				PeInfo[NumberOfProcess].PeakWorkingSetSize = pmc32.PeakWorkingSetSize;
				PeInfo[NumberOfProcess].QuotaNonPagedPoolUsage = pmc32.QuotaNonPagedPoolUsage;
				PeInfo[NumberOfProcess].QuotaPagedPoolUsage = pmc32.QuotaPagedPoolUsage;
				PeInfo[NumberOfProcess].QuotaPeakNonPagedPoolUsage = pmc32.QuotaPeakNonPagedPoolUsage;
				PeInfo[NumberOfProcess].QuotaPeakPagedPoolUsage = pmc32.QuotaPeakPagedPoolUsage;
				//	Obtainig Working Set Size.
				PeInfo[NumberOfProcess].WorkingSetSize = pmc32.WorkingSetSize;
			}
			//	Priority Base of process's threads.
			PeInfo[NumberOfProcess].Priority = GetPriorityClass(hProcess);
			//	Retrieving Process Cycle.
			QueryProcessCycleTime(hProcess,&PeInfo[NumberOfProcess].CycleTime);
			//	Retrieving Process String information.
			GetFileInfo(NumberOfProcess);
		// Closing a handle which was refering to an especific process in the snapshot.
		CloseHandle(hProcess);
	//	Next Process.
	++NumberOfProcess;
	}while(Process32Next(hProcessSnap,&pe32));
	// Keeping number of processes.
	CoreProcessCount = NumberOfProcess;
	// Closing the handle aftrer finishing work with it.
	CloseHandle(hProcessSnap);
	// If everything went well, returns TRUE as output.
	return TRUE;
}