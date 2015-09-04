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

/*	Lists threads of the running process plus
*	Some information about them. */
BOOL GetThreadsList(INT NumberOfProcess,DWORD dwTID){
	//	Initializing Number of threads.
	INT NumberOfThreads=0;
	THREADENTRY32 th32;
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	HANDLE hThread=INVALID_HANDLE_VALUE;
	//	Taking a sanpshot from threads of the process.
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0);
	//	Setting size of the structure before using it.
	th32.dwSize=sizeof(THREADENTRY32);
	//	obtaining the first process and checking for error.
	if(!Thread32First(hThreadSnap,&th32)){
		//	If there was an error ( because each process has at least one thread ).
		PeInfo[NumberOfProcess].ThreadCount = NumberOfThreads;
		// Clean the snapshot object.
		CloseHandle(hThreadSnap);
		return FALSE;
	}
	do{
		if(th32.th32OwnerProcessID == dwTID){
			//	Opens an existing thread object.
			hThread = OpenThread(THREAD_QUERY_INFORMATION ,FALSE,th32.th32ThreadID);
			//	Obtaining thread PIDs.
			PeInfo[NumberOfProcess].Threads[NumberOfThreads].dwTID = th32.th32ThreadID;
			//	Obtaining Thread Priority.
			PeInfo[NumberOfProcess].Threads[NumberOfThreads].ThreadPriority = GetThreadPriority(hThread);
			//	The number of CPU clock cycles used by the thread. This value includes cycles spent in both user mode and kernel mode.
			QueryThreadCycleTime(hThread,&PeInfo[NumberOfProcess].Threads[NumberOfThreads].CycleTime);
		//	Closes the handle.
		CloseHandle(hThread);
			//	Adding NumberOfThreads.
			++NumberOfThreads;
		}
	}while(Thread32Next(hThreadSnap,&th32));
	//	Closing the handle object.
	CloseHandle(hThreadSnap);
	//	assigning number of threads to the variable of that process.
	PeInfo[NumberOfProcess].ThreadCount = NumberOfThreads;
	return TRUE;
}