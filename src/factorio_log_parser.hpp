#pragma once

#include <solanaceae/util/event_provider.hpp>

#include <FileWatch.hpp>

#include <string>
#include <string_view>
#include <mutex>
#include <queue>
#include <fstream>

// fwd
struct ConfigModelI;

namespace FactorioLog::Events {

	struct Join {
		std::string_view player_name;
	};

	struct Leave {
		std::string_view player_name;
		std::string_view reason;
	};

	struct Chat {
		std::string_view player_name;
		std::string_view message;
	};

	struct Died {
		std::string_view player_name;
		std::string_view reason;
	};

	struct Evolution {
		// ?
		std::string_view evo;
	};

	struct ResearchStarted {
		std::string_view name;
	};

	struct ResearchFinished {
		std::string_view name;
	};

	struct ResearchCancelled {
		std::string_view name;
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
	std::string _log_file_path;
	filewatch::FileWatch<std::string> _fw;
	std::ifstream _log_file;

	struct EventEntry {
		std::string event;
		std::string params;
	};
	std::queue<EventEntry> _event_queue;
	std::mutex _event_queue_mutex;

	public:
		FactorioLogParser(ConfigModelI& conf);
		virtual ~FactorioLogParser(void);

		float tick(float delta);

	protected:
		void onFileEvent(const std::string& path, const filewatch::Event change_type);
		void resetLogFile(void);

	protected:
		void queueRaw(std::string_view event, std::string_view params);
		void dispatchRaw(std::string_view event, std::string_view params);

		void throwJoin(std::string_view params);
		void throwLeave(std::string_view params);
		void throwChat(std::string_view params);
		void throwDied(std::string_view params);
		void throwEvolution(std::string_view params);
		void throwResearchStarted(std::string_view params);
		void throwResearchFinished(std::string_view params);
		void throwResearchCancelled(std::string_view params);
};

