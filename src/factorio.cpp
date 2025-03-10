#include "./factorio.hpp"

#include <solanaceae/util/config_model.hpp>
#include <solanaceae/util/utils.hpp>

#include <solanaceae/contact/contact_store_i.hpp>

#include <solanaceae/message3/components.hpp>
#include <solanaceae/contact/components.hpp>

void Factorio::sendToLinked(const std::string& message) {
	for (const auto& h : _linked_contacts) {
		if (!static_cast<bool>(h)) {
			continue;
		}

		_rmm.sendText(h, message);
	}
}

Factorio::Factorio(ConfigModelI& conf, ContactStore4I& cs, RegistryMessageModelI& rmm, FactorioLogParser& flp) :
	_conf(conf),
	_cs(cs),
	_rmm(rmm),
	_rmm_sr(_rmm.newSubRef(this)),
	_flp(flp),
	_flp_sr(_flp.newSubRef(this))
{
	// config
	for (const auto&& [contact_id, enabled] : conf.entries_bool("Factorio", "link_to_contact")) {
		//std::cout << "config id:" << contact_id << " e:" << enabled << "\n";

		const auto id_vec = hex2bin(contact_id);

		// search
		ContactHandle4 h = _cs.getOneContactByID(ByteSpan{id_vec});
		if (!static_cast<bool>(h)) {
			// not found, create thin contact with just id
			h = _cs.contactHandle(_cs.registry().create());
			h.emplace<Contact::Components::ID>(id_vec);
			_cs.throwEventConstruct(h);
			std::cout << "Factorio: contact not found, created thin contact from ID. (" << contact_id << ")\n";
		}
		_linked_contacts.push_back(h);
	}

	_rmm_sr.subscribe(RegistryMessageModel_Event::message_construct);

	_flp_sr
		.subscribe(FactorioLogParser_Event::join)
		.subscribe(FactorioLogParser_Event::leave)
		.subscribe(FactorioLogParser_Event::chat)
		.subscribe(FactorioLogParser_Event::died)
		.subscribe(FactorioLogParser_Event::evolution)
		.subscribe(FactorioLogParser_Event::research_started)
		.subscribe(FactorioLogParser_Event::research_finished)
		.subscribe(FactorioLogParser_Event::research_cancelled)
	;
}

Factorio::~Factorio(void) {
}

bool Factorio::onEvent(const Message::Events::MessageConstruct& e) {
	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::Join& e) {
	std::cout << "Factorio: event join " << e.player_name << "\n";

	if (!_conf.get_bool("Factorio", "forward", "join").value_or(true)) {
		return false;
	}

	std::string message{e.player_name};
	message += " joined";

	sendToLinked(message);

	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::Leave& e) {
	std::cout << "Factorio: event leave " << e.player_name << " " << e.reason << "\n";

	if (!_conf.get_bool("Factorio", "forward", "leave").value_or(true)) {
		return false;
	}

	std::string message{e.player_name};
	message += " left";

	sendToLinked(message);

	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::Chat& e) {
	std::cout << "Factorio: event chat " << e.player_name << ": " << e.message << "\n";

	if (!_conf.get_bool("Factorio", "forward", "chat").value_or(true)) {
		return false;
	}

	// ignore pings
	constexpr std::string_view ping_prefix{"[gps="};
	if (e.message.size() > ping_prefix.size() && e.message.substr(0, ping_prefix.size()) == ping_prefix) {
		return false;
	}

	std::string message{"<"};
	message += e.player_name;
	message += ">: ";
	message += e.message;

	sendToLinked(message);

	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::Died& e) {
	std::cout << "Factorio: event died " << e.player_name << ": " << e.reason << "\n";

	if (!_conf.get_bool("Factorio", "forward", "died").value_or(true)) {
		return false;
	}

	std::string message{e.player_name};
	message += " died from ";
	message += e.reason;

	sendToLinked(message);

	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::Evolution& e) {
	std::cout << "Factorio: event evolution " << e.evo << "\n";

	//if (!_conf.get_bool("Factorio", "forward", "evolution").value_or(true)) {
	//    return false;
	//}

	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::ResearchStarted& e) {
	std::cout << "Factorio: event research started " << e.name << "\n";

	//if (!_conf.get_bool("Factorio", "forward", "research_started").value_or(true)) {
	//    return false;
	//}

	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::ResearchFinished& e) {
	std::cout << "Factorio: event research finished " << e.name << "\n";

	if (!_conf.get_bool("Factorio", "forward", "research_finished").value_or(true)) {
		return false;
	}

	std::string message{"Research "};
	message += e.name;
	message += " finished!";

	sendToLinked(message);

	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::ResearchCancelled& e) {
	std::cout << "Factorio: event research cancelled " << e.name << "\n";

	//if (!_conf.get_bool("Factorio", "forward", "research_cancelled").value_or(true)) {
	//    return false;
	//}

	return false;
}

