#pragma once

#include <solanaceae/contact/fwd.hpp>
#include <solanaceae/message3/registry_message_model.hpp>

#include "./factorio_log_parser.hpp"

#include <vector>

// fwd
struct ConfigModelI;

class Factorio : public RegistryMessageModelEventI, public FactorioLogParserEventI {
	ConfigModelI& _conf;
	ContactStore4I& _cs;
	RegistryMessageModelI& _rmm;
	RegistryMessageModelI::SubscriptionReference _rmm_sr;
	FactorioLogParser& _flp;
	FactorioLogParser::SubscriptionReference _flp_sr;

	std::vector<ContactHandle4> _linked_contacts;

	void sendToLinked(const std::string& message);

	public:
		Factorio(ConfigModelI& conf, ContactStore4I& cs, RegistryMessageModelI& rmm, FactorioLogParser& flp);
		virtual ~Factorio(void);

	protected: // rmm
		bool onEvent(const Message::Events::MessageConstruct& e) override;

	protected: // flp
		bool onEvent(const FactorioLog::Events::Join&) override;
		bool onEvent(const FactorioLog::Events::Leave&) override;
		bool onEvent(const FactorioLog::Events::Chat&) override;
		bool onEvent(const FactorioLog::Events::Died&) override;
		bool onEvent(const FactorioLog::Events::Evolution&) override;
		bool onEvent(const FactorioLog::Events::ResearchStarted&) override;
		bool onEvent(const FactorioLog::Events::ResearchFinished&) override;
		bool onEvent(const FactorioLog::Events::ResearchCancelled&) override;
};

