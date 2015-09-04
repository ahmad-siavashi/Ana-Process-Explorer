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
//	Finds parent of a process.
WCHAR *FindParent(DWORD ParentPID);
//	Arrange processes to make the process tree simple.
VOID ArrangeParent(VOID);
//	changes the place of two structures in the array of structures.
VOID Swap(DWORD i,DWORD j);
//	Retrieving System Up time.
VOID SystemUpTime(VOID);
//	Retrieves total CPU usage.
VOID TotalCPU_Usage(VOID);
//	Converts Bytes to the given unit.
INT64 ByteTo(INT64 Bytes,INT Unit);