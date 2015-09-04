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
//	Translates the value returned by the GetSystemInfo function to a constant string.
WCHAR *ProcessorArchitectureTranslate(DWORD wpa);
//	Changes the value returned by the GetPriorityClass function to a meaningfull string.
WCHAR *ProcessPriorityTranslate(DWORD PriorityClass);
//	Changes the value returned by the GetThreadPriority function to a meaningfull string.
WCHAR *ThreadPriorityTranslate(DWORD PriorityClass);