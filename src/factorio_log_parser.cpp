#include "./factorio_log_parser.hpp"

#include "./log_parse.hpp"

FactorioLogParser::FactorioLogParser(void) :
	_fw("test.txt", [this](const auto& path, const auto event){ this->onFileEvent(path, event);})
{
}

FactorioLogParser::~FactorioLogParser(void) {
}

void FactorioLogParser::onFileEvent(const std::string& path, const filewatch::Event change_type) {
	std::cout << "file even " << filewatch::event_to_string(change_type) << " on '" << path << "'\n";

	// on create, close open log file and reopen and skip to end
	// on mod (?), read line, parse


	std::string line;

	const auto parse_res = log_parse_line(line);
	if (parse_res.has_value()) {
		dispatchRaw(parse_res.value().event, parse_res.value().params);
	}
}

void FactorioLogParser::dispatchRaw(std::string_view event, std::string_view params) {
	if (event == "JOIN") {
		throwJoin(params);
	} else if (event == "LEAVE") {
		throwLeave(params);
	} else if (event == "CHAT") {
		throwChat(params);
	} else if (event == "DIED") {
		throwDied(params);
	} else if (event == "EVOLUTION") {
		throwEvolution(params);
	} else if (event == "RESEARCH STARTED") {
		throwResearchStarted(params);
	} else if (event == "RESEARCH FINISHED") {
		throwResearchFinished(params);
	} else if (event == "RESEARCH CANCELLED") {
		throwResearchCancelled(params);
	} else {
		std::cerr << "FLP error: unknown event parsed: '" << event << "'\n";
	}
}

void FactorioLogParser::throwJoin(std::string_view params) {
	if (params == "<server>") {
		return;
	}

	dispatch(
		FactorioLogParser_Event::join,
		FactorioLog::Events::Join{
			params
		}
	);
}

void FactorioLogParser::throwLeave(std::string_view params) {
	const auto user = params.substr(0, params.find_first_of(' '));
	if (user.empty() || user.size() == params.size()) {
		std::cerr << "FLP error: invalid LEAVE params: '" << params << "'\n";
		return;
	}

	if (user == "<server>") {
		return;
	}

	auto reason = params;
	reason.remove_prefix(user.size()+1);

	dispatch(
		FactorioLogParser_Event::leave,
		FactorioLog::Events::Leave{
			user,
			reason
		}
	);
}

void FactorioLogParser::throwChat(std::string_view params) {
	const auto user = params.substr(0, params.find_first_of(':'));

	if (user.empty() || user.size() == params.size() || user.size() + 2 > params.size()) {
		std::cerr << "FLP error: invalid CHAT params: '" << params << "'\n";
		return;
	}

	if (user == "<server>") {
		return;
	}

	auto message = params;
	message.remove_prefix(user.size()+2); // ": "
	if (message.empty()) {
		std::cerr << "FLP error: empty message? '" << params << "'\n";
		return;
	}

	dispatch(
		FactorioLogParser_Event::chat,
		FactorioLog::Events::Chat{
			user,
			message
		}
	);
}

void FactorioLogParser::throwDied(std::string_view params) {
	const auto user = params.substr(0, params.find_first_of(' '));
	if (user.empty() || user.size() == params.size()) {
		std::cerr << "FLP error: invalid DIED params: '" << params << "'\n";
		return;
	}

	if (user == "<server>") {
		return;
	}

	auto reason = params;
	reason.remove_prefix(user.size()+1);

	dispatch(
		FactorioLogParser_Event::died,
		FactorioLog::Events::Died{
			user,
			reason
		}
	);
}

void FactorioLogParser::throwEvolution(std::string_view params) {
	if (params.empty()) {
		return; // ???
	}

	dispatch(
		FactorioLogParser_Event::evolution,
		FactorioLog::Events::Evolution{
			params
		}
	);
}

void FactorioLogParser::throwResearchStarted(std::string_view params) {
	dispatch(
		FactorioLogParser_Event::research_started,
		FactorioLog::Events::ResearchStarted{
			params
		}
	);
}

void FactorioLogParser::throwResearchFinished(std::string_view params) {
	dispatch(
		FactorioLogParser_Event::research_finished,
		FactorioLog::Events::ResearchFinished{
			params
		}
	);
}

void FactorioLogParser::throwResearchCancelled(std::string_view params) {
	dispatch(
		FactorioLogParser_Event::research_cancelled,
		FactorioLog::Events::ResearchCancelled{
			params
		}
	);
}

