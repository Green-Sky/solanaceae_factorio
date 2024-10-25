#include "./factorio.hpp"

#include <solanaceae/util/config_model.hpp>
#include <solanaceae/util/utils.hpp>

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

Factorio::Factorio(ConfigModelI& conf, Contact3Registry& cr, RegistryMessageModelI& rmm, FactorioLogParser& flp) :
	_cr(cr),
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
		Contact3Handle h;
		auto view = _cr.view<Contact::Components::ID>();
		for (const auto c : view) {
			if (view.get<Contact::Components::ID>(c).data == id_vec) {
				h = Contact3Handle{_cr, c};
				std::cout << "Factorio: found contact for link.\n";
				break;
			}
		}
		if (!static_cast<bool>(h)) {
			// not found, create thin contact with just id
			h = {_cr, _cr.create()};
			h.emplace<Contact::Components::ID>(id_vec);
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

	std::string message{e.player_name};
	message += " joined";

	sendToLinked(message);

	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::Leave& e) {
	std::cout << "Factorio: event leave " << e.player_name << " " << e.reason << "\n";

	std::string message{e.player_name};
	message += " left";

	sendToLinked(message);

	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::Chat& e) {
	std::cout << "Factorio: event chat " << e.player_name << ": " << e.message << "\n";

	std::string message{"<"};
	message += e.player_name;
	message += ">: ";
	message += e.message;

	sendToLinked(message);

	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::Died& e) {
	std::cout << "Factorio: event died " << e.player_name << ": " << e.reason << "\n";

	std::string message{e.player_name};
	message += " died from ";
	message += e.reason;

	sendToLinked(message);

	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::Evolution& e) {
	std::cout << "Factorio: event evolution " << e.evo << "\n";
	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::ResearchStarted& e) {
	std::cout << "Factorio: event research started " << e.name << "\n";
	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::ResearchFinished& e) {
	std::cout << "Factorio: event research finished " << e.name << "\n";

	std::string message{"Research "};
	message += e.name;
	message += " finished!";

	sendToLinked(message);

	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::ResearchCancelled& e) {
	std::cout << "Factorio: event research cancelled " << e.name << "\n";
	return false;
}

