#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "debug.hpp"

#if FMTK_DEBUG
#define LOG_LEVEL() spdlog::set_level(spdlog::level::trace)
#else
#define LOG_LEVEL() spdlog::set_level(spdlog::level::info);
#endif

#define LOG_INIT()                                                                        \
	do                                                                                    \
	{                                                                                     \
		static auto stdout_color_logger = spdlog::stdout_color_st("stdout_color_logger"); \
		spdlog::set_default_logger(stdout_color_logger);                                  \
		spdlog::set_pattern("[%^%L%$] %v");                                               \
		LOG_LEVEL();                                                                      \
	} while (0)

#define LOG(level, source, format, ...) do { spdlog::##level("[" + std::string(#source) + "] " ##format, __VA_ARGS__); } while(0)

#endif // !LOGGING_HPP
