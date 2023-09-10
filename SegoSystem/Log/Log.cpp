#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Sego{
    
    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

    void Log::Log_Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("Sego");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("Edit");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
	

}