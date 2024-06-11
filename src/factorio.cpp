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
	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::Leave& e) {
	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::Chat& e) {
	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::Died& e) {
	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::Evolution& e) {
	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::ResearchStarted& e) {
	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::ResearchFinished& e) {
	return false;
}

bool Factorio::onEvent(const FactorioLog::Events::ResearchCancelled& e) {
	return false;
}

