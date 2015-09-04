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
//	Opens the current tab.
VOID DrawCurrentTab(BOOL FillTabInfo);
//	Closes the tab which was open.
VOID CloseCurrentTab(VOID);
//	Fills current tab information.
VOID FillCurrentTabInformationAndSelectItem(VOID);
//	Writes current tab labels.
VOID DrawCurrentTabFrameAndLabels(VOID);
//	Lists Processes.
VOID ListProcesses(INT StartLine,INT ThreadNumber,INT Count);
//	Lists selected threads of the selected process in the screen.
VOID ListThreads(INT StartLine,INT ThreadNumber,INT Count);
//	Lists selected modules of the selected process in the screen.
VOID ListModules(INT StartLine,INT ThreadNumber,INT Count);
//	Highlights/UnHighlights the selected lines.
VOID HighlightXYXLine(BOOL Highlight,INT x1,INT y1,INT Width,INT Line);
//	Scrolls tabs and tab's lists.
VOID Scroll(INT Direction);
//	Opens a menu shows messages.
VOID DrawCurrentTabPopUpMenu(BOOL FillMenuInfo);
//	Lists selected menu's items.
VOID ListCurrentPopUpMenuItems(INT StartLine,INT FirstItem,INT Count);
//	Cleaning the menu square.
VOID CloseCurrentPopUpMenu(VOID);
//	Writes current tab PopUp Menu frame and labels.
VOID DrawCurrentTabPopUpMenuFrameAndLabels(VOID);
//	Fills current tab PopUpMenu information.
VOID FillCurrentTabPopUpMenuInformationAndSelectFirstItem(VOID);
//	Fills Status bar and System Up time information.
VOID FillSedentaryInfo(VOID);
//	Fills Details tab information.
VOID ListDetails(INT Start);
/*	Fills System Info tab information.	*/
VOID ListSystemInfoTab(INT Start);