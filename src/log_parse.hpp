#pragma once

#include <string_view>
#include <optional>

struct LPLRes {
	std::string_view event;
	std::string_view info;
};

std::optional<LPLRes> log_parse_line(std::string_view line);
