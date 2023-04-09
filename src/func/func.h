#pragma once

#include "../util/offsets.h"
#include "../core/interfaces.h"
#include "vars.h"


class CUserCmd;
namespace hacks
{
	void BunnyHop(CUserCmd* cmd) noexcept;
	int TriggerBot(CUserCmd* cmd) noexcept;
}