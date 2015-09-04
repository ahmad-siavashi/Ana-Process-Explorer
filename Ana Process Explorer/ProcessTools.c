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
#include <strsafe.h>
#include <pdh.h>
#include <pdhmsg.h>
#pragma comment(lib, "pdh.lib")
#pragma comment(lib,"version.lib")

BOOL CPU_RunOnce=TRUE;

/* Terminates the indicated process.	*/
BOOL EndProcess(DWORD pid){
	DWORD lpExitCode;
	// Opens a handle to the given process.
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
	// Gets the Exit code of the process.
	GetExitCodeProcess(hProcess,&lpExitCode);
	// Terminates it.
	return TerminateProcess(hProcess,lpExitCode);
}

/* Kills Process Tree. ( Recursive Algorithm )	*/
VOID KillProcessTree(DWORD PID){
	DWORD i;
	//	Searching processes.
	for(i=0;i<PerInfo.ProcessCount;++i)
		//	If the process had a child, then kill its children.
		if(PID == PeInfo[i].ParentPID)
			KillProcessTree(PeInfo[i].PID);
	//	If the process doesn't have any child, terminates it.
	EndProcess(PID);
}

/* Changes the priority class of the indicated process.	*/
BOOL SetProcessPriority(DWORD PID,INT NewClass){
	//	Opens a handle to the process.
	HANDLE hProcess = OpenProcess(PROCESS_SET_INFORMATION,FALSE,PID);
	//	If the function succeeds, the return value is nonzero, otherwise it is zero.
	return SetPriorityClass(hProcess,NewClass);
}

/*	Retrieves Process CPU Usage.	*/
DWORD ProcessCPUTime(WCHAR * Instance){
	static PDH_STATUS				status;
	static PDH_FMT_COUNTERVALUE		value;
	static HQUERY					query;
	static HCOUNTER					counter;
	static DWORD					ret;
	DOUBLE							cput=0;
	WCHAR							Buffer[100];
	CHAR							Temp[100]= "\\Process(";
	CHAR							Temp2[100]=")\\% Processor Time";
	INT i,j;
	for(i=0;i<strlen(Temp);++i)
		Buffer[i] = Temp[i];
	for(j=0;Instance[j] != '.';++i,++j)
		Buffer[i] = Instance[j];
	for(j=0;Temp2[j]!='\0';++i,++j)
		Buffer[i] = Temp2[j];
	Buffer[i] = '\0';
	if(CPU_RunOnce){
		status = PdhOpenQuery(NULL, 0, &query);
		if(status != ERROR_SUCCESS)	return;
		PdhAddCounter(query, Buffer, 0, &counter);
		CPU_RunOnce=FALSE;
		PdhCollectQueryData(query); // No error checking here
		return;
	}
	status = PdhCollectQueryData(query);
	if(status != ERROR_SUCCESS)	return;
	status = PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE | PDH_FMT_NOCAP100 ,&ret, &value);
	if(status != ERROR_SUCCESS)	return;
	cput = value.doubleValue/2;
	return (DWORD) cput;
}

/*	Chaging Process WorkingSet Size.	*/
INT ProcessWorkingSetSizeSpace(DWORD PID,SIZE_T MinWS,SIZE_T MaxWS){
	HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA,FALSE,PID);
	SetProcessWorkingSetSize(hProcess,MinWS,MaxWS);
	CloseHandle(hProcess);
	return TRUE;
}

/*	Gets a file's information.	*/
BOOL GetFileInfo(INT NumberOfProcess){
	DWORD	Size=0,dwHandle=0;
	VOID	* Data=NULL;
	VOID	* Buffer=NULL;
	UINT	Len=0;
	UINT	cbTranslate=0;
	WCHAR	SubBlock[MAX_PATH];
	BOOL	VarQueryValueResult = FALSE;
	struct LANGANDCODEPAGE {
		WORD wLanguage;
		WORD wCodePage;
	} *lpTranslate;
	HRESULT hr;
	Size=GetFileVersionInfoSize(PeInfo[NumberOfProcess].ProcessPath,&dwHandle);
	Data = malloc(Size);
	GetFileVersionInfo(PeInfo[NumberOfProcess].ProcessPath,dwHandle,Size,Data);
	VarQueryValueResult = VerQueryValue(Data,TEXT("\\VarFileInfo\\Translation"),(LPVOID*)&lpTranslate,&cbTranslate);
	if(VarQueryValueResult != FALSE){
		//	Retrieving Company Name.
			hr = StringCchPrintf(SubBlock,50,TEXT("\\StringFileInfo\\%04x%04x\\CompanyName"),lpTranslate[0].wLanguage,lpTranslate[0].wCodePage);
			if(!FAILED(hr) && Size > 0 && VerQueryValue(Data,SubBlock,&Buffer,&Len))
				wcscpy(PeInfo[NumberOfProcess].ProcessCompanyName,(WCHAR * )Buffer);
		//	Retrieving Description.
			hr = StringCchPrintf(SubBlock,50,TEXT("\\StringFileInfo\\%04x%04x\\FileDescription"),lpTranslate[0].wLanguage,lpTranslate[0].wCodePage);
			if(!FAILED(hr) && Size > 0 && VerQueryValue(Data,SubBlock,&Buffer,&Len))
				wcscpy(PeInfo[NumberOfProcess].ProcessDescription,(WCHAR * )Buffer);
		//	Retrieving FileVersion.
			hr = StringCchPrintf(SubBlock,50,TEXT("\\StringFileInfo\\%04x%04x\\FileVersion"),lpTranslate[0].wLanguage,lpTranslate[0].wCodePage);
			if(!FAILED(hr) && Size > 0 && VerQueryValue(Data,SubBlock,&Buffer,&Len))
				wcscpy(PeInfo[NumberOfProcess].ProcessFileVersion,(WCHAR * )Buffer);
		//	Retrieving InternalName.
			hr = StringCchPrintf(SubBlock,50,TEXT("\\StringFileInfo\\%04x%04x\\InternalName"),lpTranslate[0].wLanguage,lpTranslate[0].wCodePage);
			if(!FAILED(hr) && Size > 0 && VerQueryValue(Data,SubBlock,&Buffer,&Len))
				wcscpy(PeInfo[NumberOfProcess].ProcessInternalName,(WCHAR * )Buffer);
		//	Retrieving LegalCopyright.
			hr = StringCchPrintf(SubBlock,50,TEXT("\\StringFileInfo\\%04x%04x\\LegalCopyright"),lpTranslate[0].wLanguage,lpTranslate[0].wCodePage);
			if(!FAILED(hr) && Size > 0 && VerQueryValue(Data,SubBlock,&Buffer,&Len))
				wcscpy(PeInfo[NumberOfProcess].ProcessLegalCopyright,(WCHAR * )Buffer);
		//	Retrieving LegalTradeMarks.
			hr = StringCchPrintf(SubBlock,50,TEXT("\\StringFileInfo\\%04x%04x\\LegalTrademarks"),lpTranslate[0].wLanguage,lpTranslate[0].wCodePage);
			if(!FAILED(hr) && Size > 0 && VerQueryValue(Data,SubBlock,&Buffer,&Len))
				wcscpy(PeInfo[NumberOfProcess].ProcessLegalTrademarks,(WCHAR * )Buffer);
		//	Retrieving OriginalFilename.
			hr = StringCchPrintf(SubBlock,50,TEXT("\\StringFileInfo\\%04x%04x\\OriginalFilename"),lpTranslate[0].wLanguage,lpTranslate[0].wCodePage);
			if(!FAILED(hr) && Size > 0 && VerQueryValue(Data,SubBlock,&Buffer,&Len))
				wcscpy(PeInfo[NumberOfProcess].ProcessOriginalFilename,(WCHAR * )Buffer);
		//	Retrieving ProductName.
			hr = StringCchPrintf(SubBlock,50,TEXT("\\StringFileInfo\\%04x%04x\\ProductName"),lpTranslate[0].wLanguage,lpTranslate[0].wCodePage);
			if(!FAILED(hr) && Size > 0 && VerQueryValue(Data,SubBlock,&Buffer,&Len))
				wcscpy(PeInfo[NumberOfProcess].ProcessProductName,(WCHAR * )Buffer);
		//	Retrieving ProductVersion.
			hr = StringCchPrintf(SubBlock,50,TEXT("\\StringFileInfo\\%04x%04x\\ProductVersion"),lpTranslate[0].wLanguage,lpTranslate[0].wCodePage);
			if(!FAILED(hr) && Size > 0 && VerQueryValue(Data,SubBlock,&Buffer,&Len))
				wcscpy(PeInfo[NumberOfProcess].ProcessProductVersion,(WCHAR * )Buffer);
		//	Retrieving PrivateBuild.
			hr = StringCchPrintf(SubBlock,50,TEXT("\\StringFileInfo\\%04x%04x\\PrivateBuild"),lpTranslate[0].wLanguage,lpTranslate[0].wCodePage);
			if(!FAILED(hr) && Size > 0 && VerQueryValue(Data,SubBlock,&Buffer,&Len))
				wcscpy(PeInfo[NumberOfProcess].ProcessPrivateBuild,(WCHAR * )Buffer);
		//	Retrieving SpecialBuild.
			hr = StringCchPrintf(SubBlock,50,TEXT("\\StringFileInfo\\%04x%04x\\SpecialBuild"),lpTranslate[0].wLanguage,lpTranslate[0].wCodePage);
			if(!FAILED(hr) && Size > 0 && VerQueryValue(Data,SubBlock,&Buffer,&Len))
				wcscpy(PeInfo[NumberOfProcess].ProcessSpecialBuild,(WCHAR * )Buffer);
		//	Retrieving Comments.
			hr = StringCchPrintf(SubBlock,50,TEXT("\\StringFileInfo\\%04x%04x\\Comments"),lpTranslate[0].wLanguage,lpTranslate[0].wCodePage);
			if(!FAILED(hr) && Size > 0 && VerQueryValue(Data,SubBlock,&Buffer,&Len))
				wcscpy(PeInfo[NumberOfProcess].ProcessComments,(WCHAR * )Buffer);
	}else if(VarQueryValueResult == FALSE) {
		//	If the required data wasn't present.
			wcscpy(PeInfo[NumberOfProcess].ProcessDescription,TEXT("N/A"));
			wcscpy(PeInfo[NumberOfProcess].ProcessCompanyName,TEXT("N/A"));
			wcscpy(PeInfo[NumberOfProcess].ProcessFileVersion,TEXT("N/A"));
			wcscpy(PeInfo[NumberOfProcess].ProcessInternalName,TEXT("N/A"));
			wcscpy(PeInfo[NumberOfProcess].ProcessLegalCopyright,TEXT("N/A"));
			wcscpy(PeInfo[NumberOfProcess].ProcessLegalTrademarks,TEXT("N/A"));
			wcscpy(PeInfo[NumberOfProcess].ProcessOriginalFilename,TEXT("N/A"));
			wcscpy(PeInfo[NumberOfProcess].ProcessProductName,TEXT("N/A"));
			wcscpy(PeInfo[NumberOfProcess].ProcessProductVersion,TEXT("N/A"));
			wcscpy(PeInfo[NumberOfProcess].ProcessPrivateBuild,TEXT("N/A"));
			wcscpy(PeInfo[NumberOfProcess].ProcessSpecialBuild,TEXT("N/A"));
			wcscpy(PeInfo[NumberOfProcess].ProcessComments,TEXT("N/A"));
	}
	if(VarQueryValueResult != FALSE) {
		//	If the required data wasn't present.
			if(!wcscmp(PeInfo[NumberOfProcess].ProcessDescription,TEXT("")))
				wcscpy(PeInfo[NumberOfProcess].ProcessDescription,TEXT("N/A"));
			if(!wcscmp(PeInfo[NumberOfProcess].ProcessCompanyName,TEXT("")))
				wcscpy(PeInfo[NumberOfProcess].ProcessCompanyName,TEXT("N/A"));
			if(!wcscmp(PeInfo[NumberOfProcess].ProcessFileVersion,TEXT("")))
				wcscpy(PeInfo[NumberOfProcess].ProcessFileVersion,TEXT("N/A"));
			if(!wcscmp(PeInfo[NumberOfProcess].ProcessInternalName,TEXT("")))
				wcscpy(PeInfo[NumberOfProcess].ProcessInternalName,TEXT("N/A"));
			if(!wcscmp(PeInfo[NumberOfProcess].ProcessLegalCopyright,TEXT("")))
				wcscpy(PeInfo[NumberOfProcess].ProcessLegalCopyright,TEXT("N/A"));
			if(!wcscmp(PeInfo[NumberOfProcess].ProcessLegalTrademarks,TEXT("")))
				wcscpy(PeInfo[NumberOfProcess].ProcessLegalTrademarks,TEXT("N/A"));
			if(!wcscmp(PeInfo[NumberOfProcess].ProcessOriginalFilename,TEXT("")))
				wcscpy(PeInfo[NumberOfProcess].ProcessOriginalFilename,TEXT("N/A"));
			if(!wcscmp(PeInfo[NumberOfProcess].ProcessProductName,TEXT("")))
				wcscpy(PeInfo[NumberOfProcess].ProcessProductName,TEXT("N/A"));
			if(!wcscmp(PeInfo[NumberOfProcess].ProcessProductVersion,TEXT("")))
				wcscpy(PeInfo[NumberOfProcess].ProcessProductVersion,TEXT("N/A"));
			if(!wcscmp(PeInfo[NumberOfProcess].ProcessPrivateBuild,TEXT("")))
				wcscpy(PeInfo[NumberOfProcess].ProcessPrivateBuild,TEXT("N/A"));
			if(!wcscmp(PeInfo[NumberOfProcess].ProcessSpecialBuild,TEXT("")))
				wcscpy(PeInfo[NumberOfProcess].ProcessSpecialBuild,TEXT("N/A"));
			if(!wcscmp(PeInfo[NumberOfProcess].ProcessComments,TEXT("")))
				wcscpy(PeInfo[NumberOfProcess].ProcessComments,TEXT("N/A"));
	}
	free(Data);
	return VarQueryValueResult;
}