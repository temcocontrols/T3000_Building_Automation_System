#ifndef __IMPROVED_MONTHCAL_H
#define __IMPROVED_MONTHCAL_H

#include <SDKDDKVer.h>
#include <afx.h>
#include <afxdtctl.h>
#include <Windows.h>
#include <uxtheme.h>
#include "unicode.h"

#pragma comment(lib, "uxtheme")

#define Alloc(s) malloc((s))
#define ReAlloc(s,t) realloc((s),(t))
#define Free(s) free((s))
#define GetSize(s) GetSize((s));

#define MCSC_SELECTEDTEXT      6  // selection text color
#define MCSC_SELECTEDBK        7  // selection background color
#define MCSC_ABBREVIATIONSTEXT 8  // abbreviations text color
#define MCSC_ABBREVIATIONSBK   9  // abbreviations background color
#define MCSC_ABBREVIATIONSLINE 10 // abbreviations line color

typedef struct tagSELECTION_ITEM
{
	SYSTEMTIME          date;
	tagSELECTION_ITEM * next;
} SELECTION_ITEM, *LPSELECTION_ITEM;

typedef struct
{
	INT               size;
	LPSELECTION_ITEM  first;
} SELECTION_INFO, *LPSELECTION_INFO;

// MCN_SELCHANGEEX is sent whenever the currently displayed date changes
// via month change, year change, keyboard navigation, prev/next button
//
typedef struct tagNMSELCHANGEEX
{
	NMHDR           nmhdr;  // this must be first, so we don't break WM_NOTIFY

	SELECTION_INFO  selectionInfo;
} NMSELCHANGEEX, *LPNMSELCHANGEEX;

VOID   MONTHCAL_Register(VOID);
VOID   MONTHCAL_Unregister(VOID);

#define MULTIPLEMONTHCAL_CLASSA          "MultipleMonthCal32"
#define MULTIPLEMONTHCAL_CLASSW          L"MultipleMonthCal32"

#ifdef UNICODE
#define MULTIPLEMONTHCAL_CLASS           MULTIPLEMONTHCAL_CLASSW
#else
#define MULTIPLEMONTHCAL_CLASS           MULTIPLEMONTHCAL_CLASSA
#endif

#endif  /* __IMPROVED_MONTHCAL_H */
