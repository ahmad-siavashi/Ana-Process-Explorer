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

#define LONGEST_64BIT_NUM 21

/*	Limited length prints.	*/
VOID LPrintXY(WCHAR *Text,INT LenghtLimit,INT x,INT y){
	INT i;
	WCHAR Temp[MAX_PATH];
	if(wcslen(Text) <= (size_t)LenghtLimit)
		PrintXY(Text,x,y);
	else{
		for(i=0;i<LenghtLimit - 3;++i)
			Temp[i] = Text[i];
		Temp[i]='\0';
		wcscat(Temp,TEXT("..."));
		PrintXY(Temp,x,y);
	}
}

/*	Limited length number prints.	*/
VOID LNPrintXY(SIZE_T Num,INT LenghtLimit,INT x,INT y){
	INT i;
	WCHAR Temp[MAX_PATH];
	WCHAR	Text[LONGEST_64BIT_NUM];
	for(i=0;Num>0;++i,Num/=10)
		Text[i] = Num%10 + '0';
	if(i==0)
		Text[i++]='0';
	Text[i]='\0';
	_wcsrev(Text);
	if(wcslen(Text) <=  (size_t)LenghtLimit)
		PrintXY(Text,x,y);
	else{
		for(i=0;i<LenghtLimit - 3;++i)
			Temp[i] = Text[i];
		Temp[i]='\0';
		wcscat(Temp,TEXT("..."));
		PrintXY(Temp,x,y);
	}
}

/*	Converts a long number to string. then prints it in the given position using PrintXY function.	*/
VOID NPrintXY(ULONG64 Num,INT x,INT y){
	INT i;
	WCHAR	Text[LONGEST_64BIT_NUM];
	for(i=0;Num>0;++i,Num/=10)
		Text[i] = Num%10 + '0';
	if(i==0)
		Text[i++]='0';
	Text[i]='\0';
	wcsrev(Text);
	PrintXY(Text,x,y);
}

/*	Changes Console Cursor Position.	*/
VOID GotoXY(INT x,INT y){
	COORD Pos = {x,y};
	SetConsoleCursorPosition(hConsole,Pos);
}

/*	Prints something in the given position.	*/
VOID PrintXY(WCHAR *Text,INT x,INT y){
	GotoXY(x,y);
	wprintf(Text);
}

/*	Cleans the screen with the given start point and len.	*/
VOID ClsXYXCount(INT x,INT y,INT Width,INT Count){
	INT i;
	DWORD cCharsWritten;
	//	Start point of cleaning the screen.
    COORD coordScreen = {x,y};
    //	Filling the given position with blanks.
	for(i=0;i < Count;i++){
		FillConsoleOutputCharacter( hConsole, (WCHAR) ' ',(Width), coordScreen, &cCharsWritten );
		++coordScreen.Y;
	}
}

/*	Fills the given position with the given color.	*/
VOID ColorXYXCount(INT Color,INT x,INT y,INT Width,INT Count){
	COORD WriteCoord = {x,y};
	DWORD NumberOfAttrsWritten;
	FillConsoleOutputAttribute(hConsole,Color,Width,WriteCoord,&NumberOfAttrsWritten);
}