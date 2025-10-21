#pragma once

#include <string>
#include <iostream>


class Logger
{
public:
	static Logger Get()
	{
		static Logger instance;

		return instance;
	}

	void Info(const std::string& message)
	{
		std::cout << "[INFO] " << message << std::endl;
	}

	void Warn(const std::string& message)
	{
		std::cout << "[WARN] " << message << std::endl;
	}

	void Error(const std::string& message)
	{
		std::cerr << "[ERROR] " << message << std::endl;
	}
};
