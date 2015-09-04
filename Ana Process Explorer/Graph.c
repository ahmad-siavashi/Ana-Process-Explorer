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
#include <stdio.h>
#include "AnaMainHeader.h"
#include "DrawTools.h"
/////////////////////////////////////////////////////////////////////////////////////
#define COLUMN_WIDTH					7
#define DISTANCE_BETWEEN_GRAPH_COLUMNS	0
#define MAX_NUMBER_OF_COLUMN			25
#define PERECENTAGE_LEN					5
/////////////////////////////////////////////////////////////////////////////////////
// Graph Dimensions info.
static INT GraphInfo[MAX_NUMBER_OF_COLUMN][2]={{-1,0},{-1,1},{-1,0},{-1,1},{-1,0},{-1,1},{-1,0},{-1,1},{-1,0},{-1,1},{-1,0},{-1,1},
											{-1,0},{-1,1},{-1,0},{-1,1},{-1,0},{-1,1},{-1,1},{-1,0},{-1,1},{-1,0},{-1,1},{-1,0},{-1,1}};

static INT StatusHistory=0;
INT Color[6]={BACKGROUND_GREEN|BACKGROUND_BLUE|BACKGROUND_INTENSITY , BACKGROUND_GREEN | BACKGROUND_BLUE ,
			BACKGROUND_GREEN |BACKGROUND_INTENSITY, BACKGROUND_GREEN,
			BACKGROUND_RED |BACKGROUND_INTENSITY,BACKGROUND_RED};
INT MaxPossibleGraphColumns;
INT Ratio;

/////////////////////////////////////////////////////////////////////////////////////
VOID PaintRegion(INT Top,INT Bottom,INT Left,INT Right,INT color){
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo; 
	SMALL_RECT srctScrollRect, srctClipRect;	
	CHAR_INFO chiFill;
	COORD coordDest;
	// Get the screen buffer size.
	GetConsoleScreenBufferInfo(hConsole, &csbiInfo);
	// The scrolling rectangle is the bottom 15 rows of the 
	// screen buffer. 
	srctScrollRect.Top = Top; 
	srctScrollRect.Bottom = Bottom; 
	srctScrollRect.Left = Left; 
	srctScrollRect.Right = Right; 
	// The destination for the scroll rectangle is one row up. 
	coordDest.X = 0; 
	coordDest.Y = csbiInfo.dwSize.Y + 100; 
	// The clipping rectangle is the same as the scrolling rectangle. 
	// The destination row is left unchanged. 
	srctClipRect = srctScrollRect; 
	// Fill the bottom row with green blanks. 
	chiFill.Attributes = color; 
	chiFill.Char.UnicodeChar = (WCHAR)' '; 
	// Scroll up one line. 
	WaitForSingleObject(hScreenMutex,INFINITE);
	ScrollConsoleScreenBuffer(hConsole, &srctScrollRect,&srctClipRect,coordDest,&chiFill);
	ReleaseMutex(hScreenMutex);
}

INT EvaluatePosition(INT Right,INT n){
	//position =end of box - ((size of colm + distance between col)*(n+1)
	return Right -((COLUMN_WIDTH + DISTANCE_BETWEEN_GRAPH_COLUMNS)*(n+1));
}
VOID HorizontalBlankXYCount(INT x,INT y,INT Num,INT CurrentColm){
	INT i;
	WaitForSingleObject(hScreenMutex,INFINITE);
	for(i=0;i<Num;i++)
		PrintXY(BLANK,x+i,y);
	ReleaseMutex(hScreenMutex);
}

VOID DrawOneColumn(INT Top,INT x,INT y,INT num,INT CurrentColm){
	INT i;
	for(i=0;i<num;i++)
		if((y-i)>=Top)
			HorizontalBlankXYCount(x,y-i,COLUMN_WIDTH,CurrentColm);
}

VOID DrawAllColumns(INT Top,INT Right,INT Bottom,INT MaxPossibleGraphColumns,INT Status){
	INT i;
	for(i=0;i<MaxPossibleGraphColumns;i++){
		if (GraphInfo[i][0]!=-1){
				switch (Status){
				case 0:
					if (GraphInfo[i][1])
						SetConsoleTextAttribute(hConsole,Color[0]);
					else 
						SetConsoleTextAttribute(hConsole,Color[1]);
					break;
				case 1:
					if (GraphInfo[i][1])
						SetConsoleTextAttribute(hConsole,Color[2]);
					else 
						SetConsoleTextAttribute(hConsole,Color[3]);
					break;
				case 2:
					if (GraphInfo[i][1])
						SetConsoleTextAttribute(hConsole,Color[4]);
					else 
						SetConsoleTextAttribute(hConsole,Color[5]);
					break;
			}
			if (GraphInfo[i][0]/Ratio==0)
				DrawOneColumn(Top,EvaluatePosition(Right,i),Bottom,1,i);
			else
				DrawOneColumn(Top,EvaluatePosition(Right,i),Bottom,GraphInfo[i][0]/Ratio,i);
			}
		SetConsoleTextAttribute(hConsole,REGULAR_COLOR);
	}
}

VOID SetInfo(INT Top,INT Bottom,INT Right,INT Left,SIZE_T Percentage){
	INT i,j;
	MaxPossibleGraphColumns=(Right - Left - DISTANCE_BETWEEN_GRAPH_COLUMNS)/(COLUMN_WIDTH + DISTANCE_BETWEEN_GRAPH_COLUMNS);
	for(i=MaxPossibleGraphColumns;i>0;i--)
		for(j=0;j<2;j++)
			GraphInfo[i][j]=GraphInfo[i-1][j];
	GraphInfo[0][0]=Percentage;
	if (GraphInfo[1][1]==0)
		GraphInfo[0][1]=1;
	else
		GraphInfo[0][1]=0;
	Ratio = 100/(Bottom - Top - 1);
}

VOID ClsGraphColumns(INT Top,INT Right,INT Bottom,INT MaxPossibleGraphColumns){
	INT i;
	WaitForSingleObject(hScreenMutex,INFINITE);
	for(i=0;i<MaxPossibleGraphColumns;i++){
		if (GraphInfo[i][0]!=-1){
			SetConsoleTextAttribute(hConsole,REGULAR_COLOR);
			if (GraphInfo[i][0]/Ratio==0)
				DrawOneColumn(Top,EvaluatePosition(Right,i),Bottom,1,i);
			else
				DrawOneColumn(Top,EvaluatePosition(Right,i),Bottom,GraphInfo[i][0]/Ratio,i);
			}
		SetConsoleTextAttribute(hConsole,REGULAR_COLOR);
	}
	ReleaseMutex(hScreenMutex);
}

VOID EvacuateGraphHistory(VOID){
	INT i;
	for(i=0;i<MAX_NUMBER_OF_COLUMN;i++){
		GraphInfo[i][0]=-1;
		GraphInfo[i][1]=0;
	}
}

VOID GraphStaticPart(INT Top,INT Bottom,INT Left){
	WaitForSingleObject(hScreenMutex,INFINITE);
	PrintXY(TEXT("100%%"),Left,Top);
	PrintXY(TEXT("0  %%"),Left,Bottom);
	PrintXY(TEXT("50 %%"),Left,(Bottom+Top)/2);
	if ((((Bottom+Top)/2)+Bottom)/2-(Bottom+Top)/2>6){
		PrintXY(TEXT("75 %%"),Left,(((Bottom+Top)/2)+Top)/2);
		PrintXY(TEXT("25 %%"),Left,(((Bottom+Top)/2)+Bottom)/2);
	}
	ReleaseMutex(hScreenMutex);
} 

VOID DrawGraph(INT Top,INT Bottom,INT Left,INT Right,SIZE_T Percentage,INT Status){
	GraphStaticPart(Top,Bottom,Left);
	PaintRegion(Top,Bottom,Left + PERECENTAGE_LEN,Right,REGULAR_COLOR);
	SetInfo(Top,Bottom,Right,Left + PERECENTAGE_LEN,Percentage);
	if (Ratio>0)  DrawAllColumns(Top,Right,Bottom,MaxPossibleGraphColumns,Status);
}