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
//	Build Version: 1.0			Note: Change this, if you did a new build.
#include <Windows.h>
#include <wchar.h>
#include "AnaCoreMainHeader.h"
#include "DrawTools.h"

//	Macros.
#define MID_TEXT_START_X(START,END,LEN)			(START+(((((START)+END)-(START))-LEN)/2))
#define MID_X									((WindowSize.X - LEFT_MARGIN - RIGHT_MARGIN)/2)
#define MID_Y(HEIGHT)							(LIST_START_Y + HEIGHT/2)

//	Min Window Size.
#define MIN_WINDOW_X_SIZE	107
#define	MIN_WINDOW_Y_SIZE	30

//	Max Lines defines.
	//	Evaluate max possible lines in the new size.
#define MAX_POSSIBLE_LINES					((WindowSize.Y - (FIRST_COLUMN_FROM_ABOVE + STATUS_BAR_HEIGHT + BOTTOM_MARGIN) - 1)>0?(WindowSize.Y - (FIRST_COLUMN_FROM_ABOVE + STATUS_BAR_HEIGHT + BOTTOM_MARGIN) - 1):(0))
#define DETAILS_MAX_POSSIBLE_LINES			((WindowSize.Y - (DETAILS_START_Y + STATUS_BAR_HEIGHT + BOTTOM_MARGIN) - 1)>0?((WindowSize.Y - (DETAILS_START_Y + STATUS_BAR_HEIGHT + BOTTOM_MARGIN) - 1)):(0))
#define SYSTEM_INFO_TAB_MAX_POSSIBLE_LINES	((WindowSize.Y - (SYSTEM_INFO_TAB_START_Y + STATUS_BAR_HEIGHT + BOTTOM_MARGIN) - 1)>0?((WindowSize.Y - (SYSTEM_INFO_TAB_START_Y + STATUS_BAR_HEIGHT + BOTTOM_MARGIN) - 1)):(0))

//	Plus the ':' character.
#define CPU_USAGE_LEN		11
#define COMMIT_CHARGE_LEN	15
#define PROCESSES_LEN		11
#define PHYSICAL_USAGE_LEN	16
//	Start (X) Position of labels.
	// 3 integer numbers plus percentage sign.
#define NUMBER_PRECISION	3
#define CPU_USAGE			(LEFT_MARGIN + LEFT_STATUS_BAR_DISTANCE)
#define COMMIT_CHARGE		(CPU_USAGE + CPU_USAGE_LEN + NUMBER_PRECISION)
#define PROCESSES			(COMMIT_CHARGE + COMMIT_CHARGE_LEN + NUMBER_PRECISION)
#define PHYSICAL_USAGE		(PROCESSES + PROCESSES_LEN + NUMBER_PRECISION)

//	Highlight Options.
#define DO		1
#define UNDO	0

//	Tab Names:
enum Tabs { GENERAL , DETAILS , MODULES , THREADS , GRAPH , SYSTEM_INFO_TAB };
//	Tabs menus items defines.
enum GeneralMenuDefines	{ KILL_PROCESS , KILL_PROCESS_TREE , SET_PROCESS_PRIORITY , SET_PROCESS_WORKING_SET_SIZE };
enum GraphMenuDefines { CPU_USAGE_ITEM , COMMIT_CHARGE_ITEM , PHYSICAL_USAGE_ITEM };
enum SetProcessPriorityMenu {PROC_IDLE,PROC_BELOW_NORMAL,PROC_NORMAL,PROC_ABOVE_NORMAL,PROC_HIGH,PROC_REAL_TIME};
enum ThreadsMenuDefines	{ RESUME_THREAD , SUSPEND_THREAD , SET_THREAD_PRIORITY };
enum SetThreadPriorityMenu {THREAD_LOWEST,THREAD_BELOW_NORMAL,THREAD_NORMAL,THREAD_ABOVE_NORMAL,THREAD_HIGHEST,THREAD_TIME_CRITICAL};

/*	Conditional Inclusions.	*/
#define TABS_UP_BOARDER_LINES FALSE

/*	Lines.	*/
#define VERTICAL_TAB_LINE	(TEXT("|"))
#define HORIZONTAL_TAB_LINE	(TEXT("-"))
#define BOARDER_LINE		(TEXT("_"))
#define BLANK				(TEXT(" "))

/*	Scroll Defines.	*/
#define UP		0
#define RIGHT	1
#define LEFT    2
#define DOWN    3

/*	 Tab defines.	*/
	//	Which Tab To Be Open When The Program Starts.
#define INITIAL_TAB		GENERAL
#define NUMBER_OF_TABS	6
#define TABS_START_POS	5
#define TABS_HEIGHT		4
#define TAB_WIDTH		14
	// Tabs Label Position.
#define TABS_LABEL_FROM_LEFT	4
#define TABS_LABEL_HEIGHT		1

//	Tab Columns defines.
#define LABEL_X_POS					LEFT_MARGIN + 2
#define LABEL_Y_POS					TABS_HEIGHT + 1
#define FIRST_COLUMN_FROM_LEFT		LABEL_X_POS
#define FIRST_COLUMN_FROM_ABOVE		LABEL_Y_POS + 3
#define Y_LINE_UNDER_LABELS			LABEL_Y_POS	+ 1
#define DISTANCE_BETWEEN_COLUMNS	5

/*	Deminsions.	*/
#define LEFT_MARGIN					1
#define RIGHT_MARGIN				3
#define BOTTOM_MARGIN				2
#define STATUS_BAR_HEIGHT			3
#define LEFT_STATUS_BAR_DISTANCE	2
	// Up Time Lable.
#define SYSTEM_UP_TIME_LABEL_X		((TABS_START_POS + NUMBER_OF_TABS * TAB_WIDTH) + 5)
#define SYSTEM_UP_TIME_LABEL_Y		(TABS_HEIGHT - 3)
#define SYSTEM_UP_TIME_X			(SYSTEM_UP_TIME_LABEL_X - 2)
#define SYSTEM_UP_TIME_Y			(SYSTEM_UP_TIME_LABEL_Y + 1)

/*	Console Options.	*/
#define CONSOLE_INITIALIZING_COLOR	("Color 1F")
#define CONSOLE_TITLE				(TEXT("Ana Process Explorer"))

//	Basic list options.
#define LIST_START_X	FIRST_COLUMN_FROM_LEFT
#define LIST_START_Y	FIRST_COLUMN_FROM_ABOVE
#define LIST_WIDTH		(WindowSize.X - (LEFT_MARGIN + RIGHT_MARGIN + FIRST_COLUMN_FROM_LEFT))

/*	Lenght Options.	*/
#define MAX_NAME_LEN					30
#define MAX_PID_LEN						5
#define MAX_PRIORITY_LEN				15
#define MAX_VISIBLE_TEXT_IN_COLUMN		15

//	Threads tab Columns defines.
#define MAX_THREAD_ID_LEN			10
#define MAX_THREAD_CYCLES_LEN		21
#define MAX_THREAD_PRIORITY			15
#define THREAD_ID_LABEL				(TEXT("Thread ID"))
#define THREAD_CYCLES_LABEL			(TEXT("Thread Cycles"))
#define THREAD_PRIORITY_LABEL		(TEXT("Thread Priority"))

//	Modules tab Columns defines.
#define MAX_MODULE_NAME_LEN			MAX_NAME_LEN
#define MAX_MODULE_PATH_LEN			3*MAX_VISIBLE_TEXT_IN_COLUMN
#define MAX_MODULE_BASE_SIZE_LEN	15
#define MODULE_NAME_LABEL			(TEXT("Module Name"))
#define MODULE_BASE_SIZE_LABEL		(TEXT("Base Size(KB)"))
#define MODULE_PATH_LABEL			(TEXT("Path"))

// General tab Columns defines.
#define MAX_PROCESS_NAME_LEN				MAX_NAME_LEN
#define PROCESS_NAME_LABEL					(TEXT("Process"))
#define MAX_PROCESS_ID_LEN					5
#define PROCESS_ID_LABEL					(TEXT("PID"))
#define MAX_PROCESS_CPU_USAGE_LEN			2
#define PROCESS_CPU_USAGE_LABEL				(TEXT("CPU"))
#define MAX_PROCESS_PRIORITY_LEN			13
#define PROCESS_PRIORITY_LABEL				(TEXT("Priority"))
#define MAX_PROCESS_WORKING_SET_SIZE_LEN	21
#define PROCESS_WORKING_SET_SIZE_LABEL		(TEXT("WorkingSetSize (KB)"))
#define MAX_PROCESS_DESCRIPTION_LEN			MAX_VISIBLE_TEXT_IN_COLUMN
#define PROCESS_DESCRIPTION_LABEL			(TEXT("Description"))
#define MAX_PROCESS_COMPANY_NAME_LEN		MAX_VISIBLE_TEXT_IN_COLUMN
#define PROCESS_COMPANY_NAME_LABEL			(TEXT("Company Name"))
#define PROCESS_COMPANY_NAME_LABEL_LEN		wcslen(PROCESS_COMPANY_NAME_LABEL)

/*	Pop Up Menu Defines.	*/
#define MAX_MENU_ITEM_LEN					30
#define MENU_HEAD_VERTICAL_SPACE			3
#define MENU_ITEMS_FROM_HEAD_2ND_LINE		1
#define MENU_FIRST_ITEM_FROM_ABOVE_Y		(WindowSize.Y - (BOTTOM_MARGIN + STATUS_BAR_HEIGHT + MENU_HEIGHT - MENU_HEAD_VERTICAL_SPACE) + MENU_ITEMS_FROM_HEAD_2ND_LINE)
#define MENU_DOWN_START_Y					(WindowSize.Y - (BOTTOM_MARGIN + STATUS_BAR_HEIGHT))
#define MENU_HEIGHT							(MAX_POSSIBLE_LINES/3)
#define MENU_MAX_POSSIBLE_ITEMS				(((MENU_HEIGHT-MENU_HEAD_VERTICAL_SPACE-1)>0)?(MENU_HEIGHT-MENU_HEAD_VERTICAL_SPACE-1):(0))
	//	Graph menu.
#define GRAPH_MENU_HEAD						TEXT("Select Graph")
#define GRAPH_MENU_HEAD_LEN					11
#define GRAPH_MENU_ITEMS					3
#define GRAPH_MENU_WIDTH					30
#define GRAPH_MENU_LEFT_START_X				MID_TEXT_START_X(LEFT_MARGIN , WindowSize.X-(LEFT_MARGIN+RIGHT_MARGIN),GRAPH_MENU_WIDTH)
	//	General menu.
#define GENERAL_MENU_HEAD					TEXT("Process ID: ")
#define GENERAL_MENU_HEAD_LEN				12
#define GENERAL_MENU_ITEMS					4
#define GENERAL_MENU_WIDTH					40
#define GENERAL_MENU_LEFT_START_X			MID_TEXT_START_X(LEFT_MARGIN , WindowSize.X-(LEFT_MARGIN+RIGHT_MARGIN),GENERAL_MENU_WIDTH)
	//	Threads menu.
#define THREAD_MENU_HEAD					TEXT("Thread ID: ")
#define THREAD_MENU_HEAD_LEN				11
#define THREADS_MENU_ITEMS					3
#define THREADS_MENU_WIDTH					30
#define THREADS_MENU_LEFT_START_X			MID_TEXT_START_X(LEFT_MARGIN , WindowSize.X-(LEFT_MARGIN+RIGHT_MARGIN),THREADS_MENU_WIDTH)

/* Set Priority PopUpMenu Defines.	*/
#define PROCESS_SET_PRIORITY_MENU_ITEMS		6
#define THREAD_SET_PRIORITY_MENU_ITEMS		6

//	CLS Defines.
#define CLS_TAB						(ClsXYXCount(LEFT_MARGIN , TABS_HEIGHT , WindowSize.X - ( RIGHT_MARGIN + LEFT_MARGIN) , WindowSize.Y - (TABS_HEIGHT + BOTTOM_MARGIN + STATUS_BAR_HEIGHT)))
#define CLS_BELOW_TAB				(ClsXYXCount(LEFT_MARGIN, FIRST_COLUMN_FROM_ABOVE,WindowSize.X - ( RIGHT_MARGIN + LEFT_MARGIN), MAX_POSSIBLE_LINES + 1))
#define CLS_TAB_LEFT_SIDE			(ClsXYXCount(LEFT_MARGIN, FIRST_COLUMN_FROM_ABOVE,MID_X - LEFT_MARGIN, MAX_POSSIBLE_LINES + 1))
#define CLS_TAB_RIGHT_SIDE			(ClsXYXCount(MID_X+1, FIRST_COLUMN_FROM_ABOVE,(WindowSize.X-RIGHT_MARGIN) - MID_X - 1, MAX_POSSIBLE_LINES + 1))
#define CLS_GENERAL_MENU			(ClsXYXCount(GENERAL_MENU_LEFT_START_X , MENU_DOWN_START_Y - MENU_HEIGHT , GENERAL_MENU_WIDTH + 1 , MENU_HEIGHT))
#define CLS_GRAPH_MENU				(ClsXYXCount(GRAPH_MENU_LEFT_START_X , MENU_DOWN_START_Y - MENU_HEIGHT , GRAPH_MENU_WIDTH + 1 , MENU_HEIGHT))
#define CLS_INSIDE_GENERAL_MENU		(ClsXYXCount(GENERAL_MENU_LEFT_START_X + 1, MENU_FIRST_ITEM_FROM_ABOVE_Y, GENERAL_MENU_WIDTH -1, MENU_MAX_POSSIBLE_ITEMS))
#define CLS_INSIDE_GRAPH_MENU		(ClsXYXCount(GRAPH_MENU_LEFT_START_X + 1, MENU_FIRST_ITEM_FROM_ABOVE_Y, GRAPH_MENU_WIDTH -1, MENU_MAX_POSSIBLE_ITEMS))
#define CLS_THREADS_MENU			(ClsXYXCount(THREADS_MENU_LEFT_START_X , MENU_DOWN_START_Y - MENU_HEIGHT , THREADS_MENU_WIDTH + 1 , MENU_HEIGHT))
#define CLS_INSIDE_THREADS_MENU		(ClsXYXCount(THREADS_MENU_LEFT_START_X + 1, MENU_FIRST_ITEM_FROM_ABOVE_Y, THREADS_MENU_WIDTH -1, MENU_MAX_POSSIBLE_ITEMS))

//	Colors.
#define HIGHLIGHT_COLOR				((BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY) | FOREGROUND_BLUE)
#define REGULAR_COLOR				(BACKGROUND_BLUE | (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY))

//	Details Tab.
#define DETAILS_ITEMS			30
#define MAX_DETAILS_LABEL_LEN	40
	//
#define DETAILS_START_X							(LEFT_MARGIN+1)
#define DETAILS_START_Y							(Y_LINE_UNDER_LABELS+3)
#define DETAILS_ITEMS_VERTICAL_DISTANCE			3
#define DETAILS_Y_LINE_UNDER_LABELS				(LABEL_Y_POS + 1)
#define DETAILS_FIRST_SIDE_LABEL				TEXT("Properties")
#define DETAILS_SECOND_SIDE_LABEL				TEXT("CPU Usage")

//	System Info Tab.
#define SYSTEM_INFO_TAB_ITEMS			19
#define MAX_SYSTEM_INFO_TAB_LABEL_LEN	40
	//
#define SYSTEM_INFO_TAB_START_X							(LEFT_MARGIN+1)
#define SYSTEM_INFO_TAB_START_Y							(Y_LINE_UNDER_LABELS+3)
#define SYSTEM_INFO_TAB_ITEMS_VERTICAL_DISTANCE			3
#define SYSTEM_INFO_TAB_Y_LINE_UNDER_LABELS				(LABEL_Y_POS	+ 1)
#define SYSTEM_INFO_TAB_FIRST_SIDE_LABEL				TEXT("Computer Information")
#define SYSTEM_INFO_TAB_SECOND_SIDE_LABEL				TEXT("Memory Information")

//	A handle to console.
HANDLE hConsole;

// Max number of characters in the user screen size.
COORD WindowSize;

// A Structure to hold Threads tab columns information.
struct tagTabDataColumns{
	struct tagThreadsTab {
		//	Visibility of column.
		BOOL ThreadPIDColumn_Visibile;
		//	Position of the column's label.
		COORD ThreadPIDLabel_Pos;
		//	X,y start coordinates.
		COORD ThreadPIDList_StartPos;
		//
		BOOL ThreadPriority_Visibile;
		COORD ThreadPriorityLabel_Pos;
		COORD ThreadPriority_StartPos;
		//
		BOOL ThreadCycles_Visibile;
		COORD ThreadCyclesLabel_Pos;
		COORD ThreadCycles_StartPos;
	}Threads;
	struct tagModulesTab{
		BOOL	ModuleNameColumn_Visibile;
		COORD	ModuleNameLabel_Pos;
		COORD	ModuleNameList_StartPos;
		//
		BOOL	ModuleBaseSizeColumn_Visibile;
		COORD	ModuleBaseSizeLabel_Pos;
		COORD	ModuleBaseSizeList_StartPos;
		//
		BOOL	ModulePathColumn_Visibile;
		COORD	ModulePathLabel_Pos;
		COORD	ModulePathList_StartPos;
	}Modules;
	struct tagProcessesTab{
		BOOL	ProcessNameColumn_Visibile;
		COORD	ProcessNameLabel_Pos;
		COORD	ProcessNameList_StartPos;
		//
		BOOL	ProcessIDColumn_Visibile;
		COORD	ProcessIDLabel_Pos;
		COORD	ProcessIDList_StartPos;
		//
		BOOL	ProcessCPU_UsageColumn_Visibile;
		COORD	ProcessCPU_UsageLabel_Pos;
		COORD	ProcessCPU_UsageList_StartPos;
		//
		BOOL	ProcessPriorityColumn_Visibile;
		COORD	ProcessPriorityLabel_Pos;
		COORD	ProcessPriorityList_StartPos;
		//
		BOOL	ProcessWorkingSetSizeColumn_Visibile;
		COORD	ProcessWorkingSetSizeLabel_Pos;
		COORD	ProcessWorkingSetSizeList_StartPos;
		//
		BOOL	ProcessDescriptionColumn_Visibile;
		COORD	ProcessDescriptionLabel_Pos;
		COORD	ProcessDescriptionList_StartPos;
		//
		BOOL	ProcessCompanyNameColumn_Visibile;
		COORD	ProcessCompanyNameLabel_Pos;
		COORD	ProcessCompanyNameList_StartPos;
	}Processes;
};
extern struct tagTabDataColumns Tab;
//	Holds shown and selected items on the screen.
struct tagSelectedItems{
	INT CurrentTab;
	INT CurrentGraph;
	struct tagProcess{
		INT First;
		INT Current;
	}Process;
	struct tagDetail{
		INT First;
	}Detail;
	struct tagSystemInfoTab{
		INT First;
	}SystemInfoTab;
	struct tagThread{
		INT First;
		INT Current;
	}Thread;
	struct tagModule{
		INT First;
		INT Current;
	}Module;
	//	Menu Options.
	struct tagMenu{
		struct tagGeneral{
			INT First;
			INT Current;
		}General;
		struct tagThreads{
			INT First;
			INT Current;
		}Threads;
		struct tagGraph{
			INT First;
			INT Current;
		}Graph;
		struct tagSetPriority{
			struct tagThreadPriority{
				INT First;
				INT Current;
			}Thread;
			struct tagProcessPriority{
				INT First;
				INT Current;
			}Process;
		}SetPriority;
	}PopUpMenu;
};

// Details Labels.
struct tagDetailsLabels{
		WCHAR	Items[DETAILS_ITEMS][MAX_DETAILS_LABEL_LEN];
};

// System Info Tab Labels.
struct tagSystemInfoTabLabels{
	struct tagComupterInfo{
		WCHAR	Items[SYSTEM_INFO_TAB_ITEMS][MAX_SYSTEM_INFO_TAB_LABEL_LEN];
	}ComputerInfo;
	struct tagMemoryInfo{
		WCHAR	Items[SYSTEM_INFO_TAB_ITEMS][MAX_SYSTEM_INFO_TAB_LABEL_LEN];
	}MemoryInfo;
};

//	PopUp Menu items.
struct tagPopUpMenuItems{
	struct tagGeneralMenu{
		WCHAR Items[GENERAL_MENU_ITEMS][MAX_MENU_ITEM_LEN];
	}General;
	struct tagThreadsMenu{
		WCHAR Items[THREADS_MENU_ITEMS][MAX_MENU_ITEM_LEN];
	}Threads;
	struct tagGraphMenu{
		WCHAR Items[GRAPH_MENU_ITEMS][MAX_MENU_ITEM_LEN];
	}Graph;
	struct tagSetPriorityMenu{
		struct tagThreadMenuPriority{
			WCHAR Items[THREAD_SET_PRIORITY_MENU_ITEMS][MAX_MENU_ITEM_LEN];
		}SetThreadPriority;
		struct tagProcessMenuPriority{
			WCHAR Items[PROCESS_SET_PRIORITY_MENU_ITEMS][MAX_MENU_ITEM_LEN];
		}SetProcessPriority;
	}SetPriority;
};

//	Holds each tab's menu items.
extern struct tagPopUpMenuItems PopUpMenuItems;
//	Holds selected items.
extern struct tagSelectedItems Selected;
//	The Distance between two labels in status bar is up to the screen size.
extern INT Labels_Distance;
//	Shows whether we are in a menu, etc or not.
extern BOOL InMenu;
//	Being in Set Priority Menu.
extern BOOL InSetPriorityMenu;
//	Handles to the mutex object for multithreading purposes.
extern HANDLE hScreenMutex;