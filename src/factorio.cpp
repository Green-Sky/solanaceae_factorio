#include "./factorio.hpp"
#include "factorio_log_parser.hpp"

#include <solanaceae/message3/components.hpp>
#include <solanaceae/contact/components.hpp>

Factorio::Factorio(Contact3Registry& cr, RegistryMessageModel& rmm, FactorioLogParser& flp) :
	_cr(cr),
	_rmm(rmm),
	_flp(flp)
{
	_rmm.subscribe(this, RegistryMessageModel_Event::message_construct);

	_flp.subscribe(this, FactorioLogParser_Event::join);
	_flp.subscribe(this, FactorioLogParser_Event::leave);
	_flp.subscribe(this, FactorioLogParser_Event::chat);
	_flp.subscribe(this, FactorioLogParser_Event::died);
	_flp.subscribe(this, FactorioLogParser_Event::evolution);
	_flp.subscribe(this, FactorioLogParser_Event::research_started);
	_flp.subscribe(this, FactorioLogParser_Event::research_finished);
	_flp.subscribe(this, FactorioLogParser_Event::research_cancelled);
}

Factorio::~Factorio(void) {
}

bool Factorio::onEvent(const Message::Events::MessageConstruct& e) {
	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::Join& e) {
	std::cout << "F: event join " << e.player_name << "\n";
	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::Leave& e) {
	std::cout << "F: event leave " << e.player_name << " " << e.reason << "\n";
	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::Chat& e) {
	std::cout << "F: event chat " << e.player_name << ": " << e.message << "\n";
	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::Died& e) {
	std::cout << "F: event died " << e.player_name << ": " << e.reason << "\n";
	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::Evolution& e) {
	std::cout << "F: event evolution " << e.evo << "\n";
	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::ResearchStarted& e) {
	std::cout << "F: event research started " << e.name << "\n";
	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::ResearchFinished& e) {
	std::cout << "F: event research finished " << e.name << "\n";
	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::ResearchCancelled& e) {
	std::cout << "F: event research cancelled " << e.name << "\n";
	return false;
}

