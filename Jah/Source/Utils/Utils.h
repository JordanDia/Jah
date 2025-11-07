#pragma once

#include <string>

namespace Jah {

	class FileDialogs
	{
	public:

		// These return empty strings if cancellede
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};

}