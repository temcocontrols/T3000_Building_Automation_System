

#include <string.h>

#include "Platform.h"

#include "SplitVector.h"
#include "Partitioning.h"
#include "RunStyles.h"
#include "ConnecttionState.h"

#ifdef T3000_NAMESPACE
using namespace T3000_NAMESPACE;
#endif

ConnecttionState::ConnecttionState() : visible(0), expanded(0), heights(0), displayLines(0), linesInDocument(1) {
	//InsertLine(0);
}

ConnecttionState::~ConnecttionState() {
	Clear();
}

void ConnecttionState::EnsureData() {
	if (OneToOne()) {
		visible = new RunStyles();
		expanded = new RunStyles();
		heights = new RunStyles();
		displayLines = new Partitioning(4);
		InsertLines(0, linesInDocument);
	}
}

void ConnecttionState::Clear() {
	delete visible;
	visible = 0;
	delete expanded;
	expanded = 0;
	delete heights;
	heights = 0;
	delete displayLines;
	displayLines = 0;
	linesInDocument = 1;
}

int ConnecttionState::LinesInDoc() const {
	if (OneToOne()) {
		return linesInDocument;
	} else {
		return displayLines->Partitions() - 1;
	}
}

int ConnecttionState::LinesDisplayed() const {
	if (OneToOne()) {
		return linesInDocument;
	} else {
		return displayLines->PositionFromPartition(LinesInDoc());
	}
}

int ConnecttionState::DisplayFromDoc(int lineDoc) const {
	if (OneToOne()) {
		return lineDoc;
	} else {
		if (lineDoc > displayLines->Partitions())
			lineDoc = displayLines->Partitions();
		return displayLines->PositionFromPartition(lineDoc);
	}
}

int ConnecttionState::DocFromDisplay(int lineDisplay) const {
	if (OneToOne()) {
		return lineDisplay;
	} else {
		if (lineDisplay <= 0) {
			return 0;
		}
		if (lineDisplay > LinesDisplayed()) {
			return displayLines->PartitionFromPosition(LinesDisplayed());
		}
		int lineDoc = displayLines->PartitionFromPosition(lineDisplay);
		PLATFORM_ASSERT(GetVisible(lineDoc));
		return lineDoc;
	}
}

void ConnecttionState::InsertLine(int lineDoc) {
	if (OneToOne()) {
		linesInDocument++;
	} else {
		visible->InsertSpace(lineDoc, 1);
		visible->SetValueAt(lineDoc, 1);
		expanded->InsertSpace(lineDoc, 1);
		expanded->SetValueAt(lineDoc, 1);
		heights->InsertSpace(lineDoc, 1);
		heights->SetValueAt(lineDoc, 1);
		int lineDisplay = DisplayFromDoc(lineDoc);
		displayLines->InsertPartition(lineDoc, lineDisplay);
		displayLines->InsertText(lineDoc, 1);
	}
}

void ConnecttionState::InsertLines(int lineDoc, int lineCount) {
	for (int l = 0; l < lineCount; l++) {
		InsertLine(lineDoc + l);
	}
	Check();
}

void ConnecttionState::DeleteLine(int lineDoc) {
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
	}
}

void ConnecttionState::DeleteLines(int lineDoc, int lineCount) {
	for (int l = 0; l < lineCount; l++) {
		DeleteLine(lineDoc);
	}
	Check();
}

bool ConnecttionState::GetVisible(int lineDoc) const {
	if (OneToOne()) {
		return true;
	} else {
		if (lineDoc >= visible->Length())
			return true;
		return visible->ValueAt(lineDoc) == 1;
	}
}

bool ConnecttionState::SetVisible(int lineDocStart, int lineDocEnd, bool visible_) {
	if (OneToOne() && visible_) {
		return false;
	} else {
		EnsureData();
		int delta = 0;
		Check();
		if ((lineDocStart <= lineDocEnd) && (lineDocStart >= 0) && (lineDocEnd < LinesInDoc())) {
			for (int line = lineDocStart; line <= lineDocEnd; line++) {
				if (GetVisible(line) != visible_) {
					int difference = visible_ ? heights->ValueAt(line) : -heights->ValueAt(line);
					visible->SetValueAt(line, visible_ ? 1 : 0);
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

bool ConnecttionState::GetExpanded(int lineDoc) const {
	if (OneToOne()) {
		return true;
	} else {
		Check();
		return expanded->ValueAt(lineDoc) == 1;
	}
}

bool ConnecttionState::SetExpanded(int lineDoc, bool expanded_) {
	if (OneToOne() && expanded_) {
		return false;
	} else {
		EnsureData();
		if (expanded_ != (expanded->ValueAt(lineDoc) == 1)) {
			expanded->SetValueAt(lineDoc, expanded_ ? 1 : 0);
			Check();
			return true;
		} else {
			Check();
			return false;
		}
	}
}

int ConnecttionState::GetHeight(int lineDoc) const {
	if (OneToOne()) {
		return 1;
	} else {
		return heights->ValueAt(lineDoc);
	}
}

// Set the number of display lines needed for this line.
// Return true if this is a change.
bool ConnecttionState::SetHeight(int lineDoc, int height) {
	if (OneToOne() && (height == 1)) {
		return false;
	} else {
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
	}
}

void ConnecttionState::ShowAll() {
	int lines = LinesInDoc();
	Clear();
	linesInDocument = lines;
}

// Debugging checks

void ConnecttionState::Check() const {
#ifdef CHECK_CORRECTNESS
	for (int vline = 0;vline < LinesDisplayed(); vline++) {
		const int lineDoc = DocFromDisplay(vline);
		PLATFORM_ASSERT(GetVisible(lineDoc));
	}
	for (int lineDoc = 0;lineDoc < LinesInDoc(); lineDoc++) {
		const int displayThis = DisplayFromDoc(lineDoc);
		const int displayNext = DisplayFromDoc(lineDoc + 1);
		const int height = displayNext - displayThis;
		PLATFORM_ASSERT(height >= 0);
		if (GetVisible(lineDoc)) {
			PLATFORM_ASSERT(GetHeight(lineDoc) == height);
		} else {
			PLATFORM_ASSERT(0 == height);
		}
	}
#endif
}
