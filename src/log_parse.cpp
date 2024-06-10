#include "./log_parse.hpp"

#include <regex>

std::optional<LPLRes> log_parse_line(std::string_view line) {
	static const std::regex mod_match{".*Factorio-Event-Logger+.*\\[([A-Z ]+)\\] (.+)$"};

	std::cmatch matches;
	if (!std::regex_match(line.cbegin(), line.cend(), matches, mod_match)) {
		return std::nullopt;
	}

	if (matches.empty() || matches.size() != 3) {
		return std::nullopt;
	}

	return LPLRes{
		std::string_view{matches[1].first, static_cast<size_t>(matches[1].second - matches[1].first)},
		std::string_view{matches[2].first, static_cast<size_t>(matches[2].second - matches[2].first)},
	};
}
