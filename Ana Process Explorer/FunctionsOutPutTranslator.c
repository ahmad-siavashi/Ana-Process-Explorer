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

/*	Changes the value returned by the GetPriorityClass function
*	to a meaningfull string.	*/
CONST WCHAR * ProcessPriorityTranslate(DWORD PriorityClass){
	switch(PriorityClass){
	case 0x00000040:
		return TEXT("IDLE");
	case 0x00004000:
		return TEXT("Below Normal");
	case 0x00000020:
		return TEXT("Normal");
	case 0x00008000:
		return TEXT("Above Normal");
	case 0x00000080:
		return TEXT("High");
	case 0x00000100:
		return TEXT("Real Time");
	default:
		return TEXT("Unknown");
	}
}

/*	Changes the value returned by the GetThreadPriority function
*	to a meaningfull string.	*/
WCHAR * ThreadPriorityTranslate(DWORD PriorityClass){
	switch(PriorityClass){
	case THREAD_PRIORITY_IDLE:
		return TEXT("IDLE");
	case THREAD_PRIORITY_BELOW_NORMAL:
		return TEXT("Below Normal");
	case THREAD_PRIORITY_NORMAL:
		return TEXT("Normal");
	case THREAD_PRIORITY_ABOVE_NORMAL:
		return TEXT("Above Normal");
	case THREAD_PRIORITY_HIGHEST:
		return TEXT("Highest");
	case THREAD_PRIORITY_LOWEST:
		return TEXT("Lowest");
	case THREAD_PRIORITY_TIME_CRITICAL:
		return TEXT("Time Critical");
	default:
		return TEXT("Unknown");
	}
}

/*	Translates the value returned by the GetSystemInfo
*	function to a constant string.	*/
WCHAR *ProcessorArchitectureTranslate(DWORD wpa){
	switch(wpa){
	case PROCESSOR_ARCHITECTURE_AMD64:
		return TEXT("X64");
	case PROCESSOR_ARCHITECTURE_IA64:
		return TEXT("Intel Itanium-based");
	case PROCESSOR_ARCHITECTURE_INTEL:
		return TEXT("x86");
	case PROCESSOR_ARCHITECTURE_UNKNOWN:
	default:
		return TEXT("Unknown");
	}
}