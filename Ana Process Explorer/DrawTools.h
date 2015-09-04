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
//	Prints something in the given position.
VOID PrintXY(WCHAR * Text,INT x,INT y);
//	Changes Console Cursor Position.
VOID GotoXY(INT x,INT y);
//	Cleans the screen with the given start point and len.
VOID ClsXYXCount(INT x,INT y,INT Width,INT Count);
//	Converts a long 64bit unsigned number to string. then prints it in the given position using PrintXY function.
VOID NPrintXY(ULONG64 Num,INT x,INT y);
//	Fills the given position with the given color.
VOID ColorXYXCount(INT Color,INT x,INT y,INT Width,INT Count);
//	Limited length prints.
VOID LPrintXY(WCHAR *Text,INT LenghtLimitation,INT x,INT y);
//	Limited length number prints.
VOID LNPrintXY(SIZE_T Num,INT LenghtLimit,INT x,INT y);