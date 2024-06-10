cmake_minimum_required(VERSION 3.9...3.24 FATAL_ERROR)

project(solanaceae)

add_executable(solanaceae_factorio_test1
	./test1.cpp
)

target_compile_features(solanaceae_factorio_test1 PUBLIC cxx_std_17)
target_link_libraries(solanaceae_factorio_test1 PUBLIC
	solanaceae_factorio
)

add_test(NAME solanaceae_factorio_test1 COMMAND solanaceae_factorio_test1)

########################################
