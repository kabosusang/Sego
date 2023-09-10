#include "core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Sego{

    class Sego_API Log
    {
    public:
        static void Log_Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger;}
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger;}


    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;

    };

}

// Core log macros
#define SG_CORE_ERROR(...)		::Sego::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SG_CORE_WARN(...)		::Sego::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SG_CORE_INFO(...)		::Sego::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SG_CORE_TRACE(...)		::Sego::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SG_CORE_FATAL(...)		::Sego::Log::GetCoreLogger()->fatal(__VA_ARGS__)


//Client log macros

#define SG_ERROR(...)		::Sego::Log::GetClientLogger()->error(__VA_ARGS__)
#define SG_WARN(...)		::Sego::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SG_INFO(...)		::Sego::Log::GetClientLogger()->info(__VA_ARGS__)
#define SG_TRACE(...)		::Sego::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SG_FATAL(...)		::Sego::Log::GetClientLogger()->fatal(__VA_ARGS__)

