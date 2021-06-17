#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "debug.hpp"

#if FMTK_DEBUG
#define LOG_LEVEL() spdlog::set_level(spdlog::level::trace)
#else
#define LOG_LEVEL() spdlog::set_level(spdlog::level::debug);
#endif

#define LOG_INIT()                                                                                \
	do                                                                                            \
	{                                                                                             \
		std::vector<spdlog::sink_ptr> sinks;                                                      \
		sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());                 \
		sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("fmtk.log", true));   \
		auto logger = std::make_shared<spdlog::logger>("logger", begin(sinks), end(sinks));       \
		spdlog::set_default_logger(logger);                                                       \
		spdlog::set_pattern("[%^%L%$] %v");                                                       \
		LOG_LEVEL();                                                                              \
	} while (0)

#define LOG(level, source, format, ...) do { spdlog::level("[" #source "] " format, __VA_ARGS__); } while(0)
#define LOGW(level, source, format, ...) do { spdlog::level(L"[" L#source L"] " L##format, __VA_ARGS__); } while(0)

#endif // !LOGGING_HPP
