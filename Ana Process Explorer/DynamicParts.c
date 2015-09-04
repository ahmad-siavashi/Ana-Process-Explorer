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
#include "AnaMainHeader.h"
#include "DynamicParts.h"
#include "FunctionsOutPutTranslator.h"
#include "CoreTools.h"
#include "Graph.h"
#include <strsafe.h>

//	Number Of spaces in this define is up to the TabWidth size.
#define LEFT_OPEN_TAB_CHAR	(TEXT("/"))
#define RIGHT_OPEN_TAB_CHAR	(TEXT("\\"))

//	Necessary Structures.
struct tagTabDataColumns Tab = {
		/*	ThreadsTab.	*/
		{TRUE,{LABEL_X_POS,LABEL_Y_POS},{FIRST_COLUMN_FROM_LEFT,FIRST_COLUMN_FROM_ABOVE},
		TRUE,{LABEL_X_POS + DISTANCE_BETWEEN_COLUMNS + MAX_THREAD_ID_LEN,LABEL_Y_POS},{LABEL_X_POS + DISTANCE_BETWEEN_COLUMNS + MAX_THREAD_ID_LEN,FIRST_COLUMN_FROM_ABOVE},
		TRUE,{LABEL_X_POS + 2 * DISTANCE_BETWEEN_COLUMNS + MAX_THREAD_ID_LEN + MAX_THREAD_CYCLES_LEN,LABEL_Y_POS},{LABEL_X_POS + 2 * DISTANCE_BETWEEN_COLUMNS + MAX_THREAD_ID_LEN + MAX_THREAD_CYCLES_LEN,FIRST_COLUMN_FROM_ABOVE}},
		/*	ModulesTab. */
		{TRUE,{LABEL_X_POS, LABEL_Y_POS},{FIRST_COLUMN_FROM_LEFT, FIRST_COLUMN_FROM_ABOVE},
		TRUE,{LABEL_X_POS + DISTANCE_BETWEEN_COLUMNS + MAX_MODULE_NAME_LEN, LABEL_Y_POS},{FIRST_COLUMN_FROM_LEFT + DISTANCE_BETWEEN_COLUMNS + MAX_MODULE_NAME_LEN, FIRST_COLUMN_FROM_ABOVE},
		TRUE,{LABEL_X_POS + 2 * DISTANCE_BETWEEN_COLUMNS + MAX_MODULE_NAME_LEN + MAX_MODULE_BASE_SIZE_LEN, LABEL_Y_POS},{FIRST_COLUMN_FROM_LEFT  + 2 * DISTANCE_BETWEEN_COLUMNS + MAX_MODULE_NAME_LEN + MAX_MODULE_BASE_SIZE_LEN, FIRST_COLUMN_FROM_ABOVE}},
		/*	GeneralTab.	*/
			// Process Name
		{TRUE,{LABEL_X_POS, LABEL_Y_POS},{FIRST_COLUMN_FROM_LEFT, FIRST_COLUMN_FROM_ABOVE},
			// PID
		TRUE,{LABEL_X_POS + MAX_PROCESS_NAME_LEN + DISTANCE_BETWEEN_COLUMNS, LABEL_Y_POS},{FIRST_COLUMN_FROM_LEFT + MAX_PROCESS_NAME_LEN + DISTANCE_BETWEEN_COLUMNS, FIRST_COLUMN_FROM_ABOVE},
			// CPU Usage
		FALSE,{LABEL_X_POS + MAX_PROCESS_NAME_LEN + 2 * DISTANCE_BETWEEN_COLUMNS + MAX_PROCESS_ID_LEN, LABEL_Y_POS},{FIRST_COLUMN_FROM_LEFT + MAX_PROCESS_NAME_LEN + 2 * DISTANCE_BETWEEN_COLUMNS + MAX_PROCESS_ID_LEN, FIRST_COLUMN_FROM_ABOVE},
			//	Priority
		TRUE,{LABEL_X_POS + MAX_PROCESS_NAME_LEN + 3 * DISTANCE_BETWEEN_COLUMNS + MAX_PROCESS_ID_LEN + MAX_PROCESS_CPU_USAGE_LEN, LABEL_Y_POS},{FIRST_COLUMN_FROM_LEFT + MAX_PROCESS_NAME_LEN + 3 * DISTANCE_BETWEEN_COLUMNS + MAX_PROCESS_ID_LEN + MAX_PROCESS_CPU_USAGE_LEN, FIRST_COLUMN_FROM_ABOVE},
			// Working Set Size
		TRUE,{LABEL_X_POS + MAX_PROCESS_NAME_LEN + 4 * DISTANCE_BETWEEN_COLUMNS + MAX_PROCESS_ID_LEN + MAX_PROCESS_PRIORITY_LEN + MAX_PROCESS_CPU_USAGE_LEN, LABEL_Y_POS},{FIRST_COLUMN_FROM_LEFT + MAX_PROCESS_NAME_LEN + 4 * DISTANCE_BETWEEN_COLUMNS + MAX_PROCESS_ID_LEN + MAX_PROCESS_PRIORITY_LEN + MAX_PROCESS_CPU_USAGE_LEN, FIRST_COLUMN_FROM_ABOVE},
			//	Description
		TRUE,{LABEL_X_POS + MAX_PROCESS_NAME_LEN + 5 * DISTANCE_BETWEEN_COLUMNS + MAX_PROCESS_ID_LEN + MAX_PROCESS_PRIORITY_LEN + MAX_PROCESS_WORKING_SET_SIZE_LEN + MAX_PROCESS_CPU_USAGE_LEN, LABEL_Y_POS},{FIRST_COLUMN_FROM_LEFT + MAX_PROCESS_NAME_LEN + 5 * DISTANCE_BETWEEN_COLUMNS + MAX_PROCESS_ID_LEN + MAX_PROCESS_PRIORITY_LEN + MAX_PROCESS_WORKING_SET_SIZE_LEN + MAX_PROCESS_CPU_USAGE_LEN, FIRST_COLUMN_FROM_ABOVE},
			//	Company Name
		TRUE,{LABEL_X_POS + MAX_PROCESS_NAME_LEN + 6 * DISTANCE_BETWEEN_COLUMNS + MAX_PROCESS_ID_LEN + MAX_PROCESS_PRIORITY_LEN + MAX_PROCESS_WORKING_SET_SIZE_LEN + MAX_PROCESS_DESCRIPTION_LEN + MAX_PROCESS_CPU_USAGE_LEN, LABEL_Y_POS},{FIRST_COLUMN_FROM_LEFT + MAX_PROCESS_NAME_LEN + 6 * DISTANCE_BETWEEN_COLUMNS + MAX_PROCESS_ID_LEN + MAX_PROCESS_PRIORITY_LEN + MAX_PROCESS_WORKING_SET_SIZE_LEN + MAX_PROCESS_DESCRIPTION_LEN + MAX_PROCESS_CPU_USAGE_LEN, FIRST_COLUMN_FROM_ABOVE}}
		//	The End.
		};

//	Menus items.
struct tagPopUpMenuItems PopUpMenuItems = {
/*	General Menu.	*/			{TEXT("Kill Process"),TEXT("Kill Process Tree"),TEXT("Set Process Priority"),TEXT("Set Process WorkingSetSize")},
/*	Threads Menu.	*/			{TEXT("Resume Thread"),TEXT("Suspend Thread"),TEXT("Set Thread Priority")},
/*	Graph Menu.		*/			{TEXT("CPU Usage"),TEXT("Commit Charge"),TEXT("Physical Usage")},
/*	SetPriorityMenu.	*/		{{TEXT("Lowest"),TEXT("Below Normal"),TEXT("Normal"),TEXT("Above Normal"),TEXT("Highest"),TEXT("Time Critical")},
									{TEXT("IDLE"),TEXT("Below Normal"),TEXT("Normal"),TEXT("Above Normal"),TEXT("High"),TEXT("Real Time")}}
								};

struct tagDetailsLabels Details = {
/* Details Info Labels	*/		   TEXT("Process Name:"),TEXT("Process ID:"), TEXT("Parent ID:"), TEXT("Parent Name:"),
								   TEXT("Thread Count:"),TEXT("Module Count:"),TEXT("Priority:"),TEXT("Process Cycles:"), 
								   TEXT("Page Fault Count:"),TEXT("Page File Usage(KB):"),TEXT("Peak Page File Usage(KB):"),
								   TEXT("Peak Working Set Size(KB):"),TEXT("Quota NonPageed Pool Usage(KB):"),TEXT("Quota Paged Pool Usage(KB):"),
								   TEXT("Quota Peak NonPaged Pool Usage(KB):"),TEXT("Quta Peak Paged Pool Usage(KB):"),TEXT("WorkingSetSize(KB):"),
								   TEXT("Company Name:"),TEXT("Description:"),TEXT("Path:"),TEXT("File Version:"),TEXT("Internal Name:"),TEXT("Legal Copyright:"),
								   TEXT("Legal Trademarks"),TEXT("Original FileName:"),TEXT("Product Name:"),TEXT("Product Version:"),
								   TEXT("Private Build:"),TEXT("Special Build:"),TEXT("Comments:")
								  };

struct tagSystemInfoTabLabels SystemInfoTab ={{TEXT("Processor Architecture:"),TEXT("Number Of Processors:"),TEXT("Min App Address:"),TEXT("Max App Address:")},
/*	System Info Tab Com_Info Labels.	*/	 {TEXT("Commit Total:"),TEXT("Commit Limit:"),TEXT("Commit Peak:"),
												TEXT("Physical Total(Pages):"),TEXT("Physical Total(KB):"),TEXT("Physical Available(Pages):"),TEXT("Physical Available(KB):"),
												TEXT("Total Page File(KB):"),TEXT("Available Page File(KB):"),TEXT("Total Virtual(KB):"),TEXT("Available Virtual(KB):"),TEXT("System Cache(Pages):"),
												TEXT("Kernel Total(Pages):"),TEXT("Kernel Paged(Pages):"),TEXT("Kernel NonPaged(Pages):"),TEXT("Page Size(KB):"),
												TEXT("Handle Count:"),TEXT("Process Count:"),TEXT("Thread Count:")
											 }
};

//	Selected items initializing.
struct tagSelectedItems Selected = {INITIAL_TAB,0,{0,0},{0},{0},{0,0}};

//	Being in a Menu or not.
BOOL InMenu = FALSE;
//	Being in Set Priority Menu.
BOOL InSetPriorityMenu = FALSE;

/*	Opens The Current Tab.	*/
VOID DrawCurrentTab(BOOL FillTabInfo){
	//	Preparing column's labels for the current tab.
	DrawCurrentTabFrameAndLabels();
	if(FillTabInfo)
		//	Fills current tab information.
		FillCurrentTabInformationAndSelectItem();
}

/*	Closes the tab which was open.	*/
VOID CloseCurrentTab(VOID){
	INT i;
	switch(Selected.CurrentTab){
			case DETAILS:
				CPU_RunOnce = TRUE;
				PaintRegion(FIRST_COLUMN_FROM_ABOVE,FIRST_COLUMN_FROM_ABOVE + MAX_POSSIBLE_LINES,MID_X + LEFT_MARGIN + 1,WindowSize.X - RIGHT_MARGIN - 1,REGULAR_COLOR);
				EvacuateGraphHistory();
				break;
			case GENERAL:
				(Selected.Process.Current - Selected.Process.First >= MAX_POSSIBLE_LINES)
					? (HighlightXYXLine(UNDO,LIST_START_X,LIST_START_Y,LIST_WIDTH,MAX_POSSIBLE_LINES - 1))
					: HighlightXYXLine(UNDO,LIST_START_X,LIST_START_Y,LIST_WIDTH,Selected.Process.Current - Selected.Process.First);
				break;
			case THREADS:
				(Selected.Thread.Current - Selected.Thread.First >= MAX_POSSIBLE_LINES)
					? (HighlightXYXLine(UNDO,LIST_START_X,LIST_START_Y,LIST_WIDTH,MAX_POSSIBLE_LINES - 1))
					: (HighlightXYXLine(UNDO,LIST_START_X,LIST_START_Y,LIST_WIDTH,Selected.Thread.Current - Selected.Thread.First));
				break;
			case MODULES:
				(Selected.Module.Current - Selected.Module.First >= MAX_POSSIBLE_LINES)
					? (HighlightXYXLine(UNDO,LIST_START_X,LIST_START_Y,LIST_WIDTH,MAX_POSSIBLE_LINES - 1))
					: (HighlightXYXLine(UNDO,LIST_START_X,LIST_START_Y,LIST_WIDTH,Selected.Module.Current - Selected.Module.First));
				break;
			case GRAPH:
				PaintRegion(FIRST_COLUMN_FROM_ABOVE,FIRST_COLUMN_FROM_ABOVE + MAX_POSSIBLE_LINES,LEFT_MARGIN,WindowSize.X - RIGHT_MARGIN - 1,REGULAR_COLOR);
				EvacuateGraphHistory();
				break;
		}
	WaitForSingleObject(hScreenMutex,INFINITE);
#if !TABS_UP_BOARDER_LINES
		PrintXY(VERTICAL_TAB_LINE, TABS_START_POS + (Selected.CurrentTab) * TAB_WIDTH, 0);
		for( i = 1 ; i < TAB_WIDTH ; ++i)
			PrintXY(BLANK, TABS_START_POS + (Selected.CurrentTab) * TAB_WIDTH + i, 0);
		PrintXY(VERTICAL_TAB_LINE, TABS_START_POS + (Selected.CurrentTab) * TAB_WIDTH + i, 0);
#endif
		for(i=0;i<=TAB_WIDTH + 1;++i)
			PrintXY(BOARDER_LINE,TABS_START_POS + (Selected.CurrentTab) * TAB_WIDTH + i - 1,TABS_HEIGHT - 1);
		PrintXY(VERTICAL_TAB_LINE,TABS_START_POS + (Selected.CurrentTab) * TAB_WIDTH,TABS_HEIGHT - 1);
		PrintXY(VERTICAL_TAB_LINE,TABS_START_POS + (Selected.CurrentTab + 1) * TAB_WIDTH ,TABS_HEIGHT - 1);
		PrintXY(BOARDER_LINE,TABS_START_POS + (Selected.CurrentTab + 1) * TAB_WIDTH + 1,TABS_HEIGHT - 1);
		// Clears the tab's screen.
		CLS_TAB;
		ReleaseMutex(hScreenMutex);
}

/*	Fills System Info tab information.	*/
VOID ListSystemInfoTab(INT Start){
	INT Y=SYSTEM_INFO_TAB_START_Y;
	WaitForSingleObject(hScreenMutex,INFINITE);
	//	 Computer Information.
	CLS_TAB_LEFT_SIDE;
		//	Processor Architecture.
	PrintXY(SystemInfoTab.ComputerInfo.Items[0],SYSTEM_INFO_TAB_START_X,Y);
	LPrintXY(ProcessorArchitectureTranslate(SysInfo.wProcessorArchitecture),MID_X - (SYSTEM_INFO_TAB_START_X + MAX_DETAILS_LABEL_LEN + LEFT_MARGIN + 1),SYSTEM_INFO_TAB_START_X + MAX_SYSTEM_INFO_TAB_LABEL_LEN,Y);
	Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
		//	Number Of Processors.
	PrintXY(SystemInfoTab.ComputerInfo.Items[1],SYSTEM_INFO_TAB_START_X,Y);
	LNPrintXY(SysInfo.dwNumberOfProcessors,MID_X - (SYSTEM_INFO_TAB_START_X + MAX_DETAILS_LABEL_LEN + LEFT_MARGIN + 1),SYSTEM_INFO_TAB_START_X + MAX_SYSTEM_INFO_TAB_LABEL_LEN,Y);
	Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
		//	Min App Address.
	PrintXY(SystemInfoTab.ComputerInfo.Items[2],SYSTEM_INFO_TAB_START_X,Y);
	LNPrintXY((SIZE_T)SysInfo.lpMinimumApplicationAddress,MID_X - (SYSTEM_INFO_TAB_START_X + MAX_DETAILS_LABEL_LEN + LEFT_MARGIN + 1),SYSTEM_INFO_TAB_START_X + MAX_SYSTEM_INFO_TAB_LABEL_LEN,Y);
	Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
		//	Max App Address.
	PrintXY(SystemInfoTab.ComputerInfo.Items[3],SYSTEM_INFO_TAB_START_X,Y);
	LNPrintXY((SIZE_T)SysInfo.lpMaximumApplicationAddress,MID_X - (SYSTEM_INFO_TAB_START_X + MAX_DETAILS_LABEL_LEN + LEFT_MARGIN + 1),SYSTEM_INFO_TAB_START_X + MAX_SYSTEM_INFO_TAB_LABEL_LEN,Y);
	Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
	//	 Memory Information.
	Y=SYSTEM_INFO_TAB_START_Y;
	CLS_TAB_RIGHT_SIDE;
	switch(Start){
		//	Commit Total.
		case 0:	if(Y < SYSTEM_INFO_TAB_START_Y + SYSTEM_INFO_TAB_MAX_POSSIBLE_LINES){
			PrintXY(SystemInfoTab.MemoryInfo.Items[0],MID_X + SYSTEM_INFO_TAB_START_X,Y);
			LNPrintXY(PerInfo.CommitTotal,(WindowSize.X - RIGHT_MARGIN) - (MID_X + SYSTEM_INFO_TAB_START_X + MAX_DETAILS_LABEL_LEN + 1),MID_X + SYSTEM_INFO_TAB_START_X + MAX_SYSTEM_INFO_TAB_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
		//	Commit Limit.
		case 1:	if(Y < SYSTEM_INFO_TAB_START_Y + SYSTEM_INFO_TAB_MAX_POSSIBLE_LINES){
			PrintXY(SystemInfoTab.MemoryInfo.Items[1],MID_X + SYSTEM_INFO_TAB_START_X,Y);
			LNPrintXY(PerInfo.CommitLimit,(WindowSize.X - RIGHT_MARGIN) - (MID_X + SYSTEM_INFO_TAB_START_X + MAX_DETAILS_LABEL_LEN + 1),MID_X + SYSTEM_INFO_TAB_START_X + MAX_SYSTEM_INFO_TAB_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
		//	Commit Peak.
		case 2:	if(Y < SYSTEM_INFO_TAB_START_Y + SYSTEM_INFO_TAB_MAX_POSSIBLE_LINES){
			PrintXY(SystemInfoTab.MemoryInfo.Items[2],MID_X + SYSTEM_INFO_TAB_START_X,Y);
			LNPrintXY(PerInfo.CommitPeak,(WindowSize.X - RIGHT_MARGIN) - (MID_X + SYSTEM_INFO_TAB_START_X + MAX_DETAILS_LABEL_LEN + 1),MID_X + SYSTEM_INFO_TAB_START_X + MAX_SYSTEM_INFO_TAB_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
		//	Physical Total Pages.
		case 3:	if(Y < SYSTEM_INFO_TAB_START_Y + SYSTEM_INFO_TAB_MAX_POSSIBLE_LINES){
			PrintXY(SystemInfoTab.MemoryInfo.Items[3],MID_X + SYSTEM_INFO_TAB_START_X,Y);
			LNPrintXY(PerInfo.PhysicalTotal,(WindowSize.X - RIGHT_MARGIN) - (MID_X + SYSTEM_INFO_TAB_START_X + MAX_DETAILS_LABEL_LEN + 1),MID_X + SYSTEM_INFO_TAB_START_X + MAX_SYSTEM_INFO_TAB_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
		//	Physical Total KB.
		case 4:	if(Y < SYSTEM_INFO_TAB_START_Y + SYSTEM_INFO_TAB_MAX_POSSIBLE_LINES){
			PrintXY(SystemInfoTab.MemoryInfo.Items[4],MID_X + SYSTEM_INFO_TAB_START_X,Y);
			LNPrintXY(ByteTo(SysMemory.dwTotalPhys,KB),(WindowSize.X - RIGHT_MARGIN) - (MID_X + SYSTEM_INFO_TAB_START_X + MAX_DETAILS_LABEL_LEN + 1),MID_X + SYSTEM_INFO_TAB_START_X + MAX_SYSTEM_INFO_TAB_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
		//	Physical Available Pages.
		case 5:	if(Y < SYSTEM_INFO_TAB_START_Y + SYSTEM_INFO_TAB_MAX_POSSIBLE_LINES){
			PrintXY(SystemInfoTab.MemoryInfo.Items[5],MID_X + SYSTEM_INFO_TAB_START_X,Y);
			LNPrintXY(PerInfo.PhysicalAvailable,(WindowSize.X - RIGHT_MARGIN) - (MID_X + SYSTEM_INFO_TAB_START_X + MAX_DETAILS_LABEL_LEN + 1),MID_X + SYSTEM_INFO_TAB_START_X + MAX_SYSTEM_INFO_TAB_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
		//	Physical Available KB.
		case 6:	if(Y < SYSTEM_INFO_TAB_START_Y + SYSTEM_INFO_TAB_MAX_POSSIBLE_LINES){
			PrintXY(SystemInfoTab.MemoryInfo.Items[6],MID_X + SYSTEM_INFO_TAB_START_X,Y);
			LNPrintXY(ByteTo(SysMemory.dwAvailPhys,KB),(WindowSize.X - RIGHT_MARGIN) - (MID_X + SYSTEM_INFO_TAB_START_X + MAX_DETAILS_LABEL_LEN + 1),MID_X + SYSTEM_INFO_TAB_START_X + MAX_SYSTEM_INFO_TAB_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
		//	Total Page File KB.
		case 7:	if(Y < SYSTEM_INFO_TAB_START_Y + SYSTEM_INFO_TAB_MAX_POSSIBLE_LINES){
			PrintXY(SystemInfoTab.MemoryInfo.Items[7],MID_X + SYSTEM_INFO_TAB_START_X,Y);
			LNPrintXY(ByteTo(SysMemory.dwTotalPageFile,KB),(WindowSize.X - RIGHT_MARGIN) - (MID_X + SYSTEM_INFO_TAB_START_X + MAX_DETAILS_LABEL_LEN + 1),MID_X + SYSTEM_INFO_TAB_START_X + MAX_SYSTEM_INFO_TAB_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
		//	Available Page File KB.
		case 8:	if(Y < SYSTEM_INFO_TAB_START_Y + SYSTEM_INFO_TAB_MAX_POSSIBLE_LINES){
			PrintXY(SystemInfoTab.MemoryInfo.Items[8],MID_X + SYSTEM_INFO_TAB_START_X,Y);
			LNPrintXY(ByteTo(SysMemory.dwAvailPageFile,KB),(WindowSize.X - RIGHT_MARGIN) - (MID_X + SYSTEM_INFO_TAB_START_X + MAX_DETAILS_LABEL_LEN + 1),MID_X + SYSTEM_INFO_TAB_START_X + MAX_SYSTEM_INFO_TAB_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
		//	Total Virtual KB.
		case 9:	if(Y < SYSTEM_INFO_TAB_START_Y + SYSTEM_INFO_TAB_MAX_POSSIBLE_LINES){
			PrintXY(SystemInfoTab.MemoryInfo.Items[9],MID_X + SYSTEM_INFO_TAB_START_X,Y);
			LNPrintXY(ByteTo(SysMemory.dwTotalVirtual,KB),(WindowSize.X - RIGHT_MARGIN) - (MID_X + SYSTEM_INFO_TAB_START_X + MAX_DETAILS_LABEL_LEN + 1),MID_X + SYSTEM_INFO_TAB_START_X + MAX_SYSTEM_INFO_TAB_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
		//	Available Virtual KB.
		case 10: if(Y < SYSTEM_INFO_TAB_START_Y + SYSTEM_INFO_TAB_MAX_POSSIBLE_LINES){
			PrintXY(SystemInfoTab.MemoryInfo.Items[10],MID_X + SYSTEM_INFO_TAB_START_X,Y);
			LNPrintXY(ByteTo(SysMemory.dwAvailVirtual,KB),(WindowSize.X - RIGHT_MARGIN) - (MID_X + SYSTEM_INFO_TAB_START_X + MAX_DETAILS_LABEL_LEN + 1),MID_X + SYSTEM_INFO_TAB_START_X + MAX_SYSTEM_INFO_TAB_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
		//	System Cache.
		case 11: if(Y < SYSTEM_INFO_TAB_START_Y + SYSTEM_INFO_TAB_MAX_POSSIBLE_LINES){
			PrintXY(SystemInfoTab.MemoryInfo.Items[11],MID_X + SYSTEM_INFO_TAB_START_X,Y);
			LNPrintXY(PerInfo.SystemCache,(WindowSize.X - RIGHT_MARGIN) - (MID_X + SYSTEM_INFO_TAB_START_X + MAX_DETAILS_LABEL_LEN + 1),MID_X + SYSTEM_INFO_TAB_START_X + MAX_SYSTEM_INFO_TAB_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
		//	Kernel Total.
		case 12: if(Y < SYSTEM_INFO_TAB_START_Y + SYSTEM_INFO_TAB_MAX_POSSIBLE_LINES){
			PrintXY(SystemInfoTab.MemoryInfo.Items[12],MID_X + SYSTEM_INFO_TAB_START_X,Y);
			LNPrintXY(PerInfo.KernelTotal,(WindowSize.X - RIGHT_MARGIN) - (MID_X + SYSTEM_INFO_TAB_START_X + MAX_DETAILS_LABEL_LEN + 1),MID_X + SYSTEM_INFO_TAB_START_X + MAX_SYSTEM_INFO_TAB_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
		//	Kernel Paged.
		case 13: if(Y < SYSTEM_INFO_TAB_START_Y + SYSTEM_INFO_TAB_MAX_POSSIBLE_LINES){
			PrintXY(SystemInfoTab.MemoryInfo.Items[13],MID_X + SYSTEM_INFO_TAB_START_X,Y);
			LNPrintXY(PerInfo.KernelPaged,(WindowSize.X - RIGHT_MARGIN) - (MID_X + SYSTEM_INFO_TAB_START_X + MAX_DETAILS_LABEL_LEN + 1),MID_X + SYSTEM_INFO_TAB_START_X + MAX_SYSTEM_INFO_TAB_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
		//	Kernel NonPaged.
		case 14: if(Y < SYSTEM_INFO_TAB_START_Y + SYSTEM_INFO_TAB_MAX_POSSIBLE_LINES){
			PrintXY(SystemInfoTab.MemoryInfo.Items[14],MID_X + SYSTEM_INFO_TAB_START_X,Y);
			LNPrintXY(PerInfo.KernelNonpaged,(WindowSize.X - RIGHT_MARGIN) - (MID_X + SYSTEM_INFO_TAB_START_X + MAX_DETAILS_LABEL_LEN + 1),MID_X + SYSTEM_INFO_TAB_START_X + MAX_SYSTEM_INFO_TAB_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
		//	Page Size.
		case 15: if(Y < SYSTEM_INFO_TAB_START_Y + SYSTEM_INFO_TAB_MAX_POSSIBLE_LINES){
			PrintXY(SystemInfoTab.MemoryInfo.Items[15],MID_X + SYSTEM_INFO_TAB_START_X,Y);
			LNPrintXY(ByteTo(PerInfo.PageSize,KB),(WindowSize.X - RIGHT_MARGIN) - (MID_X + SYSTEM_INFO_TAB_START_X + MAX_DETAILS_LABEL_LEN + 1),MID_X + SYSTEM_INFO_TAB_START_X + MAX_SYSTEM_INFO_TAB_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
		// Handle Count.
		case 16: if(Y < SYSTEM_INFO_TAB_START_Y + SYSTEM_INFO_TAB_MAX_POSSIBLE_LINES){
			PrintXY(SystemInfoTab.MemoryInfo.Items[16],MID_X + SYSTEM_INFO_TAB_START_X,Y);
			LNPrintXY(PerInfo.HandleCount,(WindowSize.X - RIGHT_MARGIN) - (MID_X + SYSTEM_INFO_TAB_START_X + MAX_DETAILS_LABEL_LEN + 1),MID_X + SYSTEM_INFO_TAB_START_X + MAX_SYSTEM_INFO_TAB_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
		//	Process Count.
		case 17: if(Y < SYSTEM_INFO_TAB_START_Y + SYSTEM_INFO_TAB_MAX_POSSIBLE_LINES){
			PrintXY(SystemInfoTab.MemoryInfo.Items[17],MID_X + SYSTEM_INFO_TAB_START_X,Y);
			LNPrintXY(PerInfo.ProcessCount,(WindowSize.X - RIGHT_MARGIN) - (MID_X + SYSTEM_INFO_TAB_START_X + MAX_DETAILS_LABEL_LEN + 1),MID_X + SYSTEM_INFO_TAB_START_X + MAX_SYSTEM_INFO_TAB_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
				 }
		//	Thread Count.
		case 18: if(Y < SYSTEM_INFO_TAB_START_Y + SYSTEM_INFO_TAB_MAX_POSSIBLE_LINES){
			PrintXY(SystemInfoTab.MemoryInfo.Items[18],MID_X + SYSTEM_INFO_TAB_START_X,Y);
			LNPrintXY(PerInfo.ThreadCount,(WindowSize.X - RIGHT_MARGIN) - (MID_X + SYSTEM_INFO_TAB_START_X + MAX_DETAILS_LABEL_LEN + 1),MID_X + SYSTEM_INFO_TAB_START_X + MAX_SYSTEM_INFO_TAB_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
	}
	ReleaseMutex(hScreenMutex);
}

/*	Fills Details tab information.	*/
VOID ListDetails(INT Start){
	INT Y=DETAILS_START_Y;
	WaitForSingleObject(hScreenMutex,INFINITE);
	CLS_TAB_LEFT_SIDE;
	switch(Start){
			//	 Process Name.
	case 0:	if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[0],DETAILS_START_X,Y);
				LPrintXY(PeInfo[Selected.Process.Current].ProcessName,MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			//	Process ID.
	case 1:	if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[1],DETAILS_START_X,Y);
				LNPrintXY(PeInfo[Selected.Process.Current].PID,MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			//	Parent ID.
	case 2:	if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[2],DETAILS_START_X,Y);
				LNPrintXY(PeInfo[Selected.Process.Current].ParentPID,MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			//	Parent Name.
	case 3:	if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[3],DETAILS_START_X,Y);
				LPrintXY(FindParent(PeInfo[Selected.Process.Current].ParentPID),MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			//	Thread Count.
	case 4:	if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[4],DETAILS_START_X,Y);
				LNPrintXY(PeInfo[Selected.Process.Current].ThreadCount,MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			//	Module Count.
	case 5:	if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[5],DETAILS_START_X,Y);
				LNPrintXY(PeInfo[Selected.Process.Current].ModuleCount,MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			//	Priority.
	case 6:	if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[6],DETAILS_START_X,Y);
				LPrintXY(ProcessPriorityTranslate(PeInfo[Selected.Process.Current].Priority),MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			//	Process Cycles.
	case 7:	if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[7],DETAILS_START_X,Y);
				LNPrintXY(PeInfo[Selected.Process.Current].CycleTime,MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			//	Page Fault Count.
	case 8:	if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[8],DETAILS_START_X,Y);
				LNPrintXY(PeInfo[Selected.Process.Current].PageFaultCount,MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			//	Page File Usage.
	case 9:	if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[9],DETAILS_START_X,Y);
				LNPrintXY(ByteTo(PeInfo[Selected.Process.Current].PagefileUsage,KB),MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			//	Peak Page File Usage.
	case 10: if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[10],DETAILS_START_X,Y);
				LNPrintXY(ByteTo(PeInfo[Selected.Process.Current].PeakPagefileUsage,KB),MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			 //	Peak Working Set Size.
	case 11: if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[11],DETAILS_START_X,Y);
				LNPrintXY(ByteTo(PeInfo[Selected.Process.Current].PeakWorkingSetSize,KB),MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			 //	Quota NonPaged Pool Usage.
	case 12: if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[12],DETAILS_START_X,Y);
				LNPrintXY(ByteTo(PeInfo[Selected.Process.Current].QuotaNonPagedPoolUsage,KB),MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			 //	Quota Page Pool Usage.
	case 13: if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[13],DETAILS_START_X,Y);
				LNPrintXY(ByteTo(PeInfo[Selected.Process.Current].QuotaPagedPoolUsage,KB),MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			 //	Quota Peak NonPaged Pool Usage.
	case 14: if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[14],DETAILS_START_X,Y);
				LNPrintXY(ByteTo(PeInfo[Selected.Process.Current].QuotaPeakNonPagedPoolUsage,KB),MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			 //	Quota Peak Paged Pool Usage.
	case 15: if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[15],DETAILS_START_X,Y);
				LNPrintXY(ByteTo(PeInfo[Selected.Process.Current].QuotaPeakPagedPoolUsage,KB),MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			 //	WorkingSetSize.
	case 16: if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[16],DETAILS_START_X,Y);
				LNPrintXY(ByteTo(PeInfo[Selected.Process.Current].WorkingSetSize,KB),MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			 //	Companu Name.
	case 17:	if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){	
				PrintXY(Details.Items[17],DETAILS_START_X,Y);
				LPrintXY(PeInfo[Selected.Process.Current].ProcessCompanyName,MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			//	Description.
	case 18:if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[18],DETAILS_START_X,Y);
				LPrintXY(PeInfo[Selected.Process.Current].ProcessDescription,MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			//	Path.
	case 19:if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[19],DETAILS_START_X,Y);
				LPrintXY(PeInfo[Selected.Process.Current].ProcessPath,MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			// File Version.
	case 20:if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[20],DETAILS_START_X,Y);
				LPrintXY(PeInfo[Selected.Process.Current].ProcessFileVersion,MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			//	Internal Name.
	case 21:if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[21],DETAILS_START_X,Y);
				LPrintXY(PeInfo[Selected.Process.Current].ProcessInternalName,MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			//	Legal Copyright.
	case 22:if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[22],DETAILS_START_X,Y);
				LPrintXY(PeInfo[Selected.Process.Current].ProcessLegalCopyright,MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			//	Legal Trademarks.
	case 23:if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[23],DETAILS_START_X,Y);
				LPrintXY(PeInfo[Selected.Process.Current].ProcessLegalTrademarks,MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			//	Original FileName.
	case 24:if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[24],DETAILS_START_X,Y);
				LPrintXY(PeInfo[Selected.Process.Current].ProcessOriginalFilename,MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			//	Product Name.
	case 25:if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[25],DETAILS_START_X,Y);
				LPrintXY(PeInfo[Selected.Process.Current].ProcessProductName,MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			//	Product Version.
	case 26:if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[26],DETAILS_START_X,Y);
				LPrintXY(PeInfo[Selected.Process.Current].ProcessProductVersion,MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			//	Private Build.
	case 27:if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[27],DETAILS_START_X,Y);
				LPrintXY(PeInfo[Selected.Process.Current].ProcessPrivateBuild,MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			//	Special Build.
	case 28:if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[28],DETAILS_START_X,Y);
				LPrintXY(PeInfo[Selected.Process.Current].ProcessSpecialBuild,MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
			//	Comments.
	case 29:if(Y < DETAILS_START_Y + DETAILS_MAX_POSSIBLE_LINES){
				PrintXY(Details.Items[29],DETAILS_START_X,Y);
				LPrintXY(PeInfo[Selected.Process.Current].ProcessComments,MID_X - (DETAILS_START_X + MAX_DETAILS_LABEL_LEN + 1),DETAILS_START_X + MAX_DETAILS_LABEL_LEN,Y);
				Y+=DETAILS_ITEMS_VERTICAL_DISTANCE;
			}
	}
	ReleaseMutex(hScreenMutex);
}

/*	Writes current tab labels.	*/
VOID DrawCurrentTabFrameAndLabels(VOID){
	INT i;
	if(Selected.CurrentTab >= NUMBER_OF_TABS)
		Selected.CurrentTab %= NUMBER_OF_TABS;
	WaitForSingleObject(hScreenMutex,INFINITE);
#if !TABS_UP_BOARDER_LINES
		PrintXY(BLANK, TABS_START_POS + (Selected.CurrentTab) * TAB_WIDTH, 0);
		for( i = 1 ; i < TAB_WIDTH ; ++i)
			PrintXY(HORIZONTAL_TAB_LINE, TABS_START_POS + (Selected.CurrentTab) * TAB_WIDTH + i, 0);
		PrintXY(BLANK, TABS_START_POS + (Selected.CurrentTab) * TAB_WIDTH + i, 0);
#endif
		PrintXY(LEFT_OPEN_TAB_CHAR,TABS_START_POS + (Selected.CurrentTab) * TAB_WIDTH - 1,TABS_HEIGHT - 1);
		for(i=1;i<=TAB_WIDTH;++i)
			PrintXY(BLANK,TABS_START_POS + (Selected.CurrentTab) * TAB_WIDTH + i - 1,TABS_HEIGHT - 1);
		PrintXY(BLANK,TABS_START_POS + (Selected.CurrentTab + 1) * TAB_WIDTH ,TABS_HEIGHT - 1);
		PrintXY(RIGHT_OPEN_TAB_CHAR,TABS_START_POS + (Selected.CurrentTab + 1) * TAB_WIDTH + 1,TABS_HEIGHT - 1);
	switch(Selected.CurrentTab){
	/*	Printing Labels.	*/
	case SYSTEM_INFO_TAB:
		PrintXY(SYSTEM_INFO_TAB_FIRST_SIDE_LABEL,LABEL_X_POS,LABEL_Y_POS);
		PrintXY(SYSTEM_INFO_TAB_SECOND_SIDE_LABEL,MID_X + LABEL_X_POS,LABEL_Y_POS);
		break;
	case DETAILS:
		PrintXY(DETAILS_FIRST_SIDE_LABEL,LABEL_X_POS,LABEL_Y_POS);
		PrintXY(DETAILS_SECOND_SIDE_LABEL,MID_X + LABEL_X_POS,LABEL_Y_POS);
		break;
	case THREADS:
		if(Tab.Threads.ThreadPIDColumn_Visibile)
			PrintXY(THREAD_ID_LABEL,Tab.Threads.ThreadPIDLabel_Pos.X,Tab.Threads.ThreadPIDLabel_Pos.Y);
		if(Tab.Threads.ThreadCycles_Visibile)
			PrintXY(THREAD_CYCLES_LABEL,Tab.Threads.ThreadCyclesLabel_Pos.X,Tab.Threads.ThreadCyclesLabel_Pos.Y);
		if(Tab.Threads.ThreadPriority_Visibile)
			PrintXY(THREAD_PRIORITY_LABEL,Tab.Threads.ThreadPriorityLabel_Pos.X,Tab.Threads.ThreadPriorityLabel_Pos.Y);
		break;
	case MODULES:
		if(Tab.Modules.ModuleNameColumn_Visibile)
			PrintXY(MODULE_NAME_LABEL,Tab.Modules.ModuleNameLabel_Pos.X,Tab.Modules.ModuleNameLabel_Pos.Y);
		if(Tab.Modules.ModuleBaseSizeColumn_Visibile)
			PrintXY(MODULE_BASE_SIZE_LABEL , Tab.Modules.ModuleBaseSizeLabel_Pos.X , Tab.Modules.ModuleBaseSizeLabel_Pos.Y );
		if(Tab.Modules.ModulePathColumn_Visibile)
			PrintXY(MODULE_PATH_LABEL,Tab.Modules.ModulePathLabel_Pos.X,Tab.Modules.ModulePathLabel_Pos.Y);
		break;
	case GENERAL:
		if(Tab.Processes.ProcessNameColumn_Visibile)
			PrintXY(PROCESS_NAME_LABEL,Tab.Processes.ProcessNameLabel_Pos.X,Tab.Processes.ProcessNameLabel_Pos.Y);
		if(Tab.Processes.ProcessIDColumn_Visibile)
			PrintXY(PROCESS_ID_LABEL,Tab.Processes.ProcessIDLabel_Pos.X,Tab.Processes.ProcessIDLabel_Pos.Y);
		if(Tab.Processes.ProcessCPU_UsageColumn_Visibile)
			PrintXY(PROCESS_CPU_USAGE_LABEL,Tab.Processes.ProcessCPU_UsageLabel_Pos.X,Tab.Processes.ProcessCPU_UsageLabel_Pos.Y);
		if(Tab.Processes.ProcessPriorityColumn_Visibile)
			PrintXY(PROCESS_PRIORITY_LABEL,Tab.Processes.ProcessPriorityLabel_Pos.X,Tab.Processes.ProcessPriorityLabel_Pos.Y);
		if(Tab.Processes.ProcessWorkingSetSizeColumn_Visibile)
			PrintXY(PROCESS_WORKING_SET_SIZE_LABEL,Tab.Processes.ProcessWorkingSetSizeLabel_Pos.X,Tab.Processes.ProcessWorkingSetSizeLabel_Pos.Y);
		if(Tab.Processes.ProcessDescriptionColumn_Visibile)
			PrintXY(PROCESS_DESCRIPTION_LABEL,Tab.Processes.ProcessDescriptionLabel_Pos.X,Tab.Processes.ProcessDescriptionLabel_Pos.Y);
		if(Tab.Processes.ProcessCompanyNameColumn_Visibile)
			PrintXY(PROCESS_COMPANY_NAME_LABEL,Tab.Processes.ProcessCompanyNameLabel_Pos.X,Tab.Processes.ProcessCompanyNameLabel_Pos.Y);
		break;
	}
	/*	Drawing Lines.	*/
	switch(Selected.CurrentTab){
	case GENERAL:
	case GRAPH:
	case MODULES:
	case THREADS:
		//	the line under labels.
		for(i=LEFT_MARGIN;i<WindowSize.X-RIGHT_MARGIN;++i)
			PrintXY(BOARDER_LINE,i,Y_LINE_UNDER_LABELS);
		break;
	case DETAILS:
		//	The line under labels.
		for(i=LEFT_MARGIN;i<WindowSize.X-RIGHT_MARGIN;++i)
			PrintXY(BOARDER_LINE,i,DETAILS_Y_LINE_UNDER_LABELS);
		//	Vertical line.
		for(i=FIRST_COLUMN_FROM_ABOVE;i<FIRST_COLUMN_FROM_ABOVE + MAX_POSSIBLE_LINES + 1;++i)
			PrintXY(VERTICAL_TAB_LINE,MID_X,i);
		break;
	case SYSTEM_INFO_TAB:
		//	The line under labels.
		for(i=LEFT_MARGIN;i<WindowSize.X-RIGHT_MARGIN;++i)
			PrintXY(BOARDER_LINE,i,SYSTEM_INFO_TAB_Y_LINE_UNDER_LABELS);
		//	Vertical line.
		for(i=FIRST_COLUMN_FROM_ABOVE;i<FIRST_COLUMN_FROM_ABOVE + MAX_POSSIBLE_LINES + 1;++i)
			PrintXY(VERTICAL_TAB_LINE,MID_X,i);
		break;
	}
	ReleaseMutex(hScreenMutex);
	return;
}

/*	Fills current tab information.	*/
VOID FillCurrentTabInformationAndSelectItem(VOID){
	switch(Selected.CurrentTab){
	case GENERAL:
		//	Cleans the tab before putting info within it.
		CLS_BELOW_TAB;
		HighlightXYXLine(UNDO,LIST_START_X,LIST_START_Y,LIST_WIDTH,Selected.Process.Current - Selected.Process.First);
		//
		if(Selected.Process.Current >= CoreProcessCount && CoreProcessCount)
			Selected.Process.Current = CoreProcessCount - 1;
		if(Selected.Process.Current < Selected.Process.First)
			Selected.Process.First = Selected.Process.Current;
		//	Selectes the selected item in the current opened tab.
		(Selected.Process.Current - Selected.Process.First >= MAX_POSSIBLE_LINES)
			? ListProcesses(0,(Selected.Process.First = Selected.Process.Current - MAX_POSSIBLE_LINES + 1),MAX_POSSIBLE_LINES)
			: ((Selected.Process.First + MAX_POSSIBLE_LINES > CoreProcessCount)?(ListProcesses(0,Selected.Process.First = CoreProcessCount-MAX_POSSIBLE_LINES ,MAX_POSSIBLE_LINES)):(ListProcesses(0,Selected.Process.First,MAX_POSSIBLE_LINES)));
		if(MAX_POSSIBLE_LINES > 0 && CoreProcessCount != 0)
			HighlightXYXLine(DO,LIST_START_X,LIST_START_Y,LIST_WIDTH,Selected.Process.Current - Selected.Process.First);
		break;
	case DETAILS:
		if(DETAILS_ITEMS - Selected.Detail.First < DETAILS_MAX_POSSIBLE_LINES/DETAILS_ITEMS_VERTICAL_DISTANCE)
			(Selected.Detail.First - (DETAILS_MAX_POSSIBLE_LINES/DETAILS_ITEMS_VERTICAL_DISTANCE - ( DETAILS_ITEMS - Selected.Detail.First)) >= 0)?(Selected.Detail.First -= (DETAILS_MAX_POSSIBLE_LINES/DETAILS_ITEMS_VERTICAL_DISTANCE - ( DETAILS_ITEMS - Selected.Detail.First))):(Selected.Detail.First=0);
		ListDetails(Selected.Detail.First);
		break;
	case SYSTEM_INFO_TAB:
		if(SYSTEM_INFO_TAB_ITEMS - Selected.SystemInfoTab.First < SYSTEM_INFO_TAB_MAX_POSSIBLE_LINES/SYSTEM_INFO_TAB_ITEMS_VERTICAL_DISTANCE)
			(Selected.SystemInfoTab.First - (SYSTEM_INFO_TAB_MAX_POSSIBLE_LINES/SYSTEM_INFO_TAB_ITEMS_VERTICAL_DISTANCE - ( SYSTEM_INFO_TAB_ITEMS - Selected.SystemInfoTab.First)) >= 0)?(Selected.SystemInfoTab.First -= (SYSTEM_INFO_TAB_MAX_POSSIBLE_LINES/SYSTEM_INFO_TAB_ITEMS_VERTICAL_DISTANCE - ( SYSTEM_INFO_TAB_ITEMS - Selected.SystemInfoTab.First))):(Selected.SystemInfoTab.First=0);
		ListSystemInfoTab(Selected.SystemInfoTab.First);
		break;
	case MODULES:
		//	Cleans the tab before putting info within it.
		CLS_BELOW_TAB;
		HighlightXYXLine(UNDO,LIST_START_X,LIST_START_Y,LIST_WIDTH,Selected.Module.Current - Selected.Module.First);
		//
		if(Selected.Module.Current >= PeInfo[Selected.Process.Current].ModuleCount && PeInfo[Selected.Process.Current].ModuleCount)
			Selected.Module.Current = PeInfo[Selected.Process.Current].ModuleCount - 1;
		if(Selected.Module.Current < Selected.Module.First)
			Selected.Module.First = Selected.Module.Current;
		//	Selectes the selected item in the current opened tab.
		(Selected.Module.Current - Selected.Module.First >= MAX_POSSIBLE_LINES)
			? ListModules(0,(Selected.Module.First = Selected.Module.Current - MAX_POSSIBLE_LINES + 1),MAX_POSSIBLE_LINES)
			: (ListModules(0,Selected.Module.First,MAX_POSSIBLE_LINES));
		if(MAX_POSSIBLE_LINES > 0 && PeInfo[Selected.Process.Current].ModuleCount != 0)
			HighlightXYXLine(DO,LIST_START_X,LIST_START_Y,LIST_WIDTH,Selected.Module.Current - Selected.Module.First);
		break;
	case THREADS:
		//	Cleans the tab before putting info within it.
		CLS_BELOW_TAB;
		HighlightXYXLine(UNDO,LIST_START_X,LIST_START_Y,LIST_WIDTH,Selected.Thread.Current - Selected.Thread.First);
		//
		if(Selected.Thread.Current >= PeInfo[Selected.Thread.Current].ThreadCount && PeInfo[Selected.Thread.Current].ThreadCount)
			Selected.Thread.Current = PeInfo[Selected.Thread.Current].ThreadCount - 1;
		if(Selected.Thread.Current < Selected.Thread.First)
			Selected.Thread.First = Selected.Thread.Current;
		//	Selectes the selected item in the current opened tab.
		((Selected.Thread.Current - Selected.Thread.First) + 1 > MAX_POSSIBLE_LINES)
			? ListThreads(0 , (Selected.Thread.First = Selected.Thread.Current - MAX_POSSIBLE_LINES + 1) , MAX_POSSIBLE_LINES)
			: (ListThreads(0,Selected.Thread.First,MAX_POSSIBLE_LINES));
		if(MAX_POSSIBLE_LINES > 0 && PeInfo[Selected.Process.Current].ThreadCount != 0)
			HighlightXYXLine(DO,LIST_START_X,LIST_START_Y,LIST_WIDTH,Selected.Thread.Current - Selected.Thread.First);
		break;
	case GRAPH:
		//	Empty.
		break;
	default:
		break;
	}
}

/*	Lists Processes.	*/
VOID ListProcesses(INT StartLine,INT ProcessNumber,INT Count){
	INT i;
	WaitForSingleObject(hScreenMutex,INFINITE);
	for(i=0 ; i < MAX_POSSIBLE_LINES && (ProcessNumber + i) < CoreProcessCount && i < Count ; ++StartLine , ++i){
		if(Tab.Processes.ProcessNameColumn_Visibile)
			LPrintXY(PeInfo[ProcessNumber + i].ProcessName,MAX_PROCESS_NAME_LEN,Tab.Processes.ProcessNameList_StartPos.X,Tab.Processes.ProcessNameList_StartPos.Y + StartLine);
		if(Tab.Processes.ProcessIDColumn_Visibile)
			NPrintXY(PeInfo[ProcessNumber + i].PID,Tab.Processes.ProcessIDList_StartPos.X,Tab.Processes.ProcessIDList_StartPos.Y + StartLine);
		if(Tab.Processes.ProcessCPU_UsageColumn_Visibile)
			NPrintXY((ULONG64)PeInfo[ProcessNumber + i].CPU_Usage,Tab.Processes.ProcessCPU_UsageList_StartPos.X,Tab.Processes.ProcessCPU_UsageList_StartPos.Y + StartLine);
		if(Tab.Processes.ProcessPriorityColumn_Visibile)
			PrintXY(ProcessPriorityTranslate(PeInfo[ProcessNumber + i].Priority),Tab.Processes.ProcessPriorityList_StartPos.X,Tab.Processes.ProcessPriorityList_StartPos.Y + StartLine);
		if(Tab.Processes.ProcessWorkingSetSizeColumn_Visibile)
			NPrintXY(ByteTo(PeInfo[ProcessNumber + i].WorkingSetSize,KB),Tab.Processes.ProcessWorkingSetSizeList_StartPos.X,Tab.Processes.ProcessWorkingSetSizeList_StartPos.Y + StartLine);
		if(Tab.Processes.ProcessDescriptionColumn_Visibile)
			LPrintXY(PeInfo[ProcessNumber + i].ProcessDescription,MAX_VISIBLE_TEXT_IN_COLUMN ,Tab.Processes.ProcessDescriptionList_StartPos.X,Tab.Processes.ProcessDescriptionList_StartPos.Y + StartLine);
		if(Tab.Processes.ProcessCompanyNameColumn_Visibile)
			LPrintXY(PeInfo[ProcessNumber + i].ProcessCompanyName,MAX_VISIBLE_TEXT_IN_COLUMN + (WindowSize.X - Tab.Processes.ProcessCompanyNameList_StartPos.X - 2 * RIGHT_MARGIN - MAX_VISIBLE_TEXT_IN_COLUMN),Tab.Processes.ProcessCompanyNameList_StartPos.X,Tab.Processes.ProcessCompanyNameList_StartPos.Y + StartLine);
	}
	ReleaseMutex(hScreenMutex);
}

/*	Lists selected threads of the selected process in the screen.	*/
VOID ListThreads(INT StartLine,INT ThreadNumber,INT Count){
	INT i;
	WaitForSingleObject(hScreenMutex,INFINITE);
	for(i=0 ; i < MAX_POSSIBLE_LINES && ThreadNumber < PeInfo[Selected.Process.Current].ThreadCount && i < Count ; ++StartLine,++i, ++ThreadNumber){
		if(Tab.Threads.ThreadPIDColumn_Visibile)
			NPrintXY(PeInfo[Selected.Process.Current].Threads[ThreadNumber].dwTID,Tab.Threads.ThreadPIDList_StartPos.X,Tab.Threads.ThreadPIDList_StartPos.Y+StartLine);
		if(Tab.Threads.ThreadCycles_Visibile)
			NPrintXY(PeInfo[Selected.Process.Current].Threads[ThreadNumber].CycleTime,Tab.Threads.ThreadCycles_StartPos.X,Tab.Threads.ThreadCycles_StartPos.Y+StartLine);
		if(Tab.Threads.ThreadPriority_Visibile)
			PrintXY(ThreadPriorityTranslate(PeInfo[Selected.Process.Current].Threads[ThreadNumber].ThreadPriority),Tab.Threads.ThreadPriority_StartPos.X,Tab.Threads.ThreadPriority_StartPos.Y+StartLine);
	}
	ReleaseMutex(hScreenMutex);
}

/*	Lists selected modules of the selected process in the screen.	*/
VOID ListModules(INT StartLine,INT ModuleNumber,INT Count){
	INT i;
	WaitForSingleObject(hScreenMutex,INFINITE);
	for(i=0 ; i < MAX_POSSIBLE_LINES && ModuleNumber < PeInfo[Selected.Process.Current].ModuleCount && i < Count ; ++StartLine,++i, ++ModuleNumber){
		if(Tab.Modules.ModuleNameColumn_Visibile)
			LPrintXY(PeInfo[Selected.Process.Current].Modules[ModuleNumber].ModuleName,MAX_MODULE_NAME_LEN,Tab.Modules.ModuleNameList_StartPos.X,Tab.Modules.ModuleNameList_StartPos.Y + StartLine);
		if(Tab.Modules.ModuleNameColumn_Visibile)
			NPrintXY(ByteTo(PeInfo[Selected.Process.Current].Modules[ModuleNumber].ModuleBaseSize,KB),Tab.Modules.ModuleBaseSizeList_StartPos.X,Tab.Modules.ModuleBaseSizeList_StartPos.Y + StartLine);
		if(Tab.Modules.ModulePathColumn_Visibile)
			LPrintXY(PeInfo[Selected.Process.Current].Modules[ModuleNumber].ModulePath,MAX_VISIBLE_TEXT_IN_COLUMN + (WindowSize.X - Tab.Modules.ModulePathList_StartPos.X - 2 * RIGHT_MARGIN - MAX_VISIBLE_TEXT_IN_COLUMN),Tab.Modules.ModulePathList_StartPos.X,Tab.Modules.ModulePathList_StartPos.Y + StartLine);
	}
	ReleaseMutex(hScreenMutex);
}

/*	Highlights/UnHighlights the selected lines.	*/
VOID HighlightXYXLine(BOOL Highlight,INT x,INT y,INT Width,INT Line){
	//	Start point.
	COORD Pos = {x,y + ((Line>0?(Line):(0)))};
	WaitForSingleObject(hScreenMutex,INFINITE);
	(Highlight)?ColorXYXCount(HIGHLIGHT_COLOR,Pos.X,Pos.Y,Width,1):ColorXYXCount(REGULAR_COLOR,Pos.X,Pos.Y,Width,1);
	ReleaseMutex(hScreenMutex);
}

/*	Scrolls tabs and tab's lists.	*/
VOID Scroll(INT Direction){
	// Horizentol Scrolling.
	switch(Direction){
		case LEFT:
			//	If no menu was opened.
			if(!InMenu){
				CloseCurrentTab();
				((Selected.CurrentTab-1)<0)?(Selected.CurrentTab=(NUMBER_OF_TABS-1)):(Selected.CurrentTab=(Selected.CurrentTab-1));
				DrawCurrentTab(TRUE);
			}
			break;
		case RIGHT:
			//	If no menu was opened.
			if(!InMenu){
				CloseCurrentTab();
				((Selected.CurrentTab+1)>=NUMBER_OF_TABS)?(Selected.CurrentTab=0):(Selected.CurrentTab=(Selected.CurrentTab+1));
				DrawCurrentTab(TRUE);
			}
			break;
	}
	WaitForSingleObject(hScreenMutex,INFINITE);
	//	Vertical Scrolling.
	switch(Selected.CurrentTab){
	case SYSTEM_INFO_TAB:
		switch(Direction){
		case UP:
			if(Selected.SystemInfoTab.First != 0)
				if(Selected.SystemInfoTab.First - 1 >= 0){
					--Selected.SystemInfoTab.First;
					ListSystemInfoTab(Selected.SystemInfoTab.First);
				}
			break;
		case DOWN:
			if(SYSTEM_INFO_TAB_MAX_POSSIBLE_LINES/SYSTEM_INFO_TAB_ITEMS_VERTICAL_DISTANCE < (SYSTEM_INFO_TAB_ITEMS - Selected.SystemInfoTab.First))
				if(Selected.SystemInfoTab.First + 1 < SYSTEM_INFO_TAB_ITEMS){
					++Selected.SystemInfoTab.First;
					ListSystemInfoTab(Selected.SystemInfoTab.First);
				}
			break;
		}
		break;
	case DETAILS:
		switch(Direction){
		case UP:
			if(Selected.Detail.First != 0)
				if(Selected.Detail.First - 1 >= 0){
					--Selected.Detail.First;
					ListDetails(Selected.Detail.First);
				}
			break;
		case DOWN:
			if(DETAILS_MAX_POSSIBLE_LINES/DETAILS_ITEMS_VERTICAL_DISTANCE < (DETAILS_ITEMS - Selected.Detail.First))
				if(Selected.Detail.First + 1 < DETAILS_ITEMS){
					++Selected.Detail.First;
					ListDetails(Selected.Detail.First);
				}
			break;
		}
		break;
		case GRAPH:
		switch(Direction){
			case UP:				
				if(InMenu && (Selected.PopUpMenu.Graph.Current - 1) >= 0  && MENU_MAX_POSSIBLE_ITEMS > 0){
						if(Selected.PopUpMenu.Graph.Current - Selected.PopUpMenu.Graph.First > 0){
							//	Some special conditions may happen during resizing.
							HighlightXYXLine(UNDO,GRAPH_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,GRAPH_MENU_WIDTH-1,Selected.PopUpMenu.Graph.Current - Selected.PopUpMenu.Graph.First);
							--Selected.PopUpMenu.Graph.Current;
							HighlightXYXLine(DO,GRAPH_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,GRAPH_MENU_WIDTH-1,Selected.PopUpMenu.Graph.Current - Selected.PopUpMenu.Graph.First);
						}else{
							//	Special Conditions.
							CLS_INSIDE_GRAPH_MENU;
							--Selected.PopUpMenu.Graph.First;
							ListCurrentPopUpMenuItems(0,Selected.PopUpMenu.Graph.First,MENU_MAX_POSSIBLE_ITEMS);
							--Selected.PopUpMenu.Graph.Current;
							HighlightXYXLine(DO,GRAPH_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,GRAPH_MENU_WIDTH-1,0);
						}
					}
				break;
			case DOWN:
					if(InMenu && (Selected.PopUpMenu.Graph.Current + 1) < GRAPH_MENU_ITEMS && MENU_MAX_POSSIBLE_ITEMS > 0){
						if(Selected.PopUpMenu.Graph.Current - Selected.PopUpMenu.Graph.First < MENU_MAX_POSSIBLE_ITEMS - 1){
							//	Some special conditions may happen during resizing.
							HighlightXYXLine(UNDO,GRAPH_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,GRAPH_MENU_WIDTH-1,Selected.PopUpMenu.Graph.Current - Selected.PopUpMenu.Graph.First);
							++Selected.PopUpMenu.Graph.Current;
							HighlightXYXLine(DO,GRAPH_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,GRAPH_MENU_WIDTH-1,Selected.PopUpMenu.Graph.Current - Selected.PopUpMenu.Graph.First);
						}else{
							//	Special Conditions.
							++Selected.PopUpMenu.Graph.Current;
							CLS_INSIDE_GRAPH_MENU;
							++Selected.PopUpMenu.Graph.First;
							ListCurrentPopUpMenuItems(0,Selected.PopUpMenu.Graph.First,MENU_MAX_POSSIBLE_ITEMS);
							HighlightXYXLine(DO,GRAPH_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,GRAPH_MENU_WIDTH-1,MENU_MAX_POSSIBLE_ITEMS-1);
						}
					}
					break;
		}
		break;
	case GENERAL:
		switch(Direction){
			case UP:
				//	Scrolling Up highlight options.
					// Normal Conditions.
				if((Selected.Process.Current - 1) >= 0  && MAX_POSSIBLE_LINES > 0 && !InMenu){
					if(Selected.Process.Current - Selected.Process.First > 0){
						HighlightXYXLine(UNDO,LIST_START_X,LIST_START_Y,LIST_WIDTH,Selected.Process.Current - Selected.Process.First);
						--Selected.Process.Current;
						HighlightXYXLine(DO,LIST_START_X,LIST_START_Y,LIST_WIDTH,Selected.Process.Current - Selected.Process.First);
					}else{
						// Special Conditions.
						CLS_BELOW_TAB;
						--Selected.Process.First;
						ListProcesses(0,Selected.Process.First,MAX_POSSIBLE_LINES);
						--Selected.Process.Current;
						HighlightXYXLine(DO,LIST_START_X,LIST_START_Y,LIST_WIDTH,0);
					}
					//	Setting selected options to zero for the new selected process.
					Selected.Module.First = Selected.Module.Current = 0;
					Selected.Thread.First = Selected.Thread.Current = 0;
					Selected.Detail.First = 0;
				}
				//	If its menu was open.
				else if(InMenu)
					if(!InSetPriorityMenu && (Selected.PopUpMenu.General.Current - 1) >= 0  && MENU_MAX_POSSIBLE_ITEMS > 0){
						if(Selected.PopUpMenu.General.Current - Selected.PopUpMenu.General.First > 0){
							//	Some special conditions may happen during resizing.
							HighlightXYXLine(UNDO,GENERAL_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,GENERAL_MENU_WIDTH-1,Selected.PopUpMenu.General.Current - Selected.PopUpMenu.General.First);
							--Selected.PopUpMenu.General.Current;
							HighlightXYXLine(DO,GENERAL_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,GENERAL_MENU_WIDTH-1,Selected.PopUpMenu.General.Current - Selected.PopUpMenu.General.First);
						}else{
							//	Special Conditions.
							CLS_INSIDE_GENERAL_MENU;
							--Selected.PopUpMenu.General.First;
							ListCurrentPopUpMenuItems(0,Selected.PopUpMenu.General.First,MENU_MAX_POSSIBLE_ITEMS);
							--Selected.PopUpMenu.General.Current;
							HighlightXYXLine(DO,GENERAL_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,GENERAL_MENU_WIDTH-1,0);
						}
					}
					else if(InSetPriorityMenu && (Selected.PopUpMenu.SetPriority.Process.Current - 1) >= 0  && MENU_MAX_POSSIBLE_ITEMS > 0){
						if(Selected.PopUpMenu.SetPriority.Process.Current - Selected.PopUpMenu.SetPriority.Process.First > 0){
							//	Some special conditions may happen during resizing.
							HighlightXYXLine(UNDO,GENERAL_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,GENERAL_MENU_WIDTH-1,Selected.PopUpMenu.SetPriority.Process.Current - Selected.PopUpMenu.SetPriority.Process.First);
							--Selected.PopUpMenu.SetPriority.Process.Current;
							HighlightXYXLine(DO,GENERAL_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,GENERAL_MENU_WIDTH-1,Selected.PopUpMenu.SetPriority.Process.Current - Selected.PopUpMenu.SetPriority.Process.First);
						}else{
							//	Special Conditions.
							CLS_INSIDE_GENERAL_MENU;
							--Selected.PopUpMenu.SetPriority.Process.First;
							ListCurrentPopUpMenuItems(0,Selected.PopUpMenu.SetPriority.Process.First,MENU_MAX_POSSIBLE_ITEMS);
							--Selected.PopUpMenu.SetPriority.Process.Current;
							HighlightXYXLine(DO,GENERAL_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,GENERAL_MENU_WIDTH-1,0);
						}
					}
				break;
			case DOWN:
				//	Scrolling Down highlight options.
					//	Normal Conditions.
				if(Selected.Process.Current + 1 < CoreProcessCount && MAX_POSSIBLE_LINES > 0 && !InMenu){
					if(Selected.Process.Current - Selected.Process.First < MAX_POSSIBLE_LINES - 1){
						HighlightXYXLine(UNDO,LIST_START_X,LIST_START_Y,LIST_WIDTH,Selected.Process.Current - Selected.Process.First);
						++Selected.Process.Current;
						HighlightXYXLine(DO,LIST_START_X,LIST_START_Y,LIST_WIDTH,Selected.Process.Current - Selected.Process.First);
					}else{
						//	Special Conditions.
						++Selected.Process.Current;
						CLS_BELOW_TAB;
						++Selected.Process.First;
						ListProcesses(0,Selected.Process.First,MAX_POSSIBLE_LINES);
						HighlightXYXLine(DO,LIST_START_X,LIST_START_Y,LIST_WIDTH,MAX_POSSIBLE_LINES-1);
					}
					//	Setting selected options to zero for the new selected process.
					Selected.Module.First = Selected.Module.Current = 0;
					Selected.Thread.First = Selected.Thread.Current = 0;
					Selected.Detail.First = 0;
				}
				//	If its menu was open.
				else if(InMenu)
					if(!InSetPriorityMenu && (Selected.PopUpMenu.General.Current + 1) < GENERAL_MENU_ITEMS && MENU_MAX_POSSIBLE_ITEMS > 0){
						if(Selected.PopUpMenu.General.Current - Selected.PopUpMenu.General.First < MENU_MAX_POSSIBLE_ITEMS - 1){
							//	Some special conditions may happen during resizing.
							HighlightXYXLine(UNDO,GENERAL_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,GENERAL_MENU_WIDTH-1,Selected.PopUpMenu.General.Current - Selected.PopUpMenu.General.First);
							++Selected.PopUpMenu.General.Current;
							HighlightXYXLine(DO,GENERAL_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,GENERAL_MENU_WIDTH-1,Selected.PopUpMenu.General.Current - Selected.PopUpMenu.General.First);
						}else{
							//	Special Conditions.
							++Selected.PopUpMenu.General.Current;
							CLS_INSIDE_GENERAL_MENU;
							++Selected.PopUpMenu.General.First;
							ListCurrentPopUpMenuItems(0,Selected.PopUpMenu.General.First,MENU_MAX_POSSIBLE_ITEMS);
							HighlightXYXLine(DO,GENERAL_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,GENERAL_MENU_WIDTH-1,MENU_MAX_POSSIBLE_ITEMS-1);
						}
					}
					else if(InSetPriorityMenu && (Selected.PopUpMenu.SetPriority.Process.Current + 1) < PROCESS_SET_PRIORITY_MENU_ITEMS && MENU_MAX_POSSIBLE_ITEMS > 0){
						if(Selected.PopUpMenu.SetPriority.Process.Current - Selected.PopUpMenu.SetPriority.Process.First < MENU_MAX_POSSIBLE_ITEMS - 1){
							//	Some special conditions may happen during resizing.
							HighlightXYXLine(UNDO,GENERAL_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,GENERAL_MENU_WIDTH-1,Selected.PopUpMenu.SetPriority.Process.Current - Selected.PopUpMenu.SetPriority.Process.First);
							++Selected.PopUpMenu.SetPriority.Process.Current;
							HighlightXYXLine(DO,GENERAL_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,GENERAL_MENU_WIDTH-1,Selected.PopUpMenu.SetPriority.Process.Current - Selected.PopUpMenu.SetPriority.Process.First);
						}else{
							//	Special Conditions.
							++Selected.PopUpMenu.SetPriority.Process.Current;
							CLS_INSIDE_GENERAL_MENU;
							++Selected.PopUpMenu.SetPriority.Process.First;
							ListCurrentPopUpMenuItems(0,Selected.PopUpMenu.SetPriority.Process.First,MENU_MAX_POSSIBLE_ITEMS);
							HighlightXYXLine(DO,GENERAL_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,GENERAL_MENU_WIDTH-1,MENU_MAX_POSSIBLE_ITEMS-1);
						}
					}
					break;
		}
		break;
	case THREADS:
		switch(Direction){
			case UP:
				//	Scrolling Up highlight options.
					// Normal Conditions.
				if((Selected.Thread.Current - 1) >= 0 && MAX_POSSIBLE_LINES > 0 && !InMenu)
					if(Selected.Thread.Current - Selected.Thread.First > 0){
						HighlightXYXLine(UNDO,LIST_START_X,LIST_START_Y,LIST_WIDTH,Selected.Thread.Current - Selected.Thread.First);
						--Selected.Thread.Current;
						HighlightXYXLine(DO,LIST_START_X,LIST_START_Y,LIST_WIDTH,Selected.Thread.Current - Selected.Thread.First);
					}else{
						// Special Conditions.
						CLS_BELOW_TAB;
						--Selected.Thread.First;
						ListThreads(0 , Selected.Thread.First , MAX_POSSIBLE_LINES);
						--Selected.Thread.Current;
						HighlightXYXLine(DO,LIST_START_X,LIST_START_Y,LIST_WIDTH,0);
					}
					//	If its menu was open.
				else if(InMenu)
					if(!InSetPriorityMenu && (Selected.PopUpMenu.Threads.Current - 1) >= 0  && MENU_MAX_POSSIBLE_ITEMS > 0){
						if(Selected.PopUpMenu.Threads.Current - Selected.PopUpMenu.Threads.First > 0){
							//	Some special conditions may happen during resizing.
							HighlightXYXLine(UNDO,THREADS_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,THREADS_MENU_WIDTH-1,Selected.PopUpMenu.Threads.Current - Selected.PopUpMenu.Threads.First);
							--Selected.PopUpMenu.Threads.Current;
							HighlightXYXLine(DO,THREADS_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,THREADS_MENU_WIDTH-1,Selected.PopUpMenu.Threads.Current - Selected.PopUpMenu.Threads.First);
						}else{
							//	Special Conditions.
							CLS_INSIDE_THREADS_MENU;
							--Selected.PopUpMenu.Threads.First;
							ListCurrentPopUpMenuItems(0,Selected.PopUpMenu.Threads.First,MENU_MAX_POSSIBLE_ITEMS);
							--Selected.PopUpMenu.Threads.Current;
							HighlightXYXLine(DO,THREADS_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,THREADS_MENU_WIDTH-1,0);
						}
					}
					else if(InSetPriorityMenu && (Selected.PopUpMenu.SetPriority.Thread.Current - 1) >= 0  && MENU_MAX_POSSIBLE_ITEMS > 0){
						if(Selected.PopUpMenu.SetPriority.Thread.Current - Selected.PopUpMenu.SetPriority.Thread.First > 0){
							//	Some special conditions may happen during resizing.
							HighlightXYXLine(UNDO,THREADS_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,THREADS_MENU_WIDTH-1,Selected.PopUpMenu.SetPriority.Thread.Current - Selected.PopUpMenu.SetPriority.Thread.First);
							--Selected.PopUpMenu.SetPriority.Thread.Current;
							HighlightXYXLine(DO,THREADS_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,THREADS_MENU_WIDTH-1,Selected.PopUpMenu.SetPriority.Thread.Current - Selected.PopUpMenu.SetPriority.Thread.First);
						}else{
							//	Special Conditions.
							CLS_INSIDE_THREADS_MENU;
							--Selected.PopUpMenu.SetPriority.Thread.First;
							ListCurrentPopUpMenuItems(0,Selected.PopUpMenu.SetPriority.Thread.First,MENU_MAX_POSSIBLE_ITEMS);
							--Selected.PopUpMenu.SetPriority.Thread.Current;
							HighlightXYXLine(DO,THREADS_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,THREADS_MENU_WIDTH-1,0);
						}
					}
				break;
			case DOWN:
				//	Scrolling Down highlight options.
					//	Normal Conditions.
				if(Selected.Thread.Current + 1 < PeInfo[Selected.Process.Current].ThreadCount && MAX_POSSIBLE_LINES > 0 && !InMenu)
					if(Selected.Thread.Current - Selected.Thread.First < MAX_POSSIBLE_LINES - 1){
						//	Some special conditions may happen during resizing.
						HighlightXYXLine(UNDO,LIST_START_X,LIST_START_Y,LIST_WIDTH,Selected.Thread.Current - Selected.Thread.First);
						++Selected.Thread.Current;
						HighlightXYXLine(DO,LIST_START_X,LIST_START_Y,LIST_WIDTH,Selected.Thread.Current - Selected.Thread.First);
					}else{
						//	Special Conditions.
						++Selected.Thread.Current;
						CLS_BELOW_TAB;
						++Selected.Thread.First;
						ListThreads(0,Selected.Thread.First,MAX_POSSIBLE_LINES);
						HighlightXYXLine(DO,LIST_START_X,LIST_START_Y,LIST_WIDTH,MAX_POSSIBLE_LINES-1);
					}
				//	If its menu was open.
				else if(InMenu)
					if(!InSetPriorityMenu && (Selected.PopUpMenu.Threads.Current + 1) < THREADS_MENU_ITEMS && MENU_MAX_POSSIBLE_ITEMS > 0){
						if(Selected.PopUpMenu.Threads.Current - Selected.PopUpMenu.Threads.First < MENU_MAX_POSSIBLE_ITEMS - 1){
							//	Some special conditions may happen during resizing.
							HighlightXYXLine(UNDO,THREADS_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,THREADS_MENU_WIDTH-1,Selected.PopUpMenu.Threads.Current - Selected.PopUpMenu.Threads.First);
							++Selected.PopUpMenu.Threads.Current;
							HighlightXYXLine(DO,THREADS_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,THREADS_MENU_WIDTH-1,Selected.PopUpMenu.Threads.Current - Selected.PopUpMenu.Threads.First);
						}else{
							//	Special Conditions.
							++Selected.PopUpMenu.Threads.Current;
							CLS_INSIDE_THREADS_MENU;
							++Selected.PopUpMenu.Threads.First;
							ListCurrentPopUpMenuItems(0,Selected.PopUpMenu.Threads.First,MENU_MAX_POSSIBLE_ITEMS);
							HighlightXYXLine(DO,THREADS_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,THREADS_MENU_WIDTH-1,MENU_MAX_POSSIBLE_ITEMS-1);
						}
					}
					else if(InSetPriorityMenu && (Selected.PopUpMenu.SetPriority.Thread.Current + 1) < THREAD_SET_PRIORITY_MENU_ITEMS && MENU_MAX_POSSIBLE_ITEMS > 0){
						if(Selected.PopUpMenu.SetPriority.Thread.Current - Selected.PopUpMenu.SetPriority.Thread.First < MENU_MAX_POSSIBLE_ITEMS - 1){
							//	Some special conditions may happen during resizing.
							HighlightXYXLine(UNDO,THREADS_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,THREADS_MENU_WIDTH-1,Selected.PopUpMenu.SetPriority.Thread.Current - Selected.PopUpMenu.SetPriority.Thread.First);
							++Selected.PopUpMenu.SetPriority.Thread.Current;
							HighlightXYXLine(DO,THREADS_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,THREADS_MENU_WIDTH-1,Selected.PopUpMenu.SetPriority.Thread.Current - Selected.PopUpMenu.SetPriority.Thread.First);
						}else{
							//	Special Conditions.
							++Selected.PopUpMenu.SetPriority.Thread.Current;
							CLS_INSIDE_THREADS_MENU;
							++Selected.PopUpMenu.SetPriority.Thread.First;
							ListCurrentPopUpMenuItems(0,Selected.PopUpMenu.SetPriority.Thread.First,MENU_MAX_POSSIBLE_ITEMS);
							HighlightXYXLine(DO,THREADS_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,THREADS_MENU_WIDTH-1,MENU_MAX_POSSIBLE_ITEMS-1);
						}
					}
					break;
		}
		break;
	case MODULES:
		switch(Direction){
			case UP:
				//	Scrolling Up highlight options.
					// Normal Conditions.
				if((Selected.Module.Current - 1) >= 0 && MAX_POSSIBLE_LINES > 0 && !InMenu)
					if(Selected.Module.Current - Selected.Module.First > 0){
						HighlightXYXLine(UNDO,LIST_START_X,LIST_START_Y,LIST_WIDTH,Selected.Module.Current - Selected.Module.First);
						--Selected.Module.Current;
						HighlightXYXLine(DO,LIST_START_X,LIST_START_Y,LIST_WIDTH,Selected.Module.Current - Selected.Module.First);
					}else{
						// Special Conditions.
						CLS_BELOW_TAB;
						--Selected.Module.First;
						ListModules(0,Selected.Module.First,MAX_POSSIBLE_LINES);
						--Selected.Module.Current;
						HighlightXYXLine(DO,LIST_START_X,LIST_START_Y,LIST_WIDTH,0);
					}
				break;
			case DOWN:
				//	Scrolling Down highlight options.
					//	Normal Conditions.
				if(Selected.Module.Current + 1 < PeInfo[Selected.Process.Current].ModuleCount && MAX_POSSIBLE_LINES > 0 && !InMenu)
					if(Selected.Module.Current - Selected.Module.First < MAX_POSSIBLE_LINES - 1){
						HighlightXYXLine(UNDO,LIST_START_X,LIST_START_Y,LIST_WIDTH,Selected.Module.Current - Selected.Module.First);
						++Selected.Module.Current;
						HighlightXYXLine(DO,LIST_START_X,LIST_START_Y,LIST_WIDTH,Selected.Module.Current - Selected.Module.First);
					}else{
						//	Special Conditions.
						++Selected.Module.Current;
						CLS_BELOW_TAB;
						++Selected.Module.First;
						ListModules(0,Selected.Module.First,MAX_POSSIBLE_LINES);
						HighlightXYXLine(DO,LIST_START_X,LIST_START_Y,LIST_WIDTH,MAX_POSSIBLE_LINES-1);
					}
				break;
		}
		break;
	}
	ReleaseMutex(hScreenMutex);
}

/*	Lists selected menu's items.	*/
VOID ListCurrentPopUpMenuItems(INT StartLine,INT FirstItem,INT Count){
	INT i;
	WaitForSingleObject(hScreenMutex,INFINITE);
	switch(Selected.CurrentTab){
	case GENERAL:
		if(!InSetPriorityMenu){
			for(i=0 ; FirstItem < GENERAL_MENU_ITEMS &&  i < Count && i < MENU_MAX_POSSIBLE_ITEMS; ++i,++StartLine,++FirstItem)
				PrintXY(PopUpMenuItems.General.Items[FirstItem],MID_TEXT_START_X(GENERAL_MENU_LEFT_START_X, GENERAL_MENU_WIDTH ,wcslen(PopUpMenuItems.General.Items[i])), MENU_FIRST_ITEM_FROM_ABOVE_Y  + StartLine);
		}
		else if(InSetPriorityMenu){
			for(i=0 ; FirstItem < PROCESS_SET_PRIORITY_MENU_ITEMS &&  i < Count && i < MENU_MAX_POSSIBLE_ITEMS; ++i,++StartLine,++FirstItem)
				PrintXY(PopUpMenuItems.SetPriority.SetProcessPriority.Items[FirstItem],MID_TEXT_START_X(THREADS_MENU_LEFT_START_X, THREADS_MENU_WIDTH ,wcslen(PopUpMenuItems.SetPriority.SetProcessPriority.Items[i])), MENU_FIRST_ITEM_FROM_ABOVE_Y  + StartLine);
		}
		break;
	case THREADS:
		if(!InSetPriorityMenu){
			for(i=0 ; FirstItem < THREADS_MENU_ITEMS &&  i < Count && i < MENU_MAX_POSSIBLE_ITEMS; ++i,++StartLine,++FirstItem)
				PrintXY(PopUpMenuItems.Threads.Items[FirstItem],MID_TEXT_START_X(THREADS_MENU_LEFT_START_X, THREADS_MENU_WIDTH ,wcslen(PopUpMenuItems.Threads.Items[i])), MENU_FIRST_ITEM_FROM_ABOVE_Y  + StartLine);
		}
		else if(InSetPriorityMenu){
			for(i=0 ; FirstItem < THREAD_SET_PRIORITY_MENU_ITEMS &&  i < Count && i < MENU_MAX_POSSIBLE_ITEMS; ++i,++StartLine,++FirstItem)
				PrintXY(PopUpMenuItems.SetPriority.SetThreadPriority.Items[FirstItem],MID_TEXT_START_X(THREADS_MENU_LEFT_START_X, THREADS_MENU_WIDTH ,wcslen(PopUpMenuItems.SetPriority.SetThreadPriority.Items[i])), MENU_FIRST_ITEM_FROM_ABOVE_Y  + StartLine);
		}
		break;
	case GRAPH:
		for(i=0 ; FirstItem < GRAPH_MENU_ITEMS &&  i < Count && i < MENU_MAX_POSSIBLE_ITEMS; ++i,++StartLine,++FirstItem)
			PrintXY(PopUpMenuItems.Graph.Items[FirstItem],MID_TEXT_START_X(GRAPH_MENU_LEFT_START_X, GRAPH_MENU_WIDTH ,wcslen(PopUpMenuItems.Graph.Items[i])), MENU_FIRST_ITEM_FROM_ABOVE_Y  + StartLine);
		break;
	}
	ReleaseMutex(hScreenMutex);
}

/*	Cleaning the menu square.	*/
VOID CloseCurrentPopUpMenu(VOID){
	INT i;
	WaitForSingleObject(hScreenMutex,INFINITE);
	switch(Selected.CurrentTab){
	case GENERAL:
		ClsXYXCount(GENERAL_MENU_LEFT_START_X , MENU_DOWN_START_Y - MENU_HEIGHT , GENERAL_MENU_WIDTH + 1 , MENU_HEIGHT);
		for(i = GENERAL_MENU_LEFT_START_X ; i <= GENERAL_MENU_WIDTH + GENERAL_MENU_LEFT_START_X; i++)
			PrintXY(BOARDER_LINE,i,MENU_DOWN_START_Y);
		if(!InSetPriorityMenu){
			HighlightXYXLine(UNDO,GENERAL_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,GENERAL_MENU_WIDTH-1,Selected.PopUpMenu.General.Current - Selected.PopUpMenu.General.First);
			Selected.PopUpMenu.General.First = Selected.PopUpMenu.General.Current = 0;
		}
		else if(InSetPriorityMenu){
			HighlightXYXLine(UNDO,GENERAL_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,GENERAL_MENU_WIDTH-1,Selected.PopUpMenu.SetPriority.Process.Current - Selected.PopUpMenu.SetPriority.Process.First);
			Selected.PopUpMenu.SetPriority.Process.First = Selected.PopUpMenu.SetPriority.Process.Current = 0;
		}
		break;
	case THREADS:
		ClsXYXCount(THREADS_MENU_LEFT_START_X , MENU_DOWN_START_Y - MENU_HEIGHT , THREADS_MENU_WIDTH + 1 , MENU_HEIGHT);
		for(i = THREADS_MENU_LEFT_START_X ; i <= THREADS_MENU_WIDTH + THREADS_MENU_LEFT_START_X; i++)
			PrintXY(BOARDER_LINE,i,MENU_DOWN_START_Y);
		if(!InSetPriorityMenu){
			HighlightXYXLine(UNDO,THREADS_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,THREADS_MENU_WIDTH-1,Selected.PopUpMenu.Threads.Current - Selected.PopUpMenu.Threads.First);
			Selected.PopUpMenu.Threads.First = Selected.PopUpMenu.Threads.Current = 0;
		}
		else if(InSetPriorityMenu){
			HighlightXYXLine(UNDO,THREADS_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,THREADS_MENU_WIDTH-1,Selected.PopUpMenu.SetPriority.Thread.Current - Selected.PopUpMenu.SetPriority.Thread.First);
			Selected.PopUpMenu.SetPriority.Thread.First = Selected.PopUpMenu.SetPriority.Thread.Current = 0;
		}
		break;
	case GRAPH:
		ClsXYXCount(GRAPH_MENU_LEFT_START_X , MENU_DOWN_START_Y - MENU_HEIGHT , GRAPH_MENU_WIDTH + 1 , MENU_HEIGHT);
		for(i = GRAPH_MENU_LEFT_START_X ; i <= GRAPH_MENU_WIDTH + GRAPH_MENU_LEFT_START_X; i++)
			PrintXY(BOARDER_LINE,i,MENU_DOWN_START_Y);
		HighlightXYXLine(UNDO,GRAPH_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,GRAPH_MENU_WIDTH-1,Selected.PopUpMenu.Graph.Current - Selected.PopUpMenu.Graph.First);
		Selected.PopUpMenu.Graph.First = Selected.PopUpMenu.Graph.Current = 0;
		break;
	}
	if(!InSetPriorityMenu)
		InMenu=FALSE;
	else if(InSetPriorityMenu){
		InSetPriorityMenu=FALSE;
		InMenu=FALSE;
	}
	FillCurrentTabInformationAndSelectItem();
	ReleaseMutex(hScreenMutex);
}

/*	Opens a menu shows messages.	*/
VOID DrawCurrentTabPopUpMenu(BOOL FillMenuInfo){
	switch(Selected.CurrentTab){
	case GENERAL:
	case THREADS:
	case GRAPH:
		InMenu = TRUE;
	}
	WaitForSingleObject(hScreenMutex,INFINITE);
	DrawCurrentTabPopUpMenuFrameAndLabels();
	if(FillMenuInfo)
		//	Fills current tab PopUpMenu information.
		FillCurrentTabPopUpMenuInformationAndSelectFirstItem();
	ReleaseMutex(hScreenMutex);
}

/*	Writes current tab PopUp Menu frame and labels.	*/
VOID DrawCurrentTabPopUpMenuFrameAndLabels(VOID){
	INT i;
	WaitForSingleObject(hScreenMutex,INFINITE);
	switch(Selected.CurrentTab){
	case GRAPH:
		//	Cleaning the menu square and below it.
		CLS_GRAPH_MENU;
		PaintRegion(MENU_DOWN_START_Y - MENU_HEIGHT,MENU_DOWN_START_Y-1,GRAPH_MENU_LEFT_START_X,GRAPH_MENU_LEFT_START_X+GRAPH_MENU_WIDTH ,REGULAR_COLOR);
		//	Vertical Lines.
		for(i=MENU_DOWN_START_Y; i > MENU_DOWN_START_Y - MENU_HEIGHT;--i){
			PrintXY(VERTICAL_TAB_LINE , GRAPH_MENU_LEFT_START_X , i);
			PrintXY(VERTICAL_TAB_LINE , GRAPH_MENU_LEFT_START_X + GRAPH_MENU_WIDTH , i);
		}
		//	Horizontal Line.
		for(i=GRAPH_MENU_LEFT_START_X + 1;i < GRAPH_MENU_WIDTH + GRAPH_MENU_LEFT_START_X;++i)
			PrintXY(HORIZONTAL_TAB_LINE,i,MENU_DOWN_START_Y - MENU_HEIGHT);
		//	Writing menu's head label : Thread ID.
		PrintXY(GRAPH_MENU_HEAD , MID_TEXT_START_X(GRAPH_MENU_LEFT_START_X, GRAPH_MENU_WIDTH , GRAPH_MENU_HEAD_LEN), MENU_DOWN_START_Y - MENU_HEIGHT + 1);
		//	The Line under menu's head.
		for(i=GRAPH_MENU_LEFT_START_X + 1 ; i < GRAPH_MENU_WIDTH + GRAPH_MENU_LEFT_START_X ; ++i)
			PrintXY(HORIZONTAL_TAB_LINE,i,MENU_DOWN_START_Y - MENU_HEIGHT + MENU_HEAD_VERTICAL_SPACE - 1);
		break;
	case GENERAL:
		HighlightXYXLine(UNDO,LIST_START_X,LIST_START_Y,LIST_WIDTH,Selected.Process.Current - Selected.Process.First);
		//	Cleaning the menu square and below it.
		CLS_GENERAL_MENU;
		//	Vertical Lines.
		for(i=MENU_DOWN_START_Y; i > MENU_DOWN_START_Y - MENU_HEIGHT;--i){
			PrintXY(VERTICAL_TAB_LINE , GENERAL_MENU_LEFT_START_X , i);
			PrintXY(VERTICAL_TAB_LINE , GENERAL_MENU_LEFT_START_X + GENERAL_MENU_WIDTH , i);
		}
		//	Horizontal Line.
		for(i=GENERAL_MENU_LEFT_START_X + 1;i < GENERAL_MENU_WIDTH + GENERAL_MENU_LEFT_START_X;++i)
			PrintXY(HORIZONTAL_TAB_LINE,i,MENU_DOWN_START_Y - MENU_HEIGHT);
		//	Writing menu's head label : Thread ID.
		PrintXY(GENERAL_MENU_HEAD , MID_TEXT_START_X(GENERAL_MENU_LEFT_START_X, GENERAL_MENU_WIDTH , GENERAL_MENU_HEAD_LEN), MENU_DOWN_START_Y - MENU_HEIGHT + 1);
		NPrintXY(PeInfo[Selected.Process.Current].PID,MID_TEXT_START_X(GENERAL_MENU_LEFT_START_X, GENERAL_MENU_WIDTH , GENERAL_MENU_HEAD_LEN) + GENERAL_MENU_HEAD_LEN,MENU_DOWN_START_Y - MENU_HEIGHT + 1);
		//	The Line under menu's head.
		for(i=GENERAL_MENU_LEFT_START_X + 1 ; i < GENERAL_MENU_WIDTH + GENERAL_MENU_LEFT_START_X ; ++i)
			PrintXY(HORIZONTAL_TAB_LINE,i,MENU_DOWN_START_Y - MENU_HEIGHT + MENU_HEAD_VERTICAL_SPACE - 1);
		break;
	case THREADS:
		HighlightXYXLine(UNDO,LIST_START_X,LIST_START_Y,LIST_WIDTH,Selected.Thread.Current - Selected.Thread.First);
		//	Cleaning the menu square and below it.
		CLS_THREADS_MENU;
		//	Vertical Lines.
		for(i=MENU_DOWN_START_Y; i > MENU_DOWN_START_Y - MENU_HEIGHT;--i){
			PrintXY(VERTICAL_TAB_LINE , THREADS_MENU_LEFT_START_X , i);
			PrintXY(VERTICAL_TAB_LINE , THREADS_MENU_LEFT_START_X + THREADS_MENU_WIDTH , i);
		}
		//	Horizontal Line.
		for(i=THREADS_MENU_LEFT_START_X + 1;i < THREADS_MENU_WIDTH + THREADS_MENU_LEFT_START_X;++i)
			PrintXY(HORIZONTAL_TAB_LINE,i,MENU_DOWN_START_Y - MENU_HEIGHT);
		//	Writing menu's head label : Thread ID.
		PrintXY(THREAD_MENU_HEAD , MID_TEXT_START_X(THREADS_MENU_LEFT_START_X, THREADS_MENU_WIDTH , THREAD_MENU_HEAD_LEN), MENU_DOWN_START_Y - MENU_HEIGHT + 1);
		NPrintXY(PeInfo[Selected.Process.Current].Threads[Selected.Thread.Current].dwTID,MID_TEXT_START_X(THREADS_MENU_LEFT_START_X, THREADS_MENU_WIDTH , THREAD_MENU_HEAD_LEN) + THREAD_MENU_HEAD_LEN,MENU_DOWN_START_Y - MENU_HEIGHT + 1);
		//	The Line under menu's head.
		for(i=THREADS_MENU_LEFT_START_X + 1 ; i < THREADS_MENU_WIDTH + THREADS_MENU_LEFT_START_X ; ++i)
			PrintXY(HORIZONTAL_TAB_LINE,i,MENU_DOWN_START_Y - MENU_HEIGHT + MENU_HEAD_VERTICAL_SPACE - 1);
		break;
	}
	ReleaseMutex(hScreenMutex);
}

/*	Fills current tab PopUpMenu information.	*/
VOID FillCurrentTabPopUpMenuInformationAndSelectFirstItem(VOID){
	WaitForSingleObject(hScreenMutex,INFINITE);
	switch(Selected.CurrentTab){
	case GENERAL:
		if(!InSetPriorityMenu){
			//	Lists menu items.
			((Selected.PopUpMenu.General.Current - Selected.PopUpMenu.General.First) + 1 > MENU_MAX_POSSIBLE_ITEMS) 
				? ListCurrentPopUpMenuItems(0,(Selected.PopUpMenu.General.First = Selected.PopUpMenu.General.Current - MENU_MAX_POSSIBLE_ITEMS + 1),MENU_MAX_POSSIBLE_ITEMS)
				: (ListCurrentPopUpMenuItems(0,Selected.PopUpMenu.General.First ,MENU_MAX_POSSIBLE_ITEMS));

			if(Selected.PopUpMenu.General.Current < Selected.PopUpMenu.General.First)
				Selected.PopUpMenu.General.First = Selected.PopUpMenu.General.Current;
			//	Selecting first menu.
			if(MENU_MAX_POSSIBLE_ITEMS > 0 && GENERAL_MENU_ITEMS != 0)
				HighlightXYXLine(DO,GENERAL_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,GENERAL_MENU_WIDTH-1,Selected.PopUpMenu.General.Current - Selected.PopUpMenu.General.First);
		}
		else if(InSetPriorityMenu){
			//	Lists menu items.
			((Selected.PopUpMenu.SetPriority.Process.Current - Selected.PopUpMenu.SetPriority.Process.First) + 1 > MENU_MAX_POSSIBLE_ITEMS)
				? ListCurrentPopUpMenuItems(0,(Selected.PopUpMenu.SetPriority.Process.First = Selected.PopUpMenu.SetPriority.Process.Current - MENU_MAX_POSSIBLE_ITEMS + 1),MENU_MAX_POSSIBLE_ITEMS)
				: (ListCurrentPopUpMenuItems(0,Selected.PopUpMenu.SetPriority.Process.First ,MENU_MAX_POSSIBLE_ITEMS));

			if(Selected.PopUpMenu.SetPriority.Process.Current < Selected.PopUpMenu.SetPriority.Process.First)
				Selected.PopUpMenu.SetPriority.Process.First = Selected.PopUpMenu.SetPriority.Process.Current;
			//	Selecting first menu.
			if(MENU_MAX_POSSIBLE_ITEMS > 0 && PROCESS_SET_PRIORITY_MENU_ITEMS != 0)
				HighlightXYXLine(DO,GENERAL_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,GENERAL_MENU_WIDTH-1,Selected.PopUpMenu.SetPriority.Process.Current - Selected.PopUpMenu.SetPriority.Process.First);
		}
		break;
	case DETAILS:
		//	Empty.
		break;
	case MODULES:
		//	Empty.
		break;
	case THREADS:
		if(!InSetPriorityMenu){
			//	Lists menu items.
			((Selected.PopUpMenu.Threads.Current - Selected.PopUpMenu.Threads.First) + 1 > MENU_MAX_POSSIBLE_ITEMS)
				? ListCurrentPopUpMenuItems(0,(Selected.PopUpMenu.Threads.First = Selected.PopUpMenu.Threads.Current - MENU_MAX_POSSIBLE_ITEMS + 1),MENU_MAX_POSSIBLE_ITEMS)
				: (ListCurrentPopUpMenuItems(0,Selected.PopUpMenu.Threads.First ,MENU_MAX_POSSIBLE_ITEMS));

			if(Selected.PopUpMenu.Threads.Current < Selected.PopUpMenu.Threads.First)
				Selected.PopUpMenu.Threads.First = Selected.PopUpMenu.Threads.Current;
			//	Selecting first menu.
			if(MENU_MAX_POSSIBLE_ITEMS > 0 && THREADS_MENU_ITEMS != 0)
				HighlightXYXLine(DO,THREADS_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,THREADS_MENU_WIDTH-1,Selected.PopUpMenu.Threads.Current - Selected.PopUpMenu.Threads.First);
		}
		else if(InSetPriorityMenu){
			//	Lists menu items.
			((Selected.PopUpMenu.SetPriority.Thread.Current - Selected.PopUpMenu.SetPriority.Thread.First) + 1 > MENU_MAX_POSSIBLE_ITEMS)
				? ListCurrentPopUpMenuItems(0,(Selected.PopUpMenu.SetPriority.Thread.First = Selected.PopUpMenu.SetPriority.Thread.Current - MENU_MAX_POSSIBLE_ITEMS + 1),MENU_MAX_POSSIBLE_ITEMS)
				: (ListCurrentPopUpMenuItems(0,Selected.PopUpMenu.SetPriority.Thread.First ,MENU_MAX_POSSIBLE_ITEMS));

			if(Selected.PopUpMenu.SetPriority.Thread.Current < Selected.PopUpMenu.SetPriority.Thread.First)
				Selected.PopUpMenu.SetPriority.Thread.First = Selected.PopUpMenu.SetPriority.Thread.Current;
			//	Selecting first menu.
			if(MENU_MAX_POSSIBLE_ITEMS > 0 && THREAD_SET_PRIORITY_MENU_ITEMS != 0)
				HighlightXYXLine(DO,THREADS_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,THREADS_MENU_WIDTH-1,Selected.PopUpMenu.SetPriority.Thread.Current - Selected.PopUpMenu.SetPriority.Thread.First);
		}
		break;
	case GRAPH:
		//	Lists menu items.
		((Selected.PopUpMenu.Graph.Current - Selected.PopUpMenu.Graph.First) + 1 > MENU_MAX_POSSIBLE_ITEMS)
			? ListCurrentPopUpMenuItems(0,(Selected.PopUpMenu.Graph.First = Selected.PopUpMenu.Graph.Current - MENU_MAX_POSSIBLE_ITEMS + 1),MENU_MAX_POSSIBLE_ITEMS)
			: (ListCurrentPopUpMenuItems(0,Selected.PopUpMenu.Graph.First ,MENU_MAX_POSSIBLE_ITEMS));

		if(Selected.PopUpMenu.Graph.Current < Selected.PopUpMenu.Graph.First)
			Selected.PopUpMenu.Graph.First = Selected.PopUpMenu.Graph.Current;
		//	Selecting first menu.
		if(MENU_MAX_POSSIBLE_ITEMS > 0 && GRAPH_MENU_ITEMS != 0)
			HighlightXYXLine(DO,GRAPH_MENU_LEFT_START_X+1,MENU_FIRST_ITEM_FROM_ABOVE_Y,GRAPH_MENU_WIDTH-1,Selected.PopUpMenu.Graph.Current - Selected.PopUpMenu.Graph.First);
		break;
	default:
		break;
	}
	ReleaseMutex(hScreenMutex);
}

/*	Fills Status bar and System Up time information.	*/
VOID FillSedentaryInfo(VOID){
	//	SystemUpTime.
		//	Formated text.
	WCHAR SysUpTime[12];
	WaitForSingleObject(hScreenMutex,INFINITE);
	StringCchPrintf(SysUpTime,12,TEXT("%0.2d:%0.2d:%0.2d:%0.2d"),SystemCPU_UsageAndUpTimeInfo.Days,SystemCPU_UsageAndUpTimeInfo.Hours,SystemCPU_UsageAndUpTimeInfo.Minutes,SystemCPU_UsageAndUpTimeInfo.Seconds);
	PrintXY(SysUpTime,SYSTEM_UP_TIME_X,SYSTEM_UP_TIME_Y);
	/*	Status Bar.	*/
		//	CPU Usage.
	ClsXYXCount(CPU_USAGE + CPU_USAGE_LEN,WindowSize.Y-BOTTOM_MARGIN-STATUS_BAR_HEIGHT / 2,NUMBER_PRECISION,1);
	NPrintXY(SystemCPU_UsageAndUpTimeInfo.CPU_Usage,CPU_USAGE + CPU_USAGE_LEN,WindowSize.Y-BOTTOM_MARGIN-STATUS_BAR_HEIGHT / 2);
		//	Commit Charge.
	ClsXYXCount((COMMIT_CHARGE + COMMIT_CHARGE_LEN + 1 * Labels_Distance),WindowSize.Y-BOTTOM_MARGIN-STATUS_BAR_HEIGHT / 2,NUMBER_PRECISION,1);
	NPrintXY((100*PerInfo.CommitTotal/PerInfo.CommitLimit),(COMMIT_CHARGE + COMMIT_CHARGE_LEN + 1 * Labels_Distance),WindowSize.Y-BOTTOM_MARGIN-STATUS_BAR_HEIGHT / 2);
		//	Process Count.
	
	ClsXYXCount((PROCESSES + PROCESSES_LEN + 2 * Labels_Distance),WindowSize.Y-BOTTOM_MARGIN-STATUS_BAR_HEIGHT / 2,NUMBER_PRECISION,1);
	NPrintXY(CoreProcessCount,(PROCESSES + PROCESSES_LEN + 2 * Labels_Distance),WindowSize.Y-BOTTOM_MARGIN-STATUS_BAR_HEIGHT / 2);
		//	Physical Usage.
	ClsXYXCount(PHYSICAL_USAGE + PHYSICAL_USAGE_LEN + 3 * Labels_Distance,WindowSize.Y-BOTTOM_MARGIN-STATUS_BAR_HEIGHT / 2,NUMBER_PRECISION,1);
	NPrintXY(SysMemory.dwMemoryLoad,PHYSICAL_USAGE + PHYSICAL_USAGE_LEN + 3 * Labels_Distance,WindowSize.Y-BOTTOM_MARGIN-STATUS_BAR_HEIGHT / 2);
	ReleaseMutex(hScreenMutex);
}