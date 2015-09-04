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
/*	Main libraries.	*/
// All over the core, we've used MS defines and APIs.
#include <Windows.h>
// WIN32 tool help functions, types, and definitions.
#include <TlHelp32.h>
// We've used WCHAR type to work with strings all over the core.
#include <wchar.h>
//	Process Status Library.
#include <Psapi.h>
#pragma comment(lib,"psapi.lib")

/*	LoadAllInformation inputs defines.	*/
#define	ALL		1
#define DECIDE	0

/*	Main defines.	*/
//	Maximum length of process name.
#define MAXFILENAME			30
//	Maximun length of process description and company name.
#define MAXDESCLENGHT		50
//	Lenght of the array of tagProcessInfo structure.
#define PROCESSSTRUCTLENGHT	200
//	Lenght of the array of tagModuleInfo structure.
#define MODULESTRUCTLENGHT	50
//	Lenght of the array of tagThreadInfo structure.
#define THREADSTRUCTLENGHT	150

/*	Unit defines.	*/
#define KB	1

/*	Global Structures.	*/
//	A structure which keeps information about a process.
struct tagProcessInfo{
	WCHAR ProcessName[MAXFILENAME];
	WCHAR ProcessPath[MAX_PATH];
	WCHAR ProcessDescription[MAXDESCLENGHT];
	WCHAR ProcessCompanyName[MAXDESCLENGHT];
	WCHAR ProcessFileVersion[MAXDESCLENGHT];
	WCHAR ProcessInternalName[MAXDESCLENGHT];
	WCHAR ProcessLegalCopyright[MAXDESCLENGHT];
	WCHAR ProcessLegalTrademarks[MAXDESCLENGHT];
	WCHAR ProcessOriginalFilename[MAXDESCLENGHT];
	WCHAR ProcessProductName[MAXDESCLENGHT];
	WCHAR ProcessProductVersion[MAXDESCLENGHT];
	WCHAR ProcessPrivateBuild[MAXDESCLENGHT];
	WCHAR ProcessSpecialBuild[MAXDESCLENGHT];
	WCHAR ProcessComments[MAXDESCLENGHT];
	DWORD PID;
	DWORD ParentPID;
	DWORD Priority;
	DWORD CPU_Usage;
	ULONG64 CycleTime;
	DWORD PageFaultCount;
    SIZE_T PeakWorkingSetSize;
    SIZE_T WorkingSetSize;
    SIZE_T QuotaPeakPagedPoolUsage;
    SIZE_T QuotaPagedPoolUsage;
    SIZE_T QuotaPeakNonPagedPoolUsage;
    SIZE_T QuotaNonPagedPoolUsage;
    SIZE_T PagefileUsage;
    SIZE_T PeakPagefileUsage;
	//	Works as a flag, so that if the process was used in the tree, it won't be printed once more.
	BOOL InTree;
	//	From 0 to NumberOfModule.
	INT ModuleCount;
	struct tagModuleInfo{
		WCHAR ModuleName[MAXFILENAME];
		WCHAR ModulePath[MAX_PATH];
		DWORD ModuleBaseSize;
	}Modules[MODULESTRUCTLENGHT];
	//	From 0 to NumberOfThreads.
	INT ThreadCount;
	struct tagThreadsInfo{
		DWORD dwTID;
		INT ThreadPriority;
		ULONG64 CycleTime;
	}Threads[THREADSTRUCTLENGHT];
};
//	Creating an array from tagProcessInfo structure.
struct tagProcessInfo PeInfo[PROCESSSTRUCTLENGHT];
//	Holds number of counted processes.
extern INT CoreProcessCount;
//	For Retrieving CPU Usage of each process.
extern	BOOL	CPU_RunOnce;
//	Creating a structure to hold System information.
SYSTEM_INFO SysInfo;
//	Creating a structure to hold Memory information.
MEMORYSTATUS SysMemory;

/*	System CPU Usage and Up Time structure.	*/
struct tagSystemCPUTimeInfo{
	//	CPU Usage.
	ULONGLONG CPU_Usage;
	//	UpTime.
	INT Days;
	INT Hours;
	INT Minutes;
	INT Seconds;
}SystemCPU_UsageAndUpTimeInfo;

//	Creating a structure to get system perfermence information.
	PERFORMANCE_INFORMATION PerInfo;