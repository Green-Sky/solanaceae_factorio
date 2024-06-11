#include "./factorio_log_parser.hpp"

#include <solanaceae/util/config_model.hpp>

#include "./log_parse.hpp"

FactorioLogParser::FactorioLogParser(ConfigModelI& conf) :
	_log_file_path(conf.get_string("FactorioLogParser", "log_file_path").value_or("factorio-current.log")),
	_fw(_log_file_path, [this](const auto& path, const auto event){ this->onFileEvent(path, event);})
{
	resetLogFile();
}

FactorioLogParser::~FactorioLogParser(void) {
}

float FactorioLogParser::tick(float) {
	std::lock_guard lg{_event_queue_mutex};
	while (!_event_queue.empty()) {
		dispatchRaw(_event_queue.front().event, _event_queue.front().params);
		_event_queue.pop();
	}

	return 10.f;
}

void FactorioLogParser::onFileEvent(const std::string& path, const filewatch::Event change_type) {
	// compare path?

	if (change_type == filewatch::Event::added) {
		// on create, close open log file and reopen and skip to end
		resetLogFile();
	} else if (change_type == filewatch::Event::modified) {
		// on mod, read lines and parse
		if (!_log_file.is_open()) {
			std::cerr << "FLP: modified file not open!\n";
			//resetLogFile();
		} else {
			std::string line;
			while (std::getline(_log_file, line).good()) {
				if (line.empty()) {
					std::cerr << "FLP error: getline empty??\n";
					continue;
				}

				const auto parse_res = log_parse_line(line);
				if (parse_res.has_value()) {
					queueRaw(parse_res.value().event, parse_res.value().params);
				}
			}
			_log_file.clear(); // reset eof and fail bits
		}
	}
}

void FactorioLogParser::resetLogFile(void) {
	std::cerr << "FLP: resetting log file\n";
	if (_log_file.is_open()) {
		_log_file.close();
		_log_file.clear();
	}
	_log_file.open(_log_file_path, std::ios::in | std::ios::binary | std::ios::ate);
	if (!_log_file.is_open()) {
		std::cerr << "FLP error: failed opening file\n";
	}
}

void FactorioLogParser::queueRaw(std::string_view event, std::string_view params) {
	std::lock_guard lg{_event_queue_mutex};
	_event_queue.push(EventEntry{static_cast<std::string>(event), static_cast<std::string>(params)});
	//std::cerr << "enqued '" << event << "': '" << params << "'\n";
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

