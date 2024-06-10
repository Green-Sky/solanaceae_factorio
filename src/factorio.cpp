#include "./factorio.hpp"

#include <solanaceae/message3/components.hpp>
#include <solanaceae/contact/components.hpp>

Factorio::Factorio(Contact3Registry& cr, RegistryMessageModel& rmm) : _cr(cr), _rmm(rmm) {
	_rmm.subscribe(this, RegistryMessageModel_Event::message_construct);
}

Factorio::~Factorio(void) {
}

bool Factorio::onEvent(const Message::Events::MessageConstruct& e) {
	return false;
}

