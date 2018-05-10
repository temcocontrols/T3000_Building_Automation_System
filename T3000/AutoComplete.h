

#ifndef AUTOCOMPLETE_H
#define AUTOCOMPLETE_H

namespace Scintilla {

/**
 */
class AutoComplete {
	bool active;
	std::string stopChars;
	std::string fillUpChars;
	char separator;
	char typesep; // Type seperator
	enum { maxItemLen=1000 };
	std::vector<int> sortMatrix;

public:

	bool ignoreCase;
	bool chooseSingle;
	std::unique_ptr<ListBox> lb;
	Sci::Position posStart;
	int startLen;
	/// Should autocompletion be canceled if editor's currentPos <= startPos?
	bool cancelAtStartPos;
	bool autoHide;
	bool dropRestOfWord;
	unsigned int ignoreCaseBehaviour;
	int widthLBDefault;
	int heightLBDefault;
	/** SC_ORDER_PRESORTED:   Assume the list is presorted; selection will fail if it is not alphabetical<br />
	 *  SC_ORDER_PERFORMSORT: Sort the list alphabetically; start up performance cost for sorting<br />
	 *  SC_ORDER_CUSTOM:      Handle non-alphabetical entries; start up performance cost for generating a sorted lookup table
	 */
	int autoSort;

	AutoComplete();
	~AutoComplete();

	/// Is the auto completion list displayed?
	bool Active() const;

	/// Display the auto completion list positioned to be near a character position
	void Start(Window &parent, int ctrlID, Sci::Position position, Point location,
		int startLen_, int lineHeight, bool unicodeMode, int technology);

	/// The stop chars are characters which, when typed, cause the auto completion list to disappear
	void SetStopChars(const char *stopChars_);
	bool IsStopChar(char ch);

	/// The fillup chars are characters which, when typed, fill up the selected word
	void SetFillUpChars(const char *fillUpChars_);
	bool IsFillUpChar(char ch);

	/// The separator character is used when interpreting the list in SetList
	void SetSeparator(char separator_);
	char GetSeparator() const;

	/// The typesep character is used for separating the word from the type
	void SetTypesep(char separator_);
	char GetTypesep() const;

	/// The list string contains a sequence of words separated by the separator character
	void SetList(const char *list);

	/// Return the position of the currently selected list item
	int GetSelection() const;

	/// Return the value of an item in the list
	std::string GetValue(int item) const;

	void Show(bool show);
	void Cancel();

	/// Move the current list element by delta, scrolling appropriately
	void Move(int delta);

	/// Select a list element that starts with word as the current element
	void Select(const char *word);
};


class CallTip {
	int startHighlight;    // character offset to start and...
	int endHighlight;      // ...end of highlighted text
	char *val;
	Font font;
	PRectangle rectUp;      // rectangle of last up angle in the tip
	PRectangle rectDown;    // rectangle of last down arrow in the tip
	int lineHeight;         // vertical line spacing
	int offsetMain;         // The alignment point of the call tip
	int tabSize;            // Tab size in pixels, <=0 no TAB expand
	bool useStyleCallTip;   // if true, STYLE_CALLTIP should be used

	// Private so CallTip objects can not be copied
	CallTip(const CallTip &) {}
	CallTip &operator=(const CallTip &) { return *this; }
	void DrawChunk(Surface *surface, int &x, const char *s,
		int posStart, int posEnd, int ytext, PRectangle rcClient,
		bool highlight, bool draw);
	int PaintContents(Surface *surfaceWindow, bool draw);
	bool IsTabCharacter(char c);
	int NextTabPos(int x);

public:
	Window wCallTip;
	Window wDraw;
	bool inCallTipMode;
	int posStartCallTip;
	ColourPair colourBG;
	ColourPair colourUnSel;
	ColourPair colourSel;
	ColourPair colourShade;
	ColourPair colourLight;
	int codePage;
	int clickPlace;

	CallTip();
	~CallTip();

	/// Claim or accept palette entries for the colours required to paint a calltip.
	void RefreshColourPalette(Palette &pal, bool want);

	void PaintCT(Surface *surfaceWindow);

	void MouseClick(Point pt);

	/// Setup the calltip and return a rectangle of the area required.
	PRectangle CallTipStart(int pos, Point pt, const char *defn,
		const char *faceName, int size, int codePage_, 
		int characterSet, Window &wParent);

	void CallTipCancel();

	/// Set a range of characters to be displayed in a highlight style.
	/// Commonly used to highlight the current parameter.
	void SetHighlight(int start, int end);

	/// Set the tab size in pixels for the call tip. 0 or -ve means no tab expand.
	void SetTabSize(int tabSz);

	/// Used to determine which STYLE_xxxx to use for call tip information
	bool UseStyleCallTip() const { return useStyleCallTip;}

	// Modify foreground and background colours
	void SetForeBack(const ColourPair &fore, const ColourPair &back);
};
	
}

#endif
