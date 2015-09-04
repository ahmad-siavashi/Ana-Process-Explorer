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
#include "StaticParts.h"

//	The Distance between two labels in status bar is up to the screen size.
INT Labels_Distance;

/*	Draws differnt static parts of the program.	*/
VOID DrawStaticBordersAndStyles(VOID){
	DrawBoarders();
	WriteStaticLabels();
}

/*	Draws border lines of the console frame.	*/
VOID DrawBoarders(VOID){
	INT x,y;
	INT i, j;
	//	Horizontal lines.
	for(x=LEFT_MARGIN;x<WindowSize.X-RIGHT_MARGIN;++x){
		//	Bottom Border of tabs.
		PrintXY(BOARDER_LINE,x,TABS_HEIGHT-1);
		//	Upper line of status bar.
		PrintXY(BOARDER_LINE,x,WindowSize.Y-BOTTOM_MARGIN-STATUS_BAR_HEIGHT);
		//	beneath line of status bar.
		PrintXY(BOARDER_LINE,x,WindowSize.Y-BOTTOM_MARGIN);
	}
	//	Vertical lines.
	for(y=TABS_HEIGHT;y<=WindowSize.Y-BOTTOM_MARGIN;++y){
		//	Right side boarder.
		PrintXY(TEXT("|"),WindowSize.X-RIGHT_MARGIN,y);
		//	Left side boarder.
		PrintXY(TEXT("|"),0,y);
	}
/*	Drawing Tabs.	*/
	#if TABS_UP_BOARDER_LINES
	//	Horizontal Lines.
	for(i = 0 ; i < NUMBER_OF_TABS ; ++i)
		for( j = 1 ; j < TAB_WIDTH ; ++j)
			PrintXY(HORIZONTAL_TAB_LINE, TABS_START_POS + j + i * TAB_WIDTH, 0);
#endif
	//	Vertical Lines.
	for(j=0 ; j < NUMBER_OF_TABS + 1 ; ++j)
		for(
#if TABS_UP_BOARDER_LINES
			i=1; 
#else
			i=0;
#endif
			i < TABS_HEIGHT ; ++i)
			PrintXY(VERTICAL_TAB_LINE, TABS_START_POS + j * TAB_WIDTH, i);
}

/*	Writes labels.	*/
VOID WriteStaticLabels(VOID){
	//	The Distance between two labels in status bar is up to the screen size.
	Labels_Distance = WindowSize.X/14;
	//	System Up Time Label.
	PrintXY(TEXT("Up Time"),SYSTEM_UP_TIME_LABEL_X,SYSTEM_UP_TIME_LABEL_Y);
	PrintXY(TEXT("00:00:00:00"),SYSTEM_UP_TIME_X,SYSTEM_UP_TIME_Y);
	/*	 Tabs Labels.	*/
	PrintXY(TEXT("General"),TABS_START_POS + TABS_LABEL_FROM_LEFT,TABS_LABEL_HEIGHT);
	PrintXY(TEXT("Details"),TABS_START_POS + TABS_LABEL_FROM_LEFT + TAB_WIDTH,TABS_LABEL_HEIGHT);
	PrintXY(TEXT("Modules"),TABS_START_POS + TABS_LABEL_FROM_LEFT + 2 * TAB_WIDTH,TABS_LABEL_HEIGHT);
	PrintXY(TEXT("Threads"),TABS_START_POS + TABS_LABEL_FROM_LEFT + 3 * TAB_WIDTH,TABS_LABEL_HEIGHT);
	PrintXY(TEXT(" Graph"),TABS_START_POS + TABS_LABEL_FROM_LEFT + 4 * TAB_WIDTH,TABS_LABEL_HEIGHT);
	PrintXY(TEXT("System Info"),TABS_START_POS + TABS_LABEL_FROM_LEFT + 5 * TAB_WIDTH - 2,TABS_LABEL_HEIGHT);
	/*	Status Bar Labels.	*/
		//	CPU Usage.
	PrintXY(TEXT("CPU Usage:    %%"),CPU_USAGE,WindowSize.Y-BOTTOM_MARGIN-STATUS_BAR_HEIGHT / 2);
		//	Commit Charge.
	PrintXY(TEXT("Commit Charge:    %%"),(COMMIT_CHARGE + 1 * Labels_Distance),WindowSize.Y-BOTTOM_MARGIN-STATUS_BAR_HEIGHT / 2);
		//	Processes.
	PrintXY(TEXT("Processes:    "),(PROCESSES + 2 * Labels_Distance),WindowSize.Y-BOTTOM_MARGIN-STATUS_BAR_HEIGHT / 2);
		//	Pysical Usage.
	PrintXY(TEXT("Physical Usage:    %%"),(PHYSICAL_USAGE + 3 * Labels_Distance),WindowSize.Y-BOTTOM_MARGIN-STATUS_BAR_HEIGHT / 2);
}