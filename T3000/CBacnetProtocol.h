

#ifndef CBacnetProtocol_H
#define CBacnetProtocol_H

namespace T3000 {

class CBacnetProtocol {
private:
	std::unique_ptr<Partitioning> starts;
	std::unique_ptr<SplitVector<int>> styles;
	int RunFromPosition(int position) const;
	int SplitRun(int position);
	void RemoveRun(int run);
	void RemoveRunIfEmpty(int run);
	void RemoveRunIfSameAsPrevious(int run);
public:
	CBacnetProtocol();
	// Deleted so CBacnetProtocol objects can not be copied.
	CBacnetProtocol(const CBacnetProtocol &) = delete;
	void operator=(const CBacnetProtocol &) = delete;
	~CBacnetProtocol();
	int Length() const;
	int ValueAt(int position) const;
	int FindNextChange(int position, int end) const;
	int StartRun(int position) const;
	int EndRun(int position) const;
	// Returns true if some values may have changed
	bool FillRange(int &position, int value, int &fillLength);
	void SetValueAt(int position, int value);
	void InsertSpace(int position, int insertLength);
	void DeleteAll();
	void DeleteRange(int position, int deleteLength);
	int Runs() const;
	bool AllSame() const;
	bool AllSameAs(int value) const;
	int Find(int value, int start) const;

	void Check() const;
};

}

#endif
