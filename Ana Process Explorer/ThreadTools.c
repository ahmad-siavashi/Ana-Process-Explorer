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

/*	Suspends a thread.	*/
INT SleepThread(INT ThreadID){
	//	Opens an existing thread object.
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS,FALSE,ThreadID);
	//	Suspends the thread.
	SuspendThread(hThread);
	//	Closes the handle.
	CloseHandle(hThread);
	//	Everything was fine.
	return TRUE;
}

/*	Resumes a suspended thread.	*/
INT ContinueThread(INT ThreadID){
	//	Opens an existing thread object.
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS,FALSE,ThreadID);
	//	Resumes the Thread.
	ResumeThread(hThread);
	//	Closes the handle.
	CloseHandle(hThread);
	//	Didn't face any error.
	return TRUE;
}

/*	Changes the priority of a specific thread.	*/
INT ThreadPriority(INT ThreadID,INT PriorityClass){
	//	Opens an existing thread object.
	HANDLE hThread = OpenThread(THREAD_SET_INFORMATION,FALSE,ThreadID);
	//	Changes Thread Priority.
	SetThreadPriority(hThread,PriorityClass);
	//	Closes the handle.
	CloseHandle(hThread);
	//	There was no error.
	return TRUE;
}