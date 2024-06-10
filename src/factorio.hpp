#pragma once

#include <solanaceae/message3/registry_message_model.hpp>

class Factorio : public RegistryMessageModelEventI {
	Contact3Registry& _cr;
	RegistryMessageModel& _rmm;

	public:
		Factorio(Contact3Registry& cr, RegistryMessageModel& rmm);
		virtual ~Factorio(void);

	protected: // rmm
		bool onEvent(const Message::Events::MessageConstruct& e) override;
};

