cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

include(FetchContent)

if (NOT TARGET solanaceae_message3)
	FetchContent_Declare(solanaceae_message3
		GIT_REPOSITORY https://github.com/Green-Sky/solanaceae_message3.git
		GIT_TAG master
		EXCLUDE_FROM_ALL
	)
	FetchContent_MakeAvailable(solanaceae_message3)
endif()

if (NOT TARGET solanaceae_plugin)
	FetchContent_Declare(solanaceae_plugin
		GIT_REPOSITORY https://github.com/Green-Sky/solanaceae_plugin.git
		GIT_TAG master
		EXCLUDE_FROM_ALL
	)
	FetchContent_MakeAvailable(solanaceae_plugin)
endif()

if (NOT TARGET filewatch)
	FetchContent_Declare(filewatch
		GIT_REPOSITORY https://github.com/ThomasMonkman/filewatch.git
		GIT_TAG master
		EXCLUDE_FROM_ALL
	)

	# does not provide a proper cmake
	FetchContent_GetProperties(filewatch)
	if(NOT filewatch_POPULATED)
		FetchContent_Populate(filewatch)

		add_library(filewatch INTERFACE
			${filewatch_SOURCE_DIR}/FileWatch.hpp
		)
		target_include_directories(filewatch INTERFACE ${filewatch_SOURCE_DIR})
		target_compile_features(filewatch INTERFACE cxx_std_11)
	endif()
endif()
