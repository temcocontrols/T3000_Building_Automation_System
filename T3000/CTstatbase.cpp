

#include <cstddef>
#include <cassert>
#include <cstring>

#include <stdexcept>
#include <vector>
#include <algorithm>
#include <memory>

#include "Platform.h"

#include "Position.h"
#include "UniqueString.h"
#include "SplitVector.h"
#include "Partitioning.h"
#include "RunStyles.h"
#include "SparseVector.h"
#include "CTStatBase.h"

using namespace T3000;

CTStatBase::CTStatBase() : linesInDocument(1) {
}

CTStatBase::~CTStatBase() {
	Clear();
}

void CTStatBase::EnsureData() {
	if (OneToOne()) {
		visible.reset(new RunStyles());
		expanded.reset(new RunStyles());
		heights.reset(new RunStyles());
		foldDisplayTexts.reset(new SparseVector<UniqueString>());
		displayLines.reset(new Partitioning(4));
		InsertLines(0, linesInDocument);
	}
}

void CTStatBase::Clear() {
	visible.reset();
	expanded.reset();
	heights.reset();
	foldDisplayTexts.reset();
	displayLines.reset();
	linesInDocument = 1;
}

Sci::Line CTStatBase::LinesInDoc() const {
	if (OneToOne()) {
		return linesInDocument;
	} else {
		return displayLines->Partitions() - 1;
	}
}

Sci::Line CTStatBase::LinesDisplayed() const {
	if (OneToOne()) {
		return linesInDocument;
	} else {
		return displayLines->PositionFromPartition(LinesInDoc());
	}
}

Sci::Line CTStatBase::DisplayFromDoc(Sci::Line lineDoc) const {
	if (OneToOne()) {
		return (lineDoc <= linesInDocument) ? lineDoc : linesInDocument;
	} else {
		if (lineDoc > displayLines->Partitions())
			lineDoc = displayLines->Partitions();
		return displayLines->PositionFromPartition(lineDoc);
	}
}

Sci::Line CTStatBase::DisplayLastFromDoc(Sci::Line lineDoc) const {
	return DisplayFromDoc(lineDoc) + GetHeight(lineDoc) - 1;
}

Sci::Line CTStatBase::DocFromDisplay(Sci::Line lineDisplay) const {
	if (OneToOne()) {
		return lineDisplay;
	} else {
		if (lineDisplay <= 0) {
			return 0;
		}
		if (lineDisplay > LinesDisplayed()) {
			return displayLines->PartitionFromPosition(LinesDisplayed());
		}
		Sci::Line lineDoc = displayLines->PartitionFromPosition(lineDisplay);
		PLATFORM_ASSERT(GetVisible(lineDoc));
		return lineDoc;
	}
}

void CTStatBase::InsertLine(Sci::Line lineDoc) {
	if (OneToOne()) {
		linesInDocument++;
	} else {
		visible->InsertSpace(lineDoc, 1);
		visible->SetValueAt(lineDoc, 1);
		expanded->InsertSpace(lineDoc, 1);
		expanded->SetValueAt(lineDoc, 1);
		heights->InsertSpace(lineDoc, 1);
		heights->SetValueAt(lineDoc, 1);
		foldDisplayTexts->InsertSpace(lineDoc, 1);
		foldDisplayTexts->SetValueAt(lineDoc, nullptr);
		Sci::Line lineDisplay = DisplayFromDoc(lineDoc);
		displayLines->InsertPartition(lineDoc, lineDisplay);
		displayLines->InsertText(lineDoc, 1);
	}
}

void CTStatBase::InsertLines(Sci::Line lineDoc, Sci::Line lineCount) {
	for (int l = 0; l < lineCount; l++) {
		InsertLine(lineDoc + l);
	}
	Check();
}

void CTStatBase::DeleteLine(Sci::Line lineDoc) {
	if (OneToOne()) {
		linesInDocument--;
	} else {
		if (GetVisible(lineDoc)) {
			displayLines->InsertText(lineDoc, -heights->ValueAt(lineDoc));
		}
		displayLines->RemovePartition(lineDoc);
		visible->DeleteRange(lineDoc, 1);
		expanded->DeleteRange(lineDoc, 1);
		heights->DeleteRange(lineDoc, 1);
		foldDisplayTexts->DeletePosition(lineDoc);
	}
}

void CTStatBase::DeleteLines(Sci::Line lineDoc, Sci::Line lineCount) {
	for (Sci::Line l = 0; l < lineCount; l++) {
		DeleteLine(lineDoc);
	}
	Check();
}

bool CTStatBase::GetVisible(Sci::Line lineDoc) const {
	if (OneToOne()) {
		return true;
	} else {
		if (lineDoc >= visible->Length())
			return true;
		return visible->ValueAt(lineDoc) == 1;
	}
}

bool CTStatBase::SetVisible(Sci::Line lineDocStart, Sci::Line lineDocEnd, bool isVisible) {
	if (OneToOne() && isVisible) {
		return false;
	} else {
		EnsureData();
		Sci::Line delta = 0;
		Check();
		if ((lineDocStart <= lineDocEnd) && (lineDocStart >= 0) && (lineDocEnd < LinesInDoc())) {
			for (Sci::Line line = lineDocStart; line <= lineDocEnd; line++) {
				if (GetVisible(line) != isVisible) {
					int difference = isVisible ? heights->ValueAt(line) : -heights->ValueAt(line);
					visible->SetValueAt(line, isVisible ? 1 : 0);
					displayLines->InsertText(line, difference);
					delta += difference;
				}
			}
		} else {
			return false;
		}
		Check();
		return delta != 0;
	}
}

bool CTStatBase::HiddenLines() const {
	if (OneToOne()) {
		return false;
	} else {
		return !visible->AllSameAs(1);
	}
}

const char *CTStatBase::GetFoldDisplayText(Sci::Line lineDoc) const {
	Check();
	return foldDisplayTexts->ValueAt(lineDoc).get();
}

bool CTStatBase::SetFoldDisplayText(Sci::Line lineDoc, const char *text) {
	EnsureData();
	const char *foldText = foldDisplayTexts->ValueAt(lineDoc).get();
	if (!foldText || !text || 0 != strcmp(text, foldText)) {
		foldDisplayTexts->SetValueAt(lineDoc, UniqueStringCopy(text));
		Check();
		return true;
	} else {
		Check();
		return false;
	}
}

bool CTStatBase::GetExpanded(Sci::Line lineDoc) const {
	if (OneToOne()) {
		return true;
	} else {
		Check();
		return expanded->ValueAt(lineDoc) == 1;
	}
}

bool CTStatBase::SetExpanded(Sci::Line lineDoc, bool isExpanded) {
	if (OneToOne() && isExpanded) {
		return false;
	} else {
		EnsureData();
		if (isExpanded != (expanded->ValueAt(lineDoc) == 1)) {
			expanded->SetValueAt(lineDoc, isExpanded ? 1 : 0);
			Check();
			return true;
		} else {
			Check();
			return false;
		}
	}
}

bool CTStatBase::GetFoldDisplayTextShown(Sci::Line lineDoc) const {
	return !GetExpanded(lineDoc) && GetFoldDisplayText(lineDoc);
}

Sci::Line CTStatBase::ContractedNext(Sci::Line lineDocStart) const {
	if (OneToOne()) {
		return -1;
	} else {
		Check();
		if (!expanded->ValueAt(lineDocStart)) {
			return lineDocStart;
		} else {
			Sci::Line lineDocNextChange = expanded->EndRun(lineDocStart);
			if (lineDocNextChange < LinesInDoc())
				return lineDocNextChange;
			else
				return -1;
		}
	}
}

int CTStatBase::GetHeight(Sci::Line lineDoc) const {
	if (OneToOne()) {
		return 1;
	} else {
		return heights->ValueAt(lineDoc);
	}
}

// Set the number of display lines needed for this line.
// Return true if this is a change.
bool CTStatBase::SetHeight(Sci::Line lineDoc, int height) {
	if (OneToOne() && (height == 1)) {
		return false;
	} else if (lineDoc < LinesInDoc()) {
		EnsureData();
		if (GetHeight(lineDoc) != height) {
			if (GetVisible(lineDoc)) {
				displayLines->InsertText(lineDoc, height - GetHeight(lineDoc));
			}
			heights->SetValueAt(lineDoc, height);
			Check();
			return true;
		} else {
			Check();
			return false;
		}
	} else {
		return false;
	}
}

void CTStatBase::ShowAll() {
	Sci::Line lines = LinesInDoc();
	Clear();
	linesInDocument = lines;
}

// Debugging checks

void CTStatBase::Check() const {
#ifdef CHECK_CORRECTNESS
	for (Sci::Line vline = 0; vline < LinesDisplayed(); vline++) {
		const Sci::Line lineDoc = DocFromDisplay(vline);
		PLATFORM_ASSERT(GetVisible(lineDoc));
	}
	for (Sci::Line lineDoc = 0; lineDoc < LinesInDoc(); lineDoc++) {
		const Sci::Line displayThis = DisplayFromDoc(lineDoc);
		const Sci::Line displayNext = DisplayFromDoc(lineDoc + 1);
		const Sci::Line height = displayNext - displayThis;
		PLATFORM_ASSERT(height >= 0);
		if (GetVisible(lineDoc)) {
			PLATFORM_ASSERT(GetHeight(lineDoc) == height);
		} else {
			PLATFORM_ASSERT(0 == height);
		}
	}
#endif
}
