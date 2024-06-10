#include "log_parse.hpp"

#include <cassert>

int main(void) {

	{
		const auto parse_res = log_parse_line(" 442.539 Script @__Factorio-Event-Logger__/logger.lua:65: [RESEARCH CANCELLED] worker-robots-speed");
		assert(parse_res.has_value());
		assert(parse_res.value().event == "RESEARCH CANCELLED");
		assert(parse_res.value().info == "worker-robots-speed");
	}

	{
		const auto parse_res = log_parse_line(" 224.089 Info ServerMultiplayerManager.cpp:944: updateTick(38228812) received stateChanged peerID(2) oldState(ConnectedLoadingMap) newState(TryingToCatchUp)");
		assert(!parse_res.has_value());
	}

	return 0;
}

