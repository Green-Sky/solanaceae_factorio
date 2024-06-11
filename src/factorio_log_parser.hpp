#pragma once

#include <solanaceae/util/event_provider.hpp>

#include <FileWatch.hpp>

#include <string>

namespace FactorioLog::Events {

	// TODO: string views?
	struct Join {
		std::string player_name;
	};

	struct Leave {
		std::string player_name;
	};

	struct Chat {
		std::string player_name;
		std::string message;
	};

	struct Died {
		std::string player_name;
	};

	struct Evolution {
		// ?
		std::string evo;
	};

	struct ResearchStarted {
		std::string name;
	};

	struct ResearchFinished {
		std::string name;
	};

	struct ResearchCancelled {
		std::string name;
	};

} // FactorioLog::Events

enum class FactorioLogParser_Event : uint32_t {
	join,
	leave,
	chat,
	died,
	evolution,
	research_started,
	research_finished,
	research_cancelled,

	MAX
};

struct FactorioLogParserEventI {
	using enumType = FactorioLogParser_Event;

	virtual ~FactorioLogParserEventI(void) {}

	virtual bool onEvent(const FactorioLog::Events::Join&) { return false; }
	virtual bool onEvent(const FactorioLog::Events::Leave&) { return false; }
	virtual bool onEvent(const FactorioLog::Events::Chat&) { return false; }
	virtual bool onEvent(const FactorioLog::Events::Died&) { return false; }
	virtual bool onEvent(const FactorioLog::Events::Evolution&) { return false; }
	virtual bool onEvent(const FactorioLog::Events::ResearchStarted&) { return false; }
	virtual bool onEvent(const FactorioLog::Events::ResearchFinished&) { return false; }
	virtual bool onEvent(const FactorioLog::Events::ResearchCancelled&) { return false; }
};
using FactorioLogParserEventProviderI = EventProviderI<FactorioLogParserEventI>;

class FactorioLogParser : public FactorioLogParserEventProviderI {
	filewatch::FileWatch<std::string> _fw;

	public:
		FactorioLogParser(void);
		virtual ~FactorioLogParser(void);

	protected:
		void onFileEvent(const std::string& path, const filewatch::Event change_type);
};

