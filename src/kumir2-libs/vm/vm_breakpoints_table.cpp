#include "vm_breakpoints_table.hpp"

namespace VM
{

bool BreakpointsTable::processBreakpointHit(
	uint8_t modId, int lineNo,
	const BreakpointConditionChecker *conditionChecker
) {
	(void) conditionChecker;
	if (-1 == lineNo) {
		return false;
	}

	bool result = false;
	const BreakpointLocation loc(modId, lineNo);
	BreaksTable::iterator shitIt = singleHits_.find(loc);
	if (singleHits_.end() != shitIt) {
		result = true;
		singleHits_.erase(shitIt);
	}
	if (!result) {
		BreaksTable::const_iterator locIt = breakpoints_.find(loc);
		if (breakpoints_.end() != locIt) {
			const BreakpointData &data = locIt->second;
			if (data.enabled) {
				result = true; // TODO check for hit count and condition
			}
		}
	}
	return result;
}

void BreakpointsTable::registerSourceFileName(
	const std::wstring &sourceFileName,
	uint8_t modId
) {
	sourceToIds_[sourceFileName] = modId;
	idsToSources_[modId] = sourceFileName;
}

const std::wstring &BreakpointsTable::registeredSourceFileName(
	uint8_t modId
) const {
	return idsToSources_.at(modId);
}

void BreakpointsTable::removeAllBreakpoints()
{
	singleHits_.clear();
	breakpoints_.clear();
}

void BreakpointsTable::insertOrChangeBreakpoint(
	bool enabled,
	const std::wstring &fileName,
	uint32_t lineNo, uint32_t ignoreCount,
	const BreakpointCondition &condition
) {
	SourcesToIdsTable::const_iterator fnIt = sourceToIds_.find(fileName);
	if (sourceToIds_.end() != fnIt) {
		uint8_t modId = fnIt->second;
		const BreakpointLocation loc(modId, lineNo);
		BreaksTable::iterator locIt = breakpoints_.find(loc);
		if (breakpoints_.end() != locIt) {
			BreakpointData &data = locIt->second;
			data.enabled = enabled;
			data.ignoreCount = ignoreCount;
			data.condition = condition;
		} else {
			BreakpointData data;
			data.enabled = enabled;
			data.ignoreCount = ignoreCount;
			data.condition = condition;
			breakpoints_[loc] = data;
		}
	}
}

void BreakpointsTable::insertSingleHitBreakpoint(
	const std::wstring &fileName,
	uint32_t lineNo
) {
	SourcesToIdsTable::const_iterator fnIt = sourceToIds_.find(fileName);
	if (sourceToIds_.end() != fnIt) {
		uint8_t modId = fnIt->second;
		const BreakpointLocation loc(modId, lineNo);
		BreakpointData data;
		data.condition = 0;
		data.ignoreCount = 0;
		data.hitCount = 0;
		data.enabled = true;
		singleHits_[loc] = data;
	}
}

void BreakpointsTable::removeBreakpoint(
	const std::wstring &fileName,
	uint32_t lineNo
) {
	SourcesToIdsTable::const_iterator fnIt = sourceToIds_.find(fileName);
	if (sourceToIds_.end() != fnIt) {
		const uint8_t modId = fnIt->second;
		const BreakpointLocation loc(modId, lineNo);
		BreaksTable::iterator locIt = breakpoints_.find(loc);
		if (breakpoints_.end() != locIt) {
			breakpoints_.erase(locIt);
		}
	}
}

} // namespace VM

