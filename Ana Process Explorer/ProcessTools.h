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
//	Terminates the given process.
BOOL EndProcess(DWORD pid);
//	Changes the priority of a specific function.
BOOL SetProcessPriority(DWORD PID,INT NEW_CLASS);
//	Retrieves Process CPU Usage.
DWORD ProcessCPUTime(WCHAR * Instance);
// Kills Process Tree. ( Recursive Algorithm )
VOID KillProcessTree(DWORD PID);
//	Chaging Process WorkingSet Size.
BOOL ProcessWorkingSetSizeSpace(DWORD PID,SIZE_T MinWS,SIZE_T MaxWS);
//	Gets a file's information.
BOOL GetFileInfo(INT NumberOfProcess);