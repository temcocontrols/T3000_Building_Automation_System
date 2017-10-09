

#ifndef CTStatBase_H
#define CTStatBase_H

namespace T3000 {

template<class T>
class SparseVector;

/**
 */
class CTStatBase {
	// These contain 1 element for every document line.
	std::unique_ptr<RunStyles> visible;
	std::unique_ptr<RunStyles> expanded;
	std::unique_ptr<RunStyles> heights;
	std::unique_ptr<SparseVector<UniqueString>> foldDisplayTexts;
	std::unique_ptr<Partitioning> displayLines;
	Sci::Line linesInDocument;

	void EnsureData();

	bool OneToOne() const {
		// True when each document line is exactly one display line so need for
		// complex data structures.
		return visible == nullptr;
	}

public:
	CTStatBase();
	// Deleted so CTStatBase objects can not be copied.
	CTStatBase(const CTStatBase &) = delete;
	void operator=(const CTStatBase &) = delete;
	virtual ~CTStatBase();

	void Clear();

	Sci::Line LinesInDoc() const;
	Sci::Line LinesDisplayed() const;
	Sci::Line DisplayFromDoc(Sci::Line lineDoc) const;
	Sci::Line DisplayLastFromDoc(Sci::Line lineDoc) const;
	Sci::Line DocFromDisplay(Sci::Line lineDisplay) const;

	void InsertLine(Sci::Line lineDoc);
	void InsertLines(Sci::Line lineDoc, Sci::Line lineCount);
	void DeleteLine(Sci::Line lineDoc);
	void DeleteLines(Sci::Line lineDoc, Sci::Line lineCount);

	bool GetVisible(Sci::Line lineDoc) const;
	bool SetVisible(Sci::Line lineDocStart, Sci::Line lineDocEnd, bool isVisible);
	bool HiddenLines() const;

	const char *GetFoldDisplayText(Sci::Line lineDoc) const;
	bool SetFoldDisplayText(Sci::Line lineDoc, const char *text);

	bool GetExpanded(Sci::Line lineDoc) const;
	bool SetExpanded(Sci::Line lineDoc, bool isExpanded);
	bool GetFoldDisplayTextShown(Sci::Line lineDoc) const;
	Sci::Line ContractedNext(Sci::Line lineDocStart) const;

	int GetHeight(Sci::Line lineDoc) const;
	bool SetHeight(Sci::Line lineDoc, int height);

	void ShowAll();
	void Check() const;
};

}

#endif
