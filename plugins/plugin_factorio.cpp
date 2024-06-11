#include <solanaceae/plugin/solana_plugin_v1.h>

#include <entt/entt.hpp>
#include <entt/fwd.hpp>

#include "factorio_log_parser.hpp"
#include "factorio.hpp"

#include <iostream>

static std::unique_ptr<FactorioLogParser> g_flp = nullptr;
static std::unique_ptr<Factorio> g_f = nullptr;

constexpr const char* plugin_name = "Factorio";

extern "C" {

SOLANA_PLUGIN_EXPORT const char* solana_plugin_get_name(void) {
	return plugin_name;
}

SOLANA_PLUGIN_EXPORT uint32_t solana_plugin_get_version(void) {
	return SOLANA_PLUGIN_VERSION;
}

SOLANA_PLUGIN_EXPORT uint32_t solana_plugin_start(struct SolanaAPI* solana_api) {
	std::cout << "PLUGIN " << plugin_name << " START()\n";

	if (solana_api == nullptr) {
		return 1;
	}

	try {
		auto* cr = PLUG_RESOLVE_INSTANCE_VERSIONED(Contact3Registry, "1");
		auto* rmm = PLUG_RESOLVE_INSTANCE(RegistryMessageModel);

		// static store, could be anywhere tho
		// construct with fetched dependencies
		g_flp = std::make_unique<FactorioLogParser>();
		g_f = std::make_unique<Factorio>(*cr, *rmm, *g_flp);

		// register types
		PLUG_PROVIDE_INSTANCE(FactorioLogParser, plugin_name, g_flp.get());
		PLUG_PROVIDE_INSTANCE(Factorio, plugin_name, g_f.get());
	} catch (const ResolveException& e) {
		std::cerr << "PLUGIN " << plugin_name << " " << e.what << "\n";
		return 2;
	}

	return 0;
}

SOLANA_PLUGIN_EXPORT void solana_plugin_stop(void) {
	std::cout << "PLUGIN " << plugin_name << " STOP()\n";

	g_f.reset();
	g_flp.reset();
}

SOLANA_PLUGIN_EXPORT float solana_plugin_tick(float) {
	//return g_rpbot->tick(delta);
	return 1000.f;
}

} // extern C

