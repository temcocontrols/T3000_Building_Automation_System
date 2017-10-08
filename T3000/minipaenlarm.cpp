

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cstdarg>

#include <stdexcept>
#include <vector>
#include <algorithm>
#include <memory>

#include "Platform.h"

#include "T3000.h"
#include "Position.h"
#include "SplitVector.h"
#include "Partitioning.h"
#include "RunStyles.h"
#include "MINIPANEL_ARM.h"

using namespace T3000;

MINIPANEL_ARM::MINIPANEL_ARM(int indicator_) : indicator(indicator_) {
}

MINIPANEL_ARM::~MINIPANEL_ARM() {
}

bool MINIPANEL_ARM::Empty() const {
	return (rs.Runs() == 1) && (rs.AllSameAs(0));
}

MINIPANEL_ARMList::MINIPANEL_ARMList() : currentIndicator(0), currentValue(1), current(nullptr),
	lengthDocument(0), clickNotified(false) {
}

MINIPANEL_ARMList::~MINIPANEL_ARMList() {
	current = nullptr;
}

MINIPANEL_ARM *MINIPANEL_ARMList::MINIPANEL_ARMFromIndicator(int indicator) {
	for (const std::unique_ptr<MINIPANEL_ARM> &deco : MINIPANEL_ARMList) {
		if (deco->Indicator() == indicator) {
			return deco.get();
		}
	}
	return nullptr;
}

MINIPANEL_ARM *MINIPANEL_ARMList::Create(int indicator, int length) {
	currentIndicator = indicator;
	std::unique_ptr<MINIPANEL_ARM> decoNew(new MINIPANEL_ARM(indicator));
	decoNew->rs.InsertSpace(0, length);

	std::vector<std::unique_ptr<MINIPANEL_ARM>>::iterator it = std::lower_bound(
		MINIPANEL_ARMList.begin(), MINIPANEL_ARMList.end(), decoNew, 
		[](const std::unique_ptr<MINIPANEL_ARM> &a, const std::unique_ptr<MINIPANEL_ARM> &b) {
		return a->Indicator() < b->Indicator();
	});
	std::vector<std::unique_ptr<MINIPANEL_ARM>>::iterator itAdded = 
		MINIPANEL_ARMList.insert(it, std::move(decoNew));

	SetView();

	return itAdded->get();
}

void MINIPANEL_ARMList::Delete(int indicator) {
	MINIPANEL_ARMList.erase(std::remove_if(MINIPANEL_ARMList.begin(), MINIPANEL_ARMList.end(),
		[=](const std::unique_ptr<MINIPANEL_ARM> &deco) {
		return deco->Indicator() == indicator;
	}), MINIPANEL_ARMList.end());
	current = nullptr;
	SetView();
}

void MINIPANEL_ARMList::SetCurrentIndicator(int indicator) {
	currentIndicator = indicator;
	current = MINIPANEL_ARMFromIndicator(indicator);
	currentValue = 1;
}

void MINIPANEL_ARMList::SetCurrentValue(int value) {
	currentValue = value ? value : 1;
}

bool MINIPANEL_ARMList::FillRange(int &position, int value, int &fillLength) {
	if (!current) {
		current = MINIPANEL_ARMFromIndicator(currentIndicator);
		if (!current) {
			current = Create(currentIndicator, lengthDocument);
		}
	}
	const bool changed = current->rs.FillRange(position, value, fillLength);
	if (current->Empty()) {
		Delete(currentIndicator);
	}
	return changed;
}

void MINIPANEL_ARMList::InsertSpace(int position, int insertLength) {
	const bool atEnd = position == lengthDocument;
	lengthDocument += insertLength;
	for (const std::unique_ptr<MINIPANEL_ARM> &deco : MINIPANEL_ARMList) {
		deco->rs.InsertSpace(position, insertLength);
		if (atEnd) {
			deco->rs.FillRange(position, 0, insertLength);
		}
	}
}

void MINIPANEL_ARMList::DeleteRange(int position, int deleteLength) {
	lengthDocument -= deleteLength;
	for (const std::unique_ptr<MINIPANEL_ARM> &deco : MINIPANEL_ARMList) {
		deco->rs.DeleteRange(position, deleteLength);
	}
	DeleteAnyEmpty();
	if (MINIPANEL_ARMList.size() != MINIPANEL_ARMView.size()) {
		// One or more empty MINIPANEL_ARMs deleted so update view.
		current = nullptr;
		SetView();
	}
}

void MINIPANEL_ARMList::DeleteLexerMINIPANEL_ARMs() {
	MINIPANEL_ARMList.erase(std::remove_if(MINIPANEL_ARMList.begin(), MINIPANEL_ARMList.end(),
		[=](const std::unique_ptr<MINIPANEL_ARM> &deco) {
		return deco->Indicator() < INDIC_CONTAINER;
	}), MINIPANEL_ARMList.end());
	current = nullptr;
	SetView();
}

void MINIPANEL_ARMList::DeleteAnyEmpty() {
	if (lengthDocument == 0) {
		MINIPANEL_ARMList.clear();
	} else {
		MINIPANEL_ARMList.erase(std::remove_if(MINIPANEL_ARMList.begin(), MINIPANEL_ARMList.end(),
			[=](const std::unique_ptr<MINIPANEL_ARM> &deco) {
			return deco->Empty();
		}), MINIPANEL_ARMList.end());
	}
}

void MINIPANEL_ARMList::SetView() {
	MINIPANEL_ARMView.clear();
	for (const std::unique_ptr<MINIPANEL_ARM> &deco : MINIPANEL_ARMList) {
		MINIPANEL_ARMView.push_back(deco.get());
	}
}

int MINIPANEL_ARMList::AllOnFor(int position) const {
	int mask = 0;
	for (const std::unique_ptr<MINIPANEL_ARM> &deco : MINIPANEL_ARMList) {
		if (deco->rs.ValueAt(position)) {
			if (deco->Indicator() < INDIC_IME) {
				mask |= 1 << deco->Indicator();
			}
		}
	}
	return mask;
}

int MINIPANEL_ARMList::ValueAt(int indicator, int position) {
	const MINIPANEL_ARM *deco = MINIPANEL_ARMFromIndicator(indicator);
	if (deco) {
		return deco->rs.ValueAt(position);
	}
	return 0;
}

int MINIPANEL_ARMList::Start(int indicator, int position) {
	const MINIPANEL_ARM *deco = MINIPANEL_ARMFromIndicator(indicator);
	if (deco) {
		return deco->rs.StartRun(position);
	}
	return 0;
}

int MINIPANEL_ARMList::End(int indicator, int position) {
	const MINIPANEL_ARM *deco = MINIPANEL_ARMFromIndicator(indicator);
	if (deco) {
		return deco->rs.EndRun(position);
	}
	return 0;
}
