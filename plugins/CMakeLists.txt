cmake_minimum_required(VERSION 3.9...3.24 FATAL_ERROR)

add_library(plugin_factorio SHARED
	./plugin_factorio.cpp
)

target_link_libraries(plugin_factorio PUBLIC
	solanaceae_plugin
	solanaceae_factorio
)

set_target_properties(plugin_factorio PROPERTIES
	C_VISIBILITY_PRESET hidden
)
# probably not enough
target_compile_definitions(plugin_factorio PUBLIC ENTT_API_IMPORT)

