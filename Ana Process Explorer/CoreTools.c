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
#include "CoreTools.h"

/*	Finds parent of a process.	*/
WCHAR *FindParent(DWORD ParentPID){
	DWORD i;
	for(i=0;i<PerInfo.ProcessCount;++i){
		if(PeInfo[i].PID == ParentPID)
			return PeInfo[i].ProcessName;
	}
	return TEXT("N/A");
}

/*	Arranges the structure according to parentPID
*	It helps so that all processes with the same parent
*	will be listed together.	*/
VOID ArrangeParent(VOID){
	DWORD i,j,k;
	for(i=k=0;i<PerInfo.ProcessCount;i=k){
		for(j=k=i+1;j<PerInfo.ProcessCount;++j){
			if(PeInfo[i].ParentPID == PeInfo[j].ParentPID)
				Swap(k++,j);
		}
	}
}

/*	Intergange the i(th) and the j(th) elements of the structure.	*/
VOID Swap(DWORD i,DWORD j){
	struct tagProcessInfo temp;
	temp = PeInfo[i];
	PeInfo[i]=PeInfo[j];
	PeInfo[j]=temp;
}

/*	Calculate System Up Time.	*/
VOID SystemUpTime(VOID){
    DWORD Time;
	//	Retrieves System up time in milliseconds.
    Time=GetTickCount();
	//	Evaluating system time in days, hours,minutes and seconds.
	SystemCPU_UsageAndUpTimeInfo.Days=Time/(24*60*60*1000),Time-=SystemCPU_UsageAndUpTimeInfo.Days*24*60*60*1000;
	SystemCPU_UsageAndUpTimeInfo.Hours=Time/(1000*60*60),Time-=SystemCPU_UsageAndUpTimeInfo.Hours*60*1000*60;
	SystemCPU_UsageAndUpTimeInfo.Minutes=Time/60000,Time-=SystemCPU_UsageAndUpTimeInfo.Minutes*60000;
	SystemCPU_UsageAndUpTimeInfo.Seconds=Time/1000;
}

/*	Retrieves total CPU usage.	*/
VOID TotalCPU_Usage(VOID){
	ULARGE_INTEGER IdleTime,KernelTime,UserTime;
	static ULONGLONG Idle=0,Kernel=0,User=0,Sys=0;
	static BOOL FirstTime = TRUE;
	//	Retrieving System Times.
	GetSystemTimes((LPFILETIME)&IdleTime.u,(LPFILETIME)&KernelTime.u,(LPFILETIME)&UserTime.u);
	//	Evaluating the difference between the old Idle time and the new one.
		//	And so for all other times.
	Idle	=	IdleTime.QuadPart	-	Idle;
	Kernel	=	KernelTime.QuadPart -	Kernel;
	User	=	UserTime.QuadPart	-	User;
	if(!FirstTime){
		//	Calculating the total system time.
		Sys = Kernel + User ;
		//	Function's Output.
			//	Time that system has been used divided by total time.
		if(Sys)
			SystemCPU_UsageAndUpTimeInfo.CPU_Usage = 100 * (Sys - Idle)/Sys;
	}
	FirstTime = FALSE;
}

/*	Converts Bytes to the given unit.	*/
INT64 ByteTo(INT64 Bytes,INT Unit){
	return (Bytes/(1024*Unit));
}