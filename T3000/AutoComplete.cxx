
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <cstdio>

#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>

#include "Platform.h"

#include "Scintilla.h"
#include "CharacterSet.h"
#include "Position.h"
#include "AutoComplete.h"

using namespace Scintilla;

AutoComplete::AutoComplete() :
	active(false),
	separator(' '),
	typesep('?'),
	ignoreCase(false),
	chooseSingle(false),
	posStart(0),
	startLen(0),
	cancelAtStartPos(true),
	autoHide(true),
	dropRestOfWord(false),
	ignoreCaseBehaviour(SC_CASEINSENSITIVEBEHAVIOUR_RESPECTCASE),
	widthLBDefault(100),
	heightLBDefault(100),
	autoSort(SC_ORDER_PRESORTED) {
	lb.reset(ListBox::Allocate());
}

AutoComplete::~AutoComplete() {
	if (lb) {
		lb->Destroy();
	}
}

bool AutoComplete::Active() const {
	return active;
}

void AutoComplete::Start(Window &parent, int ctrlID,
	Sci::Position position, Point location, int startLen_,
	int lineHeight, bool unicodeMode, int technology) {
	if (active) {
		Cancel();
	}
	lb->Create(parent, ctrlID, location, lineHeight, unicodeMode, technology);
	lb->Clear();
	active = true;
	startLen = startLen_;
	posStart = position;
}

void AutoComplete::SetStopChars(const char *stopChars_) {
	stopChars = stopChars_;
}

bool AutoComplete::IsStopChar(char ch) {
	return ch && (stopChars.find(ch) != std::string::npos);
}

void AutoComplete::SetFillUpChars(const char *fillUpChars_) {
	fillUpChars = fillUpChars_;
}

bool AutoComplete::IsFillUpChar(char ch) {
	return ch && (fillUpChars.find(ch) != std::string::npos);
}

void AutoComplete::SetSeparator(char separator_) {
	separator = separator_;
}

char AutoComplete::GetSeparator() const {
	return separator;
}

void AutoComplete::SetTypesep(char separator_) {
	typesep = separator_;
}

char AutoComplete::GetTypesep() const {
	return typesep;
}

struct Sorter {
	AutoComplete *ac;
	const char *list;
	std::vector<int> indices;

	Sorter(AutoComplete *ac_, const char *list_) : ac(ac_), list(list_) {
		int i = 0;
		while (list[i]) {
			indices.push_back(i); // word start
			while (list[i] != ac->GetTypesep() && list[i] != ac->GetSeparator() && list[i])
				++i;
			indices.push_back(i); // word end
			if (list[i] == ac->GetTypesep()) {
				while (list[i] != ac->GetSeparator() && list[i])
					++i;
			}
			if (list[i] == ac->GetSeparator()) {
				++i;
				// preserve trailing separator as blank entry
				if (!list[i]) {
					indices.push_back(i);
					indices.push_back(i);
				}
			}
		}
		indices.push_back(i); // index of last position
	}

	bool operator()(int a, int b) {
		const int lenA = indices[a * 2 + 1] - indices[a * 2];
		const int lenB = indices[b * 2 + 1] - indices[b * 2];
		const int len  = std::min(lenA, lenB);
		int cmp;
		if (ac->ignoreCase)
			cmp = CompareNCaseInsensitive(list + indices[a * 2], list + indices[b * 2], len);
		else
			cmp = strncmp(list + indices[a * 2], list + indices[b * 2], len);
		if (cmp == 0)
			cmp = lenA - lenB;
		return cmp < 0;
	}
};

void AutoComplete::SetList(const char *list) {
	if (autoSort == SC_ORDER_PRESORTED) {
		lb->SetList(list, separator, typesep);
		sortMatrix.clear();
		for (int i = 0; i < lb->Length(); ++i)
			sortMatrix.push_back(i);
		return;
	}

	Sorter IndexSort(this, list);
	sortMatrix.clear();
	for (int i = 0; i < static_cast<int>(IndexSort.indices.size()) / 2; ++i)
		sortMatrix.push_back(i);
	std::sort(sortMatrix.begin(), sortMatrix.end(), IndexSort);
	if (autoSort == SC_ORDER_CUSTOM || sortMatrix.size() < 2) {
		lb->SetList(list, separator, typesep);
		PLATFORM_ASSERT(lb->Length() == static_cast<int>(sortMatrix.size()));
		return;
	}

	std::string sortedList;
	char item[maxItemLen];
	for (size_t i = 0; i < sortMatrix.size(); ++i) {
		int wordLen = IndexSort.indices[sortMatrix[i] * 2 + 2] - IndexSort.indices[sortMatrix[i] * 2];
		if (wordLen > maxItemLen-2)
			wordLen = maxItemLen - 2;
		memcpy(item, list + IndexSort.indices[sortMatrix[i] * 2], wordLen);
		if ((i+1) == sortMatrix.size()) {
			// Last item so remove separator if present
			if ((wordLen > 0) && (item[wordLen-1] == separator))
				wordLen--;
		} else {
			// Item before last needs a separator
			if ((wordLen == 0) || (item[wordLen-1] != separator)) {
				item[wordLen] = separator;
				wordLen++;
			}
		}
		item[wordLen] = '\0';
		sortedList += item;
	}
	for (int i = 0; i < static_cast<int>(sortMatrix.size()); ++i)
		sortMatrix[i] = i;
	lb->SetList(sortedList.c_str(), separator, typesep);
}

int AutoComplete::GetSelection() const {
	return lb->GetSelection();
}

std::string AutoComplete::GetValue(int item) const {
	char value[maxItemLen];
	lb->GetValue(item, value, sizeof(value));
	return std::string(value);
}

void AutoComplete::Show(bool show) {
	lb->Show(show);
	if (show)
		lb->Select(0);
}

void AutoComplete::Cancel() {
	if (lb->Created()) {
		lb->Clear();
		lb->Destroy();
		active = false;
	}
}


void AutoComplete::Move(int delta) {
	const int count = lb->Length();
	int current = lb->GetSelection();
	current += delta;
	if (current >= count)
		current = count - 1;
	if (current < 0)
		current = 0;
	lb->Select(current);
}

void AutoComplete::Select(const char *word) {
	size_t lenWord = strlen(word);
	int location = -1;
	int start = 0; // lower bound of the api array block to search
	int end = lb->Length() - 1; // upper bound of the api array block to search
	while ((start <= end) && (location == -1)) { // Binary searching loop
		int pivot = (start + end) / 2;
		char item[maxItemLen];
		lb->GetValue(sortMatrix[pivot], item, maxItemLen);
		int cond;
		if (ignoreCase)
			cond = CompareNCaseInsensitive(word, item, lenWord);
		else
			cond = strncmp(word, item, lenWord);
		if (!cond) {
			// Find first match
			while (pivot > start) {
				lb->GetValue(sortMatrix[pivot-1], item, maxItemLen);
				if (ignoreCase)
					cond = CompareNCaseInsensitive(word, item, lenWord);
				else
					cond = strncmp(word, item, lenWord);
				if (0 != cond)
					break;
				--pivot;
			}
			location = pivot;
			if (ignoreCase
				&& ignoreCaseBehaviour == SC_CASEINSENSITIVEBEHAVIOUR_RESPECTCASE) {
				// Check for exact-case match
				for (; pivot <= end; pivot++) {
					lb->GetValue(sortMatrix[pivot], item, maxItemLen);
					if (!strncmp(word, item, lenWord)) {
						location = pivot;
						break;
					}
					if (CompareNCaseInsensitive(word, item, lenWord))
						break;
				}
			}
		} else if (cond < 0) {
			end = pivot - 1;
		} else if (cond > 0) {
			start = pivot + 1;
		}
	}
	if (location == -1) {
		if (autoHide)
			Cancel();
		else
			lb->Select(-1);
	} else {
		if (autoSort == SC_ORDER_CUSTOM) {
			// Check for a logically earlier match
			char item[maxItemLen];
			for (int i = location + 1; i <= end; ++i) {
				lb->GetValue(sortMatrix[i], item, maxItemLen);
				if (CompareNCaseInsensitive(word, item, lenWord))
					break;
				if (sortMatrix[i] < sortMatrix[location] && !strncmp(word, item, lenWord))
					location = i;
			}
		}
		lb->Select(sortMatrix[location]);
	}
}

CallTip::CallTip() {
	wCallTip = 0;
	inCallTipMode = false;
	posStartCallTip = 0;
	val = 0;
	rectUp = PRectangle(0,0,0,0);
	rectDown = PRectangle(0,0,0,0);
	lineHeight = 1;
	startHighlight = 0;
	endHighlight = 0;
	tabSize = 0;
	useStyleCallTip = false;    // for backwards compatibility

#ifdef __APPLE__
	// proper apple colours for the default
	colourBG.desired = ColourDesired(0xff, 0xff, 0xc6);
	colourUnSel.desired = ColourDesired(0, 0, 0);
#else
	colourBG.desired = ColourDesired(0xff, 0xff, 0xff);
	colourUnSel.desired = ColourDesired(0x80, 0x80, 0x80);
#endif
	colourSel.desired = ColourDesired(0, 0, 0x80);
	colourShade.desired = ColourDesired(0, 0, 0);
	colourLight.desired = ColourDesired(0xc0, 0xc0, 0xc0);
}


CallTip::~CallTip() {
	font.Release();
	wCallTip.Destroy();
	delete []val;
	val = 0;
}

void CallTip::RefreshColourPalette(Palette &pal, bool want) {
	pal.WantFind(colourBG, want);
	pal.WantFind(colourUnSel, want);
	pal.WantFind(colourSel, want);
	pal.WantFind(colourShade, want);
	pal.WantFind(colourLight, want);
}

// Although this test includes 0, we should never see a \0 character.
static bool IsArrowCharacter(char ch) {
	return (ch == 0) || (ch == '\001') || (ch == '\002');
}

// We ignore tabs unless a tab width has been set.
bool CallTip::IsTabCharacter(char ch) {
	return (tabSize > 0) && (ch == '\t');
}

int CallTip::NextTabPos(int x) {
	if (tabSize > 0) {              // paranoia... not called unless this is true
		x -= insetX;                // position relative to text
		x = (x + tabSize) / tabSize;  // tab "number"
		return tabSize*x + insetX;  // position of next tab
	} else {
		return x + 1;                 // arbitrary
	}
}

// Draw a section of the call tip that does not include \n in one colour.
// The text may include up to numEnds tabs or arrow characters.
void CallTip::DrawChunk(Surface *surface, int &x, const char *s,
	int posStart, int posEnd, int ytext, PRectangle rcClient,
	bool highlight, bool draw) {
	s += posStart;
	int len = posEnd - posStart;

	// Divide the text into sections that are all text, or that are
	// single arrows or single tab characters (if tabSize > 0).
	int maxEnd = 0;
	const int numEnds = 10;
	int ends[numEnds + 2];
	for (int i=0;i<len;i++) {
		if ((maxEnd < numEnds) &&
		        (IsArrowCharacter(s[i]) || IsTabCharacter(s[i])) ) {
			if (i > 0)
				ends[maxEnd++] = i;
			ends[maxEnd++] = i+1;
		}
	}
	ends[maxEnd++] = len;
	int startSeg = 0;
	int xEnd;
	for (int seg = 0; seg<maxEnd; seg++) {
		int endSeg = ends[seg];
		if (endSeg > startSeg) {
			if (IsArrowCharacter(s[startSeg])) {
				bool upArrow = s[startSeg] == '\001';
				rcClient.left = x;
				rcClient.right = rcClient.left + widthArrow;
				if (draw) {
					const int halfWidth = widthArrow / 2 - 3;
					const int centreX = rcClient.left + widthArrow / 2 - 1;
					const int centreY = (rcClient.top + rcClient.bottom) / 2;
					surface->FillRectangle(rcClient, colourBG.allocated);
					PRectangle rcClientInner(rcClient.left + 1, rcClient.top + 1,
					                         rcClient.right - 2, rcClient.bottom - 1);
					surface->FillRectangle(rcClientInner, colourUnSel.allocated);

					if (upArrow) {      // Up arrow
						Point pts[] = {
    						Point(centreX - halfWidth, centreY + halfWidth / 2),
    						Point(centreX + halfWidth, centreY + halfWidth / 2),
    						Point(centreX, centreY - halfWidth + halfWidth / 2),
						};
						surface->Polygon(pts, sizeof(pts) / sizeof(pts[0]),
                 						colourBG.allocated, colourBG.allocated);
					} else {            // Down arrow
						Point pts[] = {
    						Point(centreX - halfWidth, centreY - halfWidth / 2),
    						Point(centreX + halfWidth, centreY - halfWidth / 2),
    						Point(centreX, centreY + halfWidth - halfWidth / 2),
						};
						surface->Polygon(pts, sizeof(pts) / sizeof(pts[0]),
                 						colourBG.allocated, colourBG.allocated);
					}
				}
				xEnd = rcClient.right;
				offsetMain = xEnd;
				if (upArrow) {
					rectUp = rcClient;
				} else {
					rectDown = rcClient;
				}
			} else if (IsTabCharacter(s[startSeg])) {
				xEnd = NextTabPos(x);
			} else {
				xEnd = x + surface->WidthText(font, s + startSeg, endSeg - startSeg);
				if (draw) {
					rcClient.left = x;
					rcClient.right = xEnd;
					surface->DrawTextTransparent(rcClient, font, ytext,
										s+startSeg, endSeg - startSeg,
					                             highlight ? colourSel.allocated : colourUnSel.allocated);
				}
			}
			x = xEnd;
			startSeg = endSeg;
		}
	}
}

int CallTip::PaintContents(Surface *surfaceWindow, bool draw) {
	PRectangle rcClientPos = wCallTip.GetClientPosition();
	PRectangle rcClientSize(0, 0, rcClientPos.right - rcClientPos.left,
	                        rcClientPos.bottom - rcClientPos.top);
	PRectangle rcClient(1, 1, rcClientSize.right - 1, rcClientSize.bottom - 1);

	// To make a nice small call tip window, it is only sized to fit most normal characters without accents
	int ascent = surfaceWindow->Ascent(font) - surfaceWindow->InternalLeading(font);

	// For each line...
	// Draw the definition in three parts: before highlight, highlighted, after highlight
	int ytext = rcClient.top + ascent + 1;
	rcClient.bottom = ytext + surfaceWindow->Descent(font) + 1;
	char *chunkVal = val;
	bool moreChunks = true;
	int maxWidth = 0;

	while (moreChunks) {
		char *chunkEnd = strchr(chunkVal, '\n');
		if (chunkEnd == NULL) {
			chunkEnd = chunkVal + strlen(chunkVal);
			moreChunks = false;
		}
		int chunkOffset = chunkVal - val;
		int chunkLength = chunkEnd - chunkVal;
		int chunkEndOffset = chunkOffset + chunkLength;
		int thisStartHighlight = Platform::Maximum(startHighlight, chunkOffset);
		thisStartHighlight = Platform::Minimum(thisStartHighlight, chunkEndOffset);
		thisStartHighlight -= chunkOffset;
		int thisEndHighlight = Platform::Maximum(endHighlight, chunkOffset);
		thisEndHighlight = Platform::Minimum(thisEndHighlight, chunkEndOffset);
		thisEndHighlight -= chunkOffset;
		rcClient.top = ytext - ascent - 1;

		int x = insetX;     // start each line at this inset

		DrawChunk(surfaceWindow, x, chunkVal, 0, thisStartHighlight,
			ytext, rcClient, false, draw);
		DrawChunk(surfaceWindow, x, chunkVal, thisStartHighlight, thisEndHighlight,
			ytext, rcClient, true, draw);
		DrawChunk(surfaceWindow, x, chunkVal, thisEndHighlight, chunkLength,
			ytext, rcClient, false, draw);

		chunkVal = chunkEnd + 1;
		ytext += lineHeight;
		rcClient.bottom += lineHeight;
		maxWidth = Platform::Maximum(maxWidth, x);
	}
	return maxWidth;
}

void CallTip::PaintCT(Surface *surfaceWindow) {
	if (!val)
		return;
	PRectangle rcClientPos = wCallTip.GetClientPosition();
	PRectangle rcClientSize(0, 0, rcClientPos.right - rcClientPos.left,
	                        rcClientPos.bottom - rcClientPos.top);
	PRectangle rcClient(1, 1, rcClientSize.right - 1, rcClientSize.bottom - 1);

	surfaceWindow->FillRectangle(rcClient, colourBG.allocated);

	offsetMain = insetX;    // initial alignment assuming no arrows
	PaintContents(surfaceWindow, true);

#ifndef __APPLE__
	// OSX doesn't put borders on "help tags"
	// Draw a raised border around the edges of the window
	surfaceWindow->MoveTo(0, rcClientSize.bottom - 1);
	surfaceWindow->PenColour(colourShade.allocated);
	surfaceWindow->LineTo(rcClientSize.right - 1, rcClientSize.bottom - 1);
	surfaceWindow->LineTo(rcClientSize.right - 1, 0);
	surfaceWindow->PenColour(colourLight.allocated);
	surfaceWindow->LineTo(0, 0);
	surfaceWindow->LineTo(0, rcClientSize.bottom - 1);
#endif
}

void CallTip::MouseClick(Point pt) {
	clickPlace = 0;
	if (rectUp.Contains(pt))
		clickPlace = 1;
	if (rectDown.Contains(pt))
		clickPlace = 2;
}

PRectangle CallTip::CallTipStart(int pos, Point pt, const char *defn,
                                 const char *faceName, int size,
                                 int codePage_, int characterSet, Window &wParent) {
	clickPlace = 0;
	if (val)
		delete []val;
	val = new char[strlen(defn) + 1];
	if (!val)
		return PRectangle();
	strcpy(val, defn);
	codePage = codePage_;
	Surface *surfaceMeasure = Surface::Allocate();
	if (!surfaceMeasure)
		return PRectangle();
	surfaceMeasure->Init(wParent.GetID());
	surfaceMeasure->SetUnicodeMode(SC_CP_UTF8 == codePage);
	surfaceMeasure->SetDBCSMode(codePage);
	startHighlight = 0;
	endHighlight = 0;
	inCallTipMode = true;
	posStartCallTip = pos;
	int deviceHeight = surfaceMeasure->DeviceHeightFont(size);
	font.Create(faceName, characterSet, deviceHeight, false, false);
	// Look for multiple lines in the text
	// Only support \n here - simply means container must avoid \r!
	int numLines = 1;
	const char *newline;
	const char *look = val;
	rectUp = PRectangle(0,0,0,0);
	rectDown = PRectangle(0,0,0,0);
	offsetMain = insetX;            // changed to right edge of any arrows
	int width = PaintContents(surfaceMeasure, false) + insetX;
	while ((newline = strchr(look, '\n')) != NULL) {
		look = newline + 1;
		numLines++;
	}
	lineHeight = surfaceMeasure->Height(font);

	// Extra line for border and an empty line at top and bottom. The returned
	// rectangle is aligned to the right edge of the last arrow encountered in
	// the tip text, else to the tip text left edge.
	int height = lineHeight * numLines - surfaceMeasure->InternalLeading(font) + 2 + 2;
	delete surfaceMeasure;
	return PRectangle(pt.x - offsetMain, pt.y + 1, pt.x + width - offsetMain, pt.y + 1 + height);
}

void CallTip::CallTipCancel() {
	inCallTipMode = false;
	if (wCallTip.Created()) {
		wCallTip.Destroy();
	}
}

void CallTip::SetHighlight(int start, int end) {
	// Avoid flashing by checking something has really changed
	if ((start != startHighlight) || (end != endHighlight)) {
		startHighlight = start;
		endHighlight = end;
		if (wCallTip.Created()) {
			wCallTip.InvalidateAll();
		}
	}
}

// Set the tab size (sizes > 0 enable the use of tabs). This also enables the
// use of the STYLE_CALLTIP.
void CallTip::SetTabSize(int tabSz) {
	tabSize = tabSz;
	useStyleCallTip = true;
}

// It might be better to have two access functions for this and to use
// them for all settings of colours.
void CallTip::SetForeBack(const ColourPair &fore, const ColourPair &back) {
	colourBG = back;
	colourUnSel = fore;
}
