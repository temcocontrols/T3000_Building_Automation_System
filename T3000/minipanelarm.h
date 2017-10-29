#ifndef MINIPANEL_ARM_H
#define MINIPANEL_ARM_H

namespace T3000 {

class MINIPANEL_ARM {
	int indicator;
public:
	RunStyles rs;

	explicit MINIPANEL_ARM(int indicator_);
	~MINIPANEL_ARM();

	bool Empty() const;
	int Indicator() const {
		return indicator;
	}
};

class MINIPANEL_ARMList {
	int currentIndicator;
	int currentValue;
	MINIPANEL_ARM *current;	// Cached so FillRange doesn't have to search for each call.
	int lengthDocument;
	// Ordered by indicator
	std::vector<std::unique_ptr<MINIPANEL_ARM>> MINIPANEL_ARMList;
	std::vector<const MINIPANEL_ARM*> MINIPANEL_ARMView;	// Read-only view of MINIPANEL_ARMList
	bool clickNotified;

	MINIPANEL_ARM *MINIPANEL_ARMFromIndicator(int indicator);
	MINIPANEL_ARM *Create(int indicator, int length);
	void Delete(int indicator);
	void DeleteAnyEmpty();
	void SetView();
public:

	MINIPANEL_ARMList();
	~MINIPANEL_ARMList();

	const std::vector<const MINIPANEL_ARM*> &View() const { return MINIPANEL_ARMView; }

	void SetCurrentIndicator(int indicator);
	int GetCurrentIndicator() const { return currentIndicator; }

	void SetCurrentValue(int value);
	int GetCurrentValue() const { return currentValue; }

	// Returns true if some values may have changed
	bool FillRange(int &position, int value, int &fillLength);

	void InsertSpace(int position, int insertLength);
	void DeleteRange(int position, int deleteLength);

	void DeleteLexerMINIPANEL_ARMs();

	int AllOnFor(int position) const;
	int ValueAt(int indicator, int position);
	int Start(int indicator, int position);
	int End(int indicator, int position);

	bool ClickNotified() const {
		return clickNotified;
	}
	void SetClickNotified(bool notified) {
		clickNotified = notified;
	}
};

}

#endif
