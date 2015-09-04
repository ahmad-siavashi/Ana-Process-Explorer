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
#include <Windows.h>
#include <conio.h>
#include <strsafe.h>
#include "AnaMainHeader.h"
#include "ProcessTools.h"
#include "ThreadTools.h"
#include "StaticParts.h"
#include "DynamicParts.h"
#include "Graph.h"
#include "Core.h"

/*	Functions' declarations.	*/
BOOL CheckConsoleSize(VOID);

/*	Checks whether there is enough space to show a column or not.	*/
VOID CheckSectionsVisibility(VOID){
	static INT i,j;
	(((i=WindowSize.X - Tab.Processes.ProcessCompanyNameList_StartPos.X - RIGHT_MARGIN) > (j=PROCESS_COMPANY_NAME_LABEL_LEN)))
		? (Tab.Processes.ProcessCompanyNameColumn_Visibile = TRUE)
		: (Tab.Processes.ProcessCompanyNameColumn_Visibile = FALSE);
	(((i=Tab.Processes.ProcessDescriptionList_StartPos.X + MAX_PROCESS_DESCRIPTION_LEN) < (j=WindowSize.X - RIGHT_MARGIN)))
		? (Tab.Processes.ProcessDescriptionColumn_Visibile = TRUE)
		: (Tab.Processes.ProcessDescriptionColumn_Visibile = FALSE);
}

/*	Does the suitable action according to the selected menu item.	*/
VOID PerformCurrentTabMenuCommand(VOID){
	switch(Selected.CurrentTab){
	case GRAPH:
		switch(Selected.PopUpMenu.Graph.Current){
		case CPU_USAGE_ITEM:
			Selected.CurrentGraph = CPU_USAGE_ITEM;
			EvacuateGraphHistory();
			CloseCurrentPopUpMenu();
			break;
		case COMMIT_CHARGE_ITEM:
			Selected.CurrentGraph = COMMIT_CHARGE_ITEM;
			EvacuateGraphHistory();
			CloseCurrentPopUpMenu();
			break;
		case PHYSICAL_USAGE_ITEM:
			Selected.CurrentGraph = PHYSICAL_USAGE_ITEM;
			EvacuateGraphHistory();
			CloseCurrentPopUpMenu();
			break;
		}
		break;
	case GENERAL:
		if(!InSetPriorityMenu){
			switch(Selected.PopUpMenu.General.Current){
			case KILL_PROCESS:
				EndProcess(PeInfo[Selected.Process.Current].PID);
				CloseCurrentPopUpMenu();
				break;
			case KILL_PROCESS_TREE:
				KillProcessTree(PeInfo[Selected.Process.Current].PID);
				CloseCurrentPopUpMenu();
				break;
			case SET_PROCESS_PRIORITY:
				CloseCurrentPopUpMenu();
				InSetPriorityMenu=TRUE;
				DrawCurrentTabPopUpMenu(TRUE);
				break;
			}
		}
		else if(InSetPriorityMenu){
			enum Options { IDLE , BELOW_NORMAL , NORMAL , ABOVE_NORMAL , HIGH , REAL_TIME };
			switch(Selected.PopUpMenu.SetPriority.Process.Current){
			case IDLE:
				SetProcessPriority(PeInfo[Selected.Process.Current].PID,IDLE_PRIORITY_CLASS);
				CloseCurrentPopUpMenu();
				break;
			case BELOW_NORMAL:
				SetProcessPriority(PeInfo[Selected.Process.Current].PID,BELOW_NORMAL_PRIORITY_CLASS);
				CloseCurrentPopUpMenu();
				break;
			case NORMAL:
				SetProcessPriority(PeInfo[Selected.Process.Current].PID,NORMAL_PRIORITY_CLASS);
				CloseCurrentPopUpMenu();
				break;
			case ABOVE_NORMAL:
				SetProcessPriority(PeInfo[Selected.Process.Current].PID,ABOVE_NORMAL_PRIORITY_CLASS);
				CloseCurrentPopUpMenu();
				break;
			case HIGH:
				SetProcessPriority(PeInfo[Selected.Process.Current].PID,HIGH_PRIORITY_CLASS);
				CloseCurrentPopUpMenu();
				break;
			case REAL_TIME:
				SetProcessPriority(PeInfo[Selected.Process.Current].PID,REALTIME_PRIORITY_CLASS);
				CloseCurrentPopUpMenu();
				break;
			}
		}
		break;
	case THREADS:
		if(!InSetPriorityMenu){
			switch(Selected.PopUpMenu.Threads.Current){
			case RESUME_THREAD:
				ContinueThread(PeInfo[Selected.Process.Current].Threads[Selected.PopUpMenu.Threads.Current].dwTID);
				CloseCurrentPopUpMenu();
				break;
			case SUSPEND_THREAD:
				SleepThread(PeInfo[Selected.Process.Current].Threads[Selected.PopUpMenu.Threads.Current].dwTID);
				CloseCurrentPopUpMenu();
				break;
			case SET_THREAD_PRIORITY:
				CloseCurrentPopUpMenu();
				InSetPriorityMenu=TRUE;
				DrawCurrentTabPopUpMenu(TRUE);
				break;
			}
		}
		else if(InSetPriorityMenu){
			enum Options { LOWEST , BELOW_NORMAL , NORMAL , ABOVE_NORMAL , HIGHEST , TIME_CRITICAL };
			switch(Selected.PopUpMenu.SetPriority.Thread.Current){
			case LOWEST:
				ThreadPriority(PeInfo[Selected.Process.Current].Threads[Selected.Thread.Current].dwTID,THREAD_PRIORITY_LOWEST);
				CloseCurrentPopUpMenu();
				break;
			case BELOW_NORMAL:
				ThreadPriority(PeInfo[Selected.Process.Current].Threads[Selected.Thread.Current].dwTID,THREAD_PRIORITY_BELOW_NORMAL);
				CloseCurrentPopUpMenu();
				break;
			case NORMAL:
				ThreadPriority(PeInfo[Selected.Process.Current].Threads[Selected.Thread.Current].dwTID,THREAD_PRIORITY_NORMAL);
				CloseCurrentPopUpMenu();
				break;
			case ABOVE_NORMAL:
				ThreadPriority(PeInfo[Selected.Process.Current].Threads[Selected.Thread.Current].dwTID,THREAD_PRIORITY_ABOVE_NORMAL);
				CloseCurrentPopUpMenu();
				break;
			case HIGHEST:
				ThreadPriority(PeInfo[Selected.Process.Current].Threads[Selected.Thread.Current].dwTID,THREAD_PRIORITY_HIGHEST);
				CloseCurrentPopUpMenu();
				break;
			case TIME_CRITICAL:
				ThreadPriority(PeInfo[Selected.Process.Current].Threads[Selected.Thread.Current].dwTID,THREAD_PRIORITY_TIME_CRITICAL);
				CloseCurrentPopUpMenu();
				break;
			}
		}
		break;
	}
}

/*	Refreshes everything.	*/
VOID Refresh(VOID){
	//	Loads All Information.
	LoadAllInformation(DECIDE);
	//	Watching console size changes.
	CheckConsoleSize();
		/* This function starts working on console.	*/
	WaitForSingleObject(hScreenMutex,INFINITE);
	//	Fills current tab information.
	DrawCurrentTab(TRUE);
	//	Fills Status bar and System Up time information.
	FillSedentaryInfo();
	//	Draw graph.
	switch(Selected.CurrentTab){
	case GRAPH:
		switch(Selected.CurrentGraph){
		case CPU_USAGE_ITEM:
			DrawGraph(FIRST_COLUMN_FROM_ABOVE,FIRST_COLUMN_FROM_ABOVE + MAX_POSSIBLE_LINES,LEFT_MARGIN,WindowSize.X - RIGHT_MARGIN - 1,(SIZE_T)SystemCPU_UsageAndUpTimeInfo.CPU_Usage,0);
			break;
		case COMMIT_CHARGE_ITEM:
			DrawGraph(FIRST_COLUMN_FROM_ABOVE,FIRST_COLUMN_FROM_ABOVE + MAX_POSSIBLE_LINES,LEFT_MARGIN,WindowSize.X - RIGHT_MARGIN - 1,100*PerInfo.CommitTotal/PerInfo.CommitLimit,1);
			break;
		case PHYSICAL_USAGE_ITEM:
			DrawGraph(FIRST_COLUMN_FROM_ABOVE,FIRST_COLUMN_FROM_ABOVE + MAX_POSSIBLE_LINES,LEFT_MARGIN,WindowSize.X - RIGHT_MARGIN - 1,SysMemory.dwMemoryLoad,2);
			break;
		}
		break;
	case DETAILS:
		(Selected.Process.Current > 1)?(PeInfo[Selected.Process.Current].CPU_Usage = ProcessCPUTime(PeInfo[Selected.Process.Current].ProcessName)):(PeInfo[Selected.Process.Current].CPU_Usage = 0);
		DrawGraph(FIRST_COLUMN_FROM_ABOVE,FIRST_COLUMN_FROM_ABOVE + MAX_POSSIBLE_LINES, MID_X + LEFT_MARGIN + 1,WindowSize.X - RIGHT_MARGIN - 1,PeInfo[Selected.Process.Current].CPU_Usage,2);
		break;
	}
	ReleaseMutex(hScreenMutex);
	//	If a menu was open redraw it.
	if(InMenu)
		DrawCurrentTabPopUpMenu(TRUE);
}
/*	ReDraw, Actually ReCall all functions that write or draw something
*	Which is dependent to console size.	*/
VOID ReDrawAll(VOID){
	//	Checks whether there is enough space to show a column or not.
	CheckSectionsVisibility();
	WaitForSingleObject(hScreenMutex,INFINITE);
	//	Clearing all the screen.
	system("cls");
	DrawStaticBordersAndStyles();
	//	Writes current tab labels.
	DrawCurrentTab(FALSE);
	if(InMenu)
		//	Writes current tab PopUp Menu frame and labels.
		DrawCurrentTabPopUpMenu(FALSE);
	ReleaseMutex(hScreenMutex);
}

/*	Watches console size changes by the user.	*/
BOOL CheckConsoleSize(VOID){
	//	the difference between actual size and the size which the following function returns.
	enum Difference { DIFF = 3 };
	//	obtaining size information.
	CONSOLE_SCREEN_BUFFER_INFO ConBuffInfo;
	GetConsoleScreenBufferInfo(hConsole,&ConBuffInfo);
	if((WindowSize.X != ConBuffInfo.srWindow.Right - ConBuffInfo.srWindow.Left + DIFF || WindowSize.Y != ConBuffInfo.srWindow.Bottom - ConBuffInfo.srWindow.Top + DIFF)){
		WindowSize.X = ConBuffInfo.srWindow.Right - ConBuffInfo.srWindow.Left + DIFF;
		WindowSize.Y = ConBuffInfo.srWindow.Bottom - ConBuffInfo.srWindow.Top + DIFF;
		(WindowSize.X < MIN_WINDOW_X_SIZE)?(WindowSize.X = MIN_WINDOW_X_SIZE):(FALSE);
		(WindowSize.Y < MIN_WINDOW_Y_SIZE)?(WindowSize.Y = MIN_WINDOW_Y_SIZE):(FALSE);
		//	Redrawing a new boarder and style.
		ReDrawAll();
		//	If the screen size changes.
		return TRUE;
	}
	//	If the screen size was the same.
	return FALSE;
}

/*	Gets keyboard keys and perfoms the suitable action.	*/
BOOL GetKey(VOID){
	INT Key;
	enum keys { UP_ARROW = 72, LEFT_ARROW = 75, RIGHT_ARROW = 77, DOWN_ARROW = 80, ENTER = 13, ESC = 27 };
	while (TRUE){
		Key=getch();
		if(Key==224){
			Key=getch();
			switch(Key){
			case UP_ARROW:
				Scroll(UP);
				break;
			case DOWN_ARROW:
				Scroll(DOWN);
				break;
			case LEFT_ARROW:
				Scroll(LEFT);
				break;
			case RIGHT_ARROW:
				Scroll(RIGHT);
				break;
			}
		}else{
			switch(Key){
			case ENTER:
				if(!InMenu){
					DrawCurrentTabPopUpMenu(TRUE);
				}else
					PerformCurrentTabMenuCommand();
				break;
			case ESC:
				if(InMenu){
					//	Closing current open menu.
					if(!InSetPriorityMenu)	CloseCurrentPopUpMenu();
					if(InSetPriorityMenu){
						CloseCurrentPopUpMenu();
						InSetPriorityMenu=FALSE;
						DrawCurrentTabPopUpMenu(TRUE);
					}
				}
				break;
			}
		}
	}
}