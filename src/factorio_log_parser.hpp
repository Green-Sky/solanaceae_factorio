#pragma once

#include <FileWatch.hpp>

#include <string>

class FactorioLogParser {
	filewatch::FileWatch<std::string> _fw;

	public:
		FactorioLogParser(void);
		virtual ~FactorioLogParser(void);

	protected:
		void onFileEvent(const std::string& path, const filewatch::Event change_type);
};

