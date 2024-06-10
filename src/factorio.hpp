#pragma once

#include <solanaceae/message3/registry_message_model.hpp>

#include <FileWatch.hpp>

#include <string>

class Factorio : public RegistryMessageModelEventI {
	Contact3Registry& _cr;
	RegistryMessageModel& _rmm;

	filewatch::FileWatch<std::string> _fw;

	public:
		Factorio(Contact3Registry& cr, RegistryMessageModel& rmm);
		virtual ~Factorio(void);

	protected: // rmm
		bool onEvent(const Message::Events::MessageConstruct& e) override;

	protected:
		void onFileEvent(const std::string& path, const filewatch::Event change_type);
};

