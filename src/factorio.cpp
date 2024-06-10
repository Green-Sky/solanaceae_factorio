#include "./factorio.hpp"

#include <solanaceae/message3/components.hpp>
#include <solanaceae/contact/components.hpp>

#include "./log_parse.hpp"

Factorio::Factorio(Contact3Registry& cr, RegistryMessageModel& rmm) :
	_cr(cr),
	_rmm(rmm),
	_fw("test.txt", [this](const auto& path, const auto event){ this->onFileEvent(path, event);})
{


	_rmm.subscribe(this, RegistryMessageModel_Event::message_construct);
}

Factorio::~Factorio(void) {
}

bool Factorio::onEvent(const Message::Events::MessageConstruct& e) {
	return false;
}

void onFileEvent(const std::string& path, const filewatch::Event change_type) {
	std::cout << "file even " << filewatch::event_to_string(change_type) << " on '" << path << "'\n";
}

