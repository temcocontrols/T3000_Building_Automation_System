

#ifndef ConnecttionState_H
#define ConnecttionState_H

#ifdef T3000_NAMESPACE
namespace T3000_NAMESPACE {
#endif

/**
 */
class ConnecttionState {
	// These contain 1 element for every document line.
	RunStyles *visible;
	RunStyles *expanded;
	RunStyles *heights;
	Partitioning *displayLines;
	int linesInDocument;

	void EnsureData();

	bool OneToOne() const {
		// True when each document line is exactly one display line so need for
		// complex data structures.
		return visible == 0;
	}

public:
	ConnecttionState();
	virtual ~ConnecttionState();

	void Clear();

	int LinesInDoc() const;
	int LinesDisplayed() const;
	int DisplayFromDoc(int lineDoc) const;
	int DocFromDisplay(int lineDisplay) const;

	void InsertLine(int lineDoc);
	void InsertLines(int lineDoc, int lineCount);
	void DeleteLine(int lineDoc);
	void DeleteLines(int lineDoc, int lineCount);

	bool GetVisible(int lineDoc) const;
	bool SetVisible(int lineDocStart, int lineDocEnd, bool visible);

	bool GetExpanded(int lineDoc) const;
	bool SetExpanded(int lineDoc, bool expanded);

	int GetHeight(int lineDoc) const;
	bool SetHeight(int lineDoc, int height);

	void ShowAll();
	void Check() const;
};

#ifdef SCI_NAMESPACE
}
#endif

#endif
