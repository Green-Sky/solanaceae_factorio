cmake_minimum_required(VERSION 3.9...3.24 FATAL_ERROR)

project(solanaceae)

add_library(solanaceae_factorio
	./log_parse.hpp
	./log_parse.cpp
	./factorio.hpp
	./factorio.cpp
)

target_include_directories(solanaceae_factorio PUBLIC .)
target_compile_features(solanaceae_factorio PUBLIC cxx_std_17)
target_link_libraries(solanaceae_factorio PUBLIC
	solanaceae_message3
	filewatch
)

########################################

