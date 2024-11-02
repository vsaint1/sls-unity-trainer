#pragma once

#include "mem/memory.h"
#include "utils.h"
#include <MinHook.h>
#include <cstdint>
#include "globals.h"

#ifdef _DEBUG
#include <spdlog/spdlog.h>
#else 
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_OFF
#include <spdlog/spdlog.h>
#endif