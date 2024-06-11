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


	dispatch(
		FactorioLogParser_Event::join,
		FactorioLog::Events::Join{
			"Player"
		}
	);
}

