

/////////////////////////////////  Includes  //////////////////////////////////

#include "stdafx.h"
#include "CT3000CO2_NET.h"


//////////////////////////////// Statics / Macros /////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


////////////////////////////////// Implementation /////////////////////////////

IMPLEMENT_DYNAMIC(CCT3000CO2_NET, CWnd)

CCT3000CO2_NET::CCT3000CO2_NET() : m_DirectFunction(0),
                                   m_DirectPointer(0)
{
}

BOOL CCT3000CO2_NET::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, DWORD dwExStyle, LPVOID lpParam)
{
  //Call our base class implementation of CWnd::CreateEx
	BOOL bSuccess = CreateEx(dwExStyle, _T("scintilla"), NULL, dwStyle, rect, pParentWnd, nID, lpParam);

  //Setup the direct access data
  if (bSuccess)
    SetupDirectAccess();

  //If we are running as Unicode, then use the UTF8 codepage
#ifdef _UNICODE
  SetCodePage(SC_CP_UTF8);
#endif

  return bSuccess;
}

void CCT3000CO2_NET::SetupDirectAccess()
{
  //Setup the direct access data
  m_DirectFunction = GetDirectFunction();
	m_DirectPointer = GetDirectPointer();
}

CCT3000CO2_NET::~CCT3000CO2_NET()
{
	DestroyWindow();
}

inline LRESULT CCT3000CO2_NET::Call(UINT message, WPARAM wParam, LPARAM lParam, BOOL bDirect)
{
  ASSERT(::IsWindow(m_hWnd)); //Window must be valid

  if (bDirect)
  {
    ASSERT(m_DirectFunction); //Direct function must be valid
    return (reinterpret_cast<SciFnDirect>(m_DirectFunction))(m_DirectPointer, message, wParam, lParam);
  }
  else
    return SendMessage(message, wParam, lParam);
}

LRESULT CCT3000CO2_NET::GetDirectFunction()
{
  return SendMessage(SCI_GETDIRECTFUNCTION, 0, 0);
}

LRESULT CCT3000CO2_NET::GetDirectPointer()
{
  return SendMessage(SCI_GETDIRECTPOINTER, 0, 0);
}

CString CCT3000CO2_NET::GetSelText()
{
  //Call the function which does the work
  CString sSelText;
  GetSelText(sSelText.GetBufferSetLength(GetSelectionEnd() - GetSelectionStart()));
  sSelText.ReleaseBuffer();
  return sSelText;
}

#ifdef _UNICODE
int CCT3000CO2_NET::W2UTF8(const wchar_t* pszText, int nLength, char*& pszUTF8Text)
{
  //Validate our parameters
  ASSERT(pszText);

  //First call the function to determine how much heap space we need to allocate
	int nUTF8Length = WideCharToMultiByte(CP_UTF8, 0, pszText, nLength, NULL, 0, NULL, NULL);

  //If the calculated length is zero, then ensure we have at least room for a NULL terminator
  if (nUTF8Length == 0)
    nUTF8Length = 1;

  //Now allocate the heap space
  pszUTF8Text = new char[nUTF8Length];
  pszUTF8Text[0] = '\0';

  //And now recall with the buffer to get the converted text
	int nCharsWritten = WideCharToMultiByte(CP_UTF8, 0, pszText, nLength, pszUTF8Text, nUTF8Length, NULL, NULL);
  if (nLength != -1)
    return nCharsWritten; //Trust the return value
  else
    return nCharsWritten ? nCharsWritten - 1 : 0; //Exclude the NULL terminator written
}

int CCT3000CO2_NET::UTF82W(const char* pszText, int nLength, wchar_t*& pszWText)
{
  //Validate our parameters
  ASSERT(pszText);

  //First call the function to determine how much heap space we need to allocate
	int nWideLength = MultiByteToWideChar(CP_UTF8, 0, pszText, nLength, NULL, 0);

  //If the calculated length is zero, then ensure we have at least room for a NULL terminator
  if (nWideLength == 0)
    nWideLength = 1;

  //Now allocate the heap space
  pszWText = new wchar_t[nWideLength];
  pszWText[0] = '\0';

  //And now recall with the buffer to get the converted text
	int nCharsWritten = MultiByteToWideChar(CP_UTF8, 0, pszText, nLength, pszWText, nWideLength);
  if (nLength != -1)
    return nCharsWritten; //Trust the return value
  else
    return nCharsWritten ? nCharsWritten - 1 : 0; //Exclude the NULL terminator written
}

void CCT3000CO2_NET::AddText(int length, const wchar_t* text, BOOL bDirect)
{
  //Convert the unicode text to UTF8
  char* pszUTF8;
  int nUTF8Length = W2UTF8(text, length, pszUTF8);

  //Call the native scintilla version of the function with the UTF8 text
  AddText(nUTF8Length, pszUTF8, bDirect);

  //Tidy up the heap memory before we return
  delete [] pszUTF8;
}

void CCT3000CO2_NET::InsertText(long pos, const wchar_t* text, BOOL bDirect)
{
  //Convert the unicode text to UTF8
  char* pszUTF8;
  W2UTF8(text, -1, pszUTF8);

  //Call the native scintilla version of the function with the UTF8 text
  InsertText(pos, pszUTF8, bDirect);

  //Tidy up the heap memory before we return
  delete [] pszUTF8;
}

int CCT3000CO2_NET::GetSelText(wchar_t* text, BOOL bDirect)
{
  //Allocate some heap memory to contain the UTF8 text
  char* pszUTF8 = new char[GetSelectionEnd() - GetSelectionStart() + 1];

  //Call the function which does the work
  GetSelText(pszUTF8, bDirect);

  //Now convert the UTF8 text back to Unicode
  wchar_t* pszWText;
  int nWLength = UTF82W(pszUTF8, -1, pszWText);
  ASSERT(text);
#if (_MSC_VER >= 1400)
  wcscpy_s(text, nWLength+1, pszWText);
#else  
  wcscpy(text, pszWText);
#endif

  //Tidy up the heap memory before we return
  delete [] pszWText;
  delete [] pszUTF8;

  return nWLength;  
}

int CCT3000CO2_NET::GetCurLine(int length, wchar_t* text, BOOL bDirect)
{
  //Allocate some heap memory to contain the UTF8 text
  int nUTF8Length = text ? length*4 : GetCurLine(0, static_cast<char*>(NULL), bDirect)*4;
  char* pszUTF8 = new char[nUTF8Length]; //A Unicode character can take up to 4 octets when expressed as UTF8

  //Call the function which does the work
  int nOriginalReturn = GetCurLine(nUTF8Length, pszUTF8, bDirect);

  //Now convert the UTF8 text back to Unicode
  wchar_t* pszWText;
  int nReturnLength = UTF82W(pszUTF8, -1, pszWText);
  int nReturn = nReturnLength + 1;
  if (text)
  {
    //Copy as much text as possible into the output parameter
    int i;
		for (i=0; i<length-1 && i<nReturnLength; i++) 
			text[i] = pszWText[i];
		text[i] = L'\0';

    //What we have text to return, use the original return value
    nReturn = nOriginalReturn;
  }

  //Tidy up the heap memory before we return
  delete [] pszWText;
  delete [] pszUTF8;

  return nReturn;  
}

void CCT3000CO2_NET::StyleSetFont(int style, const wchar_t* fontName, BOOL bDirect)
{
  char* pszUTF8;
  W2UTF8(fontName, -1, pszUTF8);

  StyleSetFont(style, pszUTF8, bDirect);

  //Tidy up the heap memory before we return
  delete [] pszUTF8;
}

void CCT3000CO2_NET::SetWordChars(const wchar_t* characters, BOOL bDirect)
{
  //Convert the unicode text to UTF8
  char* pszUTF8;
  W2UTF8(characters, -1, pszUTF8);

  SetWordChars(pszUTF8, bDirect);

  //Tidy up the heap memory before we return
  delete [] pszUTF8;
}

void CCT3000CO2_NET::AutoCShow(int lenEntered, const wchar_t* itemList, BOOL bDirect)
{
  //Convert the unicode text to UTF8
  char* pszUTF8;
  W2UTF8(itemList, -1, pszUTF8);

  //Call the native scintilla version of the function with the UTF8 text
  AutoCShow(lenEntered, pszUTF8, bDirect);

  //Tidy up the heap memory before we return
  delete [] pszUTF8;
}

void CCT3000CO2_NET::AutoCStops(const wchar_t* characterSet, BOOL bDirect)
{
  //Convert the unicode text to UTF8
  char* pszUTF8;
  W2UTF8(characterSet, -1, pszUTF8);

  //Call the native scintilla version of the function with the UTF8 text
  AutoCStops(pszUTF8, bDirect);

  //Tidy up the heap memory before we return
  delete [] pszUTF8;
}

void CCT3000CO2_NET::AutoCSelect(const wchar_t* text, BOOL bDirect)
{
  //Convert the unicode text to UTF8
  char* pszUTF8;
  W2UTF8(text, -1, pszUTF8);

  //Call the native scintilla version of the function with the UTF8 text
  AutoCSelect(pszUTF8, bDirect);

  //Tidy up the heap memory before we return
  delete [] pszUTF8;
}

void CCT3000CO2_NET::AutoCSetFillUps(const wchar_t* characterSet, BOOL bDirect)
{
  //Convert the unicode text to UTF8
  char* pszUTF8;
  W2UTF8(characterSet, -1, pszUTF8);

  //Call the native scintilla version of the function with the UTF8 text
  AutoCSetFillUps(pszUTF8, bDirect);

  //Tidy up the heap memory before we return
  delete [] pszUTF8;
}

void CCT3000CO2_NET::UserListShow(int listType, const wchar_t* itemList, BOOL bDirect)
{
  //Convert the unicode text to UTF8
  char* pszUTF8;
  W2UTF8(itemList, -1, pszUTF8);

  //Call the native scintilla version of the function with the UTF8 text
  UserListShow(listType, pszUTF8, bDirect);

  //Tidy up the heap memory before we return
  delete [] pszUTF8;
}

int CCT3000CO2_NET::GetLine(int line, wchar_t* text, BOOL bDirect)
{
  //Validate our parameters
  ASSERT(text);

  //Work out how big the input buffer is (for details on this, see the EM_GETLINE documentation)
  WORD* wBuffer = reinterpret_cast<WORD*>(text);
  WORD wInputBufferSize = wBuffer[0];

  //Allocate some heap memory to contain the UTF8 text
  int nUTF8Length = wInputBufferSize*4;
  char* pszUTF8 = new char[nUTF8Length]; //A Unicode character can take up to 4 octets when expressed as UTF8

  //Set the buffer size in the new buffer
  wBuffer = reinterpret_cast<WORD*>(pszUTF8);
  wBuffer[0] = wInputBufferSize;

  //Call the function which does the work
  int nChars = GetLine(line, pszUTF8, bDirect);

  //Now convert the UTF8 text back to Unicode
  wchar_t* pszWText;
  int nWLength = UTF82W(pszUTF8, nChars, pszWText);
  for (int i=0; i<=nWLength && i<=wInputBufferSize; i++)
    text[i] = pszWText[i];  

  //Tidy up the heap memory before we return
  delete [] pszWText;
  delete [] pszUTF8;

  return nWLength;  
}

void CCT3000CO2_NET::ReplaceSel(const wchar_t* text, BOOL bDirect)
{
  //Convert the unicode text to UTF8
  char* pszUTF8;
  W2UTF8(text, -1, pszUTF8);

  //Call the native scintilla version of the function with the UTF8 text
  ReplaceSel(pszUTF8, bDirect);

  //Tidy up the heap memory before we return
  delete [] pszUTF8;
}

void CCT3000CO2_NET::SetText(const wchar_t* text, BOOL bDirect)
{
  //Convert the unicode text to UTF8
  char* pszUTF8;
  W2UTF8(text, -1, pszUTF8);

  //Call the native scintilla version of the function with the UTF8 text
  SetText(pszUTF8, bDirect);

  //Tidy up the heap memory before we return
  delete [] pszUTF8;
}

int CCT3000CO2_NET::GetText(int length, wchar_t* text, BOOL bDirect)
{
  //Allocate some heap memory to contain the UTF8 text
  int nUTF8Length = length*4;
  char* pszUTF8 = new char[nUTF8Length]; //A Unicode character can take up to 4 octets when expressed as UTF8

  //Call the function which does the work
  GetText(nUTF8Length, pszUTF8, bDirect);

  //Now convert the UTF8 text back to Unicode
  wchar_t* pszWText;
  int nWLength = UTF82W(pszUTF8, -1, pszWText);

  //Copy as much text as possible into the output parameter
  ASSERT(text);
  int i;
	for (i=0; i<length-1 && i<nWLength; i++) 
		text[i] = pszWText[i];
	text[i] = L'\0';

  //Tidy up the heap memory before we return
  delete [] pszWText;
  delete [] pszUTF8;

  return nWLength;  
}

int CCT3000CO2_NET::ReplaceTarget(int length, const wchar_t* text, BOOL bDirect)
{
  //Convert the unicode text to UTF8
  char* pszUTF8;
  int nUTF8Length = W2UTF8(text, length, pszUTF8);

  //Call the native scintilla version of the function with the UTF8 text
  int nReturn = ReplaceTarget(nUTF8Length, pszUTF8, bDirect);

  //Tidy up the heap memory before we return
  delete [] pszUTF8;

  return nReturn;
}

int CCT3000CO2_NET::ReplaceTargetRE(int length, const wchar_t* text, BOOL bDirect)
{
  //Convert the unicode text to UTF8
  char* pszUTF8;
  int nUTF8Length = W2UTF8(text, length, pszUTF8);

  //Call the native scintilla version of the function with the UTF8 text
  int nReturn = ReplaceTargetRE(nUTF8Length, pszUTF8, bDirect);

  //Tidy up the heap memory before we return
  delete [] pszUTF8;

  return nReturn;
}

int CCT3000CO2_NET::SearchInTarget(int length, const wchar_t* text, BOOL bDirect)
{
  //Convert the unicode text to UTF8
  char* pszUTF8;
  int nUTF8Length = W2UTF8(text, length, pszUTF8);

  //Call the native scintilla version of the function with the UTF8 text
  int nReturn = SearchInTarget(nUTF8Length, pszUTF8, bDirect);

  //Tidy up the heap memory before we return
  delete [] pszUTF8;

  return nReturn;
}

void CCT3000CO2_NET::CallTipShow(long pos, const wchar_t* definition, BOOL bDirect)
{
  //Convert the unicode text to UTF8
  char* pszUTF8;
  W2UTF8(definition, -1, pszUTF8);

  //Call the native scintilla version of the function with the UTF8 text
  CallTipShow(pos, pszUTF8, bDirect);

  //Tidy up the heap memory before we return
  delete [] pszUTF8;
}

int CCT3000CO2_NET::TextWidth(int style, const wchar_t* text, BOOL bDirect)
{
  //Convert the unicode text to UTF8
  char* pszUTF8;
  W2UTF8(text, -1, pszUTF8);

  //Call the native scintilla version of the function with the UTF8 text
  int nReturn = TextWidth(style, pszUTF8, bDirect);

  //Tidy up the heap memory before we return
  delete [] pszUTF8;

  return nReturn;
}

void CCT3000CO2_NET::AppendText(int length, const wchar_t* text, BOOL bDirect)
{
  //Convert the unicode text to UTF8
  char* pszUTF8;
  int nUTF8Length = W2UTF8(text, length, pszUTF8);

  //Call the native scintilla version of the function with the UTF8 text
  AppendText(nUTF8Length, pszUTF8, bDirect);

  //Tidy up the heap memory before we return
  delete [] pszUTF8;
}

int CCT3000CO2_NET::SearchNext(int flags, const wchar_t* text, BOOL bDirect)
{
  //Convert the unicode text to UTF8
  char* pszUTF8;
  W2UTF8(text, -1, pszUTF8);

  //Call the native scintilla version of the function with the UTF8 text
  int nReturn = SearchNext(flags, pszUTF8, bDirect);

  //Tidy up the heap memory before we return
  delete [] pszUTF8;

  return nReturn;
}

int CCT3000CO2_NET::SearchPrev(int flags, const wchar_t* text, BOOL bDirect)
{
  //Convert the unicode text to UTF8
  char* pszUTF8;
  W2UTF8(text, -1, pszUTF8);

  //Call the native scintilla version of the function with the UTF8 text
  int nReturn = SearchPrev(flags, pszUTF8, bDirect);

  //Tidy up the heap memory before we return
  delete [] pszUTF8;

  return nReturn;
}

void CCT3000CO2_NET::CopyText(int length, const wchar_t* text, BOOL bDirect)
{
  //Convert the unicode text to UTF8
  char* pszUTF8;
  int nUTF8Length = W2UTF8(text, length, pszUTF8);

  //Call the native scintilla version of the function with the UTF8 text
  CopyText(nUTF8Length, pszUTF8, bDirect);

  //Tidy up the heap memory before we return
  delete [] pszUTF8;
}

void CCT3000CO2_NET::SetWhitespaceChars(const wchar_t* characters, BOOL bDirect)
{
  //Convert the unicode text to UTF8
  char* pszUTF8;
  W2UTF8(characters, -1, pszUTF8);

  //Call the native scintilla version of the function with the UTF8 text
  SetWhitespaceChars(pszUTF8, bDirect);

  //Tidy up the heap memory before we return
  delete [] pszUTF8;
}

void CCT3000CO2_NET::SetProperty(const wchar_t* key, const wchar_t* value, BOOL bDirect)
{
  //Convert the unicode text to UTF8
  char* pszUTF8Key;
  W2UTF8(key, -1, pszUTF8Key);
  char* pszUTF8Value;
  W2UTF8(value, -1, pszUTF8Value);

  //Call the native scintilla version of the function with the UTF8 text
  SetProperty(pszUTF8Key, pszUTF8Value, bDirect);

  //Tidy up the heap memory before we return
  delete [] pszUTF8Value;
  delete [] pszUTF8Key;
}

void CCT3000CO2_NET::SetKeyWords(int keywordSet, const wchar_t* keyWords, BOOL bDirect)
{
  //Convert the unicode text to UTF8
  char* pszUTF8;
  W2UTF8(keyWords, -1, pszUTF8);

  //Call the native scintilla version of the function with the UTF8 text
  SetKeyWords(keywordSet, pszUTF8, bDirect);

  //Tidy up the heap memory before we return
  delete [] pszUTF8;
}

void CCT3000CO2_NET::SetLexerLanguage(const wchar_t* language, BOOL bDirect)
{
  //Convert the unicode text to UTF8
  char* pszUTF8;
  W2UTF8(language, -1, pszUTF8);

  //Call the native scintilla version of the function with the UTF8 text
  SetLexerLanguage(pszUTF8, bDirect);

  //Tidy up the heap memory before we return
  delete [] pszUTF8;
}

void CCT3000CO2_NET::LoadLexerLibrary(const wchar_t* path, BOOL bDirect)
{
  //Convert the unicode text to UTF8
  char* pszUTF8;
  W2UTF8(path, -1, pszUTF8);

  //Call the native scintilla version of the function with the UTF8 text
  LoadLexerLibrary(pszUTF8, bDirect);

  //Tidy up the heap memory before we return
  delete [] pszUTF8;
}

int CCT3000CO2_NET::GetProperty(const wchar_t* key, wchar_t* buf, BOOL bDirect)
{
  //Validate our parameters
  ASSERT(key);

  //Convert the Key value to UTF8
  char* pszUTF8Key;
  W2UTF8(key, -1, pszUTF8Key);

  //Allocate some heap memory to contain the UTF8 text
  int nUTF8ValueLength = GetProperty(pszUTF8Key, 0, bDirect);
  char* pszUTF8Value = new char[nUTF8ValueLength + 1]; //Don't forget the NULL terminator

  //Call the function which does the work
  GetProperty(pszUTF8Key, pszUTF8Value, bDirect);

  //Now convert the UTF8 text back to Unicode
  wchar_t* pszWText;
  int nWLength = UTF82W(pszUTF8Value, -1, pszWText);
  if (buf)
  #if (_MSC_VER >= 1400)
    wcscpy_s(buf, nWLength+1, pszWText);
  #else
    wcscpy(buf, pszWText);
  #endif

  //Tidy up the heap memory before we return
  delete [] pszWText;
  delete [] pszUTF8Value;
  delete [] pszUTF8Key;

  return nWLength;  
}

int CCT3000CO2_NET::GetPropertyExpanded(const wchar_t* key, wchar_t* buf, BOOL bDirect)
{
  //Validate our parameters
  ASSERT(key);

  //Convert the Key value to UTF8
  char* pszUTF8Key;
  W2UTF8(key, -1, pszUTF8Key);

  //Allocate some heap memory to contain the UTF8 text
  int nUTF8ValueLength = GetPropertyExpanded(pszUTF8Key, 0, bDirect);
  char* pszUTF8Value = new char[nUTF8ValueLength + 1]; //Don't forget the NULL terminator

  //Call the function which does the work
  GetPropertyExpanded(pszUTF8Key, pszUTF8Value, bDirect);

  //Now convert the UTF8 text back to Unicode
  wchar_t* pszWText;
  int nWLength = UTF82W(pszUTF8Value, -1, pszWText);
  if (buf)
  #if (_MSC_VER >= 1400)
    wcscpy_s(buf, nWLength+1, pszWText);
  #else
    wcscpy(buf, pszWText);
  #endif

  //Tidy up the heap memory before we return
  delete [] pszWText;
  delete [] pszUTF8Value;
  delete [] pszUTF8Key;

  return nWLength;  
}

int CCT3000CO2_NET::GetPropertyInt(const wchar_t* key, BOOL bDirect)
{
  //Convert the unicode text to UTF8
  char* pszUTF8;
  W2UTF8(key, -1, pszUTF8);

  //Call the native scintilla version of the function with the UTF8 text
  int nReturn = GetPropertyInt(pszUTF8, bDirect);

  //Tidy up the heap memory before we return
  delete [] pszUTF8;

  return nReturn;
}

int CCT3000CO2_NET::StyleGetFont(int style, wchar_t* fontName, BOOL bDirect)
{
  //Allocate a UTF8 buffer to contain the font name. See the notes for
  //SCI_STYLEGETFONT / SCI_STYLESETFONT on the reasons why we can use
  //a statically sized buffer of 32 characters in size. Note it is 33 below
  //to include space for the NULL terminator
  char szUTF8FontName[33*4]; //A Unicode character can take up to 4 octets when expressed as UTF8

  //Call the native scintilla version of the function with a UTF8 text buffer
  int nReturn = StyleGetFont(style, szUTF8FontName, bDirect);

  //Now convert the UTF8 text back to Unicode
  wchar_t* pszWFontName;
  int nWLength = UTF82W(szUTF8FontName, -1, pszWFontName);
  if (fontName)
  #if (_MSC_VER >= 1400)
    wcscpy_s(fontName, nWLength+1, pszWFontName);
  #else
    wcscpy(fontName, pszWFontName);
    nWLength; //To get rid of "local variable is initialized but not referenced" warning when compiled in VC 6
  #endif

  //Tidy up the heap memory before we return
  delete [] pszWFontName;

  return nReturn;
}

#endif //#ifdef _UNICODE

int CCT3000CO2_NET::GetLineEx(int line, TCHAR* text, int nMaxChars, BOOL bDirect)
{
  //Validate out parameters
  ASSERT(text);

  //Explicitly set the first value of text to nMaxChars (for details on this, see the EM_GETLINE documentation)
  WORD* wBuffer = reinterpret_cast<WORD*>(text);
  wBuffer[0] = nMaxChars;

  //now call off to the other implementation to do the real work
  return GetLine(line, text, bDirect);
}


//Everything else after this point was auto generated using the "ConvertScintillaiface.js" script

void CCT3000CO2_NET::AddText(int length, const char* text, BOOL bDirect)
{
  Call(SCI_ADDTEXT, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text), bDirect);
}

void CCT3000CO2_NET::AddStyledText(int length, char* c, BOOL bDirect)
{
  Call(SCI_ADDSTYLEDTEXT, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(c), bDirect);
}

void CCT3000CO2_NET::InsertText(long pos, const char* text, BOOL bDirect)
{
  Call(SCI_INSERTTEXT, static_cast<WPARAM>(pos), reinterpret_cast<LPARAM>(text), bDirect);
}

void CCT3000CO2_NET::ClearAll(BOOL bDirect)
{
  Call(SCI_CLEARALL, 0, 0, bDirect);
}

void CCT3000CO2_NET::ClearDocumentStyle(BOOL bDirect)
{
  Call(SCI_CLEARDOCUMENTSTYLE, 0, 0, bDirect);
}

int CCT3000CO2_NET::GetLength(BOOL bDirect)
{
  return Call(SCI_GETLENGTH, 0, 0, bDirect);
}

int CCT3000CO2_NET::GetCharAt(long pos, BOOL bDirect)
{
  return Call(SCI_GETCHARAT, static_cast<WPARAM>(pos), 0, bDirect);
}

long CCT3000CO2_NET::GetCurrentPos(BOOL bDirect)
{
  return Call(SCI_GETCURRENTPOS, 0, 0, bDirect);
}

long CCT3000CO2_NET::GetAnchor(BOOL bDirect)
{
  return Call(SCI_GETANCHOR, 0, 0, bDirect);
}

int CCT3000CO2_NET::GetStyleAt(long pos, BOOL bDirect)
{
  return Call(SCI_GETSTYLEAT, static_cast<WPARAM>(pos), 0, bDirect);
}

void CCT3000CO2_NET::Redo(BOOL bDirect)
{
  Call(SCI_REDO, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetUndoCollection(BOOL collectUndo, BOOL bDirect)
{
  Call(SCI_SETUNDOCOLLECTION, static_cast<WPARAM>(collectUndo), 0, bDirect);
}

void CCT3000CO2_NET::SelectAll(BOOL bDirect)
{
  Call(SCI_SELECTALL, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetSavePoint(BOOL bDirect)
{
  Call(SCI_SETSAVEPOINT, 0, 0, bDirect);
}

int CCT3000CO2_NET::GetStyledText(TextRange* tr, BOOL bDirect)
{
  return Call(SCI_GETSTYLEDTEXT, 0, reinterpret_cast<LPARAM>(tr), bDirect);
}

BOOL CCT3000CO2_NET::CanRedo(BOOL bDirect)
{
  return Call(SCI_CANREDO, 0, 0, bDirect);
}

int CCT3000CO2_NET::MarkerLineFromHandle(int handle, BOOL bDirect)
{
  return Call(SCI_MARKERLINEFROMHANDLE, static_cast<WPARAM>(handle), 0, bDirect);
}

void CCT3000CO2_NET::MarkerDeleteHandle(int handle, BOOL bDirect)
{
  Call(SCI_MARKERDELETEHANDLE, static_cast<WPARAM>(handle), 0, bDirect);
}

BOOL CCT3000CO2_NET::GetUndoCollection(BOOL bDirect)
{
  return Call(SCI_GETUNDOCOLLECTION, 0, 0, bDirect);
}

int CCT3000CO2_NET::GetViewWS(BOOL bDirect)
{
  return Call(SCI_GETVIEWWS, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetViewWS(int viewWS, BOOL bDirect)
{
  Call(SCI_SETVIEWWS, static_cast<WPARAM>(viewWS), 0, bDirect);
}

long CCT3000CO2_NET::PositionFromPoint(int x, int y, BOOL bDirect)
{
  return Call(SCI_POSITIONFROMPOINT, static_cast<WPARAM>(x), static_cast<LPARAM>(y), bDirect);
}

long CCT3000CO2_NET::PositionFromPointClose(int x, int y, BOOL bDirect)
{
  return Call(SCI_POSITIONFROMPOINTCLOSE, static_cast<WPARAM>(x), static_cast<LPARAM>(y), bDirect);
}

void CCT3000CO2_NET::GotoLine(int line, BOOL bDirect)
{
  Call(SCI_GOTOLINE, static_cast<WPARAM>(line), 0, bDirect);
}

void CCT3000CO2_NET::GotoPos(long pos, BOOL bDirect)
{
  Call(SCI_GOTOPOS, static_cast<WPARAM>(pos), 0, bDirect);
}

void CCT3000CO2_NET::SetAnchor(long posAnchor, BOOL bDirect)
{
  Call(SCI_SETANCHOR, static_cast<WPARAM>(posAnchor), 0, bDirect);
}

int CCT3000CO2_NET::GetCurLine(int length, char* text, BOOL bDirect)
{
  return Call(SCI_GETCURLINE, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text), bDirect);
}

long CCT3000CO2_NET::GetEndStyled(BOOL bDirect)
{
  return Call(SCI_GETENDSTYLED, 0, 0, bDirect);
}

void CCT3000CO2_NET::ConvertEOLs(int eolMode, BOOL bDirect)
{
  Call(SCI_CONVERTEOLS, static_cast<WPARAM>(eolMode), 0, bDirect);
}

int CCT3000CO2_NET::GetEOLMode(BOOL bDirect)
{
  return Call(SCI_GETEOLMODE, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetEOLMode(int eolMode, BOOL bDirect)
{
  Call(SCI_SETEOLMODE, static_cast<WPARAM>(eolMode), 0, bDirect);
}

void CCT3000CO2_NET::StartStyling(long pos, int mask, BOOL bDirect)
{
  Call(SCI_STARTSTYLING, static_cast<WPARAM>(pos), static_cast<LPARAM>(mask), bDirect);
}

void CCT3000CO2_NET::SetStyling(int length, int style, BOOL bDirect)
{
  Call(SCI_SETSTYLING, static_cast<WPARAM>(length), static_cast<LPARAM>(style), bDirect);
}

BOOL CCT3000CO2_NET::GetBufferedDraw(BOOL bDirect)
{
  return Call(SCI_GETBUFFEREDDRAW, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetBufferedDraw(BOOL buffered, BOOL bDirect)
{
  Call(SCI_SETBUFFEREDDRAW, static_cast<WPARAM>(buffered), 0, bDirect);
}

void CCT3000CO2_NET::SetTabWidth(int tabWidth, BOOL bDirect)
{
  Call(SCI_SETTABWIDTH, static_cast<WPARAM>(tabWidth), 0, bDirect);
}

int CCT3000CO2_NET::GetTabWidth(BOOL bDirect)
{
  return Call(SCI_GETTABWIDTH, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetCodePage(int codePage, BOOL bDirect)
{
  Call(SCI_SETCODEPAGE, static_cast<WPARAM>(codePage), 0, bDirect);
}

void CCT3000CO2_NET::SetUsePalette(BOOL usePalette, BOOL bDirect)
{
  Call(SCI_SETUSEPALETTE, static_cast<WPARAM>(usePalette), 0, bDirect);
}

void CCT3000CO2_NET::MarkerDefine(int markerNumber, int markerSymbol, BOOL bDirect)
{
  Call(SCI_MARKERDEFINE, static_cast<WPARAM>(markerNumber), static_cast<LPARAM>(markerSymbol), bDirect);
}

void CCT3000CO2_NET::MarkerSetFore(int markerNumber, COLORREF fore, BOOL bDirect)
{
  Call(SCI_MARKERSETFORE, static_cast<WPARAM>(markerNumber), static_cast<LPARAM>(fore), bDirect);
}

void CCT3000CO2_NET::MarkerSetBack(int markerNumber, COLORREF back, BOOL bDirect)
{
  Call(SCI_MARKERSETBACK, static_cast<WPARAM>(markerNumber), static_cast<LPARAM>(back), bDirect);
}

int CCT3000CO2_NET::MarkerAdd(int line, int markerNumber, BOOL bDirect)
{
  return Call(SCI_MARKERADD, static_cast<WPARAM>(line), static_cast<LPARAM>(markerNumber), bDirect);
}

void CCT3000CO2_NET::MarkerDelete(int line, int markerNumber, BOOL bDirect)
{
  Call(SCI_MARKERDELETE, static_cast<WPARAM>(line), static_cast<LPARAM>(markerNumber), bDirect);
}

void CCT3000CO2_NET::MarkerDeleteAll(int markerNumber, BOOL bDirect)
{
  Call(SCI_MARKERDELETEALL, static_cast<WPARAM>(markerNumber), 0, bDirect);
}

int CCT3000CO2_NET::MarkerGet(int line, BOOL bDirect)
{
  return Call(SCI_MARKERGET, static_cast<WPARAM>(line), 0, bDirect);
}

int CCT3000CO2_NET::MarkerNext(int lineStart, int markerMask, BOOL bDirect)
{
  return Call(SCI_MARKERNEXT, static_cast<WPARAM>(lineStart), static_cast<LPARAM>(markerMask), bDirect);
}

int CCT3000CO2_NET::MarkerPrevious(int lineStart, int markerMask, BOOL bDirect)
{
  return Call(SCI_MARKERPREVIOUS, static_cast<WPARAM>(lineStart), static_cast<LPARAM>(markerMask), bDirect);
}

void CCT3000CO2_NET::MarkerDefinePixmap(int markerNumber, const char* pixmap, BOOL bDirect)
{
  Call(SCI_MARKERDEFINEPIXMAP, static_cast<WPARAM>(markerNumber), reinterpret_cast<LPARAM>(pixmap), bDirect);
}

void CCT3000CO2_NET::MarkerAddSet(int line, int set, BOOL bDirect)
{
  Call(SCI_MARKERADDSET, static_cast<WPARAM>(line), static_cast<LPARAM>(set), bDirect);
}

void CCT3000CO2_NET::MarkerSetAlpha(int markerNumber, int alpha, BOOL bDirect)
{
  Call(SCI_MARKERSETALPHA, static_cast<WPARAM>(markerNumber), static_cast<LPARAM>(alpha), bDirect);
}

void CCT3000CO2_NET::SetMarginTypeN(int margin, int marginType, BOOL bDirect)
{
  Call(SCI_SETMARGINTYPEN, static_cast<WPARAM>(margin), static_cast<LPARAM>(marginType), bDirect);
}

int CCT3000CO2_NET::GetMarginTypeN(int margin, BOOL bDirect)
{
  return Call(SCI_GETMARGINTYPEN, static_cast<WPARAM>(margin), 0, bDirect);
}

void CCT3000CO2_NET::SetMarginWidthN(int margin, int pixelWidth, BOOL bDirect)
{
  Call(SCI_SETMARGINWIDTHN, static_cast<WPARAM>(margin), static_cast<LPARAM>(pixelWidth), bDirect);
}

int CCT3000CO2_NET::GetMarginWidthN(int margin, BOOL bDirect)
{
  return Call(SCI_GETMARGINWIDTHN, static_cast<WPARAM>(margin), 0, bDirect);
}

void CCT3000CO2_NET::SetMarginMaskN(int margin, int mask, BOOL bDirect)
{
  Call(SCI_SETMARGINMASKN, static_cast<WPARAM>(margin), static_cast<LPARAM>(mask), bDirect);
}

int CCT3000CO2_NET::GetMarginMaskN(int margin, BOOL bDirect)
{
  return Call(SCI_GETMARGINMASKN, static_cast<WPARAM>(margin), 0, bDirect);
}

void CCT3000CO2_NET::SetMarginSensitiveN(int margin, BOOL sensitive, BOOL bDirect)
{
  Call(SCI_SETMARGINSENSITIVEN, static_cast<WPARAM>(margin), static_cast<LPARAM>(sensitive), bDirect);
}

BOOL CCT3000CO2_NET::GetMarginSensitiveN(int margin, BOOL bDirect)
{
  return Call(SCI_GETMARGINSENSITIVEN, static_cast<WPARAM>(margin), 0, bDirect);
}

void CCT3000CO2_NET::StyleClearAll(BOOL bDirect)
{
  Call(SCI_STYLECLEARALL, 0, 0, bDirect);
}

void CCT3000CO2_NET::StyleSetFore(int style, COLORREF fore, BOOL bDirect)
{
  Call(SCI_STYLESETFORE, static_cast<WPARAM>(style), static_cast<LPARAM>(fore), bDirect);
}

void CCT3000CO2_NET::StyleSetBack(int style, COLORREF back, BOOL bDirect)
{
  Call(SCI_STYLESETBACK, static_cast<WPARAM>(style), static_cast<LPARAM>(back), bDirect);
}

void CCT3000CO2_NET::StyleSetBold(int style, BOOL bold, BOOL bDirect)
{
  Call(SCI_STYLESETBOLD, static_cast<WPARAM>(style), static_cast<LPARAM>(bold), bDirect);
}

void CCT3000CO2_NET::StyleSetItalic(int style, BOOL italic, BOOL bDirect)
{
  Call(SCI_STYLESETITALIC, static_cast<WPARAM>(style), static_cast<LPARAM>(italic), bDirect);
}

void CCT3000CO2_NET::StyleSetSize(int style, int sizePoints, BOOL bDirect)
{
  Call(SCI_STYLESETSIZE, static_cast<WPARAM>(style), static_cast<LPARAM>(sizePoints), bDirect);
}

void CCT3000CO2_NET::StyleSetFont(int style, const char* fontName, BOOL bDirect)
{
  Call(SCI_STYLESETFONT, static_cast<WPARAM>(style), reinterpret_cast<LPARAM>(fontName), bDirect);
}

void CCT3000CO2_NET::StyleSetEOLFilled(int style, BOOL filled, BOOL bDirect)
{
  Call(SCI_STYLESETEOLFILLED, static_cast<WPARAM>(style), static_cast<LPARAM>(filled), bDirect);
}

void CCT3000CO2_NET::StyleResetDefault(BOOL bDirect)
{
  Call(SCI_STYLERESETDEFAULT, 0, 0, bDirect);
}

void CCT3000CO2_NET::StyleSetUnderline(int style, BOOL underline, BOOL bDirect)
{
  Call(SCI_STYLESETUNDERLINE, static_cast<WPARAM>(style), static_cast<LPARAM>(underline), bDirect);
}

COLORREF CCT3000CO2_NET::StyleGetFore(int style, BOOL bDirect)
{
  return Call(SCI_STYLEGETFORE, static_cast<WPARAM>(style), 0, bDirect);
}

COLORREF CCT3000CO2_NET::StyleGetBack(int style, BOOL bDirect)
{
  return Call(SCI_STYLEGETBACK, static_cast<WPARAM>(style), 0, bDirect);
}

BOOL CCT3000CO2_NET::StyleGetBold(int style, BOOL bDirect)
{
  return Call(SCI_STYLEGETBOLD, static_cast<WPARAM>(style), 0, bDirect);
}

BOOL CCT3000CO2_NET::StyleGetItalic(int style, BOOL bDirect)
{
  return Call(SCI_STYLEGETITALIC, static_cast<WPARAM>(style), 0, bDirect);
}

int CCT3000CO2_NET::StyleGetSize(int style, BOOL bDirect)
{
  return Call(SCI_STYLEGETSIZE, static_cast<WPARAM>(style), 0, bDirect);
}

int CCT3000CO2_NET::StyleGetFont(int style, char* fontName, BOOL bDirect)
{
  return Call(SCI_STYLEGETFONT, static_cast<WPARAM>(style), reinterpret_cast<LPARAM>(fontName), bDirect);
}

BOOL CCT3000CO2_NET::StyleGetEOLFilled(int style, BOOL bDirect)
{
  return Call(SCI_STYLEGETEOLFILLED, static_cast<WPARAM>(style), 0, bDirect);
}

BOOL CCT3000CO2_NET::StyleGetUnderline(int style, BOOL bDirect)
{
  return Call(SCI_STYLEGETUNDERLINE, static_cast<WPARAM>(style), 0, bDirect);
}

int CCT3000CO2_NET::StyleGetCase(int style, BOOL bDirect)
{
  return Call(SCI_STYLEGETCASE, static_cast<WPARAM>(style), 0, bDirect);
}

int CCT3000CO2_NET::StyleGetCharacterSet(int style, BOOL bDirect)
{
  return Call(SCI_STYLEGETCHARACTERSET, static_cast<WPARAM>(style), 0, bDirect);
}

BOOL CCT3000CO2_NET::StyleGetVisible(int style, BOOL bDirect)
{
  return Call(SCI_STYLEGETVISIBLE, static_cast<WPARAM>(style), 0, bDirect);
}

BOOL CCT3000CO2_NET::StyleGetChangeable(int style, BOOL bDirect)
{
  return Call(SCI_STYLEGETCHANGEABLE, static_cast<WPARAM>(style), 0, bDirect);
}

BOOL CCT3000CO2_NET::StyleGetHotSpot(int style, BOOL bDirect)
{
  return Call(SCI_STYLEGETHOTSPOT, static_cast<WPARAM>(style), 0, bDirect);
}

void CCT3000CO2_NET::StyleSetCase(int style, int caseForce, BOOL bDirect)
{
  Call(SCI_STYLESETCASE, static_cast<WPARAM>(style), static_cast<LPARAM>(caseForce), bDirect);
}

void CCT3000CO2_NET::StyleSetCharacterSet(int style, int characterSet, BOOL bDirect)
{
  Call(SCI_STYLESETCHARACTERSET, static_cast<WPARAM>(style), static_cast<LPARAM>(characterSet), bDirect);
}

void CCT3000CO2_NET::StyleSetHotSpot(int style, BOOL hotspot, BOOL bDirect)
{
  Call(SCI_STYLESETHOTSPOT, static_cast<WPARAM>(style), static_cast<LPARAM>(hotspot), bDirect);
}

void CCT3000CO2_NET::SetSelFore(BOOL useSetting, COLORREF fore, BOOL bDirect)
{
  Call(SCI_SETSELFORE, static_cast<WPARAM>(useSetting), static_cast<LPARAM>(fore), bDirect);
}

void CCT3000CO2_NET::SetSelBack(BOOL useSetting, COLORREF back, BOOL bDirect)
{
  Call(SCI_SETSELBACK, static_cast<WPARAM>(useSetting), static_cast<LPARAM>(back), bDirect);
}

int CCT3000CO2_NET::GetSelAlpha(BOOL bDirect)
{
  return Call(SCI_GETSELALPHA, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetSelAlpha(int alpha, BOOL bDirect)
{
  Call(SCI_SETSELALPHA, static_cast<WPARAM>(alpha), 0, bDirect);
}

BOOL CCT3000CO2_NET::GetSelEOLFilled(BOOL bDirect)
{
  return Call(SCI_GETSELEOLFILLED, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetSelEOLFilled(BOOL filled, BOOL bDirect)
{
  Call(SCI_SETSELEOLFILLED, static_cast<WPARAM>(filled), 0, bDirect);
}

void CCT3000CO2_NET::SetCaretFore(COLORREF fore, BOOL bDirect)
{
  Call(SCI_SETCARETFORE, static_cast<WPARAM>(fore), 0, bDirect);
}

void CCT3000CO2_NET::AssignCmdKey(DWORD km, int msg, BOOL bDirect)
{
  Call(SCI_ASSIGNCMDKEY, static_cast<WPARAM>(km), static_cast<LPARAM>(msg), bDirect);
}

void CCT3000CO2_NET::ClearCmdKey(DWORD km, BOOL bDirect)
{
  Call(SCI_CLEARCMDKEY, static_cast<WPARAM>(km), 0, bDirect);
}

void CCT3000CO2_NET::ClearAllCmdKeys(BOOL bDirect)
{
  Call(SCI_CLEARALLCMDKEYS, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetStylingEx(int length, const char* styles, BOOL bDirect)
{
  Call(SCI_SETSTYLINGEX, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(styles), bDirect);
}

void CCT3000CO2_NET::StyleSetVisible(int style, BOOL visible, BOOL bDirect)
{
  Call(SCI_STYLESETVISIBLE, static_cast<WPARAM>(style), static_cast<LPARAM>(visible), bDirect);
}

int CCT3000CO2_NET::GetCaretPeriod(BOOL bDirect)
{
  return Call(SCI_GETCARETPERIOD, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetCaretPeriod(int periodMilliseconds, BOOL bDirect)
{
  Call(SCI_SETCARETPERIOD, static_cast<WPARAM>(periodMilliseconds), 0, bDirect);
}

void CCT3000CO2_NET::SetWordChars(const char* characters, BOOL bDirect)
{
  Call(SCI_SETWORDCHARS, 0, reinterpret_cast<LPARAM>(characters), bDirect);
}

void CCT3000CO2_NET::BeginUndoAction(BOOL bDirect)
{
  Call(SCI_BEGINUNDOACTION, 0, 0, bDirect);
}

void CCT3000CO2_NET::EndUndoAction(BOOL bDirect)
{
  Call(SCI_ENDUNDOACTION, 0, 0, bDirect);
}

void CCT3000CO2_NET::IndicSetStyle(int indic, int style, BOOL bDirect)
{
  Call(SCI_INDICSETSTYLE, static_cast<WPARAM>(indic), static_cast<LPARAM>(style), bDirect);
}

int CCT3000CO2_NET::IndicGetStyle(int indic, BOOL bDirect)
{
  return Call(SCI_INDICGETSTYLE, static_cast<WPARAM>(indic), 0, bDirect);
}

void CCT3000CO2_NET::IndicSetFore(int indic, COLORREF fore, BOOL bDirect)
{
  Call(SCI_INDICSETFORE, static_cast<WPARAM>(indic), static_cast<LPARAM>(fore), bDirect);
}

COLORREF CCT3000CO2_NET::IndicGetFore(int indic, BOOL bDirect)
{
  return Call(SCI_INDICGETFORE, static_cast<WPARAM>(indic), 0, bDirect);
}

void CCT3000CO2_NET::IndicSetUnder(int indic, BOOL under, BOOL bDirect)
{
  Call(SCI_INDICSETUNDER, static_cast<WPARAM>(indic), static_cast<LPARAM>(under), bDirect);
}

BOOL CCT3000CO2_NET::IndicGetUnder(int indic, BOOL bDirect)
{
  return Call(SCI_INDICGETUNDER, static_cast<WPARAM>(indic), 0, bDirect);
}

void CCT3000CO2_NET::SetWhitespaceFore(BOOL useSetting, COLORREF fore, BOOL bDirect)
{
  Call(SCI_SETWHITESPACEFORE, static_cast<WPARAM>(useSetting), static_cast<LPARAM>(fore), bDirect);
}

void CCT3000CO2_NET::SetWhitespaceBack(BOOL useSetting, COLORREF back, BOOL bDirect)
{
  Call(SCI_SETWHITESPACEBACK, static_cast<WPARAM>(useSetting), static_cast<LPARAM>(back), bDirect);
}

void CCT3000CO2_NET::SetStyleBits(int bits, BOOL bDirect)
{
  Call(SCI_SETSTYLEBITS, static_cast<WPARAM>(bits), 0, bDirect);
}

int CCT3000CO2_NET::GetStyleBits(BOOL bDirect)
{
  return Call(SCI_GETSTYLEBITS, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetLineState(int line, int state, BOOL bDirect)
{
  Call(SCI_SETLINESTATE, static_cast<WPARAM>(line), static_cast<LPARAM>(state), bDirect);
}

int CCT3000CO2_NET::GetLineState(int line, BOOL bDirect)
{
  return Call(SCI_GETLINESTATE, static_cast<WPARAM>(line), 0, bDirect);
}

int CCT3000CO2_NET::GetMaxLineState(BOOL bDirect)
{
  return Call(SCI_GETMAXLINESTATE, 0, 0, bDirect);
}

BOOL CCT3000CO2_NET::GetCaretLineVisible(BOOL bDirect)
{
  return Call(SCI_GETCARETLINEVISIBLE, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetCaretLineVisible(BOOL show, BOOL bDirect)
{
  Call(SCI_SETCARETLINEVISIBLE, static_cast<WPARAM>(show), 0, bDirect);
}

COLORREF CCT3000CO2_NET::GetCaretLineBack(BOOL bDirect)
{
  return Call(SCI_GETCARETLINEBACK, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetCaretLineBack(COLORREF back, BOOL bDirect)
{
  Call(SCI_SETCARETLINEBACK, static_cast<WPARAM>(back), 0, bDirect);
}

void CCT3000CO2_NET::StyleSetChangeable(int style, BOOL changeable, BOOL bDirect)
{
  Call(SCI_STYLESETCHANGEABLE, static_cast<WPARAM>(style), static_cast<LPARAM>(changeable), bDirect);
}

void CCT3000CO2_NET::AutoCShow(int lenEntered, const char* itemList, BOOL bDirect)
{
  Call(SCI_AUTOCSHOW, static_cast<WPARAM>(lenEntered), reinterpret_cast<LPARAM>(itemList), bDirect);
}

void CCT3000CO2_NET::AutoCCancel(BOOL bDirect)
{
  Call(SCI_AUTOCCANCEL, 0, 0, bDirect);
}

BOOL CCT3000CO2_NET::AutoCActive(BOOL bDirect)
{
  return Call(SCI_AUTOCACTIVE, 0, 0, bDirect);
}

long CCT3000CO2_NET::AutoCPosStart(BOOL bDirect)
{
  return Call(SCI_AUTOCPOSSTART, 0, 0, bDirect);
}

void CCT3000CO2_NET::AutoCComplete(BOOL bDirect)
{
  Call(SCI_AUTOCCOMPLETE, 0, 0, bDirect);
}

void CCT3000CO2_NET::AutoCStops(const char* characterSet, BOOL bDirect)
{
  Call(SCI_AUTOCSTOPS, 0, reinterpret_cast<LPARAM>(characterSet), bDirect);
}

void CCT3000CO2_NET::AutoCSetSeparator(int separatorCharacter, BOOL bDirect)
{
  Call(SCI_AUTOCSETSEPARATOR, static_cast<WPARAM>(separatorCharacter), 0, bDirect);
}

int CCT3000CO2_NET::AutoCGetSeparator(BOOL bDirect)
{
  return Call(SCI_AUTOCGETSEPARATOR, 0, 0, bDirect);
}

void CCT3000CO2_NET::AutoCSelect(const char* text, BOOL bDirect)
{
  Call(SCI_AUTOCSELECT, 0, reinterpret_cast<LPARAM>(text), bDirect);
}

void CCT3000CO2_NET::AutoCSetCancelAtStart(BOOL cancel, BOOL bDirect)
{
  Call(SCI_AUTOCSETCANCELATSTART, static_cast<WPARAM>(cancel), 0, bDirect);
}

BOOL CCT3000CO2_NET::AutoCGetCancelAtStart(BOOL bDirect)
{
  return Call(SCI_AUTOCGETCANCELATSTART, 0, 0, bDirect);
}

void CCT3000CO2_NET::AutoCSetFillUps(const char* characterSet, BOOL bDirect)
{
  Call(SCI_AUTOCSETFILLUPS, 0, reinterpret_cast<LPARAM>(characterSet), bDirect);
}

void CCT3000CO2_NET::AutoCSetChooseSingle(BOOL chooseSingle, BOOL bDirect)
{
  Call(SCI_AUTOCSETCHOOSESINGLE, static_cast<WPARAM>(chooseSingle), 0, bDirect);
}

BOOL CCT3000CO2_NET::AutoCGetChooseSingle(BOOL bDirect)
{
  return Call(SCI_AUTOCGETCHOOSESINGLE, 0, 0, bDirect);
}

void CCT3000CO2_NET::AutoCSetIgnoreCase(BOOL ignoreCase, BOOL bDirect)
{
  Call(SCI_AUTOCSETIGNORECASE, static_cast<WPARAM>(ignoreCase), 0, bDirect);
}

BOOL CCT3000CO2_NET::AutoCGetIgnoreCase(BOOL bDirect)
{
  return Call(SCI_AUTOCGETIGNORECASE, 0, 0, bDirect);
}

void CCT3000CO2_NET::UserListShow(int listType, const char* itemList, BOOL bDirect)
{
  Call(SCI_USERLISTSHOW, static_cast<WPARAM>(listType), reinterpret_cast<LPARAM>(itemList), bDirect);
}

void CCT3000CO2_NET::AutoCSetAutoHide(BOOL autoHide, BOOL bDirect)
{
  Call(SCI_AUTOCSETAUTOHIDE, static_cast<WPARAM>(autoHide), 0, bDirect);
}

BOOL CCT3000CO2_NET::AutoCGetAutoHide(BOOL bDirect)
{
  return Call(SCI_AUTOCGETAUTOHIDE, 0, 0, bDirect);
}

void CCT3000CO2_NET::AutoCSetDropRestOfWord(BOOL dropRestOfWord, BOOL bDirect)
{
  Call(SCI_AUTOCSETDROPRESTOFWORD, static_cast<WPARAM>(dropRestOfWord), 0, bDirect);
}

BOOL CCT3000CO2_NET::AutoCGetDropRestOfWord(BOOL bDirect)
{
  return Call(SCI_AUTOCGETDROPRESTOFWORD, 0, 0, bDirect);
}

void CCT3000CO2_NET::RegisterImage(int type, const char* xpmData, BOOL bDirect)
{
  Call(SCI_REGISTERIMAGE, static_cast<WPARAM>(type), reinterpret_cast<LPARAM>(xpmData), bDirect);
}

void CCT3000CO2_NET::ClearRegisteredImages(BOOL bDirect)
{
  Call(SCI_CLEARREGISTEREDIMAGES, 0, 0, bDirect);
}

int CCT3000CO2_NET::AutoCGetTypeSeparator(BOOL bDirect)
{
  return Call(SCI_AUTOCGETTYPESEPARATOR, 0, 0, bDirect);
}

void CCT3000CO2_NET::AutoCSetTypeSeparator(int separatorCharacter, BOOL bDirect)
{
  Call(SCI_AUTOCSETTYPESEPARATOR, static_cast<WPARAM>(separatorCharacter), 0, bDirect);
}

void CCT3000CO2_NET::AutoCSetMaxWidth(int characterCount, BOOL bDirect)
{
  Call(SCI_AUTOCSETMAXWIDTH, static_cast<WPARAM>(characterCount), 0, bDirect);
}

int CCT3000CO2_NET::AutoCGetMaxWidth(BOOL bDirect)
{
  return Call(SCI_AUTOCGETMAXWIDTH, 0, 0, bDirect);
}

void CCT3000CO2_NET::AutoCSetMaxHeight(int rowCount, BOOL bDirect)
{
  Call(SCI_AUTOCSETMAXHEIGHT, static_cast<WPARAM>(rowCount), 0, bDirect);
}

int CCT3000CO2_NET::AutoCGetMaxHeight(BOOL bDirect)
{
  return Call(SCI_AUTOCGETMAXHEIGHT, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetIndent(int indentSize, BOOL bDirect)
{
  Call(SCI_SETINDENT, static_cast<WPARAM>(indentSize), 0, bDirect);
}

int CCT3000CO2_NET::GetIndent(BOOL bDirect)
{
  return Call(SCI_GETINDENT, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetUseTabs(BOOL useTabs, BOOL bDirect)
{
  Call(SCI_SETUSETABS, static_cast<WPARAM>(useTabs), 0, bDirect);
}

BOOL CCT3000CO2_NET::GetUseTabs(BOOL bDirect)
{
  return Call(SCI_GETUSETABS, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetLineIndentation(int line, int indentSize, BOOL bDirect)
{
  Call(SCI_SETLINEINDENTATION, static_cast<WPARAM>(line), static_cast<LPARAM>(indentSize), bDirect);
}

int CCT3000CO2_NET::GetLineIndentation(int line, BOOL bDirect)
{
  return Call(SCI_GETLINEINDENTATION, static_cast<WPARAM>(line), 0, bDirect);
}

long CCT3000CO2_NET::GetLineIndentPosition(int line, BOOL bDirect)
{
  return Call(SCI_GETLINEINDENTPOSITION, static_cast<WPARAM>(line), 0, bDirect);
}

int CCT3000CO2_NET::GetColumn(long pos, BOOL bDirect)
{
  return Call(SCI_GETCOLUMN, static_cast<WPARAM>(pos), 0, bDirect);
}

void CCT3000CO2_NET::SetHScrollBar(BOOL show, BOOL bDirect)
{
  Call(SCI_SETHSCROLLBAR, static_cast<WPARAM>(show), 0, bDirect);
}

BOOL CCT3000CO2_NET::GetHScrollBar(BOOL bDirect)
{
  return Call(SCI_GETHSCROLLBAR, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetIndentationGuides(int indentView, BOOL bDirect)
{
  Call(SCI_SETINDENTATIONGUIDES, static_cast<WPARAM>(indentView), 0, bDirect);
}

int CCT3000CO2_NET::GetIndentationGuides(BOOL bDirect)
{
  return Call(SCI_GETINDENTATIONGUIDES, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetHighlightGuide(int column, BOOL bDirect)
{
  Call(SCI_SETHIGHLIGHTGUIDE, static_cast<WPARAM>(column), 0, bDirect);
}

int CCT3000CO2_NET::GetHighlightGuide(BOOL bDirect)
{
  return Call(SCI_GETHIGHLIGHTGUIDE, 0, 0, bDirect);
}

int CCT3000CO2_NET::GetLineEndPosition(int line, BOOL bDirect)
{
  return Call(SCI_GETLINEENDPOSITION, static_cast<WPARAM>(line), 0, bDirect);
}

int CCT3000CO2_NET::GetCodePage(BOOL bDirect)
{
  return Call(SCI_GETCODEPAGE, 0, 0, bDirect);
}

COLORREF CCT3000CO2_NET::GetCaretFore(BOOL bDirect)
{
  return Call(SCI_GETCARETFORE, 0, 0, bDirect);
}

BOOL CCT3000CO2_NET::GetUsePalette(BOOL bDirect)
{
  return Call(SCI_GETUSEPALETTE, 0, 0, bDirect);
}

BOOL CCT3000CO2_NET::GetReadOnly(BOOL bDirect)
{
  return Call(SCI_GETREADONLY, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetCurrentPos(long pos, BOOL bDirect)
{
  Call(SCI_SETCURRENTPOS, static_cast<WPARAM>(pos), 0, bDirect);
}

void CCT3000CO2_NET::SetSelectionStart(long pos, BOOL bDirect)
{
  Call(SCI_SETSELECTIONSTART, static_cast<WPARAM>(pos), 0, bDirect);
}

long CCT3000CO2_NET::GetSelectionStart(BOOL bDirect)
{
  return Call(SCI_GETSELECTIONSTART, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetSelectionEnd(long pos, BOOL bDirect)
{
  Call(SCI_SETSELECTIONEND, static_cast<WPARAM>(pos), 0, bDirect);
}

long CCT3000CO2_NET::GetSelectionEnd(BOOL bDirect)
{
  return Call(SCI_GETSELECTIONEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetPrintMagnification(int magnification, BOOL bDirect)
{
  Call(SCI_SETPRINTMAGNIFICATION, static_cast<WPARAM>(magnification), 0, bDirect);
}

int CCT3000CO2_NET::GetPrintMagnification(BOOL bDirect)
{
  return Call(SCI_GETPRINTMAGNIFICATION, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetPrintColourMode(int mode, BOOL bDirect)
{
  Call(SCI_SETPRINTCOLOURMODE, static_cast<WPARAM>(mode), 0, bDirect);
}

int CCT3000CO2_NET::GetPrintColourMode(BOOL bDirect)
{
  return Call(SCI_GETPRINTCOLOURMODE, 0, 0, bDirect);
}

long CCT3000CO2_NET::FindText(int flags, TextToFind* ft, BOOL bDirect)
{
  return Call(SCI_FINDTEXT, static_cast<WPARAM>(flags), reinterpret_cast<LPARAM>(ft), bDirect);
}

long CCT3000CO2_NET::FormatRange(BOOL draw, RangeToFormat* fr, BOOL bDirect)
{
  return Call(SCI_FORMATRANGE, static_cast<WPARAM>(draw), reinterpret_cast<LPARAM>(fr), bDirect);
}

int CCT3000CO2_NET::GetFirstVisibleLine(BOOL bDirect)
{
  return Call(SCI_GETFIRSTVISIBLELINE, 0, 0, bDirect);
}

int CCT3000CO2_NET::GetLine(int line, char* text, BOOL bDirect)
{
  return Call(SCI_GETLINE, static_cast<WPARAM>(line), reinterpret_cast<LPARAM>(text), bDirect);
}

int CCT3000CO2_NET::GetLineCount(BOOL bDirect)
{
  return Call(SCI_GETLINECOUNT, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetMarginLeft(int pixelWidth, BOOL bDirect)
{
  Call(SCI_SETMARGINLEFT, 0, static_cast<LPARAM>(pixelWidth), bDirect);
}

int CCT3000CO2_NET::GetMarginLeft(BOOL bDirect)
{
  return Call(SCI_GETMARGINLEFT, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetMarginRight(int pixelWidth, BOOL bDirect)
{
  Call(SCI_SETMARGINRIGHT, 0, static_cast<LPARAM>(pixelWidth), bDirect);
}

int CCT3000CO2_NET::GetMarginRight(BOOL bDirect)
{
  return Call(SCI_GETMARGINRIGHT, 0, 0, bDirect);
}

BOOL CCT3000CO2_NET::GetModify(BOOL bDirect)
{
  return Call(SCI_GETMODIFY, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetSel(long start, long end, BOOL bDirect)
{
  Call(SCI_SETSEL, static_cast<WPARAM>(start), static_cast<LPARAM>(end), bDirect);
}

int CCT3000CO2_NET::GetSelText(char* text, BOOL bDirect)
{
  return Call(SCI_GETSELTEXT, 0, reinterpret_cast<LPARAM>(text), bDirect);
}

int CCT3000CO2_NET::GetTextRange(TextRange* tr, BOOL bDirect)
{
  return Call(SCI_GETTEXTRANGE, 0, reinterpret_cast<LPARAM>(tr), bDirect);
}

void CCT3000CO2_NET::HideSelection(BOOL normal, BOOL bDirect)
{
  Call(SCI_HIDESELECTION, static_cast<WPARAM>(normal), 0, bDirect);
}

int CCT3000CO2_NET::PointXFromPosition(long pos, BOOL bDirect)
{
  return Call(SCI_POINTXFROMPOSITION, 0, static_cast<LPARAM>(pos), bDirect);
}

int CCT3000CO2_NET::PointYFromPosition(long pos, BOOL bDirect)
{
  return Call(SCI_POINTYFROMPOSITION, 0, static_cast<LPARAM>(pos), bDirect);
}

int CCT3000CO2_NET::LineFromPosition(long pos, BOOL bDirect)
{
  return Call(SCI_LINEFROMPOSITION, static_cast<WPARAM>(pos), 0, bDirect);
}

long CCT3000CO2_NET::PositionFromLine(int line, BOOL bDirect)
{
  return Call(SCI_POSITIONFROMLINE, static_cast<WPARAM>(line), 0, bDirect);
}

void CCT3000CO2_NET::LineScroll(int columns, int lines, BOOL bDirect)
{
  Call(SCI_LINESCROLL, static_cast<WPARAM>(columns), static_cast<LPARAM>(lines), bDirect);
}

void CCT3000CO2_NET::ScrollCaret(BOOL bDirect)
{
  Call(SCI_SCROLLCARET, 0, 0, bDirect);
}

void CCT3000CO2_NET::ReplaceSel(const char* text, BOOL bDirect)
{
  Call(SCI_REPLACESEL, 0, reinterpret_cast<LPARAM>(text), bDirect);
}

void CCT3000CO2_NET::SetReadOnly(BOOL readOnly, BOOL bDirect)
{
  Call(SCI_SETREADONLY, static_cast<WPARAM>(readOnly), 0, bDirect);
}

void CCT3000CO2_NET::Null(BOOL bDirect)
{
  Call(SCI_NULL, 0, 0, bDirect);
}

BOOL CCT3000CO2_NET::CanPaste(BOOL bDirect)
{
  return Call(SCI_CANPASTE, 0, 0, bDirect);
}

BOOL CCT3000CO2_NET::CanUndo(BOOL bDirect)
{
  return Call(SCI_CANUNDO, 0, 0, bDirect);
}

void CCT3000CO2_NET::EmptyUndoBuffer(BOOL bDirect)
{
  Call(SCI_EMPTYUNDOBUFFER, 0, 0, bDirect);
}

void CCT3000CO2_NET::Undo(BOOL bDirect)
{
  Call(SCI_UNDO, 0, 0, bDirect);
}

void CCT3000CO2_NET::Cut(BOOL bDirect)
{
  Call(SCI_CUT, 0, 0, bDirect);
}

void CCT3000CO2_NET::Copy(BOOL bDirect)
{
  Call(SCI_COPY, 0, 0, bDirect);
}

void CCT3000CO2_NET::Paste(BOOL bDirect)
{
  Call(SCI_PASTE, 0, 0, bDirect);
}

void CCT3000CO2_NET::Clear(BOOL bDirect)
{
  Call(SCI_CLEAR, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetText(const char* text, BOOL bDirect)
{
  Call(SCI_SETTEXT, 0, reinterpret_cast<LPARAM>(text), bDirect);
}

int CCT3000CO2_NET::GetText(int length, char* text, BOOL bDirect)
{
  return Call(SCI_GETTEXT, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text), bDirect);
}

int CCT3000CO2_NET::GetTextLength(BOOL bDirect)
{
  return Call(SCI_GETTEXTLENGTH, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetOvertype(BOOL overtype, BOOL bDirect)
{
  Call(SCI_SETOVERTYPE, static_cast<WPARAM>(overtype), 0, bDirect);
}

BOOL CCT3000CO2_NET::GetOvertype(BOOL bDirect)
{
  return Call(SCI_GETOVERTYPE, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetCaretWidth(int pixelWidth, BOOL bDirect)
{
  Call(SCI_SETCARETWIDTH, static_cast<WPARAM>(pixelWidth), 0, bDirect);
}

int CCT3000CO2_NET::GetCaretWidth(BOOL bDirect)
{
  return Call(SCI_GETCARETWIDTH, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetTargetStart(long pos, BOOL bDirect)
{
  Call(SCI_SETTARGETSTART, static_cast<WPARAM>(pos), 0, bDirect);
}

long CCT3000CO2_NET::GetTargetStart(BOOL bDirect)
{
  return Call(SCI_GETTARGETSTART, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetTargetEnd(long pos, BOOL bDirect)
{
  Call(SCI_SETTARGETEND, static_cast<WPARAM>(pos), 0, bDirect);
}

long CCT3000CO2_NET::GetTargetEnd(BOOL bDirect)
{
  return Call(SCI_GETTARGETEND, 0, 0, bDirect);
}

int CCT3000CO2_NET::ReplaceTarget(int length, const char* text, BOOL bDirect)
{
  return Call(SCI_REPLACETARGET, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text), bDirect);
}

int CCT3000CO2_NET::ReplaceTargetRE(int length, const char* text, BOOL bDirect)
{
  return Call(SCI_REPLACETARGETRE, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text), bDirect);
}

int CCT3000CO2_NET::SearchInTarget(int length, const char* text, BOOL bDirect)
{
  return Call(SCI_SEARCHINTARGET, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text), bDirect);
}

void CCT3000CO2_NET::SetSearchFlags(int flags, BOOL bDirect)
{
  Call(SCI_SETSEARCHFLAGS, static_cast<WPARAM>(flags), 0, bDirect);
}

int CCT3000CO2_NET::GetSearchFlags(BOOL bDirect)
{
  return Call(SCI_GETSEARCHFLAGS, 0, 0, bDirect);
}

void CCT3000CO2_NET::CallTipShow(long pos, const char* definition, BOOL bDirect)
{
  Call(SCI_CALLTIPSHOW, static_cast<WPARAM>(pos), reinterpret_cast<LPARAM>(definition), bDirect);
}

void CCT3000CO2_NET::CallTipCancel(BOOL bDirect)
{
  Call(SCI_CALLTIPCANCEL, 0, 0, bDirect);
}

BOOL CCT3000CO2_NET::CallTipActive(BOOL bDirect)
{
  return Call(SCI_CALLTIPACTIVE, 0, 0, bDirect);
}

long CCT3000CO2_NET::CallTipPosStart(BOOL bDirect)
{
  return Call(SCI_CALLTIPPOSSTART, 0, 0, bDirect);
}

void CCT3000CO2_NET::CallTipSetHlt(int start, int end, BOOL bDirect)
{
  Call(SCI_CALLTIPSETHLT, static_cast<WPARAM>(start), static_cast<LPARAM>(end), bDirect);
}

void CCT3000CO2_NET::CallTipSetBack(COLORREF back, BOOL bDirect)
{
  Call(SCI_CALLTIPSETBACK, static_cast<WPARAM>(back), 0, bDirect);
}

void CCT3000CO2_NET::CallTipSetFore(COLORREF fore, BOOL bDirect)
{
  Call(SCI_CALLTIPSETFORE, static_cast<WPARAM>(fore), 0, bDirect);
}

void CCT3000CO2_NET::CallTipSetForeHlt(COLORREF fore, BOOL bDirect)
{
  Call(SCI_CALLTIPSETFOREHLT, static_cast<WPARAM>(fore), 0, bDirect);
}

void CCT3000CO2_NET::CallTipUseStyle(int tabSize, BOOL bDirect)
{
  Call(SCI_CALLTIPUSESTYLE, static_cast<WPARAM>(tabSize), 0, bDirect);
}

int CCT3000CO2_NET::VisibleFromDocLine(int line, BOOL bDirect)
{
  return Call(SCI_VISIBLEFROMDOCLINE, static_cast<WPARAM>(line), 0, bDirect);
}

int CCT3000CO2_NET::DocLineFromVisible(int lineDisplay, BOOL bDirect)
{
  return Call(SCI_DOCLINEFROMVISIBLE, static_cast<WPARAM>(lineDisplay), 0, bDirect);
}

int CCT3000CO2_NET::WrapCount(int line, BOOL bDirect)
{
  return Call(SCI_WRAPCOUNT, static_cast<WPARAM>(line), 0, bDirect);
}

void CCT3000CO2_NET::SetFoldLevel(int line, int level, BOOL bDirect)
{
  Call(SCI_SETFOLDLEVEL, static_cast<WPARAM>(line), static_cast<LPARAM>(level), bDirect);
}

int CCT3000CO2_NET::GetFoldLevel(int line, BOOL bDirect)
{
  return Call(SCI_GETFOLDLEVEL, static_cast<WPARAM>(line), 0, bDirect);
}

int CCT3000CO2_NET::GetLastChild(int line, int level, BOOL bDirect)
{
  return Call(SCI_GETLASTCHILD, static_cast<WPARAM>(line), static_cast<LPARAM>(level), bDirect);
}

int CCT3000CO2_NET::GetFoldParent(int line, BOOL bDirect)
{
  return Call(SCI_GETFOLDPARENT, static_cast<WPARAM>(line), 0, bDirect);
}

void CCT3000CO2_NET::ShowLines(int lineStart, int lineEnd, BOOL bDirect)
{
  Call(SCI_SHOWLINES, static_cast<WPARAM>(lineStart), static_cast<LPARAM>(lineEnd), bDirect);
}

void CCT3000CO2_NET::HideLines(int lineStart, int lineEnd, BOOL bDirect)
{
  Call(SCI_HIDELINES, static_cast<WPARAM>(lineStart), static_cast<LPARAM>(lineEnd), bDirect);
}

BOOL CCT3000CO2_NET::GetLineVisible(int line, BOOL bDirect)
{
  return Call(SCI_GETLINEVISIBLE, static_cast<WPARAM>(line), 0, bDirect);
}

void CCT3000CO2_NET::SetFoldExpanded(int line, BOOL expanded, BOOL bDirect)
{
  Call(SCI_SETFOLDEXPANDED, static_cast<WPARAM>(line), static_cast<LPARAM>(expanded), bDirect);
}

BOOL CCT3000CO2_NET::GetFoldExpanded(int line, BOOL bDirect)
{
  return Call(SCI_GETFOLDEXPANDED, static_cast<WPARAM>(line), 0, bDirect);
}

void CCT3000CO2_NET::ToggleFold(int line, BOOL bDirect)
{
  Call(SCI_TOGGLEFOLD, static_cast<WPARAM>(line), 0, bDirect);
}

void CCT3000CO2_NET::EnsureVisible(int line, BOOL bDirect)
{
  Call(SCI_ENSUREVISIBLE, static_cast<WPARAM>(line), 0, bDirect);
}

void CCT3000CO2_NET::SetFoldFlags(int flags, BOOL bDirect)
{
  Call(SCI_SETFOLDFLAGS, static_cast<WPARAM>(flags), 0, bDirect);
}

void CCT3000CO2_NET::EnsureVisibleEnforcePolicy(int line, BOOL bDirect)
{
  Call(SCI_ENSUREVISIBLEENFORCEPOLICY, static_cast<WPARAM>(line), 0, bDirect);
}

void CCT3000CO2_NET::SetTabIndents(BOOL tabIndents, BOOL bDirect)
{
  Call(SCI_SETTABINDENTS, static_cast<WPARAM>(tabIndents), 0, bDirect);
}

BOOL CCT3000CO2_NET::GetTabIndents(BOOL bDirect)
{
  return Call(SCI_GETTABINDENTS, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetBackSpaceUnIndents(BOOL bsUnIndents, BOOL bDirect)
{
  Call(SCI_SETBACKSPACEUNINDENTS, static_cast<WPARAM>(bsUnIndents), 0, bDirect);
}

BOOL CCT3000CO2_NET::GetBackSpaceUnIndents(BOOL bDirect)
{
  return Call(SCI_GETBACKSPACEUNINDENTS, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetMouseDwellTime(int periodMilliseconds, BOOL bDirect)
{
  Call(SCI_SETMOUSEDWELLTIME, static_cast<WPARAM>(periodMilliseconds), 0, bDirect);
}

int CCT3000CO2_NET::GetMouseDwellTime(BOOL bDirect)
{
  return Call(SCI_GETMOUSEDWELLTIME, 0, 0, bDirect);
}

int CCT3000CO2_NET::WordStartPosition(long pos, BOOL onlyWordCharacters, BOOL bDirect)
{
  return Call(SCI_WORDSTARTPOSITION, static_cast<WPARAM>(pos), static_cast<LPARAM>(onlyWordCharacters), bDirect);
}

int CCT3000CO2_NET::WordEndPosition(long pos, BOOL onlyWordCharacters, BOOL bDirect)
{
  return Call(SCI_WORDENDPOSITION, static_cast<WPARAM>(pos), static_cast<LPARAM>(onlyWordCharacters), bDirect);
}

void CCT3000CO2_NET::SetWrapMode(int mode, BOOL bDirect)
{
  Call(SCI_SETWRAPMODE, static_cast<WPARAM>(mode), 0, bDirect);
}

int CCT3000CO2_NET::GetWrapMode(BOOL bDirect)
{
  return Call(SCI_GETWRAPMODE, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetWrapVisualFlags(int wrapVisualFlags, BOOL bDirect)
{
  Call(SCI_SETWRAPVISUALFLAGS, static_cast<WPARAM>(wrapVisualFlags), 0, bDirect);
}

int CCT3000CO2_NET::GetWrapVisualFlags(BOOL bDirect)
{
  return Call(SCI_GETWRAPVISUALFLAGS, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetWrapVisualFlagsLocation(int wrapVisualFlagsLocation, BOOL bDirect)
{
  Call(SCI_SETWRAPVISUALFLAGSLOCATION, static_cast<WPARAM>(wrapVisualFlagsLocation), 0, bDirect);
}

int CCT3000CO2_NET::GetWrapVisualFlagsLocation(BOOL bDirect)
{
  return Call(SCI_GETWRAPVISUALFLAGSLOCATION, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetWrapStartIndent(int indent, BOOL bDirect)
{
  Call(SCI_SETWRAPSTARTINDENT, static_cast<WPARAM>(indent), 0, bDirect);
}

int CCT3000CO2_NET::GetWrapStartIndent(BOOL bDirect)
{
  return Call(SCI_GETWRAPSTARTINDENT, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetLayoutCache(int mode, BOOL bDirect)
{
  Call(SCI_SETLAYOUTCACHE, static_cast<WPARAM>(mode), 0, bDirect);
}

int CCT3000CO2_NET::GetLayoutCache(BOOL bDirect)
{
  return Call(SCI_GETLAYOUTCACHE, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetScrollWidth(int pixelWidth, BOOL bDirect)
{
  Call(SCI_SETSCROLLWIDTH, static_cast<WPARAM>(pixelWidth), 0, bDirect);
}

int CCT3000CO2_NET::GetScrollWidth(BOOL bDirect)
{
  return Call(SCI_GETSCROLLWIDTH, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetScrollWidthTracking(BOOL tracking, BOOL bDirect)
{
  Call(SCI_SETSCROLLWIDTHTRACKING, static_cast<WPARAM>(tracking), 0, bDirect);
}

BOOL CCT3000CO2_NET::GetScrollWidthTracking(BOOL bDirect)
{
  return Call(SCI_GETSCROLLWIDTHTRACKING, 0, 0, bDirect);
}

int CCT3000CO2_NET::TextWidth(int style, const char* text, BOOL bDirect)
{
  return Call(SCI_TEXTWIDTH, static_cast<WPARAM>(style), reinterpret_cast<LPARAM>(text), bDirect);
}

void CCT3000CO2_NET::SetEndAtLastLine(BOOL endAtLastLine, BOOL bDirect)
{
  Call(SCI_SETENDATLASTLINE, static_cast<WPARAM>(endAtLastLine), 0, bDirect);
}

BOOL CCT3000CO2_NET::GetEndAtLastLine(BOOL bDirect)
{
  return Call(SCI_GETENDATLASTLINE, 0, 0, bDirect);
}

int CCT3000CO2_NET::TextHeight(int line, BOOL bDirect)
{
  return Call(SCI_TEXTHEIGHT, static_cast<WPARAM>(line), 0, bDirect);
}

void CCT3000CO2_NET::SetVScrollBar(BOOL show, BOOL bDirect)
{
  Call(SCI_SETVSCROLLBAR, static_cast<WPARAM>(show), 0, bDirect);
}

BOOL CCT3000CO2_NET::GetVScrollBar(BOOL bDirect)
{
  return Call(SCI_GETVSCROLLBAR, 0, 0, bDirect);
}

void CCT3000CO2_NET::AppendText(int length, const char* text, BOOL bDirect)
{
  Call(SCI_APPENDTEXT, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text), bDirect);
}

BOOL CCT3000CO2_NET::GetTwoPhaseDraw(BOOL bDirect)
{
  return Call(SCI_GETTWOPHASEDRAW, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetTwoPhaseDraw(BOOL twoPhase, BOOL bDirect)
{
  Call(SCI_SETTWOPHASEDRAW, static_cast<WPARAM>(twoPhase), 0, bDirect);
}

void CCT3000CO2_NET::TargetFromSelection(BOOL bDirect)
{
  Call(SCI_TARGETFROMSELECTION, 0, 0, bDirect);
}

void CCT3000CO2_NET::LinesJoin(BOOL bDirect)
{
  Call(SCI_LINESJOIN, 0, 0, bDirect);
}

void CCT3000CO2_NET::LinesSplit(int pixelWidth, BOOL bDirect)
{
  Call(SCI_LINESSPLIT, static_cast<WPARAM>(pixelWidth), 0, bDirect);
}

void CCT3000CO2_NET::SetFoldMarginColour(BOOL useSetting, COLORREF back, BOOL bDirect)
{
  Call(SCI_SETFOLDMARGINCOLOUR, static_cast<WPARAM>(useSetting), static_cast<LPARAM>(back), bDirect);
}

void CCT3000CO2_NET::SetFoldMarginHiColour(BOOL useSetting, COLORREF fore, BOOL bDirect)
{
  Call(SCI_SETFOLDMARGINHICOLOUR, static_cast<WPARAM>(useSetting), static_cast<LPARAM>(fore), bDirect);
}

void CCT3000CO2_NET::LineDown(BOOL bDirect)
{
  Call(SCI_LINEDOWN, 0, 0, bDirect);
}

void CCT3000CO2_NET::LineDownExtend(BOOL bDirect)
{
  Call(SCI_LINEDOWNEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::LineUp(BOOL bDirect)
{
  Call(SCI_LINEUP, 0, 0, bDirect);
}

void CCT3000CO2_NET::LineUpExtend(BOOL bDirect)
{
  Call(SCI_LINEUPEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::CharLeft(BOOL bDirect)
{
  Call(SCI_CHARLEFT, 0, 0, bDirect);
}

void CCT3000CO2_NET::CharLeftExtend(BOOL bDirect)
{
  Call(SCI_CHARLEFTEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::CharRight(BOOL bDirect)
{
  Call(SCI_CHARRIGHT, 0, 0, bDirect);
}

void CCT3000CO2_NET::CharRightExtend(BOOL bDirect)
{
  Call(SCI_CHARRIGHTEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::WordLeft(BOOL bDirect)
{
  Call(SCI_WORDLEFT, 0, 0, bDirect);
}

void CCT3000CO2_NET::WordLeftExtend(BOOL bDirect)
{
  Call(SCI_WORDLEFTEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::WordRight(BOOL bDirect)
{
  Call(SCI_WORDRIGHT, 0, 0, bDirect);
}

void CCT3000CO2_NET::WordRightExtend(BOOL bDirect)
{
  Call(SCI_WORDRIGHTEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::Home(BOOL bDirect)
{
  Call(SCI_HOME, 0, 0, bDirect);
}

void CCT3000CO2_NET::HomeExtend(BOOL bDirect)
{
  Call(SCI_HOMEEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::LineEnd(BOOL bDirect)
{
  Call(SCI_LINEEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::LineEndExtend(BOOL bDirect)
{
  Call(SCI_LINEENDEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::DocumentStart(BOOL bDirect)
{
  Call(SCI_DOCUMENTSTART, 0, 0, bDirect);
}

void CCT3000CO2_NET::DocumentStartExtend(BOOL bDirect)
{
  Call(SCI_DOCUMENTSTARTEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::DocumentEnd(BOOL bDirect)
{
  Call(SCI_DOCUMENTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::DocumentEndExtend(BOOL bDirect)
{
  Call(SCI_DOCUMENTENDEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::PageUp(BOOL bDirect)
{
  Call(SCI_PAGEUP, 0, 0, bDirect);
}

void CCT3000CO2_NET::PageUpExtend(BOOL bDirect)
{
  Call(SCI_PAGEUPEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::PageDown(BOOL bDirect)
{
  Call(SCI_PAGEDOWN, 0, 0, bDirect);
}

void CCT3000CO2_NET::PageDownExtend(BOOL bDirect)
{
  Call(SCI_PAGEDOWNEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::EditToggleOvertype(BOOL bDirect)
{
  Call(SCI_EDITTOGGLEOVERTYPE, 0, 0, bDirect);
}

void CCT3000CO2_NET::Cancel(BOOL bDirect)
{
  Call(SCI_CANCEL, 0, 0, bDirect);
}

void CCT3000CO2_NET::DeleteBack(BOOL bDirect)
{
  Call(SCI_DELETEBACK, 0, 0, bDirect);
}

void CCT3000CO2_NET::Tab(BOOL bDirect)
{
  Call(SCI_TAB, 0, 0, bDirect);
}

void CCT3000CO2_NET::BackTab(BOOL bDirect)
{
  Call(SCI_BACKTAB, 0, 0, bDirect);
}

void CCT3000CO2_NET::NewLine(BOOL bDirect)
{
  Call(SCI_NEWLINE, 0, 0, bDirect);
}

void CCT3000CO2_NET::FormFeed(BOOL bDirect)
{
  Call(SCI_FORMFEED, 0, 0, bDirect);
}

void CCT3000CO2_NET::VCHome(BOOL bDirect)
{
  Call(SCI_VCHOME, 0, 0, bDirect);
}

void CCT3000CO2_NET::VCHomeExtend(BOOL bDirect)
{
  Call(SCI_VCHOMEEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::ZoomIn(BOOL bDirect)
{
  Call(SCI_ZOOMIN, 0, 0, bDirect);
}

void CCT3000CO2_NET::ZoomOut(BOOL bDirect)
{
  Call(SCI_ZOOMOUT, 0, 0, bDirect);
}

void CCT3000CO2_NET::DelWordLeft(BOOL bDirect)
{
  Call(SCI_DELWORDLEFT, 0, 0, bDirect);
}

void CCT3000CO2_NET::DelWordRight(BOOL bDirect)
{
  Call(SCI_DELWORDRIGHT, 0, 0, bDirect);
}

void CCT3000CO2_NET::DelWordRightEnd(BOOL bDirect)
{
  Call(SCI_DELWORDRIGHTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::LineCut(BOOL bDirect)
{
  Call(SCI_LINECUT, 0, 0, bDirect);
}

void CCT3000CO2_NET::LineDelete(BOOL bDirect)
{
  Call(SCI_LINEDELETE, 0, 0, bDirect);
}

void CCT3000CO2_NET::LineTranspose(BOOL bDirect)
{
  Call(SCI_LINETRANSPOSE, 0, 0, bDirect);
}

void CCT3000CO2_NET::LineDuplicate(BOOL bDirect)
{
  Call(SCI_LINEDUPLICATE, 0, 0, bDirect);
}

void CCT3000CO2_NET::LowerCase(BOOL bDirect)
{
  Call(SCI_LOWERCASE, 0, 0, bDirect);
}

void CCT3000CO2_NET::UpperCase(BOOL bDirect)
{
  Call(SCI_UPPERCASE, 0, 0, bDirect);
}

void CCT3000CO2_NET::LineScrollDown(BOOL bDirect)
{
  Call(SCI_LINESCROLLDOWN, 0, 0, bDirect);
}

void CCT3000CO2_NET::LineScrollUp(BOOL bDirect)
{
  Call(SCI_LINESCROLLUP, 0, 0, bDirect);
}

void CCT3000CO2_NET::DeleteBackNotLine(BOOL bDirect)
{
  Call(SCI_DELETEBACKNOTLINE, 0, 0, bDirect);
}

void CCT3000CO2_NET::HomeDisplay(BOOL bDirect)
{
  Call(SCI_HOMEDISPLAY, 0, 0, bDirect);
}

void CCT3000CO2_NET::HomeDisplayExtend(BOOL bDirect)
{
  Call(SCI_HOMEDISPLAYEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::LineEndDisplay(BOOL bDirect)
{
  Call(SCI_LINEENDDISPLAY, 0, 0, bDirect);
}

void CCT3000CO2_NET::LineEndDisplayExtend(BOOL bDirect)
{
  Call(SCI_LINEENDDISPLAYEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::HomeWrap(BOOL bDirect)
{
  Call(SCI_HOMEWRAP, 0, 0, bDirect);
}

void CCT3000CO2_NET::HomeWrapExtend(BOOL bDirect)
{
  Call(SCI_HOMEWRAPEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::LineEndWrap(BOOL bDirect)
{
  Call(SCI_LINEENDWRAP, 0, 0, bDirect);
}

void CCT3000CO2_NET::LineEndWrapExtend(BOOL bDirect)
{
  Call(SCI_LINEENDWRAPEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::VCHomeWrap(BOOL bDirect)
{
  Call(SCI_VCHOMEWRAP, 0, 0, bDirect);
}

void CCT3000CO2_NET::VCHomeWrapExtend(BOOL bDirect)
{
  Call(SCI_VCHOMEWRAPEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::LineCopy(BOOL bDirect)
{
  Call(SCI_LINECOPY, 0, 0, bDirect);
}

void CCT3000CO2_NET::MoveCaretInsideView(BOOL bDirect)
{
  Call(SCI_MOVECARETINSIDEVIEW, 0, 0, bDirect);
}

int CCT3000CO2_NET::LineLength(int line, BOOL bDirect)
{
  return Call(SCI_LINELENGTH, static_cast<WPARAM>(line), 0, bDirect);
}

void CCT3000CO2_NET::BraceHighlight(long pos1, long pos2, BOOL bDirect)
{
  Call(SCI_BRACEHIGHLIGHT, static_cast<WPARAM>(pos1), static_cast<LPARAM>(pos2), bDirect);
}

void CCT3000CO2_NET::BraceBadLight(long pos, BOOL bDirect)
{
  Call(SCI_BRACEBADLIGHT, static_cast<WPARAM>(pos), 0, bDirect);
}

long CCT3000CO2_NET::BraceMatch(long pos, BOOL bDirect)
{
  return Call(SCI_BRACEMATCH, static_cast<WPARAM>(pos), 0, bDirect);
}

BOOL CCT3000CO2_NET::GetViewEOL(BOOL bDirect)
{
  return Call(SCI_GETVIEWEOL, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetViewEOL(BOOL visible, BOOL bDirect)
{
  Call(SCI_SETVIEWEOL, static_cast<WPARAM>(visible), 0, bDirect);
}

int CCT3000CO2_NET::GetDocPointer(BOOL bDirect)
{
  return Call(SCI_GETDOCPOINTER, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetDocPointer(int pointer, BOOL bDirect)
{
  Call(SCI_SETDOCPOINTER, 0, static_cast<LPARAM>(pointer), bDirect);
}

void CCT3000CO2_NET::SetModEventMask(int mask, BOOL bDirect)
{
  Call(SCI_SETMODEVENTMASK, static_cast<WPARAM>(mask), 0, bDirect);
}

int CCT3000CO2_NET::GetEdgeColumn(BOOL bDirect)
{
  return Call(SCI_GETEDGECOLUMN, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetEdgeColumn(int column, BOOL bDirect)
{
  Call(SCI_SETEDGECOLUMN, static_cast<WPARAM>(column), 0, bDirect);
}

int CCT3000CO2_NET::GetEdgeMode(BOOL bDirect)
{
  return Call(SCI_GETEDGEMODE, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetEdgeMode(int mode, BOOL bDirect)
{
  Call(SCI_SETEDGEMODE, static_cast<WPARAM>(mode), 0, bDirect);
}

COLORREF CCT3000CO2_NET::GetEdgeColour(BOOL bDirect)
{
  return Call(SCI_GETEDGECOLOUR, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetEdgeColour(COLORREF edgeColour, BOOL bDirect)
{
  Call(SCI_SETEDGECOLOUR, static_cast<WPARAM>(edgeColour), 0, bDirect);
}

void CCT3000CO2_NET::SearchAnchor(BOOL bDirect)
{
  Call(SCI_SEARCHANCHOR, 0, 0, bDirect);
}

int CCT3000CO2_NET::SearchNext(int flags, const char* text, BOOL bDirect)
{
  return Call(SCI_SEARCHNEXT, static_cast<WPARAM>(flags), reinterpret_cast<LPARAM>(text), bDirect);
}

int CCT3000CO2_NET::SearchPrev(int flags, const char* text, BOOL bDirect)
{
  return Call(SCI_SEARCHPREV, static_cast<WPARAM>(flags), reinterpret_cast<LPARAM>(text), bDirect);
}

int CCT3000CO2_NET::LinesOnScreen(BOOL bDirect)
{
  return Call(SCI_LINESONSCREEN, 0, 0, bDirect);
}

void CCT3000CO2_NET::UsePopUp(BOOL allowPopUp, BOOL bDirect)
{
  Call(SCI_USEPOPUP, static_cast<WPARAM>(allowPopUp), 0, bDirect);
}

BOOL CCT3000CO2_NET::SelectionIsRectangle(BOOL bDirect)
{
  return Call(SCI_SELECTIONISRECTANGLE, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetZoom(int zoom, BOOL bDirect)
{
  Call(SCI_SETZOOM, static_cast<WPARAM>(zoom), 0, bDirect);
}

int CCT3000CO2_NET::GetZoom(BOOL bDirect)
{
  return Call(SCI_GETZOOM, 0, 0, bDirect);
}

int CCT3000CO2_NET::CreateDocument(BOOL bDirect)
{
  return Call(SCI_CREATEDOCUMENT, 0, 0, bDirect);
}

void CCT3000CO2_NET::AddRefDocument(int doc, BOOL bDirect)
{
  Call(SCI_ADDREFDOCUMENT, 0, static_cast<LPARAM>(doc), bDirect);
}

void CCT3000CO2_NET::ReleaseDocument(int doc, BOOL bDirect)
{
  Call(SCI_RELEASEDOCUMENT, 0, static_cast<LPARAM>(doc), bDirect);
}

int CCT3000CO2_NET::GetModEventMask(BOOL bDirect)
{
  return Call(SCI_GETMODEVENTMASK, 0, 0, bDirect);
}

void CCT3000CO2_NET::SCISetFocus(BOOL focus, BOOL bDirect)
{
  Call(SCI_SETFOCUS, static_cast<WPARAM>(focus), 0, bDirect);
}

BOOL CCT3000CO2_NET::GetFocus(BOOL bDirect)
{
  return Call(SCI_GETFOCUS, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetStatus(int statusCode, BOOL bDirect)
{
  Call(SCI_SETSTATUS, static_cast<WPARAM>(statusCode), 0, bDirect);
}

int CCT3000CO2_NET::GetStatus(BOOL bDirect)
{
  return Call(SCI_GETSTATUS, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetMouseDownCaptures(BOOL captures, BOOL bDirect)
{
  Call(SCI_SETMOUSEDOWNCAPTURES, static_cast<WPARAM>(captures), 0, bDirect);
}

BOOL CCT3000CO2_NET::GetMouseDownCaptures(BOOL bDirect)
{
  return Call(SCI_GETMOUSEDOWNCAPTURES, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetCursor(int cursorType, BOOL bDirect)
{
  Call(SCI_SETCURSOR, static_cast<WPARAM>(cursorType), 0, bDirect);
}

int CCT3000CO2_NET::GetCursor(BOOL bDirect)
{
  return Call(SCI_GETCURSOR, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetControlCharSymbol(int symbol, BOOL bDirect)
{
  Call(SCI_SETCONTROLCHARSYMBOL, static_cast<WPARAM>(symbol), 0, bDirect);
}

int CCT3000CO2_NET::GetControlCharSymbol(BOOL bDirect)
{
  return Call(SCI_GETCONTROLCHARSYMBOL, 0, 0, bDirect);
}

void CCT3000CO2_NET::WordPartLeft(BOOL bDirect)
{
  Call(SCI_WORDPARTLEFT, 0, 0, bDirect);
}

void CCT3000CO2_NET::WordPartLeftExtend(BOOL bDirect)
{
  Call(SCI_WORDPARTLEFTEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::WordPartRight(BOOL bDirect)
{
  Call(SCI_WORDPARTRIGHT, 0, 0, bDirect);
}

void CCT3000CO2_NET::WordPartRightExtend(BOOL bDirect)
{
  Call(SCI_WORDPARTRIGHTEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetVisiblePolicy(int visiblePolicy, int visibleSlop, BOOL bDirect)
{
  Call(SCI_SETVISIBLEPOLICY, static_cast<WPARAM>(visiblePolicy), static_cast<LPARAM>(visibleSlop), bDirect);
}

void CCT3000CO2_NET::DelLineLeft(BOOL bDirect)
{
  Call(SCI_DELLINELEFT, 0, 0, bDirect);
}

void CCT3000CO2_NET::DelLineRight(BOOL bDirect)
{
  Call(SCI_DELLINERIGHT, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetXOffset(int newOffset, BOOL bDirect)
{
  Call(SCI_SETXOFFSET, static_cast<WPARAM>(newOffset), 0, bDirect);
}

int CCT3000CO2_NET::GetXOffset(BOOL bDirect)
{
  return Call(SCI_GETXOFFSET, 0, 0, bDirect);
}

void CCT3000CO2_NET::ChooseCaretX(BOOL bDirect)
{
  Call(SCI_CHOOSECARETX, 0, 0, bDirect);
}

void CCT3000CO2_NET::GrabFocus(BOOL bDirect)
{
  Call(SCI_GRABFOCUS, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetXCaretPolicy(int caretPolicy, int caretSlop, BOOL bDirect)
{
  Call(SCI_SETXCARETPOLICY, static_cast<WPARAM>(caretPolicy), static_cast<LPARAM>(caretSlop), bDirect);
}

void CCT3000CO2_NET::SetYCaretPolicy(int caretPolicy, int caretSlop, BOOL bDirect)
{
  Call(SCI_SETYCARETPOLICY, static_cast<WPARAM>(caretPolicy), static_cast<LPARAM>(caretSlop), bDirect);
}

void CCT3000CO2_NET::SetPrintWrapMode(int mode, BOOL bDirect)
{
  Call(SCI_SETPRINTWRAPMODE, static_cast<WPARAM>(mode), 0, bDirect);
}

int CCT3000CO2_NET::GetPrintWrapMode(BOOL bDirect)
{
  return Call(SCI_GETPRINTWRAPMODE, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetHotspotActiveFore(BOOL useSetting, COLORREF fore, BOOL bDirect)
{
  Call(SCI_SETHOTSPOTACTIVEFORE, static_cast<WPARAM>(useSetting), static_cast<LPARAM>(fore), bDirect);
}

COLORREF CCT3000CO2_NET::GetHotspotActiveFore(BOOL bDirect)
{
  return Call(SCI_GETHOTSPOTACTIVEFORE, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetHotspotActiveBack(BOOL useSetting, COLORREF back, BOOL bDirect)
{
  Call(SCI_SETHOTSPOTACTIVEBACK, static_cast<WPARAM>(useSetting), static_cast<LPARAM>(back), bDirect);
}

COLORREF CCT3000CO2_NET::GetHotspotActiveBack(BOOL bDirect)
{
  return Call(SCI_GETHOTSPOTACTIVEBACK, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetHotspotActiveUnderline(BOOL underline, BOOL bDirect)
{
  Call(SCI_SETHOTSPOTACTIVEUNDERLINE, static_cast<WPARAM>(underline), 0, bDirect);
}

BOOL CCT3000CO2_NET::GetHotspotActiveUnderline(BOOL bDirect)
{
  return Call(SCI_GETHOTSPOTACTIVEUNDERLINE, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetHotspotSingleLine(BOOL singleLine, BOOL bDirect)
{
  Call(SCI_SETHOTSPOTSINGLELINE, static_cast<WPARAM>(singleLine), 0, bDirect);
}

BOOL CCT3000CO2_NET::GetHotspotSingleLine(BOOL bDirect)
{
  return Call(SCI_GETHOTSPOTSINGLELINE, 0, 0, bDirect);
}

void CCT3000CO2_NET::ParaDown(BOOL bDirect)
{
  Call(SCI_PARADOWN, 0, 0, bDirect);
}

void CCT3000CO2_NET::ParaDownExtend(BOOL bDirect)
{
  Call(SCI_PARADOWNEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::ParaUp(BOOL bDirect)
{
  Call(SCI_PARAUP, 0, 0, bDirect);
}

void CCT3000CO2_NET::ParaUpExtend(BOOL bDirect)
{
  Call(SCI_PARAUPEXTEND, 0, 0, bDirect);
}

long CCT3000CO2_NET::PositionBefore(long pos, BOOL bDirect)
{
  return Call(SCI_POSITIONBEFORE, static_cast<WPARAM>(pos), 0, bDirect);
}

long CCT3000CO2_NET::PositionAfter(long pos, BOOL bDirect)
{
  return Call(SCI_POSITIONAFTER, static_cast<WPARAM>(pos), 0, bDirect);
}

void CCT3000CO2_NET::CopyRange(long start, long end, BOOL bDirect)
{
  Call(SCI_COPYRANGE, static_cast<WPARAM>(start), static_cast<LPARAM>(end), bDirect);
}

void CCT3000CO2_NET::CopyText(int length, const char* text, BOOL bDirect)
{
  Call(SCI_COPYTEXT, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text), bDirect);
}

void CCT3000CO2_NET::SetSelectionMode(int mode, BOOL bDirect)
{
  Call(SCI_SETSELECTIONMODE, static_cast<WPARAM>(mode), 0, bDirect);
}

int CCT3000CO2_NET::GetSelectionMode(BOOL bDirect)
{
  return Call(SCI_GETSELECTIONMODE, 0, 0, bDirect);
}

long CCT3000CO2_NET::GetLineSelStartPosition(int line, BOOL bDirect)
{
  return Call(SCI_GETLINESELSTARTPOSITION, static_cast<WPARAM>(line), 0, bDirect);
}

long CCT3000CO2_NET::GetLineSelEndPosition(int line, BOOL bDirect)
{
  return Call(SCI_GETLINESELENDPOSITION, static_cast<WPARAM>(line), 0, bDirect);
}

void CCT3000CO2_NET::LineDownRectExtend(BOOL bDirect)
{
  Call(SCI_LINEDOWNRECTEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::LineUpRectExtend(BOOL bDirect)
{
  Call(SCI_LINEUPRECTEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::CharLeftRectExtend(BOOL bDirect)
{
  Call(SCI_CHARLEFTRECTEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::CharRightRectExtend(BOOL bDirect)
{
  Call(SCI_CHARRIGHTRECTEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::HomeRectExtend(BOOL bDirect)
{
  Call(SCI_HOMERECTEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::VCHomeRectExtend(BOOL bDirect)
{
  Call(SCI_VCHOMERECTEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::LineEndRectExtend(BOOL bDirect)
{
  Call(SCI_LINEENDRECTEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::PageUpRectExtend(BOOL bDirect)
{
  Call(SCI_PAGEUPRECTEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::PageDownRectExtend(BOOL bDirect)
{
  Call(SCI_PAGEDOWNRECTEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::StutteredPageUp(BOOL bDirect)
{
  Call(SCI_STUTTEREDPAGEUP, 0, 0, bDirect);
}

void CCT3000CO2_NET::StutteredPageUpExtend(BOOL bDirect)
{
  Call(SCI_STUTTEREDPAGEUPEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::StutteredPageDown(BOOL bDirect)
{
  Call(SCI_STUTTEREDPAGEDOWN, 0, 0, bDirect);
}

void CCT3000CO2_NET::StutteredPageDownExtend(BOOL bDirect)
{
  Call(SCI_STUTTEREDPAGEDOWNEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::WordLeftEnd(BOOL bDirect)
{
  Call(SCI_WORDLEFTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::WordLeftEndExtend(BOOL bDirect)
{
  Call(SCI_WORDLEFTENDEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::WordRightEnd(BOOL bDirect)
{
  Call(SCI_WORDRIGHTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::WordRightEndExtend(BOOL bDirect)
{
  Call(SCI_WORDRIGHTENDEXTEND, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetWhitespaceChars(const char* characters, BOOL bDirect)
{
  Call(SCI_SETWHITESPACECHARS, 0, reinterpret_cast<LPARAM>(characters), bDirect);
}

void CCT3000CO2_NET::SetCharsDefault(BOOL bDirect)
{
  Call(SCI_SETCHARSDEFAULT, 0, 0, bDirect);
}

int CCT3000CO2_NET::AutoCGetCurrent(BOOL bDirect)
{
  return Call(SCI_AUTOCGETCURRENT, 0, 0, bDirect);
}

void CCT3000CO2_NET::Allocate(int bytes, BOOL bDirect)
{
  Call(SCI_ALLOCATE, static_cast<WPARAM>(bytes), 0, bDirect);
}

int CCT3000CO2_NET::TargetAsUTF8(char* s, BOOL bDirect)
{
  return Call(SCI_TARGETASUTF8, 0, reinterpret_cast<LPARAM>(s), bDirect);
}

void CCT3000CO2_NET::SetLengthForEncode(int bytes, BOOL bDirect)
{
  Call(SCI_SETLENGTHFORENCODE, static_cast<WPARAM>(bytes), 0, bDirect);
}

int CCT3000CO2_NET::EncodedFromUTF8(const char* utf8, char* encoded, BOOL bDirect)
{
  return Call(SCI_ENCODEDFROMUTF8, reinterpret_cast<WPARAM>(utf8), reinterpret_cast<LPARAM>(encoded), bDirect);
}

int CCT3000CO2_NET::FindColumn(int line, int column, BOOL bDirect)
{
  return Call(SCI_FINDCOLUMN, static_cast<WPARAM>(line), static_cast<LPARAM>(column), bDirect);
}

BOOL CCT3000CO2_NET::GetCaretSticky(BOOL bDirect)
{
  return Call(SCI_GETCARETSTICKY, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetCaretSticky(BOOL useCaretStickyBehaviour, BOOL bDirect)
{
  Call(SCI_SETCARETSTICKY, static_cast<WPARAM>(useCaretStickyBehaviour), 0, bDirect);
}

void CCT3000CO2_NET::ToggleCaretSticky(BOOL bDirect)
{
  Call(SCI_TOGGLECARETSTICKY, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetPasteConvertEndings(BOOL convert, BOOL bDirect)
{
  Call(SCI_SETPASTECONVERTENDINGS, static_cast<WPARAM>(convert), 0, bDirect);
}

BOOL CCT3000CO2_NET::GetPasteConvertEndings(BOOL bDirect)
{
  return Call(SCI_GETPASTECONVERTENDINGS, 0, 0, bDirect);
}

void CCT3000CO2_NET::SelectionDuplicate(BOOL bDirect)
{
  Call(SCI_SELECTIONDUPLICATE, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetCaretLineBackAlpha(int alpha, BOOL bDirect)
{
  Call(SCI_SETCARETLINEBACKALPHA, static_cast<WPARAM>(alpha), 0, bDirect);
}

int CCT3000CO2_NET::GetCaretLineBackAlpha(BOOL bDirect)
{
  return Call(SCI_GETCARETLINEBACKALPHA, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetCaretStyle(int caretStyle, BOOL bDirect)
{
  Call(SCI_SETCARETSTYLE, static_cast<WPARAM>(caretStyle), 0, bDirect);
}

int CCT3000CO2_NET::GetCaretStyle(BOOL bDirect)
{
  return Call(SCI_GETCARETSTYLE, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetIndicatorCurrent(int indicator, BOOL bDirect)
{
  Call(SCI_SETINDICATORCURRENT, static_cast<WPARAM>(indicator), 0, bDirect);
}

int CCT3000CO2_NET::GetIndicatorCurrent(BOOL bDirect)
{
  return Call(SCI_GETINDICATORCURRENT, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetIndicatorValue(int value, BOOL bDirect)
{
  Call(SCI_SETINDICATORVALUE, static_cast<WPARAM>(value), 0, bDirect);
}

int CCT3000CO2_NET::GetIndicatorValue(BOOL bDirect)
{
  return Call(SCI_GETINDICATORVALUE, 0, 0, bDirect);
}

void CCT3000CO2_NET::IndicatorFillRange(int position, int fillLength, BOOL bDirect)
{
  Call(SCI_INDICATORFILLRANGE, static_cast<WPARAM>(position), static_cast<LPARAM>(fillLength), bDirect);
}

void CCT3000CO2_NET::IndicatorClearRange(int position, int clearLength, BOOL bDirect)
{
  Call(SCI_INDICATORCLEARRANGE, static_cast<WPARAM>(position), static_cast<LPARAM>(clearLength), bDirect);
}

int CCT3000CO2_NET::IndicatorAllOnFor(int position, BOOL bDirect)
{
  return Call(SCI_INDICATORALLONFOR, static_cast<WPARAM>(position), 0, bDirect);
}

int CCT3000CO2_NET::IndicatorValueAt(int indicator, int position, BOOL bDirect)
{
  return Call(SCI_INDICATORVALUEAT, static_cast<WPARAM>(indicator), static_cast<LPARAM>(position), bDirect);
}

int CCT3000CO2_NET::IndicatorStart(int indicator, int position, BOOL bDirect)
{
  return Call(SCI_INDICATORSTART, static_cast<WPARAM>(indicator), static_cast<LPARAM>(position), bDirect);
}

int CCT3000CO2_NET::IndicatorEnd(int indicator, int position, BOOL bDirect)
{
  return Call(SCI_INDICATOREND, static_cast<WPARAM>(indicator), static_cast<LPARAM>(position), bDirect);
}

void CCT3000CO2_NET::SetPositionCache(int size, BOOL bDirect)
{
  Call(SCI_SETPOSITIONCACHE, static_cast<WPARAM>(size), 0, bDirect);
}

int CCT3000CO2_NET::GetPositionCache(BOOL bDirect)
{
  return Call(SCI_GETPOSITIONCACHE, 0, 0, bDirect);
}

void CCT3000CO2_NET::CopyAllowLine(BOOL bDirect)
{
  Call(SCI_COPYALLOWLINE, 0, 0, bDirect);
}

void CCT3000CO2_NET::StartRecord(BOOL bDirect)
{
  Call(SCI_STARTRECORD, 0, 0, bDirect);
}

void CCT3000CO2_NET::StopRecord(BOOL bDirect)
{
  Call(SCI_STOPRECORD, 0, 0, bDirect);
}

void CCT3000CO2_NET::SetLexer(int lexer, BOOL bDirect)
{
  Call(SCI_SETLEXER, static_cast<WPARAM>(lexer), 0, bDirect);
}

int CCT3000CO2_NET::GetLexer(BOOL bDirect)
{
  return Call(SCI_GETLEXER, 0, 0, bDirect);
}

void CCT3000CO2_NET::Colourise(long start, long end, BOOL bDirect)
{
  Call(SCI_COLOURISE, static_cast<WPARAM>(start), static_cast<LPARAM>(end), bDirect);
}

void CCT3000CO2_NET::SetProperty(const char* key, const char* value, BOOL bDirect)
{
  Call(SCI_SETPROPERTY, reinterpret_cast<WPARAM>(key), reinterpret_cast<LPARAM>(value), bDirect);
}

void CCT3000CO2_NET::SetKeyWords(int keywordSet, const char* keyWords, BOOL bDirect)
{
  Call(SCI_SETKEYWORDS, static_cast<WPARAM>(keywordSet), reinterpret_cast<LPARAM>(keyWords), bDirect);
}

void CCT3000CO2_NET::SetLexerLanguage(const char* language, BOOL bDirect)
{
  Call(SCI_SETLEXERLANGUAGE, 0, reinterpret_cast<LPARAM>(language), bDirect);
}

void CCT3000CO2_NET::LoadLexerLibrary(const char* path, BOOL bDirect)
{
  Call(SCI_LOADLEXERLIBRARY, 0, reinterpret_cast<LPARAM>(path), bDirect);
}

int CCT3000CO2_NET::GetProperty(const char* key, char* buf, BOOL bDirect)
{
  return Call(SCI_GETPROPERTY, reinterpret_cast<WPARAM>(key), reinterpret_cast<LPARAM>(buf), bDirect);
}

int CCT3000CO2_NET::GetPropertyExpanded(const char* key, char* buf, BOOL bDirect)
{
  return Call(SCI_GETPROPERTYEXPANDED, reinterpret_cast<WPARAM>(key), reinterpret_cast<LPARAM>(buf), bDirect);
}

int CCT3000CO2_NET::GetPropertyInt(const char* key, BOOL bDirect)
{
  return Call(SCI_GETPROPERTYINT, reinterpret_cast<WPARAM>(key), 0, bDirect);
}

int CCT3000CO2_NET::GetStyleBitsNeeded(BOOL bDirect)
{
  return Call(SCI_GETSTYLEBITSNEEDED, 0, 0, bDirect);
}
