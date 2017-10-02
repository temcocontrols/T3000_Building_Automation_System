#ifndef CASEFOLDER_H
#define CASEFOLDER_H

namespace T3000_CaseFolder {

class CaseFolder {
public:
	virtual ~CaseFolder();
	virtual size_t Fold(char *folded, size_t sizeFolded, const char *mixed, size_t lenMixed) = 0;
};

class CaseFolderTable : public CaseFolder {
protected:
	char mapping[256];
public:
	CaseFolderTable();
	virtual ~CaseFolderTable();
	size_t Fold(char *folded, size_t sizeFolded, const char *mixed, size_t lenMixed) override;
	void SetTranslation(char ch, char chTranslation);
	void StandardASCII();
};

class ICaseConverter;

class CaseFolderUnicode : public CaseFolderTable {
	ICaseConverter *converter;
public:
	CaseFolderUnicode();
	size_t Fold(char *folded, size_t sizeFolded, const char *mixed, size_t lenMixed) override;
};

}

#endif
