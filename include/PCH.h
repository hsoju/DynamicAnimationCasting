#pragma once
#pragma warning(push)
#pragma warning(disable : 5105)
#pragma warning(disable : 4189)

#define NOMINMAX

#include <Windows.h>

#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
#include <REL/Relocation.h>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>

#pragma warning(pop)

// Compatible declarations with other sample projects.
#define DLLEXPORT __declspec(dllexport)

using namespace std::literals;
using namespace REL::literals;

namespace logger = SKSE::log;

namespace util {
    using SKSE::stl::report_and_fail;
}

#include "Plugin.h"
