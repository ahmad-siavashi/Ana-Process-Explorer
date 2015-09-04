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
//ReDraw, Actually ReCall all functions that write or draw something which is dependent to console size.
VOID ReDrawAll(VOID);
//	Watches console size changes by the user.
BOOL CheckConsoleSize(VOID);
//	Gets keyboard keys and perfoms the suitable action.
BOOL GetKey(VOID);
//	Does the suitable action according to the selected menu item.
VOID PerformCurrentTabMenuCommand(VOID);
//	Refreshes everything.
VOID Refresh(VOID);
//	Checks whether there is enough space to show a column or not.
VOID CheckSectionsVisibility(VOID);